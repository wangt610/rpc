file(GLOB callee_rpc ${CMAKE_CURRENT_SOURCE_DIR}/rpc/mprpcapplication.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/rpc/rpcheader.pb.cc
                     ${CMAKE_CURRENT_SOURCE_DIR}/rpc/rpcprovide.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/rpc/zookeeperutil.cpp)
file(GLOB caller_rpc ${CMAKE_CURRENT_SOURCE_DIR}/rpc/mprpcapplication.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/rpc/rpcheader.pb.cc
                     ${CMAKE_CURRENT_SOURCE_DIR}/rpc/mprpcchannel.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/rpc/zookeeperutil.cpp)
                     add_library(callee ${callee_rpc})
                     add_library(caller ${caller_rpc})
target_link_libraries(callee muduo_net muduo_base protobuf pthread zookeeper_mt)
target_link_libraries(caller muduo_net muduo_base protobuf pthread zookeeper_mt)