
#pragma once

#include "../ICommands.hpp"
#include "../../Global.hpp"

class Channel;

class InviteCommand : public ICommands
{

public:
    void execute();

private:
    void parseDriver();
    void sendInvitation(std::string &user);
    std::vector<std::string> _hold;
    Channel *channel;
    std::string _channel;
    std::string _nickName;
    std::string _user;
};
