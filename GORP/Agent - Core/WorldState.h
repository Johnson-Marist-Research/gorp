#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <optional>
#include "WorldProperty.h"

// Will need a WorldProperty file

class WorldState {
public:
	//std::map<std::string, std::shared_ptr<WorldProperty>> properties{};
	std::map<std::string, std::optional<WorldProperty>> properties{};

	// Zero-Argument Constructor
	WorldState();

	//WorldState(std::map<std::string, std::shared_ptr<WorldProperty>> props);
	WorldState(std::map<std::string, std::optional<WorldProperty>> props);

	std::string _to_string() const;

    //bool equals(std::shared_ptr<WorldState> b);
    bool equals(std::optional<WorldState> b);

	//std::shared_ptr<WorldState> duplicate();
	std::optional<WorldState> duplicate();

	int size();

	bool has(std::string key);
	//bool insert(std::shared_ptr<WorldProperty> prop);
	bool insert(std::optional<WorldProperty> prop);

	//bool add_property(std::string key, std::shared_ptr<WorldProperty> prop);
	bool add_property(std::string key, std::optional<WorldProperty> prop);
	//std::shared_ptr<WorldProperty> get_property(std::string key);
	std::optional<WorldProperty> get_property(std::string key);

	void drop_property(std::string key);

	//bool satisfies(std::shared_ptr<WorldState> goal);
	bool satisfies(std::optional<WorldState> goal);

	/*static std::shared_ptr<WorldState> difference(std::shared_ptr<WorldState> a, std::shared_ptr<WorldState> b);
	static std::shared_ptr<WorldState> reduce_by(std::shared_ptr<WorldState> goal, std::shared_ptr<WorldState> effects, bool forbid_conflicts);
	static std::shared_ptr<WorldState> expand_by(std::shared_ptr<WorldState> goal, WorldState preconditions);*/
	static std::optional<WorldState> difference(std::optional<WorldState> a, std::optional<WorldState> b);
	static std::optional<WorldState> reduce_by(std::optional<WorldState> goal, std::optional<WorldState> effects, bool forbid_conflicts);
	static std::optional<WorldState> expand_by(std::optional<WorldState> goal, WorldState preconditions);
};
