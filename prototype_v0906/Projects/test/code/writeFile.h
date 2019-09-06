#include <stdlib.h>
#include "fftw3.h"
#include <iostream>
#include "writeFile.cpp"
#define PulseNumber 8
#define SampleNumber 2700
void writeData(const char* fileName,fftw_complex **complex_array);