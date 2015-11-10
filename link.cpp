#include "link.h"

Link::Link() {
	destID = -1;
	cost = -1;
}

Link::Link(int destID_in, int cost_in) {
	destID = destID_in;
	cost = cost_in;
}