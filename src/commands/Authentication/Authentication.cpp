#include "Authentication.hpp"

void	authentication() {
	int hold_auth = Server::get_server()->getCurrClt()->get_auth();
	if (hold_auth >= M_REGISTRE)
		return;

	int hold_cmd = parsser::getInstence()->getCommand();

	if (hold_auth < M_PASS) {
		if (hold_cmd != CmdSpace::PASS) {
			utils::reply(ERR_NOTREGISTERED);
			utils::exit_msg("");
		}
	}
	else if (hold_auth >= M_PASS) {
		if (hold_cmd != CmdSpace::NICK && hold_cmd != CmdSpace::USER && hold_cmd != CmdSpace::PASS) {
			utils::reply(ERR_NOTREGISTERED);
			utils::exit_msg("");
		}
	}
}
