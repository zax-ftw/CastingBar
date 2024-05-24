#include "RangedState.h"
#include "DefaultState.h"

#include "Utils/GameSettings.h"

using namespace RE;


float GetWeaponSpeed(const ActorValueOwner* a_actor, TESObjectWEAP* a_weapon, bool a_left)
{
	using func_t = decltype(&GetWeaponSpeed);
	REL::Relocation<func_t> func{ RELOCATION_ID(25851, 26417) };
	return func(a_actor, a_weapon, a_left);
}

RangedState::RangedState()
{
	logger::trace("RangedState::Ctor");
}

std::unique_ptr<IState> RangedState::HandleAction(Actor* actor, Action action)
{
	logger::trace("RangedState::HandleAction {}", 
		magic_enum::enum_name(action));

	switch (action) {
	case Action::kBowRelease:
	case Action::kBowReset:
	case Action::kWeaponSheathe:
	case Action::kWeaponDraw:
		return std::make_unique<DefaultState>();
	}
	return nullptr;
}

std::optional<float> RangedState::GetProgress(PlayerCharacter* player)
{
	float drawTime = GetBowDrawnTime(player);
	if (drawTime > 0.0) {

		float bowSpeed = GetEquippedWeaponSpeed(player, false);
		float arrPower = GetArrowPower(drawTime, bowSpeed);
		float minPower = GetArrowMinPower();

		//logger::trace("drawTime: {} | bowSpeed: {} | arrPower: {} | minPower: {}",
		//	drawTime, bowSpeed, arrPower, minPower);

		return (arrPower - minPower) / (1.0f - minPower);
	}
	return std::nullopt;
}

float RangedState::GetEquippedWeaponSpeed(Actor* actor, bool leftHand)
{
	auto weapon = skyrim_cast<TESObjectWEAP*>(
		actor->GetEquippedObject(leftHand));

	if (weapon) {
		auto avo = actor->AsActorValueOwner();
		return GetWeaponSpeed(avo, weapon, leftHand);
	}
	return 1.0f;
}

float RangedState::GetBowDrawnTime(PlayerCharacter* player)
{
	auto& runtime = player->GetPlayerRuntimeData();
	const auto arr = reinterpret_cast<const PlayerArrayBA0*>(&(runtime.unkBA0));
	return arr->size() > 0 ? arr->back().bowDrawTime : 0.0f;
}

float RangedState::GetArrowPower(float drawTime, float bowSpeed)
{
	return _GetArrowPower(drawTime, bowSpeed);
}

float RangedState::GetArrowMinPower()
{
	return Utils::GetGameSettingFloat("fArrowMinPower", 0.35);
}

void RangedState::InstallHooks(SKSE::Trampoline& trampoline)
{
	// Compatibility with Scrambled Bugs by KernalsEgg
	_GetArrowPower = trampoline.write_call<5>(
		RELOCATION_ID(42928, 44108).address() + OFFSET(0x604, 0x5D6, 0),
		&RangedState::GetArrowPower);
}
