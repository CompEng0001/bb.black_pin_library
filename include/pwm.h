#ifndef PWM_H
#define PWM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Base paths for PWM sysfs
#define PWM_BASE_PATH "/sys/class/pwm"
#define PWM_PERIOD_PATH "/sys/class/pwm/pwm-%s:%s/period"
#define PWM_DUTY_CYCLE_PATH "/sys/class/pwm/pwm-%s:%s/duty_cycle"
#define PWM_ENABLE_PATH "/sys/class/pwm/pwm-%s:%s/enable"
#define PWM_PIN_MODE_PATH "/sys/devices/platform/ocp/ocp:%s_pinmux/state"

#define STATE_BUF 6

// Default values that will be used by pwm_clean_up
#define PWM_PERIOD_DEFAULT 10000000
#define PWM_DUTY_CYCLE_DEFAULT 0
#define PWM_ENABLE_DEFAULT 0


// Structure to hold PWM mapping information
typedef struct {
    char physical_pin[10];     // Physical pin on the BeagleBone Black (e.g., "P9_16")
    char pwm_chip_channel[10]; // PWM chip and channel (e.g., "4:1")
} PWMPinMap;


// Define a structure to hold precomputed file paths for PWM control
typedef struct {
    char phy_pin[10];
    char chip[4];
    char channel[4];
    char period_path[128];
    char duty_cycle_path[128];
    char enable_path[128];
    char pin_mode_path[256];
    char pin_mode_state[6];
} PWM;

// Define the pin_map array with physical pins and their corresponding PWM chip:channel
PWMPinMap pwm_phy_pin_map[] = {
    {"P9_16", "4:1"},  // eHRPWM1B
    {"P9_14", "4:0"},  // eHRPWM1A
    {"P9_21", "1:1"},  // eHRPWM0B
    {"P9_22", "1:0"},  // eHRPWM0A
    {"P8_13", "7:1"},  // eHRPWM2B
    {"P8_19", "7:0"},  // eHRPWM2A
    {"P9_28", "3:0"},  // eCAP1
    {"P9_42", "0:0"}   // eCAP0
};

// Function prototypes
int pwm_init(PWM *pwm, const char *pin_name);
int pwm_cleanup(PWM *pwm);
int pwm_set_period(PWM *pwm, unsigned int period_ns);
int pwm_set_duty_cycle(PWM *pwm, unsigned int duty_cycle_ns);
int pwm_enable(PWM *pwm);
int pwm_disable(PWM *pwm);
int pwm_get_pin_mode(PWM *pwm);
int pwm_set_pin_mode(PWM *pwm);
int pwm_unset_pin_mode(PWM *pwm);
unsigned int map(u_int16_t adc_value, unsigned int period_ns);

#endif // PWM_H