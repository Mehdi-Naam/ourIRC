#include "CommandFactory.hpp"
#include <exception>
#include "../Global.hpp"


 void   CommandFactory::createCommand()
{
    parsser *Parser = parsser::getInstence();
    CmdSpace::CmdType data = Parser->getCommand();
    ICommands *cmd = NULL;

    switch (data)
    {
    case CmdSpace::PRIVMSG:
        cmd = new PrvmsgCommand();
        break;
    case CmdSpace::PASS:
        cmd = new PassCommand();
        break;
    case CmdSpace::NICK:
        cmd = new NickCommand();
        break;
    case CmdSpace::USER:
        cmd = new UserCommand();
        break;
    case CmdSpace::MODE:
        cmd = new ModeCommand();
        break;
    case CmdSpace::KICK:
        cmd = new KickCommand();
        break;
    case CmdSpace::TOPIC:
        cmd = new TopicCommand();
        break;
    case CmdSpace::JOIN:
        cmd = new JoinCommand();
        break;
    case CmdSpace::INVITE:
        cmd = new InviteCommand();
		break;
    case CmdSpace::BOT:
        cmd = new Bot();
        break;
    case CmdSpace::PONG:
		  break ;
    case CmdSpace::QUIT:
		  break ;
    default:
		  utils::reply(ERR_UNKNOWNCOMMAND);
   
    }

	try {
		if (cmd)
			cmd->execute();	
	} catch (std::exception & e) {
		std::cout << e.what();
	}
    delete cmd ;
}
