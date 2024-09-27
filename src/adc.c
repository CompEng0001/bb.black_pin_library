#include "../include/adc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

/****************************************************************
 * adc_get_value
 *
 * @brief Read the raw ADC value from the specified ADC pin.
 *
 * @param adcPin The ADC pin number to read from.
 * @param value Pointer to an unsigned int where the read value will be stored.
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
int adc_get_value(unsigned int adcPin, unsigned int *value) {
    int fd;
    char buf[MAX_BUF];
    char retChars[ADC_BUF];

    snprintf(buf, sizeof(buf), SYSFS_ADC_DIR "/in_voltage%d_raw", adcPin);

    fd = open(buf, O_RDONLY);
    if (fd < 0) {
        perror("adc/get-value");
        return fd;
    }

    read(fd, retChars, sizeof(retChars));

    *value = strtol(retChars, NULL, 0);

    close(fd);
    return 0;
}

/****************************************************************
 * adc_fd_open
 *
 * @brief Open the file descriptor for the specified ADC pin.
 *
 * @param adcPin The ADC pin number to open.
 * @return The file descriptor on success, or a negative error code on failure.
 ****************************************************************/
int adc_fd_open(unsigned int adcPin) {
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_ADC_DIR "/in_voltage%d_raw", adcPin);

    fd = open(buf, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("adc/fd_open");
    }
    return fd;
}

/****************************************************************
 * adc_fd_close
 *
 * @brief Close the given file descriptor.
 *
 * @param fd The file descriptor to close.
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
int adc_fd_close(unsigned int fd) {
    return close(fd);
}

/****************************************************************
 * adc_to_voltage
 *
 * @brief Convert an ADC value to a voltage.
 *
 * @param adcValue The ADC value to convert.
 * @param voltage Pointer to a float where the calculated voltage will be stored.
 * @return 0 on success.
 ****************************************************************/
int adc_to_voltage(unsigned adcValue, float *voltage) {
    *voltage = (((float)adcValue * REFVOLTAGE) / (float)MAXADC);
    return 0;
}

/***************************************************************
 * enable_buffer
 *
 * @brief Enable continuous mode for the ADC.
 *
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
int enable_buffer(void) {
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_ADC_ENABLE_PATH);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("adc/buffer/enable");
        return fd;
    }

    write(fd, "1", 2);
    close(fd);

    return 0;
}

/***************************************************************
 * disable_buffer
 *
 * @brief Disable continuous mode for the ADC.
 *
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
int disable_buffer(void) {
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_ADC_ENABLE_PATH);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("adc/buffer/enable");
        return fd;
    }

    write(fd, "0", 2);
    close(fd);

    return 0;
}

/***************************************************************
 * set_buffer_length
 *
 * @brief Set the length of the ADC buffer.
 *
 * @param len The length to set for the buffer.
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
int set_buffer_length(unsigned int len) {
    int fd;
    char buf[MAX_BUF];
    char lenBuf[MAX_LEN_BUF];

    snprintf(buf, sizeof(buf), SYSFS_ADC_LENGTH_PATH);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("adc/buffer/length");
        return fd;
    }

    snprintf(lenBuf, sizeof(lenBuf), "%d", len);

    write(fd, lenBuf, sizeof(lenBuf));
    close(fd);

    return 0;
}

/***************************************************************
 * enable_channel
 *
 * @brief Enable a specific ADC channel for reading.
 *
 * @param adcPin The ADC pin number to enable.
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
int enable_channel(unsigned int adcPin) {
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_ADC_SCAN_DIR "/in_voltage%d_en", adcPin);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("adc/scan_element/");
        return fd;
    }

    write(fd, "1", 2);
    close(fd);

    return 0;
}

/***************************************************************
 * disable_channel
 *
 * @brief Disable a specific ADC channel.
 *
 * @param adcPin The ADC pin number to disable.
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
int disable_channel(unsigned int adcPin) {
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_ADC_SCAN_DIR "/in_voltage%d_en", adcPin);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("adc/scan_element/");
        return fd;
    }

    write(fd, "0", 2);
    close(fd);

    return 0;
}

/****************************************************************
 * adc_get_buffer
 *
 * @brief Read buffered ADC value from the specified ADC pin.
 *
 * @param adcPin The ADC pin number to read from.
 * @param value Pointer to an unsigned int where the read value will be stored.
 * @return 0 on success, or a negative error code on failure.
 ****************************************************************/
int adc_get_buffer(unsigned int adcPin, unsigned int *value) {
    int fd;
    char buf[MAX_BUF];
    char retChars[ADC_BUF];

    snprintf(buf, sizeof(buf), SYSFS_ADC_DIR "/in_voltage%d_raw", adcPin);

    fd = open(buf, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("adc/get-buffer");
        return fd;
    }

    ssize_t bytesRead = read(fd, retChars, sizeof(retChars));
    if (bytesRead < 0) {
        perror("adc/get-buffer/read");
        close(fd);
        return -1;
    }

    *value = strtol(retChars, NULL, 0);

    close(fd);
    return 0;
}