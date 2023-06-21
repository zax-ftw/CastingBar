#include "DefaultState.h"
#include "RightMagicState.h"
#include "LeftMagicState.h"
#include "RangedState.h"
#include "VoiceState.h"

#include "Settings.h"

using namespace RE;


enum ItemType
{
	kHandToHand = 0,
	kOneHandSword = 1,
	kOneHandDagger = 2,
	kOneHandAxe = 3,
	kOneHandMace = 4,
	kTwoHandSword = 5,
	kTwoHandAxe = 6,
	kBow = 7,
	kStaff = 8,
	kSpell = 9,
	kShield = 10,
	kTorch = 11,
	kCrossbow = 12
};

ItemType GetObjectTypeId(TESForm *a_form)
{
	using func_t = decltype(&GetObjectTypeId);
	REL::Relocation<func_t> func{ RELOCATION_ID(14125, 14220) };
	return func(a_form);
}

ItemType GetEquippedObjectType(Actor *actor, bool leftHand)
{
	TESForm* object = actor->GetEquippedObject(leftHand);
	return GetObjectTypeId(object);
}

DefaultState::DefaultState()
{
	logger::trace("DefaultState::Ctor");
}

std::unique_ptr<IState> DefaultState::HandleAction(Actor* actor, Action action)
{
	logger::trace("DefaultState::HandleAction {}", 
		magic_enum::enum_name(action));

	switch (action) 
	{
	case Action::kSpellCastRight:
		if (Settings::EnableForSpell) {
			return std::make_unique<RightMagicState>();
		}
		break;
	case Action::kSpellCastLeft:
		if (Settings::EnableForSpell) {
			return std::make_unique<LeftMagicState>();
		}
		break;
	case Action::kBowDraw:
		{
			auto type = GetEquippedObjectType(actor, false);
			switch (type)
			{
			case ItemType::kBow:
				if (Settings::EnableForBow) {
					return std::make_unique<RangedState>();
				}
				break;
			case ItemType::kCrossbow:
				if (Settings::EnableForBow) {
					//return std::make_unique<CrossbowState>();
				}
				break;
			}
		}
		break;
	case Action::kVoiceCast:
		if (Settings::EnableForShout) {
			return std::make_unique<VoiceState>();
		}
		break;
	}

	return nullptr;
}
