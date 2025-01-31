#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define N_SAMPLES 1000
#define THRESHOLD 2000 // Adjust this threshold based on your signal
#define SMPLE_RATE 10000
uint16_t sample_buf[N_SAMPLES];

void __not_in_flash_func(adc_capture)(uint16_t *buf, size_t count) {
    adc_fifo_setup(true, false, 0, false, false);
    adc_run(true);
    for (size_t i = 0; i < count; i = i + 1)
        buf[i] = adc_fifo_get_blocking();
    adc_run(false);
    adc_fifo_drain();
}

float estimate_frequency(uint16_t *buf, size_t count, float sample_rate) {
    size_t peak_count = 0;
    size_t last_peak_index = 0;
    float total_period = 0;

    for (size_t i = 1; i < count - 1; i++) {
        if (buf[i] > THRESHOLD && buf[i] > buf[i - 1] && buf[i] > buf[i + 1]) {
            if (peak_count > 0) {
                total_period += (i - last_peak_index) / sample_rate;
            }
            last_peak_index = i;
            peak_count++;
        }
    }

    if (peak_count > 1) {
        float average_period = total_period / (peak_count - 1);
        return 1.0 / average_period;
    } else {
        return 0; // Frequency estimation not possible
    }
}

int main(void) {
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);

    // Set all pins to input (as far as SIO is concerned)
    gpio_set_dir_all_bits(0);
    for (int i = 2; i < 30; ++i) {
        gpio_set_function(i, GPIO_FUNC_SIO);
        if (i >= 26) {
            gpio_disable_pulls(i);
            gpio_set_input_enabled(i, false);
        }
    }

    while (1) {
        char c = getchar();
        printf("%c", c);
        if (c == 'S') {
            adc_capture(sample_buf, N_SAMPLES);
            float frequency = estimate_frequency(sample_buf, N_SAMPLES, SMPLE_RATE); // Assuming 50kHz sample rate
            printf("Estimated Frequency: %.2f Hz\n", frequency);
        }
    }
}