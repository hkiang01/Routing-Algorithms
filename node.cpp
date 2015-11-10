#include "node.h"

//default constructor
Node::Node()
{
	id = -1;
	//links = std::vector<Link>();
	//routeTable = std::vector<RouteTableEntry>();
}

Node::Node(int id_in) {
	id = id_in;
	//links = std::vector<Link>();
	//routeTable = std::vector<RouteTableEntry>();
}

Node::Node(int id_in, std::vector<Link> links_in, std::vector<RouteTableEntry> routeTable_in) {
	id = id_in;
	for(std::vector<Link>::iterator it = links_in.begin(); it != links_in.end(); ++it) {
		links.push_back(*it);
	}
	for(std::vector<RouteTableEntry>::iterator it = routeTable_in.begin(); it!= routeTable_in.end(); ++it) {
		routeTable.push_back(*it);
	}
}
