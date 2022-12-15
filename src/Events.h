#pragma once

namespace RE
{
    struct MenuModeChangeEvent 
	{
        enum Mode : std::uint8_t 
		{
            kHidden = 0,
            kDisplayed = 1
        };

        BSFixedString menu;
        Mode mode;
    };

}  // namespace RE

class MenuModeEventHandler : public RE::BSTEventSink<RE::MenuModeChangeEvent>
{
public:
    using MenuModeChangeEventSource = RE::BSTEventSource<RE::MenuModeChangeEvent>;

    static MenuModeEventHandler& GetSingleton()
	{
        static MenuModeEventHandler singleton;
        return singleton;
    }

    static void Register();

    RE::BSEventNotifyControl ProcessEvent(const RE::MenuModeChangeEvent* a_event, RE::BSTEventSource<RE::MenuModeChangeEvent>* a_source) override;

private:
    MenuModeEventHandler() noexcept = default;
    MenuModeEventHandler(const MenuModeEventHandler&) = delete;
    MenuModeEventHandler(MenuModeEventHandler&&) = delete;
    MenuModeEventHandler& operator=(const MenuModeEventHandler&) = delete;
    MenuModeEventHandler& operator=(MenuModeEventHandler&&) = delete;
};
