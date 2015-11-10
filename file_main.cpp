#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "file_core.h"
using namespace std;
 
 
int main(){
        string filename = "topology.txt";
        string filename1 = "changes.txt";
        string filename2 = "messages.txt";
        FileHandler * h = new FileHandler();
        h->parse(filename,"TOPO_FILE");
        h->parse(filename1,"CHANGES_FILE");
        h->parse(filename2,"MESSAGES_FILE");
        int len = h->getNumInit();
        int * row_one = h->getGraph(0);
        printf("Number of rows: %d\n",len);
        printf("First row: %d %d %d\n",row_one[0],row_one[1],row_one[2]);
        printf("Changes file\n");
        printf("Number of changes: %d\n",h->getNumChanges());
        int * change_one = h->getChange(0);
        printf("First change: %d %d %d\n",change_one[0],change_one[1],change_one[2]);
        printf("Messages\n");
        printf("Number of messages: %d\n",h->getNumMessages());
        printf("First message: %s\n",h->getMessage(0).c_str());
        free(h);
}