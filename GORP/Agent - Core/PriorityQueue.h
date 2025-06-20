#pragma once

class PriorityQueue {
public:
	// var precedes:Callable
	// var heap:Array
	
	//void _init(f: Callable);
	// Observers
	bool is_empty();
	void front();
};

class Element {
public:
	// Original is just "var key" and "var value", so I'm guessing at the data type
	int key;
	int value;
	void _init(int k, int v) {
		key = k;
		value = v;
	}
};
/*
# Auxiliary observers

func left(i:int) -> int:
	return 2 * (i+1) - 1

func parent(i:int) -> int:
	return floor((i - 1) / 2) # floor is redundant here, but retained for clarity of purpose

func right(i:int) -> int:
	return 2 * (i+1)

# Mutators

## Inserts an new element into the queue while preserving the heap property
func insert(x:Variant, priority:int):
	heap.push_back(null)
	update_key(heap.size() - 1, Element.new(priority, x))

## Removes and returns the element of the queue with the highest priority
func extract() -> Variant:
	if heap.size() < 1:
		return null # error “heap underflow”
	var extremum = heap[0].value
	heap[0] = heap.back()
	heap.pop_back()
	heapify(0)
	return extremum

# Auxiliary mutators

## Maintains the heap property by floating the root down to its proper position
func heapify(i:int):
	var l = left(i)
	var r = right(i)
	var extremum:int
	if l < heap.size() and heap[l].key < heap[i].key:
		extremum = l
	else:
		extremum = i
	if r < heap.size() and heap[r].key < heap[extremum].key:
		extremum = r
	if extremum != i:
		var tmp = heap[i]
		heap[i] = heap[extremum]
		heap[extremum] = tmp
		heapify(extremum)

## Updates the value of an element's key without violating the heap property
func update_key(i:int, elem:Element):
	var prev_key = heap[i].key if heap[i] else INF
	heap[i] = elem
	if prev_key < elem.key:
		heapify(i)
		return
	var j = parent(i)
	while i > 0 and heap[i].key < heap[j].key:
		#print("        " + str(i))
		var tmp = heap[i]
		heap[i] = heap[j]
		heap[j] = tmp
		i = j
		j = parent(i)
*/