#include "routetableentry.h"

RouteTableEntry::RouteTableEntry() {
	dest = -1;
	src = -1;
	cost = -1;
}

RouteTableEntry::RouteTableEntry(int dest_in, int src_in, int cost_in) {
	dest = dest_in;
	src = src_in;
	cost = cost_in;
}