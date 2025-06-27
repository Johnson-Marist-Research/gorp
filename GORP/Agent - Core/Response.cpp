#include "Response.h"

#include <iostream>

// Initialize the Response variables
Response::Response(std::string name, float cost, WorldState before, WorldState after) {
	this->name = name;
	this->cost = cost;
	this->preconditions = before;
	this->effects = after;
	//this->procedure = proc;
};

void Response::execute() {
	std::cout << "Running Response.execute()" << std::endl;
	// procedure.call()
}