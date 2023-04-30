#include <std_include.hpp>
#include "loader/component_loader.hpp"

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

void send_message(const int client_num, const char* msg) {
  game::Cbuf_AddText(game::LOCAL_CLIENT_0,
                     utils::string::va("tell %i \"%s\"\n", client_num, msg));
}

void mute_player(const game::client_s* client) {
  const auto xuid = client->xuid;
  mute_list.access([&xuid](client_list& clients) { clients.insert(xuid); });

  send_message(client->gentity->entnum, "You were muted");
}

void unmute_player(const game::client_s* client) {

  const auto xuid = client->xuid;
  mute_list.access([&xuid](client_list& clients) { clients.erase(xuid); });

  send_message(client->gentity->entnum, "You were unmuted");
}

void client_command_stub(const int client_number) {
  char buf[1024]{};

  if (!game::g_entities[client_number].client) {
    // Not in game
    return;
  }

  game::SV_Cmd_ArgvBuffer(0, buf, sizeof(buf));

  if (utils::string::starts_with(buf, "say")) {
    if (sv_disableChat->current.enabled) {
      send_message(client_number, "Text chat is disabled");
      return;
    }

    const auto is_muted =
        mute_list.access<bool>([&](const client_list& clients) {
          return clients.contains(game::svs_clients[client_number].xuid);
        });

    if (is_muted) {
      send_message(client_number, "You are muted");
      return;
    }
  }

  client_command_hook.invoke<void>(client_number);
}
} // namespace

class component final : public component_interface {
public:
  void post_unpack() override {
    client_command_hook.create(game::select(0x63DB70, 0x4AF770),
                               client_command_stub);
    add_chat_commands();

    sv_disableChat = game::Dvar_RegisterBool(
        "sv_disableChat", false, 0, "Disable chat messages from clients");
  }

private:
  static void add_chat_commands() {
    command::add_sv("muteClient", [](const command::params_sv& params) {
      if (params.size() < 2) {
        game::Com_Printf(game::CON_CHANNEL_DONT_FILTER,
                         "Usage: %s <client number> : prevent the player from "
                         "using the chat\n",
                         params.get(0));
        return;
      }

      const auto* client = game::SV_GetPlayerByNum();
      if (!client) {
        return;
      }

      assert(client->gentity);

      if (!client->gentity->client) {
        return;
      }

      mute_player(client);
    });

    command::add_sv("unmute", [](const command::params_sv& params) {
      if (params.size() < 2) {
        game::Com_Printf(game::CON_CHANNEL_DONT_FILTER,
                         "Usage: %s <client number>\n", params.get(0));
        return;
      }

      const auto* client = game::SV_GetPlayerByNum();
      if (!client) {
        if (std::strcmp(params.get(1), "all") == 0) {
          mute_list.access([](client_list& clients) { clients.clear(); });
        }
        return;
      }

      assert(client->gentity);

      if (!client->gentity->client) {
        return;
      }

      unmute_player(client);
    });
  }
};
} // namespace chat

REGISTER_COMPONENT(chat::component)
