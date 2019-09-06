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
    explicit CompProperty(QWidget *parent = nullptr);
    ~CompProperty();

private:
    Ui::CompProperty *ui;
};

#endif // COMPPROPERTY_H
