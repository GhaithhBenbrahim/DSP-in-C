#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


#define SIGNAL_LENGTH 320
#define IMPULSE_RESPONSE_LENGTH 29

extern double InputSignal_f32_1kHz_15kHz[SIGNAL_LENGTH];
extern double Impulse_response[IMPULSE_RESPONSE_LENGTH];
double OutputSignal[SIGNAL_LENGTH + IMPULSE_RESPONSE_LENGTH ];

void calc_convolution(double *sig_src_arr, double *sig_dest_arr, double *imp_response_arr,
     int signal_length, int impulse_length) ;


int main() {

    FILE *f_input_imp , *f_input_sig , *f_output_sig;

    calc_convolution(&InputSignal_f32_1kHz_15kHz[0], &OutputSignal[0], &Impulse_response[0],
                     SIGNAL_LENGTH, IMPULSE_RESPONSE_LENGTH);


    f_input_sig = fopen("InputSignal.dat", "w");
    f_input_imp = fopen("Impulse_response.dat", "w");
    f_output_sig = fopen("OutputSignal.dat", "w");

    for (int i = 0; i < SIGNAL_LENGTH; i++) {
        fprintf(f_input_sig, "\n%f", InputSignal_f32_1kHz_15kHz[i]);
    }

    for (int i = 0; i < IMPULSE_RESPONSE_LENGTH; i++) {
        fprintf(f_input_imp, "\n%f", Impulse_response[i]);
    }   

    for (int i = 0; i < SIGNAL_LENGTH + IMPULSE_RESPONSE_LENGTH; i++) {
        fprintf(f_output_sig, "\n%f", OutputSignal[i]);
    }

    fclose(f_input_sig);
    fclose(f_input_imp);
    fclose(f_output_sig);

    printf("Convolution completed and results saved to files.\n");


    

    return 0;  // Return success
}

void calc_convolution(double *sig_src_arr, double *sig_dest_arr, double *imp_response_arr,
     int signal_length, int impulse_length) 
     {
        int i, j;
        for(i = 0; i < (signal_length + impulse_length) ; i++) {
            sig_dest_arr[i] = 0;
            }
        for(i = 0; i < signal_length; i++) {
            for(j = 0; j < impulse_length; j++) {
                sig_dest_arr[i + j] += sig_src_arr[i] * imp_response_arr[j];
            }


     }
    }
    
