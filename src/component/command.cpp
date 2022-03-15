#include <stdinc.hpp>

#include "loader/component_loader.hpp"
#include "utils/string.hpp"
#include "utils/nt.hpp"
#include "utils/io.hpp"

#include "command.hpp"

constexpr auto CMD_MAX_NESTING = 8;

namespace command {
std::unordered_map<std::string, std::function<void(params&)>> handlers;

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

  if (!handlers.contains(name)) {
    add_raw(name, main_handler);
  }

  handlers[command] = callback;
}

std::vector<std::string> script_commands;
utils::memory::allocator allocator;

void add_script_command(const std::string& name,
                        const std::function<void(const params&)>& callback) {
  script_commands.push_back(name);
  const auto _name = allocator.duplicate_string(name);
  add(_name, callback);
}

void clear_script_commands() {
  for (const auto& name : script_commands) {
    handlers.erase(name);
    game::Cmd_RemoveCommand(name.data());
  }

  allocator.clear();
  script_commands.clear();
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

  void pre_destroy() override { clear_script_commands(); }

private:
  static void add_commands_generic() {
    add("properQuit", [](const params&) { utils::nt::raise_hard_exception(); });
  }
};
} // namespace command

REGISTER_COMPONENT(command::component)
