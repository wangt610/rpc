#ifndef FIEXDTHREADPOOL_HPP
#define FIEXDTHREADPOOL_HPP
#include"SyncQueue.hpp"
#include<functional>
#include<list>
#include<thread>
#include<vector>
#include<atomic>

class FixedThreadPool {
    // Class definition goes here
     public:
    using task=std::function<void()>;
    private:
    std::vector<std::thread> threadgroup;
    SyncQueue<task> taskqueue;
    std::atomic_bool isrunning;
    std::once_flag flag;
    void Start(int threadNum){
        isrunning=true;
        for(int i=0;i<threadNum;++i){
            threadgroup.emplace_back([this](){this->Run();});
        }
    }
    void Run(){
        while(isrunning){
            task t;
            taskqueue.Take(t);
            if(!isrunning) break;
            if(t){
                t();
            }
        }
    }
    void stop(){
        taskqueue.Stop();
        for(auto& th:threadgroup){
            if(th.joinable()){
                th.join();
            }
        }
        threadgroup.clear();
    }
    public:
    FixedThreadPool(int threadNum=4,int maxTaskCount=200):taskqueue(maxTaskCount),isrunning(false){
       Start(threadNum);
    }
    ~FixedThreadPool(){
        Stop();
    }
    void Stop(){
        std::call_once(flag,[this]{stop();});
    }
    void AddTask(const task& t){
            taskqueue.Put(t);
    }
    void AddTask(task&& t){
            taskqueue.Put(std::forward<task>(t));
    }
};
#endif