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

    // ----------------------------- PORTS -----------------------------
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

// Easier to do the parsing in a stand-alone function
std::string Sensor::getMACAddress(std::string line) {
    // Allows us to read through a string like its one continuous input/output stream
    std::stringstream ss(line);
    // Makes a bunch of different string variables at once. Takes up less space than creating six lines of "std::string...."
    // Each of these strings correspond to one of the six sections of the MAC address
    std::string ip, hw_type_string, flagsString, macAddress, mask, device;
    // Converts information from file
    ss >> ip >> hw_type_string >> flagsString >> macAddress >> mask >> device;
    return macAddress;
}

// ----------------------------- ARP Table -----------------------------
int Sensor::checkARPTable() {
    // ARP Table on Linux can be read like a text file by using "/proc/net/arp"
    // We use ifstream to read it
    std::ifstream arpFile("/proc/net/arp");

    if (!arpFile.is_open()) {
        std::cerr << "Could not locate /proc/net/arp" << std::endl;
        // Indicates that the file couldn't be opened for some reason
        return 1;
    }

    std::string line;
    // Skips the header line (doesn't have any information we need)
    std::getline(arpFile, line);

    while (std::getline(arpFile, line)) {
        std::string mac = getMACAddress(line);
        // 00:00:00:00:00:00 is for broadcast MAC addresses, so they can happen more than once and be fine
        // Other than that, just avoid adding empty/invalid MAC addresses to our list
        if (!mac.empty() && mac != "00:00:00:00:00:00") {
            macAddresses[mac]++;
        }
    }

    // Done with the file, so we can close it
    // We'll check if there are duplicate MAC addresses when we pivot to Agent.cpp
    arpFile.close();
    return 0;
}