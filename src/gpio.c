#include "../include/gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

/****************************************************************
 * find_gpio_number
 *
 * @brief Find the GPIO number corresponding to a given pin name.
 *
 * @param pin_name The name of the pin to search for.
 * @return The GPIO number if found, or -1 if the pin name is not found.
 ****************************************************************/
int find_gpio_number(const char *pin_name) {
    for (int i = 0; i < sizeof(gpio_pin_map) / sizeof(GPIOPinMap); i++) {
        if (strcmp(gpio_pin_map[i].pin_name, pin_name) == 0) {
            return gpio_pin_map[i].gpio_number;
        }
    }
    return -1;  // Return -1 if the pin is not found
}

/****************************************************************
 * led_set_value
 *
 * @brief Set the brightness value of the LED.
 *
 * @param gpio The GPIO number corresponding to the LED.
 * @param value The desired brightness level (LOW or HIGH).
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
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

/****************************************************************
 * gpio_set_dir
 *
 * @brief Set the direction of a GPIO pin (input or output).
 *
 * @param pin_name The name of the pin to configure.
 * @param out_flag The direction to set (OUTPUT_PIN or INPUT_PIN).
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
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

/****************************************************************
 * gpio_set_value
 *
 * @brief Set the value of a GPIO pin (HIGH or LOW).
 *
 * @param pin_name The name of the pin to set.
 * @param value The desired value to set (LOW or HIGH).
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
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

/****************************************************************
 * gpio_get_value
 *
 * @brief Read the value of a GPIO pin (HIGH or LOW).
 *
 * @param pin_name The name of the pin to read from.
 * @param value Pointer to an unsigned int where the read value will be stored.
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
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

/****************************************************************
 * gpio_set_edge
 *
 * @brief Set the edge detection for a GPIO pin.
 *
 * @param pin_name The name of the pin to configure.
 * @param edge The edge type to set (e.g., "rising", "falling", or "both").
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
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

/****************************************************************
 * gpio_fd_open
 *
 * @brief Open a file descriptor for a GPIO pin value.
 *
 * @param pin_name The name of the pin to open.
 * @return The file descriptor on success, or -1 on failure.
 ****************************************************************/
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

/****************************************************************
 * gpio_fd_close
 *
 * @brief Close the given file descriptor.
 *
 * @param fd The file descriptor to close.
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
int gpio_fd_close(int fd) {
    return close(fd);
}