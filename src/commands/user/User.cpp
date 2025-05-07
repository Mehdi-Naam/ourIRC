#include "User.hpp"
#include <cstddef>
#include <vector>


void UserCommand::_span() {

	span = std::string::npos;

	for (size_t i = 0; i < _hold.size(); i++)
		if (_hold[i][0] == ':')
			span = i;
}

void UserCommand::execute() {

	Usr = parsser::getInstence()->getParameter();
	if (Usr.empty()) {
		utils::reply(ERR_NEEDMOREPARAMS(Server::get_server()->getCurrClt()->getNick(), "USER"));
		utils::exit_msg("");
	}
	utils::splitUsingSep(Usr, _hold, ' ');
	_span();

	if (span == std::string::npos && _hold.size() == 4) {

		_user		 	= _hold[0];
		_realname		= _hold[_hold.size() - 1];
	}
	else if (span + 1 == 4) {
		_user		 	= _hold[0];

		size_t pos = Usr.find(':', 0);
		_realname = Usr.substr(pos + 1);
	}
	else {
		utils::reply(ERR_NEEDMOREPARAMS(Server::get_server()->getCurrClt()->getNick(), "USER"));
		utils::exit_msg("");

	}
	Server::get_server()->getCurrCltIt()->second->setUser(_user);
	Server::get_server()->getCurrCltIt()->second->setRealName(_realname);
	if (Server::get_server()->getCurrClt()->get_flag() == 1){
		Server::get_server()->getCurrClt()->set_auth(M_USER + Server::get_server()->getCurrClt()->get_auth());
		Server::get_server()->getCurrClt()->set_flag(2);
	}
	else {
		utils::reply(ERR_ALREADYREGISTRED(Server::get_server()->getCurrClt()->getNick()));
		utils::exit_msg("");
	}

	std::cout << "USER  : " << GRE << "Done." << WHI << std::endl;
}