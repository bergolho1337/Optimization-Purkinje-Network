cmake_minimum_required(VERSION 2.8)

PROJECT(PointToSphere)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(PointToSphere MACOSX_BUNDLE PointToSphere)

if(VTK_LIBRARIES)
  target_link_libraries(PointToSphere ${VTK_LIBRARIES})
else()
  target_link_libraries(PointToSphere vtkHybrid vtkWidgets)
endif()
