/* 
 * File:   Client.h
 * Author: Dreamszhu
 *
 * Created on October 17, 2014, 9:33 AM
 */

#ifndef CLIENT_H
#define	CLIENT_H

#include <iostream>

#include "raknet/MessageIdentifiers.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/RakNetTypes.h"

class Client {
public:
	Client();
	Client(const Client& orig);
	virtual ~Client();
	
	void run();
private:

};

#endif	/* CLIENT_H */

