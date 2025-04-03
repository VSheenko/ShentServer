cmake_minimum_required(VERSION 3.28)
project(ShentServer)
include(FetchContent)


set(CMAKE_CXX_STANDARD 23)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS)
find_package(jwt-cpp CONFIG REQUIRED)
find_package(PostgreSQL REQUIRED)

find_path(HIREDIS_HEADER hiredis)
find_library(HIREDIS_LIB hiredis)
find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

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
        local_data/JsonConfigParser.cpp
        local_data/JsonConfigParser.h
        local_data/ShentDB.cpp
        local_data/ShentDB.h
        local_data/model/PqSettings.cpp
        local_data/model/PqSettings.h
        local_data/model/RedisSettings.h)


target_link_libraries(ShentServer PRIVATE ${Boost_LIBRARIES})
target_link_libraries(ShentServer PRIVATE ${jwt-cpp_LIBRARIES})
target_link_libraries(ShentServer PRIVATE PostgreSQL::PostgreSQL)
target_link_libraries(ShentServer PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(ShentServer PRIVATE ${HIREDIS_HEADER})
target_link_libraries(ShentServer PRIVATE ${HIREDIS_LIB})

target_include_directories(ShentServer PRIVATE ${REDIS_PLUS_PLUS_HEADER})
target_link_libraries(ShentServer PRIVATE ${REDIS_PLUS_PLUS_LIB})
