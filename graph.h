#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include "node.h"

class Graph {
	public:
		Graph();
		Graph(int id_in, std::vector<Node> nodes_in);
		void addNode(Node node_in);
		Node findNode(int nodeID);
		bool addLink(int sourceID, int destID, int cost);
		bool changeLink(int sourceID, int destID, int newCost);

		int id;
		std::vector<Node> nodes;
};

#endif