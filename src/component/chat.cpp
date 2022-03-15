#include <stdinc.hpp>

#include "loader/component_loader.hpp"
#include "utils/hook.hpp"
#include "utils/string.hpp"

#include "command.hpp"

namespace chat {
namespace {
std::mutex chat_mutex;
std::unordered_set<std::uint64_t> mute_list{};

void mute_player(const game::client_s* cl) {
  std::unique_lock<std::mutex> _(chat_mutex);

  if (mute_list.contains(cl->xuid)) {
    game::SV_GameSendServerCommand(
        -1, game::SV_CMD_CAN_IGNORE,
        utils::string::va("%c \"%s is already muted\"", 0x65, cl->name));
    return;
  }

  mute_list.insert(cl->xuid);
}

void unmute_player(const game::client_s* cl) {
  std::unique_lock<std::mutex> _(chat_mutex);

  mute_list.erase(cl->xuid);

  game::SV_GameSendServerCommand(
      cl->gentity->entnum, game::SV_CMD_CAN_IGNORE,
      utils::string::va("%c \"You were unmuted\"", 0x65));
}

void client_command(int client_number) {
  char buf[1024] = {0};

  if (game::g_entities[client_number].client == nullptr) {
    // Not in game
    return;
  }

  game::SV_Cmd_ArgvBuffer(0, buf, sizeof(buf));

  std::unique_lock<std::mutex> _(chat_mutex);
  if (utils::string::starts_with(buf, "say") &&
      mute_list.contains(game::svs_clients[client_number].xuid)) {
    game::SV_GameSendServerCommand(
        client_number, game::SV_CMD_CAN_IGNORE,
        utils::string::va("%c \"You are muted\"", 0x65));
    return;
  }

  game::ClientCommand(client_number);
}
} // namespace

class component final : public component_interface {
public:
  void post_unpack() override {
    utils::hook::call(SELECT(0x58DA1C, 0x4FB3BD), client_command);

    add_chat_commands();
  }

private:
  static void add_chat_commands() {
    command::add("sayAs", [](const command::params& params) {
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

    command::add("mutePlayer", [](const command::params& params) {
      if (params.size() < 2) {
        game::Com_Printf(game::CON_CHANNEL_DONT_FILTER,
                         "Usage: mutePlayer <client number>\n");
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

    command::add("unmutePlayer", [](const command::params& params) {
      if (params.size() < 2) {
        game::Com_Printf(game::CON_CHANNEL_DONT_FILTER,
                         "Usage: unmutePlayer <client number>\n");
        return;
      }

      const auto* client = game::SV_GetPlayerByNum();

      if (client == nullptr)
        return;

      assert(client->gentity != nullptr);

      if (client->gentity->client == nullptr)
        return;

      unmute_player(client);
    });
  }
};
} // namespace chat

REGISTER_COMPONENT(chat::component)
