#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SAMPLE_RATE 10000 // 10 KSPS
#define DURATION 10 // 10 seconds
#define PI 3.14159265358979323846

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

int main() {

    double base_frequency = 2000.0;
    double frequency_drift = 0.01;
    double dc_offset = 1300.0;
    double dc_drift = 0.05;
    double amplitude = 300.0;
    double amplitude_variation = 0.05;
    double noise_stddev = 0.67;
    
    remove("signal.txt"); 
    remove("freq.txt");
    FILE *file = fopen("signal.txt", "w");
    FILE *freq_file = fopen("freq.txt", "w");

    for (int i = 0; i < SAMPLE_RATE * DURATION; i++) {
        double t = (double)i / SAMPLE_RATE;
        double current_frequency = base_frequency * (1 + frequency_drift * t);
        double current_dc_offset = dc_offset * (1 + dc_drift * t);
        double current_amplitude = amplitude * (1 + amplitude_variation * t);
        double noise = generate_gaussian_noise(0, noise_stddev);

        double signal = current_dc_offset + current_amplitude * sin(2 * PI * current_frequency * t) + noise;
        fprintf(file, "%f %f\n", t, signal);
        fprintf(freq_file, "%f %f\n", t, current_frequency);
    }

    fclose(file);
    fclose(freq_file);

    return 0;
}