#ifndef ROUTETABLEENTRY_H
#define ROUTETABLEENTRY_H

#include <iostream>
#include <vector>

class RouteTableEntry {

	public:
		int dest;
		int next;
		int cost;
};

#endif