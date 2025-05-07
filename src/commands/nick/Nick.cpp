#include "Nick.hpp"

bool    NickCommand::client_nik() {

    voc_cli::iterator itb = Server::get_server()->get_vec_cli().begin();
    voc_cli::iterator ite = Server::get_server()->get_vec_cli().end();
    for (voc_cli::iterator it = itb; it != ite; it++) {
        if (it->first == Nick)
            return false;
    }
    return true;
}

std::string check_user() {

	std::string user = Server::get_server()->getCurrClt()->getUser();
	if (user.empty())
		user = "relname";
	return user;
}

void NickCommand::execute() {
    Nick = parsser::getInstence()->getParameter();
	if (Nick.empty()) {
		utils::reply(ERR_NONICKNAMEGIVEN);
		utils::exit_msg("");
	}

	std::string user = check_user();
    if (client_nik() == true && utils::checkTrueName(Nick, 'n') ) {

		std::string old_nick = Server::get_server()->getCurrClt()->getNick();

        Server::get_server()->getCurrCltIt()->second->setNick(Nick);
        Server::get_server()->getCurrCltIt()->first = Nick;

		if (!old_nick.empty())
			utils::reply(RPL_NICK(old_nick, Server::get_server()->getCurrClt()->getUser(), Server::get_server()->getCurrClt()->get_ip(), Nick));

		std::cout << "NICK  : " << GRE << "Done." << WHI << std::endl;

		if (Server::get_server()->getCurrClt()->get_auth() < M_REGISTRE)
			Server::get_server()->getCurrClt()->set_auth(M_NICK + Server::get_server()->getCurrClt()->get_auth());
    }
	else if (client_nik() == false) {

		utils::reply(ERR_NICKNAMEINUSE(Nick, user));
		utils::exit_msg("");
	}
	else {
		utils::reply(ERR_ERRONEUSNICKNAME(Nick, user));
		utils::exit_msg("");
	}
}