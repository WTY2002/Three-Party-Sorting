cmake_minimum_required(VERSION 3.28)
project(three_party_sorting CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置工程需要的头文件路径
set(INC_DIR ./include)
# 设置工程需要的库文件路径，这就是我存放dll文件的位置
set(LINK_DIR ./dll)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})

# 添加可执行文件
add_executable(three_party_sorting main.cpp
        include/Semi_honest_security.cpp
        include/Semi_honest_security.h
        include/malicious_security.cpp
        include/malicious_security.h
)

# 将libcrypto链接至工程
target_link_libraries(three_party_sorting libcrypto.dll)