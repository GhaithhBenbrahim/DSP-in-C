#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif 

/**@note Cutoff freq is normalized and must be between 0 and 0.5 
 * represents the nyquist frequency.
 * in the exemple the sample rate is 48khz there for nyquist is 24khz
 * filter kernel 10khz cutoff frequency is (10/24)*0.5 = 0.2083333
 */
  
#define SIGNAL_LENGTH 320
#define KERNAL_LENGTH 29

extern double InputSignal_f32_1kHz_15kHz[SIGNAL_LENGTH];

void windowed_sinc_lpf(double *sig_src_arr , double *sig_dest_arr,
                       double *filter_kernel, double cutoff_freq,
                       int signal_length, int filter_length);

double filter_kernel[KERNAL_LENGTH] = {0}; // Filter kernel for the low-pass filter

double OutputSignal[SIGNAL_LENGTH - KERNAL_LENGTH] = {0}; // Output signal after filtering



int main() {

    FILE *f_input_signal, *f_output_signal, *f_filter_kernel;

    windowed_sinc_lpf(InputSignal_f32_1kHz_15kHz, OutputSignal, filter_kernel, 
                            0.2083333, (int) SIGNAL_LENGTH, (int) KERNAL_LENGTH);

    f_input_signal = fopen("InputSignal.dat", "w");
    f_output_signal = fopen("OutputSignal.dat", "w");
    f_filter_kernel = fopen("FilterKernel.dat", "w");   

    for (int i = 0; i < SIGNAL_LENGTH; i++) {
        fprintf(f_input_signal, "\n%f", InputSignal_f32_1kHz_15kHz[i]);
        if (i < KERNAL_LENGTH) {
            fprintf(f_filter_kernel, "\n%f", filter_kernel[i]);
        }
        if (i >= KERNAL_LENGTH) {
            fprintf(f_output_signal, "\n%f", OutputSignal[i]);
        }
    }   

    

    fclose(f_input_signal);
    fclose(f_output_signal);
    fclose(f_filter_kernel);        

    printf("Windowed Sinc LPF completed and results saved to files.\n");


    return 0;  // Return success

}

void windowed_sinc_lpf(double *sig_src_arr , double *sig_dest_arr,
                       double *filter_kernel, double cutoff_freq,
                       int signal_length, int filter_length)
{
    //calculate the low pass filter kernal
    for (int i = 0; i < filter_length; i++) {
        double n = (double)i - (double)filter_length / 2.0;
        if (n == 0) {
            filter_kernel[i] = 2.0 * M_PI * cutoff_freq;
        } else {
            filter_kernel[i] = sin(2.0 * M_PI * cutoff_freq * n) / (M_PI * n);
            filter_kernel[i] *= 0.54 - 0.46 * cos(2.0 * M_PI * n); // Hamming window
        }
    }

    // Convolve the input signal with the filter kernel
    for (int i = 0; i < signal_length; i++) {
        sig_dest_arr[i] = 0.0;
        for (int j = 0; j < filter_length; j++) {
            sig_dest_arr[i] += sig_src_arr[i - j] * filter_kernel[j];
        }
    }
}