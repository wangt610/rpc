#include "mprpcapplication.hpp"
#include <unistd.h>
#include <string.h>
#include <sstream>

// 静态成员初始化
MprpcApplication* MprpcApplication::m_instance = nullptr;
std::mutex MprpcApplication::m_mutex;

// 1. 获取单例实例（线程安全的懒汉模式）
MprpcApplication& MprpcApplication::GetInstance() {
    if (m_instance == nullptr) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_instance == nullptr) {
            m_instance = new MprpcApplication();
        }
    }
    return *m_instance;
}

// 2. 框架初始化核心逻辑
void MprpcApplication::Init(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " -i <config_file>" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 解析命令行参数（-i 指定配置文件）
    int opt = 0;
    std::string config_file;
    while ((opt = getopt(argc, argv, "i:")) != -1) {
        switch (opt) {
            case 'i':
                config_file = optarg;
                break;
            case '?':
                std::cerr << "invalid option: " << static_cast<char>(optopt) << std::endl;
                exit(EXIT_FAILURE);
            case ':':
                std::cerr << "option " << static_cast<char>(optopt) << " requires an argument" << std::endl;
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }

    // 加载配置文件
    LoadConfigFile(config_file);

    // 初始化日志/网络等组件（可扩展）
    std::cout << "MprpcApplication init success!" << std::endl;
    std::cout << "rpcserver_ip: " << GetRpcServerIp("rpcserver_ip") << std::endl;
    std::cout << "rpcserver_port: " << GetRpcServerPort("rpcserver_port") << std::endl;
    std::cout << "zookeeper_ip: " << GetZkServerIp("zookeeper_ip" ) << std::endl;
    std::cout << "zookeeper_port: " << GetZkServerPort("zookeeper_port") << std::endl;
}

// 解析配置文件（格式：key=value，忽略注释行#）
void MprpcApplication::LoadConfigFile(const std::string& config_file) {
    FILE* fp = fopen(config_file.c_str(), "r");
    if (fp == nullptr) {
        std::cerr << "config file " << config_file << " not exist!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 逐行解析配置
    char buf[512] = {0};
    while (fgets(buf, sizeof(buf), fp) != nullptr) {
        // 去掉换行符/空格
        std::string line(buf);
        Trim(line);

        // 跳过空行/注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // 解析key=value
        size_t pos = line.find('=');
        if (pos == std::string::npos) {
            // 配置格式错误
            continue;
        }

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        Trim(key);
        Trim(value);
        m_config_map[key] = value;
    }

    fclose(fp);
}

// 辅助函数：去除字符串首尾空格（内部使用）
void Trim(std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        s = "";
        return;
    }
    size_t end = s.find_last_not_of(" \t\r\n");
    s = s.substr(start, end - start + 1);
}

// 3. 通用配置获取
std::string MprpcApplication::GetConfig(const std::string& key) {
    auto it = m_config_map.find(key);
    if (it == m_config_map.end()) {
        return "";
    }
    return it->second;
}

// 4. 便捷配置获取（封装类型转换）
std::string MprpcApplication::GetRpcServerIp(std::string ip) {
    return GetConfig(ip);
}

uint16_t MprpcApplication::GetRpcServerPort(std::string ip) {
    std::string port_str = GetConfig(ip);
    return std::stoi(port_str);
}

std::string MprpcApplication::GetZkServerIp(std::string ip) {
    return GetConfig(ip);
}

uint16_t MprpcApplication::GetZkServerPort(std::string ip) {
    std::string port_str = GetConfig(ip);
    return std::stoi(port_str);
}

// 5. 框架销毁（释放资源）
void MprpcApplication::Destroy() {
    // 释放日志资源、网络连接、zk客户端句柄等（可扩展）
    std::cout << "MprpcApplication destroy success!" << std::endl;
    if (m_instance != nullptr) {
        delete m_instance;
        m_instance = nullptr;
    }
}
