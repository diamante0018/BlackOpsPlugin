#include <std_include.hpp>

#include <utils/string.hpp>
#include <utils/nt.hpp>

#include "command.hpp"

constexpr auto CMD_MAX_NESTING = 8;

namespace command {
std::unordered_map<std::string, sv_command_param_function> handlers;

game::CmdArgs* get_cmd_args() {
  return static_cast<game::CmdArgs*>(
      game::Sys_GetValue(game::THREAD_VALUE_CMD));
}

void main_handler() {
  params_sv params;

  const auto command = utils::string::to_lower(params[0]);

  if (const auto got = handlers.find(command); got != handlers.end()) {
    got->second(params);
  }
}

params::params() : nesting_(get_cmd_args()->nesting) {
  assert(this->nesting_ < CMD_MAX_NESTING);
}

int params::size() const { return get_cmd_args()->argc[this->nesting_]; }

const char* params::get(const int index) const {
  if (index >= this->size()) {
    return "";
  }

  return get_cmd_args()->argv[this->nesting_][index];
}

std::string params::join(const int index) const {
  std::string result;

  for (auto i = index; i < this->size(); i++) {
    if (i > index)
      result.append(" ");
    result.append(this->get(i));
  }
  return result;
}

params_sv::params_sv() : nesting_(game::sv_cmd_args->nesting) {
  assert(game::sv_cmd_args->nesting < CMD_MAX_NESTING);
}

int params_sv::size() const { return game::sv_cmd_args->argc[this->nesting_]; }

const char* params_sv::get(const int index) const {
  if (index >= this->size()) {
    return "";
  }

  return game::sv_cmd_args->argv[this->nesting_][index];
}

std::string params_sv::join(const int index) const {
  std::string result;

  for (auto i = index; i < this->size(); ++i) {
    if (i > index)
      result.append(" ");
    result.append(this->get(i));
  }
  return result;
}

void add_raw(const char* name, void (*callback)()) {
  game::Cmd_AddCommandInternal(
      name, game::Cbuf_AddServerText_f,
      utils::memory::get_allocator()->allocate<game::cmd_function_s>());
  game::Cmd_AddServerCommandInternal(
      name, callback,
      utils::memory::get_allocator()->allocate<game::cmd_function_s>());
}

void add_sv(const char* name, const sv_command_param_function& callback) {
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
} // namespace command
