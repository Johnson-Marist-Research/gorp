#include "WorldState.h"

#include <map>

/*WorldState::WorldState()
{
    // Not sure if this is needed, but I'll leave it here just in case
}*/

void WorldState::_init(WorldProperty props) {
	// This really isn't right, but I'll leave it as a placeholder for now
	// As it is, it rewrites the ENTIRITY of properties to be props. That is not right.
	for (int i = 0; i < sizeof(properties); i++) {
		this->properties[i] = props;
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

/*WorldState WorldState::duplicate() {
	// This doesn't work right now, so come back to it later
	return WorldState.new(properties.duplicate());
}*/

int WorldState::size() {
	return properties.size();
}

bool WorldState::has(int key) {
	// This doesn't work right now, so come back to it later
	// Originally "return properties.has(key)". Don't think this works, but is a quick fix to get the program to RUN in the first place.
	return true;
}

bool WorldState::insert(WorldProperty prop) {
	// Figure out how to write this in C++
	//properties[prop.to_key()] = prop;
	return true;
}

// Adds a property to this world state or overwrites it if already present
bool WorldState::add_property(int key, WorldProperty prop) {
	properties[key] = prop;
	return true;
}

// This one's a mess too
WorldProperty WorldState::get_property(int key) {
	return properties[key];
}

void WorldState::drop_property(int key) {
	// map has a built-in erase function
	properties.erase(key);
}

void WorldState::drop_properties(int keys[]) {
	// map has a built-in erase function
	// Check if this works later. I think it will, but it pays to be sure.
	for (int i = 0; i < sizeof(keys); i++) {
		properties.erase(keys[i]);
	}
}

// Determine whether all properties of a goal state are present in this state with their required values
bool WorldState::satisfies(WorldState goal) {
	for (int i = 0; i < sizeof(goal.properties); i++) {
		// Have to check this one over too :/
		if ((this->has(i) == false) or (this->get_property(i).value != goal.get_property(i).value)) {
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
WorldState WorldState::difference(WorldState a, WorldState b) {
	// Duplicate doesn't work so I'm ignoring it for now
	//WorldState c = a.duplicate();
	WorldState c = a;
	for (int i = 0; i < sizeof(b.properties); i++) {
		// Check this later
		WorldProperty prop = a.get_property(i);
		// Original is having trouble comparing WorldState to a boolean; need to fix this,
		// But am taking out "prop" in the comparison just to get it running. NEED TO FIX THIS LATER.
		//if ((prop && prop.value) == b.get_property(i).value) {
		if ((prop.value) == b.get_property(i).value) {
			c.drop_property(i);
		}
	}
	return c;
}

// Remove satisfied properties from a goal state, as long as no conflicts exist
WorldState WorldState::reduce_by(WorldState goal, WorldState effects, bool forbid_conflict = true) {
	// Duplicate doesn't work so I'm ignoring it for now
	//WorldState new_goal = goal.duplicate();
	WorldState new_goal = goal;
	for (int i = 0; i < sizeof(goal.properties); i++) {
		if (effects.has(i)) {
			if (effects.get_property(i).value == goal.get_property(i).value) {
				new_goal.drop_property(i);
			}
			else if (forbid_conflict) {
				// Problem: Need to be able to return null or a WorldState, but it won't let us return both
				//return nullptr;
				return new_goal; // PLACEHOLDER UNTIL I CAN GET "return nullptr" to work
			}
		}
	}
	return new_goal;
}

WorldState WorldState::expand_by(WorldState goal, WorldState preconditions) {
	// Duplicate doesn't work so I'm ignoring it for now
	//WorldState new_goal = goal.duplicate();
	WorldState new_goal = goal;
	for (int i = 0; i < sizeof(preconditions.properties); i++) {
		if (goal.has(i) == false) {
			new_goal.add_property(i, preconditions.get_property(i));
		}
		else if (goal.get_property(i).value != preconditions.get_property(i).value) {
			// Fix this later :(
			//return nullptr;
			return new_goal; // PLACEHOLDER UNTIL I CAN GET "return nullptr" to work
		}
	}
	return new_goal;
}