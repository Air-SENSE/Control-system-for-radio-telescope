# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Đặt đường dẫn đến các component
list(APPEND EXTRA_COMPONENT_DIRS components/bts7960 components/mpu6050)

project(MPU6050feedback)

