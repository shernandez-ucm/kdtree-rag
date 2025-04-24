cmake_minimum_required(VERSION 3.10)
project(LlamaEmbeddingClient)

set(CMAKE_CXX_STANDARD 17)

set(EIGEN_ROOT_DIR "../eigen/")

find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(JSONCPP jsoncpp)

include_directories(${EIGEN_ROOT_DIR} ${CURL_INCLUDE_DIR} ${JSONCPP_INCLUDE_DIRS})

link_directories(${JSONCPP_LIBRARY_DIRS})

add_executable(llama_client src/llama_client.cpp src/main.cpp)

target_link_libraries(llama_client ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES})

add_executable(llama_wikihow src/wikihow.cpp)