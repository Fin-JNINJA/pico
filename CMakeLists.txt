cmake_minimum_required(VERSION 3.13)

include("$ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake")
project(pico C CXX ASM)

pico_sdk_init()

add_executable(pico morse_code.c buzzer.c)
target_link_libraries(pico pico_stdlib hardware_pwm hardware_adc)
