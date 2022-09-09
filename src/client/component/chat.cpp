#include <std_include.hpp>
#include "../loader/component_loader.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/concurrency.hpp>

#include "command.hpp"

namespace chat {
namespace {
utils::hook::detour client_command_hook;

using client_list = std::unordered_set<std::uint64_t>;
utils::concurrency::container<client_list> mute_list;

const game::dvar_t* sv_disableChat;

void mute_player(const game::client_s* client) {
  const auto xuid = client->xuid;
  mute_list.access([&](client_list& clients) { clients.insert(xuid); });
}

void unmute_player(const game::client_s* client) {

  const auto xuid = client->xuid;
  mute_list.access([&](client_list& clients) { clients.erase(xuid); });

  game::SV_GameSendServerCommand(
      client->gentity->entnum, game::SV_CMD_CAN_IGNORE,
      utils::string::va("%c \"You were unmuted\"", 0x65));
}

void client_command_stub(const int client_number) {
  char buf[1024]{};

  if (game::g_entities[client_number].client == nullptr) {
    // Not in game
    return;
  }

  game::SV_Cmd_ArgvBuffer(0, buf, sizeof(buf));

  if (utils::string::starts_with(buf, "say")) {
    if (sv_disableChat->current.enabled) {
      game::SV_GameSendServerCommand(
          client_number, game::SV_CMD_CAN_IGNORE,
          utils::string::va("%c \"Chat is disabled\"", 0x65));
      return;
    }

    const auto is_muted =
        mute_list.access<bool>([&](const client_list& clients) {
          return clients.contains(game::svs_clients[client_number].xuid);
        });

    if (is_muted) {
      game::SV_GameSendServerCommand(
          client_number, game::SV_CMD_CAN_IGNORE,
          utils::string::va("%c \"You are muted\"", 0x65));
      return;
    }
  }

  client_command_hook.invoke<void>(client_number);
}
} // namespace

class component final : public component_interface {
public:
  void post_unpack() override {
    client_command_hook.create(SELECT_VALUE(0x63DB70, 0x4AF770),
                               client_command_stub);
    add_chat_commands();

    sv_disableChat = game::Dvar_RegisterBool(
        "sv_disableChat", false, 0, "Disable chat messages from clients");
  }

private:
  static void add_chat_commands() {
    command::add("sayAs", [](const command::params_sv& params) {
      if (params.size() < 3) {
        game::Com_Printf(game::CON_CHANNEL_DONT_FILTER,
                         "Usage: sayAs <client number> <message>\n");
        return;
      }

      const auto* client = game::SV_GetPlayerByNum();

      if (client == nullptr)
        return;

      auto* const gentity = client->gentity;
      assert(gentity != nullptr);

      if (gentity->client == nullptr)
        return;

      const auto message = params.join(2);
      game::G_Say(gentity, nullptr, 0, message.data());
    });

    command::add("muteClient", [](const command::params_sv& params) {
      if (params.size() < 2) {
        game::Com_Printf(game::CON_CHANNEL_DONT_FILTER,
                         "Usage: %s <client number> : prevent the player from "
                         "using the chat\n",
                         params.get(0));
        return;
      }

      const auto* client = game::SV_GetPlayerByNum();

      if (client == nullptr)
        return;

      assert(client->gentity != nullptr);

      if (client->gentity->client == nullptr)
        return;

      mute_player(client);
    });

    command::add("unmute", [](const command::params_sv& params) {
      if (params.size() < 2) {
        game::Com_Printf(game::CON_CHANNEL_DONT_FILTER,
                         "Usage: %s <client number>\n", params.get(0));
        return;
      }

      const auto* client = game::SV_GetPlayerByNum();

      if (client == nullptr) {
        if (std::strcmp(params.get(1), "all") == 0) {
          mute_list.access([&](client_list& clients) { clients.clear(); });
        }
        return;
      }

      assert(client->gentity != nullptr);

      if (client->gentity->client == nullptr)
        return;

      unmute_player(client);
    });
  }
};
} // namespace chat

REGISTER_COMPONENT(chat::component)
