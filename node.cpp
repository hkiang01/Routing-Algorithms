#include "node.h"

//default constructor
Node::Node()
{
	id = -1;
	links = std::vector<Link>();
	routeTable = std::vector<RouteTableEntry>();
}

Node::Node(int id_in) {
	id = id_in;
	links = std::vector<Link>();
	routeTable = std::vector<RouteTableEntry>();
}

Node::Node(int id_in, std::vector<Link> links_in, std::vector<Link> routeTable_in) {
	id = id_in;
	links = links_in;
	routeTable = routeTable_in;
}