cmake_minimum_required(VERSION 3.16)
include(FetchContent)
project(TradingRunner)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ASIO_HAS_STD_CHRONO")
set (CMAKE_CXX_STANDARD 17)

# === boost ===

set(OPENSSL_ROOT_DIR /usr/local/opt/openssl)

find_package(Boost REQUIRED COMPONENTS filesystem system thread regex chrono random)
find_package(OpenSSL REQUIRED)


# === websocket++ ===
FetchContent_Declare(websocketpp
        GIT_REPOSITORY https://github.com/zaphoyd/websocketpp.git
        GIT_TAG 0.8.2)
FetchContent_GetProperties(websocketpp)
if(NOT websocketpp_POPULATED)
    FetchContent_Populate(websocketpp)
    add_subdirectory(${websocketpp_SOURCE_DIR} ${websocketpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# add interface library with all websocketpp dependencies
add_library(Websockets INTERFACE)
target_include_directories(Websockets INTERFACE ${websocketpp_SOURCE_DIR})
target_link_libraries(Websockets INTERFACE Boost::system Boost::thread Boost::regex Boost::random)


add_executable(TradingRunner websocket_main.cpp Trader/Trader.hpp Orderbook/OrderBook.hpp Orderbook/Message.hpp)
target_link_libraries(TradingRunner Websockets  OpenSSL::SSL)


# == G test ==
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()


# Add test cpp file
add_executable(runUnitTests
        Trader/Trader_test.cpp
        Orderbook/Message_test.cpp
        Orderbook/OrderBook_test.cpp
        )

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest_main)
add_test(
        NAME runUnitTests
        COMMAND runUnitTests
)


# Add OrderBook executable
add_executable(OrderBook orderbook_main.cpp Orderbook/OrderBook.hpp Orderbook/Message.hpp)

add_executable(WebsocketTest filetest_websocket_main.cpp Trader/Trader.hpp Orderbook/OrderBook.hpp Orderbook/Message.hpp)
