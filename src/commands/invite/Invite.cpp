#include "Invite.hpp"

void InviteCommand::parseDriver()
{
    std::string data = parsser::getInstence()->getParameter();
    utils::splitUsingSep(data, _hold, ' ');
    _nickName = Server::get_server()->getCurrClt()->getNick();
}

void InviteCommand::sendInvitation(std::string &user)
{
    Client *invitingClient = Server::get_server()->getClient(user);
    if (invitingClient == NULL)
    {
        utils::reply(ERR_NOSUCHNICK(Server::get_server()->getCurrClt()->getNick(), user));
        return;
    }
    channel->addInvitation(invitingClient);
}

void InviteCommand::execute()
{
    parseDriver();
    if (_hold.size() == 2)
    {
        channel = Server::get_server()->getChannel(_hold[1]);
        if (channel != NULL)
        {
            channel->checkCltIsAdmin();
            sendInvitation(_hold[0]);
        }
        else
            utils::reply(ERR_NOSUCHCHANNEL(_nickName, _hold[0]));
    }
    else
    {
        utils::reply(ERR_NEEDMOREPARAMS(_nickName, "INVITE"));
    }
}