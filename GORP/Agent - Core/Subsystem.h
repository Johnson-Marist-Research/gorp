#pragma once

#include "Blackboard.h"

// Focusing on connecting the Subsystems to the Blackboard
class Subsystem {
public:
	void _ready();
	// Think this function is for checking the blackboard
	void _on_posted(int key);
private:
	// "subject" is a Node2D variable in the example. Figure out what to substitute it with here
	// Might be able to use string or int
	Blackboard blackboard;
	int key;

	// There's a "Tween" variable in the example
	// I think it's for timing when to check the blackboard? Will figure that out later.
};