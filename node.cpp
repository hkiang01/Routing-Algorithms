#include "node.h"


//default constructor
Node::Node()
{
	id = -1;
	//links.clear();
	//links = std::vector<Link>();
	//routeTable.clear();
	//routeTable = std::vector<RouteTableEntry>();
}

Node::Node(int id_in) {
	id = id_in;
	links.clear();
	links = std::vector<Link>();
	routeTable.clear();
	routeTable = std::vector<RouteTableEntry>();
}


Node::Node(int id_in, std::vector<Link> links_in,
	std::vector<RouteTableEntry> routeTable_in,
	std::vector<Node> neighbors_in) {
	id = id_in;
	for(std::vector<Link>::iterator it = links_in.begin(); it != links_in.end(); ++it) {
		links.push_back(*it);
	}
	for(std::vector<RouteTableEntry>::iterator it = routeTable_in.begin(); it!= routeTable_in.end(); ++it) {
		routeTable.push_back(*it);
	}
	for(std::vector<Node>::iterator it = neighbors_in.begin(); it != neighbors_in.end(); ++it) {
		neighbors.push_back(*it);
	}
}

void Node::printRouteTable() {
	std::cout << "Printing Node " << id << "'s routing table..." << std::endl;
	for(std::vector<RouteTableEntry>::iterator it = routeTable.begin(); it != routeTable.end(); ++it) {
		it->printEntry();
	}
}

void Node::printLinks() {
	std::cout << "Printing links attached to Node " << id << "..." << std::endl;
	for(std::vector<Link>::iterator it = links.begin(); it != links.end(); ++it) {
		it->printLink();
	}
}

void Node::printNeighbors() {
	std::cout << "Printing neighbors of Node " << id << "..." << std::endl;
	for(std::vector<Node>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
		std::cout << it->id << " ";
	}
	std::cout << std::endl;
}

struct isNode
{
	isNode(int const& node_id) : node_id_(node_id) {  }
	bool operator () (Node const& n) { return n.id == node_id_;}
private:
	int node_id_;
};

bool Node::removeNeighbor(int neighborID) {
	std::vector<Node>::iterator it = std::find_if (neighbors.begin(), neighbors.end(), isNode(neighborID));
	if(it!=neighbors.end()) { //node found, can be deleted
		neighbors.erase(it);
		return true;
	}
	return false;
}

bool Node::isNeighbor(int neighborID) {
	std::vector<Node>::iterator it = std::find_if (neighbors.begin(), neighbors.end(), isNode(neighborID));
	if(it!=neighbors.end()) { //node found, can be deleted
		return true;
	}
	return false;
}

void Node::addLink(Link link_in) {
	links.push_back(link_in);
}

void Node::addNeighbor(Node neighbor_in) {
	neighbors.push_back(neighbor_in);
}

struct isLink
{
	//source: http://stackoverflow.com/questions/15517991/search-a-vector-of-objects-by-object-attribute
	isLink(int const& link_id) : link_id_(link_id) { }
	bool operator () (Link const& l) { return l.destID == link_id_; }
private:
	int link_id_;
};

bool Node::setLink(int link_id, int new_cost) {
	  std::vector<Link>::iterator it = std::find_if (links.begin(), links.end(), isLink(link_id));
	  //it points to a Link in links
	  if(it!=links.end()) { //link found
	  	it->cost = new_cost;
	  	return true;
	  }
	  //link not found
	  return false;
}

bool Node::removeLink(int link_id) {
	  std::vector<Link>::iterator it = std::find_if (links.begin(), links.end(), isLink(link_id));
	  //it points to a Link in links
	  if(it!=links.end()) { //link found, can be removed
	  	links.erase(it);
	  	return true;
	  }
	  return false;
}

int Node::getLinkCost(int link_id) {
	std::vector<Link>::iterator it = std::find_if (links.begin(), links.end(), isLink(link_id));
	if(it!=links.end()) { //link found
		return it->cost;
	}
	if(link_id==id) return 0;
	//link not found
	return -999;
}

struct isRoute
{
	isRoute(int const& dest_id) : dest_id_(dest_id) { }
	bool operator () (RouteTableEntry const& l) { return l.dest == dest_id_; }
private:
	int dest_id_;
};

RouteTableEntry * Node::findRouteTableEntry(int dest_id) {
	//RouteTableEntry route; //blank route table entry
	//std::vector<RouteTableEntry>::iterator it = std::find_if (routeTable.begin(), routeTable.end(), isRoute(dest_id));
	for(std::vector<RouteTableEntry>::iterator it = routeTable.begin();it!=routeTable.end();++it){
		if((*it).dest==dest_id)
			return &(*it);
	}
	return NULL;
	//if(it!=routeTable.end()) {
	//	return &(*it);
	//}
	//return NULL;
}

RouteTableEntry * Node::getNextHop(int dest_id) {
	return this->findRouteTableEntry(dest_id);
	/**
	RouteTableEntry route; //blank route table entr
	std::vector<RouteTableEntry>::iterator it = std::find_if (routeTable.begin(), routeTable.end(), isRoute(dest_id));
	if(it!=routeTable.end()) {
		return &(*it);
	}
	return NULL;
	**/
}

std::string Node::printRoutingTableInOrder() {
	std::string retString;
	std::stringstream ss;
	std::vector<int> destIDs;
	for(std::vector<RouteTableEntry>::iterator it = routeTable.begin(); it!=routeTable.end(); ++it) {
		destIDs.push_back(it->dest);
	}
	std::sort(destIDs.begin(), destIDs.end());
	for(std::vector<int>::iterator it=destIDs.begin(); it!= destIDs.end(); ++it) {
		RouteTableEntry *entry = this->findRouteTableEntry(*it);
		ss<<entry->dest<<" "<<entry->next<<" "<<entry->cost<<std::endl;
	}

	return ss.str();
}


void Node::initRoutingTables(int maxNode) {
	int i;
	for(i=1; i<=maxNode; i++) {
		RouteTableEntry * entry = new RouteTableEntry(i, i, this->getLinkCost(i));
		routeTable.push_back(*entry);
	}
}

void Node::localUpdate() {
	for(std::vector<RouteTableEntry>::iterator it = routeTable.begin(); it!= routeTable.end(); ++it) {
		it->next = it->dest;
		if(this->getLinkCost(it->dest)<0){
			it->next = -999;
		}
		it->cost = this->getLinkCost(it->dest);
	}
}

bool Node::updateRouteTable(int dest_in, int next_in, int cost_in) {
	RouteTableEntry *entry = this->findRouteTableEntry(dest_in);
	if(entry) {
		entry->next = next_in;
		entry->cost = cost_in;
		return true;
	}
	return false;
}

void Node::newRouteTableEntry(int dest_in, int next_in, int cost_in) {
	RouteTableEntry entry = RouteTableEntry(dest_in, next_in, cost_in);
	routeTable.push_back(entry);
}
