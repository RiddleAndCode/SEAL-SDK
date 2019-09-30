add_subdirectory(Unity)




macro(do_test test_name sources)
    add_executable(${test_name}
        ${sources}
        micro-ecc/src/uECC.c
    )

    target_include_directories(${test_name} PUBLIC
        ../inc
        micro-ecc/include
    )

    if(DYNAMIC_LINKING)
        target_link_libraries(${test_name}
            seadyn
            dl
            Unity
            gcov
        )
    else()
        target_link_libraries(${test_name}
            sea
            dl
            Unity
            gcov
        )
    endif()

    target_compile_features(${test_name} PRIVATE
        c_std_99
    )
    add_test(${test_name} ${test_name})
endmacro()

if(TARGET_GROUP STREQUAL community)
    get_filename_component(name community_tests.c NAME_WE)
    get_filename_component(file community_tests.c NAME)
    do_test(${name} ${file})

endif()








