cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(pico_sdk_import.cmake)

project(pico-i2clib C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()
add_library(pico-i2clib
    src/
)

target_include_directories(pico-i2clib PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        include
)

target_link_libraries(pico-i2clib
	pico_stdlib
    hardware_irq
    hardware_i2c
    hardware_gpio
)

