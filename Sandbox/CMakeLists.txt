add_executable(Sandbox
    main.cpp
)

target_compile_features(Sandbox
    PRIVATE
        cxx_std_20
)

target_link_libraries(Sandbox
    PRIVATE
        Jeno
)

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:Sandbox>/Assets
)