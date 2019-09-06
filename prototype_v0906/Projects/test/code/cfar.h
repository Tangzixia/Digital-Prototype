#include "fftw3.h"
#include "cfar.cpp"


void constantFalseAlarmRate(fftw_complex** cfar_result,fftw_complex** noise_gate,fftw_complex **mtd,
    int protect_num,int reference_num,int alpha);