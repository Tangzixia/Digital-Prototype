#include <stdio.h>
#include "vsip.h"
int main(){
    vsip_init((void*)0);
    /*vsip_index length = 10;
    vsip_scalar_f scale = 1;
    vsip_fft_dir dir = VSIP_FFT_FWD;
    vsip_alg_hint hint = VSIP_ALG_SPACE;
    vsip_length ntimes = 10;
    printf(vsip_ccfftip_create_f(length,scale,dir,ntimes,hint));*/
    vsip_mview_f *a = vsip_mcreate_f(50,25,VSIP_ROW,VSIP_MEM_NONE);
    vsip_vview_f *b = vsip_vcreate_f(50,0);
    vsip_vview_f *c = vsip_vcreate_f(50,0);
    vsip_vfill_f(5,b);
    vsip_mfill_f(5,a);
    vsip_vview_f *kernel = vsip_vcreate_f(5,0);
    vsip_vfill_f(1,kernel);
    vsip_conv1d_f *plan1 = vsip_conv1d_create_f(kernel,VSIP_NONSYM,50,2,VSIP_SUPPORT_FULL,1000000,VSIP_ALG_SPACE);
    vsip_convolve1d_f(plan1,b,c);
    //vsip_vmul_f(a,b,c);
    int i = 0;
    int j = 0;
    int col = (int)vsip_mgetcollength_f(a);
    int row = (int)vsip_mgetrowlength_f(a);
    for (i = 0; i < row; i++)
    {
        for (j = 0;j < col;j++){
            //printf("%6.3f\t",vsip_vget_f(a,i));
            printf("%6.3f\t",vsip_mget_f(a,i,j));
        }
        printf("%d",i);
        printf("\n");
    }
    vsip_malldestroy_f(a);
    vsip_finalize((void*)0);
    return 0;
}