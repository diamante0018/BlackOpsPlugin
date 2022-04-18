#include <std_include.hpp>

#include "../loader/component_loader.hpp"

#include <utils/string.hpp>
#include <utils/nt.hpp>

#include "command.hpp"

constexpr auto CMD_MAX_NESTING = 8;

namespace command {
std::unordered_map<std::string, std::function<void(params&)>> handlers;

namespace {
void cmd_vstr_f(const params& params) {
  if (params.size() < 2) {
    game::Com_Printf(game::CON_CHANNEL_DONT_FILTER,
                     "vstr <variablename> : execute a variable command\n");
    return;
  }

  const auto* dvar_name = params.get(1);
  const auto* dvar = game::Dvar_FindVar(dvar_name);

  if (dvar == nullptr) {
    game::Com_Printf(game::CON_CHANNEL_DONT_FILTER, "%s doesn't exist\n",
                     dvar_name);
    return;
  }

  if (dvar->type == game::DVAR_TYPE_STRING ||
      dvar->type == game::DVAR_TYPE_ENUM) {
    execute(dvar->current.string);
  } else {
    game::Com_Printf(game::CON_CHANNEL_DONT_FILTER,
                     "%s is not a string-based dvar\n", dvar->name);
  }
}
} // namespace

void main_handler() {
  params params = {};

  const auto command = utils::string::to_lower(params[0]);

  if (const auto got = handlers.find(command); got != handlers.end()) {
    got->second(params);
  }
}

params::params() : nesting_(game::sv_cmd_args->nesting) {
  assert(game::sv_cmd_args->nesting < CMD_MAX_NESTING);
}

int params::size() const { return game::sv_cmd_args->argc[this->nesting_]; }

const char* params::get(const int index) const {
  if (index >= this->size()) {
    return "";
  }

  return game::sv_cmd_args->argv[this->nesting_][index];
}

std::string params::join(const int index) const {
  std::string result = {};

  for (auto i = index; i < this->size(); i++) {
    if (i > index)
      result.append(" ");
    result.append(this->get(i));
  }
  return result;
}

void add_raw(const char* name, void (*callback)()) {
  game::Cmd_AddCommandInternal(
      name, callback,
      utils::memory::get_allocator()->allocate<game::cmd_function_t>());
}

void add(const char* name, const std::function<void(const params&)>& callback) {
  const auto command = utils::string::to_lower(name);

  if (!handlers.contains(command)) {
    add_raw(name, main_handler);
  }

  handlers[command] = callback;
}

void execute(std::string command, const bool sync) {
  command += "\n";

  if (sync) {
    game::Cmd_ExecuteSingleCommand(game::LOCAL_CLIENT_0, 0, command.data());
  } else {
    game::Cbuf_AddText(game::LOCAL_CLIENT_0, command.data());
  }
}

class component final : public component_interface {
public:
  void post_unpack() override { add_commands_generic(); }

private:
  static void add_commands_generic() {
    add("properQuit", [](const params&) { utils::nt::raise_hard_exception(); });

    add("echo", [](const params& params) {
      for (auto i = 1; i < params.size(); i++) {
        game::Com_Printf(game::CON_CHANNEL_DONT_FILTER, "%s ", params.get(i));
      }

      game::Com_Printf(game::CON_CHANNEL_DONT_FILTER, "\n");
    });

    game::Cmd_RemoveCommand("vstr");
    add("vstr", cmd_vstr_f);
  }
};
} // namespace command

REGISTER_COMPONENT(command::component)
