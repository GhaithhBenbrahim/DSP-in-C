#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif 


#define SIGNAL_LENGTH 320

extern double _320_pts_ecg_IMX[SIGNAL_LENGTH];
extern double _320_pts_ecg_REX[SIGNAL_LENGTH];

void Rect_to_Polar(double *real, double *imag, double *magnitude, double *phase, int length);

double magnitude[SIGNAL_LENGTH];
double phase[SIGNAL_LENGTH];


int main() {

    FILE *f_input_IMX, *f_input_REX, *f_output_mag, *f_output_phase;

    // Calculate the magnitude and phase from rectangular coordinates
    Rect_to_Polar(&_320_pts_ecg_REX[0], &_320_pts_ecg_IMX[0], &magnitude[0], &phase[0], SIGNAL_LENGTH);

    f_input_IMX = fopen("InputSignal_IMX.dat", "w");
    f_input_REX = fopen("InputSignal_REX.dat", "w");
    f_output_mag = fopen("Magnitude.dat", "w");
    f_output_phase = fopen("Phase.dat", "w");

    for (int i = 0; i < SIGNAL_LENGTH; i++) {   
        fprintf(f_input_IMX, "\n%f", _320_pts_ecg_IMX[i]);
        fprintf(f_input_REX, "\n%f", _320_pts_ecg_REX[i]);
    }
    for (int i = 0; i < SIGNAL_LENGTH; i++) {   
        fprintf(f_output_mag, "\n%f", magnitude[i]);
        fprintf(f_output_phase, "\n%f", phase[i]);
    }   

    fclose(f_input_IMX);
    fclose(f_input_REX);
    fclose(f_output_mag);
    fclose(f_output_phase);     

    printf("Rectangular to Polar conversion completed and results saved to files.\n");

    return 0;  // Return success

}

void Rect_to_Polar(double *real, double *imag, double *magnitude, double *phase, int length) {
    for (int i = 0; i < length; i++) {
        magnitude[i] = sqrt(real[i] * real[i] + imag[i] * imag[i]);
        if(magnitude[i] == 0) {
            magnitude[i] = pow(10, -20); 
            phase[i] = atan2(imag[i], real[i]);
        }
        if(real[i] < 0 && imag[i] < 0) {
            phase[i] = phase[i] - M_PI;
        }
        if(real[i] < 0 && imag[i] >= 0) {
            phase[i] = phase[i] + M_PI;
        }
        //phase[i] = atan2(imag[i], real[i]);
    }
}