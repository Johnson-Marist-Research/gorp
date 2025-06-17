#pragma once

#include <string>

class WorldProperty {
public:
	// subject was originally a Node: See if std::string works for it
	std::string subject;
	std::string name;
	bool value;

	void _init(std::string subject, std::string name, bool value);
	std::string _to_string();
	std::string to_key();
};