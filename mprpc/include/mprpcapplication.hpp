#ifndef MPRPCAPPLICATION_HPP
#define MPRPCAPPLICATION_HPP
#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>

// RPC框架核心应用类（单例模式）
class MprpcApplication {
public:
    // 1. 单例获取：懒汉模式（线程安全）
    static MprpcApplication& GetInstance();

    // 2. 框架初始化：解析命令行参数 + 加载配置文件
    // 命令行参数格式：./rpcserver -i config.conf
    void Init(int argc, char** argv);

    // 3. 通用配置获取接口
    std::string GetConfig(const std::string& key);

    // 4. 便捷配置获取接口（封装GetConfig，避免重复解析字符串）
    std::string GetRpcServerIp(std::string ip);   // 获取RPC服务器IP
    uint16_t GetRpcServerPort(std::string ip);    // 获取RPC服务器端口
    std::string GetZkServerIp(std::string ip);    // 获取Zookeeper注册中心IP
    uint16_t GetZkServerPort(std::string ip);     // 获取Zookeeper注册中心端口

    // 5. 框架销毁：释放全局资源
    void Destroy();

    // 禁止拷贝和移动（单例模式必须）
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication& operator=(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
    MprpcApplication& operator=(MprpcApplication&&) = delete;

private:
    // 私有构造/析构：单例模式
    MprpcApplication() = default;
    ~MprpcApplication() = default;

    // 解析配置文件（内部辅助函数）
    void LoadConfigFile(const std::string& config_file);

    // 全局配置存储（key-value）
    std::unordered_map<std::string, std::string> m_config_map;

    // 单例锁（保证线程安全）
    static std::mutex m_mutex;
    static MprpcApplication* m_instance;
};

// class MprpcApplication{
//     public:
//     static void Init(int argc, char** argv);
//     static MprpcApplication& GetInstance();
//     //static MprpcConfig& GetConfig();
//     private:
//     MprpcApplication(){}
//     //拷贝构造
//     MprpcApplication(const MprpcApplication&)=delete;
//     //移动构造
//     MprpcApplication(MprpcApplication&&)=delete; 
// };
#endif