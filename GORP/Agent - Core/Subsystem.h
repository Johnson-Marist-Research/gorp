#pragma once

#include "Blackboard.h"
#include "Response.h"
#include "WorkingMemory.h"

#include <iostream>
#include <vector>

// Will need for chmod and mode_t for turning off executable privileges in files
#include <sys/stat.h>

// Focusing on connecting the Subsystems to the Blackboard
class Subsystem {
public:
    std::string subsystemName;

	void _ready();
	// Think this function is for checking the blackboard
	void _on_posted(int key);
	std::string findIPFromMAC(std::string targetMAC);
	void deleteLineFromFile(std::string target);
	int quarantineFile(WorkingMemory& workingMemory);
	void execute_plan(std::vector<Response>& current_plan, WorkingMemory& workingMemory);
private:
	Blackboard blackboard;
	int key;
};
