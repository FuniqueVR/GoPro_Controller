cmake_minimum_required(VERSION 3.14)
project(GoPro_Controller LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)

# --- NUCLEAR STATIC OVERRIDES (MUST BE AT THE TOP) ---
# These must run before ANY include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Global Static" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "Global Static" FORCE)

# libhv specific overrides
set(HV_BUILD_STATIC ON CACHE BOOL "Build libhv static" FORCE)
set(HV_BUILD_SHARED OFF CACHE BOOL "NO Shared libhv" FORCE)
set(HV_WITH_MQTT OFF CACHE BOOL "Disable MQTT" FORCE) # Optional: Faster build

# Options
option(BUILD_SERVER "Build the server (RPi) executable" ON)
option(BUILD_MASTER "Build the master (PC) executable" ON)
# Dependencies

# libhv (Required for Server, used in Master too if needed, but primarily Server)
if(BUILD_SERVER OR BUILD_MASTER)
    # --- GLOBAL CURL CONFIGURATION (Apply to BOTH Server and Master) ---
    # Disable protocols we don't need to prevent errors like 'ldapsb_tls'
    set(CURL_DISABLE_LDAP ON CACHE INTERNAL "" FORCE)
    set(CURL_DISABLE_LDAPS ON CACHE INTERNAL "" FORCE)
    set(CURL_DISABLE_RTSP ON CACHE INTERNAL "" FORCE)
    set(CURL_DISABLE_DICT ON CACHE INTERNAL "" FORCE)
    set(CURL_DISABLE_TFTP ON CACHE INTERNAL "" FORCE)
    set(CURL_DISABLE_TELNET ON CACHE INTERNAL "" FORCE)
    set(CURL_DISABLE_POP3 ON CACHE INTERNAL "" FORCE)
    set(CURL_DISABLE_IMAP ON CACHE INTERNAL "" FORCE)
    set(CURL_DISABLE_SMB ON CACHE INTERNAL "" FORCE)
    set(CURL_DISABLE_SMTP ON CACHE INTERNAL "" FORCE)
    set(CURL_DISABLE_GOPHER ON CACHE INTERNAL "" FORCE)
    # Curl Configuration for Static Builds
    set(BUILD_CURL_EXE OFF CACHE INTERNAL "")
    set(BUILD_TESTING OFF CACHE INTERNAL "")
    set(CURL_USE_OPENSSL OFF CACHE INTERNAL "")

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
        "${MDNS_DIR}/src/mdns.cpp"
        "${MDNS_DIR}/src/mdns.h"
        "${MDNS_DIR}/src/utils.cpp"
    )
    if (BUILD_SERVER)
        # --- Curl Configuration for AArch64 ---
        set(SIZEOF_OFF_T 8 CACHE INTERNAL "")
        set(SIZEOF_CURL_OFF_T 8 CACHE INTERNAL "")
        set(SIZEOF_SIZE_T 8 CACHE INTERNAL "")
        set(SIZEOF_LONG 8 CACHE INTERNAL "")
        set(HAVE_RECV ON CACHE INTERNAL "")
        set(HAVE_SEND ON CACHE INTERNAL "")
        set(HAVE_BOOL_T ON CACHE INTERNAL "")

        # Disable features that usually break cross-compiles
        set(CURL_DISABLE_LDAP ON CACHE INTERNAL "")
        set(CURL_DISABLE_LDAPS ON CACHE INTERNAL "")
        set(BUILD_CURL_EXE OFF CACHE INTERNAL "")
        set(BUILD_TESTING OFF CACHE INTERNAL "")
        set(CURL_USE_OPENSSL OFF CACHE INTERNAL "")
        set(CURL_ENABLE_SSL OFF CACHE INTERNAL "")
    endif()
    FetchContent_Declare(
        curl
        URL https://github.com/curl/curl/releases/download/curl-8_5_0/curl-8.5.0.tar.gz
    )
    FetchContent_MakeAvailable(curl) 
endif()

if(BUILD_MASTER)
    # OpenGL
    find_package(OpenGL REQUIRED)

    if(WIN32)
        set(SDL_RENDER_D3D OFF CACHE BOOL "" FORCE)
        set(SDL_RENDER_D3D11 OFF CACHE BOOL "" FORCE)
        set(SDL_RENDER_D3D12 OFF CACHE BOOL "" FORCE)
        set(SDL_DIRECTX OFF CACHE BOOL "" FORCE)
        set(CMAKE_DISABLE_PRECOMPILE_HEADERS ON CACHE BOOL "" FORCE)
    endif()

    # SDL3
    add_subdirectory(lib/SDL3 EXCLUDE_FROM_ALL)

    if(WIN32)
        message(STATUS "Using curl install the window prebuild binary for import here")
    else()
        find_package(OpenCV REQUIRED COMPONENTS core imgcodecs highgui)
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
    file(GLOB_RECURSE SERVER_SOURCE CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/src/server/*.h"
        "${CMAKE_SOURCE_DIR}/src/server/*.hpp"
        "${CMAKE_SOURCE_DIR}/src/server/*.c"
        "${CMAKE_SOURCE_DIR}/src/server/*.cpp"
    )
    link_directories(/usr/lib/aarch64-linux-gnu)
    add_executable(server
        ${SERVER_SOURCE}
        ${MDNS_SOURCES}
    )
    if(WIN32)
        target_compile_options(server PRIVATE "/std:c++17")
        add_custom_command(TARGET server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_SOURCE_DIR}/Roboto-Medium.ttf"
                $<TARGET_FILE_DIR:server>
                COMMENT "Copying Font to output directory"
        )
        target_link_options(server PRIVATE "-static-libgcc" "-static-libstdc++" "-static")
    endif()
    target_link_libraries(server hv_static libcurl)
    # Ensure json include is available for server too if it uses nlohmann/json
    target_include_directories(server PRIVATE lib/json/include "${MDNS_DIR}/src" "${MDNS_DIR}/include") 
endif()

# --- Master Executable ---
if(BUILD_MASTER)
    file(GLOB_RECURSE MASTER_SOURCE CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/src/master/*.h"
        "${CMAKE_SOURCE_DIR}/src/master/*.hpp"
        "${CMAKE_SOURCE_DIR}/src/master/*.c"
        "${CMAKE_SOURCE_DIR}/src/master/*.cpp"
    )
    add_executable(master
        ${MASTER_SOURCE}
        lib/imgui/misc/cpp/imgui_stdlib.cpp
        ${IMGUI_SOURCES}
        ${MDNS_SOURCES}
    )

    if (WIN32)
        set(BUILD_LIST "core,imgproc,imgcodecs,videoio" CACHE STRING "" FORCE)
        # Disable examples, tests, apps
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(BUILD_PERF_TESTS OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_apps OFF CACHE BOOL "" FORCE)
        set(BUILD_DOCS OFF CACHE BOOL "" FORCE)
        
        # Disable language bindings
        set(BUILD_opencv_python2 OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_python3 OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_java OFF CACHE BOOL "" FORCE)
        
        # ENABLE GStreamer (the whole point!)
        set(WITH_GSTREAMER ON CACHE BOOL "" FORCE)
        
        # Disable heavy features
        set(WITH_CUDA OFF CACHE BOOL "" FORCE)
        set(WITH_OPENCL OFF CACHE BOOL "" FORCE)
        set(WITH_IPP OFF CACHE BOOL "" FORCE)
        set(WITH_TBB OFF CACHE BOOL "" FORCE)
        set(WITH_OPENGL OFF CACHE BOOL "" FORCE)
        
        # Disable GUI toolkits
        set(WITH_GTK OFF CACHE BOOL "" FORCE)
        set(WITH_QT OFF CACHE BOOL "" FORCE)
        set(WITH_WIN32UI OFF CACHE BOOL "" FORCE)
        
        # Disable extra image formats
        set(WITH_TIFF OFF CACHE BOOL "" FORCE)
        set(WITH_WEBP OFF CACHE BOOL "" FORCE)
        set(WITH_JASPER OFF CACHE BOOL "" FORCE)
        set(WITH_OPENEXR OFF CACHE BOOL "" FORCE)
        
        # Keep basic formats
        set(WITH_JPEG ON CACHE BOOL "" FORCE)
        set(WITH_PNG ON CACHE BOOL "" FORCE)
        set(BUILD_ZLIB ON CACHE BOOL "" FORCE)
        set(BUILD_PNG ON CACHE BOOL "" FORCE)
        set(BUILD_JPEG ON CACHE BOOL "" FORCE)
        
        # Disable extra modules
        set(BUILD_opencv_calib3d OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_dnn OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_features2d OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_flann OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_highgui OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_ml OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_photo OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_stitching OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_video OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_videostab OFF CACHE BOOL "" FORCE)
        set(BUILD_opencv_objdetect OFF CACHE BOOL "" FORCE)

        set(OPENCV_DIR "${CMAKE_SOURCE_DIR}/lib/opencv")
        target_compile_options(master PRIVATE "/std:c++17")

        target_link_directories(master PRIVATE
            "${CMAKE_BINARY_DIR}"
            "lib/opencv/include"
            "lib/opencv/modules/core/include"
            "lib/opencv/modules/imgproc/include"
            "lib/opencv/modules/imgcodecs/include"
            "lib/opencv/modules/videoio/include"
        )
        set_property(TARGET master PROPERTY C_STANDARD 17)
        set_property(TARGET master PROPERTY CXX_STANDARD 17)
        set_property(TARGET master PROPERTY CXX_STANDARD_REQUIRED ON)
        target_link_libraries(master PRIVATE
            opencv_core
            opencv_imgproc
            opencv_imgcodecs
            opencv_videoio
            OpenGL::GL
            SDL3::SDL3
            hv_static
            libcurl
            winpthread
        )

        target_include_directories(master PRIVATE
            lib/opencv/build/include
        )
        message(STATUS "Master will be built WITH OpenCV support")
        add_custom_command(TARGET master POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_SOURCE_DIR}/Roboto-Medium.ttf"
                $<TARGET_FILE_DIR:master>
                COMMENT "Copying Font to output directory"
        )
        target_link_options(master PRIVATE "-static-libgcc" "-static-libstdc++" "-static")
    else()
        target_compile_features(master PRIVATE cxx_std_17)
        target_link_libraries(master PRIVATE
            ${OpenCV_LIBS}
            OpenGL::GL
            SDL3::SDL3
            hv_static
            libcurl
        )
    endif()
endif()

# ============================================================
# Summary
# ============================================================
message(STATUS "")
message(STATUS "========================================")
message(STATUS "GoPro Controller - Configuration")
message(STATUS "========================================")
message(STATUS "Build Server:   ${BUILD_SERVER}")
message(STATUS "Build Master:   ${BUILD_MASTER}")
if(BUILD_MASTER)
    message(STATUS "OpenCV:         ${HAVE_OPENCV} (from source)")
endif()
message(STATUS "========================================")
message(STATUS "")
message(STATUS "NOTE: First build may take 15-30 minutes (OpenCV compilation)")
message(STATUS "Subsequent builds will be much faster (cached)")