#include <iostream>
// Will probably need to "#include" a few of the files
#include "WorkingMemory.h"
#include "Blackboard.h"

/*
Basic Test Idea: Planner closes all open ports
*/
/*
TODO:
- Check "Simple Brain" in online GOAP example - can test GORP pipeline without a huge complicated Planner (good for first test)
- Add a timer for the sensors to check the system
- Get Planner set up
	- Figure out the C++ equivalent of .has (Planner.cpp, distance())
	- Figure out c++ equivalent of .get_property (Planner.cpp, distance())
- Finish WorldState.cpp's _to_string(), get_property(), satisfies(), difference() functions
- The "procedure" variable in Response.h was a "Callable". Not sure what that is in C++
	- Because of this, the execute() function in Response.cpp does not work
	- https://cppreference.com/w/cpp/utility/functional/function.html
- WorldProperty.h: Find C++ equivalent to Node --> Can replace Node with a string name (Node was acting as an identifier)
	- Could also be a bool depending on the exact object
	- Can also make a Subject class
- Make array/map for all the ports
	- Find command that locates all open ports
	- Each port needs to have a property or two attached to it (Ex: port_open)
	- There are a lot of ports. List of just open ports?
- WorkingMemory might store list of all open ports, but Planner is only concerned with the port that has unusual traffic.
	- As such, WorkingMemory only needs to share the affected port with Planner
- Coordinate the communication between...
	- Sensors and WorkingMemory
	- WorkingMemory and Planner
	- Planner and Blackboard
	- Blackboard and Subsystems

	- Idea: Set up main() with certain key prompts. Ping between GORP components when certain keys are pressed.
*/

/*
BUGS:
- WorkingMemory.cpp --> forget(): Overwriting is weird. Might not overwrite last item in array
- I haven't specified the size of the various maps in the header files, so that might be a problem at some point.
- POTENTIAL BUG: Check if the use of this-> in WorldProperty is the proper way to do that.
- _init() in WorldState.cpp is not configured correctly at all.
- Planner.cpp: distance() does not like !=
	- unify(): does not like the WorldState unsatisfied variable declaration
		- Actually, unify() doesn't like a lot of what is going on
	- devise_plan() is pretty unhappy too :/
*/

/*
GOALS FOR THIS WEEK:
- Complete PriorityQueue
- Functions for each GORP component
- Find specific command to list every open port, then store info in a map
	- Command is "netstat -lntu"
- Continue to finish Planner
	- Frankly, Planner is a mess right now, so it will have to be a key focus.

- Create a day-by-day plan for next week (Saturday to-do)
	- Start goal and end goal (Monday and Friday)
	- Break down into individual days between that
*/

int main() {
	WorkingMemory memory;
	Blackboard blackboard;

	// Could connect the two of them by having memory store the input in blackboard at the same time
	// But these are supposed to be connected through Planner, so will hold off and try to get Planner working
	memory.store(0, "There's something fishy going on ò_ó");
	blackboard.post(0, "Keeping you posted :D");

	std::string input;

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
		else if (input == "c" || input == "C") {
			std::cout << "\nYou entered " + input;
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