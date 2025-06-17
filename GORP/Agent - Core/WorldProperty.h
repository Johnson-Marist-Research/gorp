#pragma once

#include <string>

class WorldProperty {
public:
private:
	// subject was originally a Node: Find equivalent in C++

};
/*
## A game object this property relates to or is about
var subject:Node

## A unique identifier for this property of the world/character
var name:StringName

## The data value for this property
var value:bool

func _init(subject:Node, name:StringName, value:bool):
	self.subject = subject
	self.name = name
	self.value = value

func _to_string() -> String:
	return "<" + subject.name + "." + name + " = " + str(value) + ">"

func to_key() -> StringName:
	return subject.name + ": " + name
*/