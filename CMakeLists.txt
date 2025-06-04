cmake_minimum_required(VERSION 3.14)
set(PICO_BOARD pico)

include(pico_sdk_import.cmake)
project(GPS_Client VERSION 1.0.0)

add_executable(${PROJECT_NAME}
    src/GPS_Client.cpp
    src/TinyGPSPlus.cpp
    src/Nokia5110.c
)

pico_sdk_init()

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_time
    hardware_uart
    hardware_gpio
    hardware_spi
)

target_include_directories(${PROJECT_NAME} PUBLIC
    inc
)
