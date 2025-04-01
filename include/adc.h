#ifndef ADC_H_
#define ADC_H_

/**************************************************
 * Contents                                       *
 * ***********************************************/

#define SYSFS_ADC_DIR "/sys/bus/iio/devices/iio:device0"
#define SYSFS_ADC_ENABLE_PATH "/sys/bus/iio/devices/iio:device0/buffer/enable"
#define SYSFS_ADC_LENGTH_PATH "/sys/bus/iio/devices/iio:device0/buffer/length"
#define SYSFS_ADC_SCAN_DIR "/sys/bus/iio/devices/iio:device0/scan_elements"

#define POLL_TIMEOUT (3 * 1000) /*3 seconds*/

#define MAX_BUF 256
#define MAX_LEN_BUF 100
#define ADC_BUF 6
#define REFVOLTAGE 1.8
#define MAXADC 4095 
#define MAX_SAMPLE_RATE 2000000 // 200kSPS

/************************************************
 * ADC RELATED                                  *
************************************************/
int adc_get_value(unsigned int adcPin, unsigned int *value);
int adc_fd_open(unsigned int gpio);
int adc_fd_close(unsigned int fd);
int adc_to_voltage(unsigned int adcValue, float *voltage);
int adc_get_buffer(unsigned int adcPin, unsigned int *value);
int enable_channel(unsigned int adcPin);
int disable_channel(unsigned int adcPin);
int enable_buffer(void);
int disable_buffer(void);
int set_buffer_length(unsigned int len);

#endif /* ADC_H_*/