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
	case Action::kAttackStop:
		return std::make_unique<DefaultState>();
	}

	return nullptr;
}

std::optional<float> MagicState::GetProgress(RE::PlayerCharacter* player)
{
	RE::MagicCaster* caster = GetMagicCaster(player);

	RE::MagicItem* spell = caster->currentSpell;

	using S = RE::MagicCaster::State;
	auto state = caster->state.underlying();

	if (spell) {
		float chargeTime = spell->GetChargeTime();	
		
		if (i == 0) {
			castingTime = caster->castingTimer;
			
			i++;
		}
		if (state == static_cast<uint32_t>(S::kUnk04) || state == static_cast<uint32_t>(S::kUnk08) || state == static_cast<uint32_t>(S::kUnk09)) {
			
			i = 0;
		}
		if (spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration) {
			if (chargeTime > 0.0 || castingTime > 0.0) {
				float castingTimer = caster->castingTimer;
				return (castingTime - castingTimer) / castingTime;
			}
		} else {
			if (chargeTime > 0.0 && castingTime > 0.0) {
				float castingTimer = caster->castingTimer;
				return (chargeTime - castingTimer) / chargeTime;
			}
		}
		
	}
	return std::nullopt;
}

MagicCaster* MagicState::GetMagicCaster(Actor* actor)
{
	return actor->GetMagicCaster(slot);
}
