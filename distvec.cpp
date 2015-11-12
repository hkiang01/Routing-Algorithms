#include <iostream>
#include <string>
#include <stdlib.h>
#include "node.h"
#include "link.h"
#include "routetableentry.h"
#include "file_core.h"
using namespace std;

FileHandler * loadFiles(string topo, string messages, string changes){
	FileHandler * fh = new FileHandler();
	fh->parse(topo,"TOPO_FILE");
	fh->parse(messages,"MESSAGES_FILE");
	fh->parse(changes,"CHANGES_FILE");
	return fh;
}

int main(int argc, char * argv []) {
	if(argc!=4){
		cout<<"Error, wrong number of arguments"<<endl;
		return 1;
	}
	string topo(argv[1]);
	string messages(argv[2]);
	string changes(argv[3]);
	FileHandler * fh = loadFiles(topo,messages,changes);
	std::cout << "test" << std::endl;
	free(fh);
	return 0;
}
