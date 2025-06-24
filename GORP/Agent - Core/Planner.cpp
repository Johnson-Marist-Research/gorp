#include "Planner.h"
#include "PriorityQueue.h"

#include <vector>

// Calculates the distance between two states by iterating through each entry in properties.
// If the source WorldState does not have the current key, we add one to the distance.
// Otherwise, if the current source WorldState's value (as retrieved by get_property()) does not equal the destination WorldState's value,
// then we add one to the distance, as one property needs to be changed.
// Once the calculations are complete, we return the distance (d).
float Planner::distance(std::shared_ptr<WorldState> src, std::shared_ptr<WorldState> dst) {
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
	// Can check if a pointer is null, but not a class
	std::shared_ptr<WorldState> unsatisfied;
	// Remove satisfied properties from a goal state, as long as no conflicts exist
	unsatisfied->reduce_by(goal, response.effects, true);
	if (unsatisfied == NULL) {
		return nullptr;
	}
	std::shared_ptr<WorldState> new_goal;
	new_goal->expand_by(unsatisfied, response.preconditions);
	if (new_goal == goal) {
		return nullptr;
	}
	return new_goal;
};

// We devise a plan to address the current goal.
std::forward_list<Response> Planner::devise_plan(std::shared_ptr<WorldState> current_state, std::shared_ptr<WorldState> goal, std::vector<Response> const& responses) {
	std::forward_list<Response> plan;
	// First, we take note of where we came from and the current cost to get from there (goal) to here (came from).
	std::map<std::shared_ptr<WorldState>, std::shared_ptr<std::pair<std::shared_ptr<WorldState>, Response>>> came_from = {};
	std::map<std::shared_ptr<WorldState>, float> cost_so_far = {};

	// Calculate the distance (difference()) between the goal and the current_state
	// Original is "goal = WorldState.difference(goal, current_state)", so I'm just guessing with this :(
	goal->difference(goal, current_state); // Only need to satisfy the unsatisfied

	// Since goal is the start, it does not "come from" anything. Therefore, goal's came_from is null.
	came_from[goal] = nullptr;
	// Since we have not started any calculations yet, the current cost to get from the goal to the destinaton is 0.
	cost_so_far[goal] = 0;

	// Need to implement PriorityQueue class
	std::shared_ptr<PriorityQueue> frontier;
	// Use insert() from the PriorityQueue class to set the initial value of the frontier PriorityQueue
	// The second argument was originally listed as an integer in the first version of insert()
	frontier->insert(goal, cost_so_far[goal]);

	// Here, we set the value of the "start" WorldState.
	// We will go from start to goal, but since we need to figure out where start is, we will initalize it with no value for now.
	std::shared_ptr<WorldState> start;

	// We need to iterate through each entry in frontier.
	// This will allow us to trace the entire path from goal to start.
	while (!sizeof(frontier) <= 0) {
		// Out ultimate goal is not the same as the current goal. The current_goal is the next "node" in the graph
		// that we are tracing to make it back to start.
		// As such, we are determining the next sub-goal by extracting the next entry in frontier.
		std::shared_ptr<WorldState> current_goal = frontier->extract();

		// If the current_goal is the same as the current_state, then we have reached our starting state.
		// Since A* Search works backwards from the goal, this means that we have found a path from the goal to the start.
		// We can set the start equal to the current sub-goal, then break the while loop. 
		if (current_state->satisfies(current_goal)) {
			start = current_goal;
			break;
		}

		//for (int response = 0; response < sizeof(responses); response++) {
		// If the current_state is not the same as current_goal, we need to calculate a possible path from here to there.
		// As such, we will run through every possible response to determine the best solution.
		for (const auto& response : responses){
			// Creating a path by using response to connect us to current_goal
			std::shared_ptr<WorldState> next = unify(response, current_goal);

			// If a path is not found, just continue. We'll address that later.
			if (next == NULL) { continue; }

			// Remove satisfied properties from a goal state, as long as no conflicts exist
			next = next->reduce_by(next, current_state, false);

			// Calculating the cost to move from starting point to goal point
			float g_cost = cost_so_far[current_goal] + response.cost;
			// Calculating h_cost and priority
			if ((cost_so_far.count(next) <= 0) || (g_cost < cost_so_far[next])) {
				cost_so_far[next] = g_cost;
				float h_cost = distance(next, current_state);
				float priority = g_cost + h_cost;
				frontier->insert(next, priority);
				// could have came_from[next] = std::make_pair
				// Use .first and .second to access
				came_from[next] = std::make_shared<std::pair<std::shared_ptr<WorldState>, Response>>(std::make_pair(current_goal, response));
			}
		}
	}

	// Construct a plan from the start state to the goal state, if possible
	std::shared_ptr<WorldState> n = start;
	while (n != goal) {
		if (came_from.count(n) <= 0) {
			break;
		}
		// Unlike list, forward_list doesn't have a built-in function to add an element to its end
		// Access response
		// plan.push_front(came_from[n]->second);
		// This is all one big calculation in order to append came_from[n]->second to the end of plan
		// end() returns one position past the end of the forward list
		// As such, we need to move back one spot to put the element at the end of the list
		auto insertPosition = plan.end();
		advance(insertPosition, -1);
		plan.insert_after(insertPosition, came_from[n]->second);
		// Probably has something to do with the fact that I haven't implemented the "procedure" part of Response.h yet
		// Access state
		n = came_from[n]->first;
	}
	return plan;
};