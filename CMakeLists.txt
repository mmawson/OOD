# CMAKE to generate executable ===================================================================================

cmake_minimum_required(VERSION 3.23)
project(OOD1)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(OOD1
        FileIO.cpp
        FileIO.hpp
        MapReduce_Phase3.cpp
        Functions.cpp
        Functions.h)

target_link_libraries(OOD1 ${CMAKE_DL_LIBS})

SET(CMAKE_CXX_FLAGS -pthread)

# CMAKE to generate reduce.so ===================================================================================
# Specify the minimum version.
cmake_minimum_required(VERSION 3.23)

# Specify the project info.
project(reduceNew VERSION 1.0.0 DESCRIPTION "Reduce Library")

# Declare the library target.
add_library(${PROJECT_NAME} MODULE
        Reduce.cpp
        Reduce.hpp
        )

# Configure the directories to search for header files.
target_include_directories(${PROJECT_NAME} PRIVATE OOD1)

# Set the version property.
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# Set the shared object version property to the project's major version.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

# Set the public header property to the one with the actual API.
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER OOD/Reduce.hpp)




#Cmake to generate map.so ==========================================================================

# Specify the minimum version.
cmake_minimum_required(VERSION 3.23)

# Specify the project info.
project(mapNew VERSION 1.0.0 DESCRIPTION "Map Library")

# Declare the library target.
add_library(${PROJECT_NAME} MODULE
        Map.cpp
        Map.hpp
        )

# Configure the directories to search for header files.
target_include_directories(${PROJECT_NAME} PRIVATE OOD1)

# Set the version property.
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# Set the shared object version property to the project's major version.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

# Set the public header property to the one with the actual API.
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER OOD/Map.hpp)
