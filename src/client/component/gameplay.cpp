#include <std_include.hpp>

#include "../loader/component_loader.hpp"
#include <utils/hook.hpp>

namespace gameplay {
namespace {
game::dvar_s** player_sustainAmmo = nullptr;
game::dvar_s** player_sprintStrafeSpeedScale = nullptr;

game::dvar_s** jump_height = nullptr;

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
    fld dword ptr [eax + 0x18] // dvar_s.current.value

    pop eax
    ret
  }
}

void __declspec(naked) jump_start_mp_stub() {
  static DWORD func = 0x7AEF10;

  __asm {
    mov esi, jump_height
    mov esi, [esi] // Access pointer
    movss xmm0, dword ptr [esi + 0x18] // dvar_s.current.value

    // Game's code
    mov ecx, ebp
    mov esi, ebx

    push 0x5CE940 // return address
    jmp func
  }
}

void __declspec(naked) jump_start_sp_stub() {
  static DWORD func = 0x75BDD0;

  __asm {
    mov esi, jump_height
    mov esi, [esi] // Access pointer
    movss xmm0, dword ptr [esi + 0x18] // dvar_s.current.value

    // Game's code
    mov edx, ebp
    mov esi, ebx

    push 0x5D0440 // return address
    jmp func
  }
}
} // namespace

class component final : public component_interface {
public:
  void post_unpack() override {
    if (game::environment::is_mp()) {
      jump_height = reinterpret_cast<game::dvar_s**>(0xC4F7D8);
      utils::hook::jump(0x5CE92F, jump_start_mp_stub);
    }

    if (game::environment::is_sp()) {
      player_sustainAmmo = reinterpret_cast<game::dvar_s**>(0xBCD250);
      pm_weapon_use_ammo_hook.create(0x6979B0, &pm_weapon_use_ammo_stub);

      jump_height = reinterpret_cast<game::dvar_s**>(0xBCAE58);
      utils::hook::jump(0x5D042F, jump_start_sp_stub);
    }

    player_sprintStrafeSpeedScale =
        reinterpret_cast<game::dvar_s**>(SELECT_VALUE(0xC51AE8, 0xBCD18C));
    utils::hook::jump(SELECT_VALUE(0x6344B0, 0x611BC0),
                      bg_get_sprint_strafe_speed_scale_stub);
  }
};
} // namespace gameplay

REGISTER_COMPONENT(gameplay::component)
