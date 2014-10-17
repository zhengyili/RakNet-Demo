/* 
 * File:   Server.cpp
 * Author: Dreamszhu
 * 
 * Created on October 17, 2014, 9:21 AM
 */

#include "Server.h"
#include "raknet/BitStream.h"

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

	RakNet::Packet *packet;
	unsigned char typeId;

	while (1) {
		packet = server->Receive();
		if (packet) {
			RakNet::BitStream stream(packet->data, packet->length, false);

			stream.Read(typeId);
			switch (typeId) {
				case ID_USER_PACKET_ENUM:
				{
					RakNet::RakString msg;
					stream.Read(msg);
					std::cout << "Receive msg:" << msg << std::endl;

					RakNet::BitStream sendStream;
					sendStream.Write((RakNet::MessageID)ID_USER_PACKET_ENUM);
					sendStream.Write(RakNet::RakString("Welcome!"));

					uint32_t sends = server->Send(&sendStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					std::cout << "send client msg:" << sends << std::endl;
					break;
				}
				default:
					break;
			}

			server->DeallocatePacket(packet);
		}		
	}
	
	RakNet::RakPeerInterface::DestroyInstance(server);
}

