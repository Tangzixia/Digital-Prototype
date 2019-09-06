#include "fdpc.cpp"


void frequencyDomainPulseCompression(fftw_complex* echo,fftw_complex* echo_pulse,fftw_complex* pc_fft,
    fftw_complex* echo_fft,fftw_complex* coeff_fft,fftw_plan echo_plan,fftw_plan pcplan,
    fftw_complex* pc_freq0,fftw_complex* pc_freq1,fftw_complex** pc);