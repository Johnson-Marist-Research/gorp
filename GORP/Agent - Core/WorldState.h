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
	std::map<std::string, std::optional<WorldProperty>> properties{};

	// Zero-Argument Constructor
	WorldState();

	WorldState(std::map<std::string, std::optional<WorldProperty>> props);

	std::string _to_string() const;

    bool equals(WorldState const& b) const;

	WorldState duplicate() const;

	int size() const;

	bool has(std::string key) const;

	bool insert(std::optional<WorldProperty> prop);

	bool add_property(std::string key, std::optional<WorldProperty> prop);

	std::optional<WorldProperty> get_property(std::string key) const;

	void drop_property(std::string key);

	bool satisfies(WorldState const& goal) const;

	static WorldState difference(WorldState const& a, WorldState const& b);
	static std::optional<WorldState> reduce_by(WorldState const& goal, WorldState const& effects, bool forbid_conflicts);
	static std::optional<WorldState> expand_by(WorldState const& goal, WorldState const& preconditions);

	friend bool operator==(WorldState const& a, WorldState const& b);
	friend bool operator<(WorldState const& a, WorldState const& b);
};

bool operator==(WorldState const& a, WorldState const& b);
bool operator<(WorldState const& a, WorldState const& b);
