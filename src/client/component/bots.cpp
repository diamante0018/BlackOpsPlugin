#include <std_include.hpp>

#include "../loader/component_loader.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>

namespace bots {
namespace {
using bot_entry = std::pair<std::string, std::string>;
std::vector<bot_entry> bot_names;
utils::hook::detour sv_bot_name_random_hook;

// Json file is expected to contain a key for the bot's name. Value should be a
// string for the clantag
void load_bot_data() {
  if (!utils::io::file_exists("bots/bots.json")) {
    game::Com_Printf(game::CON_CHANNEL_SERVER, "bots.json was not found\n");
    return;
  }

  rapidjson::Document obj;
  const rapidjson::ParseResult result =
      obj.Parse(utils::io::read_file("bots/bots.json").data());

  if (!result || !obj.IsObject()) {
    game::Com_Printf(game::CON_CHANNEL_SERVER,
                     "Failed to parse ban file. Empty?\n");
    return;
  }

  for (rapidjson::Value::ConstMemberIterator itr = obj.MemberBegin();
       itr != obj.MemberEnd(); ++itr) {
    if (itr->value.GetType() == rapidjson::Type::kStringType) {
      bot_names.emplace_back(
          std::make_pair(itr->name.GetString(), itr->value.GetString()));
    }
  }
}

const char* sv_bot_name_random_stub() {
  static auto loaded = false;

  if (bot_names.empty() && !loaded) {
    load_bot_data();
    loaded = true;
  }

  if (!bot_names.empty()) {
    static size_t bot_id = 0;
    bot_id %= bot_names.size();
    const auto& entry = bot_names.at(bot_id++);
    return entry.first.data();
  }

  return sv_bot_name_random_hook.invoke<const char*>();
}

int build_connect_string(char* buf, const char* connect_string,
                         const char* name, const char* xuid, int protocol,
                         int port) {
  // Default
  auto clan_tag = "3arc"s;
  for (const auto& [bot_name, tag] : bot_names) {
    if (bot_name == name) {
      // Found their clantag
      clan_tag = tag;
      break;
    }
  }

  return _snprintf_s(buf, 0x400, _TRUNCATE, connect_string, name,
                     clan_tag.data(), xuid, protocol, port);
}
} // namespace

class component final : public component_interface {
public:
  void post_unpack() override {
    if (game::current == game::gamemode::zombies)
      return;

    utils::hook::set<const char*>(
        0x6B6294,
        "connect "
        "\"\\cg_predictItems\\1\\cl_punkbuster\\0\\cl_"
        "anonymous\\0\\color\\4\\head\\"
        "default\\model\\multi\\snaps\\20\\rate\\5000\\name\\%s\\clanAbbrev\\%"
        "s\\bdOnlineUserID\\%s\\protocol\\%d\\qport\\%d\"");

    sv_bot_name_random_hook.create(0x49ED80, &sv_bot_name_random_stub);
    utils::hook::call(0x6B6299, build_connect_string);
  }
};
} // namespace bots

REGISTER_COMPONENT(bots::component)
