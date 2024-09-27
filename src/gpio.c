#include "../include/gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>


// Function to find GPIO number from pin name
int find_gpio_number(const char *pin_name) {
    for (int i = 0; i < sizeof(gpio_pin_map) / sizeof(GPIOPinMap); i++) {
        if (strcmp(gpio_pin_map[i].pin_name, pin_name) == 0) {
            return gpio_pin_map[i].gpio_number;
        }
    }
    return -1;  // Return -1 if the pin is not found
}

// GPIO functions using pin names directly

int led_set_value(unsigned int gpio, PIN_VALUE value) {

    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), LED_DIR "%d/brightness", gpio);

    int fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("led/set-value");
        return fd;
    }

    if (value == LOW) {
        write(fd, "0", 2);
    } else {
        write(fd, "1", 2);
    }

    close(fd);
    return 0;
}

int gpio_set_dir(const char *pin_name, PIN_DIRECTION out_flag) {
    unsigned int gpio = find_gpio_number(pin_name);
    if (gpio == -1) {
        fprintf(stderr, "Invalid pin name: %s\n", pin_name);
        return -1;
    }

    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);

    int fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/direction");
        return fd;
    }

    if (out_flag == OUTPUT_PIN) {
        write(fd, "out", 4);
    } else {
        write(fd, "in", 3);
    }

    close(fd);
    return 0;
}

int gpio_set_value(const char *pin_name, PIN_VALUE value) {
    unsigned int gpio = find_gpio_number(pin_name);
    if (gpio == -1) {
        fprintf(stderr, "Invalid pin name: %s\n", pin_name);
        return -1;
    }

    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    int fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/set-value");
        return fd;
    }

    if (value == LOW) {
        write(fd, "0", 2);
    } else {
        write(fd, "1", 2);
    }

    close(fd);
    return 0;
}

int gpio_get_value(const char *pin_name, unsigned int *value) {
    unsigned int gpio = find_gpio_number(pin_name);
    if (gpio == -1) {
        fprintf(stderr, "Invalid pin name: %s\n", pin_name);
        return -1;
    }

    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    int fd = open(buf, O_RDONLY);
    if (fd < 0) {
        perror("gpio/get-value");
        return fd;
    }

    char ch;
    read(fd, &ch, 1);

    *value = (ch != '0') ? 1 : 0;

    close(fd);
    return 0;
}

int gpio_set_edge(const char *pin_name, const char *edge) {
    unsigned int gpio = find_gpio_number(pin_name);
    if (gpio == -1) {
        fprintf(stderr, "Invalid pin name: %s\n", pin_name);
        return -1;
    }

    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

    int fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/set-edge");
        return fd;
    }

    write(fd, edge, strlen(edge) + 1);
    close(fd);
    return 0;
}

int gpio_fd_open(const char *pin_name) {
    unsigned int gpio = find_gpio_number(pin_name);
    if (gpio == -1) {
        fprintf(stderr, "Invalid pin name: %s\n", pin_name);
        return -1;
    }

    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    int fd = open(buf, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("gpio/fd_open");
    }
    return fd;
}

int gpio_fd_close(int fd) {
    return close(fd);
}
