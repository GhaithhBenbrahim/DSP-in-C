#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SIGNAL_LENGTH 320
#define KERNEL_LENGTH 29

extern double InputSignal_f32_1kHz_15kHz[SIGNAL_LENGTH];

void bandpass_via_spectral_inversion(double *input, double *output, double *bp_kernel,
                                     int input_length, int kernel_length,
                                     double low_cutoff, double high_cutoff);

int main() {
    double bp_kernel[KERNEL_LENGTH] = {0};
    double OutputSignal[SIGNAL_LENGTH - KERNEL_LENGTH + 1] = {0};

    // Example: Band-pass between 5kHz and 15kHz, sample rate 48kHz
    // Normalized frequencies: 0.1Khz = 0.00208333, 5.2Khz = 0.1083333 keep 1khz
    // Normalized frequencies: 12kHz = 0.25, 17kHz = 0.3541667  keep 15kHz
    // These values are normalized to the Nyquist frequency (24kHz for 48kHz sample rate)
    // low_cutoff = 5 / 24 * 0.5 = 0.1041667
    // high_cutoff = 15 / 24 * 0.5 = 0.3125
    // Adjusted for the band-pass filter design
    
    bandpass_via_spectral_inversion(InputSignal_f32_1kHz_15kHz, OutputSignal, bp_kernel,
                                    SIGNAL_LENGTH, KERNEL_LENGTH, 0.25, 0.3541667);

    FILE *f_kernel = fopen("BandPassKernel.dat", "w");
    FILE *f_output = fopen("BandPassOutput.dat", "w");
    for (int n = 0; n < KERNEL_LENGTH; n++)
        fprintf(f_kernel, "%f\n", bp_kernel[n]);
    for (int i = 0; i < SIGNAL_LENGTH - KERNEL_LENGTH + 1; i++)
        fprintf(f_output, "%f\n", OutputSignal[i]);
    fclose(f_kernel);
    fclose(f_output);

    printf("Band-pass filter (via spectral inversion) completed and results saved to files.\n");
    return 0;
}

void bandpass_via_spectral_inversion(double *input, double *output, double *bp_kernel,
                                     int input_length, int kernel_length,
                                     double low_cutoff, double high_cutoff) {
    int M = kernel_length - 1;

    double lp_low[KERNEL_LENGTH] = {0};    // Low-pass at low_cutoff
    double lp_high[KERNEL_LENGTH] = {0};   // Low-pass at high_cutoff (to invert)
    double hp_high[KERNEL_LENGTH] = {0};   // High-pass from inversion of lp_high
    double br_kernel[KERNEL_LENGTH] = {0}; // Band-reject kernel

    // Step 1: Low-pass filter at low_cutoff
    for (int n = 0; n < kernel_length; n++) {
        double x = n - M / 2.0;
        if (x == 0.0)
            lp_low[n] = 2.0 * low_cutoff;
        else
            lp_low[n] = sin(2.0 * M_PI * low_cutoff * x) / (M_PI * x);
        // Apply Hamming window
        lp_low[n] *= 0.54 - 0.46 * cos(2.0 * M_PI * n / M);
    }

    // Step 2: Low-pass at high_cutoff (for inversion)
    for (int n = 0; n < kernel_length; n++) {
        double x = n - M / 2.0;
        if (x == 0.0)
            lp_high[n] = 2.0 * high_cutoff;
        else
            lp_high[n] = sin(2.0 * M_PI * high_cutoff * x) / (M_PI * x);
        lp_high[n] *= 0.54 - 0.46 * cos(2.0 * M_PI * n / M);
    }

    // Step 3: Spectral inversion to get high-pass from lp_high
    for (int n = 0; n < kernel_length; n++) {
        hp_high[n] = -lp_high[n];
    }
    hp_high[M / 2] += 1.0;

    // Step 4: Add low-pass and high-pass to get band-reject
    for (int n = 0; n < kernel_length; n++) {
        br_kernel[n] = lp_low[n] + hp_high[n];
    }

    // Step 5: Spectral inversion to get band-pass
    for (int n = 0; n < kernel_length; n++) {
        bp_kernel[n] = -br_kernel[n];
    }
    bp_kernel[M / 2] += 1.0;

    // Step 6: Convolve input signal with band-pass kernel
    int out_length = input_length - kernel_length + 1;
    for (int i = 0; i < out_length; i++) {
        output[i] = 0.0;
        for (int j = 0; j < kernel_length; j++) {
            output[i] += input[i + j] * bp_kernel[j];
        }
    }
}
