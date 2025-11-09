cmake_minimum_required(VERSION 3.20)
project(SmartFarm LANGUAGES CXX)

# ---- General settings ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Dependencies ----
include(FetchContent)
find_package(Threads REQUIRED)

# Try to find ASIO (system package)
find_package(Asio QUIET)

if (NOT Asio_FOUND)
    message(STATUS "Asio not found — fetching standalone version via FetchContent...")
    FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-36-0
    )
    FetchContent_MakeAvailable(asio)
endif()

# Try to find spdlog
find_package(spdlog QUIET)

if (NOT spdlog_FOUND)
    message(STATUS "spdlog not found - fetching standalone version via FetchContent...")
    FetchContent_Declare(
            spdlog
            GIT_REPOSITORY https://github.com/gabime/spdlog.git
            GIT_TAG v1.16.0
    )
    FetchContent_MakeAvailable(spdlog)
endif()

# ---- Include subdirectories ----
add_subdirectory(src)
