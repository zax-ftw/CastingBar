#pragma once

enum Action : std::uint32_t
{
kNone,
kWeaponDraw,
kWeaponSheathe,
kWeaponSwingRight,
kWeaponSwingLeft,
kAttackStop,
kPowerAttackStop,
kSpellEquip,
kSpellUnequip,
kSpellEquipRight,
kSpellEquipLeft,
kSpellCastRight,
kSpellCastLeft,
kSpellFireRight,
kSpellFireLeft,
kSpellPreAimedToAimed,
kSpellCastStop,
kVoiceCast,
kVoiceFire,
kVoiceStop,
kBowDraw,
kBowDrawn,
kBowRelease,
kBowReset
};
