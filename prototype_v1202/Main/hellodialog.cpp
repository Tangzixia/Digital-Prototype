#include "hellodialog.h"
#include "ui_hellodialog.h"
#include "mainwindownew.h"

#include <QFileDialog>
#include <QStyledItemDelegate>
#include <utils.h>
/**
* @projectName   prototype_v0719
* @brief         开始欢迎页面，也即是进入主程序之前的拦截界面。可以选择打开或者新建工程。
* @author        Antrn
* @date          2019-08-12
*/
HelloDialog::HelloDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelloDialog)
{
    ui->setupUi(this);
    // 写下一行这个在projectlist发出信号关闭的时候会出现错误
//    this->setAttribute(Qt::WA_DeleteOnClose);
    //应用样式 apply the qss style
    QFile file(":/qss/qss_hellodialog.qss");
    file.open(QIODevice::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
//    qDebug() << stylesheet;
    QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
    ui->comboBox->setItemDelegate(itemDelegate);
    ui->comboBox->setStyleSheet(stylesheet);
    file.close();

    ui->comboBox->setView(new QListView(this));
    //去掉页面边框和标题栏
    // 5.6.1
    this->setWindowFlags(Qt::FramelessWindowHint);
    // 5.9.8
    // this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowIcon(QIcon(":/img/HI.png"));
    //标题栏保留，去掉大小按钮
//    this->setWindowFlag(Qt::WindowMinimizeButtonHint, false);
//    this->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
//    去除所有按钮
//    this->setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);
    //只剩关闭按钮
//    this->setWindowFlag(Qt::WindowCloseButtonHint);

    QString styleSheet =
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
             "}";
    ui->pushButton_open->setStyleSheet(styleSheet);
    ui->pushButton_new->setStyleSheet(styleSheet);
    // 关闭按钮
    ui->pushButton->setStyleSheet(styleSheet);
    connect(ui->projectList, &ProjectList::closeHelloDialog, this, [=](){
        this->close();
    });
}

HelloDialog::~HelloDialog()
{
    delete ui;
}


/**
 * @brief 打开工程
 */
void HelloDialog::on_pushButton_open_clicked()
{
//    QProcess process;
//    process.startDetached(QStringLiteral("explorer.exe /select,\"E:\\Desktop\\mao.txt\""));

    // 用户选择dpsp的路径
    QString ppath = QFileDialog::getOpenFileName(nullptr, tr("打开工程.dpsp"), QDir::currentPath()+"/project/", "Project Files(*.dpsp)");
    if(!ppath.isEmpty()){
        // 项目名
        QString title = ppath.split("/").takeAt(ppath.split("/").length()-2);
//        qDebug() << "打开工程" << ppath.split("/").last();
        this->main = new MainWindow(title, ppath);
        // 如果之前未打开过，添加到文件中
        Utils::addProject2Pl(title, ppath);
        this->main->show();
        this->close();
    }else{
        // 用户没有选择，点击取消
    }
}

/**
 * 新建工程
 */
void HelloDialog::on_pushButton_new_clicked()
{
    MainWindowNew* mainnew = new MainWindowNew;
    mainnew->show();
    this->close();
}

/**
 * 关闭程序
 */
void HelloDialog::on_pushButton_clicked()
{
    qApp->closeAllWindows();
}
