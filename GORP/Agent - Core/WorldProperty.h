#pragma once

#include <iostream>
#include <string>

class WorldProperty {
public:
	std::string subject;
	std::string name;
	bool value;

	WorldProperty(std::string subject, std::string name, bool value);
	std::string _to_string() const;
	std::string to_key() const;
};
