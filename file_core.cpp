#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#include "file_core.h"
/**
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
**/
 
FileHandler::FileHandler(){
        graph_init = NULL;
        numInit = 0;
        changes = NULL;
        numChanges = 0;
        numMessages = 0;
        sources = NULL;
        destinations = NULL;
        messages = NULL;
}
FileHandler::~FileHandler(){
        if(graph_init!=NULL){
                for(int i=0;i<numInit;i++){
                        free(graph_init[i]);
                }
                free(graph_init);
        }
        if(changes!=NULL){
                for(int i=0;i<numChanges;i++){
                        free(changes[i]);
                }
                free(changes);
        }
        if(sources!=NULL){
                free(sources);
        }
        if(destinations!=NULL){
                free(destinations);
        }
        if(messages!=NULL){
                free(messages);
        }
}
 
int getNumLines(string filename){
        ifstream f(filename.c_str());
        if(!f) return -1;
        string line;
        int i = 0;
        for(i=0;getline(f,line);i++);
        f.close();
        return i;
}
 
void FileHandler::parse(string filename, string filetype){
        if(filetype=="TOPO_FILE"){
                int numLines = getNumLines(filename);
                numInit = numLines;
                if(numLines<=0) return;
                graph_init = new int*[numLines];
                int i = 0;
                ifstream f(filename.c_str());
                if(!f) return;
                string line;
                while(getline(f,line)){
                        int t = 0;
                        graph_init[i] = new int[3];
                        stringstream stream(line);
                        int n = 0;
                        while(stream>>n){
                                graph_init[i][t] = n;
                                t++;
                        }
                        i++;
                }
        } else if(filetype=="CHANGES_FILE"){
                int numLines = getNumLines(filename);
                numChanges = numLines;
                if(numLines<=0) return;
                changes = new int*[numLines];
                int i = 0;
                ifstream f(filename.c_str());
                if(!f) return;
                string line;
                while(getline(f,line)){
                        int t = 0;
                        changes[i] = new int[3];
                        stringstream stream(line);
                        int n = 0;
                        while(stream>>n){
                                changes[i][t] = n;
                                t++;
                        }
                        i++;
                }
        } else if(filetype=="MESSAGES_FILE"){
                int numLines = getNumLines(filename);
                numMessages = numLines;
                if(numLines<=0) return;
                sources = new int[numLines];
                destinations = new int[numLines];
                messages = new string[numLines];
                int i = 0;
                ifstream f(filename.c_str());
                if(!f) return;
                string line;
                while(getline(f,line)){
                        int source;
                        int destination;
                        char message[100];
                        sscanf(line.c_str(),"%d %d %[^\n]",&source,&destination,message);
                        string new_message(message);
                        sources[i]=source;
                        destinations[i]=destination;
                        messages[i] = new_message;
                        i++;   
                }
        } else {
                return;
        }
}
 
int FileHandler::getNumInit(){
        return numInit;
}
 
int FileHandler::getNumChanges(){
        return numChanges;
}
int FileHandler::getNumMessages(){
        return numMessages;
}
int * FileHandler::getGraph(int i){
        if(graph_init!=NULL){
                if(i>=0 && i<numInit){
                        return graph_init[i];
                } else{
                        return NULL;
                }
        } else{
                return NULL;
        }
}
int * FileHandler::getChange(int i){
        if(changes!=NULL){
                if(i>=0 && i<numChanges){
                        return changes[i];
                } else {
                        return NULL;
                }
        } else {
                return NULL;
        }
}
int FileHandler::getSource(int i){
        if(sources!=NULL){
                if(i>=0&&i<numMessages){
                        return sources[i];
                } else {
                        return -1;
                }
        } else {
                return -1;
        }
}
int FileHandler::getDest(int i){
        if(destinations!=NULL){
                if(i>=0&&i<numMessages){
                        return destinations[i];
                } else {
                        return -1;
                }
        } else {
                return -1;
        }
}
string FileHandler::getMessage(int i){
        if(messages!=NULL){
                if(i>=0&&i<numMessages){
                        return messages[i];
                } else {
                        return "ERROR";
                }
        } else {
                return "ERROR";
        }
}