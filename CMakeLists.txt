cmake_minimum_required(VERSION 2.8.12.2)
#
ADD_DEFINITIONS(-DLINUX)
#
INCLUDE_DIRECTORIES(../libmisc-cpp)
INCLUDE_DIRECTORIES($ENV{LIB_CURL}/include)
INCLUDE_DIRECTORIES($ENV{LIB_ARCHIVE}/include)
#
ADD_COMPILE_OPTIONS(-std=c++17 -fPIC -Wall)
ADD_DEFINITIONS(-Wno-builtin-macro-redefined)
SET(CMAKE_CXX_FLAGS $ENV{CXX_FLAGS})
#
FILE(GLOB SRC0 "./*.cpp")
#
PROJECT(misc-ipfs-http-c)
#
ADD_LIBRARY(misc-ipfs-http-c STATIC ${SRC0})
INCLUDE(./__file__.cmake)
REDEFINE_FILE_MACRO(misc-ipfs-http-c)

