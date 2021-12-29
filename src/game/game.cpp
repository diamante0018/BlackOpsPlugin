#include <stdinc.hpp>

namespace game
{
	gamemode current = reinterpret_cast<const char*>(0xA6840C) == "multiplayer"s
		? gamemode::multiplayer
		: gamemode::zombies;

	namespace environment
	{
		bool t6mp()
		{
			return current == gamemode::multiplayer;
		}

		bool t6zm()
		{
			return current == gamemode::zombies;
		}
	}
}
