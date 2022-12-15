#include "Events.h"

#include "CastingBar.h"
#include "Settings.h"

using namespace RE;

void MenuModeEventHandler::Register() 
{
    if (auto ui = UI::GetSingleton()) {
        ui->AddEventSink<MenuModeChangeEvent>(&GetSingleton());
	}
}

BSEventNotifyControl MenuModeEventHandler::ProcessEvent(const RE::MenuModeChangeEvent* a_event, BSTEventSource<RE::MenuModeChangeEvent>*) 
{
	using Mode = RE::MenuModeChangeEvent::Mode;

	logger::trace("{}: {}", a_event->menu, a_event->mode);

    if (a_event->menu == LoadingMenu::MENU_NAME) {
		if (a_event->mode == Mode::kHidden) {
            CastingBar::Show();
		}
    }

    return BSEventNotifyControl::kContinue;
}
