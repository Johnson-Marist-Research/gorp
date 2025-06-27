#include "Blackboard.h"

#include <iostream>

/*Blackboard::Blackboard()
{
    // Maybe we can set the size of instructions[] here
}*/

// Posts a specific instruction to the Blackboard, as determined by the inputted key.
void Blackboard::post(int key, std::string instruction)
{
    std::cout << "Running Blackboard.post()" << std::endl;
    instructions[key] = instruction;
    // This is where we would have the signal emit a "poster.emit(key)" signal
}

// Returns a specific instruction from the Blackboard, as determined by the inputted key
std::string Blackboard::fetch(int key) {
    std::cout << "Running Blackboard.fetch()" << std::endl;
    return instructions[key];
}