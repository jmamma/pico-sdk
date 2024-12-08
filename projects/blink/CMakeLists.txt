cmake_minimum_required(VERSION 3.13)
set(BUILD_CHIP "rp2040")
set(PICO_BOARD pico_w)
set(GCC_VERSION "4.0.1-8ec9d6f")
set(PICO_TOOLCHAIN_PATH "$ENV{HOME}/Library/Arduino15/packages/${BUILD_CHIP}/tools/pqt-gcc/${GCC_VERSION}/bin")
include(../../pico_sdk_init.cmake)
project(blink)
pico_sdk_init()
add_executable(blink src/blink.cpp)
target_link_libraries(blink pico_stdlib)
if (PICO_CYW43_SUPPORTED)
    target_link_libraries(blink pico_cyw43_arch_none)
endif()
pico_add_extra_outputs(blink)
file(GLOB PICOTOOL_PATH "$ENV{HOME}/Library/Arduino15/packages/${BUILD_CHIP}/tools/pqt-picotool/*/picotool")
add_custom_target(upload
    COMMAND ${CMAKE_COMMAND} -E echo "Uploading: ${PROJECT_NAME}.uf2"
    COMMAND ${PICOTOOL_PATH} load -f -x ${PROJECT_NAME}.uf2
    COMMAND ${PICOTOOL_PATH} reboot
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
