#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>

#include "link.h"
#include "routetableentry.h"

class Node {

	public:

		Node();
		Node(int id_in);
		Node(int id_in, std::vector<Link> links_in, std::vector<RouteTableEntry> routeTable_in);

		

		int id;
		std::vector<Link> links;
		std::vector<RouteTableEntry> routeTable;
};

#endif
