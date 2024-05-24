#include "MagicState.h"
#include "DefaultState.h"

using namespace RE;


MagicState::MagicState(SlotType a_slot) :
	slot(a_slot)
{
	logger::trace("MagicState::Ctor ({})",  
		magic_enum::enum_name(a_slot));
}

std::unique_ptr<IState> MagicState::HandleAction(Actor* actor, Action action)
{
	logger::trace("MagicState::HandleAction {}", 
		magic_enum::enum_name(action));

	switch (action) {
	case Action::kSpellCastStop:
	case Action::kSpellPreAimedToAimed:
	case Action::kSpellUnequip:
	case Action::kWeaponSheathe:
	case Action::kWeaponDraw:
		return std::make_unique<DefaultState>();
	}

	return nullptr;
}

std::optional<float> MagicState::GetProgress(PlayerCharacter* player)
{
	MagicCaster* caster = GetMagicCaster(player);

	MagicItem* spell = caster->currentSpell;
	if (spell) {
		float chargeTime = spell->GetChargeTime();
		if (chargeTime > 0.0 ) {
			float castingTimer = caster->castingTimer;
			return (chargeTime - castingTimer) / chargeTime;
		}
	}
	return std::nullopt;
}

MagicCaster* MagicState::GetMagicCaster(Actor* actor)
{
	return actor->GetMagicCaster(slot);
}
