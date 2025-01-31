#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define DURATION 10
#define SAMPLE_RATE 10000
#define MAX_SAMPLES (DURATION * SAMPLE_RATE)
#define DETECT_SAMPLES 10000

double peak_detection_frequency(double signal[], double length, double sampling_rate);
void read_signal_from_file(const char *filename, double time[], double signal[], int *length);

int main() {
    double signal[MAX_SAMPLES];
    double time[MAX_SAMPLES];
    int length = 0;
    double duration_estimate_func = 0;
    read_signal_from_file("signal.txt", time, signal, &length);
    remove("est_freq.txt"); 
    FILE *file = fopen("est_freq.txt", "w");

    for (int i = DETECT_SAMPLES/2 ; i < MAX_SAMPLES - DETECT_SAMPLES/2; i++)
    {
        double temp[DETECT_SAMPLES];
        for (int j = -DETECT_SAMPLES/2 ; j < DETECT_SAMPLES/2; j++)
        {
            temp[j+DETECT_SAMPLES/2] = signal[i+j];
        }
        double start = clock();
        double peak_detection_freq = peak_detection_frequency(temp, DETECT_SAMPLES, SAMPLE_RATE);
        double stop = clock();
        fprintf(file, "%f %f\n", time[i], peak_detection_freq);
        if (duration_estimate_func<(stop-start))
        {
            duration_estimate_func = stop-start;
        }
    }
    printf("Duration of peak_detection_frequency function: %f\n", duration_estimate_func);
    fclose(file);
    

    return 0;
}


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



void read_signal_from_file(const char *filename, double time[], double signal[], int *length) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    while (fscanf(file, "%lf %lf", &time[index], &signal[index]) != EOF && index < MAX_SAMPLES) {
        index++;
    }
    *length = index;

    fclose(file);
}