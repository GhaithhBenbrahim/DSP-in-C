#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SIGNAL_LENGTH 320
extern double InputSignal_f32_1kHz_15kHz[SIGNAL_LENGTH]; 
void calc_sig_dft(const double *sig_input, double *output_real, double *output_imag, int length);

double DFT_real[SIGNAL_LENGTH / 2];
double DFT_imag[SIGNAL_LENGTH / 2];


int main() {

    FILE *f_input_sig , *f_output_real, *f_output_imag ;

    // Calculate the DFT of the input signal
    calc_sig_dft(&InputSignal_f32_1kHz_15kHz[0], &DFT_real[0], &DFT_imag[0], SIGNAL_LENGTH);

    f_input_sig = fopen("InputSignal.dat", "w");
    f_output_real = fopen("DFT_Real.dat", "w");
    f_output_imag = fopen("DFT_Imaginary.dat", "w");
    
    for (int i = 0; i < SIGNAL_LENGTH; i++) {      
        fprintf(f_input_sig, "\n%f", InputSignal_f32_1kHz_15kHz[i]);
    }

    for (int i = 0; i < SIGNAL_LENGTH / 2; i++) {   
        fprintf(f_output_real, "\n%f", DFT_real[i]);
        fprintf(f_output_imag, "\n%f", DFT_imag[i]);
    }

    fclose(f_input_sig);
    fclose(f_output_real);
    fclose(f_output_imag);      

    printf("DFT completed and results saved to files.\n");
    
    

    return 0;  // Return success
}

void calc_sig_dft(const double *sig_input, double *output_real, double *output_imag, int length)
 {
    int i, j, k;
    
    for (j = 0; j < length/2; j++) {
        output_real[j] = 0.0;
        output_imag[j] = 0.0;   
    }

    for (k = 0; k < length/2; k++) {
        for (i = 0; i < length; i++) {
            output_real[k] += sig_input[i] * cos(2 * M_PI * k * i / length);
            output_imag[k] -= sig_input[i] * sin(2 * M_PI * k * i / length);
        }
    }
}


