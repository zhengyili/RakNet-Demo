/* 
 * File:   Voice.h
 * Author: Dreamszhu
 *
 * Created on October 17, 2014, 10:52 AM
 */

#ifndef VOICE_H
#define	VOICE_H

#include <iostream> 

#include "portaudio.h"
#include "RakVoice.h"

#include "raknet/MessageIdentifiers.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/RakNetTypes.h"
#include "raknet/RakString.h"
#include "raknet/BitStream.h"

class Voice {
public:
	Voice();
	virtual ~Voice();
	
	void run();
	int portAudioCallback(void *inputBuffer, void *outputBuffer);
private:
	bool mute;
	RakNet::RakPeerInterface *rakPeer;
	RakNet::RakVoice rakVoice;
	PaError err;

};

#endif	/* VOICE_H */

