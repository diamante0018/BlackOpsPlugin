#include <stdinc.hpp>

#include "loader/component_loader.hpp"

namespace chat
{
	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{

		}
	};
}

REGISTER_COMPONENT(chat::component)
