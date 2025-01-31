#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SAMPLE_RATE 10000 // 10 KSPS
#define DURATION 1 // 10 seconds
#define ADC_SAMPLES 1000
#define PI 3.14159265358979323846
#define base_frequency  1400.0
#define frequency_drift  0.01
#define dc_offset  1300.0
#define dc_drift  0.05
#define amplitude  300.0
#define amplitude_variation 0.05
#define noise_stddev  0.67


struct ADC_struct {
    double signal;
    double frequency;
};

// Function to generate Gaussian noise
double generate_gaussian_noise(double mean, double stddev) {
    static double spare;
    static int has_spare = 0;

    if (has_spare) {
        has_spare = 0;
        return mean + stddev * spare;
    }

    has_spare = 1;
    double u, v, s;
    do {
        u = (rand() / ((double) RAND_MAX)) * 2.0 - 1.0;
        v = (rand() / ((double) RAND_MAX)) * 2.0 - 1.0;
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0);

    s = sqrt(-2.0 * log(s) / s);
    spare = v * s;
    return mean + stddev * u * s;
}
struct ADC_struct ADC_virtual(int num) {
    double t = (double) num / SAMPLE_RATE;
    double current_frequency = base_frequency * (1 + frequency_drift * t);
    double current_dc_offset = dc_offset * (1 + dc_drift * t);
    double current_amplitude = amplitude * (1 + amplitude_variation * t);
    double noise = generate_gaussian_noise(0, noise_stddev);
    double signal = current_dc_offset + current_amplitude * sin(2 * PI * current_frequency * t) + noise;
    struct ADC_struct out = {signal,current_frequency};
    return out;
}

double peak_detection_frequency(double signal[], double length, double sampling_rate) {
    double peak_count = 0.0;
    for (int i = 1; i < length - 1; i++) {
        if (signal[i] > signal[i-1] && signal[i] > signal[i+1] && signal[i] > dc_offset) {
            peak_count++;
        }
    }
    double frequency = peak_count * (sampling_rate / length);
    return frequency;
}


int main() {
    double signal_arr[SAMPLE_RATE * DURATION];
    double index = 0;
    double estimate_frequency = 0;
    double real_frequency = 0;
    struct ADC_struct ADC_out;
    remove("est_freq.txt");
    remove("real_freq.txt");
    FILE *est_freq_file = fopen("est_freq.txt", "w");
    FILE *real_freq_file =fopen("real_freq.txt", "w");
    while (index<ADC_SAMPLES)
    {
        for (int i = 0; i < SAMPLE_RATE * DURATION; i++) {
            ADC_out = ADC_virtual(i+index);
            signal_arr[i] = ADC_out.signal;
            real_frequency = ADC_out.frequency;
            if (i == 0)
            {
                fprintf(real_freq_file, "%f \n", real_frequency);
            }
        }
        estimate_frequency = peak_detection_frequency(signal_arr, SAMPLE_RATE * DURATION, SAMPLE_RATE);
        fprintf(est_freq_file, "%f \n", estimate_frequency);
        
        index ++;
    }
    fclose(est_freq_file);
    fclose(real_freq_file);
}