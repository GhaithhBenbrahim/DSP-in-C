#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


#define SIGNAL_LENGTH 320
extern double InputSignal_f32_1kHz_15kHz[SIGNAL_LENGTH];
double calc_signal_mean(const double *signal, int length);
double calc_signal_variance(const double *signal,double mean,int length);

double MEAN;
double VARIANCE;

// Main function to calculate the mean of the signal

int main() {
    MEAN = calc_signal_mean(&InputSignal_f32_1kHz_15kHz[0], SIGNAL_LENGTH);
    printf("Mean of the signal: %f\n", MEAN);
    VARIANCE = calc_signal_variance(&InputSignal_f32_1kHz_15kHz[0], MEAN, SIGNAL_LENGTH);
    printf("Variance of the signal: %f\n", VARIANCE);

    return 0;  // Return success
}

double calc_signal_mean(const double *signal, int length) {

    double mean = 0.0f;
    for (size_t i = 0; i < length; i++) {
        mean += signal[i];
    }
    return mean / (double) length;

}

double calc_signal_variance(const double *signal,double mean,int length) {
    double variance = 0.0f;
    for (size_t i = 0; i < length; i++) {
        variance += (signal[i] - mean) * (signal[i] - mean);
    }   
    return variance / (double) length;
}