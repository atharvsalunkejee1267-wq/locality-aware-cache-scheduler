#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <map>
#include <algorithm>
#include <sstream>
#include <fstream>

using namespace std;

class task {
    public:
  string taskname;
    int time;
    vector<string> memory;
    int excy;
    task() {
        taskname=" ";
        time=0;
        excy=0;
    }
    task(string name,int t,vector<string> mem , int extra) {
        taskname=name;
        time=t;
        memory=mem;
        excy=extra;
    }
};

bool timecompareforsjr(const task& a,const task& b){
    if (a.time < b.time){
        return true;
    }
    else {
        return false;
    } 
}

deque<task> multilevelqueue(vector<task>t){
    map<string,vector<task>>m;
    map<string,bool>visited;
    deque<task>Queue;
for(auto i : t){
    for(auto j : i.memory){
        if(m.find(j)!=m.end()){
           m[j].push_back(i);
           break;
        }
        else{
            m[j]=vector<task>{i};
        }
    }
}

for(auto i : m){
 vector<task> v = i.second ;
 sort(v.begin(),v.end(),timecompareforsjr); 
 for (auto j : v) {
            if (!visited[j.taskname]){
                  visited[j.taskname]=true;
                   Queue.push_back(j);
            }
        }
}
return Queue;
}

void currentqueue(deque<task>& Queue) {
    cout<<"Current Queue:[";
    for (auto i : Queue){
        cout<<i.taskname<<" ";
    }
    cout<<"]"<< endl;
}

void nextcycledequeu( deque<task> & Queue){
    Queue.pop_front();
}

class node {
    public:
    string mem;
    node* next;
    node* prev;
    node(string d){
        mem=d;
        next=NULL;
        prev=NULL;
    }
};

class doublelinkedlist{
public:
    node* head;
    node* tail;
    node* evicted;
    int currentsize;
    doublelinkedlist() {
        head=new node("dummyh");
        tail=new node("dummyt");
        head->next=tail;
        tail->prev=head;
        currentsize=0;
    }

   string insertatstart(string d,int capacity){
        node* newnode=new node(d);
        newnode->next=head->next;
        newnode->prev=head;
          head->next->prev=newnode;
        head->next=newnode;
        currentsize++;
        if (currentsize > capacity) {
            node* oldest=tail->prev;
            string evicteddata=oldest->mem;
            oldest->prev->next=tail;
              tail->prev=oldest->prev;
            delete oldest;
            currentsize--;
            return evicteddata;
        }
        return ""; 
    }

    bool remove(string d){
        node* current=head->next;
        while (current!=tail) {
            if (current->mem==d) {
                current->prev->next=current->next;
                current->next->prev=current->prev;
                delete current;
                currentsize--;
                return true;
            }
            current=current->next;
        }
        return false;
    }
    void bringtofront(node* n) {
        if (n==head->next) {
                return;
        }
        n->prev->next=n->next;
        n->next->prev=n->prev;
        n->next=head->next;
        n->prev=head;
        head->next->prev=n;
        head->next=n;
    }
   void printmem(){
        node* current=head->next;
        cout<<"[";
        while (current != tail){
            cout<<current->mem<<" ";
            current=current->next;
        }
        cout<<"]";
    }
};

void insertionbetweenmem (string data,doublelinkedlist& L1,doublelinkedlist& L2,doublelinkedlist& L3){
    string removedFromL1=L1.insertatstart(data,32);
    if(!removedFromL1.empty()) {
        cout<<removedFromL1<<" out of L1-> Moving to L2"<<endl;
        string removedFromL2=L2.insertatstart(removedFromL1,128);
        if(!removedFromL2.empty()){
            cout<<removedFromL2<<" out of L2 -> Moving to L3"<<endl;
            L3.insertatstart(removedFromL2,512);
        }
    }
}

void processMemoryAccess(task t, int& totallatency,doublelinkedlist& L1,doublelinkedlist& L2,doublelinkedlist& L3,int& RAMHIT,vector<string> blocks){
    int latencyL1 = 4;
    int latencyL2 = 12;
    int latencyL3 = 40;
    int latencyRAM = 200;
    totallatency+=t.excy;
    cout<<endl;
cout<<"running "<<t.taskname<<" "<<"Burst Time:"<< t.time <<endl ;
for(int i=0; i<blocks.size();i++){
    cout<<"Memory Block "<<i+1<<":"<<blocks[i]<<endl;
}
   cout<<"Cache :";
cout<<"L1 Cache:";L1.printmem();cout<<endl;
    cout<<"L2 Cache:";L2.printmem();cout<<endl;
    cout<<"L3 Cache:";L3.printmem();cout<<endl;
    for(const string& req:t.memory){
        node* current=L1.head->next;
          bool foundinL1=false;
        while (current!=L1.tail){
            if (current->mem==req){
                foundinL1=true;
                break;
            }
            current=current->next;
        }
        if (foundinL1) {
            totallatency+=latencyL1;
            L1.bringtofront(current);
            cout<<"L1HIT"<<req<<endl;
            continue; 
        }

        current=L2.head->next;
        bool foundinL2=false;
        while(current!=L2.tail) {
            if(current->mem==req){
                foundinL2=true;
                break;
            }
                current=current->next;
        }
        if(foundinL2){
            totallatency+=latencyL1+latencyL2;
            cout<<"L1 MISS->L2 HIT"<<req<<endl;
            L2.remove(req);
            insertionbetweenmem(req,  L1,L2,L3);
            continue;
        }

        current=L3.head->next;
        bool foundinL3=false;
        while(current!=L3.tail){
            if(current->mem==req){
                foundinL3=true;
                 break;
            }
            current = current->next;
        }
        if(foundinL3){
            totallatency+=latencyL1+latencyL2+latencyL3;
            cout<<"L1MISS->L2 MISS-> L3 HIT"<<req<<endl;
            L3.remove(req);
            insertionbetweenmem(req,L1,L2,L3);
            continue;
        }

        totallatency+=latencyL1+latencyL2+latencyL3+latencyRAM;
        cout << "L1MISS->L2 MISS->L3 MISS->RAM "<<endl;
        insertionbetweenmem(req,L1,L2,L3);
        RAMHIT++;

        cout<<"L1 Cache: "; L1.printmem();cout<<endl;
    cout<<"L2 Cache: "; L2.printmem();cout<<endl;
    cout<<"L3 Cache: "; L3.printmem();cout<<endl;
}
    }

int main() {
    vector<task> givent;
    string sentence;
    int totaltask= 1;
    ifstream inpfile("tasks.txt");
    while (getline(inpfile, sentence)) {
        stringstream i(sentence);
        string z,name,memblock;
        int time;
        i>> z >> name >> z >> time >> z;
        vector<string> blocks;
        while(i>>memblock) {
            blocks.push_back(memblock);
        }
        int extracy= abs(time-(int)blocks.size());
        task a(name,time,blocks,extracy);
        givent.push_back(a);
    }
    deque<task> Queue=multilevelqueue(givent);
    currentqueue(Queue);
    doublelinkedlist L1,L2,L3;
    int totallatency=0;
    int ramhits=0;
    currentqueue(Queue);
    while (!Queue.empty()){
        task currentTask=Queue.front();
        Queue.pop_front();
        processMemoryAccess(currentTask,totallatency,L1,L2,L3,ramhits,currentTask.memory);
    }
    cout<<endl;
    cout<<"RESULT"<<endl;
    cout<<"CYCLES:"<<totallatency<<endl;
    cout<<"RAM Hits:"<<ramhits ;
    return 0;
}
