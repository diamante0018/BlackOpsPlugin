#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include <utils/hook.hpp>

#include "command.hpp"

namespace command_additions {
namespace {
void cmd_vstr_f() {
  const command::params params;

  if (params.size() < 2) {
    game::Com_Printf(game::CON_CHANNEL_DONT_FILTER,
                     "vstr <variablename> : execute a variable command\n");
    return;
  }

  const auto* dvar_name = params.get(1);
  const auto* dvar = game::Dvar_FindVar(dvar_name);

  if (!dvar) {
    game::Com_Printf(game::CON_CHANNEL_DONT_FILTER, "%s doesn't exist\n",
                     dvar_name);
    return;
  }

  if (dvar->type == game::DVAR_TYPE_STRING ||
      dvar->type == game::DVAR_TYPE_ENUM) {
    // Adds \n automatically
    command::execute(dvar->current.string);
  } else {
    game::Com_Printf(game::CON_CHANNEL_DONT_FILTER,
                     "%s is not a string-based dvar\n", dvar->name);
  }
}
} // namespace

class component final : public component_interface {
public:
  void post_unpack() override {
    // Because this plugin is loaded late we must override the structure
    auto* cmd_vstr_f_var = reinterpret_cast<game::cmd_function_s*>(
        game::select(0x355E69C, 0x243FB1C));
    cmd_vstr_f_var->function = cmd_vstr_f;

   if (game::environment::is_sp()) {
      // Fix (client) crash from SV_DropClient (server side)
      utils::hook::set<std::uint8_t>(0x634C1D, 0xEB);
   }
  }
};
} // namespace command_additions

REGISTER_COMPONENT(command_additions::component)
