#ifndef RADARPROPERTY_H
#define RADARPROPERTY_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class RadarProperty;
}

class RadarProperty : public QDialog
{
    Q_OBJECT

public:
    explicit RadarProperty(QWidget *parent = nullptr);
    ~RadarProperty();

private:
    Ui::RadarProperty *ui;
};

#endif // RADARPROPERTY_H
