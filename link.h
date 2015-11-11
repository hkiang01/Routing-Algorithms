#ifndef LINK_H
#define LINK_H

#include <iostream>
#include <vector>


class Link {

	public:
		Link();
		Link(int destID_in, int cost);
		void printLink();

		int destID;
		int cost;
};

#endif
