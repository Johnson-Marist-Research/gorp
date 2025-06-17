#include <iostream>
// Will probably need to "#include" a few of the files
// Probably Sensors and Subsystems. They're the ones directly communicating with the host system.
// Not sure about the others.
#include "Sensor.h"
#include "Subsystem.h"

/*
Basic Test Idea: Planner closes all open ports
*/
/*
TODO:
- Replace arrays with map
- Check "Simple Brain" in online GOAP example - can test GORP pipeline without a huge complicated Planner (good for first test)
- Add a timer for the sensors to check the system
- Get Planner set up
	- Figure out the C++ equivalent of .has (Planner.cpp, distance())
	- Figure out c++ equivalent of .get_property (Planner.cpp, distance())
- Figure out what is going on with the blackboard in Subsystem.h
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
*/

/*
GOALS FOR THIS WEEK:
- Replace Node with a string/bool
- Functions for each GORP component
- Find specific command to list every open port, then store info in a map
- Continue to finish Planner
*/

int main() {
	std::cout << "Test" << std::endl;
	return 0;
}