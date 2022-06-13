cmake_minimum_required(VERSION 3.20)
project(groebner_basis)

set(Boost_INCLUDE_DIR C:/Users/user/Downloads/boost_1_79_0/boost_1_79_0)
set(Boost_LIBRARY_DIR C:/Users/user/Downloads/boost_1_79_0/boost_1_79_0)
set(CMAKE_CXX_STANDARD 17 REQUIRED)

include_directories(.)

find_package(Boost 1.79 COMPONENTS)
include_directories(${Boost_INCLUDE_DIR})

add_executable(groebner_basis
        main.cpp)

target_link_libraries(groebner_basis LINK_PUBLIC ${Boost_LIBRARIES})
