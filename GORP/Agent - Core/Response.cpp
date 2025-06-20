#include "Response.h"

void Response::_init(std::string name, float cost, WorldState before, WorldState after) {
	this->name = name;
	this->cost = cost;
	this->preconditions = before;
	this->effects = after;
	//this->procedure = proc;
}

void Response::execute() {
	// procedure.call()
}