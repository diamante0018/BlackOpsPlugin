#include <stdinc.hpp>

#include "loader/component_loader.hpp"

#include "command.hpp"

namespace chat
{
	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			add_chat_commands();
		}

	private:
		static void add_chat_commands()
		{
			command::add("sayAs", [](const command::params& params)
			{
				if (params.size() < 3)
				{
					game::Com_Printf(game::CON_CHANNEL_DONT_FILTER,
						"Usage: sayAs <client number>\n");
					return;
				}

				const auto* client = game::SV_GetPlayerByNum();

				if (client == nullptr)
					return;

				auto* gentity = client->gentity;
				assert(gentity != nullptr);

				if (gentity->client == nullptr)
					return;

				const auto message = params.join(2);
				game::G_Say(gentity, nullptr, 0, message.data());
			});
		}
	};
}

REGISTER_COMPONENT(chat::component)
