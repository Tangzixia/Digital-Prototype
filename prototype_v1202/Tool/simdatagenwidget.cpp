#include "simdatagenwidget.h"
#include "ui_simdatagenwidget.h"
#include "SimDataGen.cpp"
#include <QDate>
#include <QDir>
#include <QException>
#include <QFileDialog>
#include <QMessageBox>

SimDataGenWidget::SimDataGenWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimDataGenWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowTitleHint|Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
}

SimDataGenWidget::~SimDataGenWidget()
{
    delete ui;
}

#if 0
/**
 * @brief 选择文件
 */
void SimDataGenWidget::on_pushButton_select_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("选择文件夹"),
                                                    QDir::currentPath(),
                                                    tr("text (*.txt)"));
    ui->lineEdit_path->setText(path);
    ui->pushButton_select->setText("OK");
}

#endif
/**
 * @brief OK
 */
void SimDataGenWidget::on_buttonBox_accepted()
{
    close();
}

/**
 * @brief 加载目标模型按钮
 */
void SimDataGenWidget::on_pushButton_load_clicked()
{
    //信号参数-数据结构
    SIMRE_LIB_PARAM params;
    // 赋值
    params.nPRT =  ui->lineEdit_period->text().toInt();			//雷达重复周期为脉冲重复频率(4K)的倒数 单位us
    params.nPW = ui->lineEdit_width->text().toInt();			//脉宽10us 单位us
    params.nPulse = ui->lineEdit_child->text().toInt();			//一组脉冲中的子脉冲个数：16, 单位个
    params.nBand = ui->lineEdit_bandwidth->text().toInt();			//调频带宽：50M 单位MHz
    params.nDeltaf = ui->lineEdit_step->text().toInt();		//调频带宽步进频率：暂取10 单位MHz
    params.nSampling = ui->lineEdit_datasamp->text().toInt();		//采样率：60MHz 单位MHz

    params.nGroup = ui->lineEdit_group->text().toInt();			//脉冲组数 单位个
    params.nRF = ui->lineEdit_freq->text().toInt();			//雷达载频即发射频率：10GHz 单位GHz
    params.amplitude = ui->lineEdit_amplitude->text().toInt();		//目标回波幅度： 单位m
    params.range = ui->lineEdit_posi->text().toInt();		//目标径向位置 单位km
    params.velcity = ui->lineEdit_speed->text().toInt();		//目标径向速度	 单位m
    params.omega = ui->lineEdit_angular->text().toInt();			//目标旋转角 单位度
    params.acc = ui->lineEdit_acceler->text().toInt();				//目标径向加速度 单位m

    SimDataGen sdg(params);

    // 读取模型

    // 输出数据文件
    QString path = "./echo/res"+QTime::currentTime().toString()+".txt";
    path= ui->lineEdit_path->text();
    QFile file(path);
    if(!file.open(QFile::ReadOnly)){
        QMessageBox::warning(this, "文件名设置出错！", "请检查文件名的格式是否正确！");
        return;
    }
    file.close();
    sdg.m_echo = const_cast<char *>(ui->lineEdit_path->text().toStdString().c_str());

    sdg.GenData();
    ui->pushButton_load->setText("数据生成完成");
}
