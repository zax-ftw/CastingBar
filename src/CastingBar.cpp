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

	auto player = PlayerCharacter::GetSingleton();
	controller.reset(new StateController(this, player));
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

MessageResult CastingBar::ProcessMessage(UIMessage& a_message)
{
	if (a_message.menu == MENU_NAME) {

		switch (a_message.type.get()) {
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

    return IMenu::ProcessMessage(a_message);
}

void CastingBar::AdvanceMovie(float a_interval, std::uint32_t a_currentTime) 
{
    IMenu::AdvanceMovie(a_interval, a_currentTime);

	controller->Update();
}

void CastingBar::LoadMovie(const char* a_movie)
{
	//logger::trace("CastingBar::LoadMovie: {}", a_movie);

	auto scaleform = BSScaleformManager::GetSingleton();
	scaleform->LoadMovie(this, this->uiMovie, a_movie);

	ApplyLayout();
}

void CastingBar::SetPercent(float a_percent)
{
	const GFxValue value = a_percent * 100.0f;
	
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

	uiMovie->SetMouseCursorCount(0); // disable input
	uiMovie->SetVisible(true);
}

void CastingBar::OnShow()
{
	//logger::trace("CastingBar::OnShow");
};

void CastingBar::OnHide() 
{
	//logger::trace("CastingBar::OnHide");
};
