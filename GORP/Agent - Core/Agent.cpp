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
}

// Creates a new plan if there is not a current one
// If there is a current plan, executes it
// Will probably need a timer so that this isn't firing off at every opportunity
std::shared_ptr<WorldState> Agent::process_sensor() {
	// if action_timer.is_stopped():
	if (sizeof(current_plan) == 0) {
		make_plan();
	}
	else {
		execute_plan();
	}
}

// Updates knowledge about the World States based on information from Sensors
std::shared_ptr<WorldState> Agent::update_knowledge() {
	for (auto const& key : knowledge->properties) {
		std::shared_ptr<WorldProperty> prop = knowledge->properties[key.first];
		if (prop->value == true && prop->name == "normal_traffic") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "port_open") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "excess_traffic_detected") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "port_blocked") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "no_ARP_anomalies") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "ARP_anomaly_quarantined") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "files_unchanged") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "change_detected") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "no_gaps") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "gap_detected") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "general_mode") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "safe_mode") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "dns_match") {
			std::cout << prop->name << " is true" << std::endl;
		}
		else if (prop->value == true && prop->name == "dns_mismatch") {
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
	std::shared_ptr<WorldState> current_state = update_knowledge();
	// For goal in goals
	for (auto const& goal : goals) {
		if (current_state->satisfies(goal)) {
			continue;
		}

		// For debugging purposes
		//std::cout << "Planning for goal " << goal.properties.begin()->first << std::endl;
		std::cout << "Planning for goal " << goal->_to_string() << std::endl;

		std::forward_list<Response> plan = planner.devise_plan(current_state, goal, responses);
		if (sizeof(plan) == 0) {
			// Debugging message
			std::cout << "Unable to satisfy goal " << goal->_to_string() << std::endl;
			continue;
		}
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
	if (sizeof(current_plan) == 0) {
		return;
	}
	// Make current_plan into a vector of shared pointers?
	Response next_action = current_plan.front();
	// if next_action
	if (sizeof(next_action) > 0) {
		next_action.execute();
	}
}

// Initializes the response variables
void Agent::init_responses() {
	Response response;
	/* 
	- block_port
	- unblock_port
	- block_IP_address
	- unblock_IP_address
	- revert_file
	- update_file
	- close_GORP
	- switch_to_gen_mode
	- switch_to_safe_mode
	- block_dns_response
	*/
	
	// Making this first Response as a test case
	// Once it is debugged, it can act as a blueprint for the remainder
	Response block_port((std::string "block_port"), 1,
		// Preconditions
		WorldState({
			WorldProperty(this, "excess_traffic_detected", true),
			WorldProperty(this, "port_open", true)
			}),
		//Effects
		WorldState({
			WorldProperty(this, "excess_traffic_detected", false),
			WorldProperty(this, "port_open", false),
			WorldProperty(this, "port_blocked", true)
			})
		// port_under_attack(); --> ????
	);
	/*
	Action.new("go_to_bush", 1,
			# Preconditions
			WorldState.new([
				WorldProperty.new(self, "bush_reachable", true),
				WorldProperty.new(self, "in_bed", false)
			]),
			# Effects
			WorldState.new([
				WorldProperty.new(self, "near_bed", false),
				WorldProperty.new(self, "near_bush", true),
				WorldProperty.new(self, "near_fire", false),
				WorldProperty.new(self, "near_friend", false)
			]),
			func(): goto("bush")
		),
	*/
}

// Initializes the goal variables
void Agent::init_goals() {
	WorldState goals[] = {
		WorldProperty(this, "port_under_attack", false),
		WorldProperty(this, "ARP_attack", false),
		WorldProperty(this, "unauthorized_file_change", false),
		WorldProperty(this, "unusual_log_activity", false),
		WorldProperty(this, "safe_mode_active", false),
		WorldProperty(this, "dns_attack", false)
	};
	/*
	goals = [
		# Top of list is highest priority
		WorldState.new([WorldProperty.new(self, "is_cold", false)]),
		WorldState.new([WorldProperty.new(self, "is_hungry", false)]),
		WorldState.new([WorldProperty.new(self, "is_sleepy", false)]),
		WorldState.new([WorldProperty.new(self, "is_lonely", false)])
	]
	*/
}