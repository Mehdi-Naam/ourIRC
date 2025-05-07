#include "Bot.hpp"

void Bot::parseJsonValue(const std::string &key, std::string &value)
{

    size_t i = _data.find(key);
    if (i == std::string::npos || !value.empty())
        return;
    size_t l = _data.find(':');
    std::string tmp = _data.substr(l + 1);
    utils::trim(tmp, '"');
    value = tmp;
}
void Bot::parseDriver()
{

    token = utils::getToken();
    if (token == "invalid_client")
    {
        utils::reply(RPL_PRIVMSG(_botName, _user, _ip, _nickname, "invalid_client"));
        return;
    }
    else
    {
        std::string args = parsser::getInstence()->getParameter();

        utils::splitUsingSep(args, _hold, ' ');
        if (_hold.size() != 1)
        {
            utils::reply(RPL_PRIVMSG(_botName, _user, _ip, _nickname, "try :usage => BOT <42login>"));
            return;
        }
        else
            getData(args);
    }
}

void Bot::getData(std::string &login)
{
    // curl  -H "Authorization: Bearer token" "https://api.intra.42.fr/v2/users/ayel-fil"
    std::string command = "curl -s -H \"Authorization: Bearer ";
    command += token;
    command += _URL_INFO;
    command += login;
    command += " > /tmp/info.txt";

    std::system(command.c_str());

    std::ifstream file(INFO_FILE);
    std::string buffer;

    if (file)
    {
        while (getline(file, _data, ','))
        {
            if (_data == "{}")
            {
                utils::reply(RPL_PRIVMSG(_botName, _user, _ip, _nickname, "try :invalid 42 login"));
                file.close();
                std::remove(INFO_FILE);

                return;
            }
            parseJsonValue("\"login\":", _login);
            parseJsonValue("\"displayname\"", _displayName);
            parseJsonValue("\"correction_point\":", _correction_point);
            parseJsonValue("\"level\":", _level);
            parseJsonValue("\"email\":", _email);
            parseJsonValue("\"wallet\":", _wallet);
            parseJsonValue("\"location\":", _location);
        }
    }
    file.close();
    std::remove(INFO_FILE);

    printUserDetails();
}

void Bot::printUserDetails()
{
    std::stringstream to_send_1;
    std::stringstream to_send;
    utils::reply(RPL_PRIVMSG(_botName, _user, _ip, _nickname, ":User Profile:"));

    to_send << ": | "
            << "Login: " << _login << " | "
            << "Display Name: " << _displayName << " | "
            << "Correction Points: " << _correction_point;
    to_send_1 << ": | "
              << "Level: " << _level << " | "
              << "Email: " << _email << " | "
              << "Location: " << _location << " | "
              << "Wallet: " << _wallet;

    utils::reply(RPL_PRIVMSG(_botName, _user, _ip, _nickname, to_send.str()));
    utils::reply(RPL_PRIVMSG(_botName, _user, _ip, _nickname, to_send_1.str()));
}

void Bot::execute()
{
    _user = Server::get_server()->getCurrClt()->getUser();
    _ip = Server::get_server()->getCurrClt()->get_ip();
    _nickname = Server::get_server()->getCurrClt()->getNick();
    _botName = "/42BOT";
    parseDriver();
}