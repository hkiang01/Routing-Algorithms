#include <iostream>
#include <string>
#include <stdlib.h>
#include "node.h"
#include "link.h"
#include "graph.h"
#include "routetableentry.h"
#include "file_core.h"
using namespace std;
FileHandler * fh;
Graph * g;
int currChange = 0;

bool loadGraph(){
	::g = new Graph();
	int numInit = ::fh->getNumInit();
	int i = 0;
	int * line;
	for(i=0;i<numInit;i++){
		line = ::fh->getGraph(i);
		::g->addLine(line[0],line[1],line[2]);
	}
	return true;
}

bool makeChange(){
	int numChange = ::fh->getNumChanges();
	if(::currChange>=numChange)
		return false;
	int * line = ::fh->getChange(::currChange);
	::g->changeLink(line[0],line[1],line[2]);
	::currChange++;
	return true;
}


void loadFiles(string topo, string messages, string changes){
	::fh = new FileHandler();
	::fh->parse(topo,"TOPO_FILE");
	::fh->parse(messages,"MESSAGES_FILE");
	::fh->parse(changes,"CHANGES_FILE");
}

int main(int argc, char * argv []) {
	if(argc!=4){
		cout<<"Error, wrong number of arguments"<<endl;
		return 1;
	}
	string topo(argv[1]);
	string messages(argv[2]);
	string changes(argv[3]);
	loadFiles(topo,messages,changes);
	loadGraph();
	std::cout << "test" << std::endl;
	free(::fh);
	free(::g);
	return 0;
}
