#include "thpool.c"
#include <sys/time.h>
#include <time.h>
struct param
{
    int i;
};
void subfunc(void *arg)
{
    struct param *p = (struct param *)arg;
    printf("%d\n",p->i);
}
int main()
{
    threadpool tp = thpool_init(16);
    int i = 0;
    struct timeval t1, t2, t3;
    struct param a[100];
    gettimeofday(&t1, NULL);
    for (i = 0; i < 100; i++)
    {
        a[i].i = i;
        thpool_add_work(tp, subfunc, (void *)&a[i]);
        // printf("%d\n", tp->jobqueue.len);
        
    }
    thpool_wait(tp);
    
    gettimeofday(&t2, NULL);
    // for (i = 0; i < 100000000; i++)
    // {
    //     count2++;
    // }
    // gettimeofday(&t3, NULL);
    printf("speed time is:%f(ms)\n", (double)(t2.tv_sec * 1e6 + t2.tv_usec) / 1000.0 - (double)(t1.tv_sec * 1e6 + t1.tv_usec) / 1000.0);
    // printf("speed time is:%f(ms)\n", (double)(t3.tv_sec * 1e6 + t3.tv_usec) / 1000.0 - (double)(t2.tv_sec * 1e6 + t2.tv_usec) / 1000.0);
    // printf("%lld,%lld\n", count1, count2);
    thpool_destroy(tp);
}