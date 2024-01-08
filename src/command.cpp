#include "Plugin.h"
#include "mc/server/commands/CommandParameterOption.h"
#include <ll/api/command/DynamicCommand.h>
#include <ll/api/plugin/NativePlugin.h>
#include <ll/api/service/Bedrock.h>
#include <mc/entity/utilities/ActorType.h>
#include <mc/server/commands/CommandOrigin.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/server/commands/CommandPermissionLevel.h>
#include <mc/world/Container.h>
#include <mc/world/actor/player/Player.h>
#include <mc/world/item/registry/ItemStack.h>


namespace plugin {

void Plugin::enableCommand() {
    auto& logger = mSelf.getLogger();

    mCommands["suicide"] = [&logger]() -> void {
        auto commandRegistry = ll::service::getCommandRegistry();
        if (!commandRegistry) {
            throw std::runtime_error("failed to get command registry");
        }

        auto command =
            DynamicCommand::createCommand(commandRegistry, "suicide", "Commits suicide.", CommandPermissionLevel::Any);
        command->addOverload();
        command->setCallback(
            [&logger](
                DynamicCommand const&,
                CommandOrigin const&                                     origin,
                CommandOutput&                                           output,
                std::unordered_map<std::string, DynamicCommand::Result>& result
            ) -> void {
                auto* entity = origin.getEntity();
                if (!entity || !entity->isType(ActorType::Player)) {
                    output.error("Only players can commit suicide");
                    return;
                }

                auto* player = static_cast<Player*>(entity);
                player->kill();

                logger.info("{} killed themselves", player->getRealName());
            }
        );

        DynamicCommand::setup(commandRegistry, std::move(command));
    };

    mCommands["test"] = [&logger]() -> void {
        auto commandRegistry = ll::service::getCommandRegistry();
        if (!commandRegistry) {
            throw std::runtime_error("failed to get command registry");
        }

        auto command =
            DynamicCommand::createCommand(commandRegistry, "test", "7685 test command", CommandPermissionLevel::Any);
        auto& option = command->setEnum("Command options", {"apple", "suicide"});
        command->mandatory(
            "option",
            DynamicCommand::ParameterType::Enum,
            option,
            CommandParameterOption::EnumAutocompleteExpansion
        );
        // command->mandatory("player", DynamicCommand::ParameterType::Player);
        command->addOverload({option});
        command->setCallback(
            [&logger](
                DynamicCommand const&,
                CommandOrigin const&                                     origin,
                CommandOutput&                                           output,
                std::unordered_map<std::string, DynamicCommand::Result>& result
            ) -> void {
                auto& value = result.at("option").getEnumValue();
                switch (value[0]) {
                case 's': {
                    auto* entity = origin.getEntity();
                    if (!entity || !entity->isType(ActorType::Player)) {
                        output.error("Only players can commit suicide");
                        return;
                    }

                    auto* player = static_cast<Player*>(entity);
                    player->kill();

                    logger.info("{} killed themselves", player->getRealName());
                    break;
                }
                case 'a': {
                    auto* entity = origin.getEntity();
                    if (!entity || !entity->isType(ActorType::Player)) {
                        output.error("Only players can use test apple");
                        return;
                    }

                    auto*     player = static_cast<Player*>(entity);
                    auto&     inv    = player->getInventory();
                    ItemStack itemStack("apple", 1);
                    inv.addItem(itemStack);
                    player->sendInventory(true);
                    break;
                }
                default:
                    break;
                }
            }
        );

        DynamicCommand::setup(commandRegistry, std::move(command));
    };

    for (auto& command : mCommands) {
        command.second();
        logger.info("Registered command {}", command.first);
    }
}

void Plugin::disableCommand() {
    auto& logger = mSelf.getLogger();

    auto commandRegistry = ll::service::getCommandRegistry();
    if (!commandRegistry) {
        throw std::runtime_error("failed to get command registry");
    }
    
    for (auto& command : mCommands) {
        commandRegistry->unregisterCommand(command.first);
        logger.info("Unregistered command {}.", command.first);
    }
}

} // namespace plugin