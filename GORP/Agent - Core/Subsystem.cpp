#include "Subsystem.h"

#include <iostream>

/*Subsystem::Subsystem()
{
    // This can be the base class of sensor.
    // It might be a good idea to make many different types of sensors
    // Ex: Ports, files, processes, etc
}*/

void Subsystem::_ready() {
    std::cout << "Running Subsystem.ready()" << std::endl;
    // We fetch an instance of subject (finish later)

    // We fetch an instance of blackboard (figure out later)

    // Connect the "on_posted" function to the "posted" function in Blackboard
}

void Subsystem::_on_posted(int key) {
    std::cout << "Running Subsystem._on_posted()" << std::endl;
    // This is related to timing. Have to figure that out first.
}