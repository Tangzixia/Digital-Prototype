#include "cfar.h"
#include "fdpc.h"
#include "mtd.h"
#include "readFile.h"
#include "writeFile.h"
#include <stdio.h>
#include <stdlib.h>
int main()
{
    vsip_init((void *)0);
    //雷达中用到的数据初始化
    vsip_cmview_f *coeff = vsip_cmcreate_f(1, 4096, VSIP_ROW, VSIP_MEM_NONE);
    vsip_cmview_f *data = vsip_cmcreate_f(PulseNumber, SampleNumber, VSIP_ROW, VSIP_MEM_NONE);
    vsip_fftm_f *coeff_plan = vsip_ccfftmop_create_f(1, 4096, 1, VSIP_FFT_FWD, VSIP_ROW, 1, VSIP_ALG_SPACE);
    vsip_cmview_f *coeff_fft = vsip_cmcreate_f(1, 4096, VSIP_ROW, VSIP_MEM_NONE);
    readData("/home/a913/Qt_project/20191130/prototype_v0906/Projects/test/code/coeff.txt", coeff, 0, 0);
    vsip_ccfftmop_f(coeff_plan, coeff, coeff_fft);

    vsip_cmview_f *echo_pulse = vsip_cmcreate_f(1, 4096, VSIP_ROW, VSIP_MEM_NONE);
    vsip_cmview_f *echo_fft = vsip_cmcreate_f(1, 4096, VSIP_ROW, VSIP_MEM_NONE);
    vsip_cmview_f *pc_fft = vsip_cmcreate_f(1, 4096, VSIP_ROW, VSIP_MEM_NONE);
    vsip_cmview_f *pc0 = vsip_cmcreate_f(1, 4096, VSIP_ROW, VSIP_MEM_NONE);
    vsip_fftm_f *pc_plan = vsip_ccfftmop_create_f(1, 4096, 1, VSIP_FFT_INV, VSIP_ROW, 10, VSIP_ALG_NOISE);
    vsip_cmview_f *pc = vsip_cmcreate_f(PulseNumber, SampleNumber, VSIP_ROW, VSIP_MEM_NONE);

    vsip_cmview_f *mtd = vsip_cmcreate_f(PulseNumber, SampleNumber, VSIP_ROW, VSIP_MEM_CONST);
    vsip_cmview_f *buff = vsip_cmcreate_f(PulseNumber, 1, VSIP_ROW, VSIP_MEM_NONE);
    vsip_cmview_f *buff_fft = vsip_cmcreate_f(PulseNumber, 1, VSIP_ROW, VSIP_MEM_NONE);
    vsip_fftm_f *buff_plan = vsip_ccfftmop_create_f(PulseNumber, 1, 1, VSIP_FFT_FWD, VSIP_COL, 1, VSIP_ALG_NOISE);

    vsip_cmview_f *cfar_result = vsip_cmcreate_f(PulseNumber, SampleNumber, VSIP_ROW, VSIP_MEM_NONE);
    vsip_cmview_f *noise_gate = vsip_cmcreate_f(PulseNumber, SampleNumber, VSIP_ROW, VSIP_MEM_NONE);
    vsip_cscalar_f left, right, temp;
    int protect_number = 3;
    int reference_number = 50;
    int alpha = 2;
    struct timeval t1, t2, t3; //实验计时用//time

    threadpool thpool1 = thpool_init(16);
    threadpool thpool2 = thpool_init(16);
    threadpool thpool3 = thpool_init(16);
    int i = 0;
    double count = 0.0;//time
    int number = 1; //数据的组数，一组为9600
    for (i = 0; i < number; i++)
    { // 每个for循环里面是读取一组数据（16*600）
        vsip_fftm_f *echo_plan = vsip_ccfftmop_create_f(1, 4096, 1, VSIP_FFT_FWD, VSIP_ROW, 10, VSIP_ALG_NOISE);
        readData("/home/a913/Qt_project/20191130/prototype_v0906/Projects/test/code/Echoes.txt", data, i * 16 * 4096, 1);
        gettimeofday(&t1, NULL);//time
        frequencyDomainPulseCompression(data, echo_pulse, echo_fft, coeff_fft, pc0, pc, pc_fft, echo_plan, pc_plan);
        // frequencyDomainPulseCompression1(data, echo_pulse, echo_fft, coeff_fft, pc0, pc, pc_fft, echo_plan, pc_plan, thpool1);
        movingTargetDetection(mtd, pc, buff, buff_fft, buff_plan);
        // movingTargetDetection1(mtd, pc, buff, buff_fft, buff_plan, thpool2);

        //constantFalseAlarmRate(cfar_result, noise_gate, mtd, protect_number, reference_number, alpha, left, right, temp);
        constantFalseAlarmRate1(cfar_result, noise_gate, mtd, protect_number, reference_number, alpha, left, right, temp, thpool3);

        gettimeofday(&t2, NULL);//time
        printf("speed time is:%f(ms)\n", (double)(t2.tv_sec * 1e6 + t2.tv_usec) / 1000.0 - (double)(t1.tv_sec * 1e6 + t1.tv_usec) / 1000.0);//time
        // thpool_wait(thpool1);
        // thpool_wait(thpool2);
        // thpool_wait(thpool3);
        
        // thpool_wait(thpool3);
        writeData("/home/a913/Qt_project/20191130/prototype_v0906/Projects/test/code/result.txt", cfar_result);
        count += (double)(t2.tv_sec * 1e6 + t2.tv_usec) / 1000.0 - (double)(t1.tv_sec * 1e6 + t1.tv_usec) / 1000.0;//time
    }

    thpool_destroy(thpool1);
    thpool_destroy(thpool2);

    thpool_destroy(thpool3);

    printf("the avgerage time: %f\n", count / number);//time

    vsip_finalize((void *)0);
    return 0;
}
