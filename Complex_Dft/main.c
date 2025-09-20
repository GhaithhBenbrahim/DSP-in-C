#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif 


#define SIGNAL_LENGTH 501

extern double _501pts_20Hz_sig_imx[SIGNAL_LENGTH];
extern double _501pts_20Hz_sig_rex[SIGNAL_LENGTH];

void Complex_Dft(double *sig_src_time_domain_rex, double *sig_src_time_domain_imx,
                 double *sig_dest_freq_domain_rex, double *sig_dest_freq_domain_imx,
                 int signal_length) ;

double output_rex[SIGNAL_LENGTH];
double output_imx[SIGNAL_LENGTH];


int main() {

    FILE *f_input_IMX, *f_input_REX, *f_output_rex, *f_output_imx;

    // Calculate the magnitude and phase from rectangular coordinates
    Complex_Dft(&_501pts_20Hz_sig_rex[0], &_501pts_20Hz_sig_imx[0], &output_rex[0], &output_imx[0], SIGNAL_LENGTH);

    f_input_IMX = fopen("InputSignal_IMX.dat", "w");
    f_input_REX = fopen("InputSignal_REX.dat", "w");
    f_output_imx = fopen("OutputSignal_IMX.dat", "w");
    f_output_rex = fopen("OutputSignal_REX.dat", "w");        

    for (int i = 0; i < SIGNAL_LENGTH; i++) {
        fprintf(f_input_IMX, "\n%f", _501pts_20Hz_sig_imx[i]);
        fprintf(f_input_REX, "\n%f", _501pts_20Hz_sig_rex[i]);
    }
    for (int i = 0; i < SIGNAL_LENGTH; i++) {
        fprintf(f_output_rex, "\n%f", output_rex[i]);
        fprintf(f_output_imx, "\n%f", output_imx[i]);
    }

    fclose(f_input_IMX);
    fclose(f_input_REX);
    fclose(f_output_rex);
    fclose(f_output_imx);

    printf("Complex DFT completed and results saved to files.\n");


    return 0;  // Return success

}


void Complex_Dft(double *sig_src_time_domain_rex, double *sig_src_time_domain_imx,
                 double *sig_dest_freq_domain_rex, double *sig_dest_freq_domain_imx,
                 int signal_length) 
{
    double Sre, Sim;
    for (int k = 0; k < signal_length-1; k++) {
        for(int n = 0; n < signal_length-1; n++) {
            Sre = cos(2 * M_PI * k * n / signal_length);
            Sim = -sin(2 * M_PI * k * n / signal_length);

            sig_dest_freq_domain_rex[k] += sig_src_time_domain_rex[n] * Sre - sig_src_time_domain_imx[n] * Sim;
            sig_dest_freq_domain_imx[k] += sig_src_time_domain_rex[n] * Sim + sig_src_time_domain_imx[n] * Sre;
            
        }
    }
}

        