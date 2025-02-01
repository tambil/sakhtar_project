/***************************************************
** ADC Example for Making Embedded Systems course **
** This example is lifted from the RPi Pico SDK Examples
** page: https://raspberrypi.github.io/pico-sdk-doxygen/group__hardware__adc.html#adc_example
** 
****************************************************/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define arr_size 5000
#define sample_rate 10000

double peak_detection_frequency(double signal[], double length, double sampling_rate) {
    double peak_count = 0.0;
    for (int i = 1; i < length - 1; i++) {
        if (signal[i] > signal[i-1] && signal[i] > signal[i+1]) {
            peak_count++;
        }
    }
    double frequency = peak_count * (sampling_rate / length);
    return frequency;
}
 
int main() {
    stdio_init_all();
    printf("ADC Example, measuring GPIO26\n");
 
    adc_init();
 
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);
    double signal[arr_size];

    while (1) {
        uint16_t result = adc_read();
        for(int i = 0 ; i<arr_size - 1 ; i++){
          signal[i] = signal[i+1];
        }
        signal[arr_size-1] = result;
        double freq = peak_detection_frequency(signal,arr_size,sample_rate);
        printf("estimate frequency is : %f \n" , freq);
        sleep_ms(500);
    }
}