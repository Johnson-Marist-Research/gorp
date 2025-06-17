#include "Planner.h"

// Distance between two states


float distance(WorldState src, WorldState dst) {
	float d = 0;
	for (int key : dst.properties) {
		// Figure out the C++ equivalent of .has
		if not src.has(key) {
			d += 1 // one property needs to be added
		};
		// Figure out c++ equivalent of .get_property
		else if (not src.get_property(key) == dst.get_property(key)) {
			d += 1 // one property needs to be changed
		};
	};
	return d
};