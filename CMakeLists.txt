cmake_minimum_required(VERSION 3.14)
project(GoPro_Controller)
set(CMAKE_CXX_STANDARD 17)

# Options
option(BUILD_SERVER "Build the server (RPi) executable" ON)
option(BUILD_MASTER "Build the master (PC) executable" ON)
# Dependencies

# libhv (Required for Server, used in Master too if needed, but primarily Server)
if(BUILD_SERVER OR BUILD_MASTER)
    include(FetchContent)
    FetchContent_Declare(
        libhv
        GIT_REPOSITORY https://github.com/ithewei/libhv.git
        GIT_TAG        master
    )
    FetchContent_MakeAvailable(libhv)
    include_directories(lib/mdns/include)
    set(MDNS_DIR "lib/mdns")
    file(GLOB MDNS_SOURCES
        "${MDNS_DIR}/src/logger.cpp"
        "${MDNS_DIR}/src/mdns.h"
        "${MDNS_DIR}/src/mdns.cpp"
        "${MDNS_DIR}/src/utils.cpp"
    )
endif()

if(BUILD_MASTER)
    # OpenGL
    find_package(OpenGL REQUIRED)

    # OpenCV
    find_package(OpenCV REQUIRED)
    if(OpenCV_FOUND)
        include_directories(${OpenCV_INCLUDE_DIRS})
    endif()

    # SDL3
    find_package(SDL3 REQUIRED)
    if (NOT SDL3_FOUND)
        message(STATUS "Getting SDL3 from Github")
        include(FetchContent)
        FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG        release-3.2.18
        )
        FetchContent_MakeAvailable(SDL3)
    endif()

    # ImGui
    set(IMGUI_DIR "lib/imgui")
    file(GLOB IMGUI_SOURCES
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_demo.cpp"
        "${IMGUI_DIR}/imgui_widgets.cpp"
        "${IMGUI_DIR}/imgui_tables.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
    )
    include_directories(${IMGUI_DIR})
    include_directories(${IMGUI_DIR}/backends)
    include_directories(lib/json/include)
endif()


# --- Server Executable ---
if(BUILD_SERVER)
    add_executable(server
        src/server/main.cpp
        src/server/GoProController.cpp
        src/server/UdpProxyServer.cpp
        ${MDNS_SOURCES}
    )
    target_link_libraries(server hv)
    # Ensure json include is available for server too if it uses nlohmann/json
    target_include_directories(server PRIVATE lib/json/include) 
endif()

# --- Master Executable ---
if(BUILD_MASTER)
    add_executable(master
        src/master/main.cpp
        src/master/GoProMaster.cpp
        src/master/IO.cpp
        ${IMGUI_SOURCES}
        ${MDNS_SOURCES}
    )

    target_link_libraries(master PRIVATE
        OpenGL::GL
        SDL3::SDL3
        ${OpenCV_LIBS}
        hv
    )
endif()