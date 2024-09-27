#include "../include/pwm.h"

// Initialize PWM structure with precomputed file paths
int pwm_init(PWM *pwm, const char *pin_name) {

    // Parse the pwmchip and pwm channel
    char pwm_chip[4], pwm_channel[4];

    for (int i = 0; i < sizeof(pwm_phy_pin_map) / sizeof(PWMPinMap); i++) {
        if (strcmp(pwm_phy_pin_map[i].physical_pin, pin_name) == 0) {
            // Parse the pwmchip and pwm channel
            sscanf(pwm_phy_pin_map[i].pwm_chip_channel, "%[^:]:%s", pwm_channel, pwm_chip);
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
    snprintf(pwm->pin_mode_path, sizeof(pwm->pin_mode_path), PWM_PIN_MODE_PATH, pin_name);

    // Set PWM pin for PWM mode
    pwm_set_pin_mode(pwm);

    // Get PWM pin state
    pwm_get_pin_mode(pwm);

    return 0;
}

// Clean up PWM structure (no-op in this case, but placeholder for future)
int pwm_cleanup(PWM *pwm) {
    // No dynamic memory allocation done, so no cleanup needed
    pwm_set_period(pwm, PWM_PERIOD_DEFAULT);
    pwm_set_duty_cycle(pwm, PWM_DUTY_CYCLE_DEFAULT);
    pwm_enable(pwm);
    pwm_unset_pin_mode(pwm);

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

// Get pin mode current state
int pwm_get_pin_mode(PWM *pwm){

    FILE *fp = fopen(pwm->pin_mode_path, "r");

    if (fp == NULL) {
        perror("Error opening state file");
        return -1;
    }
    

     // Clear the buffer and read up to STATE_BUF - 1 characters
    memset(pwm->pin_mode_state, 0, sizeof(pwm->pin_mode_state)); // Clear the buffer
    size_t chars_read = fread(pwm->pin_mode_state, 1, sizeof(pwm->pin_mode_state) - 1, fp);

    // Null-terminate the string to avoid junk data
    pwm->pin_mode_state[chars_read] = '\0';

    fclose(fp);  // Close the file after reading

    return 0;
}

// Set pin mode to PWM
int pwm_set_pin_mode(PWM *pwm) {
    
    FILE *fp = fopen(pwm->pin_mode_path, "w");
    if (fp == NULL) {
        perror("Error opening state file");
        return -1;
    }
    
    fprintf(fp, "pwm");
    fclose(fp);
    return 0;
}

// Set pin mode back to default
int pwm_unset_pin_mode(PWM *pwm) {
    
    FILE *fp = fopen(pwm->pin_mode_path, "w");
    if (fp == NULL) {
        perror("Error opening state file");
        return -1;
    }
    
    fprintf(fp, "default");
    fclose(fp);
    return 0;
}

// Map ADC value to PWM duty cycle with user-specified period
unsigned int map(u_int16_t adc_value, unsigned int period_ns) {

    if (adc_value > 4095) {
        fprintf(stderr, "ADC value out of range\n");
        return 1;
    }

    // Map ADC value (0 to 4095) to PWM duty cycle (0 to period_ns)
    unsigned int duty_cycle_ns = (adc_value * period_ns) / 4095;

    return duty_cycle_ns;
}