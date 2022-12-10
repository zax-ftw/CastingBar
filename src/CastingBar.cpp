#include "CastingBar.h"
#include "Settings.h"

using namespace RE;

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
	auto queue = UIMessageQueue::GetSingleton();
	if (queue) {
		queue->AddMessage(CastingBar::MENU_NAME, UI_MESSAGE_TYPE::kShow, nullptr);
	}
}

void CastingBar::Hide()
{
	auto queue = UIMessageQueue::GetSingleton();
	if (queue) {
		queue->AddMessage(CastingBar::MENU_NAME, UI_MESSAGE_TYPE::kHide, nullptr);
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
	GPtr<IMenu> menu = UI::GetSingleton()->GetMenu(CastingBar::MENU_NAME);
	if (!menu || !menu->uiMovie)
		return;

	ApplyLayout(menu);

	auto player = PlayerCharacter::GetSingleton();
	auto caster = GetCastingSource(player);

	if (caster) {

		casting = true;

		static bool flashWhenCharged = Settings::GetSingleton().flashWhenCharged;

		if (charged && flashWhenCharged) {
			//menu->uiMovie->Invoke("meter.doFlash", nullptr, nullptr, 0);
			return;
		}

		float progress = GetChargeProgress(caster);
		const GFxValue percent = progress * 100.0;

		menu->uiMovie->Invoke("meter.doShow", nullptr, nullptr, 0);
		menu->uiMovie->Invoke("meter.setMeterPercent", nullptr, &percent, 1);

		if (progress >= 1.0) {
			charged = true;
		}

	} else if (casting) {

		casting = false;
		charged = false;

		menu->uiMovie->Invoke("meter.doFadeOut", nullptr, nullptr, 0);
	}
}

void CastingBar::ApplyLayout(GPtr<IMenu> oxygenMeter)
{
	auto& settings = Settings::GetSingleton();

	const GFxValue widgetXPosition = settings.widgetXPosition;
	const GFxValue widgetYPosition = settings.widgetYPosition;
	const GFxValue widgetXScale = settings.widgetXScale;
	const GFxValue widgetYScale = settings.widgetYScale;

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
	// TODO: replace with get() when states are done
	auto state = caster->state.underlying();
	if (state != State::kCharging) {
		return 1.0f;
	}

	float targetTime = caster->currentSpell->GetChargeTime();
	float actualTime = caster->castingTimer;

	return (targetTime - actualTime) / targetTime;
}
