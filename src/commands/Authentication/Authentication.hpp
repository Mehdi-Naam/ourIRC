#ifndef AUTH_HPP
#define AUTH_HPP

#include "../../Global.hpp"

#define	 M_PASS CmdSpace::PASS
#define	 M_NICK CmdSpace::NICK
#define	 M_USER CmdSpace::USER
#define  M_REGISTRE M_NICK + M_USER + M_PASS


void	authentication();

#include "../CommandFactory.hpp"

#endif