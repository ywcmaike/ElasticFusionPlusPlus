cmake_minimum_required(VERSION 2.6.0)
include_directories("/usr/local/cuda-8.0/include")
set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
project(ElasticFusion)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

if(WIN32)
  macro(CANONIFY_BOOL var)
    if(${var})
      set(${var} TRUE)
    else()
      set(${var} FALSE)
    endif()
  endmacro()
endif()

if(WIN32)
  find_package(JPEG REQUIRED)
endif()

find_package(LAPACK REQUIRED)
find_package(BLAS REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Pangolin 0.1 REQUIRED)
find_package(CUDA REQUIRED)
find_package(OpenNI2 REQUIRED)
find_package(efusion REQUIRED)
find_package(SuiteSparse REQUIRED)

#if(WIN32)
#  find_package(RealSense QUIET)
#  CANONIFY_BOOL(REALSENSE_FOUND)
#  message(STATUS "librealsense found: ${REALSENSE_FOUND}")
#  option(WITH_REALSENSE "Build with Intel RealSense support?" ${REALSENSE_FOUND})
#endif()
#
#if(WIN32)
#  find_package(RealSense2 QUIET)
#  CANONIFY_BOOL(REALSENSE2_FOUND)
#  message(STATUS "librealsense2 found: ${REALSENSE2_FOUND}")
#  option(WITH_REALSENSE2 "Build with Intel RealSense2 support?" ${REALSENSE2_FOUND})
#endif()

FIND_PACKAGE(OpenCV REQUIRED)


if(WIN32)
  include_directories(${JPEG_INCLUDE_DIR})
endif()

include_directories(${ZLIB_INCLUDE_DIR})
include_directories(${EIGEN_INCLUDE_DIRS})
include_directories(${Pangolin_INCLUDE_DIRS})
include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${EFUSION_INCLUDE_DIR})
include_directories(${OPENNI2_INCLUDE_DIR})

#if(WITH_REALSENSE)
#  include_directories(${REALSENSE_INCLUDE_DIR})
#  add_definitions(-DWITH_REALSENSE)
#  set(EXTRA_LIBS ${EXTRA_LIBS} ${REALSENSE_LIBRARY})
#endif()
#
#if(WITH_REALSENSE2)
#  include_directories(${REALSENSE2_INCLUDE_DIR})
#  add_definitions(-DWITH_REALSENSE2)
#  set(EXTRA_LIBS ${EXTRA_LIBS} ${REALSENSE2_LIBRARY})
#endif()

#find_package(RealSense2)
#set(REALSENSE2_INCLUDE_DIR /usr/local/include)
#include_directories(${REALSENSE2_INCLUDE_DIR})
#add_definitions(-DWITH_REALSENSE2)
#set(EXTRA_LIBS ${EXTRA_LIBS} ${REALSENSE2_LIBRARY})

#LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
#set(realsense_DIR "/home/yeweicai/work/librealsense/build" CACHE PATH "Path to librealsense2 config directory." )
#ADD_DEFINITIONS(${realsense_DEFINITIONS})
#INCLUDE_DIRECTORIES(${realsense_INCLUDE_DIRS})
#LINK_LIBRARIES(${realsense_LIBRARY_DIRS})
#
#FIND_PACKAGE(realsense REQUIRED)

find_package(realsense2 REQUIRED)
if(NOT realsense2_FOUND)
  message(FATAL_ERROR "\n\n Intel RealSense SDK 2.0 is missing, please install it from https://github.com/IntelRealSense/librealsense/releases\n\n")
else()
  include_directories(${realsense2_INCLUDE_DIR})
  ADD_DEFINITIONS(${realsense2_DEFINITIONS})
  set(EXTRA_LIBS ${EXTRA_LIBS} ${realsense2_LIBRARY})
endif()

file(GLOB srcs *.cpp)
file(GLOB tools_srcs Tools/*.cpp)

if(WIN32)
  file(GLOB hdrs *.h)
  file(GLOB tools_hdrs Tools/*.h)
endif()

if(WIN32)
  add_definitions(-Dlinux=0)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
  add_definitions(-DNOMINMAX)
  set(ADDITIONAL_CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(EXTRA_WINDOWS_LIBS ${EXTRA_WINDOWS_LIBS} ws2_32 ${JPEG_LIBRARY})
else()
  add_definitions(-Dlinux=1)
endif()

set(CMAKE_CXX_FLAGS ${ADDITIONAL_CMAKE_CXX_FLAGS} "-O3 -msse2 -msse3 -Wall -std=c++11")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

add_executable(ElasticFusion
               ${srcs}
               ${tools_srcs}
               ${hdrs}
               ${tools_hdrs}
)

target_link_libraries(ElasticFusion
					  ${EXTRA_WINDOWS_LIBS}
                      ${ZLIB_LIBRARY}
                      ${Pangolin_LIBRARIES}
                      ${CUDA_LIBRARIES}
                      ${EXTRA_LIBS}
                      ${EFUSION_LIBRARY}
                      ${OPENNI2_LIBRARY}
                      ${SUITESPARSE_LIBRARIES}
                      ${BLAS_LIBRARIES}
                      ${LAPACK_LIBRARIES}
        ${OpenCV_LIBS}
)


INSTALL(TARGETS ElasticFusion
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
