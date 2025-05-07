#include "../Global.hpp"
#include "utils/Utils.hpp"

void check_is_registre()
{

	int hold_auth = Server::get_server()->getCurrClt()->get_auth();

	if (hold_auth == M_REGISTRE)
	{

		utils::reply(RPL_WELCOME(Server::get_server()->getCurrClt()->getNick()));
		utils::reply(RPL_YOURHOST(Server::get_server()->getCurrClt()->getNick()));
		utils::reply(OUS_SERVER);
		Server::get_server()->getCurrClt()->set_auth(M_REGISTRE + 1);
	}
}


void ircCommandDriver(std::string buffer)
{
	try
	{
		parsser::CreatInstence(buffer)->parse_line();
		authentication();
		CommandFactory::createCommand();
		check_is_registre();
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
	parsser::destroyInstance();
}

/*
	FUNCTION USEFUL TO GET THE COMMAND::ENUM
*/

CmdSpace::CmdType CmdSpace::getCommandEnum(std::string const &cmd)
{

	int lvl;
	lvl = (cmd == "PASS") ? PASS : (cmd == "NICK")	? NICK
							   : (cmd == "USER")	? USER
							   : (cmd == "MODE")	? MODE
							   : (cmd == "KICK")	? KICK
							   : (cmd == "JOIN")	? JOIN
							   : (cmd == "TOPIC")	? TOPIC
							   : (cmd == "INVITE")	? INVITE
							   : (cmd == "PRIVMSG") ? PRIVMSG
							   : (cmd == "PONG") 	? PONG
							   : (cmd == "QUIT") 	? QUIT
							   : (cmd == "BOT")	? BOT
													: CMDNOTFOUND;

	return static_cast<CmdType>(lvl);
}

/*
 ***  PARSSER METHODES ---
 */

parsser *parsser::singIns = NULL;

parsser::parsser(std::string const &d) : _buff(d)
{
}

parsser::~parsser()
{
}

parsser *parsser::CreatInstence(std::string data)
{

	if (!singIns)
		singIns = new parsser(data);

	return singIns;
}

parsser *parsser::getInstence()
{

	if (!singIns)
		utils::exit_msg("Warning No Insatance in Parsser exist");
	return singIns;
}

void parsser::destroyInstance()
{

	if (!singIns)
		return;

	delete singIns;
	singIns = NULL;
}

void make_it_upper(std::string &line)
{

	for (int i = 0; line[i]; i++)
		line[i] = (char)toupper(line[i]);
}

void parsser::token_cmd()
{

	std::string cmd;
	size_t p_sp = _buff.find(' ');
	cmd = _buff.substr(0, p_sp);
	if (cmd.empty())
		utils::exit_msg("");
	make_it_upper(cmd);
	scmd = cmd;
	_cmd = CmdSpace::getCommandEnum(cmd);
}

void parsser::token_parameters()
{

	size_t skip_cmd = _buff.find(' ');
	size_t skip_spc = _buff.find_first_not_of(' ', skip_cmd);

	if (skip_spc != std::string::npos)
		_parameters = _buff.substr(skip_spc, std::string::npos);
	if (_cmd != CmdSpace::PASS)
		utils::trim(_parameters,' ');
}

void parsser::parse_line()
{

	utils::remove_rcnl(_buff);
	token_cmd();
	token_parameters();
}

CmdSpace::CmdType parsser::getCommand() const
{
	return _cmd;
}

std::string parsser::getParameter() const
{
	return _parameters;
}

std::string const &parsser::getscmd()
{
	return scmd;
}