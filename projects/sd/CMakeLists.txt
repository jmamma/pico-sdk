# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)
set(BUILD_CHIP "rp2040")
set(PICO_BOARD pico_w)
set(GCC_VERSION "4.0.1-8ec9d6f")

include(../../pico_sdk_init.cmake)

# Include build functions from Pico SDK
# Set name of project (as PROJECT_NAME) and C/C++ standards
project(sd_fat_spi C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_DEFAULT_MUTEX_RECURSIVE 1)
set(PICO_USE_MALLOC_MUTEX 1)
# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME}
    src/sd.cpp
    src/hw_config.c
)

# Tell CMake where to find other source code
add_subdirectory(lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    FatFs_SPI
    )
# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

file(GLOB PICOTOOL_PATH "$ENV{HOME}/Library/Arduino15/packages/${BUILD_CHIP}/tools/pqt-picotool/*/picotool")
add_custom_target(upload
    COMMAND ${CMAKE_COMMAND} -E echo "Uploading: ${PROJECT_NAME}.uf2"
    COMMAND ${PICOTOOL_PATH} load -f -x ${PROJECT_NAME}.uf2
    COMMAND ${PICOTOOL_PATH} reboot
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

