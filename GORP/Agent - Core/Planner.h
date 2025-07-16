#pragma once

#include "WorldState.h"
#include "Response.h"

#include <memory>
#include <forward_list>
#include <vector>

class Planner {
public:
	// Do we need the WorldState header file?
	float distance(std::shared_ptr<WorldState> src, std::shared_ptr<WorldState> dst);
	// Using shared pointers so as to prevent data leaks
	std::shared_ptr<WorldState> unify(Response const& response, std::shared_ptr<WorldState> goal);
	// Need to return an array of responses
	std::vector<Response> devise_plan(std::shared_ptr<WorldState> current_state, std::shared_ptr<WorldState> goal, std::vector<Response> const& responses);
};
