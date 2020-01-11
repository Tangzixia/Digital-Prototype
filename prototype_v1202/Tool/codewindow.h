#ifndef CODEWINDOW_H
#define CODEWINDOW_H

#include <QGraphicsItem>
#include <QListWidgetItem>
#include <QMainWindow>

namespace Ui {
class CodeWindow;
}

class CodeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CodeWindow(QWidget *parent = nullptr, QListWidgetItem *item=nullptr, QGraphicsItem *gitem=nullptr);
    bool eventFilter(QObject *,QEvent *event)override;
    void newFile(const QString &fileName);
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();

    QString getCurFile() const;
    void setCurFile(const QString &value);
    bool getIsUntitled() const;
    void setIsUntitled(bool value);

public slots:
    void documentWasModified(); //文档被更该市，窗口显示更改状态标志
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionRedo_triggered();

    void on_actionUndo_triggered();

    void on_actionFull_triggered();

private:
    Ui::CodeWindow *ui;
    bool maybeSave(); //是否需要保存
    QString curFile; //当前文件路径
    bool isUntitled; //当前文件是否未被保存
};

#endif // CODEWINDOW_H
