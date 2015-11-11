#include "graph.h"

Graph::Graph() {
	id = -1;
}

Graph::Graph(int id_in, std::vector<Node> nodes_in) {
	id = id_in;
	for(std::vector<Node>::iterator it = nodes_in.begin(); it != nodes_in.end(); ++it) {
		nodes.push_back(*it);
	}
}

void Graph::addNode(Node node_in) {
	nodes.push_back(node_in);
}

struct isNode {
	isNode(int const& nodeID) : nodeID_(nodeID) { }
	bool operator () (Node const& n) { return n.id == nodeID_; }
private:
	int nodeID_;
};

Node Graph::findNode(int nodeID) {
	Node temp;
	std::vector<Node>::iterator it = std::find_if (nodes.begin(), nodes.end(), isNode(nodeID));
	if(it!=nodes.end()) {
		return *it;
	}
	return temp;
}

bool Graph::addLink(int sourceID, int destID, int cost) {
	if(cost == -999)
		return false;

	Node sourceNode = this->findNode(sourceID);
	Node destNode = this->findNode(destID);
	if(sourceNode.id == -1 || destNode.id == -1)
		return false;

	Link sourceLink = Link(destID, cost);
	sourceNode.addLink(sourceLink);
	Link destLink = Link(sourceID, cost);
	destNode.addLink(destLink);
	return true;

}

bool Graph::changeLink(int sourceID, int destID, int newCost) {
	if(newCost == -999)
		return false;

	Node sourceNode = this->findNode(sourceID);
	Node destNode = this->findNode(destID);
	if(sourceNode.id == -1 || destNode.id == -1) {
		perror("Error - changeLink: source or dest node non-existant in graph\n");
		return false;
	}

	//find neighbor of sourceNode that is of id destID
	std::vector<Node>::iterator it = std::find_if (sourceNode.neighbors.begin(), sourceNode.neighbors.end(), isNode(destID));
	if(it!=sourceNode.neighbors.end()) {//no connection exists
		perror("Error - changeLink: no connection exists between source and dest node\n");
		return false;
	}

	sourceNode.setLink(destID, newCost);
	destNode.setLink(sourceID, newCost);
	return true;
}

int Graph::routeCost(int sourceID, int destID) {
	if(sourceID < 0 || destID < 0)
		return -999;
	if(sourceID == destID)
		return 0;
	Node node = this->findNode(sourceID);
	RouteTableEntry entry = node.getNextHop(destID);
	return entry.cost;
}

std::vector<int> Graph::routePath(int sourceID, int destID, std::vector<int> path) {
	if(sourceID < 0 || destID < 0 || sourceID == destID)
		return path;
	Node node = this->findNode(sourceID);
	RouteTableEntry entry = node.getNextHop(destID);
	int nextHop = entry.next;
	path.push_back(nextHop);
	return routePath(nextHop, destID, path);
}