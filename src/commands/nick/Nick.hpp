#pragma once
#include "../CommandFactory.hpp"
#include "../../Global.hpp"
#include "../Authentication/Authentication.hpp"

class NickCommand: public ICommands {
	private:
		std::string Nick;
		int			check_nick();
		bool client_nik();

	public:

		void execute();
};