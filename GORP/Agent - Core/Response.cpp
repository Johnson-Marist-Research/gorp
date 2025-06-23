#include "Response.h"

Response::Response(std::string name, float cost, WorldState before, std::shared_ptr<WorldState> after) {
	this->name = name;
	this->cost = cost;
	this->preconditions = before;
	this->effects = after;
	//this->procedure = proc;
};

void Response::execute() {
	// procedure.call()
}