#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

// For the ARP Table
#include <fstream>
#include <string>
#include <sstream>

class PortData {
public:
	int traffic;
	bool state;
};

class ARPData {
public:
	std::vector<std::string> ip_addresses;
	int count() const {return ip_addresses.size();}
};

class WorkingMemory {
public:
	/*
	// ------------ Preconditions and Postconditions ------------
	// Ports
	bool normal_traffic;
	bool port_open;
	// Is this one necessary?
	bool excess_traffic_detected;
	bool port_blocked;

	// ARP Table
	bool no_ARP_anomalies;
	bool ARP_anomaly_quarantined;

	// File Permissions
	bool files_unchanged;
	bool change_detected;

	// Log Policing
	bool no_gaps;
	bool gap_detected;

	// Safe Mode
	bool general_mode;
	bool safe_mode;
	bool dns_match;
	bool dns_mismatch;
	*/

	// int = port number, float = traffic (in, idk, let's say megabits per second {mbps})
	// std::map<int, int> ports;
	// std::map<std::string, int> macAddresses;


	// Check if I should delete these from Agent.h later
	// Ports
	//bool normal_traffic;
	//bool port_open;
	// Is this one necessary?
	//bool excess_traffic_detected;
	//bool port_blocked;

	// Create a vector(?) of some sort of list,
	// where each entry in the vector is the list of four port-related WorldState variables.
	// Connect the vector to the ports map
	//std::vector<PortVars> portVector;


	// Currently doing random generation between 0 and 100
	// Let's say 64 is the average, so 50% greater than that would be 96
	// For sake of practice, let's say that any traffic greater than or equal to 96 is bad.
	int averageTraffic = 64;

	std::map<int, PortData> port_facts;
	std::map<std::string, ARPData> ARP_facts;

	std::map<int, int> known_facts{};

	WorkingMemory();
	void store(int key, std::string fact);
	void forget(int key);
	int recall(int key);
};

/*
Execution Options for WorkingMemory
- Create sub-fact maps for each specific section (ARP_facts, port_facts, etc). Store these in the overall know_facts map
- Get information from Sensor. Determine what type of information it is. Store in the relevent map.
- Planner decides priorities. Depending on the priority of a task, check that map first (Ex: If ARP is higher priority than ports, check that map first)
*/

/*
Agent pivot options:
- Check sensor in the middle of the planning sections (Just before devise_plan?)
- If a higher priority event occurs, set a flag on the previous calculation and pivot to the new one
    - Store old data in a map?
    - Once new plan is executed, check if there was an old plan we abandonded
    - Retart the old one where we left off.
*/
