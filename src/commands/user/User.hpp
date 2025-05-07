#pragma once
#include "../CommandFactory.hpp"
#include "../../Global.hpp"
#include "../Authentication/Authentication.hpp"


typedef std::vector<std::string> vec_s ;

class UserCommand: public ICommands {

	public:
		void execute();
	private:
		void _span();
	private:
		vec_s			_hold;

		size_t			span;
		std::string		Usr;
		std::string		_user;
		std::string		_realname;
};