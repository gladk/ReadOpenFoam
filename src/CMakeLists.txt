#===========================================================
ADD_EXECUTABLE(readopenfoam main.cpp particle.cpp)
  
TARGET_LINK_LIBRARIES(readopenfoam ${Boost_LIBRARIES})
INSTALL(TARGETS readopenfoam DESTINATION ${CMAKE_INSTALL_BINDIR})
#===========================================================
IF(${VTK_MAJOR_VERSION} EQUAL 6)
  TARGET_LINK_LIBRARIES(readopenfoam ${VTK_LIBRARIES})
  MESSAGE(STATUS "VTK version 6 is found")
  ADD_DEFINITIONS("-DVTK6")
ELSE(${VTK_MAJOR_VERSION} EQUAL 6)
  TARGET_LINK_LIBRARIES(readopenfoam vtkHybrid)
ENDIF(${VTK_MAJOR_VERSION} EQUAL 6)
#===========================================================

