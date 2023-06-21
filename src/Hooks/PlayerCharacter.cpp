#include "PlayerCharacter.h"


namespace Hooks
{	
	// Workaround for VoiceState getting stuck on unequipping spells
	// second BeginCastVoice gets send without Voice_SpellFire_Event
	void PlayerCharacterEx::DrawWeapon_Hook(RE::PlayerCharacter* a_player, bool a_draw)
	{
		//logger::trace("DrawWeapon_Hook: {}", a_draw);

		auto shoutLevel = a_player->GetCurrentShoutLevel();
		if (shoutLevel == VariationID::kNone) { // not shouting
			_DrawWeapon(a_player, a_draw);
		}
	}
}
