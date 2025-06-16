#pragma once

#include <string>

// Need two signals: knowledge_gained and knowledge_lost

class WorkingMemory {
public:
	void store(int key, std::string fact);
	void forget(int key);
	std::string recall(int key);
private:
	// Once again substituting an array for a dictionary
	std::string known_facts[];
};