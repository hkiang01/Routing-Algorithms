#include "routetableentry.h"

RouteTableEntry::RouteTableEntry() {
	dest = -1;
	next = -1;
	cost = -1;
}

RouteTableEntry::RouteTableEntry(int dest_in, int next_in, int cost_in) {
	dest = dest_in;
	next = next_in;
	cost = cost_in;
}

void RouteTableEntry::printEntry() {
	std::cout << dest << " " << next << " " << cost << std::endl;
}