#ifndef RPCPROVIDE_HPP
#define RPCPROVIDE_HPP
#include"google/protobuf/service.h"
#include<muduo/net/EventLoop.h>
#include<muduo/net/InetAddress.h>
#include<muduo/net/TcpConnection.h>
#include<string.h>
#include<functional>
#include<google/protobuf/descriptor.h>
#include<unordered_map>
class RpcProvider
{
     public:
    void NotifyService(::google::protobuf::Service* service);
    void Run();
    private:
    muduo::net::EventLoop loop;
    //处理上报的消息回调函数
    void onMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);
    //处理上报的连接函数
    void onConnection(const muduo::net::TcpConnectionPtr&);
    void SendRpcResponse(const muduo::net::TcpConnectionPtr&, google::protobuf::Message*);
    // service服务类型信息
    struct ServiceInfo
    {
        google::protobuf::Service *m_service; // 保存服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap; // 保存服务方法
    };
    // 存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;
};

#endif  // RPCPROVIDE_HPP