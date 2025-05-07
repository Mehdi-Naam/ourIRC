#ifndef CLEINT_HPP
#define CLEINT_HPP

#include <string>

class Client {

	public:
		Client ();
		~Client();
		Client (const Client&);
		Client&			operator=(const Client&);
	
		std::string		get_ip();

		int				get_fd();
		int				get_flag();
		int				&get_auth();
	
		void			set_fd(int);
		void			set_flag(int);
		void			set_auth(int);
		void			set_ip(std::string );
		
		std::string	const &	getNick() const;
		std::string	const &	getUser() const;
		std::string	const &	getRealName() const;

		void		 	setNick(std::string const & );
		void		 	setUser(std::string const & );
		void		 	setRealName(std::string const & );
	
	private:
		int 			_flag;
		int				_auth;
		int				_fd_client;
		std::string		_ip_client;
		std::string		_nick;
		std::string		_user;
		std::string		_realName;

};

#endif
