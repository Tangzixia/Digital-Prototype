#include "a_show.h"
#include "ui_a_show.h"

#include <QPainter>

A_show::A_show(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::A_show)
{
    ui->setupUi(this);
}

A_show::~A_show()
{
    delete ui;
}

void A_show::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int min = int(qMin(width(), height())-80);
    QPoint center(int(width() / 2), int(height() / 2));

    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(QColor(255, 255, 255, 255),1));

    painter.setBrush(Qt::black);
    painter.drawRect(0,0,850,300);

    painter.setBrush(Qt::white);
//    for(int i = 0; i < 855; i += 15)
//        painter.drawLine(i + 0.5, 0, i + 0.5, 300);
//    for(int j = 0; j < 300; j += 15)
//        painter.drawLine(0, j + 0.5, 850, j + 0.5);

    painter.drawLine(0, 150, 855, 150);
}


