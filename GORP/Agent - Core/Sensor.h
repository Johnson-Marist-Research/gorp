#pragma once

#include "WorkingMemory.h";

class Sensor {
public:
	void _ready();
private:
	// Creating an instance of WorkingMemory
	WorkingMemory memory;
};