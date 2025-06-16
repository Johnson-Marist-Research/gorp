#include <iostream>
// Will probably need to "#include" a few of the files
// Probably Sensors and Subsystems. They're the ones directly communicating with the host system.
// Not sure about the others.
#include "Sensor.h"
#include "Subsystem.h"

/*
TODO:
- Add a timer for the sensors to check the system
- Get Planner set up
- Coordinate the communication between...
	- Sensors and WorkingMemory
	- WorkingMemory and Planner
	- Planner and Blackboard
	- Blackboard and Subsystems
*/

int main() {
	std::cout << "Test" << std::endl;
	return 0;
}