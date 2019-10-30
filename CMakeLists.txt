cmake_minimum_required(VERSION 2.8.0)
project( opticalflow-gen  CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread")

find_package( OpenCV 4 REQUIRED )
find_package( Eigen3 REQUIRED )
find_package( yaml-cpp REQUIRED )

set(ROOT ./)
include_directories(${ROOT}/src)
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${YAML_CPP_INCLUDE_DIR})
include_directories(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB opticalflow-gen_SRC
    "src/of/*.?pp"
    "src/gen/*.?pp"
)

add_executable( opticalflow-gen ${opticalflow-gen_SRC} )
target_link_libraries( opticalflow-gen yaml-cpp ${OpenCV_LIBS} ${YAML_CPP_LIBRARIES} stdc++fs)

file(GLOB opticalflow-track_SRC
    "src/of/*.?pp"
    "src/track/*.?pp"
)

add_executable( opticalflow-track ${opticalflow-track_SRC} )
target_link_libraries( opticalflow-track yaml-cpp ${OpenCV_LIBS} ${YAML_CPP_LIBRARIES} stdc++fs)



message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")

