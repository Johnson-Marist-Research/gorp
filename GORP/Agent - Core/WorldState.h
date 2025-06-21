#pragma once

#include <string>
#include <map>
#include "WorldProperty.h"

// Will need a WorldProperty file

class WorldState {
public:
	// Was a dictionary, but will save as a map for now
	std::map<int, WorldProperty> properties{};

	void _init(WorldProperty props);

	std::string _to_string();

	std::map<int, WorldProperty> duplicate();

	int size();

	bool has(int key);
	bool insert(WorldProperty prop);

	bool add_property(int key, WorldProperty prop);
	WorldProperty get_property(int key);

	void drop_property(int key);
	void drop_properties(int keys[]);

	bool satisfies(WorldState goal);

	WorldState difference(WorldState a, WorldState b);
	WorldState reduce_by(WorldState goal, WorldState effects, bool forbid_conflicts);
	WorldState expand_by(WorldState goal, WorldState preconditions);

	WorldState return_WorldState(WorldState new_goal);
	void* return_null();
};
