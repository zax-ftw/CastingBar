#include "EventHandler.h"

#include "CastingBar.h"
#include "Settings.h"

using namespace RE;

void MenuModeEventHandler::Register()
{
	if (auto ui = UI::GetSingleton()) {
		ui->AddEventSink<MenuModeChangeEvent>(GetSingleton());
	}
}

EventResult MenuModeEventHandler::ProcessEvent(const RE::MenuModeChangeEvent* a_event, MenuModeChangeEventSource*)
{
	auto _event = reinterpret_cast<const MenuModeChangeEvent2*>(a_event);

	logger::trace("{}: {}", _event->menu, _event->mode);

	if (_event->menu == LoadingMenu::MENU_NAME) {
		if (_event->mode == MenuMode::kHidden) {
			CastingBar::Show();
		}
	}

	return BSEventNotifyControl::kContinue;
}
