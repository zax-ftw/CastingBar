#pragma once

#include "PCH.h"
#include "RE/G/GFxValue.h"
#include "RE/G/GPtr.h"

class CastingBar : RE::IMenu 
{
public:
	static constexpr const char* MENU_PATH = "CastingBar";
	static constexpr const char* MENU_NAME = "CastingBar";

	enum State
	{
		kNone = 0,
		kUnk1 = 1,
		kCharging = 2,
		kReady = 3, // fully charged
		kRelease = 4, // pre-casting for conc, release for non-conc
		kUnk5 = 5,
		kCasting = 6 // concentration spells
	};

    CastingBar();

    static void Register();
	static void Show();
	static void Hide();
	static void Update();
    static void ApplyLayout(RE::GPtr<IMenu> oxygenMeter);
	static void ToggleVisibility(bool mode);

    static RE::stl::owner<IMenu*> Creator() { return new CastingBar(); }

	void AdvanceMovie(float a_interval, std::uint32_t a_currentTime) override;

private:
	using CastingSource = RE::MagicSystem::CastingSource;

	static RE::MagicCaster* GetCastingSource(RE::Actor* actor);
    static float GetChargeProgress(RE::MagicCaster* caster);

	static inline bool casting{ false };
	static inline bool charged{ false };
};
