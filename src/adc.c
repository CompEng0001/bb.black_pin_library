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
 ****************************************************************/
int adc_get_value(unsigned int adcPin, unsigned int *value)
{
  int fd;
  char buf[MAX_BUF];
  char retChars[ADC_BUF];

  snprintf(buf, sizeof(buf), SYSFS_ADC_DIR "/in_voltage%d_raw", adcPin);

  fd = open(buf, O_RDONLY);
  if (fd < 0) {
    perror("adc/get-value");
    return fd;
  }

  read(fd, retChars, 6);

  *value = strtol(retChars,NULL,0);

  close(fd);
  return 0;
}

/****************************************************************
 * adc_fd_open
 ****************************************************************/

int adc_fd_open(unsigned int adcPin)
{
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_ADC_DIR "/in_voltage%d_raw", adcPin);

  fd = open(buf, O_RDONLY | O_NONBLOCK );
  if (fd < 0) {
    perror("adc/fd_open");
  }
  return fd;
}

/****************************************************************
 * adc_fd_close
 ****************************************************************/

int adc_fd_close(unsigned int fd)
{
  return close(fd);
}

/****************************************************************
 * adc_to_voltage
 ****************************************************************/

int adc_to_voltage(unsigned adcValue, float *voltage){

  *voltage = ( ( (float)adcValue * REFVOLTAGE ) / (float)MAXADC );

  return 0;
}

/***************************************************************
 * enable_buffer
 ****************************************************************/

int enable_buffer(void){

  int fd;
  char buf[MAX_BUF];

  // to enable continous mode
  snprintf(buf, sizeof(buf),SYSFS_ADC_ENABLE_PATH); 

  fd = open(buf,O_WRONLY);
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
 ****************************************************************/

int disable_buffer(void){

  int fd;
  char buf[MAX_BUF];

  // to enable continous mode
  snprintf(buf, sizeof(buf),SYSFS_ADC_ENABLE_PATH); 

  fd = open(buf,O_WRONLY);
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
 ****************************************************************/

int set_buffer_length(unsigned int len){

  int fd;
  char buf[MAX_BUF];
  char lenBuf[MAX_LEN_BUF];

  snprintf(buf, sizeof(buf),SYSFS_ADC_LENGTH_PATH);

  fd = open(buf,O_WRONLY);
  if (fd < 0) {
    perror("adc/buffer/length");
    return fd; 
  }

  snprintf(lenBuf, sizeof(lenBuf),"%d",len);

  write(fd, lenBuf, sizeof(lenBuf));
  close(fd);

  return 0;

}

/***************************************************************
 * enable_channel
 ****************************************************************/

int enable_channel(unsigned int adcPin){

  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf),SYSFS_ADC_SCAN_DIR "/in_voltage%d_en", adcPin); 

  fd = open(buf,O_WRONLY);
  if (fd < 0) {
    perror("adc/scan_element/");
    return fd; 
  }

  // to enable continous mode
  write(fd, "1", 2);
  close(fd);

  return 0;

}

/***************************************************************
 * disable_channel
 ****************************************************************/

int disable_channel(unsigned int adcPin){

  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf),SYSFS_ADC_SCAN_DIR "/in_voltage%d_en", adcPin); 

  fd = open(buf,O_WRONLY);
  if (fd < 0) {
    perror("adc/scan_element/");
    return fd; 
  }

  // to disable continous mode
  write(fd, "0", 2);
  close(fd);

  return 0;

}
/****************************************************************
 * adc_get_buffer
 ****************************************************************/

/****************************************************************
 * adc_get_buffer
 ****************************************************************/
int adc_get_buffer(unsigned int adcPin, unsigned int *value) {
  int fd;
  char buf[MAX_BUF];
  char retChars[ADC_BUF];

  // Path to buffered ADC values
  snprintf(buf, sizeof(buf), SYSFS_ADC_DIR "/in_voltage%d_raw", adcPin);

  // Open the buffer file
  fd = open(buf, O_RDONLY | O_NONBLOCK);
  if (fd < 0) {
    perror("adc/get-buffer");
    return fd;
  }

  // Read buffer data
  ssize_t bytesRead = read(fd, retChars, sizeof(retChars));
  if (bytesRead < 0) {
    perror("adc/get-buffer/read");
    close(fd);
    return -1;
  }

  // Convert the read data to an integer value
  *value = strtol(retChars, NULL, 0);

  close(fd);
  return 0;
}