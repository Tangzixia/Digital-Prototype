#ifndef QUDONG_H
#define QUDONG_H
#include <QString>

using namespace std;

class QuDong
{
public:
    QuDong();
    static void startRun(QString fileName, QString outName);
    static void startRunCpp(QString fileName, QString outName, QString radar_name);
};

#endif // QUDONG_H
