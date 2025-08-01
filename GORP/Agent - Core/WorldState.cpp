#include "WorldState.h"

WorldState::WorldState(){}

WorldState::WorldState(std::map<std::string, std::shared_ptr<WorldProperty>> props){
	this->properties = props;
}

// Use a range-based for loop to iterate through all the key value pairs in the map of properties
// For each property, print the value of the first element in the map, which is the key in the form of a string
std::string WorldState::_to_string() const {
	// Build a string representation of the world state
	std::string rep = "";
	// For each property add the string representation of the property to the string
	// Wrap it in braces (or similar)
	// Return the string
	for (auto& prop : properties) {
		rep.append("{");
		rep.append(prop.second->_to_string());
		rep.append("}, ");
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
	std::shared_ptr<WorldState> newState = std::make_shared<WorldState>();
	for (auto& prop : properties) {
		newState->add_property(prop.first, std::make_shared<WorldProperty>(
			prop.second->subject,
			prop.second->name,
			prop.second->value
		));
	}
	return newState;
}

int WorldState::size() {
	return properties.size();
}

bool WorldState::has(std::string key) {
	if (properties.empty()) {
		return false;
	}

	if (properties.count(key) > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool WorldState::insert(std::shared_ptr<WorldProperty> prop) {
	properties[prop->to_key()] = prop;
	return true;
}

// Adds a property to this world state or overwrites it if already present
bool WorldState::add_property(std::string key, std::shared_ptr<WorldProperty> prop) {
	properties[key] = prop;
	return true;
}

std::shared_ptr<WorldProperty> WorldState::get_property(std::string key) {
	auto it = properties.find(key);
	if(it == properties.end()){
        return nullptr;
	}
	return it->second;
}

void WorldState::drop_property(std::string key) {
	// map has a built-in erase function
	properties.erase(key);
}

// Determine whether all properties of a goal state are present in this state with their required values
bool WorldState::satisfies(std::shared_ptr<WorldState> goal) {
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
	std::shared_ptr<WorldState> c = a->duplicate();
	// Iterate through each entry in b->properties
	for (const auto& entry : b->properties){
		std::string key = entry.first;
		std::shared_ptr<WorldProperty> prop = a->get_property(key);

		if ((prop) && ((prop->value) == b->get_property(key)->value)) {
			c->drop_property(key);
		}
	}
	return c;
}

// Remove satisfied properties from a goal state, as long as no conflicts exist
std::shared_ptr<WorldState> WorldState::reduce_by(std::shared_ptr<WorldState> goal, std::shared_ptr<WorldState> effects, bool forbid_conflict = true) {
	// Duplicate doesn't work so I'm ignoring it for now
	std::shared_ptr<WorldState> new_goal = goal->duplicate();
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
	std::shared_ptr<WorldState> new_goal = goal->duplicate();
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
