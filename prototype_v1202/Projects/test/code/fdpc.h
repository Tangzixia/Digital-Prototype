#include "fdpc.c"
void frequencyDomainPulseCompression(vsip_cmview_f *echo, vsip_cmview_f *echo_pulse,
                                     vsip_cmview_f *echo_fft, vsip_cmview_f *coeff_fft,
                                     vsip_cmview_f *pc0, vsip_cmview_f *pc, vsip_cmview_f *pc_fft,
                                     vsip_fftm_f *echoPlan, vsip_fftm_f *pcPlan);
void frequencyDomainPulseCompression1(vsip_cmview_f *echo, vsip_cmview_f *echo_pulse,
                                      vsip_cmview_f *echo_fft, vsip_cmview_f *coeff_fft,
                                      vsip_cmview_f *pc0, vsip_cmview_f *pc, vsip_cmview_f *pc_fft,
                                      vsip_fftm_f *echoPlan, vsip_fftm_f *pcPlan,
                                      fftw_complex *in_fftw, fftw_complex *out_fftw,
                                      fftw_plan fdpc_plan, int row_or_col);
