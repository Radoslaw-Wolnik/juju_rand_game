# File: simulation/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(DiceGameSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find MPI package
find_package(MPI REQUIRED)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cpp/include)

# Add library for optimized simulator
add_library(optimized_simulator
    src/optimized_simulator.cpp
)

# Add executable for distributed simulator
add_executable(distributed_simulator
    src/distributed_simulator.cpp
)
target_link_libraries(distributed_simulator PRIVATE optimized_simulator MPI::MPI_CXX)

# Add executable for comprehensive simulator
add_executable(comprehensive_simulator
    src/comprehensive_simulator.cpp
)
target_link_libraries(comprehensive_simulator PRIVATE optimized_simulator)

# Find and link nlohmann/json library
find_package(nlohmann_json REQUIRED)
target_link_libraries(comprehensive_simulator PRIVATE nlohmann_json::nlohmann_json)