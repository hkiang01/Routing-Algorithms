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
	sourceNode->addNeighbor(*destNode);
	Link destLink = Link(sourceID, cost);
	destNode->addLink(destLink);
	destNode->addNeighbor(*sourceNode);
	return true;

}



bool Graph::changeLink(int sourceID, int destID, int newCost) {
	if(newCost<0){
		Node * sourceNode = this->findNode(sourceID);
		Node * destNode = this->findNode(destID);
		if(sourceNode==NULL||destNode==NULL)
			return false;
		for(std::vector<Node>::iterator it = sourceNode->neighbors.begin();it!=sourceNode->neighbors.end();it++){
			if(it->id==destID){
				sourceNode->removeLink(destID);
                		sourceNode->removeNeighbor(destID);
                		destNode->removeLink(sourceID);
                		destNode->removeNeighbor(sourceID);
                		return true;
			}
		}
		/**
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
		**/
		return false;
	}

	Node * sourceNode = this->findNode(sourceID);
	Node * destNode = this->findNode(destID);
	if(sourceNode == NULL || destNode == NULL) {
		//perror("Error - changeLink: source or dest node non-existant in graph\n");
		return false;
	}

	//find neighbor of sourceNode that is of id destID
	//std::vector<Node>::iterator it = std::find_if (sourceNode->neighbors.begin(), sourceNode->neighbors.end(), isNode(destID));
	//if(it!=sourceNode->neighbors.end()) {//no connection exists
		//perror("Error - changeLink: no connection exists between source and dest node\n");
		
	//	return false;
	//}
	for(std::vector<Node>::iterator it = sourceNode->neighbors.begin(); it!=sourceNode->neighbors.end();it++){
		if(it->id==destID){
			sourceNode->setLink(destID,newCost);
			destNode->setLink(destID,newCost);
			return true;
		}
	}
	return this->addLink(sourceID,destID,newCost);
	//sourceNode->setLink(destID, newCost);
	//destNode->setLink(sourceID, newCost);
	//return true;
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
	path.push_back(sourceID);
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
			Node * currNode = &(*it);
			for(std::vector<RouteTableEntry>::iterator itt = currNode->routeTable.begin(); itt!= currNode->routeTable.end(); ++itt) {
				RouteTableEntry *currBest = &(*itt);
				int currDest = currBest->dest;
				int currNext = currBest->next;
				int currCost = currBest->cost;
				if(itt->dest == currNode->id) continue;
				for(std::vector<Node>::iterator ittt = currNode->neighbors.begin(); ittt!=currNode->neighbors.end(); ++ittt) {
					Node *currNeighbor = this->findNode(ittt->id);
					RouteTableEntry *proposed = currNeighbor->getNextHop(currDest);
					int newCost = currNode->getLinkCost(currNeighbor->id) + proposed->cost;
					if(proposed->cost==-999) continue;
					if(newCost == currCost) {
						if(currNext > currNeighbor->id) {
							currNext = currNeighbor->id;
							changed = true;
							continue;
						}
						continue;
					}
					if(newCost < currCost || currCost < 0) {
						currNext = currNeighbor->id;
						currCost = newCost;
						changed = true;
					}
				}
				currNode->updateRouteTable(currDest, currNext, currCost);
			}
		}
	}

}
/*
void Graph::linkState() {

	for(std::vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		//Initialization
		Node *currNode = &(*it);
		std::vector<Node> knowns;
		knowns.push_back(*currNode);
		//for all nodes v, if v adjacent to u, then D(v) = c(u,v) else D(v) = INFINITY
		for(std::vector<Node>::iterator itt = this->nodes.begin(); itt != this->nodes.end(); ++itt) {
			Node *v = &(*itt);
			if(currNode->id == v->id) { //same node
				RouteTableEntry *entry = currNode->findRouteTableEntry(v->id);
				if(!entry) currNode->newRouteTableEntry(v->id, v->id, 0); //new route table entry if none exists
				else currNode->updateRouteTable(v->id, v->id, 0); //update cost to 0
			}
			else if (currNode->isNeighbor(v->id)) {
				RouteTableEntry *entry = currNode->findRouteTableEntry(v->id);
				if (!entry) currNode->newRouteTableEntry(v->id, v->id, currNode->getLinkCost(v->id));
				else currNode->updateRouteTable(v->id, v->id, currNode->getLinkCost(v->id));
			}
			else {
				RouteTableEntry *entry = currNode->findRouteTableEntry(v->id);
				if(!entry) currNode->newRouteTableEntry(v->id, -999, -999);
				else currNode->updateRouteTable(v->id, -999, -999);
			}
		}


		//Loop
		//find node w not in knowns such that distance from *currNode to w is minimum
		int minDist = INT_MAX;
		Node *w;
		for(std::vector<Node>::iterator itt = this->nodes.begin(); it!= this->nodes.end(); ++it) {
			Node *candidate = &(*itt);
			int dist = currNode->getLinkCost(candidate->id);
			if(dist < minDist && dist!= -999) {
				bool inKnowns = false;
				for(std::vector<Node>::iterator ittt = knowns.begin(); ittt != knowns.end(); ++ittt) {
					Node *knownNode = &(*ittt);
					if(knownNode->id==candidate->id){
						inKnowns = true;
						break;
					}
				}
				if(!inKnowns) {
					minDist = dist;
					w = candidate;
				}
			}
		}
		//At this point, w points to node not in knowns whose distance from currNode is least
		if(w) {
			knowns.push_back(*w);
			//update D(v) for all v adjacent to w and not in knowns
			for(std::vector<Node>::iterator itt = w->neighbors.begin(); itt != w->neighbors.end(); ++itt) {
				Node *currNeighbor = &(*itt);
				for(std::vector<Node>::iterator ittt = knowns.begin(); ittt != knowns.end(); ++it) {
					Node *currKnown = &(*ittt);
					if(currNeighbor->id != currKnown->id) {
						Node *toUpdate = this->findNode(currNeighbor->id);
						int currDist = currNode->getLinkCost(toUpdate->id);
						int newDist = currNode->getLinkCost(w->id) + w->getLinkCost(toUpdate->id);
						if(newDist < currDist) {
							toUpdate->updateRouteTable(toUpdate->id, w->id, newDist);
						}
					}
				}
			}
		}
	}

}
*/

void Graph::linkState() {
	for(std::vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {

		Node *currNode = &(*it);

		std::cout << "Node " << currNode->id << "'s route table size: " << currNode->routeTable.size() << std::endl;
		std::cout << currNode->printRoutingTableInOrder() << std::endl;
		//Initialization
		std::vector<Node> knowns;
		knowns.push_back(*currNode);
		//Initialization already done in linkstate.cpp's call to initRoutingTables
		//TODO: INITIALIZATION (necessary for subsequent calls)
		bool done = false; //one iter of while
		while(knowns.size() != nodes.size() && !done) {

			//print knowns
			std::cout << "Knowns: ";
			for(std::vector<Node>::iterator itt = knowns.begin(); itt != knowns.end(); ++itt) {
				Node *currKnown = &(*itt);
				std::cout << currKnown->id << " ";
			}
			std::cout << std::endl;

			Node *w;
			int minDist = INT_MAX;
			//find w not in knowns such that D(w) is minimum (example should find node with id 4)
			for(std::vector<Node>::iterator itt = this->nodes.begin(); itt != this->nodes.end(); ++itt) {
				Node *candidate = &(*itt);
				int dist = currNode->getLinkCost(candidate->id);
				//std::cout << "Distance from " << currNode->id << " to " << candidate->id << ": " << dist << std::endl;
				if(dist < minDist && dist != -999) {
					for(std::vector<Node>::iterator ittt = knowns.begin(); ittt != knowns.end(); ++ittt) {
						Node *currKnown = &(*ittt);
						if(currKnown->id != candidate->id && currNode->id != candidate->id) {
							//std::cout << "Node " << currNode->id << " to node " << candidate->id << ": " << dist << " (not in knowns)" << std::endl;
							minDist = dist;
							w = candidate;
						}
					}
				}	
			}

			//w points to node not in knowns such that D(w) is a minimum
			if(w) {
				std::cout << "w: " << w->id << " distance: " << minDist << std::endl;
				knowns.push_back(*w);


				//print knowns
				std::cout << "Knowns: ";
				for(std::vector<Node>::iterator itt = knowns.begin(); itt != knowns.end(); ++itt) {
					Node *currKnown = &(*itt);
					std::cout << currKnown->id << " ";
				}
				std::cout << std::endl;


				//update D(v) for all v adjacent to w and not in knowns
				std::cout << "w's neighbors..." << std::endl;
				for(std::vector<Node>::iterator itt = w->neighbors.begin(); itt != w->neighbors.end(); ++itt) {
					Node *currNeighbor = &(*itt);
					std::cout << currNeighbor->id << " " << std::endl;
					for(std::vector<Node>::iterator ittt = knowns.begin(); ittt != knowns.end(); ++ittt) {
						Node *currKnown = &(*ittt);
						if(currKnown->id != currNeighbor->id && currNode->id != currNeighbor->id) {
							std::cout << "w's neighbor " << currNeighbor->id << " not in knowns, update" << std::endl;
						}
					}
				}
				std::cout << std::endl << "done printing w's neighbors" << std::endl;;
			}

			done = true; //one iter of while
		}
		break;
	}	
}
