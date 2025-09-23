#include "WorldProperty.h"

WorldProperty::WorldProperty(std::string subject, std::string name, bool value) {
	this->subject = subject;
	this->name = name;
	this->value = value;
}

std::string WorldProperty::_to_string() const {
	// Printing value as true or false
	std::string valueTrueOrFalse = "";
	if (value) {
		valueTrueOrFalse = "true";
	}
	else {
		valueTrueOrFalse = "false";
	}
	return "<" + subject + "." + name + " = " + valueTrueOrFalse + ">";
}

// Need to return an int
// Individual key for each property (enum in WorldProperties.h)
// Or change the key in properties to a string
std::string WorldProperty::to_key() const {
	return subject + ": " + name;
}
