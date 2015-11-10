#ifndef ROUTETABLEENTRY_H
#define ROUTETABLEENTRY_H

#include <iostream>
#include <vector>

class RouteTableEntry {

	public:
		RouteTableEntry();
		RouteTableEntry(int dest_in, int src_in, int cost_in);
		void printEntry();

		int dest;
		int next;
		int cost;
};

#endif
