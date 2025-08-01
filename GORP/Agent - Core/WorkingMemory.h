#pragma once

#include <iostream>
#include <string>
#include <map>

// For the ARP Table
#include <fstream>
#include <string>
#include <sstream>

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
	std::map<int, int> ports;
	std::map<std::string, int> macAddresses;


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

	std::map<int, int> known_facts{};

	WorkingMemory();
	void store(int key, int fact);
	void forget(int key);
	int recall(int key);
};
