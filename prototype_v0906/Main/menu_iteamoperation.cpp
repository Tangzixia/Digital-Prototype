#include "menu_iteamoperation.h"
#include "ui_menu_iteamoperation.h"
#include <QDebug>
Menu_iteamOperation::Menu_iteamOperation(QWidget *parent) :
    QMenu(parent),
    ui(new Ui::Menu_iteamOperation)
{
    ui->setupUi(this);
    myAddAction();
}

Menu_iteamOperation::~Menu_iteamOperation()
{
    delete ui;
}

void Menu_iteamOperation::myAddAction(){

    QAction *del_action =new QAction("删除");
    this->addAction(del_action);
    connect(del_action , &QAction::triggered,[=](){
          itemOperate(del);
    });
    QAction *property_action =new QAction("属性");
    this->addAction(property_action);
    connect(property_action , &QAction::triggered,[=](){
          itemOperate(property);
    });
    QAction *edit_action =new QAction("编辑");
    this->addAction(edit_action);
    connect(edit_action , &QAction::triggered,[=](){
          itemOperate(edit);
    });
}

