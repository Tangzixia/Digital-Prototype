#ifndef ALGOCODEEDIT_H
#define ALGOCODEEDIT_H

#include "mysyntaxhighlighter.h"

#include <QObject>
#include <QPlainTextEdit>
#include <QTextEdit>

class AlgoCodeEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    AlgoCodeEdit();
    void newFile();
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile(){
        return curFile;
    }
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();


protected:
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void documentWasModified(); //文档被更该市，窗口显示更改状态标志
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    bool maybeSave(); //是否需要保存
    void setCurrentFile(const QString &fileName);
    QString curFile; //当前文件路径
    bool isUntitled; //当前文件是否未被保存

    QWidget *lineNumberArea;
    MySyntaxHighlighter *mshlighter;
};

#endif // ALGOCODEEDIT_H
