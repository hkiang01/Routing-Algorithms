#include "link.h"

Link::Link() {
	destID = -1;
	cost = -999;
}

Link::Link(int destID_in, int cost_in) {
	destID = destID_in;
	cost = cost_in;
}

void Link::printLink() {
	std::cout << destID << " " << cost << std::endl;
}