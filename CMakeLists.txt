cmake_minimum_required(VERSION 3.9)
project(cv2viewer)
set(CMAKE_CXX_STANDARD 14)

# Main
add_executable(cv2viewer
    src/main.cpp
    lib/imgui/imgui.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/backends/imgui_impl_sdl3.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)

find_package(OpenGL REQUIRED)

# include: OpenCV
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    target_include_directories(cv2viewer PUBLIC ${OpenCV_INCLUDE_DIRS})
endif()

# add SDL3 support
find_package(SDL3 REQUIRED)
if (NOT SDL3_FOUND)
    message(STATUS "Getting SDL3 from Github")
    include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.2.18)
else()
    message(STATUS "Using local SDL3")
endif()

# include: ImGui(symbolic linked)
include_directories(
    lib/imgui/
    lib/imgui/backends
)

# link
target_link_libraries(cv2viewer PRIVATE 
    OpenGL::GL
    SDL3::SDL3 
    ${OpenCV_LIBS})