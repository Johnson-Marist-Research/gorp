#pragma once

#include <string>
#include "WorldProperty.h"

// Will need a WorldProperty file

class WorldState {
public:
	void _init(std::string props);

	std::string _to_string();

	WorldState duplicate();

	int size();

	bool has(int key);
	bool insert(WorldProperty prop);

	std::string get_property(int key);

	void drop_property(int key);
	void drop_properties(int[] keys);

	bool satisfies(WorldState goal);

	WorldState difference(WorldState a, WorldState b);
	WorldState reduce_by(WorldState goal, WorldState effects, bool forbid_conflicts = true);
	WorldState expand_by(WorldState goal, WorldState preconditions);

private:
	// Was a dictionary, but will save as an array for now
	std::string properties[];
};
