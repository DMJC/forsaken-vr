cmake_minimum_required(VERSION 3.16)

project(forsaken LANGUAGES C)

include(GNUInstallDirs)
include(CheckIPOSupported)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

option(OPENVR "Enable OpenVR runtime integration" OFF)
option(LUA_BOT "Enable Lua bot support" OFF)
option(RENDER_DISABLED "Disable rendering backend" OFF)
option(INPUT_DISABLED "Disable input backend" OFF)
option(DXMOUSE "Enable DirectInput mouse support" OFF)
option(PROFILE "Enable gprof instrumentation" ON)
set(GL_VERSION "1" CACHE STRING "OpenGL backend version macro value")
set_property(CACHE GL_VERSION PROPERTY STRINGS 1 2 3)

if(GL_VERSION VERSION_GREATER "3")
  message(FATAL_ERROR "GL_VERSION must be one of: 1, 2, 3")
endif()

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL REQUIRED IMPORTED_TARGET sdl2)
pkg_search_module(LUA REQUIRED IMPORTED_TARGET lua lua5.1)
pkg_search_module(LUA_SOCKET REQUIRED IMPORTED_TARGET lua-socket lua5.1-socket)
pkg_check_modules(ENET REQUIRED IMPORTED_TARGET libenet)
pkg_check_modules(PNG REQUIRED IMPORTED_TARGET libpng)
pkg_check_modules(ZLIB REQUIRED IMPORTED_TARGET zlib)
pkg_check_modules(OPENAL REQUIRED IMPORTED_TARGET openal)

if(OPENVR)
  pkg_check_modules(OPENVR_PKG REQUIRED IMPORTED_TARGET openvr)
endif()

file(GLOB PROJECTX_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_executable(projectx ${PROJECTX_SRC})

target_include_directories(projectx PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_options(projectx PRIVATE
  -Wall
  -Wextra
  -pipe
  -fcommon
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT CMAKE_BUILD_TYPE)
  target_compile_definitions(projectx PRIVATE
    DEBUG_ON
    DEBUG_COMP
    DEBUG_SPOTFX_SOUND
    DEBUG_VIEWPORT
  )
endif()

if(PROFILE)
  target_compile_options(projectx PRIVATE -pg)
  target_link_options(projectx PRIVATE -pg)
endif()

target_compile_definitions(projectx PRIVATE
  NET_ENET_2
  BSP
  LUA_USE_APICHECK
  TEXTURE_PNG
  SOUND_SUPPORT
  SOUND_OPENAL
  GL=${GL_VERSION}
)

if(LUA_BOT)
  target_compile_definitions(projectx PRIVATE LUA_BOT)
endif()

if(RENDER_DISABLED)
  target_compile_definitions(projectx PRIVATE RENDER_DISABLED)
endif()

if(INPUT_DISABLED)
  target_compile_definitions(projectx PRIVATE INPUT_DISABLED)
endif()

if(DXMOUSE)
  target_compile_definitions(projectx PRIVATE DXMOUSE)
  target_include_directories(projectx PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/dinput")
  target_link_directories(projectx PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/dinput")
  target_link_libraries(projectx PRIVATE dinput dxguid)
endif()

if(OPENVR)
  target_compile_definitions(projectx PRIVATE USE_OPENVR)
endif()

target_link_libraries(projectx PRIVATE
  PkgConfig::SDL
  PkgConfig::LUA
  PkgConfig::LUA_SOCKET
  PkgConfig::ENET
  PkgConfig::PNG
  PkgConfig::ZLIB
  PkgConfig::OPENAL
  GL
  GLU
  m
  dl
)

if(OPENVR)
  target_link_libraries(projectx PRIVATE PkgConfig::OPENVR_PKG)
endif()

install(TARGETS projectx RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
