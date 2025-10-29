#include "bbb_gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==== Pin map (fill with your full table) ====
typedef struct {
    const char *name;
    unsigned chip;
    unsigned offset;
} PinMap;

static const PinMap PINMAP[] = {
    {"P8_3",  0, 6}, {"P8_4",  0, 7}, {"P8_5",  0, 2}, {"P8_6",  0, 3},
    {"P8_7",  1, 2}, {"P8_8",  1, 3}, {"P8_9",  1, 5}, {"P8_10", 1, 4},
    {"P8_11", 0,13}, {"P8_12", 0,12}, {"P8_13", 3,23}, {"P8_14", 3,26},
    {"P8_15", 0,15}, {"P8_16", 0,14}, {"P8_17", 3,27}, {"P8_18", 1, 1},
    {"P8_19", 3,22}, {"P8_20", 0,31}, {"P8_21", 0,30}, {"P8_22", 0, 5},
    {"P8_23", 0, 4}, {"P8_24", 1, 1}, {"P8_25", 0, 0}, {"P8_26", 0,29},
    {"P8_35", 3, 8}, {"P9_11", 3,30}, {"P9_12", 0,28}, {"P9_13", 3,31},
    {"P9_15", 0,16}, {"P9_17", 3, 5}, {"P9_18", 3, 4}, {"P9_22", 3, 2},
    {"P9_23", 0,17}, {"P9_24", 3,15}, {"P9_25", 2,21}, {"P9_26", 3,14},
    {"P9_27", 2,19}, {"P9_29", 2,15}, {"P9_30", 2,16}, {"P9_31", 2,14},
    {"P9_41", 2,20},
};
static const size_t PINMAP_LEN = sizeof(PINMAP)/sizeof(PINMAP[0]);

bool bbb_lookup_pin(const char *name, unsigned *chip, unsigned *offset) {
    for (size_t i = 0; i < PINMAP_LEN; ++i) {
        if (strcmp(PINMAP[i].name, name) == 0) {
            *chip   = PINMAP[i].chip;
            *offset = PINMAP[i].offset;
            return true;
        }
    }
    return false;
}

BbbGpio* bbb_open(const char *name, bool output, int init_val, const char *consumer) {
    unsigned chip_idx, offset;
    if (!bbb_lookup_pin(name, &chip_idx, &offset)) {
        fprintf(stderr, "Unknown pin: %s\n", name);
        return NULL;
    }

    char path[64];
    snprintf(path, sizeof(path), "/dev/gpiochip%u", chip_idx);

    struct gpiod_chip *chip = gpiod_chip_open(path);
    if (!chip) {
        perror("gpiod_chip_open");
        return NULL;
    }

    struct gpiod_line *line = gpiod_chip_get_line(chip, offset);
    if (!line) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(chip);
        return NULL;
    }

    int ret;
    if (output) {
        ret = gpiod_line_request_output(line, consumer ? consumer : "bbb_gpio", init_val);
    } else {
        ret = gpiod_line_request_input(line, consumer ? consumer : "bbb_gpio");
    }

    if (ret < 0) {
        perror("gpiod_line_request");
        gpiod_chip_close(chip);
        return NULL;
    }

    BbbGpio *h = calloc(1, sizeof(BbbGpio));
    h->chip = chip;
    h->line = line;
    h->chip_index = chip_idx;
    h->offset = offset;
    return h;
}

int bbb_write(BbbGpio *h, int value) {
    return gpiod_line_set_value(h->line, value);
}

int bbb_read(BbbGpio *h) {
    return gpiod_line_get_value(h->line);
}

void bbb_close(BbbGpio *h) {
    if (!h) return;
    if (h->line) gpiod_line_release(h->line);
    if (h->chip) gpiod_chip_close(h->chip);
    free(h);
}
