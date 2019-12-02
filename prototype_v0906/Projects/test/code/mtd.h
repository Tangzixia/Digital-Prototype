#include "mtd.c"
void movingTargetDetection(vsip_cmview_f *mtd, vsip_cmview_f *pc, vsip_cmview_f *buff,
                           vsip_cmview_f *buff_fft, vsip_fftm_f *buffPlan);
void submtd(void *param);
void movingTargetDetection1(vsip_cmview_f *mtd, vsip_cmview_f *pc, vsip_cmview_f *buff,
                           vsip_cmview_f *buff_fft, vsip_fftm_f *buffPlan,threadpool thpool);                        