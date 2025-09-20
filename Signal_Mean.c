#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


#define SIGNAL_LENGTH 320
extern double InputSignal_f32_1kHz_15kHz[SIGNAL_LENGTH];
double calc_signal_mean(const double *signal, int length);

double MEAN;

int main() {
    MEAN = calc_signal_mean(&InputSignal_f32_1kHz_15kHz[0], SIGNAL_LENGTH);
    printf("Mean of the signal: %f\n", MEAN);

    return 0;  // Return success
}

double calc_signal_mean(const double *signal, int length) {

    double mean = 0.0f;
    for (size_t i = 0; i < length; i++) {
        mean += signal[i];
    }
    return mean / (double) length;

}

