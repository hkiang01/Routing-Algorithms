#include <iostream>
#include <string>
#include <sstream>
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

string sendMessage(int i){
	int numMessage = ::fh->getNumMessages();
	if(i>=numMessage)
		return "";
	string message = ::fh->getMessage(i);
	int source = ::fh->getSource(i);
	int dest = ::fh->getDest(i);
	int cost = ::g->routeCost(source,dest);
	if(cost<0){
		stringstream sstm;
		sstm<<"from "<<source<<" to "<<dest<<" hops unreachable message "<<message<<endl;
		return sstm.str();
	}
	vector<int> path = ::g->routePath(source,dest,vector<int>());
	stringstream sstm;
	sstm<<"from "<<source<<" to "<<dest<<" hops";
	for(vector<int>::iterator it = path.begin();it!=path.end(); it++){
		sstm<<" "<<*it;
	}
	sstm<<" message "<<message<<endl;
	return sstm.str();
}

void loadFiles(string topo, string messages, string changes){
	::fh = new FileHandler();
	::fh->parse(topo,"TOPO_FILE");
	::fh->parse(messages,"MESSAGES_FILE");
	::fh->parse(changes,"CHANGES_FILE");
}

int main(int argc, char * argv []) {
	if(argc!=4){
		return 1;
	}
	string topo(argv[1]);
	string messages(argv[2]);
	string changes(argv[3]);
	loadFiles(topo,messages,changes);
	loadGraph();
	stringstream ss;
	
	int numMessages = ::fh->getNumMessages();
	int i = 0;
	int totalNodes = ::g->nodes.size();
	for(i=1;i<=totalNodes;i++){
		g->findNode(i)->initRoutingTables(totalNodes);
	}
	::g->distVector();
	for(i=1;i<=totalNodes;i++){
		ss<<::g->findNode(i)->printRoutingTableInOrder();
	}	
	for(i=0;i<numMessages;i++){
		ss<<sendMessage(i);
	}
	while(makeChange()){
		::g->distVector();
		for(i=1;i<=totalNodes;i++){
			ss<<::g->findNode(i)->printRoutingTableInOrder();
		}
		for(i=0;i<numMessages;i++){
			ss<<sendMessage(i);
		}
	}
	std::cout<<ss.str();
	free(::fh);
	free(::g);
	return 0;
}
