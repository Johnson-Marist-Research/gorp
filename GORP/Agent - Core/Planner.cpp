#include "Planner.h"
#include "PriorityQueue.h"
#include <unordered_map>
// Calculates the distance between two states by iterating through each entry in properties.
// If the source WorldState does not have the current key, we add one to the distance.
// Otherwise, if the current source WorldState's value (as retrieved by get_property()) does not equal the destination WorldState's value,
// then we add one to the distance, as one property needs to be changed.
// Once the calculations are complete, we return the distance (d).
// float Planner::distance(std::shared_ptr<WorldState> src, std::shared_ptr<WorldState> dst) {
float Planner::distance(std::optional<WorldState> src, std::optional<WorldState> dst) {
	float d = 0;
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
// std::shared_ptr<WorldState> Planner::unify(Response const& response, std::shared_ptr<WorldState> goal) {
std::optional<WorldState> Planner::unify(Response const& response, WorldState const& goal) {
	// Can check if a pointer is null, but not a class
	// Remove satisfied properties from a goal state, as long as no conflicts exist
	// :: is how you call a STATIC function in C++
	std::optional<WorldState> unsatisfied = WorldState::reduce_by(goal, response.effects, true);
	if (not unsatisfied.has_value() or unsatisfied->equals(goal)) {
		return std::nullopt;
	}

	auto new_goal = WorldState::expand_by(unsatisfied.value(), response.preconditions);
	return new_goal;
};

// We devise a plan to address the current goal.
// std::vector<Response> Planner::devise_plan(std::shared_ptr<WorldState> current_state, std::shared_ptr<WorldState> goal, std::vector<Response> const& responses) {
std::vector<Response> Planner::devise_plan(WorldState const& current_state, WorldState const& goal, std::vector<Response> const& responses) {
	std::vector<Response> plan;

	// Only run the rest of devise_plan() if current_state is not empty
	if (current_state.size() <= 0) {
		std::cerr << "\ncurrent_state is empty\n" << std::endl;
		std::cerr << "\nplan is empty\n" << std::endl;
		return plan;
	}
	std::cerr << "\ncurrent_state is not empty\n" << std::endl;


	// First, we take note of where we came from and the current cost to get from there (goal) to here (came from).
	std::map<std::optional<WorldState>, std::optional<std::pair<std::optional<WorldState>, Response>>> came_from;
	std::map<std::optional<WorldState>, float> cost_so_far = {};

	std::cerr << "\ngoal: " << goal._to_string() << std::endl;
	std::cerr << "\ncurrent_state: \n" << current_state._to_string() << std::endl;

	// Calculate the distance (difference()) between the goal and the current_state
	auto remaining_goal = WorldState::difference(goal, current_state); // Only need to satisfy the unsatisfied


	// Since goal is the start, it does not "come from" anything. Therefore, goal's came_from is null.
	came_from[remaining_goal] = std::nullopt;
	// Since we have not started any calculations yet, the current cost to get from the goal to the destinaton is 0.
	cost_so_far[remaining_goal] = 0;

	// Need to implement PriorityQueue class
	PriorityQueue frontier;
	// Use insert() from the PriorityQueue class to set the initial value of the frontier PriorityQueue
	// The second argument was originally listed as an integer in the first version of insert()
	frontier.insert(goal, cost_so_far[remaining_goal]);

	// Here, we set the value of the "start" WorldState.
	// We will go from start to goal, but since we need to figure out where start is, we will initalize it with no value for now.
	// std::shared_ptr<WorldState> start = current_state;
	std::optional<WorldState> start = current_state;

	// Print current state
	std::cerr << "\ncurrent_state: " << current_state._to_string() << std::endl;
	std::cerr << "\nstart: " << start->_to_string() << "\n" << std::endl;

	// We need to iterate through each entry in frontier.
	// This will allow us to trace the entire path from goal to start.
	while (!frontier.is_empty()) {
		// Out ultimate goal is not the same as the current goal. The current_goal is the next "node" in the graph
		// that we are tracing to make it back to start.
		// As such, we are determining the next sub-goal by extracting the next entry in frontier.
		// std::shared_ptr<WorldState> current_goal = frontier.extract();
		auto current_goal = frontier.extract().value();
		std::cerr << "Remaining goal: " << current_goal._to_string() << std::endl;

		// If the current_goal is the same as the current_state, then we have reached our starting state.
		// Since A* Search works backwards from the goal, this means that we have found a path from the goal to the start.
		// We can set the start equal to the current sub-goal, then break the while loop.
		if (current_state.satisfies(current_goal)) {
			std::cerr << "Satisfied: " << current_state._to_string() << " == " << current_goal._to_string() << std::endl;
			start = current_goal;
			break;
		}

		// If the current_state is not the same as current_goal, we need to calculate a possible path from here to there.
		// As such, we will run through every possible response to determine the best solution.
		std::string rep = "{";
		for (const auto& response : responses){
            // Creating a path by using response to connect us to current_goal
            // std::shared_ptr<WorldState> next = unify(response, current_goal);
			std::optional<WorldState> next = unify(response, current_goal);

			// If a path is not found, just continue. We'll address that later.
			if (next == std::nullopt) { continue; }

			rep += next->_to_string();
			rep += ", ";

			// Calculating the cost to move from starting point to goal point
			float g_cost = cost_so_far[current_goal] + response.cost;
			// Calculating h_cost and priority
			if ((cost_so_far.count(next) <= 0) || (g_cost < cost_so_far[next])) {
				cost_so_far[next] = g_cost;
				float h_cost = distance(next, current_state);
				float priority = g_cost + h_cost;
				frontier.insert(next.value(), priority);

				//std::cerr << "Inserting " << next.get() << std::endl;
				std::cerr << "Possible response '" << response.name << "': " << next->_to_string() << std::endl;

				// came_from[next] = std::make_shared<std::pair<std::shared_ptr<WorldState>, Response>>(std::make_pair(current_goal, response));
				came_from[next] = std::pair<std::optional<WorldState>, Response>(std::make_pair(current_goal, response));
			}
		}

		rep += "}";
		//std::cerr  << "\nCalculated Path of Responses (rep): \n" << rep << "\n" << std::endl;
		std::cerr << std::endl;
	}

	// Construct a plan from the start state to the goal state, if possible
	// Turn from plain WorldState to shared pointer
	// std::shared_ptr<WorldState> n = start;
	std::cerr << "\nCame-From Map: " << std::endl;
	for (auto n : came_from) {
		std::cerr << "    " << n.first->_to_string() << std::endl;

	}

	std::optional<WorldState> n = start;
	std::cerr << "Start: " << n->_to_string() << std::endl;
	while (not n->equals(goal)) {
		if (n->size() <= 0) {
			std::cerr << "\nn is empty; cannot complete the while loop\n" << std::endl;
			break;
		}

		if(!came_from.contains(n)){
			std::cerr << "Cannot find " << n->_to_string() << std::endl;
			std::cerr << "Breaking" << std::endl;
			break;
		}

		plan.push_back(came_from[n]->second);
		// Access state
		n = came_from[n]->first;
	}

	std::cerr << "\nPlan: " << std::endl;
	for (auto const& entry : plan) {
		std::cerr << entry.name << std::endl;
	}

	return plan;
};
