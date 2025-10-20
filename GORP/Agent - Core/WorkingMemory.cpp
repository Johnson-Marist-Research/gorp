#include "WorkingMemory.h"

WorkingMemory::WorkingMemory(){
    // For random numbers
    srand(time(0));
}

// Store information from sensors in WorkingMemory
void WorkingMemory::store(int key, std::string fact) {
    std::cout << "Running WorkingMemory.store()" << std::endl;
    //known_facts[key] = fact;
}

// Remove information from WorkingMemory
void WorkingMemory::forget(int key) {
    std::cout << "Running WorkingMemory.forget()" << std::endl;
    // Delete the entry at "key" and shift everything over one
    //for (int i = key - 1; i < sizeof(known_facts); i++) {
        // Basically overwriting the previous entry in the list with the next one
        // Could be a problem with the final entry, but we'll get to that later
        // known_facts[i] = known_facts[i + 1];
    //}
}

// Retrieve information already stored in WorkingMemory
int WorkingMemory::recall(int key) {
    std::cout << "Running WorkingMemory.recall()" << std::endl;
    //return known_facts[key];
    return 0;
}
