cmake_minimum_required(VERSION 3.12)

# CHANGE ME
project( CommiFaceLib LANGUAGES C CXX )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Main Library ===
add_library(${PROJECT_NAME}
    # src/i2c/i2c.cpp
    # src/i2c/wire_device.cpp

    src/util/byte_converter.cpp

    # libi2c
    lib/libi2c/src/i2c.c
)

add_subdirectory(lib/plib)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:lib>

        # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/libi2c>
        # $<INSTALL_INTERFACE:lib/libi2c>
)

# This is for this project specifically
target_link_libraries(${PROJECT_NAME}
    plib
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Add the test executables only if this project is at the top level (aka not used with add_subdirectory)
if(PROJECT_IS_TOP_LEVEL)
    add_executable(Test test/test.cpp)
    target_link_libraries(Test PRIVATE 
        ${PROJECT_NAME}
    )
endif()