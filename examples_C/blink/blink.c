#include <stdio.h>
#include <unistd.h>
#include "libioctrl.h"

int main() {
    const char *pin_name = "P8_17";  // Use the pin name directly

    gpio_set_dir(pin_name, OUTPUT_PIN);
    while (1){
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

