#include <string.h>
void writeData(const char*fileName,vsip_cmview_f *data){
    FILE *fp;
    if ((fp = fopen(fileName,"w"))){
    }
    else{
        printf("can not open file");
    }
    int row = vsip_cmgetcollength_f(data);//16
    int col = vsip_cmgetrowlength_f(data);//600
    int i,j;
    for ( i = 0; i < row; i++)
    {
        for (j = 0;j < col;j++){
            char* temp_r = (char*)malloc(sizeof(char)*100);
            char* temp_i = (char*)malloc(sizeof(char)*100);
            gcvt(vsip_cmget_f(data,i,j).r,10,temp_r);
            gcvt(vsip_cmget_f(data,i,j).i,10,temp_i);
            fputs(temp_r,fp);
            fputs("\t",fp);
            fputs(temp_i,fp);
            fputs("i\t\n",fp);
            //if ((j+1) % 5 == 0)
            //{
            //    fputs("\n",fp);
            //}
            free(temp_i);
            free(temp_r);
        }
    }
    printf("write success!\n");
}/*
int main(){
    vsip_cmview_f *data = vsip_cmcreate_f(PulseNumber,SampleNumber,VSIP_ROW,VSIP_MEM_CONST);
    readData("../../test/Echoes.txt",data,0,0);
    int i,j;
    for (i = 0; i < PulseNumber; i++){
        for (j = 0; j < SampleNumber; j++)
        {
            printf("%.3f+%.3fi ",vsip_cmget_f(data,i,j).r,vsip_cmget_f(data,i,j).i); 
        }
        printf("%d,%d\n",i,j);
    }
    printf("%ld,%ld",vsip_cmgetcollength_f(data),vsip_cmgetrowlength_f(data));
    writeData("a.txt",data);
    
}*/
