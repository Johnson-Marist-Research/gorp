#include "Agent.h"

#include <iostream>

/* TODO:
		- Agent Components
			- Decide if current_plan is needed here and how it differs from the instructions we write to blackboard
				- Maybe the blackboard can contain current_plan?
			- Is WorldState knowledge needed, or does the memory make this separate variable unnecessary?
			- Decide how to convert Sensor input into a WorldState
		*/

// Runs all the initialization functions once GORP is started
Agent::Agent() {

	knowledge = std::make_shared<WorldState>(WorldState());
	init_responses();
	init_goals();
}

// Runs the process_sensor() and update_knowledge() functions when called
void Agent::run_agent() {
	process_sensor();
	update_knowledge();
	// Following two are covered in process_sensor() for now
	//make_plan();
	//execute_plan();

	for (auto const& known_fact : workingMemory.known_facts) {
		if (known_fact.second >= ((sensor.averageTraffic * 0.5) + sensor.averageTraffic)) {
			// Oh no! Unusual amounts of traffic!
			std::cerr << "Unusual amounts of traffic on port " << known_fact.first << std::endl;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("excess_traffic_detected"), true));
			std::cerr << "\n\n------------------------ Making plan ------------------------\n\n" << std::endl;
			make_plan();

			if (!current_plan.empty()) {
				std::cerr << "\n\n------------------------ Executing plan ------------------------\n\n" << std::endl;
				execute_plan();
			}
			else {
				std::cerr << "current_plan is empty; not executing" << std::endl;
			}

			// Clears current_plan so we can make a new one if necessary
			current_plan.clear();
		}
		else {
			std::cerr << "Port " << known_fact.first << " contains the expected amount of traffic" << std::endl;
		}
	}

	// Time to check if there are any duplicate MAC addresses
	for (const auto& pair : sensor.macAddresses) {
		if (pair.second > 1) {
			std::cerr << "Duplicate MAC address found: " << pair.first << " (appears " << pair.second << " times)" << std::endl;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly_quarantined"), false));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), false));
			break;
		}
		else {
			// Just to check our bases
			std::cerr << "MAC Address: " << pair.first << " (appears " << pair.second << " times)" << std::endl;
		}
	}

	std::cerr << "\n\n------------------------ Making plan ------------------------\n\n" << std::endl;
	make_plan();

	if (!current_plan.empty()) {
		std::cerr << "\n\n------------------------ Executing plan ------------------------\n\n" << std::endl;
		execute_plan();
	}
	else {
		std::cerr << "current_plan is empty; not executing" << std::endl;
	}

	// Clears current_plan so we can make a new one if necessary
	current_plan.clear();
}

// Creates a new plan if there is not a current one
// If there is a current plan, executes it
// Will probably need a timer so that this isn't firing off at every opportunity
std::shared_ptr<WorldState> Agent::process_sensor() {
	// Placeholder WorldState to return
	std::shared_ptr<WorldState> placeholder;

	std::cout << "Running Agent.process_sensor()" << std::endl;

	// Store sensor.ports in WorkingMemory
	for (auto const& port : sensor.ports) {
		workingMemory.known_facts[port.first] = port.second;
	}

	sensor.checkARPTable();

	// What is the proper way to marry the declared WorldState in this function
	// with the WorldStates that serve as goals and responses?
	/*for (auto const& known_fact : workingMemory.known_facts) {
		if (known_fact.second >= ((sensor.averageTraffic * 0.5) + sensor.averageTraffic)) {
			// Oh no! Unusual amounts of traffic!
			std::cerr << "Unusual amounts of traffic on port " << known_fact.first << std::endl;
			if (current_plan.empty()) {
				make_plan();
			}
			else {
				execute_plan();
			}
		}
		else {
			std::cerr << "Port " << known_fact.first << " contains the expected amount of traffic" << std::endl;
		}
	}*/
	// Need to figure out how to detect things from the console
	// Can then turn that result into a WorldState
	// if action_timer.is_stopped():
	/*for (auto const& port : sensor.ports) {
		if (port.second >= ((sensor.averageTraffic * 0.5) + sensor.averageTraffic)) {
			// Oh no! Unusual amounts of traffic!
			std::cerr << "Unusual amounts of traffic on port " << port.first << std::endl;
			if (current_plan.empty()) {
				make_plan();
			}
			else {
				execute_plan();
			}
		}
		else {
			std::cerr << "Port " << port.first << " contains the expected amount of traffic" << std::endl;
		}
	}*/
	return placeholder;
}

// Updates knowledge about the World States based on information from Sensors
// Does this need to return a WorldState?
std::shared_ptr<WorldState> Agent::update_knowledge() {
	std::cout << "Running Agent.update_knowledge()" << std::endl;
	/* TODO: 
		- Sensor obtains data from host system (done)
		- Store data in WorkingMemory
		- When making plan, Agent creates a WorldState from the memory
		- Said state becomes the current_state for Planner
	*/
	// Placeholder WorldState to return
	for (auto const& key : knowledge->properties) {
		std::shared_ptr<WorldProperty> prop = knowledge->properties[key.first];
		if (prop->value == true && prop->name == "normal_traffic") {
			//WorldState port_is_unblocked;
			//port_is_unblocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), true));
			//port_is_unblocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), false));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), true));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), false));

			//knowledge = (std::make_shared<WorldState>(port_is_unblocked));
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "port_open") {
			//WorldState port_is_unblocked;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), true));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), false));

			//knowledge = (std::make_shared<WorldState>(port_is_unblocked));
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "excess_traffic_detected") {
			//WorldState port_is_unblocked;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), true));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), false));

			//knowledge = (std::make_shared<WorldState>(port_is_unblocked));
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "port_blocked") {
			//WorldState port_is_blocked;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), false));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("excess_traffic_detected"), false));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), true));

			//knowledge = (std::make_shared<WorldState>(port_is_blocked));
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "no_ARP_anomalies") {
			//WorldState ip_address_is_unblocked;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly_quarantined"), false));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), false));

			//knowledge = (std::make_shared<WorldState>(ip_address_is_unblocked));
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "ARP_anomaly_quarantined") {
			//WorldState ip_address_is_blocked;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly_quarantined"), true));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), true));

			//knowledge = (std::make_shared<WorldState>(ip_address_is_blocked));
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "files_unchanged") {
			//WorldState save_file;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("files_unchanged"), true));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("change_detected"), false));

			//knowledge = (std::make_shared<WorldState>(save_file));
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "change_detected") {
			//WorldState revert_file;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("files_unchanged"), true));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("change_detected"), false));

			//knowledge = (std::make_shared<WorldState>(revert_file));
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "no_gaps") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "gap_detected") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "general_mode") {
			//WorldState gen_mode;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("general_mode"), true));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("safe_mode"), false));

			//knowledge = (std::make_shared<WorldState>(gen_mode));
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "safe_mode") {
			//WorldState safe_mode;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("safe_mode"), true));
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("general_mode"), false));

			//knowledge = (std::make_shared<WorldState>(safe_mode));
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "dns_match") {
			//WorldState good_dns_response;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("dns_mismatch"), false));

			//knowledge = (std::make_shared<WorldState>(good_dns_response));
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "dns_mismatch") {
			//WorldState block_dns_response;
			knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("dns_mismatch"), true));

			//knowledge = (std::make_shared<WorldState>(block_dns_response));
			std::cout << prop->name << " is true" << std::endl;
		}
		else {
			if (prop->value) {
				std::cout << "The knowledge " << prop->name << " is true and unrecognized." << std::endl;
			}
			else {
				std::cout << "The knowledge " << prop->name << " is false and unrecognized." << std::endl;
			}
		}
	}
	return knowledge;

	/*
	for key in knowledge.properties:
		var prop:WorldProperty = knowledge.properties[key]
		match prop.name:
			"is_cold": prop.value = chill >= 0.5
			"is_hungry": prop.value = hunger >= 0.5
			"is_sleepy": prop.value = sleepiness >= 0.5
			"is_lonely": prop.value = loneliness >= 0.5
			"near_bed": prop.value = neighborhood.any(in_group.call("bed"))
			"near_bush": prop.value = neighborhood.any(in_group.call("bush"))
			"near_fire": prop.value = neighborhood.any(in_group.call("fire"))
			"near_friend": prop.value = neighborhood.any(in_group.call("friend"))
			"in_bed": prop.value = in_bed
			"has_berries": prop.value = berriesHeld > 0
			"bed_reachable": prop.value = reachable_locations.has("bed")
			"bush_reachable": prop.value = reachable_locations.has("bush")
			"fire_reachable": prop.value = reachable_locations.has("fire")
			"friend_reachable": prop.value = reachable_locations.has("friend")
	return knowledge
	*/
}

// Retrieves the current state of the World from update_knowledge(),
// iterates through the current goals,
// checks if current_state satisfies the current goals,
// and calls on Planner to create a plan to achieve said goals.
void Agent::make_plan() {
	std::cout << "Running Agent.make_plan()" << std::endl;
	std::shared_ptr<WorldState> current_state = update_knowledge();
	// For goal in goals
	// Found the problem! This runs through every goal we initialized in init_goals()
	for (auto const& goal : goals) {

		// Debugging
		std::cerr << "Goal in goals: ";
		for (auto const& goal2 : goals) {
			std::cerr << "<" << goal2->_to_string() << ">, " << std::endl;
		}

		if (current_state->satisfies(goal)) {
			continue;
		}

		// For debugging purposes
		//std::cout << "Planning for goal " << goal.properties.begin()->first << std::endl;
		std::cout << "Planning for goal " << goal->_to_string() << std::endl;

		std::vector<Response> plan = planner.devise_plan(current_state, goal, responses);
		if (plan.empty()) {
			// Debugging message
			std::cout << "Unable to satisfy goal " << goal->_to_string() << std::endl;
			continue;
		}

		current_plan = plan;

		// DELETE LATER: Going to break out of this after the first loop for now
		break;
		/*
		// Optional diagnostic output
		var plan_string:String = "Plan for " + str(goal.properties.keys()) + " is..."
		for action in plan:
			plan_string += action.name + " -> "
		# Prints plan_string without final arrow (that points to nothing)
		print(plan_string.substr(0, plan_string.length() - 4))
			
		current_plan = plan
		break
		*/
	}
}

// If there is a current plan, take the first element in current_plan (next_action) and executes it
// Continues to execute the steps until there are no more remaining.
void Agent::execute_plan() {
	std::cout << "Running Agent.execute_plan()" << std::endl;
	if (current_plan.empty()) {
		return;
	}
	// Make current_plan into a vector of shared pointers?
	//std::cout << "Size of the current plan is: " << std::to_string(sizeof(current_plan)) << std::endl;
	Response next_action = current_plan.front();
	// if next_action
	next_action.execute(next_action, sensor.macAddresses);

	// Clear sensor.macAddresses so that GORP doesn't get confused with outdated information next time
	sensor.macAddresses.clear();

	std::cerr << "\n****************** COMPLETED PLAN EXECUTION ******************\n\n" << std::endl;
}

// Initializes the response variables
void Agent::init_responses() {
	std::cout << "\nRunning Agent.init_response()" << std::endl;
	/* 
	- block_port
	- unblock_port
	- block_IP_address
	- unblock_IP_address
	- revert_file
	- update_file
	- switch_to_gen_mode
	- switch_to_safe_mode
	- block_dns_response

	Potential addition: close_GORP
	*/

	// ------------------- Block a port on the device -------------------
	// Preconditions
	WorldState block_port_preconds;
	block_port_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), true));
	block_port_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("excess_traffic_detected"), true));

	// Effects
	WorldState block_port_effects;
	block_port_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), false));
	block_port_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("excess_traffic_detected"), false));
	block_port_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), true));
	/*
	Commands to return:
		- findstr :<PORT NUMBER> - Locate process ID of process occurring on the port
		- taskkill /PID /F - End the process
		- netsh advfirewall firewall add rule name="<NAME>" dir=in action=allow protocol=TCP localport=<PORT NUMBER> - Create firewall to block access to the port

	*/

	// Add to responses
	Response block_port(std::string("block_port"), 1, block_port_preconds, block_port_effects);
	responses.push_back(block_port);


	// ------------------- Unblock a port on the device -------------------
	// Preconditions
	WorldState unblock_port_preconds;
	unblock_port_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), false));
	unblock_port_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), true));

	// Effects
	WorldState unblock_port_effects;
	unblock_port_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), true));
	unblock_port_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), false));

	// Add to responses
	Response unblock_port(std::string("unblock_port"), 1, unblock_port_preconds, unblock_port_effects);
	responses.push_back(unblock_port);
	/*
	Command to return:
		- firewall-cmd --zone=<ZONE NAME> --remove-port=<PORT NUMBER>/<protocol - Remove the firewall on a port
	*/


	// ------------------- Block an IP address in the ARP table -------------------
	// Preconditions
	WorldState block_IP_address_preconds;
	block_IP_address_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly_quarantined"), false));
	block_IP_address_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), false));

	// Effects
	WorldState block_IP_address_effects;
	block_IP_address_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly_quarantined"), true));
	block_IP_address_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), true));

	// Add to responses
	Response block_IP_address(std::string("block_IP_address"), 1, block_IP_address_preconds, block_IP_address_effects);
	responses.push_back(block_IP_address);
	/*
	Commands to return:
		- arp -a - Pull up the ARP table
		- arp -d - Delete affected IP addresses
	*/


	// ------------------- Unblock a previously blocked IP address -------------------
	// Preconditions
	WorldState unblock_IP_address_preconds;
	unblock_IP_address_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly_quarantined"), true));
	unblock_IP_address_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), true));

	// Effects
	WorldState unblock_IP_address_effects;
	unblock_IP_address_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly_quarantined"), false));
	unblock_IP_address_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), false));

	// Add to responses
	Response unblock_IP_address(std::string("unblock_IP_address"), 1, unblock_IP_address_preconds, unblock_IP_address_effects);
	responses.push_back(unblock_IP_address);
	/*
	Command to return:
		- arp -a - Pull up the ARP table
		- Maybe have a list of previously deleted IP addresses that we can reinstate
	*/


	// ------------------- If a file has been changed, revert it back to its last saved version -------------------
	WorldState revert_file_preconds;
	revert_file_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("files_unchanged"), false));
	revert_file_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("change_detected"), true));

	// Effects
	WorldState revert_file_effects;
	revert_file_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("files_unchanged"), true));
	revert_file_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("change_detected"), false));

	// Add to responses
	Response revert_file(std::string("revert_file"), 1, revert_file_preconds, revert_file_effects);
	responses.push_back(revert_file);
	/*
	Commands to return:
		- icacls - Check permissions of a file
		- Options from there:
			- icacls "path_to_file" /grant "user_or_group:(permissions)" - Revert file permissions to old version
			- del "path/to/file" - Delete file
	*/


	// ------------------- If a file has been changed and we want to keep that change, -------------------
	// ----------- update the program's knowledge of the file to include the most recent changes. -----------
	WorldState update_file_preconds;
	update_file_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("change_detected"), true));

	// Effects
	WorldState update_file_effects;
	update_file_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("files_unchanged"), true));
	update_file_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("change_detected"), false));

	// Add to responses
	Response update_file(std::string("update_file"), 1, update_file_preconds, update_file_effects);
	responses.push_back(update_file);
	/*
	Command to return:
		- icacls - Check permissions of a file
	*/

	// ------------------- Takes GORP out of the safe mode intended to intercept DNS requests -------------------
	WorldState gen_mode_preconds;
	gen_mode_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("general_mode"), false));

	// Effects
	WorldState gen_mode_effects;
	gen_mode_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("general_mode"), true));
	gen_mode_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("safe_mode"), false));

	// Add to responses
	Response switch_to_gen_mode(std::string("switch_to_gen_mode"), 1, gen_mode_preconds, gen_mode_effects);
	responses.push_back(switch_to_gen_mode);


	// ------------------- Switches GORP into the safe mode intended to intercept DNS requests -------------------
	WorldState safe_mode_preconds;
	safe_mode_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("safe_mode"), false));

	// Effects
	WorldState safe_mode_effects;
	safe_mode_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("safe_mode"), true));
	safe_mode_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("general_mode"), false));

	// Add to responses
	Response switch_to_safe_mode(std::string("switch_to_safe_mode"), 1, safe_mode_preconds, safe_mode_effects);
	responses.push_back(switch_to_safe_mode);


	// ------------------- Prevents the user from receiving a suspicious DNS response -------------------
	WorldState block_dns_preconds;
	block_dns_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("dns_mismatch"), true));

	// Effects
	WorldState block_dns_effects;
	block_dns_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("dns_mismatch"), false));

	// Add to responses
	Response block_dns_response(std::string("block_dns_response"), 1, block_dns_preconds, block_dns_effects);
	responses.push_back(block_dns_response);
	/*
	Commands to return:
		- Intercept the DNS response on the way back. Consult Professor Chodziutko on the best way to do this
		- nslookup - Get the expected DNS response so we can check it against the one we actually received
	*/


	// DEBUGGING 
	std::cout << "\nResponses: " << std::endl;
	for (auto const& response : responses) {
		std::cout << response.name << std::endl;
	}
	std::cout << std::endl;
}

// Initializes the goal variables
void Agent::init_goals() {
	std::cout << "Running Agent.init_goals()" << std::endl;

	// ------------------- Block a port on the device -------------------
	WorldState port_is_blocked;
	port_is_blocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), false));
	port_is_blocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("excess_traffic_detected"), false));
	port_is_blocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), true));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(port_is_blocked));

	// Old code for reference
	/*WorldState port_is_blocked({
			std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), false)
			//WorldProperty(this, "excess_traffic_detected", false),
			//WorldProperty(this, "port_blocked", true)
		});
	goals.push_back(std::make_shared<WorldState>(port_is_blocked));*/

	// ------------------- Unblock a port on the device -------------------
	WorldState port_is_unblocked;
	port_is_unblocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), true));
	port_is_unblocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), false));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(port_is_unblocked));


	// ------------------- Block an IP address in the ARP table -------------------
	WorldState ip_address_is_blocked;
	ip_address_is_blocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly_quarantined"), true));
	ip_address_is_blocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), true));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(ip_address_is_blocked));


	// ------------------- Unblock a previously blocked IP address -------------------
	WorldState ip_address_is_unblocked;
	ip_address_is_unblocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly_quarantined"), false));
	ip_address_is_unblocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), false));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(ip_address_is_unblocked));


	// ------------------- If a file has been changed, revert it back to its last saved version -------------------
	WorldState revert_file;
	revert_file.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("files_unchanged"), true));
	revert_file.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("change_detected"), false));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(revert_file));


	// ------------------- If a file has been changed and we want to keep that change, -------------------
	// ----------- update the program's knowledge of the file to include the most recent changes. -----------
	WorldState save_file;
	save_file.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("files_unchanged"), true));
	save_file.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("change_detected"), false));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(save_file));


	// ------------------- Takes GORP out of the safe mode intended to intercept DNS requests -------------------
	WorldState gen_mode;
	gen_mode.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("general_mode"), true));
	gen_mode.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("safe_mode"), false));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(gen_mode));


	// ------------------- Switches GORP into the safe mode intended to intercept DNS requests -------------------
	WorldState safe_mode;
	safe_mode.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("safe_mode"), true));
	safe_mode.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("general_mode"), false));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(safe_mode));


	// ------------------- Prevents the user from receiving a suspicious DNS response -------------------
	WorldState block_dns_response;
	block_dns_response.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("dns_mismatch"), true));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(block_dns_response));


	// DEBUGGING
	std::cout << "\nGoals: " << std::endl;
	for (auto const& goal : goals) {
		std::cout << goal << std::endl;
	}
	std::cout << std::endl;
}
