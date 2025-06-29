#include "WorldState.h"

#include <iostream>
#include <map>

WorldState::WorldState(){}

// TODO: Change parameter type to map<std::string, std::shared_ptr<WorldProperty>>
WorldState::WorldState(std::shared_ptr<WorldProperty> props)
{
    // Not sure if this is needed, but I'll leave it here just in case
	// This really isn't right, but I'll leave it as a placeholder for now
	// As it is, it rewrites the ENTIRITY of properties to be props. That is not right.
	//for (int prop = 0; prop < sizeof(properties); prop++) {
	// TODO: Replace for loop with simple assignment statement 
		// - Assign parameter to prperties
	for (const auto& entry : properties) {
		std::string key = entry.first;
		this->properties[key] = props;
	}
	/*
	func _init(props:Variant):
		if props is Dictionary:
			self.properties = props
		elif props is Array:
			for prop in props:
				self.properties[prop.to_key()] = prop
	*/
}

// Use a range-based for loop to iterate through all the key value pairs in the map of properties
// For each property, print the value of the first element in the map, which is the key in the form of a string
std::string WorldState::_to_string() const {
	std::cout << "Running WorldState._to_string()" << std::endl;
	for (auto& const key : properties) {
		return key.first;
	}
}

// TODO: Change to return a shared pointer to WorldState
std::map<std::string, std::shared_ptr<WorldProperty>> WorldState::duplicate() {
	std::cout << "Running WorldState.duplicate()" << std::endl;
	std::map<std::string, std::shared_ptr<WorldProperty>> propDuplicate = properties;
	// TODO: Make a shared pointer to a new WorldState initialized with propDuplicate
	return propDuplicate;
}

int WorldState::size() {
	std::cout << "Running WorldState.size()" << std::endl;
	return properties.size();
}

bool WorldState::has(std::string key) {
	std::cout << "Running WorldState.has()" << std::endl;
	if (properties.empty()) {
		std::cout << "properties is empty" << std::endl;
	}
	else {
		std::cout << "properties is not empty" << std::endl;
	}
	for (auto const& entry : properties) {
		std::cout << entry.first << std::endl;
	}

	if (properties.count(key) > 0) {
		return true;
	}
	else {
		return false;
	}

	/*if (!properties.empty()) {
		for (auto const& entry : properties) {
			if (entry.first == key) {
				return true;
			}
		}
	}
	return false;*/
}

bool WorldState::insert(std::shared_ptr<WorldProperty> prop) {
	std::cout << "Running WorldState.insert()" << std::endl;
	properties[prop->to_key()] = prop;
	return true;
}

// Adds a property to this world state or overwrites it if already present
bool WorldState::add_property(std::string key, std::shared_ptr<WorldProperty> prop) {
	std::cout << "Running WorldState.add_property()" << std::endl;
	properties[key] = prop;
	return true;
}

std::shared_ptr<WorldProperty> WorldState::get_property(std::string key) {
	std::cout << "Running WorldState.get_property()" << std::endl;
	return properties[key];
}

void WorldState::drop_property(std::string key) {
	std::cout << "Running WorldState.drop_property()" << std::endl;
	// map has a built-in erase function
	properties.erase(key);
}

/*void WorldState::drop_properties(std::string keys[]) {
	std::cout << "Running WorldState.drop_properties()" << std::endl;
	// map has a built-in erase function
	// Check if this works later. I think it might, but it pays to be sure.
	for (int key = 0; key < sizeof(keys); key++) {
		properties.erase(keys[key]);
	}
}*/

// Determine whether all properties of a goal state are present in this state with their required values
bool WorldState::satisfies(std::shared_ptr<WorldState> goal) {
	std::cout << "Running WorldState.satisfies()" << std::endl;
	//for (int key = 0; key < sizeof(goal.properties); key++) {
	// Iterate through each entry in goal->properties
	for (const auto& entry : goal->properties){
		std::string key = entry.first;
		// Have to check this one over too :/
		if ((this->has(key) == false) or (this->get_property(key)->value != goal->get_property(key)->value)) {
			return false;
		}
	}
	return true;
}

// Return a substate of the first state containing only those properties that are not satisfied (i.e., same in) the second state
std::shared_ptr<WorldState> WorldState::difference(std::shared_ptr<WorldState> a, std::shared_ptr<WorldState> b) {
	std::cout << "Running WorldState.difference()" << std::endl;
	// Duplicate doesn't work so I'm ignoring it for now
	//WorldState c = a.duplicate();
	std::shared_ptr<WorldState> c = a;
	//for (int key = 0; key < sizeof(b.properties); key++) {
	// Iterate through each entry in b->properties
	for (const auto& entry : b->properties){
		std::string key = entry.first;
		// Check this later
		std::shared_ptr<WorldProperty> prop = a->get_property(key);
		// Original is having trouble comparing WorldState to a boolean; need to fix this,
		// But am taking out "prop" in the comparison just to get it running. NEED TO FIX THIS LATER.
		//if ((prop && prop.value) == b.get_property(i).value) {
		if ((prop->value) == b->get_property(key)->value) {
			c->drop_property(key);
		}
	}
	return c;
}

// Remove satisfied properties from a goal state, as long as no conflicts exist
std::shared_ptr<WorldState> WorldState::reduce_by(std::shared_ptr<WorldState> goal, std::shared_ptr<WorldState> effects, bool forbid_conflict = true) {
	std::cout << "Running WorldState.reduce_by()" << std::endl;
	// Duplicate doesn't work so I'm ignoring it for now
	//WorldState new_goal = goal.duplicate();
	std::shared_ptr<WorldState> new_goal = goal;
	//for (int key = 0; key < sizeof(goal->properties); key++) {
	// Iterate through each entry in goal->properties
	for (const auto& entry : goal->properties){
		std::string key = entry.first;
		if (effects->has(key)) {
			if (effects->get_property(key)->value == goal->get_property(key)->value) {
				new_goal->drop_property(key);
			}
			else if (forbid_conflict) {
				return nullptr;
			}
		}
	}
	return new_goal;
}
// Adds a new goal to the list of possible goals
std::shared_ptr<WorldState> WorldState::expand_by(std::shared_ptr<WorldState> goal, WorldState preconditions) {
	std::cout << "Running WorldState.expand_by()" << std::endl;
	// Duplicate doesn't work so I'm ignoring it for now
	//WorldState new_goal = goal.duplicate();
	std::shared_ptr<WorldState> new_goal = goal;
	//for (int i = 0; i < sizeof(preconditions.properties); i++) {
	// Iterate through each entry in preconditions->properties
	for (const auto& entry : preconditions.properties){
		std::string key = entry.first;
		if (goal->has(key) == false) {
			new_goal->add_property(key, preconditions.get_property(key));
		}
		else if (goal->get_property(key)->value != preconditions.get_property(key)->value) {
			return nullptr;
		}
	}
	return new_goal;
}