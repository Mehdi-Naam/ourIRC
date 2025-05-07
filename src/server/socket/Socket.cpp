#include "Socket.hpp"
#include <netdb.h>
#include <sstream>
#include <iostream>

Socket::Socket() {
} //=> constractor of socket

Socket::Socket(int port) : _port(port), _fd_sock() {} //=> constractor of socket set port

std::string conver(int port) { //=> convert port from int to string

	std::stringstream prt;
	prt << port;
	std::string conv = prt.str();
	return conv;
}

void	condition_sock(int &_fd_sock, const addrinfo *result) {
	
	int			i = 1;

	_fd_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);  //=> create socket of server

	if (_fd_sock == -1) //=> check if the socket is created
		throw (std::runtime_error("Error: socket has faile"));

	if (setsockopt(_fd_sock, SOL_SOCKET , SO_REUSEADDR, &i, sizeof(i)) == -1) //=> set the socket option SO_REUSEADDR to reuse the seam port
		throw (std::runtime_error("Error: setsocketopt has faile"));

	if (fcntl(_fd_sock, F_SETFL, O_NONBLOCK)) //=> set the socket option O_NONBLOCK to non-blocking socket
		throw (std::runtime_error("Error: fcntl has faile"));

	if (bind(_fd_sock, result->ai_addr, result->ai_addrlen) == -1) //=> bind the socket by the address and port number specified in ai_addr
		throw (std::runtime_error("Error: bind has faile"));

	if (listen(_fd_sock, SOMAXCONN) == -1) //=> listen for incoming conncetions and make the socket a passive socket
		throw (std::runtime_error("Error: listen has faile"));
}

void	Socket::set_sock_srv() {
	
	addrinfo	hint;
	addrinfo*	result = NULL;
	struct		pollfd pl_soc;
	std::string conv = conver(_port);

	memset(&hint, 0, sizeof(hint)); //=> clear the hint
	hint.ai_family = AF_INET; //=> initialize the ai_family to IPV4
	hint.ai_socktype = SOCK_STREAM; //=> initialize the ai_socktype to TCP
	hint.ai_flags = AI_NUMERICSERV | AI_PASSIVE;

	int	ret = getaddrinfo(NULL, conv.c_str(), &hint, &result); //=> initialize the all of address info
	if (ret != 0) { //=> check if the address info has initialized
		std::cout << "getaddrinfo: " << gai_strerror(ret) << "." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	try {
        condition_sock(_fd_sock, result);
    } catch(std::exception & e) {
		close(_fd_sock);
		freeaddrinfo(result);
        throw (std::runtime_error(e.what()));
    }

	pl_soc.fd = _fd_sock;  //=> add the server socket to pollfd
	pl_soc.events = POLLIN; //=> set events to POLLIN for reading data
	pl_soc.revents = 0; //=> set the revents to 0
	Server::get_server()->get_pl_fd().push_back(pl_soc); //=> add the server socket to verctor ok pollfd

	freeaddrinfo(result);
}

int	Socket::get_port() const { //=> get the port
	return _port;
}

int		Socket::get_fdsock() const { //=> get socket
	return _fd_sock;
}
