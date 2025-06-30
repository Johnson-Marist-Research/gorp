#include "Sensor.h"

#include <iostream>

/*Sensor::Sensor()
{
    // This can be the base class of sensor.
    // It might be a good idea to make many different types of sensors
    // Ex: Ports, files, processes, etc
}*/

Sensor::Sensor() {
    std::cout << "Running Sensor constructor" << std::endl;
    // We fetch an instance of WorkingMemory (declared here as the variable "memory")
    // Figure out later

    // For random numbers
    srand(time(0));

    std::cerr << "Ports:" << std::endl;
    // Populate ports{} with some test cases
    for (int i = 0; i < 10; i++) {
        // Random number between 0 and 100
        int randomNum = rand() % 101;
        // i = port number / key, randomNum = traffic
        ports.insert({ i, randomNum });
        std::cerr << ports[i] << std::endl;

        // Creating new entry in portVector to correspond to new entry in ports
        PortVars newPortValues;
        newPortValues.port_open = true;
        newPortValues.port_blocked = false;
        newPortValues.normal_traffic = true;
        newPortValues.excess_traffic_detected = false;
        // Adding corresponding value to portVarsVector
        portVector.push_back(newPortValues);
    }
}