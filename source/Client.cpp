/* 
 * File:   Client.cpp
 * Author: Dreamszhu
 * 
 * Created on October 17, 2014, 9:33 AM
 */

#include "Client.h"

Client::Client() {
}

Client::Client(const Client& orig) {
}

Client::~Client() {
}

void Client::run() {
	unsigned short serverPort = 6666;

	RakNet::RakPeerInterface *client = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor(0, 0);

	client->Startup(1, &socketDescriptor, 1);

	bool ret = (client->Connect("127.0.0.1", serverPort, NULL, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED);
	
	RakAssert(ret);
	
	RakNet::Packet *packet;
	bool connected = false;

	while (1) {
		packet = client->Receive();
		if (packet) {
			if (packet->data[0] == ID_CONNECTION_REQUEST_ACCEPTED) {
				connected = true;
				std::cout << "connect success" << std::endl;

				char flag = ID_USER_PACKET_ENUM;
				char buffer[] = " hello world\n";

				buffer[0] = flag;

				uint32_t sends = client->Send((const char*)buffer, strlen(buffer) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				std::cout << "send return:" << sends << std::endl;
			}
			if (!connected) {
				std::cout << "connect failed" << std::endl;
				client->Connect("127.0.0.1", 30050, NULL, 0, 0);
			}
		}
	}

}

