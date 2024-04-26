cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
project(Analisi)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# look for GEANT4
find_package(ROOT 6.30 CONFIG REQUIRED)

# include necessary files and directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# compile every source and header file in project directory
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Create the main program using the library.
add_executable(analisi main.cpp ${sources} ${headers})
target_link_libraries(analisi PUBLIC ROOT::Core ROOT::Graf ROOT::Hist ROOT::Tree ROOT::Gpad ROOT::RIO)
