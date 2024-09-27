#include "../include/pwm.h"

// Initialize PWM structure with precomputed file paths
int pwm_init(PWM *pwm, const char *pin_name) {

    // Parse the pwmchip and pwm channel
    char pwm_chip[4], pwm_channel[4];

    for (int i = 0; i < sizeof(phy_pin_map) / sizeof(PWMPinMap); i++) {
        if (strcmp(phy_pin_map[i].physical_pin, pin_name) == 0) {
            // Parse the pwmchip and pwm channel
            sscanf(phy_pin_map[i].pwm_chip_channel, "%[^:]:%s", pwm_channel, pwm_chip);
        }
    }

    // Store the PWM chip and channel as a string

    strncpy(pwm->phy_pin, pin_name, sizeof(pwm->phy_pin));
    strncpy(pwm->chip, pwm_chip, sizeof(pwm->chip));
    strncpy(pwm->channel, pwm_channel, sizeof(pwm->channel));

    // Precompute the file paths using the defined base paths
    snprintf(pwm->period_path, sizeof(pwm->period_path), PWM_PERIOD_PATH, pwm_channel, pwm_chip);
    snprintf(pwm->duty_cycle_path, sizeof(pwm->duty_cycle_path), PWM_DUTY_CYCLE_PATH, pwm_channel, pwm_chip);
    snprintf(pwm->enable_path, sizeof(pwm->enable_path), PWM_ENABLE_PATH, pwm_channel, pwm_chip);
    

    return 0;
}

// Clean up PWM structure (no-op in this case, but placeholder for future)
int pwm_cleanup(PWM *pwm) {
    // No dynamic memory allocation done, so no cleanup needed
    pwm_set_period(pwm, PWM_PERIOD_DEFAULT);
    pwm_set_duty_cycle(pwm, PWM_DUTY_CYCLE_DEFAULT);
    pwm_enable(pwm);

    return 0;
}

// Set the PWM period
int pwm_set_period(PWM *pwm, unsigned int period_ns) {
    FILE *fp = fopen(pwm->period_path, "w");
    if (fp == NULL) {
        perror("Error opening period file");
        return -1;
    }

    fprintf(fp, "%u", period_ns);
    fclose(fp);
    return 0;
}

// Set the PWM duty cycle
int pwm_set_duty_cycle(PWM *pwm, unsigned int duty_cycle_ns) {
    FILE *fp = fopen(pwm->duty_cycle_path, "w");
    if (fp == NULL) {
        perror("Error opening duty_cycle file");
        return -1;
    }

    fprintf(fp, "%u", duty_cycle_ns);
    fclose(fp);
    return 0;
}

// Enable the PWM output
int pwm_enable(PWM *pwm) {
    FILE *fp = fopen(pwm->enable_path, "w");
    if (fp == NULL) {
        perror("Error opening enable file");
        return -1;
    }

    fprintf(fp, "1");
    fclose(fp);
    return 0;
}

// Disable the PWM output
int pwm_disable(PWM *pwm) {
    FILE *fp = fopen(pwm->enable_path, "w");
    if (fp == NULL) {
        perror("Error opening enable file");
        return -1;
    }

    fprintf(fp, "0");
    fclose(fp);
    return 0;
}
