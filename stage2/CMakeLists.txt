cmake_minimum_required(VERSION 3.5)

project(Test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 语法要求更严格（地去符合上面设定的版本）

set(HEADPATH ${CMAKE_CURRENT_SOURCE_DIR}) # 这里定义了之后子目录中的文件可以用的

find_package(Protobuf REQUIRED)  # 查找 protobuf 包

# 子库就是这样，先 add_subdirectory，然后下面再 target_link_libraries
add_subdirectory(proto2)
add_subdirectory(proto)


# 这个也是必须这样指定的。。。不然在编这个可执行文件的时候，每个文
# 件都是按照所在目录来的。。。所以如果是子目录里面的就寄掉了。。。
# 算是花了一晚上踩了个坑。。。不亏。。。
include_directories(${HEADPATH})
add_executable(test test.cpp)
target_link_libraries(test ${PROTOBUF_LIBRARIES} testlib)

