#include <std_include.hpp>

namespace game {
gamemode current = reinterpret_cast<const char*>(0xA6840C) == "multiplayer"s
                       ? gamemode::multiplayer
                       : gamemode::zombies;

namespace environment {
bool is_mp() { return current == gamemode::multiplayer; }

bool is_sp() { return current == gamemode::zombies; }
} // namespace environment
} // namespace game
