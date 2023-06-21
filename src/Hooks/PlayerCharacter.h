#pragma once

namespace Hooks
{
	class PlayerCharacterEx : public RE::PlayerCharacter
	{
	public:

		static void InstallHooks(SKSE::Trampoline& trampoline)
		{
			REL::Relocation<std::uintptr_t> vtbl{ RE::VTABLE_PlayerCharacter[0] };

			_DrawWeapon = vtbl.write_vfunc(0xA6, DrawWeapon_Hook);  // 0x530 / 8
		}

	private:
		using VariationID = RE::TESShout::VariationID;

		static void DrawWeapon_Hook(RE::PlayerCharacter* a_player, bool a_draw);

		static inline REL::Relocation<decltype(DrawWeapon_Hook)> _DrawWeapon;
	};
}
