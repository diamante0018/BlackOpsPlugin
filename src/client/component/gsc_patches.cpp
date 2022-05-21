#include <std_include.hpp>

#include "../loader/component_loader.hpp"
#include <utils/hook.hpp>

namespace gsc_patches {
namespace {
void g_log_printf(const char* fmt) { game::G_LogPrintf("%s", fmt); }
} // namespace

class component final : public component_interface {
public:
  void post_unpack() override {
    if (game::environment::is_mp()) {
      utils::hook::call(0x8426D3, g_log_printf);
    }
  }
};
} // namespace gsc_patches

REGISTER_COMPONENT(gsc_patches::component)
