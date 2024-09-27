#include "libioctrl.h"
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

int main() {
    PWM pwm;
    unsigned int adc_reading = 0;
    unsigned int user_defined_period_ns = 100000; // User-defined period in nanoseconds (1 ms for 1 kHz)
    unsigned int duty_cycle = 0;

    // Initialize PWM on a specified pin, for example "P8_13"
    pwm_init(&pwm, "P8_13");

    // Set initial PWM period
    pwm_set_period(&pwm, user_defined_period_ns);

    // Enable PWM output
    pwm_enable(&pwm);

    printf("PWM initialized with:\nPhysical Pin: %s\nChannel: %s\nChip: %s\nPeriod path: %s\nDuty Cycle path: %s\nEnable path: %s\n",
           pwm.phy_pin, pwm.channel, pwm.chip, pwm.period_path, pwm.duty_cycle_path, pwm.enable_path);

    // Loop to read ADC values and update PWM duty cycle
    for (int i = 0; i < 30; i++) {
        adc_get_value(0, &adc_reading); // Read ADC value from channel 0
        printf("ADC Reading: %u\n", adc_reading);

        // Map the ADC reading to the PWM duty cycle and update it

        duty_cycle = map(adc_reading, user_defined_period_ns);
        pwm_set_duty_cycle(&pwm, duty_cycle);

        //pwm_set_duty_cycle_from_adc(&pwm, adc_reading, user_defined_period_ns);

        sleep(1); // Update every 1 second
    }

    // Disable PWM and clean up
    pwm_disable(&pwm);
    pwm_cleanup(&pwm);

    return 0;
}