cmake_minimum_required(VERSION 3.5)
project(color_curve)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if OpenMP is supported by the compiler
find_package(OpenMP)

# If OpenMP is found, enable it
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Find OpenCV package
find_package(OpenCV REQUIRED)
include_directories(
include
${OpenCV_INCLUDE_DIRS})

# Set source files
set(SOURCES
    src/main.cpp
    src/color_curve.cpp
)

# Set header files
set(HEADERS
    include/color_curve.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX ${OpenCV_LIBS} )
