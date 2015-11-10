#ifndef LINK_H
#define LINK_H

#include <iostream>
#include <vector>


class Link {

	Link();
	Link(int destID_in, int cost);

	public:
		int destID;
		int cost;
};

#endif
