#include "Planner.h"
#include "PriorityQueue.h"

#include <iostream>
#include <vector>

// Calculates the distance between two states by iterating through each entry in properties.
// If the source WorldState does not have the current key, we add one to the distance.
// Otherwise, if the current source WorldState's value (as retrieved by get_property()) does not equal the destination WorldState's value,
// then we add one to the distance, as one property needs to be changed.
// Once the calculations are complete, we return the distance (d).
float Planner::distance(std::shared_ptr<WorldState> src, std::shared_ptr<WorldState> dst) {
	//std::cerr << "Running Planner.distance()" << std::endl;
	float d = 0;
	//for (int key = 0; key < sizeof(dst.properties); key++) {
	// Iterate through each entry in dst.properties
	for (const auto& entry : dst->properties){
		std::string key = entry.first;
		if (!src->has(key)) {
			d += 1; // one property needs to be added
		}
		else if (src->get_property(key)->value != dst->get_property(key)->value) {
			d += 1; // one property needs to be changed
		}
	}
	return d;
};

// If the goal state is a possible result of applying the action, then we return a minimal substate of all states in which this
// action could have been applied to yield the goal state.
std::shared_ptr<WorldState> Planner::unify(Response const& response, std::shared_ptr<WorldState> goal) {
	std::cerr << "Running Planner.unify()" << std::endl;
	// Can check if a pointer is null, but not a class
	std::shared_ptr<WorldState> unsatisfied;
	// Remove satisfied properties from a goal state, as long as no conflicts exist
	// :: is how you call a STATIC function in C++
	unsatisfied = WorldState::reduce_by(goal, std::make_shared<WorldState>(response.effects), true);
	if (unsatisfied == nullptr) {
		return nullptr;
	}
	if (unsatisfied->equals(goal)) {
		return nullptr;
	}
	std::shared_ptr<WorldState> new_goal;
	new_goal = WorldState::expand_by(unsatisfied, response.preconditions);
	return new_goal;
};

// We devise a plan to address the current goal.
std::vector<Response> Planner::devise_plan(std::shared_ptr<WorldState> current_state, std::shared_ptr<WorldState> goal, std::vector<Response> const& responses) {
	std::cerr << "Running Planner.devise_plan()" << std::endl;

	if (current_state->size() <= 0) {
		std::cerr << "\n\ncurrent_state is empty\n\n" << std::endl;
	}
	else {
		std::cerr << "\n\ncurrent_state is not empty\n\n" << std::endl;
	}


	std::vector<Response> plan;

	// Only run the rest of devise_plan() if current_state is not empty
	if (current_state->size() <= 0) {
		std::cerr << "\nplan is empty\n" << std::endl;
		return plan;
	}

	// First, we take note of where we came from and the current cost to get from there (goal) to here (came from).
	std::map<std::shared_ptr<WorldState>, std::shared_ptr<std::pair<std::shared_ptr<WorldState>, Response>>> came_from = {};
	std::map<std::shared_ptr<WorldState>, float> cost_so_far = {};

	std::cerr << "\nCheck 1" << std::endl;
	std::cerr << "goal: " << goal->_to_string() << std::endl;
	std::cerr << "current_state: \n" << current_state->_to_string() << std::endl;

	// Calculate the distance (difference()) between the goal and the current_state
	// Original is "goal = WorldState.difference(goal, current_state)", so I'm just guessing with this :(
	goal = WorldState::difference(goal, current_state); // Only need to satisfy the unsatisfied

	std::cerr << "\nCheck 2\n" << std::endl;

	// Since goal is the start, it does not "come from" anything. Therefore, goal's came_from is null.
	came_from[goal] = nullptr;
	// Since we have not started any calculations yet, the current cost to get from the goal to the destinaton is 0.
	cost_so_far[goal] = 0;

	// Need to implement PriorityQueue class
	PriorityQueue frontier;
	// Use insert() from the PriorityQueue class to set the initial value of the frontier PriorityQueue
	// The second argument was originally listed as an integer in the first version of insert()
	frontier.insert(goal, cost_so_far[goal]);

	// Here, we set the value of the "start" WorldState.
	// We will go from start to goal, but since we need to figure out where start is, we will initalize it with no value for now.
	// * dereferences the pointer, so we can use start as a plain WorldState
	std::shared_ptr<WorldState> start = current_state;

	// Print current state
	std::cerr << "current_state: " << current_state->_to_string() << std::endl;
	std::cerr << "start: " << start->_to_string() << std::endl;

	// We need to iterate through each entry in frontier.
	// This will allow us to trace the entire path from goal to start.
	while (!frontier.is_empty()) {
		//std::cerr << "frontier size: " << frontier.size() << std::endl;
		// Out ultimate goal is not the same as the current goal. The current_goal is the next "node" in the graph
		// that we are tracing to make it back to start.
		// As such, we are determining the next sub-goal by extracting the next entry in frontier.
		std::shared_ptr<WorldState> current_goal = frontier.extract();
		// Print current goal
		std::cerr << "current_goal: " << current_goal->_to_string() << std::endl;

		// If the current_goal is the same as the current_state, then we have reached our starting state.
		// Since A* Search works backwards from the goal, this means that we have found a path from the goal to the start.
		// We can set the start equal to the current sub-goal, then break the while loop.
		if (current_state->satisfies(current_goal)) {
			start = current_goal;
			//std::cerr << "\tstart is " << start.get() << std::endl;
			//std::cerr << "start (2nd check): " << start->_to_string() << std::endl;
			break;
		}

		//for (int response = 0; response < sizeof(responses); response++) {
		// If the current_state is not the same as current_goal, we need to calculate a possible path from here to there.
		// As such, we will run through every possible response to determine the best solution.
		std::string rep = "{";
		for (const auto& response : responses){
			// Print response name
			std::cerr << "\tResponse name: " << response.name << std::endl;

			// Creating a path by using response to connect us to current_goal
			std::shared_ptr<WorldState> next = unify(response, current_goal);

			// If a path is not found, just continue. We'll address that later.
			if (next == nullptr) { continue; }

			rep += next->_to_string();
			rep += ", ";

			// Remove satisfied properties from a goal state, as long as no conflicts exist
			//next = next->reduce_by(next, current_state, false);

			// Print next state
			std::cerr << "\t\t'next' state: " << next->_to_string() << std::endl;

			// Calculating the cost to move from starting point to goal point
			float g_cost = cost_so_far[current_goal] + response.cost;
			// Calculating h_cost and priority
			if ((cost_so_far.count(next) <= 0) || (g_cost < cost_so_far[next])) {
				cost_so_far[next] = g_cost;
				float h_cost = distance(next, current_state);
				float priority = g_cost + h_cost;
				frontier.insert(next, priority);
				std::cerr << "\tInserting " << next.get() << std::endl;
				// could have came_from[next] = std::make_pair
				// Use .first and .second to access
				came_from[next] = std::make_shared<std::pair<std::shared_ptr<WorldState>, Response>>(std::make_pair(current_goal, response));
			}
		}

		rep += "}";
		std::cerr  << "\Calculated Path of Responses (rep): \n" << rep << std::endl;
		std::cerr << std::endl;
	}

	//std::cerr << "\n\n\n ******************************** MADE IT OUT OF THE LOOP ******************************** \n\n\n" << std::endl;

	// Construct a plan from the start state to the goal state, if possible
	std::cerr << "start (check 3): " << start->_to_string() << std::endl;
	// Turn from plain WorldState to shared pointer
	std::shared_ptr<WorldState> n = start;
	//auto last = plan.begin();
	// TODO: Fix problems in this while loop
	// One problem is trying to decrement an empty WorldState (n)
	// I added the check at the start as a temporary patch
	// However, since we do not have a plan, when we jump back up to Agent.make_plan(),
	// there is no plan to build off of when we loop around again. At least, I think that is why it's failing.
	// Has something to do with a nullptr and line 264 in Agent
	// Does the problem have something to do with n being a pointer to a pointer (start)?
		// Trying to figure that out
	while (n != goal) {
		// Problem: The program crashes if we try to decrement n when it is empty
		// We need to decrement n to create a plan
		// So there is probably a wrong path somewhere
		if (n->size() <= 0) {
			std::cerr << "\nn is empty; cannot complete the while loop\n" << std::endl;
			break;
		}

		//std::cerr << "n in Planner.devise_plan() is " << n->_to_string() << std::endl;
		//if (came_from.count(n) <= 0) {
		if(!came_from.contains(n)){
			std::cerr << "Breaking" << std::endl;
			break;
		}
		// Unlike list, forward_list doesn't have a built-in function to add an element to its end
		// Access response
		// plan.push_front(came_from[n]->second);
		// This is all one big calculation in order to append came_from[n]->second to the end of plan
		// end() returns one position past the end of the forward list
		// As such, we need to move back one spot to put the element at the end of the list
		//auto insertPosition = plan.end();
		//advance(insertPosition, -1);
		//if (last != plan.end())
			//std::cerr << "    last: " << last->name << std::endl;
		//std::cerr << "    n: " << n->_to_string() << std::endl;
		//std::cerr << "    prev: " << came_from[n]->first->_to_string() << std::endl;
		//std::cerr << "    resp: " << came_from[n]->second.name << std::endl;

		//std::cerr << "came_from: " << came_from[n]->second.name << std::endl;
		//std::cerr << "n (check 1): " << n->_to_string() << std::endl;
		plan.push_back(came_from[n]->second);
		// Probably has something to do with the fact that I haven't implemented the "procedure" part of Response.h yet
		// Access state
		n = came_from[n]->first;
		//std::cerr << "n (check 2): " << n->_to_string() << std::endl;
		// Problem: as far as I can tell, we are trying to read 16 (0x10) points away from a nullptr
		// so the program doesn't want to print it
		//std::cerr << "came_from: " << came_from[n]->second.name << std::endl;
	}
	std::cerr << "\nFinal Plan: " << std::endl;
	for (auto const& entry : plan) {
		std::cerr << entry.name << std::endl;
	}
	std::cerr << "\nReached the end of Planner.devise_plan()\n" << std::endl;
	return plan;
};
