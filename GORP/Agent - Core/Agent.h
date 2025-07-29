#pragma once
#include "Blackboard.h"
#include "Planner.h"
#include "Response.h"
#include "Sensor.h"
#include "Subsystem.h"
#include "WorkingMemory.h"
#include "WorldState.h"

#include <memory>
#include <forward_list>
#include <vector>

class Agent {
public:
	// Agent Components
	Blackboard blackboard;
	Planner planner;
	Sensor sensor;
	Subsystem subsystem;
	WorkingMemory workingMemory;

	// Need to make these vectors so we can iterate through them
	// Original was "std::vector<Response> const&", but had trouble initializing them
	// Should they be vectors of shared pointers?
	// Ask about it later
	std::vector<Response> responses;
	std::vector<std::shared_ptr<WorldState>> goals;

	std::vector<Response> current_plan;
	// Changed this from a shared pointer because I was having trouble with update_knowledge()
	// Should I change it back and alter update_knowledge()?
	// This only works if the WorldState class has a zero-argument default constructor
	std::shared_ptr<WorldState> knowledge;

	// ------------ Preconditions and Postconditions ------------
	// Ports
	//bool normal_traffic;
	bool port_open;
	// Is this one necessary?
	bool excess_traffic_detected;
	bool port_blocked;

	// ARP Table
	bool ip_address_blocked;
	//bool no_ARP_anomalies;
	bool ARP_anomaly;

	// File Permissions
	bool files_unchanged;
	bool change_detected;

	// Log Policing
	//bool no_gaps;
	//bool gap_detected;

	// Safe Mode
	bool general_mode;
	bool safe_mode;
	bool dns_match;
	//bool dns_mismatch;

	// ------------ Functions ------------
	// Runs initialize functions (responses, goals, knowledge)
	Agent();

	// Runs through a loop of process, update, and execute (either continuously or tied to a timer?)
	void run_agent();

	std::shared_ptr<WorldState> process_sensor();
	std::shared_ptr<WorldState> update_knowledge();
	void make_plan();
	void execute_plan();

	void init_responses();
	void init_goals();
};