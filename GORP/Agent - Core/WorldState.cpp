#include "WorldState.h"

#include <map>

WorldState::WorldState(std::shared_ptr<WorldProperty> props)
{
    // Not sure if this is needed, but I'll leave it here just in case
	// This really isn't right, but I'll leave it as a placeholder for now
	// As it is, it rewrites the ENTIRITY of properties to be props. That is not right.
	//for (int prop = 0; prop < sizeof(properties); prop++) {
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

std::string WorldState::_to_string() {
	// Fix later
	for (int i = 0; i < sizeof(properties); i++) {
		//return std::to_string(properties[i]);
		return "_to_string() placeholder";
	}
}

std::map<std::string, std::shared_ptr<WorldProperty>> WorldState::duplicate() {
	std::map<std::string, std::shared_ptr<WorldProperty>> propDuplicate = properties;
	return propDuplicate;
}

int WorldState::size() {
	return properties.size();
}

bool WorldState::has(std::string key) {
	if (properties.count(key) > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool WorldState::insert(std::shared_ptr<WorldProperty> prop) {
	// Figure out how to write this in C++
	properties[prop->to_key()] = prop;
	return true;
}

// Adds a property to this world state or overwrites it if already present
bool WorldState::add_property(std::string key, std::shared_ptr<WorldProperty> prop) {
	properties[key] = prop;
	return true;
}

// This one's a mess too
std::shared_ptr<WorldProperty> WorldState::get_property(std::string key) {
	return properties[key];
}

void WorldState::drop_property(std::string key) {
	// map has a built-in erase function
	properties.erase(key);
}

void WorldState::drop_properties(std::string keys[]) {
	// map has a built-in erase function
	// Check if this works later. I think it might, but it pays to be sure.
	for (int key = 0; key < sizeof(keys); key++) {
		properties.erase(keys[key]);
	}
}

// Determine whether all properties of a goal state are present in this state with their required values
bool WorldState::satisfies(std::shared_ptr<WorldState> goal) {
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
/*
func satisfies(goal:WorldState) -> bool:
	for key in goal.properties:
		if not self.has(key) or self.get_property(key).value != goal.get_property(key).value:
			return false
	return true
*/

// Return a substate of the first state containing only those properties that are not satisfied (i.e., same in) the second state
std::shared_ptr<WorldState> WorldState::difference(std::shared_ptr<WorldState> a, std::shared_ptr<WorldState> b) {
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

std::shared_ptr<WorldState> WorldState::expand_by(std::shared_ptr<WorldState> goal, WorldState preconditions) {
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