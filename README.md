# CommiFaceLib
Communication Interface library for different communication protols, like Serial, I2C, and CAN. Written in C++ w/CMake.

## Prerequisites

Install the library files

```bash
# Install LibSerial library
sudo apt install libserial-dev

# Give user access to serial ports
sudo usermod -a -G dialout $USER
```

## Usage

Put this folder within `/lib`

The folder directory should look something like this

```c++
|-- project/
|	|-- build // this will be created automatically by cmake
|	|-- include/
|	|-- src/
|	|	|-- main.cpp
|	|-- test/
|	|	|-- test.cpp
|   |-- lib/
|   |   |-- CommiFaceLib/ // this library
|	|-- CMakeLists.txt
```

```cmake
# CMakeLists.txt

# CMakeLists.txt

# Add this library as a subdirectory
add_subdirectory(lib/CommiFaceLib)

# Link the dependencies / libraries
target_link_libraries( ${MAIN_EXEC} 
  ${THIS_PACKAGE_INCLUDE_DEPENDS} 
  CommiFaceLib
)

# Optional, link this with the test exec
target_link_libraries(${TEST_EXEC} 
  GTest::gtest_main
  ${THIS_PACKAGE_INCLUDE_DEPENDS} 
  CommiFaceLib
)
```