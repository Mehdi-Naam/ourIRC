#pragma once
#include "../Global.hpp"
#include <string>

/*
 *** Class Give Only One Instance 'Singleton Class'
 */
class parsser
{

public:
	static parsser *getInstence();
	static parsser *CreatInstence(std::string data);
	static void destroyInstance();

	CmdSpace::CmdType getCommand() const;
	std::string getParameter() const;
	void parse_line();

	std::string const &getscmd();

private:
	void token_cmd();
	void token_parameters();

private:
	static parsser *singIns;

	CmdSpace::CmdType _cmd;

	std::string scmd;

	std::string _parameters;
	std::string _buff;

private:
	~parsser();
	parsser(std::string const &);
	parsser(const parsser &);
	parsser &operator=(const parsser &);
};

void ircCommandDriver(std::string buffer);
