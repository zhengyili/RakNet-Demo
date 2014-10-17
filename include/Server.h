/* 
 * File:   Server.h
 * Author: Dreamszhu
 *
 * Created on October 17, 2014, 9:21 AM
 */

#ifndef SERVER_H
#define	SERVER_H

#include <iostream>

#include "raknet/MessageIdentifiers.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/RakNetTypes.h"

class Server {
public:
	Server();
	Server(const Server& orig);
	virtual ~Server();
	
	void run();
private:

};

#endif	/* SERVER_H */

