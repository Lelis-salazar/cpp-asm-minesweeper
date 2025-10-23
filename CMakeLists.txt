cmake_minimum_required(VERSION 3.16)
project(buscaminas CXX)
set(CMAKE_CXX_STANDARD 17)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

# Adding our source files (EXCLUDE the .asm file from the main executable)
set(PROJECT_SOURCES 
    sources/GameMechanics.cpp 
    sources/main.cpp 
    sources/matrixASM.cpp 
    sources/RankingHandler.cpp 
    sources/Drawings.cpp
)

set(PROJECT_HEADERS
    sources/matrixASM.hpp
    sources/RankingHandler.hpp
    sources/Drawings.hpp
)

set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/sources/")

# Declaring our executable WITHOUT the .asm file
add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMENT "Copying assets to build directory"
)

# Compile the ASM file manually with custom command
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/matrixASM.o
    COMMAND nasm -f elf64 ${CMAKE_CURRENT_SOURCE_DIR}/sources/matrixASM.asm -o ${CMAKE_CURRENT_BINARY_DIR}/matrixASM.o
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sources/matrixASM.asm
    COMMENT "Assembling matrixASM.asm with NASM"
    VERBATIM
)

# Add the object file to the executable
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/matrixASM.o)

# Flags for C++ only (these won't affect NASM now)
target_compile_options(${PROJECT_NAME} PRIVATE 
    $<$<CXX_COMPILER_ID:GNU>:-no-pie>
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# Incluir directorios
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${PROJECT_INCLUDE}
)

# Enlazar bibliotecas
target_link_libraries(${PROJECT_NAME} PRIVATE 
    raylib
)

# Dependencias del sistema
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Threads::Threads
    ${CMAKE_DL_LIBS}
)

# Dependencias específicas para Linux
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_library(M_LIB m)
    if(M_LIB)
        target_link_libraries(${PROJECT_NAME} PRIVATE m)
    endif()
    
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        X11
        ${X11_X11_LIB}
    )
endif()

# Setting ASSETS_PATH
target_compile_definitions(${PROJECT_NAME} PUBLIC 
    ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)