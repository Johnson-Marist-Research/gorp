#include "PriorityQueue.h"

#include <limits>

bool PriorityQueue::is_empty() {
	if ((heapStart == 0) && (heapEnd == -1)) {
		return true;
	}
	else {
		return false;
	}
};

Element* PriorityQueue::front() {
	return heap[0];
};

int PriorityQueue::left(int i) {
	return 2 * (i + 1) - 1;
};

int PriorityQueue::parent(int i) {
	return floor((i - 1) / 2); // floor is redundant here, but retained for clarity of purpose
};

int PriorityQueue::right(int i) {
	return 2 * (i + 1);
};

void PriorityQueue::insert(std::string x, int priority) {
	Element newElement = newElement._init(priority, x);
	update_key(sizeof(heap) - 1, newElement);
	heapEnd++;
};

Element* PriorityQueue::extract() {
	if (sizeof(heap) < 1){
		return_null(); // error “heap underflow”
		return;
	}
	Element* extremum = new Element();
	extremum = heap[0];
	heap[0] = heap[heapEnd];
	heapEnd--;
	heapify(0);
	return extremum;
};

void PriorityQueue::heapify(int i) {
	int l = left(i);
	int r = right(i);
	int extremum;
	if ((l < sizeof(heap)) && (heap[l]->key < heap[i]->key)) {
		extremum = l;
	}
	else {
		extremum = i;
	}
	if ((r < sizeof(heap)) && (heap[r]->key < heap[extremum]->key)) {
		extremum = r;
	}
	if (extremum != i) {
		Element* tmp = heap[i];
		heap[i] = heap[extremum];
		heap[extremum] = tmp;
		heapify(extremum);
	}
};

void PriorityQueue::update_key(int i, Element nonPointerElem) {
	// Pointer points to class
	Element* elem = &nonPointerElem;
	// var prev_key = heap[i].key if heap[i] else INF
	int prev_key;
	if (heapEnd > 0) {
		prev_key = heap[i]->key;
	}
	else {
		// This should be the C++ code corresponding to infinity...
		prev_key = std::numeric_limits<int>::max();
	}
	heap[i] = elem;
	if (prev_key < elem->key) {
		heapify(i);
		return;
	}
	int j = parent(i);
	while ((i > 0) && (heap[i]->key < heap[j]->key)) {
		Element* tmp = heap[i];
		heap[i] = heap[j];
		heap[j] = tmp;
		i = j;
		j = parent(i);
	}
};

std::string PriorityQueue::return_extremum(std::string extremum) {
	return extremum;
}

void* PriorityQueue::return_null() {
	return NULL;
}