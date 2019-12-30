#include "codewindow.h"
#include "ui_codewindow.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QPushButton>
#include <QListWidgetItem>
#include <QMessageBox>


// This is an auto-generated comment.
/**
* @projectName   prototype_v1202
* @brief         代码编辑窗口，包括高亮、行号、注释，支持键盘组合键Ctrl+S,Ctrl+C,Ctrl+V,Ctrl+Z...
* @author        Antrn
* @date          2019-12-30
*/

/**
 * @brief CodeWindow::CodeWindow 构造函数，初始化ui，安装事件过滤器
 * @param parent 父类
 * @param item 传入的左边的算法组件列表的对象
 */
CodeWindow::CodeWindow(QWidget *parent, QListWidgetItem *item) :
    QMainWindow(parent),
    ui(new Ui::CodeWindow)
{
    ui->setupUi(this);
    ui->plainTextEdit->installEventFilter(this);
    // 设置tab距离
    ui->plainTextEdit->setTabStopWidth(40);
    ui->plainTextEdit->setMinimumSize(800,600);
    //初始isUntitled为true，未被重命名
    isUntitled = true;
    if( item == nullptr ){
        qDebug() << "你不应该看见这几个字，表示item为null";
        return;
    }
    else{
        QString na = item->text();
        QString id = item->data(Qt::UserRole+2).toString();
        qDebug() << "编辑组件代码: " << na;
        QString code_path_name = QDir::currentPath()+"/codes/"+ na +".cpp";
        bool tof = loadFile(code_path_name);
        if(!tof){
            // 表示读取代码文件失败，需要新建文件
            newFile(code_path_name);
        }
        ui->plainTextEdit->showNormal();
//        child->move((QApplication::desktop()->width() - child->width())/2,(QApplication::desktop()->height() - child->height())/2);
    }
}

/**
 * @brief CodeWindow::eventFilter
 * @param widget 指明操作在哪个控件
 * @param event 捕捉到的事件，有事件存在，先在这里捕获进行处理
 * @return bool类型，自己处理完成与否
 */
bool CodeWindow::eventFilter(QObject *widget, QEvent *event)
{
    if(widget == ui->plainTextEdit){
        if (event->type() == QEvent::KeyPress)
        {
             QKeyEvent *ev = static_cast<QKeyEvent*>(event);
             // Ctrl+S保存操作
             if (ev->key() == Qt::Key_S && (ev->modifiers() & Qt::ControlModifier))
             {
                 if(ui->plainTextEdit->document()->isModified()){
                     save();
                     // return 说明处理完毕，false就是得继续下一步进行处理
                     return true;
                 }
             }
        }
    }
    return false;
}

/**
 * 新建代码文件
 * @brief AlgoCodeEdit::newFile
 */
void CodeWindow::newFile(const QString &fileName)
{
    qDebug() << "新建代码文件" << fileName;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.close();
    // 新建文件是空的，当然也是相当于block内容是没有修改的
    setCurFile(fileName);
}
/**
 * @brief CodeWindow::documentWasModified 槽函数，检测文档对象是否被修改
 */
void CodeWindow::documentWasModified()
{
    //设置文档的属性isModified
    setWindowModified(ui->plainTextEdit->document()->isModified());
}

/**
 * @brief CodeWindow::loadFile 加载代码文件
 * @param fileName 文件路径
 * @return 布尔类型，代表是否成功
 */
bool CodeWindow::loadFile(const QString &fileName)
{
    //新建QFile对象
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        // 在主窗口中间显示
//        int x =this->x() + (this->width() - aboutdialog.width()) / 2;
//        int y =this->y() + (this->height() - aboutdialog.height()) / 2;
//        QMessageBox::warning(this, tr("代码编辑器"),tr("无法读取文件%1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->plainTextEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    file.close();
    // 加载完文件之后，文件内容是认为未修改
    setCurFile(fileName);
    connect(ui->plainTextEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    return true;
}

// 保存
bool CodeWindow::save()
{
    //如果未被保存过，执行另存为
//    if(getIsUntitled()){
//        return saveAs();
//    }else{
        return saveFile(getCurFile());
//    }
}

/**
 * 截取文件名出来
 * @brief CodeWindow::userFriendlyCurrentFile
 * @return
 */
QString CodeWindow::userFriendlyCurrentFile()
{
    //从文见路径中提取文件名
    return QFileInfo(curFile).fileName();
}

QString CodeWindow::getCurFile() const
{
    return curFile;
}

bool CodeWindow::getIsUntitled() const
{
    return isUntitled;
}

void CodeWindow::setIsUntitled(bool value)
{
    isUntitled = value;
}

/**
 * 自定义路径另存为
 * @brief CodeWindow::saveAs
 * @return
 */
bool CodeWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"), getCurFile());
    //获取文件路径，为空返回false，否则保存
    if(fileName.isEmpty()){
        qDebug() << "另存为路径为空！";
        return false;
    }else{
        return saveFile(fileName);
    }
}

/**
 * @brief CodeWindow::saveFile
 * @param fileName 绝对路径+文件名
 * @return 是否保存成功
 */
bool CodeWindow::saveFile(const QString &fileName)
{
    // 新建文件对象
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, tr("多代码编辑器"), tr("无法写入文件 %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->plainTextEdit->toPlainText();
    QApplication::restoreOverrideCursor();
    // 文件内容已经保存
    setCurFile(fileName);
    file.close();
    return true;
}

/**
 * 设置当前的文件名，设置已经保存
 * @brief CodeWindow::setCurFile
 * @param fileName
 */
void CodeWindow::setCurFile(const QString &fileName)
{
    //取出路径中的.或..
    curFile = QFileInfo(fileName).canonicalFilePath();
//    isUntitled = false;
    ui->plainTextEdit->document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

/**
 * 弹窗提醒未保存
 * @brief CodeWindow::maybeSave
 * @return
 */
bool CodeWindow::maybeSave()
{
    // 先检查是否经过修改
    if (ui->plainTextEdit->document()->isModified()) { // 如果代码被更改过
        QMessageBox box;
        box.setWindowTitle(tr("代码编辑器"));
        box.setText(tr("是否保存对“%1”的更改？")
                        .arg(userFriendlyCurrentFile()));
        box.setIcon(QMessageBox::Warning);

        // 添加按钮，QMessageBox::YesRole可以表明这个按钮的行为
        QPushButton *yesBtn = box.addButton(tr("是(&Y)"),QMessageBox::YesRole);
        box.addButton(tr("否(&N)"),QMessageBox::NoRole);
        QPushButton *cancelBtn = box.addButton(tr("取消"), QMessageBox::RejectRole);
        box.exec(); // 弹出对话框，让用户选择是否保存修改，或者取消关闭操作
        if (box.clickedButton() == yesBtn)  // 如果用户选择是，则返回保存操作的结果
            return save();
        else if (box.clickedButton() == cancelBtn) // 如果选择取消，则返回false
            return false;
        }
        return true; // 如果代码没有更改过，则直接返回true
}

/**
 * 关闭窗口事件
 * @brief CodeWindow::closeEvent
 * @param event
 */
void CodeWindow::closeEvent(QCloseEvent *event)
{
    // 关闭前检查，如果返回true，则关闭窗口
    if (maybeSave()) {
        event->accept();
    } else {   // 否则忽略该事件
        event->ignore();
    }
}

// 保存按钮
void CodeWindow::on_actionSave_triggered()
{
    save();
}

void CodeWindow::on_actionSaveAs_triggered()
{
    saveAs();
}

void CodeWindow::on_actionRedo_triggered()
{
    ui->plainTextEdit->redo();
}

void CodeWindow::on_actionUndo_triggered()
{
    ui->plainTextEdit->undo();
}

void CodeWindow::on_actionFull_triggered()
{
    this->showMaximized();
}
