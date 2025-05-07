#include "Client.hpp"
#include <iostream>
Client::Client() { //=> constractor of client
	_auth = -1;
	_flag = 0;
} 

void	Client::set_fd(int fd) { //=> set fd of client
	_fd_client = fd;
}

void	Client::set_auth(int auth) {
	_auth = auth;
}

int		&Client::get_auth() {
	return _auth;
}

void	Client::set_ip(std::string ip) { //=> set ip of client
	_ip_client = ip;
}

int		Client::get_fd() { //=> get fd of client
	return _fd_client;
}

int		Client::get_flag() { //=> get fd of client
	return _flag;
}

std::string		Client::get_ip() { //=> get ip of client
	return _ip_client;
}

Client &Client::operator=(const Client& opr) { //=> opr = of client

	if (this != &opr) {
		_nick = opr._nick;
		_user = opr._user;
		_auth = opr._auth;
		_flag = opr._flag;
		_realName = opr._realName;
		_fd_client = opr._fd_client;
		_ip_client = opr._ip_client;
	}
	return *this;
}

Client::Client(const Client& cpy) { //=> copy constractor
	*this = cpy;
}

Client::~Client() {
	std::cout << "the clt: " << _fd_client << " : killed" << std::endl;
} //=> distractor of client

std::string const	&	Client::getNick() const { //=> get Nick
	return _nick;
}

std::string const	&	Client::getUser() const { //=> get Nick
	return _user;
}

std::string const	&	Client::getRealName() const { //=> get Nick
	return _realName;
}

void		Client::setNick(std::string const & n){
	_nick = n;
}
void		Client::setUser(std::string const & u){
	_user = u;
}

void		Client::setRealName(std::string const & r){
	_realName = r;
}

void		Client::set_flag(int flag){
	_flag = flag;
}