#include "Sensor.h"

#include <iostream>

/*Sensor::Sensor()
{
    // This can be the base class of sensor.
    // It might be a good idea to make many different types of sensors
    // Ex: Ports, files, processes, etc
}*/

void Sensor::_ready() {
    std::cout << "Running Sensor.ready()" << std::endl;
    // We fetch an instance of WorkingMemory (declared here as the variable "memory")
    // Figure out later
}