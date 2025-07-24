#pragma once

#include "WorldState.h"
#include <string>

// For system
#include <cstdlib>

// Need these for the ARP table parts
#include <fstream>
#include <string>
#include <sstream>

// Turns out removing an entry from the ARP table is kinda complicated, so we need a lot of things to make it work
/*#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <errno.h>*/

class Response {
public:
	// A unique identifier for this response
	std::string name;
	// A numeric cost associated with this response
	float cost;
	// A set of properties required in order to perform this response
	WorldState preconditions;
	// A set of properties that result from performing this response
	WorldState effects;
	// This was of type "Callable". Not sure how to transfer that to C++
	// Going to leave it as string for now
	//std::string procedure;

	// "proc" was shorthand for procedure, which is changed to string instead of Callable
	//void _init(std::string name, float cost, WorldState before, WorldState after, std::string proc);
	Response(std::string name, float cost, WorldState before, WorldState after);
	// Used if we need to delete IP addresses from the ARP table
	std::string findIPFromMAC(std::string targetMAC);
	// This calls procedure, but again, C++ doesn't have Callables, so we'll have to figure that out
	void execute(Response next_action, std::map<std::string, int> macAddresses);
};
