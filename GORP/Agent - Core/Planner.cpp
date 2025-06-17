#include "Planner.h"

// Distance between two states


/*float distance(WorldState src, WorldState dst) {
	float d = 0;
	for (int key = 0; key < sizeof(dst.properties); key++) {
		// Figure out the C++ equivalent of .has
		if (src.has(key) == false) {
			d += 1; // one property needs to be added
		}
		// Figure out c++ equivalent of .get_property
		else if ((src.get_property(key) == dst.get_property(key)) == false) {
			d += 1; // one property needs to be changed
		};
	};
	return d;
};*/