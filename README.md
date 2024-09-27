# bb.black pin library

## Overview
This is repo is designed for specifically for the BeagleBone Black.

The repo provides the `libioctrl` library which is designed to provide an interface for controlling GPIO, PWM, and ADC peripherals. It includes functions to read from and write to the GPIO pins, set PWM values, and read from ADC channels. The library can be linked both as a static and a shared library for flexibility in different project environments.

## Features
- **GPIO Control**: Functions for reading and writing to GPIO pins.
- **PWM Control**: Functions for setting PWM output on specified pins.
- **ADC Read**: Functions to retrieve ADC readings from specified channels.
- Support for both **static** and **shared** linking.

## Directory Structure

```
├── include             # Header files for the library
│   ├── adc.h
│   ├── gpio.h
│   ├── pwm.h
│   ├── libioctrl.h
├── src                 # Source files for the library
│   ├── adc.c
│   ├── gpio.c
│   ├── pwm.c
├── obj                 # Object files are generated here (created during compilation)
├── Makefile            # Makefile to build and install the library
└── README.md           # Project documentation
```

## Prerequisites
Make sure you have the following tools installed on your system:
- `gcc` 8.3.0
- `make` 4.2.1
- Root access for installation

## Building the Library

### 1. Compile the Library with `install.sh`

Just run the `install.sh` as root:

```bash
sudo install.sh
```

### 2. Compile the Library manually

To build both the static and shared libraries:

```bash
make all
```

### 2. Install the Library

To install the library and header files to the system directories:

```bash
sudo make install
```

This installs:
- The header files (`gpio.h`, `pwm.h`, `adc.h`, `libioctrl.h`) into `/usr/include/`
- The static library (`libioctrl.a`) and shared library (`libioctrl.so`) into `/usr/lib/`

### 3. Clean Build Artifacts

To clean up the compiled object files and libraries:

```bash
make clean
```

## Using the Library

### Static Library

To link your program with the static version of the library:

```bash
gcc -Wall -o your_program your_program.c -L/usr/lib -lioctrl -I/usr/include
```

### Shared Library

To link your program with the shared version of the library:

```bash
gcc -Wall -o your_program your_program.c -L/usr/lib -lioctrl -I/usr/include
```

Make sure the shared library can be found by the linker at runtime. If installed in `/usr/lib`, you can update the linker cache:

```bash
sudo ldconfig
```

### Example Program

An example program, `adc_read.c`, demonstrates how to use the library:

```c
#include "libioctrl.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    unsigned int i = 0;
    unsigned int adc_reading = 0;

    while (i < 30) {
        adc_get_value(0, &adc_reading);
        printf("ADC: %u\n", adc_reading);
        sleep(1);
        i++;
    }
    return 0;
}
```

### Compile the Example Program
To compile and run the `adc_read` example:

```bash
gcc -Wall -o adc_read adc_read.c -lioctrl -I/usr/include
./adc_read
```

## Uninstalling the Library

To uninstall the library and header files:

```bash
sudo make uninstall
```

This removes:
- Header files from `/usr/include/`
- Libraries from `/usr/lib/`

## License
This project is licensed under the MIT License.

## Contributing
Contributions are welcome! Feel free to open issues or submit pull requests.
