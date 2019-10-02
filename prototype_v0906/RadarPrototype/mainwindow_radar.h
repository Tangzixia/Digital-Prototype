#ifndef MAINWINDOW_RADAR_H
#define MAINWINDOW_RADAR_H
#include <QAbstractButton>
#include <QComboBox>
#include <QFontComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QToolBox>
#include <QToolButton>
#include <utils.h>
#include <QListWidget>
#include "diagramitem.h"
#include "radarscene.h"
#include "diagramtextitem.h"
/**
* @projectName   prototype_v0719
* @brief         编辑雷达功能的主窗口。
* @author        Antrn
* @date          2019-08-12
*/
namespace Ui {
class MainWindow_Radar;
}

class MainWindow_Radar : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_Radar(QWidget *parent = nullptr);
    explicit MainWindow_Radar(QString id, QListWidget *parent = nullptr);
//    explicit MainWindow_Radar( QString id, QWidget *parent = nullptr);
    ~MainWindow_Radar();
    DiagramItem *getDiagramItemById(int item_id);
    void deleteItemArrowById(int id);
    void deleteArrowById(int id);
    void autoloadXmlById(QString id);
    QFileInfoList getFileList(QString path);
    QString getEquip_id() const;
    void setEquip_id(const QString &value);
    void readXmlConf(QString xmlname);
    void save2XmlFile();
    //默认程序一打开就是已经保存好的状态
    static bool isSave;
    void toggleSaveXml(int flag);
    // 初始化5个算法组件
    void init5Comp();
    void loadCompByName(QString strText);
    void loadAllComps();

protected:
    void closeEvent(QCloseEvent *event);


private slots:
     void on_actionCom_list_triggered();
     void xy_show(double x,double y);

     void backgroundButtonGroupClicked(QAbstractButton *button);
     void buttonGroupClicked(int id);
     void deleteItem();
     void pointerGroupClicked(int id);
     void bringToFront();
     void sendToBack();
     void showItemProperties();
     void itemInserted(DiagramItem *item);
     void textInserted(QGraphicsTextItem *item);
     void currentFontChanged(const QFont &font);
     void fontSizeChanged(const QString &size);
     void sceneScaleChanged(const QString &scale);
     void textColorChanged();
     void itemColorChanged();
     void lineColorChanged();
     void textButtonTriggered();
     void fillButtonTriggered();
     void lineButtonTriggered();
     void handleFontChange();
     void itemSelected(QGraphicsItem *item);
     void on_actionOpenXml_triggered();

     void on_actionsave_triggered();

     void On_start();
     void On_over();
     void On_rateSignal(float rate);

     void On_isSave2False(QString message);

     // 更新右面的雷达组件的属性
     void update_Comp_property(QString name);

     void setComp_typeandMode(int id);
     void search();
     void editComplete();

private:
    QString equip_id;
    Ui::MainWindow_Radar *ui;
    QLabel *label_xy;
    QLineEdit *m_pSearchLineEdit;
    // 工具类
//    Utils *u;
    bool isSelectPath = false;
    QString userSelectPath;

    QStringList word_list;
    QStringListModel *string_list_model;

    RadarScene *scene;
//    QGraphicsView *view;

    QMenu *fileMenu;
    QMenu *itemMenu;

    QToolBar *textToolBar;
    QToolBar *editToolBar;
    QToolBar *colorToolBar;
    QToolBar *pointerToolbar;

    QComboBox *sceneScaleCombo;
    QComboBox *itemColorCombo;
    QComboBox *textColorCombo;
    QComboBox *fontSizeCombo;
    QFontComboBox *fontCombo;

    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *backgroundButtonGroup;
    QToolButton *fontColorToolButton;
    QToolButton *fillColorToolButton;
    QToolButton *lineColorToolButton;

    QLabel *label_time;
    QProgressBar *progressBar;

    void createCompBox();
    void createActions();
    void createMenus();
    void createToolbars();

    QWidget *createBackgroundCellWidget(const QString &text,
                                        const QString &image);
    QWidget *createCellWidget(const QString &text,
                              DiagramItem::DiagramType type);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &image, QColor color);
    QIcon createColorIcon(QColor color);
    void saveSnapshot(int flag);

    QAction *exitAction;
    QAction *addAction;
    QAction *deleteAction;
    QAction *propertyAction;

    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *aboutAction;

    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *textAction;
    QAction *fillAction;
    QAction *lineAction;
};

#endif // MAINWINDOW_RADAR_H
