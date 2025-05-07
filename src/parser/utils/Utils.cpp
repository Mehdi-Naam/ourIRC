#include "Utils.hpp"
#include <cctype>
#include <iostream>

void utils::remove_rcnl(std::string &buff)
{

	size_t i = 0;
	if (buff.find('\n') != std::string::npos)
		i++;
	if (buff.find(CRNL) != std::string::npos)
		i++;
	else if (buff.find('\r') != std::string::npos)
		i++;
	buff = buff.substr(0, buff.length() - i);
}

void utils::trim(std::string &line, char sp)
{

	std::string::size_type start = line.find_first_not_of(sp);
	if (start == std::string::npos)
		return;

	std::string::size_type end = line.find_last_not_of(sp);

	line = line.substr(start, end - start + 1);
}

void utils::display(std::string const &cmd, std::string const &param)
{

	std::cout << "users:  '" << cmd << "'" << std::endl;
	std::cout << "param:  '" << param << "'" << std::endl;
}

bool utils::itsUpper(std::string &line)
{

	for (int i = 0; line[i]; i++)
		if (!std::isalpha(line[i]) || std::islower(line[i]))
			return false;
	;
	return true;
}

void utils::reply(std::string const &replay)
{
	int fd = Server::get_server()->getCurrClt()->get_fd();
	if (send(fd, replay.c_str(), replay.size(), 0) == -1)
		std::cerr << "sendin::replay::send() Failed" << std::endl;
}

size_t utils::find_first_sp(std::string const &line)
{

	size_t first_sp = line.find(' ');

	if (first_sp == std::string::npos)
		first_sp = line.find('\t');

	return first_sp;
}

static void checkLineErrors(std::string const &line, char const &sep)
{

	if (line.at(line.size() - 1) == ',')
		utils::exit_msg("ERROR: parameter ',' syntaxError");
	size_t pos, prev, start = 0;
	while ((pos = line.find(sep, start)) != std::string::npos)
	{
		if (pos - prev == 1 && sep == ',')
			utils::exit_msg("ERROR: parameter ',,' syntaxError");
		prev = pos;
		start = pos + 1;
	}
}

void utils::splitUsingSep(std::string line, std::vector<std::string> &holder, char sep)
{

	if (line.empty())
		return;

	checkLineErrors(line, sep);
	std::string subted;
	size_t start = 0;
	size_t end = 0;

	while ((end = line.find(sep, start)) != std::string::npos)
	{

		subted = line.substr(start, end - start);
		holder.push_back(subted);
		start = line.find_first_not_of(sep, end);
	}
	if (start >= line.size())
		return;
	subted = line.substr(start, end - start);
	if (!(subted = line.substr(start, end - start)).empty())
		holder.push_back(subted);
}

void utils::exit_msg(std::string const &_line)
{

	std::string line = _line;
	if (!line.empty())
		line = _line + "\n";
	throw std::runtime_error(line);
}

bool utils::exit_msg_b(std::string const &_line)
{

	std::string line = _line;
	if (!line.empty())
		line = _line + "\n";
	std::cout << line << std::flush;
	return false;
}

bool utils::checkTrueName(std::string name, char type)
{

	if (type == 'c')
		name.erase(0, 1);
	if (std::isdigit(name[0]))
		return false;


	for (size_t i = 0; i < name.size(); i++)
	{
		if (!std::isalnum(name[i]) && name[i] != '_')
			return false;
	}
	return true;
}

std::string utils::getToken()
{

	std::string command = "curl -s -X POST --data \"grant_type=client_credentials&client_id=";
	command += UID;
	command += "&client_secret=";
	command += SECRET;
	command += _URL_TOKEN;
	command += " > /tmp/token.txt";
	std::system(command.c_str());

	std::ifstream file(TOKEN_FILE);
	std::stringstream buffer;

	if (file)
	{
		buffer << file.rdbuf();
		file.close();
	}

	std::remove(TOKEN_FILE);
	std::string token;
	std::vector<std::string> _hold;

	size_t i = buffer.str().find('"');
    size_t l = buffer.str().find(',', i);
    token = buffer.str().substr(i, l - i);
	 utils::splitUsingSep(token,_hold,':');
    token.clear();
    token = _hold[1];
    utils::trim(token,'"');
	return token;
}

bool utils::protectPassword(std::string const & pass) {
	if (pass.empty())
		return (utils::exit_msg_b("ERROR: EMPTY PASSWORD"));

	for (size_t i = 0; i < pass.size(); i++)
		if (pass[i] <= 32)
			return (utils::exit_msg_b("ERROR: INVALID PASSWORD"));

	return true;
}

bool utils::pars_port( std::string& port) {

    utils::trim(port,' ');
	if (port.empty())
		return false;
    for (size_t i = 0; i < port.size(); i++) {
        if (!std::isdigit(port[i]) || i > 4)
            return false;
    }
    return true;
}

size_t  utils::stringToSize_t(std::string port) {
	std::stringstream con (port);
	size_t num;
	if (!(con >> num))
		exit(1);
	return num;
}

bool utils::protectPort(string port) {

	if (!pars_port(port))
		return (utils::exit_msg_b("ERROR: INVALID PORT"));
	size_t _port = stringToSize_t(port);
	if  (_port <= 1023)
		return (utils::exit_msg_b("ERROR: Reserved Ports!"));
    if (_port > 65535)
        return (utils::exit_msg_b("ERROR: OUT OF MAX"));
	return true;
}

int utils::StringToInt(const std::string& str) {
 
    std::stringstream ss(str);
    long long num = 0;
    ss >> num;
	if (ss.fail())
		return 0;
    if (num < 0 || num > 2147483647 )
        return -1;

    return static_cast<int>(num);
}

string utils::IntToString(int num) {

	std::stringstream con;
	con << num;
	return con.str();
}
