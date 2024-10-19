# =======================================
# Identity
# =======================================

cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(
    GeneticsGameServer
    VERSION 0.0.1
    DESCRIPTION "GeneticsGameServer..."
    LANGUAGES CXX
)

# =======================================
# Fetch Content
# =======================================
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# =======================================
# Subdirectories
# =======================================

add_subdirectory(src)
add_subdirectory(tests)

# =======================================
# Libs
# =======================================

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

# =======================================
# Links
# =======================================

add_executable(GeneticsGameServer src/main.cpp)

target_link_libraries(GeneticsGameServer PRIVATE spdlog)
target_link_libraries(GeneticsGameServer PRIVATE nlohmann_json::nlohmann_json)

