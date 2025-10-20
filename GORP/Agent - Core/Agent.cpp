#include "Agent.h"

/* TODO:
        - IF TIME: Put ports in a file to read through and change instead of in the program itself
        - Keep some of the cerr
            - The onces that let us trace the execution (what initial state is, what current goal is, print final plan
		*/

// Runs all the initialization functions once GORP is started
Agent::Agent() {
	knowledge = std::make_shared<WorldState>(WorldState());
	//knowledge = WorldState();
	init_responses();
	init_goals();

	// Add our initial states to knowledge
	knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), true));
	knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("excess_traffic_detected"), false));
	knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), false));
	knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly"), false));
	knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), false));
	knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("files_unchanged"), true));
	knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("change_detected"), false));
	knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("general_mode"), true));
	knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("safe_mode"), false));
	knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("dns_mismatch"), false));
}

// Runs the process_sensor() and update_knowledge() functions when called
void Agent::run_agent() {
    while (true){
        // Implements a brief pause (currently 1000 milliseconds, or 1 second)
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // TODO Make this a loop with several steps...
        process_sensor(); // step 1
        update_knowledge(); // step 2
        //make_plan(); step 3 ( comprises 3a select_goal() and 3b devise_plan() )
        //TODO Temporarily we can break if there is no plan either because every goal is satisfied or cannot be
        //execute_plan(); step 4 ( maybe includes submit_plan() to write to the BlackBoard)

        std::cerr << "\n\n\n\n" << std::endl;

        std::cerr << "\n\n------------------------ Making plan ------------------------\n\n" << std::endl;
        make_plan();

        // Can check inside execute_plan() if current_plan is empty
        subsystem.execute_plan(blackboard.current_plan, workingMemory);

        // Clears current_plan so we can make a new one if necessary
        blackboard.current_plan.clear();
    }
}

// Creates a new plan if there is not a current one
// If there is a current plan, executes it
// Will probably need a timer so that this isn't firing off at every opportunity
void Agent::process_sensor() {
	std::cout << "Running Agent.process_sensor()" << std::endl;
	for (auto& sensor : sensors){
        sensor.sense();
	}

	//sensor.randomizeTraffic(workingMemory.ports);
	//sensor.checkARPTable(workingMemory.macAddresses);
}

// Updates knowledge about the World States based on information from Sensors
// Does this need to return a WorldState?
std::shared_ptr<WorldState> Agent::update_knowledge() {
	std::cout << "Running Agent.update_knowledge()" << std::endl;

	excessTraffic = false;
	for (auto port : workingMemory.port_facts) {
		if (port.second.traffic >= ((workingMemory.averageTraffic * 0.5) + workingMemory.averageTraffic)) {
			// Oh no! Unusual amounts of traffic!
			std::cerr << "Unusual amounts of traffic on port " << port.first << std::endl;
			knowledge->insert(std::make_shared<WorldProperty>("Port-" + std::to_string(port.first), std::string("excess_traffic_detected"), true));
			// knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("excess_traffic_detected"), true));
			excessTraffic = true;
			break;
		}
		else {
			std::cerr << "Port " << port.first << " contains the expected amount of traffic" << std::endl;
		}
	}
	// If there is no excess traffic
	/*if (!excessTraffic){
        knowledge->insert(std::make_shared<WorldProperty>("Port-" + std::to_string(port.first), std::string("excess_traffic_detected"), false));
        knowledge->insert(std::make_shared<WorldProperty>("Port-" + std::to_string(port.first), std::string("port_open"), true));
        knowledge->insert(std::make_shared<WorldProperty>("Port-" + std::to_string(port.first), std::string("port_blocked"), false));
        /*knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("excess_traffic_detected"), false));
        knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), true));
        knowledge->insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), false));*/
	// }


    duplicateMAC = false;
	// Time to check if there are any duplicate MAC addresses
	// for (auto pair : workingMemory.ARP_facts) {
	for (auto pair : workingMemory.ARP_facts) {

		if (pair.second.count() > 1) {
            knowledge->insert(std::make_shared<WorldProperty>(std::string("ARP"), std::string("ARP_anomaly"), true));
            knowledge->insert(std::make_shared<WorldProperty>(std::string("ARP"), std::string("ip_address_blocked"), false));
            duplicateMAC = true;
            break;
		}
	}
    // If there is not a duplicate ARP address
	if (!duplicateMAC){
        knowledge->insert(std::make_shared<WorldProperty>(std::string("ARP"), std::string("ARP_anomaly"), false));
        knowledge->insert(std::make_shared<WorldProperty>(std::string("ARP"), std::string("ip_address_blocked"), true));
    }


	return knowledge;
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
		if (current_state->satisfies(goal)) {
			continue;
		}

		// For debugging purposes
		std::cout << "\nPlanning for goal " << goal->_to_string() << "\n" << std::endl;

		std::vector<Response> plan = planner.devise_plan(current_state, goal, responses);
		if (plan.empty()) {
			// Debugging message
			std::cout << "Unable to satisfy goal " << goal->_to_string() << std::endl;
			continue;
		}

		blackboard.current_plan = plan;

		// Break out if we find a plan, otherwise we overwrite it with the next plan before executing it
		break;
	}
}

// If there is a current plan, take the first element in current_plan (next_action) and executes it
// Continues to execute the steps until there are no more remaining.
/*void Agent::execute_plan() {
	std::cout << "Running Agent.execute_plan()" << std::endl;
	if (blackboard.current_plan.empty()) {
		return;
	}

	Response next_action = blackboard.current_plan.front();
	next_action.execute(next_action, workingMemory);

	// Clear sensor.macAddresses so that GORP doesn't get confused with outdated information next time
	workingMemory.macAddresses.clear();

	std::cerr << "\n****************** COMPLETED PLAN EXECUTION ******************\n\n" << std::endl;
}*/

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
	block_IP_address_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly"), true));
	block_IP_address_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), false));

	// Effects
	WorldState block_IP_address_effects;
	block_IP_address_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly"), false));
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
	unblock_IP_address_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly"), true));
	unblock_IP_address_preconds.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), true));

	// Effects
	WorldState unblock_IP_address_effects;
	unblock_IP_address_effects.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly"), false));
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

	// ------------------- Block an IP address in the ARP table -------------------
	WorldState ip_address_is_blocked;
	ip_address_is_blocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly"), false));
	ip_address_is_blocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), true));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(ip_address_is_blocked));


	// ------------------- Block a port on the device -------------------
	WorldState port_is_blocked;
	port_is_blocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), false));
	port_is_blocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("excess_traffic_detected"), false));
	port_is_blocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), true));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(port_is_blocked));


	// ------------------- Unblock a port on the device -------------------
	/*WorldState port_is_unblocked;
	port_is_unblocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_open"), true));
	port_is_unblocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("port_blocked"), false));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(port_is_unblocked));*/


	// ------------------- Block an IP address in the ARP table -------------------
	//WorldState ip_address_is_blocked;
	//ip_address_is_blocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly_quarantined"), true));
	//ip_address_is_blocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), true));

	// Add to goals
	//goals.push_back(std::make_shared<WorldState>(ip_address_is_blocked));


	// ------------------- Unblock a previously blocked IP address -------------------
	//WorldState ip_address_is_unblocked;
	//ip_address_is_unblocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ARP_anomaly_quarantined"), false));
	//ip_address_is_unblocked.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("ip_address_blocked"), false));

	// Add to goals
	//goals.push_back(std::make_shared<WorldState>(ip_address_is_unblocked));


	// ------------------- If a file has been changed, revert it back to its last saved version -------------------
	/*WorldState revert_file;
	revert_file.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("files_unchanged"), true));
	revert_file.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("change_detected"), false));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(revert_file));*/


	// ------------------- If a file has been changed and we want to keep that change, -------------------
	// ----------- update the program's knowledge of the file to include the most recent changes. -----------
	/*WorldState save_file;
	save_file.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("files_unchanged"), true));
	save_file.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("change_detected"), false));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(save_file));*/


	// ------------------- Takes GORP out of the safe mode intended to intercept DNS requests -------------------
	/*WorldState gen_mode;
	gen_mode.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("general_mode"), true));
	gen_mode.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("safe_mode"), false));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(gen_mode));*/


	// ------------------- Switches GORP into the safe mode intended to intercept DNS requests -------------------
	/*WorldState safe_mode;
	safe_mode.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("safe_mode"), true));
	safe_mode.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("general_mode"), false));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(safe_mode));*/


	// ------------------- Prevents the user from receiving a suspicious DNS response -------------------
	/*WorldState block_dns_response;
	block_dns_response.insert(std::make_shared<WorldProperty>(std::string("Agent"), std::string("dns_mismatch"), true));

	// Add to goals
	goals.push_back(std::make_shared<WorldState>(block_dns_response));*/


	// DEBUGGING
	std::cout << "\nGoals: " << std::endl;
	for (auto const& goal : goals) {
		std::cout << goal->_to_string() << std::endl;
	}
	std::cout << std::endl;
}
