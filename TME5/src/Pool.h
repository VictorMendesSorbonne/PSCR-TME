#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>

namespace pr {

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
public:
    Pool(int qsize) : queue(qsize){}

    void poolworker(){
        Job *j;
        while((j=queue.pop())){
            j->run();
            delete j;
        }
    }
        
    void start (int nbthread){
        threads.reserve(nbthread);
        for(int i = 0;i<nbthread;++i){
            threads.emplace_back(&Pool::poolworker,this);
        }
        std::cout<<"start"<<std::endl;
    }

    void submit(Job * job){
        
        queue.push(job);
    }

    void stop(){
        queue.setBlocking(false);
        
        for(auto & t : threads){
            t.join();
            
        }
        threads.clear();
    }


    ~Pool() {stop();}
};


class Barrier{

    int cpt;
    std::mutex m;
    int N;
    std::condition_variable cv;

    public : 
    Barrier(int N): N(N){
        cpt = 0;
    }

    void done(){
        std::unique_lock <std::mutex> l(m);
        cpt++;
        if(N>=cpt){
            cv.notify_one();
        }
        
    }

    void wait_for(){
        std::unique_lock<std::mutex> l(m);
        while(N<cpt){           //If a signal wake the thread and not the thread which notify him
            cv.wait(l);
        }
    }
};

}

