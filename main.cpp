#include<iostream>
#include"FiexdThreadPool.hpp"
#include"AsyncLogging.hpp"
wangt::AsyncLogging *asynclog = new wangt::AsyncLogging("/home/wangt/demo/rpc/log/wangt", 1024 * 10);
void asyncWriteFile(const string& info){
    asynclog->append(info);
}
void asyncFlushFile()
{
    asynclog->flush();
}
void fun(int i){
    std::cout<<"threadpool id :"<<i<<std::endl;
    LOG_INFO<<"main: "<<i;
}
int main(){
    wangt::Logger::setOutput(asyncWriteFile);
     asynclog->start(); 
    FixedThreadPool pool(4,200);
    for(int i=0;i<1000;i++){
        pool.AddTask(std::bind(fun,i));
    }
    while(1);
    return 0;
}