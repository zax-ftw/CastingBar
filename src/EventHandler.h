#pragma once

#include "Utils/Singleton.h"


using EventResult = RE::BSEventNotifyControl;

namespace RE
{
	struct MenuModeChangeEvent2
	{
		enum Mode : std::uint8_t
		{
			kHidden = 0,
			kDisplayed = 1
		};

		BSFixedString menu;
		Mode mode;
	};

}

class MenuModeEventHandler : public ISingleton<MenuModeEventHandler>, 
	public RE::BSTEventSink<RE::MenuModeChangeEvent>
{
public:
	using MenuModeChangeEventSource = RE::BSTEventSource<RE::MenuModeChangeEvent>;

	static void Register();

	EventResult ProcessEvent(const RE::MenuModeChangeEvent* a_event, MenuModeChangeEventSource* a_source) override;

private:
	//using MenuMode = RE::MenuModeChangeEvent::Mode;
	using MenuMode = RE::MenuModeChangeEvent2::Mode;
};
