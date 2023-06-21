#include "LeftMagicState.h"
#include "DefaultState.h"

using namespace RE;


LeftMagicState::LeftMagicState() : 
	MagicState(SlotType::kLeftHand)
{
	logger::trace("LeftMagicState::Ctor");
}

std::unique_ptr<IState> LeftMagicState::HandleAction(Actor* actor, Action action)
{
	logger::trace("LeftMagicState::HandleAction {}", 
		magic_enum::enum_name(action));

	switch (action) {
	case Action::kSpellFireLeft:
	case Action::kSpellEquipLeft:
		return std::make_unique<DefaultState>();
	default:
		return MagicState::HandleAction(actor, action);
	}

	return nullptr;
}
