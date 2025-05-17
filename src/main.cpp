#include "Global.hpp"

bool	Server::_sig  = false;
Server*	Server::_Serv = NULL;

void handle_all_signals() {
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, Server::sig_handler);
	signal(SIGQUIT, Server::sig_handler);
}

void freeUpTheInstance() {
	try  {
		Server::get_server()->destroyInstanceServer();
	}
    catch(const std::exception& e) {
    }
    std::cout << "The Server Closed!" << std::endl;
}

bool validNumArgs(int ac, std::string const exec) {

	if (ac == 3)
		return true;
	std::cout << "Usage: " << exec << " <port number> <password>" << std::endl;
	return false;
}

void RunTheprogram(string pass, string port) {

	try
    {
		std::cout << ":::: SERVER ::::" << std::endl;
		handle_all_signals();
        Server::CreatServer(pass, port)->set_server();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

bool argsValid(string port ,string pass) {
	if (!utils::protectPort(port) || !utils::protectPassword(pass))
		return false;
	return true;
}

int main(int ac, char **av)
{
	if (!validNumArgs(ac, av[0]))
		return 1;

	if (!argsValid(av[1], av[2]))
		return 1;

	RunTheprogram(av[2], av[1]);
	freeUpTheInstance();

	return 0;
}