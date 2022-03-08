#include <stdinc.hpp>

namespace game {
gamemode current = reinterpret_cast<const char*>(0xA6840C) == "multiplayer"s
                       ? gamemode::multiplayer
                       : gamemode::zombies;

namespace environment {
bool t5mp() { return current == gamemode::multiplayer; }

bool t5zm() { return current == gamemode::zombies; }
} // namespace environment
} // namespace game
