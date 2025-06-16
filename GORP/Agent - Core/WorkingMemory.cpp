#include "WorkingMemory.h"

WorkingMemory::WorkingMemory()
{
    // Maybe we can set the size of known_facts[] here
}

void WorkingMemory::store(int key, std::string fact)
{
    known_facts[key] = fact;
    // This is where we would have the signal emit a "knowledge_gained.emit(key)" signal
}

void WorkingMemory::forget(int key)
{
    // Delete the entry at "key" and shift everything over one
    for (int i = key - 1; i < sizeof(known_facts); i++) {
        // Basically overwriting the previous entry in the list with the next one
        known_facts[i] = known_facts[i + 1];
    }
    // This is where we would have the signal emit a "knowledge_lost.emit(key)" signal
}

std::string WorkingMemory::recall(int key) {
    return known_facts[key];
}