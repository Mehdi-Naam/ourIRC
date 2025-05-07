#pragma once

#include "../ICommands.hpp"
#include "../../Global.hpp"

class Bot : public ICommands
{

public:
    void execute();

private:
    void parseJsonValue(const std::string &key , std::string &value);
    void parseDriver();
    void printUserDetails();
    void getData(std::string &login);
    std::vector<std::string> _hold;
    std::string  _data;
    std::string token;
    std::string _displayName;
    std::string _login;
    std::string  _correction_point;
    std::string  _level;
    std::string  _email;
    std::string  _wallet;
    std::string  _location;
    std::string  _nickname;
    std::string  _user;
    std::string  _ip;
    std::string _botName;


};
