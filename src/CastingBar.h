#pragma once

class CastingBar : public RE::IMenu, 
	public RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
public:
	using MenuOpenCloseEventSource = RE::BSTEventSource<RE::MenuOpenCloseEvent>;
	using MessageResult = RE::UI_MESSAGE_RESULTS;
	using EventResult = RE::BSEventNotifyControl;

	static constexpr const char* MENU_PATH = "CastingBar";
	static constexpr const char* MENU_NAME = "CastingBar";

	enum State
	{
		kNone = 0,
		kUnk1 = 1,
		kCharging = 2,
		kReady = 3,    // fully charged
		kRelease = 4,  // pre-casting for conc, release for non-conc
		kUnk5 = 5,
		kCasting = 6  // concentration spells
	};

	CastingBar();
	~CastingBar();

	static RE::IMenu* Creator() { return new CastingBar(); }

	static void Register();
	static void Show();
	static void Hide();

	MessageResult ProcessMessage(RE::UIMessage& a_message) override;
	void AdvanceMovie(float a_interval, std::uint32_t a_currentTime) override;
	EventResult ProcessEvent(const RE::MenuOpenCloseEvent* a_event, MenuOpenCloseEventSource* a_source) override;

private:
	using CastingSource = RE::MagicSystem::CastingSource;

	void ToggleVisibility(bool visible) { uiMovie->SetVisible(visible); };

	void Update();
	void ApplyLayout();

	void OnShow();
	void OnHide();

	static RE::MagicCaster* GetCastingSource(RE::Actor* actor);
	static float GetChargeProgress(RE::MagicCaster* caster);

	static inline bool casting{ false };
	static inline bool charged{ false };
};
