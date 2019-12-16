
#include <stdlib.h>
#include <vsip.h>
#include <string.h>
#include "constant.h"
void readData(const char *fileName,vsip_cmview_f *data,int index,int sign){
    FILE *fp;
    if(fp = fopen(fileName,"r")){
    }
    else{
        printf("can not open file!");
        return;
    }
    int i = 0,j = 0,count = 0,row = 0,col = 0;
    double real_part = 0.0,imaginary_part = 0.0;
    char *real_part_temp = (char*)malloc(sizeof(char)*100);
    char *imaginary_part_temp = (char*)malloc(sizeof(char)*100);
    while (i < vsip_cmgetrowlength_f(data)*vsip_cmgetcollength_f(data)+index)
    {
        if (feof(fp)){
            break;
        }
        fscanf(fp,"%s",real_part_temp);
        fscanf(fp,"%s",imaginary_part_temp);
        real_part = strtod(real_part_temp,NULL);
        imaginary_part = strtod(imaginary_part_temp,NULL);
        if (i >= index){
            vsip_cscalar_f temp;
            temp.r = real_part;
            temp.i = imaginary_part;
            vsip_cmput_f(data,row,col,temp);
            col ++;
            if(col > 0 && (col) % vsip_cmgetrowlength_f(data) == 0){
                row++;
                col = 0;
            }
        }
        if (sign == 1){
            fscanf(fp,"%s",real_part_temp);
        }    
        i++;
    }
}