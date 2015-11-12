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

Node * Graph::findNode(int nodeID) {
	Node temp;
	std::vector<Node>::iterator it = std::find_if (nodes.begin(), nodes.end(), isNode(nodeID));
	if(it!=nodes.end()) {
		return &(*it);
	}
	return NULL;
}

bool Graph::addLink(int sourceID, int destID, int cost) {
	if(cost == -999)
		return false;

	Node * sourceNode = this->findNode(sourceID);
	Node * destNode = this->findNode(destID);
	if(sourceNode == NULL || destNode == NULL)
		return false;

	Link sourceLink = Link(destID, cost);
	sourceNode->addLink(sourceLink);
	Link destLink = Link(sourceID, cost);
	destNode->addLink(destLink);
	return true;

}



bool Graph::changeLink(int sourceID, int destID, int newCost) {
	if(newCost <0){
		Node * sourceNode = this->findNode(sourceID);
		Node * destNode = this->findNode(destID);
		if(sourceNode==NULL||destNode==NULL)
			return false;
		std::vector<Node>::iterator it = std::find_if (sourceNode->neighbors.begin(), sourceNode->neighbors.end(), isNode(destID));
        	if(it!=sourceNode->neighbors.end()) {//no connection exists
                //perror("Error - changeLink: no connection exists between source and dest node\n");
                	return false;
        	}
		sourceNode->removeLink(destID);
		sourceNode->removeNeighbor(destID);
		destNode->removeLink(sourceID);
		destNode->removeNeighbor(sourceID);
		return true;
	}

	Node * sourceNode = this->findNode(sourceID);
	Node * destNode = this->findNode(destID);
	if(sourceNode == NULL || destNode == NULL) {
		//perror("Error - changeLink: source or dest node non-existant in graph\n");
		return false;
	}

	//find neighbor of sourceNode that is of id destID
	std::vector<Node>::iterator it = std::find_if (sourceNode->neighbors.begin(), sourceNode->neighbors.end(), isNode(destID));
	if(it!=sourceNode->neighbors.end()) {//no connection exists
		//perror("Error - changeLink: no connection exists between source and dest node\n");
		return false;
	}

	sourceNode->setLink(destID, newCost);
	destNode->setLink(sourceID, newCost);
	return true;
}

void Graph::addLine(int sourceID, int destID, int cost){
	if(cost<0){
		return;
	}
	if(!this->findNode(sourceID)){
		Node s(sourceID);
		this->addNode(s);
	}
	if(!this->findNode(destID)){
		Node d(destID);
		this->addNode(d);
	}
	this->addLink(sourceID,destID,cost);
}

int Graph::routeCost(int sourceID, int destID) {
	if(sourceID < 0 || destID < 0)
		return -999;
	if(sourceID == destID)
		return 0;
	Node * node = this->findNode(sourceID);
	RouteTableEntry * entry = node->getNextHop(destID);
	return entry->cost;
}

std::vector<int> Graph::routePath(int sourceID, int destID, std::vector<int> path) {
	if(sourceID < 0 || destID < 0 || sourceID == destID)
		return path;
	Node * node = this->findNode(sourceID);
	RouteTableEntry * entry = node->getNextHop(destID);
	int nextHop = entry->next;
	if(nextHop<0){
		std::vector<int> t;
		return t;
	}
	path.push_back(nextHop);
	return routePath(nextHop, destID, path);
}

void Graph::distVector() {
	bool changed = true;
	for(std::vector<Node>::iterator it = this->nodes.begin(); it!=this->nodes.end(); ++it) {
		it->localUpdate();
	}
	while(changed) {
		changed = false;
		for(std::vector<Node>::iterator it = this->nodes.begin(); it!=this->nodes.end(); ++it) {
			Node *currNode = &(*it);
			for(std::vector<RouteTableEntry>::iterator itt = currNode->routeTable.begin(); itt!= currNode->routeTable.end(); ++itt) {
				RouteTableEntry *currBest = &(*itt);
				if(itt->dest == currNode->id) continue;
				for(std::vector<Node>::iterator ittt = currNode->neighbors.begin(); it!=currNode->neighbors.end(); ++ittt) {
					Node *currNeighbor = &(*ittt);
					RouteTableEntry *proposed = currNeighbor->getNextHop(currBest->dest);
					int newCost = currNode->getLinkCost(currNeighbor->id) + proposed->cost;
					if(proposed->cost==-999) continue;
					if(newCost == currBest->cost) {
						if(currBest->next > currNeighbor->id) {
							currBest->next = currNeighbor->id;
							changed = true;
							continue;
						}
						continue;
					}
					if(newCost < currBest->cost || currBest->cost < 0) {
						currBest->next = currNeighbor->id;
						currBest->cost = newCost;
						changed = true;
					}
				}
				currNode->updateRouteTable(currBest->dest, currBest->next, currBest->cost);
			}
		}
	}

}
