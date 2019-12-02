#ifndef COMPPROPERTY_H
#define COMPPROPERTY_H

#include <QDialog>

namespace Ui {
class CompProperty;
}

class CompProperty : public QDialog
{
    Q_OBJECT

public:
    explicit CompProperty(QString name, QWidget *parent = nullptr);
    ~CompProperty();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CompProperty *ui;
    QString path_;
};

#endif // COMPPROPERTY_H
