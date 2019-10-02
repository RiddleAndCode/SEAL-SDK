cmake_minimum_required(VERSION 3.13)

SET(PROJECT dodo)

file(GLOB SOURCES "src/*.c")



add_executable(${PROJECT}
        ${SOURCES}
    )


link_directories(libs/)

add_compile_options(-DuECC_VLI_NATIVE_LITTLE_ENDIAN=0 -std=c99 -Wall -Wextra  -O0 -g --coverage -fsanitize=address -fno-omit-frame-pointer)

target_link_libraries(${PROJECT}
sea
)


add_subdirectory(bigchaindb/)

target_include_directories(${PROJECT} PUBLIC
inc
bigchaindb/libs/trezor-crypto/
bigchaindb/libs/json-maker
bigchaindb/libs/base64
bigchaindb/inc
)