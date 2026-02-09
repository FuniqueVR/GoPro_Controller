cmake_minimum_required(VERSION 3.14)
project(GoPro_Controller)
set(CMAKE_CXX_STANDARD 17)

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
        "${MDNS_DIR}/src/mdns.h"
        "${MDNS_DIR}/src/mdns.cpp"
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

    # SDL3
    find_package(SDL3)
    if (NOT SDL3_FOUND)
        message(STATUS "SDL3 not found on system. Getting SDL3 from GitHub...")
        include(FetchContent)
        FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG        release-3.2.0  # Use a stable release tag
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
    link_directories(/usr/lib/aarch64-linux-gnu)
    add_executable(server
        src/server/main.cpp
        src/server/GoProController.cpp
        src/server/UdpProxyServer.cpp
        ${MDNS_SOURCES}
    )
    target_link_libraries(server hv_static libcurl)
    # Ensure json include is available for server too if it uses nlohmann/json
    target_include_directories(server PRIVATE lib/json/include) 
endif()

# --- Master Executable ---
if(BUILD_MASTER)
    add_executable(master
        src/master/main.cpp
        src/master/GoProMaster.cpp
        src/master/IO.cpp
        lib/imgui/misc/cpp/imgui_stdlib.cpp
        ${IMGUI_SOURCES}
        ${MDNS_SOURCES}
    )

    if (WIN32)
        target_link_libraries(master PRIVATE
            OpenGL::GL
            SDL3::SDL3
            hv_static
            libcurl
            winpthread
        )
    else()
        target_link_libraries(master PRIVATE
            OpenGL::GL
            SDL3::SDL3
            hv_static
            libcurl
        )
    endif()

    if(WIN32)
        # This pulls in the missing __gthr symbols directly into your exe
        target_link_options(master PRIVATE "-static-libgcc" "-static-libstdc++" "-static")
        target_link_libraries(master PRIVATE ws2_32 wsock32)
    endif()
endif()