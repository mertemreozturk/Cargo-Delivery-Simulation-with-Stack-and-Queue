#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Stack.h"
#include "Queue.h"

using namespace std;

class Package{
    public:
    string name;
};

class Truck : public Package{
    public:
    string power;
};

struct CityList{
    string cityName;
    Stack<Package> stack;
    Queue<Truck> queue;
    struct CityList* next;
};

CityList* head;

struct DoublyList{
    Package cargo;
    struct DoublyList* next;
    struct DoublyList* prev;
};
// function declaritions
vector<string> parserFile(string str,char c);
void addList(string name);
void addStackToList(string packageName,string cityName);
void addQueueToList(string truckName,string cityName,string power);
void findTruckName(DoublyList* d, string cityName);
void fromStackToDoubly(DoublyList* root, int size,string cityName,int flag);
void fromDoubly(DoublyList* root,string midwayName,int findIndex);
void deleteFromDoubly(DoublyList* root,vector<int> indices);
void allPacketsToTarget(DoublyList* root,string targetName);


int main(int argc, char* argv[]){
    head = nullptr;
    ifstream input;
    input.open(argv[1]); 
    
    vector<string> cityNames;
    for (string line; getline(input, line);){  // get data from file line by line
        cityNames.push_back(line);   // store city names
    }
    
    input.close();
    // reversing vector because new node always add to head in addList function, therefore output file display correct order of city
    for ( int index = cityNames.size() - 1; index >= 0; index--){
        addList(cityNames[index]);
    }
    
    input.open(argv[2]);

    for (string line; getline(input, line);){
        vector<string> package = parserFile(line,' ');
        addStackToList(package[0],package[1]);
    }
    
    input.close();

    input.open(argv[3]);

    for (string line; getline(input, line);){
        vector<string> truck = parserFile(line,' ');
        
        addQueueToList(truck[0],truck[1],truck[2]);
    }
    
    input.close();

    DoublyList* root = nullptr;  // define cargo truck

    input.open(argv[4]);

    for (string line; getline(input, line);){
        vector<string> mission = parserFile(line,'-');
        int forsStation = stoi(mission[3]);  // start station
        int formStation = stoi(mission[4]);    // midway station
        
        root = new DoublyList();
        findTruckName(root, mission[0]);
        root->next = nullptr, root->prev = nullptr;
        
        fromStackToDoubly(root, forsStation,mission[0],0); // flag 0 for starting station, store doubly list from starting station stack
        
        fromStackToDoubly(root, formStation,mission[1],1);  // flag 1 for midway station
        
        vector<string> indices = parserFile(mission[5],',');
        vector<int> indicesList;
        for ( int ind = 0; ind < indices.size(); ind++){  // type converting string to int
            indicesList.push_back(stoi(indices[ind]));
        }
        
        for ( int i = 0; i < indicesList.size(); i++){   // add to midway station according to indices
            fromDoubly(root,mission[1],indicesList[i]);
        }
        sort(indicesList.begin(), indicesList.end());  // sorting removing packages
        deleteFromDoubly(root->next,indicesList);  // delete package if they are added to midway station
        
        
        allPacketsToTarget(root,mission[2]);   // delete all packages and truck then store them to target station
    }
    
    input.close();

    ofstream fout;
    fout.open(argv[5]);
    
    while ( head != nullptr){    // print all station and its packages and trucks
    	CityList* temp = head;
        fout << temp->cityName << endl;
        fout << "Packages:" << endl;
        
        if ( temp->stack.size() != 0){
            temp->stack.print(fout);  // print content of package stack
        }    
        fout << "Trucks:" << endl;
        
        if( temp ->queue.size() != 0){
            temp->queue.print(fout);    // print content of truck queue
        }
        fout << "-------------" << endl;   
        head = head->next;  // next city
        delete temp;
    }
    fout.close();
    
    return 0;
}

vector<string> parserFile(string str,char c){
    vector<string> wordArr;
    string word = "";
    for (auto x : str) 
    {
        if (x == c){  // if find speacial char,add them to list 
            wordArr.push_back(word);
            word = "";    // then continue for new word
        }
        else {
            word = word + x;
        }
    }
    wordArr.push_back(word);   // all list for we want words
    return wordArr;
}

void addList(string name){  // this function always add element to head of linked list
    CityList* newNode = new CityList();    // new Station
    Stack<Package> s;    // This station might have packages
    Queue<Truck> q;      // This station might have trucks
    newNode->cityName = name;     
    newNode->stack = s;
    newNode->queue = q;
    if (head == nullptr){    // if linked list is empty
        head = newNode;    // head point new node
        newNode->next = nullptr;
    }else{
        newNode->next = head;    
        head = newNode;    // new node is head now.
    }
}

void addStackToList(string packageName,string cityName){
    CityList* iter = head;
    Package p;     // new package
    p.name = packageName;
    while( iter != nullptr){   // traverse linked list and find city
        if ( iter->cityName == cityName){
            iter->stack.push(p);  // add package to stack according to city name
            return;
        }
        iter = iter->next;  // if you didn't find continue with next node
    }
}

void addQueueToList(string truckName,string cityName,string power){
    CityList* iter = head;
    Truck t;    // new truck
    t.name = truckName;
    t.power = power;
    while ( iter != nullptr){      
        if ( iter->cityName == cityName){
            iter->queue.enqueue(t);    // add truck to queue according to city name
            return;
        }
        iter = iter->next;
    }
}

void findTruckName(DoublyList* d, string cityName){    // this function find to truck for carry packages from start station to target station
    CityList* iter = head;
    while ( iter != nullptr){
        if ( iter->cityName == cityName){    // start station and truck is here
            Truck t;   
            iter->queue.getFront(t);  // take them truck from garage
            d->cargo = t;        // add that to doubly linked lists root
            iter->queue.dequeue();      // exit that from garage 
            return;     // process is ok exit the function
        }
        iter = iter->next;
    }
}

void fromStackToDoubly(DoublyList* root, int size,string cityName,int flag){
    CityList* iter = head;    // head of city linked list
    DoublyList* doublyIter = root;       // head of doubly linked list
    if ( flag == 1){        // access last element in the doubly linkedlist for midway station 
        while ( doublyIter->next != nullptr){
            doublyIter = doublyIter->next;
        }
    }
    // if flag is zero this means that doubly linked list only contain root node(truck) and add directly for start station
    while ( iter != nullptr){
        if ( iter->cityName == cityName){
            for ( int i = 0; i < size; i++){      // iterate for how many packages came from station
                DoublyList* node = new DoublyList();
                Package p;  
                iter->stack.getTop(p);      // take them from station
                node->cargo = p;       // add them doubly linked list
                doublyIter->next = node;     // current pointer point new node
                node->prev = doublyIter;       // new node's prev pointer point current pointer
                node->next = nullptr;
                doublyIter = node;       // current pointer is new node
                iter->stack.pop();      // remove this package from stack
            }
            return;      // process is ok exit the function
        }
        iter = iter->next;
    } 
}
// this function adds packages to midway station's stack from doubly list according to given indices
void fromDoubly(DoublyList* root,string midwayName,int findIndex){  
    CityList* iter = head;  
    DoublyList* doublyIter = root->next;      // don't forget first node is a truck!!

    while ( iter != nullptr){
        if ( iter->cityName == midwayName){   // find name of midway station in citylist linked list
            int count = 0;
            while ( doublyIter != nullptr ){  
                if ( count == findIndex){     // find z indices 
                    Package p;
                    
                    p = doublyIter->cargo;
                    iter->stack.push(p);  // add midway station's stack from doubly list
                    return;
                }
                doublyIter = doublyIter->next;  // next package
                count++;  
            }
        }
        iter = iter->next;
    }
}
// this function delete packages from added to midway station's stack
void deleteFromDoubly(DoublyList* root,vector<int> indices){
    DoublyList* iter = root;
    int mark = 0;  // for indices
    int count = 0;   // for doubly linked list
    while ( iter != nullptr){
        if ( count == indices[mark]){ 
            DoublyList* temp;
            temp = iter;
            iter->prev->next = temp->next;    // prev node point deleting node's next node
            if ( temp->next != nullptr){
                temp->next->prev = iter->prev;     // next node point deleting node's prev node
            }
            iter = iter->next;
            temp->prev = nullptr;
            temp->next = nullptr;
            delete temp;  // remove packages from doubly list
            mark++; // next deleting packet indices
        }else{
            iter = iter->next;
        }
        count++;
    }
}
// this function add all packages to target station's stack from doubly linked list and then add truck to its garage
void allPacketsToTarget(DoublyList* root,string targetName){
    CityList* iter = head;
    DoublyList* doublyIter = root;
    while ( iter != nullptr){
        if ( iter->cityName == targetName){   // target city found!
            while( doublyIter->next != nullptr){  // traverse all packages in doubly list
                DoublyList* temp;
                temp = doublyIter->next;
                Package p;
                p = temp->cargo;
                iter->stack.push(p);  // add stack
                doublyIter->next = temp->next;
                if ( temp->next != nullptr){
                    temp->next->prev = doublyIter;
                }
                temp->next = nullptr;  // disconnect 
                temp->prev = nullptr;
                delete temp;   // remove from doubly list
            }
            Truck t;
            t.name = doublyIter->cargo.name;
            iter->queue.enqueue(t);  // enqueue truck for target station
            delete doublyIter;  // remove truck from doubly list
            root = nullptr;
            return;
        }
        iter = iter->next;
    }
}
