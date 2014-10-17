/* 
 * File:   Voice.cpp
 * Author: Dreamszhu
 * 
 * Created on October 17, 2014, 10:52 AM
 */

#include <../4.8.2/iostream>

#include "Voice.h"

Voice::Voice() {
	err = Pa_Initialize();	
	rakPeer = RakNet::RakPeerInterface::GetInstance();
}

Voice::~Voice() {
	Pa_Terminate();
	RakNet::RakPeerInterface::DestroyInstance(rakPeer);
}

static int c_callback(void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		PaTimestamp outTime, void *userData) {
	Voice *voice = (Voice*) userData;
	return voice->portAudioCallback(inputBuffer, outputBuffer);
}

void Voice::run() {
	if (err != paNoError) {
		std::cerr << "Pa_Initialize fail:" << Pa_GetErrorText(err) << Pa_GetHostError() << std::endl;
		return;
	}
	
	mute = false;

	PortAudioStream *stream;

	unsigned int maxConnectionsAllowed = 4;
	unsigned int maxPlayersPerServer = 4;
	unsigned short serverPort = 6000;

	RakNet::SocketDescriptor socketDescriptor(serverPort, 0);

	if (rakPeer->Startup(maxConnectionsAllowed, &socketDescriptor, 1) != RakNet::RAKNET_STARTED) {
		std::cerr << "Startup fail:" << std::endl;		
		return;
	}

	rakPeer->SetMaximumIncomingConnections(maxPlayersPerServer);

	rakPeer->AttachPlugin(&rakVoice);
	rakVoice.Init(8000, (2048 / (32000 / 8000)) * sizeof (short));

	PaError err = Pa_OpenStream(&stream,
			Pa_GetDefaultInputDeviceID(), 1, paInt16, NULL, Pa_GetDefaultOutputDeviceID(), 1, paInt16, NULL,
			8000, (2048 / (32000 / 8000)), 0, 0, c_callback, 0);

	if (err != paNoError) {
		std::cerr << "Pa_OpenStream fail:" << Pa_GetErrorText(err) << std::endl;
		return;
	}

	err = Pa_StartStream(stream);

	if (err != paNoError) {
		return;
	}

	RakNet::Packet *packet;
	unsigned char typeId;

	while (1) {
		char ip[256];
		std::cout << "Enter IP of remote system: " << std::endl;
		std::cin.getline(ip, sizeof(ip));
		if (ip[0] == 0)
			strcpy(ip, "127.0.0.1");

		rakPeer->Connect(ip, serverPort, 0, 0);

		packet = rakPeer->Receive();
		while (packet) {
			RakNet::BitStream stream(packet->data, packet->length, false);

			stream.Read(typeId);
			switch (typeId) {
				case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					rakVoice.RequestVoiceChannel(packet->guid);
					break;
				}
				case ID_CONNECTION_ATTEMPT_FAILED:
					break;
				case ID_RAKVOICE_OPEN_CHANNEL_REQUEST:
					break;
				case ID_RAKVOICE_OPEN_CHANNEL_REPLY:
					break;
				case ID_NAT_TARGET_NOT_CONNECTED:
				case ID_NAT_TARGET_UNRESPONSIVE:
				case ID_NAT_CONNECTION_TO_TARGET_LOST:
				case ID_NAT_ALREADY_IN_PROGRESS:
				{
					RakNet::RakNetGUID guid;
					stream.Read(guid);
					break;
				}
				case ID_NAT_PUNCHTHROUGH_FAILED:
				case ID_NAT_PUNCHTHROUGH_SUCCEEDED:
				case ID_ALREADY_CONNECTED:
				case ID_RAKVOICE_CLOSE_CHANNEL:
				case ID_DISCONNECTION_NOTIFICATION:
				case ID_NEW_INCOMING_CONNECTION:
					break;
			}

			rakPeer->DeallocatePacket(packet);
			packet = rakPeer->Receive();
		}

		Pa_Sleep(30);
	}
}

int Voice::portAudioCallback(void *inputBuffer, void *outputBuffer) {

	if (inputBuffer && !mute) {
		unsigned i;

		for (i = 0; i < rakPeer->GetMaximumNumberOfPeers(); i++) {
			rakVoice.SendFrame(rakPeer->GetGUIDFromIndex(i), inputBuffer);
		}
	}

	rakVoice.ReceiveFrame(outputBuffer);

	return 0;
}
