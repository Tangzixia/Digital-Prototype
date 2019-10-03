#ifndef ALGORITHMCOMP_H
#define ALGORITHMCOMP_H
#include <QMap>

class AlgorithmComp
{
public:
    AlgorithmComp();
    QMap<QString, QString> getInfo() const;
    void setInfo(const QMap<QString, QString> &value);

    QString getDesc() const;
    void setDesc(const QString &value);

    QMap<QString, QMap<QString, QString> > getParam() const;
    void setParam(const QMap<QString, QMap<QString, QString> > &value);

private:
    QMap<QString, QString> info;
    QMap<QString, QMap<QString, QString>> param;
    QString desc;

};

#endif // ALGORITHMCOMP_H
