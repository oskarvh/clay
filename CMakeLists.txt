cmake_minimum_required(VERSION 3.27)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(CLAY_INCLUDE_ALL_EXAMPLES "Build all examples" OFF)
option(CLAY_INCLUDE_DEMOS "Build video demo and website" OFF)
option(CLAY_INCLUDE_CPP_EXAMPLE "Build C++ example" OFF)
option(CLAY_INCLUDE_RAYLIB_EXAMPLES "Build raylib examples" OFF)
option(CLAY_INCLUDE_SDL2_EXAMPLES "Build SDL 2 examples" OFF)
option(CLAY_INCLUDE_SDL3_EXAMPLES "Build SDL 3 examples" OFF)
option(CLAY_INCLUDE_FT81X_EXAMPLES "Build FT81X examples" ON)

if(NOT CLAY_INCLUDE_FT81X_EXAMPLES)
  # The PI Pico SDK cannot define project before the SDK is initialized 
  project(clay)
endif()
message(STATUS "CLAY_INCLUDE_DEMOS: ${CLAY_INCLUDE_DEMOS}")

if(APPLE)
  enable_language(OBJC)
endif()

if(CLAY_INCLUDE_ALL_EXAMPLES OR CLAY_INCLUDE_CPP_EXAMPLE)
  add_subdirectory("examples/cpp-project-example")
endif()
if(CLAY_INCLUDE_ALL_EXAMPLES OR CLAY_INCLUDE_DEMOS)
  if(NOT MSVC)
    add_subdirectory("examples/clay-official-website")
  endif()
  add_subdirectory("examples/introducing-clay-video-demo")
endif ()

if(CLAY_INCLUDE_ALL_EXAMPLES OR CLAY_INCLUDE_RAYLIB_EXAMPLES)
  add_subdirectory("examples/raylib-multi-context")
  add_subdirectory("examples/raylib-sidebar-scrolling-container")
endif ()
if(CLAY_INCLUDE_ALL_EXAMPLES OR CLAY_INCLUDE_SDL2_EXAMPLES)
  add_subdirectory("examples/SDL2-video-demo")
endif ()
if(NOT MSVC AND (CLAY_INCLUDE_ALL_EXAMPLES OR CLAY_INCLUDE_SDL3_EXAMPLES))
    add_subdirectory("examples/SDL3-simple-demo")
endif()
if(CLAY_INCLUDE_FT81X_EXAMPLES)
    # This is not included per default since it requires an ARM compiler
    add_subdirectory("examples/ft81x")
endif()

#  add_subdirectory("examples/cairo-pdf-rendering") Some issue with github actions populating cairo, disable for now
