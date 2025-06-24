#include "WorkingMemory.h"

// Store information from sensors in WorkingMemory
void WorkingMemory::store(int key, std::string fact)
{
    known_facts[key] = fact;
    // This is where we would have the signal emit a "knowledge_gained.emit(key)" signal
}

// Remove information from WorkingMemory
void WorkingMemory::forget(int key)
{
    // Delete the entry at "key" and shift everything over one
    for (int i = key - 1; i < sizeof(known_facts); i++) {
        // Basically overwriting the previous entry in the list with the next one
        // Could be a problem with the final entry, but we'll get to that later
        known_facts[i] = known_facts[i + 1];
    }
    // This is where we would have the signal emit a "knowledge_lost.emit(key)" signal
}

// Retrieve information already stored in WorkingMemory
std::string WorkingMemory::recall(int key) {
    return known_facts[key];
}