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

std::vector<int> Graph::routeTotalPath(int sourceID, int destID, std::vector<int> path) {
	if(sourceID < 0 || destID < 0)
		return path;
	Node *sourceNode = this->findNode(sourceID);
	
	path.push_back(sourceID);

	int hop = sourceNode->findRouteTableEntry(destID)->next;
	while(hop != destID) {
		path.push_back(hop);
		sourceNode = this->findNode(hop);
		hop = sourceNode->findRouteTableEntry(destID)->next;
	}
	path.push_back(destID);
	return path;
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

void Graph::linkState() {
	for(std::vector<Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {

		Node *currNode = &(*it);
		Node *u = currNode;

		std::cout << "Node " << currNode->id << "'s route table size: " << currNode->routeTable.size() << std::endl;
		std::cout << currNode->printRoutingTableInOrder() << std::endl;
		//Initialization
		std::vector<Node> knowns;
		knowns.push_back(*currNode);
		//Initialization already done in linkstate.cpp's call to initRoutingTables
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
				//int dist = currNode->getLinkCost(candidate->id);
				RouteTableEntry *currEntry = currNode->findRouteTableEntry(candidate->id);
				int dist = currEntry->cost;
				std::cout << "Distance from " << currNode->id << " to " << candidate->id << ": " << dist << std::endl;
				if(dist < minDist && dist != -999) {
					bool inKnowns = false;
					for(std::vector<Node>::iterator ittt = knowns.begin(); ittt != knowns.end(); ++ittt) {
						Node *currKnown = &(*ittt);
						if(currKnown->id == candidate->id) {
							inKnowns = true;
							break;
						}
					}
					if(!inKnowns) {
						std::cout << "Node " << currNode->id << " to node " << candidate->id << ": " << dist << " (" << candidate->id << " not in knowns)" << std::endl;
						minDist = dist;
						w = candidate;
					}
				}	
			}

			//w points to node not in knowns such that D(w) is a minimum
			if(w) {
				std::cout << "w: " << w->id << " distance: " << minDist << std::endl;
				knowns.push_back(*w);
				std::cout << "pushing w=" << w->id << " to knowns" << std::endl;

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
					Node *v = &(*itt);
					std::cout << v->id << " " << std::endl;
					bool inKnowns = false;
					for(std::vector<Node>::iterator ittt = knowns.begin(); ittt != knowns.end(); ++ittt) {
						Node *currKnown = &(*ittt);
						if(currKnown->id == v->id) {
							inKnowns = true;
							break;
						}
					}
					if(!inKnowns) {
						std::cout << "w's neighbor " << v->id << " not in knowns, checking condition for v: " << v->id << std::endl;
						//update D(v) = min(D(v), D(w) + c(w,v))
						//int Dv = currNode->getLinkCost(v->id);
						RouteTableEntry *DvEntry = currNode->findRouteTableEntry(v->id);
						int Dv = DvEntry->cost;
						//int Dw = currNode->getLinkCost(w->id);
						RouteTableEntry *DwEntry = currNode->findRouteTableEntry(w->id);
						int Dw = DwEntry->cost;
						//int cwv = w->getLinkCost(v->id);
						RouteTableEntry *cwvEntry = w->findRouteTableEntry(v->id);
						int cwv = cwvEntry->cost;
						std::cout << "D(v)=D(" << v->id << "): " << Dv << " D(w)=D(" << w->id << "): " << Dw << " c(w,v)=c(" << w->id << "," << v->id << "): " << cwv << std::endl;
						if( (Dv == -999) || (Dw != -999 && (Dw + cwv) < Dv) ) {
							std::cout << "Condition met, update!" << std::endl;
							RouteTableEntry *staleRoute = u->findRouteTableEntry(v->id);
							std::cout << "Stale route: (dest, next, cost)=(" << staleRoute->dest << ", " << staleRoute->next << ", " << staleRoute->cost << ")" << std::endl;
							std::cout << "entry to update: " << v->id << std::endl;

							//set D(v) <-- D(w) + c(w,v)
							int newCost = Dw + cwv;
							std::cout << "new cost: " << newCost << std::endl;

							//set router table entry for v to approprate next
							//want a path from u to v
							//base case: if u and v are neighbors,
							//then node u's router table entry for v has next = v
							//else, call get path from u to w
							//path will be of form u -> a -> ... -> z -> w
							//set node u's router table entry for v to have next = a (after u in path)
							int newNext;
							std::vector<int> currPath;
							currPath = this->routeTotalPath(u->id, w->id, currPath);
							currPath.push_back(v->id);
							if(!currPath.empty()) { //if there is a valid path
								std::cout << "path from " << u->id << " to " << v->id << ": ";
								for(std::vector<int>::iterator ittt = currPath.begin(); ittt != currPath.end(); ++ittt) {
									std::cout << *ittt << " ";
								}
								std::cout << std::endl;
								newNext = currPath[1];
								std::cout << "new next: " << newNext << std::endl;
								u->updateRouteTable(v->id, newNext, newCost);
								RouteTableEntry *newRoute = u->findRouteTableEntry(v->id);
								std::cout << "New route: (dest, next, cost)=(" << newRoute->dest << ", " << newRoute->next << ", " << newRoute->cost << ")" << std::endl;
								continue;
							}
						}
						std::cout << "node " << v->id << " updated" << std::endl;
					}
				}
				std::cout << std::endl << "done updating w's neighbors" << std::endl;
				std::cout << currNode->printRoutingTableInOrder() << std::endl;
			}
			//done = true; //one iter of while
		}
		std::cout << "FINAL ROUTING TABLE FOR NODE " << u->id << std::endl;
		std::cout << u->printRoutingTableInOrder() << std::endl;
		//break;
	}	
}
