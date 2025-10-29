#ifndef BBB_GPIO_H
#define BBB_GPIO_H

#include <gpiod.h>
#include <stdbool.h>

typedef struct {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    unsigned chip_index;
    unsigned offset;
} BbbGpio;

/**
 * Look up chip+offset for a pin label like "P8_35".
 * Returns true on success.
 */
bool bbb_lookup_pin(const char *name, unsigned *chip, unsigned *offset);

/**
 * Open a GPIO line by BeagleBone header name (e.g., "P8_35").
 * Direction: output (with initial value) or input.
 * Returns NULL on failure.
 */
BbbGpio* bbb_open(const char *name, bool output, int init_val, const char *consumer);

/** Write a value (0 or 1) to a GPIO line. */
int bbb_write(BbbGpio *h, int value);

/** Read a value (0 or 1) from a GPIO line. */
int bbb_read(BbbGpio *h);

/** Close and free all resources. */
void bbb_close(BbbGpio *h);

#endif
