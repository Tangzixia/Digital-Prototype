#include "qudong.h"
#include <iostream>
QuDong::QuDong()
{

}

void QuDong::startRun(string fileName, string outName)
{
    string a = "g++ ";
    string b = " -fopenmp -lfftw3 -lm -o ";
    string c = " && ";
    string command = a+fileName+b+outName+c+outName;
    cout << command << endl;
    system(command.c_str());
}
