#pragma once 

#include "../../Global.hpp"
#define CRNL "\r\n"

namespace utils
{
	void		trim(std::string & ,char sp);
	void		display(std::string const &, std::string const &);
	void		exit_msg(std::string const &);
	bool		exit_msg_b(std::string const &);
	void		remove_rcnl(std::string &);
	bool		itsUpper(std::string &);
	size_t		find_first_sp(std::string const &);
	void		splitUsingSep(std::string, std::vector<std::string> &, char);
	void		reply(std::string const &);
	bool		checkTrueName(std::string, char);
	std::string getToken();
	std::string getData(std::string &data);
	bool		protectPassword(std::string const & pass);
	bool     	pars_port( std::string& port);
	size_t 		stringToSize_t(std::string port);
	int			StringToInt(const std::string& str);
	bool 		protectPort(string port);
	string		IntToString(int num);
}



