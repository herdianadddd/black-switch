cmake_minimum_required(VERSION 3.10)
project(black-switch)

set(CMAKE_CXX_STANDARD 17)

add_executable(black-switch
    src/main_switch.cpp
)

target_link_libraries(black-switch
    nx
)

add_custom_command(TARGET black-switch POST_BUILD
    COMMAND elf2nro black-switch black-switch.nro
    VERBATIM
)
