cmake_minimum_required(VERSION 3.28)
project(ShentServer)

set(CMAKE_CXX_STANDARD 23)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS)

if (Boost_FOUND)
    message(STATUS "Boost found\n")
    include_directories(${Boost_INCLUDE_DIRS})

    add_executable(ShentServer main.cpp
            FrontController/server.cpp
            FrontController/server.h
            FrontController/session.cpp
            FrontController/session.h
            FrontController/router/base_handler.h
            UserController/user_handler.cpp
            UserController/user_handler.h
            FrontController/router/router.cpp
            FrontController/router/router.h
            local_data/config.cpp
            local_data/config.h)


    target_link_libraries(ShentServer ${Boost_LIBRARIES})
endif ()


