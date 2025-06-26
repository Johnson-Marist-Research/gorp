#include <iostream>
#include <chrono>
#include <thread>
// Will probably need to "#include" a few of the files
#include "WorkingMemory.h"
#include "Blackboard.h"
#include "Agent.h"
/*
Basic Test Idea: Planner closes all open ports
*/
/*
TODO:
- Add a timer for the sensors to check the system
- https://cppreference.com/w/cpp/utility/functional/function.html
- Make array/map for all the ports
	- Find command that locates all open ports
	- Each port needs to have a property or two attached to it (Ex: port_open)
	- There are a lot of ports. List of just open ports?
- WorkingMemory might store list of all open ports, but Planner is only concerned with the port that has unusual traffic.
	- As such, WorkingMemory only needs to share the affected port with Planner

	- Idea: Set up main() with certain key prompts. Ping between GORP components when certain keys are pressed.
*/

/*
BUGS:
- WorkingMemory.cpp --> forget(): Overwriting is weird. Might not overwrite last item in array
- I haven't specified the size of the various maps in the header files, so that might be a problem at some point.
- Response.cpp and is not happy that WorldState lacks a default constructor
- Check if the end of devise_plan() in Planner.cpp actually puts the element in the correct spot in the forward list
- Might have to change the keys in Blackboard from ints to strings
- MAJOR: I think that Agent.init_goals() doesn't like trying to use an Agent (this) in a WorldProperty
*/

/*
GOALS FOR THIS WEEK:
- IDEA: Wherever there is a signal in GDScript, replace it with a direct call to the signaled function
- Implement responses and goals (Agent class) --> Agent connects the GORP components
*/

int main() {
	Agent agent;
	WorkingMemory memory;
	Blackboard blackboard;

	// Could connect the two of them by having memory store the input in blackboard at the same time
	// But these are supposed to be connected through Planner, so will hold off and try to get Planner working
	memory.store(0, "There's something fishy going on ò_ó");
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
		else if (input == "d" || input == "D") {
			std::cout << "\nYou entered " + input;
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