#include "../Global.hpp"
#include "Server.hpp"
#include "client/Client.hpp"
#include "socket/Socket.hpp"
#include <netinet/in.h>
#include <string>
#include <sstream>
#include <sys/_types/_socklen_t.h>
#include <vector>

chn_map &Server::getChannels()
{
	return channels;
}

voc_cli &Server::get_vec_cli()
{
	return _all_clinet;
}

void Server::set_password(std::string password)
{ //=> set password of server from argv
	this->_password = password;
}

std::vector<struct pollfd> &Server::get_pl_fd()
{ //=> get pollfd of server and client
	return pl_fd;
}

const std::string Server::get_password() const
{ //=> get password
	return this->_password;
}

int Server::get_port() const
{ //=> get port allowed from server to client
	return this->_port;
}

Server::Server() {} //=> constractor of server

Server::~Server() {} //=> Distractor of server

void Server::close_fd(const Socket &soc)
{

	if (soc.get_fdsock() != -1)
	{ //=> close the server socket
		std::cout << RED << "Server <" << soc.get_fdsock() << "> Disconnected" << WHI << std::endl;
		close(soc.get_fdsock());
	}
	for (voc_cli::iterator itre = _all_clinet.begin(); itre != _all_clinet.end();)
	{ //=> close fd of client
		std::cout << RED << "Client <" << itre->second->get_fd() << "> Disconnected" << WHI << std::endl;
		close(itre->second->get_fd());
		delete itre->second;
		itre->second = NULL;
		itre = _all_clinet.erase(itre);
	}
	for (chn_map_it it = channels.begin(); it != channels.end();) {
		it = channels.erase(it);
	}
}

void Server::erase_client(int fd)
{

	eraseCltFromChannel();

	for (size_t i = 0; i < pl_fd.size(); i++)
	{ //=> remove pollfd of client from vector of pollfd
		if (pl_fd[i].fd == fd)
		{
			pl_fd.erase(pl_fd.begin() + i);
			close(fd); //=> close the socket client
			break;
		}
	}
	for (voc_cli::iterator itre = _all_clinet.begin(); itre != _all_clinet.end(); itre++)
	{ //=> remove client from vector of client
		if (itre->second->get_fd() == fd)
		{
			delete itre->second;
			itre->second = NULL;
			_all_clinet.erase(itre);
			close(fd); //=> close the socket client
			break;
		}
	}
}

//=> constractor of server set password and port
Server::Server(const std::string &password, int port) : _port(port), _password(password) {
}

std::string genName(int incofd)
{

	std::stringstream conv;

	conv << incofd;
	std::string name("cli_" + conv.str());
	return name;
}

void Server::Accept_clinet(const Socket &soc)
{
	struct sockaddr_in Clinet_add;
	struct pollfd pl_client;
	Client *Cli = new Client; //=> create a new client
	socklen_t len = sizeof(sockaddr_in);

	int acc_fd = accept(soc.get_fdsock(), (sockaddr*)&Clinet_add, &len); //=> accept new client
	if (acc_fd == -1)
	{
		std::cout << "Error: accept has faile" << std::endl;
		return;
	}
	pl_client.fd = acc_fd;	   //=> add the client socket to pollfd
	pl_client.events = POLLIN; //=> set event to POLLIN for read data
	pl_client.revents = 0;	   //=> set revent to 0

	Cli->setNick("");
	Cli->set_fd(acc_fd);											//=> set the client fd
	Cli->set_ip(inet_ntoa(Clinet_add.sin_addr));					//=> convert the ip address of client to string and set it
	pl_fd.push_back(pl_client);									//=> add the client socket to vector of pollfd
	_all_clinet.push_back(std::make_pair(Cli->getNick(), Cli)); //=> add the client socket to vector of client

	std::cout << GRE << "Client <" << acc_fd << "> Connnected" << WHI << std::endl;
}

void Server::sig_handler(int sig)
{

	(void)sig;
	std::cout << LOO << "Signal Received!" << WHI << std::endl;
	_sig = true; //=> set the flag to true to stop server
}

void Server::New_data(int &fds)
{
	storeCurrentClt(fds);

	char buff[MAX_BUFF]; //=> buffer for the received data

	memset(&buff, 0, sizeof(buff)); //=> clear the buffer

	size_t byte = recv(fds, buff, sizeof(buff), 0); //=> read the data of client and received

	if (byte <= 0)
	{	//=> check if the client has Disconnnected
		std::cout << RED << "Client <" << fds << "> Disconnnected" << WHI << std::endl;
		erase_client(fds); //=> erase the client
	}
	else
	{
		_buff = _buff + buff;
		if (_buff[_buff.size() - 1] == '\n')
		{
			std::vector<string> _hold;
			utils::splitUsingSep(_buff,_hold, '\n');
			for (size_t i = 0 ; i < _hold.size(); i++)
				ircCommandDriver(_hold[i]);
			_buff.clear();
			_hold.clear();
		}
	}
}

void Server::set_server()
{

	Socket sock(_port);
	sock.set_sock_srv(); //=>create socket of server

	std::cout << GRE << OUR_SERVER << WHI << std::endl;
	std::cout << GRE << "Server <" << sock.get_fdsock() << "> connected" << WHI << std::endl;
	while (Server::_sig == false)
	{ //=> run the server unitil now sigenl

		int status = 0;
		status = poll(&pl_fd[0], pl_fd.size(), 0); //=> wait for event
		if (status == -1 && Server::_sig == false)
			throw std::runtime_error("Error: poll faild");
		for (size_t i = 0; i < pl_fd.size(); i++)
		{ //=> loop in vector of pollfd
			if (pl_fd[i].revents & POLLIN)
			{ //=> check if there is data to read or new client
				if (pl_fd[i].fd == sock.get_fdsock())
				{
					Accept_clinet(sock); //=> accept new client
				}
				else
					New_data(pl_fd[i].fd); //=> receive data from a registered client
			}
		}
	}
	close_fd(sock); //=> close the fd when the server stop
}




Server *Server::CreatServer(const std::string &password, std::string port)
{
	if (!_Serv)
		_Serv = new Server(password, std::atoi(port.c_str())); //=> create a new INSTANCE
	return _Serv;
}

Server *Server::get_server()
{

	if (!_Serv)
		utils::exit_msg("SERVER INSTANCE NOT FOUND");
	return _Serv;
}

void Server::storeCurrentClt(int const &fd)
{

	for (voc_cli::iterator i = _all_clinet.begin(); i != _all_clinet.end(); i++)
	{
		if (i->second->get_fd() == fd)
		{
			curClt_it = i;
			curClt = i->second;
			return;
		}
	}
	utils::exit_msg("fd for Current Clt Not Founded");
}

voc_cli::iterator Server::getCurrCltIt()
{
	return curClt_it;
}

Client *Server::getCurrClt()
{
	return curClt;
}

Channel *Server::getChannel(const std::string &name)
{
	chn_map_it it = channels.find(name);
	if (it != channels.end())
		return &it->second;

	return NULL;
}

Client *Server::getClient(const std::string &nick)
{
	for (size_t i = 0; i < _all_clinet.size(); i++)
	{
		if (_all_clinet[i].second->getNick() == nick)
			return _all_clinet[i].second;
	}

	return NULL;
}

void Server::eraseCltFromChannel()
{

	// boocle on all Channels
	for (chn_map_it i = channels.begin(); i != channels.end();)
	{
		i->second.eraseClient();
		if (i->second.getClients().size() == 0)
		{
			std::cout << "size of clients is 0" << std::endl;
			i = channels.erase(i);
		}
		else
			i++;
	}
}

void Server::destroyInstanceServer()
{

	if (!_Serv)
		return;

	delete _Serv;
	_Serv = NULL;
}