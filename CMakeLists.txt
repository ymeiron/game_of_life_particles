cmake_minimum_required(VERSION 3.10)

project(game_of_life_particles)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(CGAL REQUIRED)
find_package(HDF5 COMPONENTS C HL REQUIRED)

include_directories(${CGAL_INCLUDE_DIRS})

add_executable(simulation simulation.cpp io.cpp neighbor_count.cpp)
target_compile_options(simulation PRIVATE -O3 -g -Wall -Wextra -Wunused)
target_link_libraries(simulation PRIVATE
    Boost::program_options
    HDF5::C
    HDF5::HL
)