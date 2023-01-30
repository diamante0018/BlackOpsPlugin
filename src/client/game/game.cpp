#include <std_include.hpp>

namespace game {
gamemode current =
    std::memcmp(reinterpret_cast<const char*>(0xA6840C), "multiplayer", 12) == 0
        ? gamemode::multiplayer
        : gamemode::zombies;

namespace environment {
bool is_mp() { return current == gamemode::multiplayer; }

bool is_sp() { return current == gamemode::zombies; }
} // namespace environment
} // namespace game
