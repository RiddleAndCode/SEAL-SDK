
file(GLOB SOURCES "*.c")
add_library(microecc STATIC
    ${SOURCES}
)
 
target_include_directories(microecc PUBLIC
    ../include
)