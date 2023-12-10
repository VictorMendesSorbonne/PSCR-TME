#include "Pool.h"

using namespace pr;
using namespace std;
Pool::Pool(int qsize) queue(qsize){}


void Pool::poolworker(){
    Job *j;
    while((j=queue.pop())){
        j->run;
        delete j;
    }
}
    
    
    
void Pool::start (int nbthread){
    threads.reserve(nbthread);
    for(int i = 0;i<nbthread;++i){
        threads.emplace_back(Pool::poolworker,this);
    }
}


void Pool::submit(Job * job){
    queue.push(job);
}


void Pool::stop(){
    queue.setisBlocking(false);
    for(auto & t : threads)
        t.join();
        
    threads.clear();
    
}


Pool::~Pool() {stop()}
