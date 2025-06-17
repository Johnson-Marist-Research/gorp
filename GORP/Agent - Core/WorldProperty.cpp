#include "WorldProperty.h"

/*WorldProperty::WorldProperty()
{
    // Not sure if this is needed, but I'll leave it here just in case
}*/

void WorldProperty::_init(std::string subject, std::string name, bool value) {
	this->subject = subject;
	this->name = name;
	this->value = value;
}

std::string WorldProperty::_to_string() {
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

std::string WorldProperty::to_key() {
	// return subject.name + ": " + name
	return subject + ": " + name;
}