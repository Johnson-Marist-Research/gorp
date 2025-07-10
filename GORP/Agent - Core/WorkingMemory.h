#pragma once

#include <string>
#include <map>

// Need two signals: knowledge_gained and knowledge_lost

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

	std::map<int, int> known_facts{};

	void store(int key, int fact);
	void forget(int key);
	int recall(int key);
};