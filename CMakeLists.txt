cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/../pico-examples/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/../pico-examples/pico_extras_import_optional.cmake)

project(blink-test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

pico_sdk_init()

include($ENV{PICO_SDK_PATH}/../pico-examples/example_auto_set_url.cmake)

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_executable(blink-test src/main.c)
target_link_libraries(blink-test pico_stdlib hardware_pwm)
pico_add_extra_outputs(blink-test)

# example_auto_set_url(blink-test)