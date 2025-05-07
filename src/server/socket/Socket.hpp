#pragma once

#include "../../Global.hpp"

//=> class for socket
class Socket {

	public:
		Socket();
		Socket(int port);
		int				get_port() const;
		int				get_fdsock() const;
		void			set_sock_srv();

	private:
		int			_port;
		int			_fd_sock;
		Socket&		operator=(const Socket&);
		Socket(const Socket&);
};

