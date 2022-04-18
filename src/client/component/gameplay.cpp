#include <std_include.hpp>

#include "../loader/component_loader.hpp"
#include <utils/hook.hpp>

namespace gameplay {
namespace {
game::dvar_s** player_sustainAmmo = nullptr;

utils::hook::detour pm_weapon_use_ammo_hook;

void pm_weapon_use_ammo_stub(void* ps, int wp, int amount) {
  if (!(*player_sustainAmmo)->current.enabled) {
    pm_weapon_use_ammo_hook.invoke<void>(ps, wp, amount);
  }
}
} // namespace

class component final : public component_interface {
public:
  void post_unpack() override {
    if (game::environment::t5zm()) {
      player_sustainAmmo = reinterpret_cast<game::dvar_s**>(0xBCD250);
      pm_weapon_use_ammo_hook.create(0x6979B0, &pm_weapon_use_ammo_stub);
    }
  }
};
} // namespace gameplay

REGISTER_COMPONENT(gameplay::component)
