/* 
 * File:   Server.cpp
 * Author: Dreamszhu
 * 
 * Created on October 17, 2014, 9:21 AM
 */

#include "Server.h"

Server::Server() {
}

Server::Server(const Server& orig) {
}

Server::~Server() {
}

void Server::run() {
	unsigned int maxConnectionsAllowed = 2;
	unsigned int maxPlayersPerServer = 2;
	unsigned short serverPort = 6666;

	RakNet::RakPeerInterface *server = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor(serverPort, 0);

	bool ret = server->Startup(maxConnectionsAllowed, &socketDescriptor, 1) == RakNet::RAKNET_STARTED;

	RakAssert(ret);

	server->SetMaximumIncomingConnections(maxPlayersPerServer);

	while (1) {
		RakNet::Packet *packet = server->Receive();
		if (packet) {
			switch (packet->data[0]) {
				case ID_USER_PACKET_ENUM:
					std::cout << "Receive data:\n" << packet->data << std::endl;
					break;
				default:
					break;
			}

		}
		server->DeallocatePacket(packet);
	}
}

