#include <stdinc.hpp>

#include "loader/component_loader.hpp"
#include "utils/hook.hpp"

namespace ban {
namespace {
const game::dvar_t* sv_discord = nullptr;
const game::dvar_t* sv_clanWebsite = nullptr;

bool out_of_band_print_hk(game::netsrc_t src, game::netadr_s to,
                          const char* msg) {
  // Proof of concept patch. Please ignore
  if (msg != "error\nPATCH_BANNED_FROM_SERVER"s) {
    return game::NET_OutOfBandPrint(src, to, msg);
  }

  const auto error_msg =
      std::format("error\nPermanently banned\nDiscord: {}\nWebsite: {}",
                  sv_discord->current.string, sv_clanWebsite->current.string);

  return game::NET_OutOfBandPrint(src, to, error_msg.data());
}
} // namespace

class component final : public component_interface {
public:
  void post_unpack() override {
    if (game::current == game::gamemode::zombies)
      return;

    sv_discord = game::Dvar_RegisterString(
        "sv_discord", "https://www.discord.gg/", game::DVAR_ARCHIVE,
        "Discord invitation link");

    sv_clanWebsite =
        game::Dvar_RegisterString("sv_clanWebsite", "https://www.google.com/",
                                  game::DVAR_ARCHIVE, "Website link");

    utils::hook::call(0x48B7E2, out_of_band_print_hk);
  }
};
} // namespace ban

REGISTER_COMPONENT(ban::component)
