#include "Plugin.h"
#include <ll/api/event/EventBus.h>
#include <ll/api/event/player/PlayerJoinEvent.h>
#include <ll/api/event/player/PlayerLeaveEvent.h>
#include <ll/api/plugin/NativePlugin.h>
#include <mc/world/actor/player/Player.h>


namespace plugin {

Plugin::Plugin(ll::plugin::NativePlugin& self) : mSelf(self) { mSelf.getLogger().info("Loading tutorial-plugin"); }

bool Plugin::enable() {
    auto& logger = mSelf.getLogger();
    logger.info("Enabling tutorial-plugin");

    enableCommand();

    auto& eventBus = ll::event::EventBus::getInstance();
    mPlayerJoinEventListener =
        eventBus.emplaceListener<ll::event::PlayerJoinEvent>([&logger](ll::event::PlayerJoinEvent const& ev) {
            const auto& sp  = ev.self();
            const auto& pos = sp.getPosition();
            logger.info("Player {} joined game at ({}, {}, {})", sp.getRealName(), pos.x, pos.y, pos.z);
        });
    mPlayerLeaveEventListener =
        eventBus.emplaceListener<ll::event::PlayerLeaveEvent>([&logger](ll::event::PlayerLeaveEvent const& ev) {
            const auto& sp  = ev.self();
            const auto& pos = sp.getPosition();
            logger.info("Player {} left game at ({}, {}, {})", sp.getRealName(), pos.x, pos.y, pos.z);
        });

    return true;
}

bool Plugin::disable() {
    mSelf.getLogger().info("Disabling tutorial-plugin");

    auto& eventBus = ll::event::EventBus::getInstance();
    eventBus.removeListener(mPlayerJoinEventListener);
    eventBus.removeListener(mPlayerLeaveEventListener);

    disableCommand();

    return true;
}

} // namespace plugin
