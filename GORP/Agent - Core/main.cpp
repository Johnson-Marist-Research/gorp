#include <iostream>
#include <chrono>
#include <thread>

#include "WorkingMemory.h"
#include "Blackboard.h"
#include "Agent.h"

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

int main() {
	Agent agent;
	WorkingMemory memory;
	Blackboard blackboard;

	// Could connect the two of them by having memory store the input in blackboard at the same time
	// But these are supposed to be connected through Planner, so will hold off and try to get Planner working
	//memory.store(0, 1);
	//blackboard.post(0, "Keeping you posted :D");

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



