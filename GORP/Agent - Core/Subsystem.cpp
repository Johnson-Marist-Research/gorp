#include "Subsystem.h"

void Subsystem::_ready() {
    std::cout << "Running Subsystem.ready()" << std::endl;
    // We fetch an instance of subject

    // We fetch an instance of blackboard

    // Connect the "on_posted" function to the "posted" function in Blackboard
}

void Subsystem::_on_posted(int key) {
    std::cout << "Running Subsystem._on_posted()" << std::endl;
}

// If there is a current plan, take the first element in current_plan (next_action) and executes it
// Continues to execute the steps until there are no more remaining.
void Subsystem::execute_plan(std::vector<Response>& current_plan, WorkingMemory& workingMemory) {
	if (current_plan.empty()) {
        std::cerr << "current_plan is empty; not executing" << std::endl;
		return;
	}

	std::cerr << "\n\n------------------------ Executing plan ------------------------\n\n" << std::endl;

	Response next_action = current_plan.front();
	next_action.execute(next_action, workingMemory);

	// Clear sensor.macAddresses so that GORP doesn't get confused with outdated information next time
	//workingMemory.macAddresses.clear();

	std::cerr << "\n****************** COMPLETED PLAN EXECUTION ******************\n\n" << std::endl;
}
