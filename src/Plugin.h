#pragma once

#include <functional>
#include <ll/api/event/ListenerBase.h>
#include <ll/api/plugin/NativePlugin.h>
#include <ll/api/command/DynamicCommand.h>
#include <string>
#include <unordered_map>


namespace plugin {

class Plugin {
public:
    explicit Plugin(ll::plugin::NativePlugin& self);

    Plugin(Plugin&&)                 = delete;
    Plugin(const Plugin&)            = delete;
    Plugin& operator=(Plugin&&)      = delete;
    Plugin& operator=(const Plugin&) = delete;

    ~Plugin() = default;

    /// @return True if the plugin is enabled successfully.
    bool enable();

    /// @return True if the plugin is disabled successfully.
    bool disable();

    void enableCommand();

    void disableCommand();

private:
    ll::plugin::NativePlugin& mSelf;
    std::unordered_map<std::string, std::function<void()>> mCommands;
    ll::event::ListenerPtr    mPlayerJoinEventListener;
    ll::event::ListenerPtr    mPlayerLeaveEventListener;
};

} // namespace plugin
