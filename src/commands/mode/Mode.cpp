#include "Mode.hpp"
#include <cstddef>
#include <ostream>

void ModeCommand::parseDriver()
{
    std::string data = parsser::getInstence()->getParameter();
    _nickName = Server::get_server()->getCurrClt()->getNick();

    if (data.empty())
    {
        utils::reply(ERR_NEEDMOREPARAMS(_nickName, "MODE"));
        utils::exit_msg("");
    }
    utils::splitUsingSep(data, _hold, ' ');
}

void ModeCommand::userLimit(std::string args)
{
    if (_adding)
    {
        if (args.empty())
        {
            utils::reply(ERR_NEEDMOREPARAMS(_nickName, "MODE"));
            return;
        }
        channel->setUserLimit(utils::StringToInt(args));
    }
    else
        channel->setUserLimit(0); /* the _user_limit is size_t type !!! */
}

void ModeCommand::changeKey(std::string &args)
{

    if (_adding)
    {
        if (args.empty())
        {
            utils::reply(ERR_NEEDMOREPARAMS(_nickName, "MODE"));
            return;
        }
        if (args.at(0) == ':')
            args = args.substr(1);
        channel->setChannelKey(args);
    }
    else
        channel->setChannelKey("");
}

void ModeCommand::changeAdmin(const std::string &args)
{
    if (args.empty())
    {
        utils::reply(ERR_NEEDMOREPARAMS(_nickName, "MODE"));
        return;
    }

    Client *user = Server::get_server()->getClient(args);
    if (user == NULL)
    {
        utils::reply(ERR_USERNOTINCHANNEL(Server::get_server()->getCurrClt()->getNick(), args, channel->getNameChannel()));
        return;
    }
    if (_adding)
    {
        channel->addOperator(user);
    }
    else
    {
        channel->removeOperator(user);
    }
}

void ModeCommand::displayTheChannelMode()
{

    channel = Server::get_server()->getChannel(_hold[0]);
    if (channel == NULL)
    {
        utils::reply(ERR_NOSUCHCHANNEL(_nickName, _hold[0]));
        return;
    }

    std::cout << YEL "genStringOfChannelModes:" WHI << std::flush;
    channel->genStringOfChannelModes();
    std::cout << GRE "DONE" WHI << std::endl;
}

void ModeCommand::execute()
{
    parseDriver();
	channel = Server::get_server()->getChannel(_hold[0]);
	if (channel == NULL){
		utils::reply(ERR_NOSUCHCHANNEL(_nickName, _hold[0]));
		return;
	}
	channel->checkCltIsAdmin();

    if (_hold.size() >= 2)
		changeMode();
    else if (_hold.size() == 1)
        displayTheChannelMode();
}

void ModeCommand::changeMode()
{
    std::string mode = _hold[1];

    if (!(mode[0] == '+' || mode[0] == '-'))
    {
        utils::reply(ERR_UMODEUNKNOWNFLAG(_nickName));
        return;
    }

    _adding = (mode[0] == '+');
    std::string modesToChange = mode.substr(1);
    std::size_t argIndex = 2; // 0: channel, 1: modes, 2+: arguments

    for (std::size_t i = 0; i < modesToChange.size(); ++i)
    {
        char modeChar = modesToChange[i];

        if ( ( _adding && (modeChar == 'k' || modeChar == 'l') ) || modeChar == 'o')
        {
            if (argIndex >= _hold.size())
            {
                utils::reply(ERR_NEEDMOREPARAMS(_nickName, "MODE"));
                continue; 
            }
        }

        switch (modeChar)
        {
        case 'i':
            channel->setInviteOnlyMode(_adding);
            break;
        case 't':
            channel->setTopicChangeMode(_adding);
            break;
        case 'k':
            changeKey(_hold[argIndex++]);
            break;
        case 'l':
            userLimit(_hold[argIndex++]);
            break;
        case 'o':
            changeAdmin(_hold[argIndex++]);
            break;
        case 's':
            break;
        case 'n':
            break;
        default:
            utils::reply(ERR_UNKNOWNMODE(_nickName, modeChar));
            break;
        }
    }
}