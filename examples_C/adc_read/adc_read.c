#include "libioctrl.h"
#include <stdio.h>
#include <unistd.h>

int main(){
	unsigned int i = 0;
	unsigned int adc_reading = 0;
	while(i < 30){
		
		adc_get_value(0, &adc_reading);
		
		printf("ADC: %u\n ",adc_reading);
		sleep(1);
        i++;
	}
	return 0;
}
