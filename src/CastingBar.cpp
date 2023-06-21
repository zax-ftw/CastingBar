#include "CastingBar.h"
#include "Settings.h"

using namespace RE;


CastingBar::CastingBar()
{
	logger::trace("CastingBar::Ctor");

	depthPriority = 0;

	menuFlags.set(UI_MENU_FLAGS::kAlwaysOpen);
	menuFlags.set(UI_MENU_FLAGS::kRequiresUpdate);
	menuFlags.set(UI_MENU_FLAGS::kAllowSaving);

	if (uiMovie) {
		uiMovie->SetMouseCursorCount(0); // disable input
		uiMovie->SetVisible(true);
	}

	if (auto scaleform = BSScaleformManager::GetSingleton()) {
		scaleform->LoadMovie(this, this->uiMovie, MENU_PATH);
	}

	auto player = PlayerCharacter::GetSingleton();
	controller.reset(new StateController(player));
}

CastingBar::~CastingBar() 
{
    logger::trace("CastingBar::Dtor");
}

void CastingBar::Register()
{
    if (auto ui = UI::GetSingleton()) {
        ui->Register(MENU_NAME, Creator);
    }
}

void CastingBar::Show() 
{
    if (auto queue = UIMessageQueue::GetSingleton()) {
        queue->AddMessage(MENU_NAME, UI_MESSAGE_TYPE::kShow, nullptr);
    }
}

void CastingBar::Hide() 
{
    if (auto queue = UIMessageQueue::GetSingleton()) {
        queue->AddMessage(MENU_NAME, UI_MESSAGE_TYPE::kHide, nullptr);
    }
}

MessageResult CastingBar::ProcessMessage(UIMessage& message)
{
	if (message.menu == MENU_NAME) {

		switch (message.type.get()) {
		case MessageType::kShow:
		case MessageType::kReshow:
			OnShow();
			break;
		case MessageType::kHide:
		case MessageType::kForceHide:
			OnHide();
			break;
		}
    }

    return IMenu::ProcessMessage(message);
}

void CastingBar::AdvanceMovie(float a_interval, std::uint32_t a_currentTime) 
{
    IMenu::AdvanceMovie(a_interval, a_currentTime);

	controller->Update(this);
}

void CastingBar::SetPercent(float percent)
{
	const GFxValue value = percent * 100.0f;

	uiMovie->Invoke("meter.doShow", nullptr, nullptr, 0);
	uiMovie->Invoke("meter.setMeterPercent", nullptr, &value, 1);

	displayed = true;
}

void CastingBar::FadeOut()
{
	if (displayed) {
		uiMovie->Invoke("meter.doFadeOut", nullptr, nullptr, 0);
		displayed = false;
	}
}

void CastingBar::ApplyLayout() 
{
	const GFxValue x_pos = Settings::WidgetPositionX;
	const GFxValue y_pos = Settings::WidgetPositionY;
	const GFxValue x_scl = Settings::WidgetScaleX;
	const GFxValue y_scl = Settings::WidgetScaleY;

    GFxValue posArray[5] = {x_pos, y_pos, 0.0, x_scl, y_scl};

    uiMovie->Invoke("meter.setLocation", nullptr, posArray, 5);
}

void CastingBar::OnShow()
{
	logger::trace("CastingBar::OnShow");

	ApplyLayout();
};

void CastingBar::OnHide() 
{
	logger::trace("CastingBar::OnHide");
};
