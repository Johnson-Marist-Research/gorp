#include "Agent.h"
#include "Planner.h"

#include <iostream>

// Runs all the initialization functions once GORP is started
Agent::Agent() {
	init_responses();
	init_goals();
	init_knowledge();
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
WorldState Agent::update_knowledge() {
	for (auto const& key : knowledge.properties) {
		std::shared_ptr<WorldProperty> prop = knowledge.properties[key];
		switch (prop.name) {
			// Add knowledge here (still have to add it to Agent.h)
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
		std::cout << "Planning for goal " << goal.properties << std::endl;

		std::forward_list<Response> plan = Planner.devise_plan(current_state, goal, responses);
		if (sizeof(plan) == 0) {
			// Debugging message
			std::cout << "Unable to satisfy goal " << goal.properties << std::endl;
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

// Initializes the knowledge variables
void Agent::init_knowledge() {
	// knowledge.insert(WorldProperty.new(self, "in_bed", in_bed))
}