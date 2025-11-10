#pragma once

#include "WorkingMemory.h"

#include <map>
#include <vector>
#include <array>

// For the ARP Table
#include <fstream>
#include <string>
#include <sstream>

#include <iostream>
#include <filesystem>
#include <fstream>

// Random number generation
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

class PortVars {
public:
	bool port_open;
	bool port_blocked;
	bool normal_traffic;
	bool excess_traffic_detected;
};

class Sensor {
public:
    WorkingMemory& memory;
    std::string sensorName;

	Sensor(std::string selectedName, WorkingMemory& mem);
	//void sense(Sensor currentSensor, std::map<int, int>& ports, std::map<std::string, int>& macAddresses);
	void sense();
	void randomizeTraffic();
	int checkARPTable();
	std::string getMACAddress(std::string line);
	std::string getIPAddress(std::string line);

    void generateFiles();
	void getBinFiles();
    void getDirectoryFiles();
};

/*class PortSensor {
public:
    void randomizeTraffic(std::map<int, int>& ports);
};

class ARPSensor {
public:
    int checkARPTable(std::map<std::string, int>& macAddresses);
	std::string getMACAddress(std::string line);
}*/
