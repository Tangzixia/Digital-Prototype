#include <vsip.h>
#include "constant.h"
#include "thpool.h"
/**
 * 动目标检测,区分不同速度的目标，有测速作用
 * mtd:输出
 * pc:输入
 * buff:截取输入的每一个脉冲
 * buff_fft:buff傅里叶变换结果
 * buffPlan:buff傅里叶变换
 * */
void movingTargetDetection(vsip_cmview_f *mtd, vsip_cmview_f *pc, vsip_cmview_f *buff,
                           vsip_cmview_f *buff_fft, vsip_fftm_f *buffPlan)
{
    int i, j;
    for (i = 0; i < SampleNumber; i++)
    {
        buff = vsip_cmsubview_f(pc, 0, i, PulseNumber, 1);
        vsip_ccfftmop_f(buffPlan, buff, buff_fft);
        for (j = 0; j < PulseNumber; j++)
        {
            vsip_cmput_f(mtd, j, i, vsip_cmget_f(buff_fft, j, 0));
        }
    }
}
struct mtdParam
{
    int col;
    vsip_cmview_f *mtd;      //输出
    vsip_cmview_f *pc;       //输入
    vsip_cmview_f *buff;     //中间变量，用来存储pc中每一列的数据
    vsip_cmview_f *buff_fft; //buff经过fft之后的数据
    vsip_fftm_f *buffPlan;   //buff fft运算中的参数
};
void submtd(void *param)
{
    //子任务
    struct mtdParam *p = (struct mtdParam *)param;
    p->buff = vsip_cmsubview_f(p->pc, 0, p->col, PulseNumber, 1);
    vsip_ccfftmop_f(p->buffPlan, p->buff, p->buff_fft);
    printf("%d\n",p->col);
    int j = 0;
    for (j = 0; j < PulseNumber; j++)
    {
        vsip_cmput_f(p->mtd, j, p->col, vsip_cmget_f(p->buff_fft, j, 0));
    }
    // printf("%d\n",col);
}
void movingTargetDetection1(vsip_cmview_f *mtd, vsip_cmview_f *pc, vsip_cmview_f *buff,
                            vsip_cmview_f *buff_fft, vsip_fftm_f *buffPlan, threadpool thpool)
{
    //线程池版本动目标检测
    struct mtdParam p[SampleNumber];

    int i = 0;
    for (i = 0; i < SampleNumber; i++)
    {
        p[i].mtd = vsip_cmcloneview_f(mtd);
        p[i].pc = vsip_cmcloneview_f(pc);
        p[i].buff = vsip_cmcloneview_f(buff);
        p[i].buff_fft = vsip_cmcloneview_f(buff_fft);
        p[i].buffPlan = buffPlan;
        p[i].col = i;
        //添加子任务
        thpool_add_work(thpool, submtd, (void *)&p[i]);
        //等待子任务结束
    }
    thpool_wait(thpool);
}