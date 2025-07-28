#include "WorldState.h"

#include <iostream>
#include <map>

WorldState::WorldState(){}

WorldState::WorldState(std::map<std::string, std::shared_ptr<WorldProperty>> props)
{
    // Not sure if this is needed, but I'll leave it here just in case
	// This really isn't right, but I'll leave it as a placeholder for now
	// As it is, it rewrites the ENTIRITY of properties to be props. That is not right.
	//for (int prop = 0; prop < sizeof(properties); prop++) {
	this->properties = props;
	/*for (const auto& entry : properties) {
		std::string key = entry.first;
		this->properties[key] = props;
	}*/
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
// TODO: Update to look for pattern
std::string WorldState::_to_string() const {
	//std::cerr << "Running WorldState._to_string()" << std::endl;
	// Build a string representation of the world state
	//  start with an empty string
	std::string rep = "";
	//  for each property add the string representation of the property to the string
	//  wrap it in braces (or similar)
	// return the string
	for (auto& prop : properties) {
		//std::cerr << "Running properties " << prop.second.get() << std::endl;
		rep.append("{");
		//rep.append(prop.first);
		//rep.append(", ");
		rep.append(prop.second->_to_string());
		rep.append("}, ");
		//return prop.first;
	}
	return rep;
}

bool WorldState::equals(std::shared_ptr<WorldState> b){
    WorldState* a = this;
    if (a->size() != b->size()){
        return false;
    }
    for (const auto& entry : b->properties){
		std::string key = entry.first;
		std::shared_ptr<WorldProperty> prop = a->get_property(key);
		// Original is having trouble comparing WorldState to a boolean; need to fix this,
		// But am taking out "prop" in the comparison just to get it running. NEED TO FIX THIS LATER.
		//if ((prop && prop.value) == b.get_property(i).value) {
		//std::cerr << "prop->value: " << prop->value << std::endl;
		//std::cerr << "b->get_property(key)->value: " << b->get_property(key)->value << std::endl;
		if ((prop) && ((prop->value) == b->get_property(key)->value)) {
			continue;
		}
		else {
            return false;
		}
	}
	return true;
}

std::shared_ptr<WorldState> WorldState::duplicate() {
	//std::cerr << "Running WorldState.duplicate()" << std::endl;
	WorldState propDuplicate = properties;
	std::shared_ptr<WorldState> newState = std::make_shared<WorldState>(propDuplicate);
	return newState;
}

int WorldState::size() {
	//std::cerr << "Running WorldState.size()" << std::endl;
	return properties.size();
}

bool WorldState::has(std::string key) {
	//std::cerr << "Running WorldState.has()" << std::endl;
	if (properties.empty()) {
		//std::cerr << "properties is empty" << std::endl;
		return false;
	}
	else {
		//std::cerr << "properties is not empty" << std::endl;
	}
	/*for (auto const& entry : properties) {
		std::cerr << entry.first << std::endl;
	}*/

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
	//std::cerr << "Running WorldState.insert()" << std::endl;
	properties[prop->to_key()] = prop;
	return true;
}

// Adds a property to this world state or overwrites it if already present
bool WorldState::add_property(std::string key, std::shared_ptr<WorldProperty> prop) {
	//std::cerr << "Running WorldState.add_property()" << std::endl;
	//std::cerr << "Key in add_property() is " << key << std::endl;
	properties[key] = prop;
	return true;
}

std::shared_ptr<WorldProperty> WorldState::get_property(std::string key) {
	//std::cerr << "Running WorldState.get_property()" << std::endl;
	//return properties[key];
	auto it = properties.find(key);
	if(it == properties.end()){
        return nullptr;
	}
	return it->second;
}

void WorldState::drop_property(std::string key) {
	//std::cerr << "Running WorldState.drop_property()" << std::endl;
	// map has a built-in erase function
	std::cerr << "Key in drop_property() is " << key << std::endl;
	//std::cerr << "\nEntries in Properties: " << std::endl;
	/*int i = 0;
	for (const auto& entry : properties) {
		std::cerr << i << ". " << entry.first << std::endl;
		i++;
	}*/
	properties.erase(key);
	/*i = 0;
	std::cerr << "\nAfter erasing the key: " << std::endl;
	for (const auto& entry : properties) {
		std::cerr << i << ". " << entry.first << std::endl;
		i++;
	}*/
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
	//std::cout << "Running WorldState.satisfies()" << std::endl;
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
	//std::cout << "Running WorldState.difference()" << std::endl;
	// Duplicate doesn't work so I'm ignoring it for now
	//WorldState c = a.duplicate();
	std::shared_ptr<WorldState> c = a;
	//for (int key = 0; key < sizeof(b.properties); key++) {
	// Iterate through each entry in b->properties
	for (const auto& entry : b->properties){
        std::cerr << "difference for loop" << std::endl;
        std::cerr << "Entry: " << entry.first << std::endl;
		std::string key = entry.first;
		std::shared_ptr<WorldProperty> prop = a->get_property(key);
		// Original is having trouble comparing WorldState to a boolean; need to fix this,
		// But am taking out "prop" in the comparison just to get it running. NEED TO FIX THIS LATER.
		//if ((prop && prop.value) == b.get_property(i).value) {
		//std::cerr << "prop->value: " << prop->value << std::endl;
		//std::cerr << "b->get_property(key)->value: " << b->get_property(key)->value << std::endl;
		if ((prop) && ((prop->value) == b->get_property(key)->value)) {
            //std::cerr << "Trying to drop a property" << std::endl;
			c->drop_property(key);
			//std::cerr << "Dropped a property" << std::endl;
		}
	}
	return c;
}

// Remove satisfied properties from a goal state, as long as no conflicts exist
std::shared_ptr<WorldState> WorldState::reduce_by(std::shared_ptr<WorldState> goal, std::shared_ptr<WorldState> effects, bool forbid_conflict = true) {
	std::cout << "Running WorldState.reduce_by()" << std::endl;
	// Duplicate doesn't work so I'm ignoring it for now
	std::shared_ptr<WorldState> new_goal = goal->duplicate();
	//std::shared_ptr<WorldState> new_goal = goal;
	//for (int key = 0; key < sizeof(goal->properties); key++) {
	// Iterate through each entry in goal->properties
	// BUG: I think it loops through, deletes something, then has trouble finding where it left off
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
