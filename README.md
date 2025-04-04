<div align="center">
	<h1 align="center"><b>bb.black pin library</b></h1>
</div>

<p align="center">
    <img alt="Static Badge" src="https://img.shields.io/badge/Beaglebone-Black?style=for-the-badge&logo=Linux&logoColor=white&labelColor=brown&color=orange" alt="Beaglebone Black">
    <img src="https://img.shields.io/badge/Made%20with-C-3776AB?style=for-the-badge&logo=c&logoColor=white" alt="Made with C">
    <a href="https://github.com/CompEng0001/bb.black_pin_library/blob/main/LICENSE.md">
        <img alt="GitHub License" src="https://img.shields.io/github/license/CompEng0001/bb.black_pin_library?style=for-the-badge", alt="License MIT">
    </a>
  <a href="https://github.com/CompEng0001/bb.black_pin_library/stargazers">
        <img src="https://img.shields.io/github/stars/CompEng0001/bb.black_pin_library?style=for-the-badge" alt="GitHub Stars">
  </a>
</p> 




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

Just run the `install.sh` as it will ask for root:

```bash
$ debian@beaglebone:~/c_libraries/bb.black_pin_library$ bash install.sh
[1/3] Cleaning build files, if they exist...
rm -rf ./obj libioctrl.a libioctrl.so
[2/3] Uninstalling globally installed libioctrl libraries, root access requried...
sudo rm -f /usr/include/gpio.h /usr/include/pwm.h /usr/include/adc.h /usr/include/libioctrl.h
sudo rm -f /usr/lib/libioctrl.a /usr/lib/libioctrl.so ./obj
sudo ldconfig
[3/3] Compiling, and installing globally, root access required...
gcc -Wall -Werror -fPIC -I./include   -c src/gpio.c -o obj/gpio.o
gcc -Wall -Werror -fPIC -I./include   -c src/pwm.c -o obj/pwm.o
gcc -Wall -Werror -fPIC -I./include   -c src/adc.c -o obj/adc.o
ar rcs libioctrl.a  ./obj/gpio.o  ./obj/pwm.o  ./obj/adc.o
gcc -shared -o libioctrl.so  ./obj/gpio.o  ./obj/pwm.o  ./obj/adc.o
sudo cp include/gpio.h include/pwm.h include/adc.h include/libioctrl.h /usr/include/
sudo cp libioctrl.a libioctrl.so /usr/lib/
sudo ldconfig
```

### 2. Manually compile the Library manually

To build both the static and shared libraries:

```bash
make all
```

### 3. Install the Library

To install the library and header files to the system directories:

```bash
sudo make install
```

This installs:
- The header files (`gpio.h`, `pwm.h`, `adc.h`, `libioctrl.h`) into `/usr/include/`
- The static library (`libioctrl.a`) and shared library (`libioctrl.so`) into `/usr/lib/`

### 4. Clean Build Artifacts

To clean up the compiled object files and libraries:

```bash
make clean
```

---------

## Examples

Example programs can be found in the [examples_C/](./examples_C/) directory. Below are examples of the the code:

### 1. Example Program GPIO

An example progam, `blink.c`, demonstrates how to use the library:

```c
    #include <stdio.h>
    #include <unistd.h>
    #include "libioctrl.h"

    int main() {
        const char *pin_name = "P9_30";  // Use the pin name directly

        // Set the GPIO direction to output
        gpio_set_dir(pin_name, OUTPUT_PIN);

        while(1){
            // Set the GPIO pin value to HIGH (turn on the LED)
            gpio_set_value(pin_name, HIGH);
            printf("LED on %s is ON\n", pin_name);

            // Wait for a few seconds
            sleep(3);

            // Set the GPIO pin value to LOW (turn off the LED)
            gpio_set_value(pin_name, LOW);
            printf("LED on %s is OFF\n", pin_name);
            sleep(3);
        }
        return 0;
    }
```

To compile and run the `blink` example:

```bash
$ gcc -Wall -o blink blink.c -lioctrl -I/usr/include
$ ./blink
```

### Example 2. Program ADC

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

To compile and run the `adc_read` example:

```bash
$ gcc -Wall -o adc_read adc_read.c -lioctrl -I/usr/include
$ ./adc_read
```

### Example 3. Program ADC Buffer mode

An example program, `adc_continuous_read.c`, demonstrates how to use the library:

```c
#include "libioctrl.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <adcPin> <bufferLength>\n", argv[0]);
        return EXIT_FAILURE;
    }

    unsigned int adcPin = atoi(argv[1]);
    unsigned int bufferLength = atoi(argv[2]);
    unsigned int adcValue;
    float voltage;

    // Enable buffer and set buffer length
    if (enable_buffer() < 0) {
        fprintf(stderr, "Error enabling buffer.\n");
        return EXIT_FAILURE;
    }

    if (set_buffer_length(bufferLength) < 0) {
        fprintf(stderr, "Error setting buffer length.\n");
        disable_buffer();
        return EXIT_FAILURE;
    }

    // Enable ADC channel
    if (enable_channel(adcPin) < 0) {
        fprintf(stderr, "Error enabling ADC channel %u.\n", adcPin);
        disable_buffer();
        return EXIT_FAILURE;
    }

    printf("Starting continuous ADC read on pin %u...\n", adcPin);

    // Continuous read loop
    for (int i = 0; i < bufferLength; i++) {
        if (adc_get_buffer(adcPin, &adcValue) == 0) {
            adc_to_voltage(adcValue, &voltage);
            printf("ADC Value: %u, Voltage: %.3f V\n", adcValue, voltage);
        } else {
            fprintf(stderr, "Error reading buffer for ADC pin %u.\n", adcPin);
            break;
        }

        //usleep(500000); // Delay between reads (adjust as needed, e.g., 500 ms)
    }

    // Cleanup: disable buffer and channel
    disable_channel(adcPin);
    disable_buffer();

    printf("Continuous ADC read completed.\n");

    return EXIT_SUCCESS;
}
```

To compile and run the `adc_continuous_read` example:

```bash
$ gcc -Wall -o adc_continuous_read adc_continuous_read.c -lioctrl -I/usr/include
$ ./adc_continuous_read
```

## Example 4. Program PWM 

An example program, `pwm_test.c`, demonstrates how to use the library:

```c
#include "libioctrl.h"
#include <string.h>
#include <unistd.h>

int main() {
    PWM pwm;

    int period = 1000000; // Set period to 1 ms (1 kHz)
    int duty = 1000000; // Set duty cycle to 0.5 ms (50%)

    // Initialize the PWM structure for chip 0, channel 0
    pwm_init(&pwm, "P8_13");

    printf("Phy: %s\nChannel: %s\nChip: %s\nPeriod path: %s\nDuty Cycle path: %s\nEnable path: %s\nPin Mode Path: %s\nPin Mode Sate: %s\n",pwm.phy_pin,pwm.channel,pwm.chip,pwm.period_path,pwm.duty_cycle_path,pwm.enable_path,pwm.pin_mode_path, pwm.pin_mode_state);


    pwm_set_period(&pwm, period);
    pwm_set_duty_cycle(&pwm, duty);
    pwm_enable(&pwm);

    sleep(2);
    pwm_set_duty_cycle(&pwm, 500000);

    sleep(2);
    pwm_set_duty_cycle(&pwm, 200000);

    sleep(2);
    pwm_set_duty_cycle(&pwm, 100000);

    sleep(2);
    pwm_disable(&pwm);
    pwm_cleanup(&pwm);

    return 0;
}
```

To compile and run the `pwm_test` example:

```bash
$ gcc -Wall -o pwm_test pwm_test.c -lioctrl -I/usr/include
$ ./pwm_test
```

## Example 5. Program ADC 2 PWM

An example program, `adc2pwm.c`, demonstrates how to use the library:

```c
#include "libioctrl.h"
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

int main() {
    PWM pwm;
    unsigned int adc_reading = 0;
    unsigned int user_defined_period_ns = 100000; // User-defined period in nanoseconds (1 ms for 1 kHz)
    unsigned int duty_cycle = 0;

    // Initialize PWM on a specified pin, for example "P8_13"
    pwm_init(&pwm, "P8_13");

    // Set initial PWM period
    pwm_set_period(&pwm, user_defined_period_ns);

    // Enable PWM output
    pwm_enable(&pwm);

    printf("Phy: %s\nChannel: %s\nChip: %s\nPeriod path: %s\nDuty Cycle path: %s\nEnable path: %s\nPin Mode Path: %s\nPin Mode Sate: %s\n",pwm.phy_pin,pwm.channel,pwm.chip,pwm.period_path,pwm.duty_cycle_path,pwm.enable_path,pwm.pin_mode_path, pwm.pin_mode_state);

    // Loop to read ADC values and update PWM duty cycle
    for (int i = 0; i < 30; i++) {
        adc_get_value(0, &adc_reading); // Read ADC value from channel 0
        printf("ADC Reading: %u\n", adc_reading);

        // Map the ADC reading to the PWM duty cycle and update it
        duty_cycle = map(adc_reading, user_defined_period_ns);

        pwm_set_duty_cycle(&pwm, duty_cycle);

        sleep(1); // Update every 1 second
    }

    // Disable PWM and clean up
    pwm_disable(&pwm);
    pwm_cleanup(&pwm);

    return 0;
}
```

To compile and run the `adc2pwm.c` example:

```bash
$ gcc -Wall -o adc2pwm adc2pwm.c -lioctrl -I/usr/include
$ ./adc2pwm
```

---- 

## Uninstalling the Library

To uninstall the library and header files:

```bash
sudo make uninstall
```

This removes:
- Header files from `/usr/include/`
- Libraries from `/usr/lib/`

------

## License
This project is licensed under the MIT License.


-----

## Contributing
Contributions are welcome! Feel free to open issues or submit pull requests.
