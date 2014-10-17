/* 
 * File:   main.cpp
 * Author: Dreamszhu
 *
 * Created on October 17, 2014, 8:30 AM
 */

#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <string>
#include <boost/format.hpp>

#include "include/raknet/MessageIdentifiers.h"
#include "include/raknet/RakPeerInterface.h"
#include "include/raknet/RakNetTypes.h"
#include "Server.h"
#include "Client.h"

static void show_usage(std::string name) {
	std::cerr << "RakNet Demo\n\n"
			<< "Usage: "
			<< "\tcommand [options]\n\n"
			<< boost::format("\t%-20s%s\n") % "-server" % "Run server"
			<< boost::format("\t%-20s%s\n") % "-client [ip]" % "Run client"
			<< std::endl;
}

char *l_opt_arg;
const char* short_options = "sc:";
struct option long_options[] = {
	{ "server", 0, NULL, 's'},
	{ "client", 1, NULL, 'c'},
	{ 0, 0, 0, 0},
};

int main(int argc, char** argv) {
	if (argc < 2) {
		show_usage(argv[0]);
		return 1;
	}

	int c;
	while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (c) {
			case 's':
			{
				std::cout << "Server run" << std::endl;
				Server server;
				server.run();
				break;
			}
			case 'c':
			{
				l_opt_arg = optarg;
				std::cout << "Connect server run" << l_opt_arg << std::endl;
				Client client;
				client.run();
				break;
			}
		}
	}
	return 0;
}

