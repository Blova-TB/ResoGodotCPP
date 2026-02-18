add_executable(GodotPPServer
    src/main.cpp
        src/godotServer.cpp
        src/godotServer.h
        ../SharedDefinition.h
)

target_link_libraries(GodotPPServer PRIVATE
        snl
        EnTT::EnTT)