cmake_minimum_required(VERSION 2.8.11)
project(osgIntersectors)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(OpenSceneGraph REQUIRED COMPONENTS osgGA osgUtil osgViewer)

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

set(SOURCES
    main.cpp
    SVMData.h
    SVMData.cpp
    DraggableWire.h
    DraggableWire.cpp
    EventHandler.h
    EventHandler.cpp
    LineIntersector.h
    LineIntersector.cpp
    PointIntersector.h
    PointIntersector.cpp
    VirtualPlaneIntersector.h
    VirtualPlaneIntersector.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${OPENSCENEGRAPH_LIBRARIES}
    )
