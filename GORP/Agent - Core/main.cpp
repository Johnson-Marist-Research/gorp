#include <iostream>
#include <chrono>
#include <thread>

#include "WorkingMemory.h"
#include "Blackboard.h"
#include "Agent.h"

/*
Notes on Testing in Sandbox:
- Works similar to testing on my own computer
- Select version and press Start
- Stick with Kali
TODO:
	- Make a fake ARP table file
*/




/*
TODO:
- Add a timer for the sensors to check the system
- https://cppreference.com/w/cpp/utility/functional/function.html
*/

/*
BUGS:
- WorkingMemory.cpp --> forget(): Overwriting is weird. Might not overwrite last item in array
- I haven't specified the size of the various maps in the header files, so that might be a problem at some point.
- Check if the end of devise_plan() in Planner.cpp actually puts the element in the correct spot in the forward list
- Problem with Agent.responses only changing a single WorldProperty:
	- Start with an empty WorldState constructor, then create an addProperty function to add as many WorldProperties as you want
	- WorldState();
	- void insert(WorldProperty prop)
*/

/*
Options for returning nothing (that isn't a shared pointer):
- Return -1 (indicates failure, but not sure it will work outside of int return types)
	- Sentinel: Unusual value used to denote failure to return object (Ex: nullptr or -1)
- Have a const* return type (int const* functionName()) to return a pointer to the element instead of the index
	- Could return nullptr with this one
- std::optional<std::string> functionName(): Gives the option of returning the string or not
	- Need to use more recent version of C++
	- return {}; to indicate returning nothing
	- When checking the return value, use std::nullopt (Ex: if (value == std::nullopt){})
*/

/*
GOALS FOR THIS WEEK:
- Debug
*/

int main() {
	Agent agent;
	WorkingMemory memory;
	Blackboard blackboard;

	// Could connect the two of them by having memory store the input in blackboard at the same time
	// But these are supposed to be connected through Planner, so will hold off and try to get Planner working
	memory.store(0, 1);
	blackboard.post(0, "Keeping you posted :D");

	std::string input;

	// Tentative Timer (only runs once for now; would need to stick in another while loop to have it run continuously)
	// For the agent timer (agent runs once every five seconds)
	// Get the starting time point
	/*auto start_time = std::chrono::steady_clock::now();
	// Calculate the end time point
	auto end_time = start_time + std::chrono::seconds(5);

	while (std::chrono::steady_clock::now() < end_time) {
		agent.run_agent();
	}*/

	// Setting up a quick demo to take input
	// This way we can test the program
	while (input != "n" && input != "N") {
		std::cout << "Please press a key: ";
		std::cin >> input;

		// Different Scenarios
		if (input == "m" || input == "M") {
			std::cout << "\nYou entered " + input;
			if (sizeof(memory) > 0) {
				std::cout << "\nMemory contains " << sizeof(memory) << " objects, which include the following : ";
				for (int i = 0; i < sizeof(memory); i++) {
					std::cout << "\n" << memory.recall(i);
				}
			}
			else {
				std::cout << "\nMemory contains nothing";
			}
		}
		else if (input == "b" || input == "B") {
			std::cout << "\nYou entered " + input;
			if (sizeof(blackboard) > 0) {
				std::cout << "\nBlackboard contains " << sizeof(memory) << " objects, which include the following: ";
				for (int i = 0; i < sizeof(blackboard); i++) {
					std::cout << "\n" << blackboard.fetch(i);
				}
			}
			else {
				std::cout << "\nBlackboard contains nothing";
			}
		}
		else if (input == "a" || input == "A") {
			std::cout << "\nYou entered " + input;
			std::cout << "\nNow running Agent...";
			agent.run_agent();
		}
		else {
			std::cout << "\nInput not recognized";
		}

		input = "";
		while (input != "y" && input != "Y" && input != "n" && input != "N") {
			std::cout << "\nDo you wish to continue? Press 'Y' for yes or 'N' for no: ";
			std::cin >> input;
			if (input != "y" && input != "Y" && input != "n" && input != "N") {
				std::cout << "\nInput not recognized. Please try again.";
			}
		}
	}
	std::cout << "\nProgram exited successfully" << std::endl;
	
	return 0;
}

/*
extends Node2D

##
@export var memory:WorkingMemory

##
@export var blackboard:Blackboard


func _ready():
	memory = memory if memory else $WorkingMemory
	blackboard = blackboard if blackboard else $Blackboard
	memory.knowledge_gained.connect(_on_knowledge_gained)


func _on_knowledge_gained(key:StringName):
	if key == &"targetpos":
		blackboard.post(&"angle", self.position.angle_to_point(memory.recall(key)))
*/

//======== EXAMPLE CODE FOR USING STD::OPTIONAL ========//
#include <optional>

/// <summary>
/// Returns the index of the target value in the vector, if it is found.
/// </summary>
/// <param name="val"></param>
/// <param name="vec"></param>
/// <returns></returns>
int findValueIn(std::string val, std::vector<std::string> const& vec)
{
	for (int i = 0; i < vec.size(); i++) {
		std::string x = vec[i];
		if (x == val) {
			return i;
		}
	}
	return -1;
}

std::string const* findValueIn2(std::string val, std::vector<std::string> const& vec)
{
	for (auto& x : vec) {
		if (x == val) {
			return &x;
		}
	}
	return nullptr;
}

std::optional<std::string> findValueIn3(std::string val, std::vector<std::string> const& vec) {
	for (auto& x : vec) {
		if (x == val) {
			return x;
		}
	}
	return {};
}

void testFind() {
	std::vector<std::string> words{ "apple", "basket", "cello" };
	std::string targetWord = "desk";

	int i = findValueIn(targetWord, words);
	if (i == -1) {
		std::cout << targetWord << " not found!\n";
	}

	auto p = findValueIn2(targetWord, words);
	if (p == nullptr) {
		std::cout << targetWord << " not found!\n";
	}

	auto w = findValueIn3(targetWord, words);
	if (w == std::nullopt) {
		std::cout << targetWord << " not found!\n";
	}
}



