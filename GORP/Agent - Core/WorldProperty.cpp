#include "WorldProperty.h"

#include <iostream>

WorldProperty::WorldProperty(std::string subject, std::string name, bool value) {
	this->subject = subject;
	this->name = name;
	this->value = value;
}

std::string WorldProperty::_to_string() {
	//std::cout << "Running WorldProperty._to_string()" << std::endl;
	// Printing value as true or false
	std::string valueTrueOrFalse = "";
	if (value) {
		valueTrueOrFalse = "true";
	}
	else {
		valueTrueOrFalse = "false";
	}
	// return "<" + subject.name + "." + name + " = " + str(value) + ">"
	return "<" + subject + "." + name + " = " + valueTrueOrFalse + ">";
}

// Need to return an int
// Individual key for each property (enum in WorldProperties.h)
// Or change the key in properties to a string
std::string WorldProperty::to_key() {
	//std::cout << "Running WorldProperty.to_key()" << std::endl;
	// return subject.name + ": " + name
	return subject + ": " + name;
}