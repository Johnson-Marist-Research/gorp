#pragma once

#include "Response.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>

class Blackboard {
public:
    std::vector<Response> current_plan;
    std::vector<int> ports_to_block;
    std::vector<int> ports_to_unblock;

	/* The examples I combed through gave the instructions in the form of a dictionary
		As far as I can tell, C++ does not have a dictionary data type.
		For now, I am using a key to refer to the instruction's position in the instructions array
		I supposed if we wanted to, we might be able to brute-force an array of arrays
		in that every position in the array contains a smaller sub-array made of of two positions, the key and the instruction.
		I think that's doable in C++? */
	void post(int key, std::string instruction);
	std::string fetch(int key);
private:
	// Leaving the "instructions" as a map
	std::map<int, std::string> instructions{};
};

/*
Execution Options for Blackboard:
- Have Blackboard store a list of instructions sent over from Planner
- When called for, Subsystems pops the last instruction out of Blackboard and executes a shell command based off that instruction

Question is: what should those instructions be? Numbers? Strings?
The current map in Blackbaord's private section is <int, string>; one for the instruction number, one for the name of the instruction.

*/
