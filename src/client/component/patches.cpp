#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/structs.hpp"
#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace patches {
namespace {
int client_num_;
std::string status_;

void server_status_basic_print([[maybe_unused]] const int channel,
                               const char* fmt) {
  status_.append(fmt);
}

void server_status_map_print([[maybe_unused]] const int channel,
                             const char* fmt, const char* map) {
  status_.append(utils::string::va(fmt, map));
}

void server_status_header_print([[maybe_unused]] const int channel,
                                [[maybe_unused]] const char* fmt) {
  status_.append("num score bot ping guid   name            "
                 "lastmsg address               qport rate\n");
}
void server_status_body_print([[maybe_unused]] const int channel,
                              [[maybe_unused]] const char* fmt) {
  status_.append("--- ----- --- ---- ---------- --------------- ------- "
                 "--------------------- ------ -----\n");
}

void server_status_client_number_print([[maybe_unused]] const int channel,
                                       const char* fmt, const int client_num) {
  client_num_ = client_num;
  status_.append(utils::string::va(fmt, client_num));
}

void server_status_score_print([[maybe_unused]] const int channel,
                               const char* fmt, const int score) {
  status_.append(utils::string::va(fmt, score));
  const int fake_player =
      game::SV_IsTestClient(client_num_) || game::SV_IsDemoClient(client_num_);
  status_.append(utils::string::va("%3i ", fake_player));
}

void server_status_ping_print([[maybe_unused]] const int channel,
                              const char* fmt, const int ping) {
  status_.append(utils::string::va(fmt, ping));
}

void server_status_guid_print([[maybe_unused]] const int channel,
                              const char* fmt, const int guid) {
  status_.append(utils::string::va(fmt, guid));
}

void server_status_name_print([[maybe_unused]] const int channel,
                              const char* fmt, const char* name) {
  status_.append(utils::string::va(fmt, name));
}

void server_status_last_msg_print([[maybe_unused]] const int channel,
                                  const char* fmt, const int last_msg) {
  status_.append(utils::string::va(fmt, last_msg));
}

void server_status_address_print([[maybe_unused]] const int channel,
                                 const char* fmt, const char* address) {
  status_.append(utils::string::va(fmt, address));
}

void server_status_qport_print([[maybe_unused]] const int channel,
                               const char* fmt, const int qport) {
  status_.append(utils::string::va(fmt, qport));
}

void server_status_rate_print([[maybe_unused]] const int channel,
                              const char* fmt, const int rate) {
  status_.append(utils::string::va(fmt, rate));
}

void server_status_terminating_print([[maybe_unused]] const int channel,
                                     const char* fmt) {
  status_.append(fmt); // final new line
  game::Com_Printf(game::CON_CHANNEL_DONT_FILTER, "%s", status_.data());

  // clear the buffer
  status_.clear();
}
} // namespace

class component final : public component_interface {
public:
  void post_unpack() override {
    if (game::environment::is_sp()) {
      return;
    }

    utils::hook::call(0x8760E2, server_status_map_print);

    utils::hook::call(0x8760EE, server_status_header_print);

    utils::hook::call(0x8760FA, server_status_body_print);

    utils::hook::call(0x876131, server_status_client_number_print);

    utils::hook::call(0x876165, server_status_score_print);

    utils::hook::call(0x87617B, server_status_basic_print); // connecting print

    utils::hook::call(0x876191, server_status_basic_print); // zombie print

    utils::hook::call(0x8761B5, server_status_ping_print);

    utils::hook::call(0x8761CB, server_status_guid_print);

    utils::hook::call(0x8761DE, server_status_name_print);

    utils::hook::call(0x876207, server_status_basic_print); // space print

    utils::hook::call(0x876227, server_status_last_msg_print);

    utils::hook::call(0x876251, server_status_address_print);

    utils::hook::call(0x876287, server_status_basic_print); // space print

    utils::hook::call(0x87629F, server_status_qport_print);

    utils::hook::call(0x8762B2, server_status_rate_print);

    utils::hook::call(0x8762BE, server_status_basic_print); // new line print

    utils::hook::call(0x8762E2, server_status_terminating_print);

    utils::hook::call(0x8762E2, server_status_terminating_print);
  }
};
} // namespace patches

REGISTER_COMPONENT(patches::component)
