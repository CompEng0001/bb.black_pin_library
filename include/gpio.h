#ifndef SIMPLEGPIO_H_
#define SIMPLEGPIO_H_

/**************************************************
 * Contents                                       *
 * ***********************************************/

#define SYSFS_GPIO_DIR "/sys/class/gpio/"

#define POLL_TIMEOUT (3 * 1000) /*3 seconds*/

#define MAX_BUF 256
#define ADC_BUF 6

#define LED_DIR "/sys/class/leds/beaglebone:green:usr"


// Define a simple mapping array for P8 and P9 pin names to GPIO numbers
typedef struct {
    char pin_name[6];
    unsigned int gpio_number;
} GPIOPinMap;

// Static array to map pin names to GPIO numbers
GPIOPinMap gpio_pin_map[] = {
    {"P8_31", 10},  {"P8_32", 11},  {"P9_31", 110}, {"P9_29", 111},
    {"P9_30", 112}, {"P9_28", 113}, {"P9_92", 114}, {"sysfs", 115},
    {"P9_91", 116}, {"P9_25", 117}, {"P9_20", 12},  {"P9_19", 13},
    {"P9_26", 14},  {"P9_24", 15},  {"A15", 19},    {"P9_22", 2},
    {"P9_41", 20},  {"P8_19", 22},  {"P8_13", 23},  {"P8_14", 26},
    {"P9_21", 3},   {"P9_11", 30},  {"P9_13", 31},  {"P8_25", 32},
    {"P8_24", 33},  {"P8_05", 34},  {"P8_06", 35},  {"P8_23", 36},
    {"P8_22", 37},  {"P8_03", 38},  {"P8_04", 39},  {"P9_18", 4},
    {"P8_12", 44},  {"P8_11", 45},  {"P8_16", 46},  {"P8_15", 47},
    {"P9_15", 48},  {"P9_23", 49},  {"P9_17", 5},   {"P9_14", 50},
    {"P9_16", 51},  {"P9_12", 60},  {"P8_26", 61},  {"P8_21", 62},
    {"P8_20", 63},  {"P8_18", 65},  {"P8_07", 66},  {"P8_08", 67},
    {"P8_10", 68},  {"P8_09", 69},  {"P9_42", 7},   {"P8_45", 70},
    {"P8_46", 71},  {"P8_43", 72},  {"P8_44", 73},  {"P8_41", 74},
    {"P8_42", 75},  {"P8_39", 76},  {"P8_40", 77},  {"P8_37", 78},
    {"P8_38", 79},  {"P8_35", 8},   {"P8_36", 80},  {"P8_34", 81},
    {"P8_27", 86},  {"P8_29", 87},  {"P8_28", 88},  {"P8_30", 89},
    {"P8_33", 9},   {"P8_17",27}
};

typedef enum{
	INPUT_PIN=0,
	OUTPUT_PIN=1
} PIN_DIRECTION;

typedef enum {
	LOW=0,
	HIGH=1
} PIN_VALUE;

/*************************************************
 *  GPIO RELATED FUNCTIONS (DIGITAL)
 ************************************************/
int find_gpio_number(const char *pin_name);
int gpio_set_dir(const char *pin_name, PIN_DIRECTION out_flag);
int gpio_set_value(const char *pin_name, PIN_VALUE value);
int gpio_get_value(const char *pin_name, unsigned int *value);
int gpio_set_edge(const char *pin_name, const char *edge);
int gpio_fd_open(const char *pin_name);
int gpio_fd_close(int fd);

/************************************************
 *   USER LEDS
 ***********************************************/

int led_set_value(unsigned int led, PIN_VALUE value); // expects an led value :[0,3]

#endif /* SIMPLEGPIO_H_*/