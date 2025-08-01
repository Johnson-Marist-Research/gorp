#pragma once

#include "WorkingMemory.h";

#include <map>
#include <vector>
#include <array>

// For the ARP Table
#include <fstream>
#include <string>
#include <sstream>

#include <iostream>
#include <filesystem>

class PortVars {
public:
	bool port_open;
	bool port_blocked;
	bool normal_traffic;
	bool excess_traffic_detected;
};

class Sensor {
public:
	Sensor();
	void randomizeTraffic(std::map<int, int>& ports);
	int checkARPTable(std::map<std::string, int>& macAddresses);
	std::string getMACAddress(std::string line);
};
