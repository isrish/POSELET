project(poseletdetection)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-fpermissive -std=c++0x -Wno-literal-suffix -fPIC -msse4.2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -g")


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(SHARED_LIBS_DEFAULT ON)

find_package(Boost 1.55 COMPONENTS program_options system filesystem REQUIRED)
message(STATUS "Using Boost headers from ${Boost_INCLUDE_DIR}")
set(GIL_DIR ${Boost_INCLUDE_DIR}/boost/gil)
message(STATUS "Using Boost.GIL headers from ${Boost_INCLUDE_DIR}/boost/gil")
set(GIL_EXT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost/gil/extension)
message(STATUS "Using Boost.GIL extensions headers from ${GIL_EXT_DIR}")

set(GIL_EXT_NUMERIC_DIR ${GIL_EXT_DIR}/numeric)
file(GLOB GIL_HPP "${GIL_DIR}/*.hpp")
file(GLOB GIL_EXT_NUMERIC_HPP ${GIL_EXT_NUMERIC_DIR}/*.hpp)

source_group("Header Files\\gil" FILES ${GIL_HPP})
source_group("Header Files\\gil\\extension\\numeric" FILES ${GIL_EXT_NUMERIC_HPP})

find_package(JPEG REQUIRED)
find_package(OpenCV REQUIRED)


find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


file(GLOB util_hdrs ${CMAKE_CURRENT_SOURCE_DIR}/util/*.cpp)
set(util_hdrs ${util_hdrs})
add_library(util SHARED ${util_hdrs})

file(GLOB poselet_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(poselet ${poselet_srcs})
add_library(poselet SHARED ${poselet} ${thirdparty} ${util_hdrs})

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} poselet util ${GIL_ALL_HPP} ${GIL_EXT_NUMERIC_HPP} )
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${JPEG_LIBRARY} ${OpenCV_LIBS} -lmagic)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/util)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rapidxml-1.13)

