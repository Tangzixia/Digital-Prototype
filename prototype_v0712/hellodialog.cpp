#include "hellodialog.h"
#include "ui_hellodialog.h"

HelloDialog::HelloDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelloDialog)
{
    ui->setupUi(this);
    //去掉窗口头部
    this->setWindowFlag(Qt::FramelessWindowHint);
//    this->setWindowFlag(Qt::WindowMinimizeButtonHint);
    ui->pushButton_open->setStyleSheet(
                //正常状态样式
                "QPushButton{"
                     "background-color:rgba(211, 215, 207, 30);"//背景色（也可以设置图片）
                     "border-style:outset;"                  //边框样式（inset/outset）
                     "border-width:1px;"                     //边框宽度像素
                     "border-radius:5px;"                   //边框圆角半径像素
                     "border-color:rgba(0,0,0,30);"    //边框颜色
                 "}"
                 //鼠标按下样式

                 "QPushButton:pressed{"
                     "background-color:rgba(100,255,100,200);"
                     "border-color:rgba(255,255,255,200);"
                     "border-style:inset;"
                 "}"

                 //鼠标悬停样式
                 "QPushButton:hover{"
                 "background-color:rgba(255,255,255,100);"
                 "border-color:rgba(30,30,30,80);"
                 "}"
     );
    ui->pushButton_new->setStyleSheet(
                //正常状态样式
                "QPushButton{"
                     "background-color:rgba(211, 215, 207, 30);"//背景色（也可以设置图片）
                     "border-style:outset;"                  //边框样式（inset/outset）
                     "border-width:1px;"                     //边框宽度像素
                     "border-radius:5px;"                   //边框圆角半径像素
                     "border-color:rgba(0,0,0,30);"    //边框颜色
                 "}"
                 //鼠标按下样式
                 "QPushButton:pressed{"
                     "background-color:rgba(100,255,100,200);"
                     "border-color:rgba(255,255,255,200);"
                     "border-style:inset;"
                 "}"

                 //鼠标悬停样式
                 "QPushButton:hover{"
                 "background-color:rgba(255,255,255,80);"
                 "border-color:rgba(30,30,30,100);"
                 "}"
     );

}

HelloDialog::~HelloDialog()
{
    delete ui;
}


void HelloDialog::on_pushButton_open_clicked()
{
    this->main = new MainWindow();
    this->main->show();
    this->close();
}

void HelloDialog::on_pushButton_new_clicked()
{

}
