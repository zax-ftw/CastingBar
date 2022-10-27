#include "CastingBar.h"
#include "Settings.h"

using namespace RE;
using namespace SKSE;

CastingBar::CastingBar()
{
	auto scaleformManager = BSScaleformManager::GetSingleton();

	inputContext = Context::kNone;
	depthPriority = 0;

	menuFlags.set(UI_MENU_FLAGS::kAlwaysOpen);
	menuFlags.set(UI_MENU_FLAGS::kRequiresUpdate);
	menuFlags.set(UI_MENU_FLAGS::kAllowSaving);

	if (uiMovie) {
		uiMovie->SetMouseCursorCount(0);  // disable input
	}

	scaleformManager->LoadMovie(this, this->uiMovie, MENU_PATH);
}	

void CastingBar::Register()
{
	auto ui = UI::GetSingleton();
	if (ui) {
		ui->Register(MENU_NAME, Creator);

		CastingBar::Show();
	}
}

void CastingBar::Show()
{
	auto msgQ = UIMessageQueue::GetSingleton();
	if (msgQ) {
		msgQ->AddMessage(CastingBar::MENU_NAME, UI_MESSAGE_TYPE::kShow, nullptr);
	}
}

void CastingBar::Hide()
{
	auto msgQ = UIMessageQueue::GetSingleton();
	if (msgQ) {
		msgQ->AddMessage(CastingBar::MENU_NAME, UI_MESSAGE_TYPE::kHide, nullptr);
	}
}

void CastingBar::ToggleVisibility(bool mode)
{
	auto ui = UI::GetSingleton();
	if (!ui)
		return;

	auto overlayMenu = ui->GetMenu(CastingBar::MENU_NAME);
	if (!overlayMenu || !overlayMenu->uiMovie)
		return;

	overlayMenu->uiMovie->SetVisible(mode);
}

void CastingBar::Update()
{
	GPtr<IMenu> oxygenMeter = UI::GetSingleton()->GetMenu(CastingBar::MENU_NAME);
	if (!oxygenMeter || !oxygenMeter->uiMovie)
		return;

	ApplyLayout(oxygenMeter);

	auto player = PlayerCharacter::GetSingleton();
	auto caster = GetCastingSource(player);

	if (caster) {

		casting = true;

		float progress = GetChargeProgress(caster);
		const GFxValue percent = progress * 100.0;

		static bool flashWhenCharged = Settings::GetSingleton()->flashWhenCharged;

		if (charged && flashWhenCharged) {
			//oxygenMeter->uiMovie->Invoke("meter.doFlash", nullptr, nullptr, 0);
            log::info("Flashing at {}", progress);
			return;
		}

		oxygenMeter->uiMovie->Invoke("meter.doShow", nullptr, nullptr, 0);
		oxygenMeter->uiMovie->Invoke("meter.setMeterPercent", nullptr, &percent, 1);

		if (progress >= 1.0) {
			charged = true;
		}

	} else if (casting) {

		casting = false;
		charged = false;
		//const GFxValue refill = 0;
		//oxygenMeter->uiMovie->Invoke("meter.updateMeterPercent", nullptr, &refill, 1);
		oxygenMeter->uiMovie->Invoke("meter.doFadeOut", nullptr, nullptr, 0);
	}
}

void CastingBar::ApplyLayout(GPtr<IMenu> oxygenMeter)
{
	auto settings = Settings::GetSingleton();

	const GFxValue widgetXPosition = settings->widgetXPosition;
	const GFxValue widgetYPosition = settings->widgetYPosition;
	const GFxValue widgetXScale = settings->widgetXScale;
	const GFxValue widgetYScale = settings->widgetYScale;

	GFxValue posArray[5]{ widgetXPosition, widgetYPosition, 0.0, widgetXScale, widgetYScale };

	oxygenMeter->uiMovie->Invoke("meter.setLocation", nullptr, posArray, 5);
}

void CastingBar::AdvanceMovie(float a_interval, std::uint32_t a_currentTime)
{
	CastingBar::Update();
	IMenu::AdvanceMovie(a_interval, a_currentTime);
}

MagicCaster* CastingBar::GetCastingSource(Actor* actor)
{
	auto caster = actor->GetMagicCaster(CastingSource::kRightHand);
	if (!caster->currentSpell) {
		caster = actor->GetMagicCaster(CastingSource::kLeftHand);
		if (!caster->currentSpell) {
			return nullptr;
		}
	}
	return caster;
}

float CastingBar::GetChargeProgress(MagicCaster* caster)
{
	// TO DO: replace with get() when states are done
	auto state = caster->state.underlying();

    log::info("State = {}", state);

	if (state != State::kCharging) {
		return 1.0f;
	}

	float targetTime = caster->currentSpell->GetChargeTime();
	float actualTime = caster->castingTimer;

	auto result = (targetTime - actualTime) / targetTime;

	return result;
}
