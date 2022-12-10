#include "Events.h"
#include "CastingBar.h"
#include "Settings.h"

using namespace RE;

MenuOpenCloseEventHandler* MenuOpenCloseEventHandler::GetSingleton()
{
	static MenuOpenCloseEventHandler singleton;
	return std::addressof(singleton);
}

void MenuOpenCloseEventHandler::Register()
{
	auto ui = UI::GetSingleton();
	ui->AddEventSink(GetSingleton());
}

BSEventNotifyControl MenuOpenCloseEventHandler::ProcessEvent(const MenuOpenCloseEvent* a_event,BSTEventSource<MenuOpenCloseEvent>*)
{
	if (a_event) {
		if (a_event->menuName == HUDMenu::MENU_NAME) {
			if (a_event->opening) {
				CastingBar::Show();
			} else {
				CastingBar::Hide();
			}
		} else if (a_event->menuName == RaceSexMenu::MENU_NAME && !a_event->opening) {
			CastingBar::Show();
		} else if (a_event->menuName == LoadingMenu::MENU_NAME && !a_event->opening) {
			CastingBar::Show();
		} else if (a_event->menuName == ContainerMenu::MENU_NAME && a_event->opening) {
			CastingBar::Hide();
		}
        if (a_event->menuName == JournalMenu::MENU_NAME && !a_event->opening) {
			Settings::GetSingleton().Load();
		}
	}

	auto controlMap = ControlMap::GetSingleton();
	if (controlMap) {
		auto& priorityStack = controlMap->contextPriorityStack;
		if (priorityStack.empty() ||
			(priorityStack.back() != UserEvents::INPUT_CONTEXT_ID::kGameplay &&
				priorityStack.back() != UserEvents::INPUT_CONTEXT_ID::kFavorites &&
				priorityStack.back() != UserEvents::INPUT_CONTEXT_ID::kConsole)) {
					CastingBar::ToggleVisibility(false);
		} else {
			CastingBar::ToggleVisibility(true);
		}
	}
	return BSEventNotifyControl::kContinue;
}
