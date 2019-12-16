// #include "readFile.h"
#include "vsip.h"
#include "constant.h"
#include <stdio.h>
#include <time.h>

struct fdpcparam
{
    int row;
    vsip_cmview_f *echo;       //接收到一组的回波数据:16x600
    vsip_cmview_f *echo_pulse; //一组数据中的一个脉冲：600
    vsip_cmview_f *echo_fft;   //echo_pulse 经过fft之后的数据
    vsip_cmview_f *coeff_fft;  // coeff经过fft之后的数据
    vsip_cmview_f *pc0;        //中间变量
    vsip_cmview_f *pc;         //中间变量
    vsip_cmview_f *pc_fft;     //中间变量 echo_fft 和 coeff_fft 相乘之后的结果
    vsip_fftm_f *echoPlan;     //echo_pulse 做计算时用到的fft参数
    vsip_fftm_f *pcPlan;       //echo_pulse 做计算时用到的fft参数
};
/**
 * 计算频域脉压并按照脉冲号、距离门号重排数据
 * 
 * echo: 回波信号，600*16
 * echo_pulse: 回波信号一次脉冲的数据
 * echo_fft: 回波信号一次脉冲的傅立叶变换结果
 * coeff_fft: 脉冲压缩系数的傅里叶变换结果
 * pc_fft: 脉冲压缩
 * pc0: 逆傅里叶变换
 * pc1: 数据截取之后的
 * pc: 输出结果
 * echoPlan: echo的傅里叶计算
 * pcPlan:pc的傅里叶计算
 * 
*/
void frequencyDomainPulseCompression(vsip_cmview_f *echo, vsip_cmview_f *echo_pulse,
                                     vsip_cmview_f *echo_fft, vsip_cmview_f *coeff_fft,
                                     vsip_cmview_f *pc0, vsip_cmview_f *pc, vsip_cmview_f *pc_fft,
                                     vsip_fftm_f *echoPlan, vsip_fftm_f *pcPlan)
{
    int i, j;
    for (i = 0; i < PulseNumber; i++)
    {
        echo_pulse = vsip_cmsubview_f(echo, i, 0, 1, SampleNumber);
        vsip_ccfftmop_f(echoPlan, echo_pulse, echo_fft);
        vsip_cmmul_f(echo_fft, coeff_fft, pc_fft);
        vsip_ccfftmop_f(pcPlan, pc_fft, pc0);
        vsip_cmrsdiv_f(pc0, 4096, pc0);
        for (j = 0; j < SampleNumber; j++)
        {
            vsip_cmput_f(pc, i, j, vsip_cmget_f(pc0, 0, j));
        }
    }
}
void subfdpc(void *p)
{
    //子任务
    struct fdpcparam *param = (struct fdpcparam *)p;
    printf("%d\n", param->row);
    int j = 0;
    param->echo_pulse = vsip_cmsubview_f(param->echo, param->row, 0, 1, SampleNumber);
    vsip_ccfftmop_f(param->echoPlan, param->echo_pulse, param->echo_fft);
    vsip_cmmul_f(param->echo_fft, param->coeff_fft, param->pc_fft);
    vsip_ccfftmop_f(param->pcPlan, param->pc_fft, param->pc0);
    vsip_cmrsdiv_f(param->pc0, 4096, param->pc0);
    for (j = 0; j < SampleNumber; j++)
    {
        vsip_cmput_f(param->pc, param->row, j, vsip_cmget_f(param->pc0, 0, j));
    }
}
void frequencyDomainPulseCompression1(vsip_cmview_f *echo, vsip_cmview_f *echo_pulse,
                                      vsip_cmview_f *echo_fft, vsip_cmview_f *coeff_fft,
                                      vsip_cmview_f *pc0, vsip_cmview_f *pc, vsip_cmview_f *pc_fft,
                                      vsip_fftm_f *echoPlan, vsip_fftm_f *pcPlan, threadpool thpool)
{
    //线程池版本频域脉冲压缩
    struct fdpcparam param[PulseNumber];

    int i = 0;
    for (i = 0; i < PulseNumber; i++)
    {
        param[i].echo = echo;
        param[i].echo_pulse = echo_pulse;
        param[i].echo_fft = echo_fft;
        param[i].coeff_fft = coeff_fft;
        param[i].pc0 = pc0;
        param[i].pc = pc;
        param[i].pc_fft = pc_fft;
        param[i].echoPlan = echoPlan;
        param[i].pcPlan = pcPlan;
        param[i].row = i;
        //添加子任务
        thpool_add_work(thpool, subfdpc, (void *)&param[i]);
    }
    thpool_wait(thpool); //等待任务队列中的所有任务执行完毕
}