#pragma once

#include "Blackboard.h"
#include "Response.h"
#include "WorkingMemory.h"

#include <iostream>
#include <vector>

// Focusing on connecting the Subsystems to the Blackboard
class Subsystem {
public:
	void _ready();
	// Think this function is for checking the blackboard
	void _on_posted(int key);
	void execute_plan(std::vector<Response>& current_plan, WorkingMemory& workingMemory);
private:
	Blackboard blackboard;
	int key;
};
