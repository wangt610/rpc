#ifndef SYNCQUEUE_HPP
#define SYNCQUEUE_HPP
#include<condition_variable>   
#include<mutex>
#include<queue>
#include<list>
#include<iostream>
using namespace std;
const int MaxTaskCount=200;
template<typename T>
class SyncQueue {
    // Class definition goes here
    private:
    std::list<T> queue;
    mutable std::mutex mtx;
    std::condition_variable notEmpty;
    std::condition_variable notFull;
    int maxTaskCount;
    bool stop;
    bool IsFull() const {
        bool full=queue.size() >= maxTaskCount;
        if(full){
            std::cout<<"SyncQueue is full!"<<std::endl;
            return true;
        }
        return false;
    }
    bool IsEmpty() const {
        bool empty=queue.empty();
        if(empty){
            std::cout<<"SyncQueue is empty!"<<std::endl;
            return true;
        }
        return false;
    }
    template<typename F>
    int Add(F&& task){
        std::unique_lock<std::mutex> lock(mtx);
        notFull.wait(lock,[this](){return !IsFull() || stop;});
        if(stop) return 2;
        queue.push_back(std::forward<F>(task));
        notEmpty.notify_one();
        return 0;
    }
    // Member variables and methods
    public:
    SyncQueue(int maxSize=MaxTaskCount):maxTaskCount(maxSize),stop(false){}
    ~SyncQueue(){
        Stop();
    }
    int Put(const T& task){
        return Add(task);
    }
    int Put(T&& task){
        return Add(std::forward<T>(task));
    }
    void Take(std::list<T>& tasks){
        std::unique_lock<std::mutex> lock(mtx);
        notEmpty.wait(lock,[this](){return !IsEmpty() || stop;});
        if(stop) return ;
        tasks=std::move(queue);
        queue.clear();
        notFull.notify_all();
    }
    void Take(T& task){
        std::unique_lock<std::mutex> lock(mtx);
        notEmpty.wait(lock,[this](){return !IsEmpty() || stop;});
        if(stop) return;
        task=std::move(queue.front());
        queue.pop_front();
        notFull.notify_one();
    }
    void Stop(){
        {
            std::lock_guard<std::mutex> lock(mtx);
            stop=true;
        }
        notEmpty.notify_all();
        notFull.notify_all();
}
    bool Empty() const{
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
    bool Full() const{
        std::lock_guard<std::mutex> lock(mtx);
        return queue.size() >= maxTaskCount;
    }
    int Size() const{
        std::lock_guard<std::mutex> lock(mtx);
        return queue.size();
    }
    int Count() const{
        std::lock_guard<std::mutex> lock(mtx);
        return queue.size();
    }
    // Public interface
};
#endif