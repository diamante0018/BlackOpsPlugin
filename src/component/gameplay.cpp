#include <stdinc.hpp>

#include "loader/component_loader.hpp"
#include "utils/hook.hpp"

namespace gameplay
{
	namespace
	{
		const game::dvar_s* player_meleeRange = nullptr;
		utils::hook::detour fire_weapon_melee_hook;

		void fire_weapon_melee_stub(game::gentity_s* ent, int time)
		{
			if (player_meleeRange->current.value == 0.0f)
				return;

			fire_weapon_melee_hook.invoke<void>(ent, time);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			player_meleeRange = *reinterpret_cast<game::dvar_s**>(SELECT(0xC51990, 0xBCAFE4));

			fire_weapon_melee_hook.create(SELECT(0x401E00, 0x465E40), &fire_weapon_melee_stub);
		}
	};
}

REGISTER_COMPONENT(gameplay::component)
