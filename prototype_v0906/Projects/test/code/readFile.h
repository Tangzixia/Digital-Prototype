#include <stdlib.h>
#include "fftw3.h"
#include "readFile.cpp"
#define PulseNumber 8
#define SampleNumber 2700

int dataLength(const char* filePath);
void readData(const char* filePath,fftw_complex* complex_array,int length);