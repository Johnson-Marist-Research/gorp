#include <iostream>
// Will probably need to "#include" a few of the files
// Probably Sensors and Subsystems. They're the ones directly communicating with the host system.
// Not sure about the others.
//#include "Sensor.h"
//#include "Subsystem.h"

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
- Finish WorldState.cpp's duplicate(), has(), _to_string(), get_property(), satisfies(), difference(), reduce_by() functions
- The "procedure" variable in Response.h was a "Callable". Not sure what that is in C++
	- Because of this, the execute() function in Response.cpp does not work
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
- Planner.cpp is having trouble with " dst.properties"
- POTENTIAL BUG: Check if the use of this-> in WorldProperty is the proper way to do that.
- _init() in WorldState.cpp is not configured correctly at all.
- In WorldState.cpp, reduce_by() needs to be able to return either null or a WorldState, but C++ only allows one return type.
*/

/*
GOALS FOR THIS WEEK:
- Replace Node with a string/bool
- Functions for each GORP component
- Find specific command to list every open port, then store info in a map
	- Command is "netstat -lntu"
- Continue to finish Planner
	- Create a "Response (aka "Action") file
*/

int main() {
	std::string input;

	// Setting up a quick demo to take input
	// This way we can test the program
	while (input != "n" && input != "N") {
		std::cout << "Please press a key: ";
		std::cin >> input;

		// Different Scenarios
		if (input == "a" || input == "A") {
			std::cout << "\nYou entered " + input;
		}
		else if (input == "b" || input == "B") {
			std::cout << "\nYou entered " + input;
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