#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;

template <typename T>
class ThreadSafeQueue{
    private:
        queue<T> q;
        mutex m;
        condition_variable cv;
        bool stopped = false;
    public:
        void push(const T& item){
            // grab lock
            // push item to queue
            // release lock
            // signal to matching thread
            {
                lock_guard<mutex> lock(m);
                q.push(item);
            }
            cv.notify_one();
        };

        void stop(){
            {
                lock_guard<mutex> lock(m);
                stopped = true;
            }
            cv.notify_all();
        };
        
        bool pop(T& item){
            // grab lock
            // sleep if item not there, be ready if there
            unique_lock<mutex> lock(m);
            while(q.empty() && !stopped){
                cv.wait(lock);
            }
            if(stopped && q.empty()){
                return false;
            }
            item = q.front();
            q.pop();
            return true;
        };
};