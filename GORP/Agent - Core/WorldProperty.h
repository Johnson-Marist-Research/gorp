#pragma once

#include <string>

class WorldProperty {
public:
	// subject was originally a Node: See if std::string works for it
	std::string subject;
	std::string name;
	bool value;

	WorldProperty(std::string subject, std::string name, bool value);
	std::string _to_string();
	std::string to_key();
};