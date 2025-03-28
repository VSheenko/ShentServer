cmake_minimum_required(VERSION 3.28)
project(ShentServer)

set(CMAKE_CXX_STANDARD 23)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS)

if (Boost_FOUND)
    message(STATUS "Boost found")
    include_directories(${Boost_INCLUDE_DIRS})

    add_executable(ShentServer main.cpp
            FrontController/FrontController.cpp
            FrontController/FrontController.h
            FrontController/server.cpp
            FrontController/server.h
            FrontController/session.cpp
            FrontController/session.h
            FrontController/BaseHandler.h)


    target_link_libraries(ShentServer ${Boost_LIBRARIES})
endif ()


