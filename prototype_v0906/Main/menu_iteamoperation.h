#ifndef MENU_ITEAMOPERATION_H
#define MENU_ITEAMOPERATION_H

#include <QMenu>

namespace Ui {
class Menu_iteamOperation;
}

class Menu_iteamOperation : public QMenu
{
    Q_OBJECT

public:
    explicit Menu_iteamOperation(QWidget *parent = nullptr);
    ~Menu_iteamOperation();
    enum OperateType{del,edit,property,rename};


signals:
    void itemOperate(OperateType);

private:
    Ui::Menu_iteamOperation *ui;
     void myAddAction();

};

#endif // MENU_ITEAMOPERATION_H
