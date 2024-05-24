#pragma once

#include "IState.h"

class RangedState : public IState
{
public:

	struct BowDrawnTimerEntry
	{
		float bowDrawTime;
		uint64_t unk1;
	};

	RangedState();

	static void InstallHooks(SKSE::Trampoline& trampoline);

	std::unique_ptr<IState> HandleAction(RE::Actor* actor, Action action) override;
	std::optional<float> GetProgress(RE::PlayerCharacter* player) override;

	const char* GetMovie() override { return "CastingBar_Bow"; };

private:
	using PlayerArrayBA0 = RE::BSTSmallArray<BowDrawnTimerEntry, 2>;

	static float GetArrowPower(float drawTime, float bowSpeed);
	static float GetArrowMinPower();

	float GetEquippedWeaponSpeed(RE::Actor* actor, bool leftHand);
	float GetBowDrawnTime(RE::PlayerCharacter* player);

	static inline REL::Relocation<decltype(GetArrowPower)> _GetArrowPower;
};
