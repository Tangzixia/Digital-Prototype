#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>
#include "thpool.c"
#include "vsip.h"
#include "constant.h"
struct cfarparam
{
    int row;
    vsip_cmview_f *cfar_result; //结果
    vsip_cmview_f *noise_gate;  //噪音门限
    vsip_cmview_f *mtd;         //输入
    int protect_number;         //保护门限值
    int reference_number;       //读取的长度
    int alpha;                  //权值
    vsip_cscalar_f left;        //中间变量
    vsip_cscalar_f right;       //中间变量
    vsip_cscalar_f temp;        //中间变量
};
void subcfar(void *param)
{
    //子任务

    struct cfarparam *parameter = (struct cfarparam *)param;
    int j = 0;
    for (j = 100; j < SampleNumber - 100; j++)
    {
        parameter->left = vsip_cmmeanval_f(vsip_cmsubview_f(parameter->mtd, parameter->row,
                                                            j - parameter->protect_number - parameter->reference_number, 1, parameter->reference_number));
        parameter->right = vsip_cmmeanval_f(vsip_cmsubview_f(parameter->mtd, parameter->row,
                                                             j + parameter->protect_number, 1, parameter->reference_number));
        parameter->temp = vsip_cadd_f(parameter->left, parameter->right);
        parameter->temp = vsip_rcmul_f(parameter->alpha, parameter->temp);
        vsip_cmput_f(parameter->noise_gate, parameter->row, j, parameter->temp);
        if (vsip_cmget_f(parameter->mtd, parameter->row, j).r > vsip_cmget_f(parameter->noise_gate, parameter->row, j).r)
        {
            vsip_cmput_f(parameter->cfar_result, parameter->row, j, parameter->temp);
        }
    }
}
void constantFalseAlarmRate(vsip_cmview_f *cfar_result, vsip_cmview_f *noise_gate, vsip_cmview_f *mtd,
                            int protect_number, int reference_number, int alpha, vsip_cscalar_f left,
                            vsip_cscalar_f right, vsip_cscalar_f temp)
{
    int i, j;
    
    for (i = 0; i < PulseNumber; i++)
    {
        // #pragma omp parallel for
        for (j = 100; j < SampleNumber - 100; j++)
        {
            left = vsip_cmmeanval_f(vsip_cmsubview_f(mtd, i, j - protect_number - reference_number, 1, reference_number));
            right = vsip_cmmeanval_f(vsip_cmsubview_f(mtd, i, j + protect_number, 1, reference_number));
            temp = vsip_cadd_f(left, right);
            temp = vsip_rcmul_f(alpha, temp);
            vsip_cmput_f(noise_gate, i, j, temp);
            if (vsip_cmget_f(mtd, i, j).r > vsip_cmget_f(noise_gate, i, j).r)
            {
                vsip_cmput_f(cfar_result, i, j, temp);
            }
        }
    }
}
void constantFalseAlarmRate1(vsip_cmview_f *cfar_result, vsip_cmview_f *noise_gate, vsip_cmview_f *mtd,
                             int protect_number, int reference_number, int alpha, vsip_cscalar_f left,
                             vsip_cscalar_f right, vsip_cscalar_f temp, threadpool thpool)
{
    //线程池版本的恒虚警率
    struct cfarparam param[PulseNumber];
    int i;
    for (i = 0; i < PulseNumber; i++)
    { //添加子任务
        param[i].cfar_result = cfar_result;
        param[i].noise_gate = noise_gate;
        param[i].mtd = mtd;
        param[i].protect_number = protect_number;
        param[i].reference_number = reference_number;
        param[i].alpha = alpha;
        param[i].left = param[i].right = param[i].temp = vsip_cmplx_f(0., 0.);
        param[i].row = i;
        thpool_add_work(thpool, subcfar, (void *)&param[i]);
    }
    thpool_wait(thpool); //等待任务队列中的子任务结束
}
