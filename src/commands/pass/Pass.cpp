#include "Pass.hpp"

void PassCommand::execute() {

    std::string pas_cli = parsser::getInstence()->getParameter();
    std::string pas_ser = Server::get_server()->get_password();

	if (pas_cli.empty()) {
		utils::reply(ERR_NEEDMOREPARAMS(Server::get_server()->getCurrClt()->getNick(), "PASS"));
		utils::exit_msg("");
	}

    if (pas_cli == pas_ser && !Server::get_server()->getCurrClt()->get_flag()) {
        std::cout << "PASS  : " << GRE << "Done." << WHI << std::endl;
		Server::get_server()->getCurrClt()->set_auth(M_PASS);
		Server::get_server()->getCurrClt()->set_flag(1);
    }
	else if (Server::get_server()->getCurrClt()->get_flag() >= 1){
		utils::reply(ERR_ALREADYREGISTRED(Server::get_server()->getCurrClt()->getNick()));
		utils::exit_msg("");
	}
	else {

		utils::reply(ERR_PASSWDMISMATCH);
		utils::exit_msg("");
	}

}