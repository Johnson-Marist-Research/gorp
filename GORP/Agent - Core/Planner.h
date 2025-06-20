#pragma once

#include "WorldState.h"
#include "Response.h"

class Planner {
public:
	// Do we need the WorldState header file?
	float distance(WorldState src, WorldState dst);
	void unify(Response response, WorldState goal);
	// Need to return an array of responses
	Response devise_plan(WorldState current_state, WorldState goal, Response responses[]);

	WorldState return_WorldState(WorldState new_goal);
	void* return_null();
};
