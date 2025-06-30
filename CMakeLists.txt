cmake_minimum_required(VERSION 3.30)

project(tests)

include_directories("${CMAKE_SOURCE_DIR}/raylib55/src"
					"${CMAKE_SOURCE_DIR}/SDL3/include"
					"${CMAKE_SOURCE_DIR}/SDL3/include/SDL3"
                    "${CMAKE_SOURCE_DIR}/include")

# SDL3 Options
set(SDL_STATIC ON)
set(SDL_SHARED OFF)
set(SDL_EXAMPLES OFF)

# SDL3
add_subdirectory(SDL3)

# Raylib Options
set(BUILD_SHARED_LIBS OFF)
set(WITH_PIC ON)
set(PLATFORM SDL)

# Build Raylib
add_subdirectory(raylib55)

# Config options
option(BUILD_RAYGUI_EXAMPLES "Build the examples." OFF)

# Directory Variables
set(RAYGUI_ROOT ${CMAKE_SOURCE_DIR}/raygui)
set(RAYGUI_SRC ${RAYGUI_ROOT}/src)
set(RAYGUI_EXAMPLES ${RAYGUI_ROOT}/examples)

# Cmake Options
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# raygui
add_library(raygui INTERFACE)
file(GLOB sources ${RAYGUI_SRC}/*.h)
set(RAYGUI_HEADERS ${sources})
install(FILES
    ${RAYGUI_HEADERS} DESTINATION include
)
target_include_directories(raygui INTERFACE ${RAYGUI_SRC})

# Examples
if(${BUILD_RAYGUI_EXAMPLES})

    # Get the sources together
    set(example_dirs
        animation_curve
        controls_test_suite
        custom_file_dialog
        custom_sliders
        image_exporter
        image_importer_raw
        portable_window
        property_list
        scroll_panel
        style_selector
    )

    set(example_sources)
    set(example_resources)

    foreach(example_dir ${example_dirs})
        # Get the .c files
        file(GLOB sources ${RAYGUI_EXAMPLES}/${example_dir}/*.c)
        list(APPEND example_sources ${sources})

        # Any any resources
        file(GLOB resources ${RAYGUI_EXAMPLES}/${example_dir}/resources/*)
        list(APPEND example_resources ${resources})
    endforeach()

    # Do each example
    foreach(example_source ${example_sources})
        # Create the basename for the example
        get_filename_component(example_name ${example_source} NAME)
        string(REPLACE ".c" "${OUTPUT_EXT}" example_name ${example_name})

        # Setup the example
        add_executable(${example_name} ${example_source})

        # target_link_libraries(${example_name} PUBLIC raylib raygui)
        target_link_libraries(${example_name} ${LIBRAYLIB})

        string(REGEX MATCH ".*/.*/" resources_dir ${example_source})
        string(APPEND resources_dir "resources")
    endforeach()

    # Copy all of the resource files to the destination
    file(COPY ${example_resources} DESTINATION "resources/")
endif()

add_executable(ReverseGUI
			   res/icon_info.rc
               src/ReverseGUI.c)

target_link_libraries(ReverseGUI raylib raygui)