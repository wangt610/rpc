include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB log_list  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(log ${log_list})
target_link_libraries(log -lpthread)