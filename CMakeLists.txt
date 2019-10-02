cmake_minimum_required(VERSION 3.13)
set(LIBS "${CMAKE_SOURCE_DIR}/libs/")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/libs/")

project(dodo)
file(GLOB SOURCES "src/*.c")
add_subdirectory(bigchaindb/src)
find_library(LIBSEA NAMES libsea.a)
find_library(LIBCBIG NAMES libcbig.a)


add_compile_options(-DuECC_VLI_NATIVE_LITTLE_ENDIAN=0 -std=c99 -O0 -g)


add_executable(${PROJECT_NAME}
        ${SOURCES}
    )

target_include_directories(${PROJECT_NAME} PUBLIC
inc
)
target_link_libraries(${PROJECT_NAME} PUBLIC
${LIBCBIG}
${LIBSEA}
)


