cmake_minimum_required(VERSION 3.13)

#includes
include(src/pico-sdk/pico_sdk_init.cmake)
include(src/freertos/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(Workshop)

#Pico SDK
pico_sdk_init()

#FreeRTOS
add_library(freertos STATIC
  ${FREERTOS_KERNEL_PATH}
  ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/port.c
)
target_include_directories(freertos PUBLIC
  .
  ${FREERTOS_KERNEL_PATH}/include
  ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/include
)
target_link_libraries(freertos
  FreeRTOS-Kernel
  FreeRTOS-Kernel-Heap3 #Use Version 3 heap management version
)


#Main:
add_executable(Workshop
        src/main.cpp
)
target_link_libraries(Workshop pico_stdlib hardware_gpio hardware_adc hardware_pwm hardware_spi hardware_i2c hardware_sync hardware_irq freertos)
pico_enable_stdio_usb(Workshop 1)
pico_enable_stdio_uart(Workshop 0)
pico_add_extra_outputs(Workshop)

#Example:
add_executable(Example
        src/example.cpp
)
target_link_libraries(Example pico_stdlib hardware_gpio hardware_adc hardware_pwm hardware_spi hardware_i2c hardware_sync hardware_irq freertos)
pico_enable_stdio_usb(Example 1)
pico_enable_stdio_uart(Example 0)
pico_add_extra_outputs(Example)

#Exercise:
add_executable(Exercise
        src/exercise.cpp
)
target_link_libraries(Exercise pico_stdlib hardware_gpio hardware_adc hardware_pwm hardware_spi hardware_i2c hardware_sync hardware_irq freertos)
pico_enable_stdio_usb(Exercise 1)
pico_enable_stdio_uart(Exercise 0)
pico_add_extra_outputs(Exercise)