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