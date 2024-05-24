#pragma once

#include "IState.h"

class MagicState : public IState
{
public:
	using SlotType = RE::MagicSystem::CastingSource;

	//enum CastState : std::uint32_t
	//{
	//	kNone = 0,
	//	kUnk1 = 1,
	//	kCharging = 2,
	//	kCharged = 3,
	//	kRelease = 4,
	//	kUnk5 = 5,
	//	kConcentrating = 6
	//};

	MagicState(SlotType a_slot);

	std::unique_ptr<IState> HandleAction(RE::Actor* actor, Action action) override;
	std::optional<float> GetProgress(RE::PlayerCharacter* player) override;

	const char* GetMovie() override { return "CastingBar_Spell"; };

private:

	RE::MagicCaster* GetMagicCaster(RE::Actor* actor);

	SlotType slot;
};
