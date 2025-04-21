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

include_directories(/usr/local/include/cryptopp)
link_directories(/usr/local/lib)


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
        local_data/model/RedisSettings.h
        local_data/dao/UserDao.cpp
        local_data/dao/UserDao.h
        local_data/model/User.cpp
        local_data/model/User.h
        local_data/repository/UserRepository.cpp
        local_data/repository/UserRepository.h
        local_data/model/Message.cpp
        local_data/model/Message.h
        local_data/dao/MessageDao.cpp
        local_data/dao/MessageDao.h
        local_data/repository/MessageRepository.cpp
        local_data/repository/MessageRepository.h
        WebSocketServer/WebSocketSession.cpp
        WebSocketServer/WebSocketSession.h
        WebSocketServer/SessionManager.cpp
        WebSocketServer/SessionManager.h
        WebSocketServer/websocket_handler.cpp
        WebSocketServer/websocket_handler.h
        local_data/model/request_target.cpp
        local_data/model/request_target.h
        AuthController/auth_handler.cpp
        AuthController/auth_handler.h
        AuthController/model/crypt_data.cpp
        AuthController/model/crypt_data.h
        Crypto/CryptoManager.cpp
        Crypto/CryptoManager.h
        AuthController/model/AuthRequest.cpp
        AuthController/model/AuthRequest.h
        local_data/model/UserAuth.hpp
)


target_link_libraries(ShentServer PRIVATE ${Boost_LIBRARIES})
target_link_libraries(ShentServer PRIVATE ${jwt-cpp_LIBRARIES})
target_link_libraries(ShentServer PRIVATE PostgreSQL::PostgreSQL)
target_link_libraries(ShentServer PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE cryptopp)

target_include_directories(ShentServer PRIVATE ${HIREDIS_HEADER})
target_link_libraries(ShentServer PRIVATE ${HIREDIS_LIB})

target_include_directories(ShentServer PRIVATE ${REDIS_PLUS_PLUS_HEADER})
target_link_libraries(ShentServer PRIVATE ${REDIS_PLUS_PLUS_LIB})
