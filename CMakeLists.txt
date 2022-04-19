cmake_minimum_required(VERSION 3.21)
project(groebner_basis)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

find_package(Boost 1.40 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(groebner_basis
        main.cpp deglex.h polynomial.h)

target_link_libraries(groebner_basis LINK_PUBLIC ${Boost_LIBRARIES})

