#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include "node.h"

class Graph {
	public:
		Graph();
		Graph(int id_in, std::vector<Node> nodes_in);

		int id;
		std::vector<Node> nodes;
};

#endif