#pragma once

#include "WorldState.h"
#include "WorkingMemory.h"

#include <string>

// Need these for the ARP table parts
#include <fstream>
#include <sstream>

#include <iostream>
// Needed for using system calls
#include <cstdlib>
// Used for checking if a file exists
#include <filesystem>

// For our current fake ARP file simulation
#include <vector>
#include <cstdio>

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

	// Procedural Precondition
	// Function that takes in WorldState and does whatever comparison we want on the WorldState
	// bool (*proc_precond)(WorldState const& s);

	// "proc" was shorthand for procedure, which is changed to string instead of Callable
	//void _init(std::string name, float cost, WorldState before, WorldState after, std::string proc);

	// Make a seconds constructor that is the exact same, but adds "bool (*proc)(WorldState const&)"

	Response(std::string name, float cost, WorldState before, WorldState after);
	// Used if we need to delete IP addresses from the ARP table
	std::string findIPFromMAC(std::string targetMAC);
	// For our current fake ARP file testing method
	void deleteLineFromFile(std::string target);
	// This calls procedure, but again, C++ doesn't have Callables, so we'll have to figure that out
	void execute(Response next_action, WorkingMemory& workingMemory);
};
