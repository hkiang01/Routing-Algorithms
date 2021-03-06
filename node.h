#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdio.h>

#include "link.h"
#include "routetableentry.h"

class Node {

	public:

		Node();
		Node(int id_in);
		Node(int id_in, std::vector<Link> links_in,
		std::vector<RouteTableEntry> routeTable_in,
		std::vector<Node> neighbors_in);
		void printRouteTable();
		std::string printRoutingTableInOrder();
		void addLink(Link link_in);
		bool setLink(int link_id, int new_cost);
		bool removeLink(int link_id);
		int getLinkCost(int link_id);
		void printLinks();
		void addNeighbor(Node neighbor_in);
		bool removeNeighbor(int neighborID);
		bool isNeighbor(int neighborID);
		void printNeighbors();
		RouteTableEntry * findRouteTableEntry(int dest_id);
		RouteTableEntry * getNextHop(int dest_id);
		void initRoutingTables(int maxNode);
		void newRouteTableEntry(int dest_in, int next_in, int cost_in);
		void localUpdate();
		bool updateRouteTable(int dest_in, int next_in, int cost_in);

		int id;
		std::vector<Link> links;
		std::vector<RouteTableEntry> routeTable;
		std::vector<Node> neighbors;
};

#endif
