cmake_minimum_required(VERSION 3.30)

project(tests)
set(PROJ ReverseGUI)

# Cmake Options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

set(WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
include(CMakeRC)

# Global Includes
include_directories(
${CMAKE_SOURCE_DIR}/src
${CMAKE_SOURCE_DIR}/vendor/SDL/include
${CMAKE_SOURCE_DIR}/vendor/raylib/src
${CMAKE_SOURCE_DIR}/vendor/imgui
${CMAKE_SOURCE_DIR}/vendor/imgui/backends
${CMAKE_SOURCE_DIR}/vendor/rlImGui
${CMAKE_SOURCE_DIR}/vendor/rlImGui/examples)

# SDL3 Options
set(SDL_STATIC OFF)
set(SDL_SHARED ON)
set(SDL_EXAMPLES OFF)

# Build SDL3
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/SDL  EXCLUDE_FROM_ALL)

# Raylib Options
set(BUILD_SHARED_LIBS ON)
set(BUILD_EXAMPLES OFF)
set(PLATFORM SDL)

# raylib workaround for shared libs
# prevents complaining about not exporting SDL3-shared
if(SDL_SHARED AND BUILD_SHARED_LIBS)
set(_saved_skip_install_rules "${CMAKE_SKIP_INSTALL_RULES}")
set(CMAKE_SKIP_INSTALL_RULES TRUE)

add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/raylib")

set(CMAKE_SKIP_INSTALL_RULES "${_saved_skip_install_rules}")
unset(_saved_skip_install_rules)
else()
# Build Raylib
add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/raylib")
endif()

# rlImGui Options
Set(RLIMGUI_EXTRAS OFF)

# Compile Executables
add_library(imgui STATIC
	${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp
	${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp
	${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
	${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
	${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp)
	
set(fonts 
	${CMAKE_SOURCE_DIR}/res/fonts/CousineNerdFontPropo-Regular.ttf
	${CMAKE_SOURCE_DIR}/res/fonts/CousineNerdFontPropo-Italic.ttf
	${CMAKE_SOURCE_DIR}/res/fonts/CousineNerdFontPropo-BoldItalic.ttf
	${CMAKE_SOURCE_DIR}/res/fonts/CousineNerdFontPropo-Bold.ttf
	${CMAKE_SOURCE_DIR}/res/fonts/CousineNerdFontMono-Regular.ttf
	${CMAKE_SOURCE_DIR}/res/fonts/CousineNerdFontMono-Italic.ttf
	${CMAKE_SOURCE_DIR}/res/fonts/CousineNerdFontMono-BoldItalic.ttf
	${CMAKE_SOURCE_DIR}/res/fonts/CousineNerdFontMono-Bold.ttf
	${CMAKE_SOURCE_DIR}/res/fonts/CousineNerdFont-Regular.ttf
	${CMAKE_SOURCE_DIR}/res/fonts/CousineNerdFont-Italic.ttf
	${CMAKE_SOURCE_DIR}/res/fonts/CousineNerdFont-BoldItalic.ttf
	${CMAKE_SOURCE_DIR}/res/fonts/CousineNerdFont-Bold.ttf)
cmrc_add_resource_library(rvg-fonts ALIAS rvg::fonts NAMESPACE rvg ${fonts})

add_library(rlimgui STATIC
${CMAKE_SOURCE_DIR}/vendor/rlImGui/rlImGui.cpp)

set(proj_sources
    src/main.cpp
    vendor/imgui/misc/cpp/imgui_stdlib.cpp
    res/icon_info.rc)

if(WIN32 AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_executable(${PROJ} WIN32
		${proj_sources})
else()
	add_executable(${PROJ}
		${proj_sources})
endif()
    
# Test Module (Scratch Pad)
add_executable(scratch WIN32
	src/scratch.cpp)

target_link_libraries(${PROJ} SDL3::SDL3 raylib imgui rlimgui rvg::fonts)
target_link_libraries(scratch SDL3::SDL3 raylib imgui rlimgui rvg::fonts)

# Use <project-root>/install unless the user set a different install prefix.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install"
        CACHE PATH "Install directory" FORCE)
endif()

# Windows: install/windows
# Linux and other platforms: install/bin
if(WIN32)
    set(APP_INSTALL_SUBDIR "windows")
else()
    set(APP_INSTALL_SUBDIR "bin")
endif()

# List every executable and shared-library target you want to install.
set(APP_TARGETS
	${PROJ}
)

install(
    TARGETS ${APP_TARGETS}
    RUNTIME_DEPENDENCY_SET app_runtime_deps
    RUNTIME DESTINATION "${APP_INSTALL_SUBDIR}"
    LIBRARY DESTINATION "${APP_INSTALL_SUBDIR}"
)

# Copy non-system runtime dependencies alongside the targets.
if(WIN32)
    install(RUNTIME_DEPENDENCY_SET app_runtime_deps
        PRE_EXCLUDE_REGEXES
            "^api-ms-.*"
            "^ext-ms-.*"
        POST_EXCLUDE_REGEXES
            ".*[Ww]indows[/\\\\].*"
        RUNTIME DESTINATION "${APP_INSTALL_SUBDIR}"
        LIBRARY DESTINATION "${APP_INSTALL_SUBDIR}"
    )
elseif(UNIX AND NOT APPLE)
    install(RUNTIME_DEPENDENCY_SET app_runtime_deps
        POST_EXCLUDE_REGEXES
            "^/lib/.*"
            "^/usr/lib/.*"
        RUNTIME DESTINATION "${APP_INSTALL_SUBDIR}"
        LIBRARY DESTINATION "${APP_INSTALL_SUBDIR}"
    )
endif()

# use the following lines to use cpack for distributables
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)
