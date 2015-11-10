#ifndef __FILE_CORE_INCLUDED__
#define __FILE_CORE_INCLUDED__
 
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
 
 
class FileHandler
{
public:
        FileHandler();
        ~FileHandler();
        void parse(string filename, string filetype);
        int getNumInit();
        int getNumChanges();
        int getNumMessages();
        int * getGraph(int i);
        int * getChange(int i);
        int getSource(int i);
        int getDest(int i);
        string getMessage(int i);
private:
        int ** graph_init;
        int numInit;
        int ** changes;
        int numChanges;
        int numMessages;
        int * sources;
        int * destinations;
        string * messages;
};
 
#endif