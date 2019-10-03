cmake_minimum_required(VERSION 3.13)

macro(use_c99)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
    endif ()
  else ()
    set (CMAKE_C_STANDARD 99)
  endif ()
endmacro(use_c99)

set(LIBS "${CMAKE_SOURCE_DIR}/libs/")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/libs/")
set(CMAKE_C99_STANDARD_COMPILE_OPTION "-std=c99")
set(CMAKE_C99_EXTENSION_COMPILE_OPTION "-std=gnu99")
use_c99()

project(dodo)
file(GLOB SOURCES "src/*.c")
add_subdirectory(bigchaindb/src)
find_library(LIBSEA NAMES libsea.a PATH "${CMAKE_SOURCE_DIR}/libs/")


add_compile_options(-DuECC_VLI_NATIVE_LITTLE_ENDIAN=0 )
add_compile_options(-std=c99)
add_compile_options( -O0)
add_compile_options( -g)

add_executable(${PROJECT_NAME}
        ${SOURCES}
    )


target_include_directories(${PROJECT_NAME} PUBLIC
inc
bigchaindb/libs/trezor-crypto/ed25519-donna
bigchaindb/inc
bigchaindb/libs/json-maker
bigchaindb/libs/base64
)
target_link_libraries(${PROJECT_NAME} PUBLIC
cbig
${LIBSEA}
)


