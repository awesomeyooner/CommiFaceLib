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

## Setup

The folder structure should resemble the following:

```c++
.
├── build
├── lib // Put here!
├── include
├── src
└── CMakeLists.txt
```

First, clone the repo into `/lib`

```bash
# Clone as standalone
$ git clone https://github.com/awesomeyooner/PlotLib.git && git submodule update --init --recursive

# Clone as submodule
$ git submodule add https://github.com/awesomeyooner/PlotLib.git && git submodule update --init --recursive
```

Also make sure that [plib](https://github.com/awesomeyooner/plib) is also put in `/lib`!

Then add these lines to your `CMakeLists.txt` file

```cmake
add_subdirectory(lib/CommiFaceLib)
target_link_libraries( ${MAIN_EXEC} CommiFaceLib )
```