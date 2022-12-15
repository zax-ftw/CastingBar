#include "CastingBar.h"

#include "Settings.h"

using namespace RE;

CastingBar::CastingBar() 
{
    logger::info("CastingBar::Ctor");

    depthPriority = 0;

    menuFlags.set(UI_MENU_FLAGS::kAlwaysOpen);
    menuFlags.set(UI_MENU_FLAGS::kRequiresUpdate);
    menuFlags.set(UI_MENU_FLAGS::kAllowSaving);

    if (uiMovie) {
        uiMovie->SetMouseCursorCount(0);  // disable input
        uiMovie->SetVisible(true);
    }

    auto scaleform = BSScaleformManager::GetSingleton();
    scaleform->LoadMovie(this, this->uiMovie, MENU_PATH);
}

CastingBar::~CastingBar() 
{
    logger::info("CastingBar::Dtor");
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
        queue->AddMessage(CastingBar::MENU_NAME, UI_MESSAGE_TYPE::kShow, nullptr);
    }
}

void CastingBar::Hide() 
{
    if (auto queue = UIMessageQueue::GetSingleton()) {
        queue->AddMessage(CastingBar::MENU_NAME, UI_MESSAGE_TYPE::kHide, nullptr);
    }
}

CastingBar::MessageResult CastingBar::ProcessMessage(UIMessage& message)
{
    using Type = RE::UI_MESSAGE_TYPE;

	if (message.menu == MENU_NAME) {

		logger::info("{}::ProcessMessage >> {}", 
			message.menu, magic_enum::enum_name(message.type.get()));

		switch (message.type.get()) {
			case Type::kShow:
            case Type::kReshow:
				OnShow();
				break;
			case Type::kHide:
            case Type::kForceHide:
				OnHide();
				break;
		}
    }

    return IMenu::ProcessMessage(message);
}

void CastingBar::AdvanceMovie(float a_interval, std::uint32_t a_currentTime) 
{
    IMenu::AdvanceMovie(a_interval, a_currentTime);

    CastingBar::Update();
}

CastingBar::EventResult CastingBar::ProcessEvent(const MenuOpenCloseEvent* a_event, MenuOpenCloseEventSource*) 
{
    auto controlMap = ControlMap::GetSingleton();
    if (controlMap) {
        auto& priorityStack = controlMap->contextPriorityStack;
        if (!priorityStack.empty() && priorityStack.back() == Context::kGameplay) {
            ToggleVisibility(true);
        } else {
            ToggleVisibility(false);
        }
    }
    return EventResult::kContinue;
}

void CastingBar::Update() 
{
    ApplyLayout();

    auto player = PlayerCharacter::GetSingleton();
    auto caster = GetCastingSource(player);

    if (caster) {

        casting = true;

        static bool flashWhenCharged = Settings::GetSingleton().flashWhenCharged;

        if (charged && flashWhenCharged) {
            // uiMovie->Invoke("meter.doFlash", nullptr, nullptr, 0);
            return;
        }

        float progress = GetChargeProgress(caster);
        const GFxValue percent = progress * 100.0;

        uiMovie->Invoke("meter.doShow", nullptr, nullptr, 0);
        uiMovie->Invoke("meter.setMeterPercent", nullptr, &percent, 1);

        if (progress >= 1.0) {
            charged = true;
        }

    } else if (casting) {

        casting = false;
        charged = false;

        uiMovie->Invoke("meter.doFadeOut", nullptr, nullptr, 0);
    }
}

void CastingBar::ApplyLayout() 
{
    auto& settings = Settings::GetSingleton();

    const GFxValue x_pos = settings.widgetXPosition;
    const GFxValue y_pos = settings.widgetYPosition;
    const GFxValue x_scl = settings.widgetXScale;
    const GFxValue y_scl = settings.widgetYScale;

    GFxValue posArray[5] = {x_pos, y_pos, 0.0, x_scl, y_scl};

    uiMovie->Invoke("meter.setLocation", nullptr, posArray, 5);
}

void CastingBar::OnShow() 
{
    if (auto ui = UI::GetSingleton()) {
        ui->AddEventSink<MenuOpenCloseEvent>(this);
    }
};

void CastingBar::OnHide() 
{
    if (auto ui = UI::GetSingleton()) {
        ui->RemoveEventSink<MenuOpenCloseEvent>(this);
    }
};

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
    // TODO: replace with state.get() when states are done
    auto state = caster->state.underlying();
    if (state != State::kCharging) {
        return 1.0f;
    }

    float targetTime = caster->currentSpell->GetChargeTime();
    float actualTime = caster->castingTimer;

    return (targetTime - actualTime) / targetTime;
}
