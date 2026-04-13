cmake_minimum_required(VERSION 3.16)

project(task_serializer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable( task_serializer main.cpp
                                list_serializer.h
                                list_serializer.cpp )

include(GNUInstallDirs)
install(TARGETS task_serializer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/inlet.in DESTINATION ${CMAKE_CURRENT_BINARY_DIR})