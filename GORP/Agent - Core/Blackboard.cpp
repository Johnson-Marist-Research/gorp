#include "Blackboard.h"

/*Blackboard::Blackboard()
{
    // Maybe we can set the size of instructions[] here
}*/

void Blackboard::post(int key, std::string instruction)
{
    instructions[key] = instruction;
    // This is where we would have the signal emit a "poster.emit(key)" signal
}

std::string Blackboard::fetch(int key) {
    return instructions[key];
}