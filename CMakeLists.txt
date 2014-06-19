project(ReadOpenFoam C CXX)
cmake_minimum_required(VERSION 2.8)

INCLUDE(GNUInstallDirs)
#===========================================================
IF(NOT(CMAKE_CXX_FLAGS))
  #If flags are not set, add all usefull flags
  SET(CMAKE_BUILD_TYPE Release)
  SET(CMAKE_CXX_FLAGS  "-Wall -fPIC -g -O2 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -Werror=format-security")
ENDIF (NOT(CMAKE_CXX_FLAGS))

#C+11 support
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
#===========================================================

FIND_PACKAGE(Boost COMPONENTS program_options filesystem system regex iostreams REQUIRED)
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ADD_DEFINITIONS( "-DBOOST_LOG_DYN_LINK" )

FIND_PACKAGE(Eigen3 REQUIRED)
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})

FIND_PACKAGE(VTK REQUIRED)
INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})
LINK_DIRECTORIES(${VTK_LIBRARY_DIRS})

#===========================================================
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)
ADD_SUBDIRECTORY("${CMAKE_SOURCE_DIR}/src")
#===========================================================

