#pragma once

#include "client/Client.hpp"
#define RED "\e[1;31m"
#define BLU "\e[0;34m"
#define BLE "\e[1;34m"
#define LOO "\e[1;35m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

#include <poll.h>	   //=> for poll
#include <vector>	   //=> for vector
#include <arpa/inet.h> //=> for inet_ntoa
#include <unistd.h>	   //=> for close
#include <map>		   //=> for continre of map

class Channel;
class Client;
class Socket;

typedef std::vector<std::pair<std::string, Client *> > voc_cli;
typedef std::map<std::string, Channel> chn_map;
typedef std::vector<struct pollfd> vec_pfd;

typedef chn_map::iterator chn_map_it;

class Server
{

public:
	static Server *CreatServer(const std::string &password, std::string port);
	static Server *get_server();
	static void sig_handler(int sig);

	const std::string get_password() const;

	int get_port() const;

	void set_password(std::string password);
	void set_server();
	void Accept_clinet(const Socket &);
	void New_data(int &);
	void close_fd(const Socket &soc);
	void erase_client(int fd);

	chn_map &getChannels();
	Channel *getChannel(const std::string &);
	Client *getClient(const std::string &);
	vec_pfd &get_pl_fd();
	voc_cli &get_vec_cli();

	void storeCurrentClt(int const &);
	Client *getCurrClt();
	voc_cli::iterator getCurrCltIt();

	void	eraseCltFromChannel();

	void	destroyInstanceServer();

private:
	Server();
	~Server();
	Server(const std::string &password, int port);
	Server &operator=(const Server &);
	Server(const Server &);

	static bool _sig;
	static Server *_Serv;

	int _port;
	std::string _password;

	voc_cli _all_clinet;
	vec_pfd pl_fd;
	chn_map channels;
	Client *curClt;
	std::string _buff;

	voc_cli::iterator curClt_it;
};
