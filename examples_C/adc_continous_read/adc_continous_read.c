#include "libioctrl.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <adcPin> <bufferLength>\n", argv[0]);
        return EXIT_FAILURE;
    }

    unsigned int adcPin = atoi(argv[1]);
    unsigned int bufferLength = atoi(argv[2]);
    unsigned int adcValue;
    float voltage;

    // Enable buffer and set buffer length
    if (enable_buffer() < 0) {
        fprintf(stderr, "Error enabling buffer.\n");
        return EXIT_FAILURE;
    }

    if (set_buffer_length(bufferLength) < 0) {
        fprintf(stderr, "Error setting buffer length.\n");
        disable_buffer();
        return EXIT_FAILURE;
    }

    // Enable ADC channel
    if (enable_channel(adcPin) < 0) {
        fprintf(stderr, "Error enabling ADC channel %u.\n", adcPin);
        disable_buffer();
        return EXIT_FAILURE;
    }

    printf("Starting continuous ADC read on pin %u...\n", adcPin);

    // Continuous read loop
    for (int i = 0; i < bufferLength; i++) {
        if (adc_get_buffer(adcPin, &adcValue) == 0) {
            adc_to_voltage(adcValue, &voltage);
            printf("ADC Value: %u, Voltage: %.3f V\n", adcValue, voltage);
        } else {
            fprintf(stderr, "Error reading buffer for ADC pin %u.\n", adcPin);
            break;
        }

        //usleep(500000); // Delay between reads (adjust as needed, e.g., 500 ms)
    }

    // Cleanup: disable buffer and channel
    disable_channel(adcPin);
    disable_buffer();

    printf("Continuous ADC read completed.\n");

    return EXIT_SUCCESS;
}
