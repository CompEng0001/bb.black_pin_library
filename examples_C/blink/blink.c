#include <libioctrl.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {
    // Open P9_12 as output, initial value 0 (off)
    BbbGpio *led = bbb_open("P9_12", true, 0, "blink");
    if (!led) {
        fprintf(stderr, "Failed to open GPIO\n");
        return 1;
    }

    // Blink LED 5 times
    for (int i = 0; i < 5; i++) {
        bbb_write(led, 1);   // LED on
        usleep(500000);
        bbb_write(led, 0);   // LED off
        usleep(500000);
    }

    bbb_close(led);
    return 0;
}

