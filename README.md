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