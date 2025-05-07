#pragma once

#include "../ICommands.hpp"
#include "../../Global.hpp"

class ModeCommand : public ICommands
{

public:
    void execute();

private:
    void parseDriver();
    void changeMode();
    void userLimit( std::string args);
    void changeAdmin(const std::string &args);
    void changeKey(std::string &args);
	void displayTheChannelMode();

private:
    std::vector<std::string>	_hold;
    bool						_adding;
    Channel						*channel;
    std::string					_nickName;
};