#pragma once
#include "../CommandFactory.hpp"
#include "../../server/client/Client.hpp"
#include "../../server/Server.hpp"
#include "../Authentication/Authentication.hpp"

class PassCommand: public ICommands {
	public:

		void execute();
};