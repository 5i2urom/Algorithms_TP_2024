cmake_minimum_required(VERSION 3.5)

project(Graphs_task1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Graphs_task1
    IGraph.h
    ListGraph.h
    MatrixGraph.h
    ListGraph.cpp
    MatrixGraph.cpp
    SetGraph.h
    SetGraph.cpp
    ArcGraph.h
    ArcGraph.cpp
    IGraph.cpp
    main.cpp)

include(GNUInstallDirs)
install(TARGETS Graphs_task1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
