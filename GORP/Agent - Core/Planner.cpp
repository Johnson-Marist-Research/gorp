#include "Planner.h"
#include "PriorityQueue.h"

// Distance between two states
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
//DEBUG: Have class return a pointer. Can return nullptrs (consider using shared_ptr to prevent memory leaks)
std::shared_ptr<WorldState> Planner::unify(std::shared_ptr<Response> response, std::shared_ptr<WorldState> goal) {
	// Can check if a pointer is null, but not a class
	std::shared_ptr<WorldState> unsatisfied;
	unsatisfied->reduce_by(goal, response->effects, true);
	if (unsatisfied == NULL) {
		return nullptr;
	}
	std::shared_ptr<WorldState> new_goal;
	new_goal->expand_by(unsatisfied, response->preconditions);
	if (new_goal == goal) {
		return nullptr;
	}
	return new_goal;
};

WorldState Planner::return_WorldState(WorldState new_goal) {
	return new_goal;
};

void* Planner::return_null() {
	return NULL;
};

std::forward_list<Response> Planner::devise_plan(std::shared_ptr<WorldState> current_state, std::shared_ptr<WorldState> goal, std::shared_ptr<Response> responses[]) {
	// DEBUG: Make this a list or vector of Responses
	std::forward_list<Response> plan;
	// Reconsider what these maps are mapping
	// DEBUG: came_from should map WorldState keys to pairs of (WorldState,Response)
	std::map<std::shared_ptr<WorldState>, std::shared_ptr<Response>> came_from = {};
	std::map<std::shared_ptr<WorldState>, std::shared_ptr<Response>> cost_so_far = {};

	// Original is "goal = WorldState.difference(goal, current_state)", so I'm just guessing with this :(
	goal->difference(goal, current_state); // Only need to satisfy the unsatisfied

	came_from[goal] = nullptr;
	// Is this alright?
	cost_so_far[goal]->cost = 0;

	// Need to implement PriorityQueue class
	std::shared_ptr<PriorityQueue> frontier;
	// The second argument was originally listed as an integer in the first version of insert()
	frontier->insert(goal, cost_so_far[goal]->cost);

	// Initialized to NULL
	std::shared_ptr<WorldState> start;

	while (!sizeof(frontier) <= 0) {
		std::shared_ptr<WorldState> current_goal = frontier->extract();

		if (current_state->satisfies(current_goal)) {
			start = current_goal;
			break;
		}

		//for (int response = 0; response < sizeof(responses); response++) {
		for (const auto& response : responses){
			std::shared_ptr<WorldState> next = unify(responses[response], current_goal);

			if (next == NULL) { continue; }

			next = next->reduce_by(next, current_state, false);

			float g_cost = cost_so_far[current_goal] + response.cost;
			if ((cost_so_far.count(next) <= 0) || (g_cost < cost_so_far[next]->cost)) {
				cost_so_far[next]->cost = g_cost;
				float h_cost = distance(next, current_state);
				float priority = g_cost + h_cost;
				frontier->insert(next, priority);
				// came_from[next] = { "state": current_goal, "action" : action };
			}
		}
	}

	// Construct a plan from the start state to the goal state, if possible
	std::shared_ptr<WorldState> n = start;
	while (n != goal) {
		if (came_from.count(n) <= 0) {
			break;
		}
		plan.append(came_from[n]->response);
		// Probably has something to do with the fact that I haven't implemented the "procedure" part of Response.h yet
		n = came_from[n]->state;
	}
	return plan;
};