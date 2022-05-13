#include <std_include.hpp>

#include "../loader/component_loader.hpp"
#include <utils/hook.hpp>

namespace gameplay {
namespace {
game::dvar_s** player_sustainAmmo = nullptr;
game::dvar_s** player_sprintStrafeSpeedScale = nullptr;

utils::hook::detour pm_weapon_use_ammo_hook;

void pm_weapon_use_ammo_stub(void* ps, int wp, int amount) {
  if (!(*player_sustainAmmo)->current.enabled) {
    pm_weapon_use_ammo_hook.invoke<void>(ps, wp, amount);
  }
}

void __declspec(naked) bg_get_sprint_strafe_speed_scale_stub() {
  __asm {
    push eax

    mov eax, player_sprintStrafeSpeedScale
    mov eax, [eax] // Access pointer
    fld dword ptr [eax + 0x18]; // dvar_s.current.value

    pop eax
    ret
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

    player_sprintStrafeSpeedScale =
        reinterpret_cast<game::dvar_s**>(SELECT(0xC51AE8, 0xBCD18C));
    utils::hook::jump(SELECT(0x6344B0, 0x611BC0),
                      bg_get_sprint_strafe_speed_scale_stub);
  }
};
} // namespace gameplay

REGISTER_COMPONENT(gameplay::component)
