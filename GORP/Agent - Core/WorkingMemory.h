#pragma once

#include <string>
#include <map>

// Need two signals: knowledge_gained and knowledge_lost

class WorkingMemory {
public:
	void store(int key, std::string fact);
	void forget(int key);
	std::string recall(int key);
private:
	// Once again substituting a map for a dictionary
	std::map<int, std::string> known_facts{};
};