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

set(SHADER_DIR "${CMAKE_SOURCE_DIR}/Assets/Shaders")

add_custom_command(
    OUTPUT "${SHADER_DIR}/Basic2DVS.cso"
    COMMAND fxc /T vs_5_0 /E main
        /Fo "${SHADER_DIR}/Basic2DVS.cso"
        "${SHADER_DIR}/Basic2DVS.hlsl"
    DEPENDS "${SHADER_DIR}/Basic2DVS.hlsl"
    COMMENT "Compiling Basic2DVS.hlsl"
)

add_custom_command(
    OUTPUT "${SHADER_DIR}/Basic2DPS.cso"
    COMMAND fxc /T ps_5_0 /E main
        /Fo "${SHADER_DIR}/Basic2DPS.cso"
        "${SHADER_DIR}/Basic2DPS.hlsl"
    DEPENDS "${SHADER_DIR}/Basic2DPS.hlsl"
    COMMENT "Compiling Basic2DPS.hlsl"
)

add_custom_target(Shaders ALL
    DEPENDS
        "${SHADER_DIR}/Basic2DVS.cso"
        "${SHADER_DIR}/Basic2DPS.cso"
)

add_dependencies(Sandbox Shaders)