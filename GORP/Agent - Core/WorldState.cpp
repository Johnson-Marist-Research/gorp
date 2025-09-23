#include "WorldState.h"

WorldState::WorldState(){}

WorldState::WorldState(std::map<std::string, std::optional<WorldProperty>> props){
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

bool WorldState::equals(WorldState const& b) const {
    if (this->size() != b.size()){
        return false;
    }
    for (const auto& entry : b.properties){
		std::string key = entry.first;
		std::optional<WorldProperty> prop = this->get_property(key);
		if ((prop) && ((prop->value) == b.get_property(key)->value)) {
			continue;
		}
		else {
			return false;
		}
	}
	return true;
}

WorldState WorldState::duplicate() const {
	WorldState newState;
	for (auto& prop : properties) {
		newState.add_property(prop.first, std::make_optional<WorldProperty>(
			prop.second->subject,
			prop.second->name,
			prop.second->value
		));
	}
	return newState;
}

int WorldState::size() const {
	return properties.size();
}

bool WorldState::has(std::string key) const {
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

bool WorldState::insert(std::optional<WorldProperty> prop) {
	properties[prop->to_key()] = prop;
	return true;
}

// Adds a property to this world state or overwrites it if already present
bool WorldState::add_property(std::string key, std::optional<WorldProperty> prop) {
	properties[key] = prop;
	return true;
}

std::optional<WorldProperty> WorldState::get_property(std::string key) const {
	auto it = properties.find(key);
	if(it == properties.end()){
        return std::nullopt;
	}
	return it->second;
}

void WorldState::drop_property(std::string key) {
	// map has a built-in erase function
	properties.erase(key);
}

// Determine whether all properties of a goal state are present in this state with their required values
bool WorldState::satisfies(WorldState const& goal) const {
	// Iterate through each entry in goal->properties
	for (const auto& entry : goal.properties){
		std::string key = entry.first;
		// Have to check this one over too :/
		if ((this->has(key) == false) or (this->get_property(key)->value != goal.get_property(key)->value)) {
			return false;
		}
	}
	return true;
}

// Return a substate of the first state containing only those properties that are not satisfied (i.e., same in) the second state
// std::shared_ptr<WorldState> WorldState::difference(std::shared_ptr<WorldState> a, std::shared_ptr<WorldState> b) {
WorldState WorldState::difference(WorldState const& a, WorldState const& b) {
	WorldState c = a.duplicate();
	// Iterate through each entry in b->properties
	for (const auto& entry : b.properties){
		std::string key = entry.first;
		std::optional<WorldProperty> prop = a.get_property(key);

		if ((prop) && ((prop->value) == b.get_property(key)->value)) {
			c.drop_property(key);
		}
	}
	return c;
}

// Remove satisfied properties from a goal state, as long as no conflicts exist
std::optional<WorldState> WorldState::reduce_by(WorldState const& goal, WorldState const& effects, bool forbid_conflict = true) {
	// Duplicate doesn't work so I'm ignoring it for now
	WorldState new_goal = goal.duplicate();
	// Iterate through each entry in goal->properties
	for (const auto& entry : goal.properties){
		std::string key = entry.first;
		if (effects.has(key)) {
			if (effects.get_property(key)->value == goal.get_property(key)->value) {
				new_goal.drop_property(key);
			}
			else if (forbid_conflict) {
				return std::nullopt;
			}
		}
	}
	return new_goal;
}
// Adds a new goal to the list of possible goals
std::optional<WorldState> WorldState::expand_by(WorldState const& goal, WorldState const& preconditions) {
	WorldState new_goal = goal.duplicate();
	// Iterate through each entry in preconditions->properties
	for (const auto& entry : preconditions.properties){
		std::string key = entry.first;
		if (goal.has(key) == false) {
			new_goal.add_property(key, preconditions.get_property(key));
		}
		else if (goal.get_property(key)->value != preconditions.get_property(key)->value) {
			return std::nullopt;
		}
	}
	return new_goal;
}

bool operator==(WorldState const& a, WorldState const& b)
{
	return a.equals(b);
}

bool operator<(WorldState const& a, WorldState const& b)
{
	for (auto i = a.properties.begin(), j = b.properties.begin(); i != a.properties.end() && j != b.properties.end(); ) {
		if (i->first < j->first) return true;
		if (i->first > j->first) return false;
        ++i; ++j;
	}
	return a.size() < b.size();
}
