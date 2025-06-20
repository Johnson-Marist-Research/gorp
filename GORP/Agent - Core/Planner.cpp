#include "Planner.h"


// Distance between two states
/*float Planner::distance(WorldState src, WorldState dst) {
	float d = 0;
	for (int key = 0; key < sizeof(dst.properties); key++) {
		if (!src.has(key)) {
			d += 1; // one property needs to be added
		}
		// Why isn't it accepting the !=
		else if (src.get_property(key) != dst.get_property(key)) {
			d += 1; // one property needs to be changed
		}
	}
	return d;
};

// If the goal state is a possible result of applying the action, then we return a minimal substate of all states in which this
// action could have been applied to yield the goal state.
void Planner::unify(Response response, WorldState goal) {
	// Added the * to make it a pointer
	// Can check if a pointer is null, but not a class
	WorldState* unsatisfied = WorldState.reduce_by(goal, response.effects);
	if (unsatisfied == NULL) {
		return_null();
	}
	WorldState new_goal = WorldState.expand_by(unsatisfied, response.preconditions);
	if (new_goal == goal) {
		return_null();
	}
	return_WorldState(new_goal);
};

WorldState WorldState::return_WorldState(WorldState new_goal) {
	return new_goal;
};

void* WorldState::return_null() {
	return NULL;
};

Response Planner::devise_plan(WorldState current_state, WorldState goal, Response responses[]) {
	// Need to initialize plan[] with a specific size, so for now I'm going to add a placeholder of 100
	Response plan[100] = {};
	std::map<int, WorldState> came_from = {};
	std::map<int, WorldState> cost_so_far = {};

	// Original is "goal = WorldState.difference(goal, current_state)", so I'm just guessing with this :(
	goal = goal.difference(goal, current_state); // Only need to satisfy the unsatisfied
	came_from[goal] = NULL;
	cost_so_far[goal] = 0;

	// Need to implement PriorityQueue class
	PriorityQueue frontier = PriorityQueue.new();
	frontier.insert(goal, cost_so_far[goal]);

	// Initialized to NULL
	WorldState start;

	while (!frontier.is_empty()) {
		WorldState current_goal = frontier.extract();

		if (current_state.satisfies(current_goal)) {
			start = current_goal;
			break;
		}

		for (int response = 0; response < sizeof(responses); response++) {
			WorldState next = unify(responses[response], current_goal);
			// if (next == NULL) { continue; }

			next = WorldState.reduce_by(next, current_state, false);

			float g_cost = cost_so_far[current_goal] + response.cost;
			if ((cost_so_far.count(next) <= 0) || (g_cost < cost_so_far[next])) {
				cost_so_far[next] = g_cost;
				float h_cost = distance(next, current_state);
				float priority = g_cost + h_cost;
				frontier.insert(next, priority);
				// came_from[next] = { "state": current_goal, "action": action }
			}
		}
	}

	// Construct a plan from the start state to the goal state, if possible
	WorldState n = start;
	while (n != goal) {
		if (came_from.count(n) <= 0) {
			break;
		}
		plan.append(came_from[n].response);
		n = came_from[n].state;
	}
	return plan;
};*/