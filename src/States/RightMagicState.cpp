#include "RightMagicState.h"
#include "DefaultState.h"

using namespace RE;


RightMagicState::RightMagicState() : 
	MagicState(SlotType::kRightHand)
{
	logger::trace("RightMagicState::Ctor");
}

std::unique_ptr<IState> RightMagicState::HandleAction(Actor* actor, Action action)
{
	logger::trace("RightMagicState::HandleAction {}", 
		magic_enum::enum_name(action));

	switch (action) {
	case Action::kSpellFireRight:
	case Action::kSpellEquipRight:
		return std::make_unique<DefaultState>();
	default:
		return MagicState::HandleAction(actor, action);
	}

	return nullptr;
}
