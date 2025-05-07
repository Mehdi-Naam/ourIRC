#pragma once


#include "../ICommands.hpp"
#include "../../server/Server.hpp"
#include "../../Global.hpp"

typedef std::vector<std::string>	vec_str;


class KickCommand : public ICommands {

	public:
		KickCommand();
		void execute();

	private:
		void prasseDriver();
		void tokenChannels();
		void tokenUsers();
		void tokenComment();

		void checkChannel();
		// void checkAlreadyKicked();
		void checkUsersIsInChannel();

		void kickTheUsers();

	private:
		std::string			_currNick;

		std::string			_parameter;
		std::string			_comment;
		std::string			_channel;

		vec_str				_users;
		chn_map_it			_it_channel;
};