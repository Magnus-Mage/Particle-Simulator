cmake_minimum_required(VERSION 3.16)
project(ParticleSimulation VERSION 1.0.0 LANGUAGES CXX)


# Add local dependency paths
set(CMAKE_PREFIX_PATH 
    "${CMAKE_SOURCE_DIR}/deps/glfw"
    "${CMAKE_SOURCE_DIR}/deps/glew" 
    "${CMAKE_SOURCE_DIR}/deps/freeglut"
    ${CMAKE_PREFIX_PATH}
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

# Add compiler flags for optimization and warnings
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Find GLFW
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found via find_package, trying pkg-config")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
endif()

# Find GLEW
find_package(GLEW QUIET)
if(NOT GLEW_FOUND)
    message(STATUS "GLEW not found via find_package, trying pkg-config")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLEW REQUIRED glew)
endif()

# Create executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/ParticleSimulation.cpp
)

# Add header files to target (for IDEs)
target_sources(${PROJECT_NAME} PRIVATE
    src/ParticleSimulation.h
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
)

# Link GLFW
if(glfw3_FOUND)
    target_link_libraries(${PROJECT_NAME} glfw)
else()
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
    target_compile_options(${PROJECT_NAME} PRIVATE ${GLFW_CFLAGS_OTHER})
endif()

# Link GLEW
if(GLEW_FOUND)
    target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
else()
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
    target_compile_options(${PROJECT_NAME} PRIVATE ${GLEW_CFLAGS_OTHER})
endif()

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    if(MSVC)
        # Set subsystem to console for MSVC
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS "/SUBSYSTEM:CONSOLE"
        )
    endif()
elseif(APPLE)
    # macOS-specific settings
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    
    target_link_libraries(${PROJECT_NAME}
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
elseif(UNIX)
    # Linux-specific settings
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME}
        ${X11_LIBRARIES}
        pthread
        dl
    )
endif()

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
)

# Copy shaders or resources if needed (placeholder)
# file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

# Installation rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ flags: ${CMAKE_CXX_FLAGS}")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "C++ release flags: ${CMAKE_CXX_FLAGS_RELEASE}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "C++ debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
endif()
