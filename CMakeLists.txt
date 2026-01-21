cmake_minimum_required(VERSION 3.14)
project(GoPro_Controller)
set(CMAKE_CXX_STANDARD 14)

# Dependencies
include(FetchContent)

# libhv
FetchContent_Declare(
    libhv
    GIT_REPOSITORY https://github.com/ithewei/libhv.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(libhv)

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

# --- Server Executable ---
add_executable(server
    src/server/main.cpp
    src/server/GoProController.cpp
)
target_link_libraries(server hv)

# --- Master Executable ---
add_executable(master
    src/master/main.cpp
    ${IMGUI_SOURCES}
)

target_link_libraries(master PRIVATE
    OpenGL::GL
    SDL3::SDL3
    ${OpenCV_LIBS}
    hv
)