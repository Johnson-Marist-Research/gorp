#pragma once

#include <string>
#include <map>
#include <memory>
#include "WorldProperty.h"

// Will need a WorldProperty file

class WorldState {
public:
	// Was a dictionary, but will save as a map for now
	std::map<std::string, std::shared_ptr<WorldProperty>> properties{};

	// Zero-Argument Constructor
	WorldState();

	//WorldState(WorldProperty props);
	WorldState(std::map<std::string, std::shared_ptr<WorldProperty>> props);

	std::string _to_string() const;

    bool equals(std::shared_ptr<WorldState> b);

	std::shared_ptr<WorldState> duplicate();

	int size();

	bool has(std::string key);
	bool insert(std::shared_ptr<WorldProperty> prop);

	bool add_property(std::string key, std::shared_ptr<WorldProperty> prop);
	std::shared_ptr<WorldProperty> get_property(std::string key);

	void drop_property(std::string key);
	//void drop_properties(std::string keys[]);

	bool satisfies(std::shared_ptr<WorldState> goal);

	static std::shared_ptr<WorldState> difference(std::shared_ptr<WorldState> a, std::shared_ptr<WorldState> b);
	static std::shared_ptr<WorldState> reduce_by(std::shared_ptr<WorldState> goal, std::shared_ptr<WorldState> effects, bool forbid_conflicts);
	static std::shared_ptr<WorldState> expand_by(std::shared_ptr<WorldState> goal, WorldState preconditions);
};
