#include "libioctrl.h"
#include <string.h>
#include <unistd.h>
int main() {
    PWM pwm;

    int period = 1000000; // Set period to 1 ms (1 kHz)
    int duty = 1000000; // Set duty cycle to 0.5 ms (50%)

    // Initialize the PWM structure for chip 0, channel 0
    pwm_init(&pwm, "P8_13");
    
    printf("Phy: %s\nChannel: %s\nChip: %s\nPeriod path: %s\nDuty Cycle path: %s\nEnable path: %s\nPin Mode Path: %s\nPin Mode Sate: %s\n",pwm.phy_pin,pwm.channel,pwm.chip,pwm.period_path,pwm.duty_cycle_path,pwm.enable_path,pwm.pin_mode_path, pwm.pin_mode_state);

    pwm_set_period(&pwm, period);
    pwm_set_duty_cycle(&pwm, duty);
    pwm_enable(&pwm);

    sleep(2);
    pwm_set_duty_cycle(&pwm, 500000);

    sleep(2);
    pwm_set_duty_cycle(&pwm, 200000);

    sleep(2);
    pwm_set_duty_cycle(&pwm, 100000);

    sleep(2);
    pwm_disable(&pwm);
    pwm_cleanup(&pwm);

    return 0;
}
