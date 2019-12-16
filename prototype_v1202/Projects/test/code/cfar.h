#include "cfar.c"
void constantFalseAlarmRate1(vsip_cmview_f *cfar_result, vsip_cmview_f *noise_gate, vsip_cmview_f *mtd,
                             int protect_number, int reference_number, int alpha, vsip_cscalar_f left,
                             vsip_cscalar_f right, vsip_cscalar_f temp,threadpool thpool);
void constantFalseAlarmRate(vsip_cmview_f *cfar_result, vsip_cmview_f *noise_gate, vsip_cmview_f *mtd,
                            int protect_number, int reference_number, int alpha, vsip_cscalar_f left,
                            vsip_cscalar_f right,vsip_cscalar_f temp);