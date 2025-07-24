#pragma once

#include "WorkingMemory.h";

#include <map>
#include <vector>
#include <array>

// For the ARP Table
#include <fstream>
#include <string>
#include <sstream>

class PortVars {
public:
	bool port_open;
	bool port_blocked;
	bool normal_traffic;
	bool excess_traffic_detected;
};

class Sensor {
public:
	// int = port number, float = traffic (in, idk, let's say megabits per second {mbps})
	std::map<int, int> ports{};
	std::map<std::string, int> macAddresses;
	

	// Check if I should delete these from Agent.h later
	// Ports
	bool normal_traffic;
	bool port_open;
	// Is this one necessary?
	bool excess_traffic_detected;
	bool port_blocked;

	// Create a vector(?) of some sort of list,
	// where each entry in the vector is the list of four port-related WorldState variables.
	// Connect the vector to the ports map
	std::vector<PortVars> portVector;


	// Currently doing random generation between 0 and 100
	// Let's say 60 is the average, so 50% greater than that would be 90
	// For sake of practice, let's say that any traffic greater than 90 is bad. 
	int averageTraffic = 60;

	Sensor();
	int checkARPTable();
	std::string getMACAddress(std::string line);
private:
	// Creating an instance of WorkingMemory
	WorkingMemory memory;
};