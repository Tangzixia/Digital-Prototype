#include "algocodeedit.h"
#include "linenumberarea.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QApplication>
#include <QFileDialog>
#include <QCloseEvent>
#include <QPushButton>
#include <QFontMetricsF>
#include <QPainter>
#include <QTextBlock>

AlgoCodeEdit::AlgoCodeEdit()
{
    //子窗口关闭时销毁这个类的对象
    setAttribute(Qt::WA_DeleteOnClose);
    //初始isUntitled为true
    isUntitled = true;

    lineNumberArea = new LineNumberArea(this);
    //每当按下回车或者删除回车(更新字符块),则newBlockCount计数,并触发该信号, newBlockCount 默认为1
    connect(this, &AlgoCodeEdit::blockCountChanged, this, &AlgoCodeEdit::updateLineNumberAreaWidth);
    connect(this, &AlgoCodeEdit::updateRequest, this, &AlgoCodeEdit::updateLineNumberArea);
    //光标改变时发出信号
    connect(this, &AlgoCodeEdit::cursorPositionChanged, this, &AlgoCodeEdit::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    //增加文本关键词高亮：char
    mshlighter = new MySyntaxHighlighter(this->document());
}

void AlgoCodeEdit::newFile()
{
    //设置文档编号，静态变量
    static int sequenceNumber = 1;
    //未被保存过
    isUntitled = true;
    //未命名文档+编号，编号累加
    curFile = tr("未命名文档%1.txt").arg(sequenceNumber++);
    //设置窗口标题，加×号
    setWindowTitle(curFile+"[*]"+tr("-文档编辑器"));
    //文档更改时发射contentsChanged信号，执行documentWasModified槽
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

}

bool AlgoCodeEdit::loadFile(const QString &fileName)
{
    //新建Q{File对象
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, tr("文档编辑器"),tr("无法读取文件%1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    return true;
}

bool AlgoCodeEdit::save()
{
    //如果wei被保存过，执行另存为
    if(isUntitled){
        return saveAs();
    }else{
        return saveFile(curFile);
    }
}

bool AlgoCodeEdit::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"), curFile);
    //获取文件路径，为空返回false，否则保存
    if(fileName.isEmpty()){
        return false;
    }else{
        return saveFile(fileName);
    }
}

bool AlgoCodeEdit::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, tr("多文档编辑器"), tr("无法写入文件 %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
    return true;
}

QString AlgoCodeEdit::userFriendlyCurrentFile()
{
    //从文见路径中提取文件名
    return QFileInfo(curFile).fileName();
}

void AlgoCodeEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
    }

}

int AlgoCodeEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    // 几位数字
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().width((QLatin1Char('9'))) * digits;

    return space;
}

void AlgoCodeEdit::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));

}

void AlgoCodeEdit::closeEvent(QCloseEvent *event)
{
    // 如果maybeSave()函数返回true，则关闭窗口
    if (maybeSave()) {
        event->accept();
    } else {   // 否则忽略该事件
        event->ignore();
    }
}

void AlgoCodeEdit::documentWasModified()
{
    //设置文档的属性isModified
    setWindowModified(document()->isModified());
}

void AlgoCodeEdit::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void AlgoCodeEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void AlgoCodeEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

bool AlgoCodeEdit::maybeSave()
{
    if (document()->isModified()) { // 如果文档被更改过
        QMessageBox box;
        box.setWindowTitle(tr("多文档编辑器"));
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
        return true; // 如果文档没有更改过，则直接返回true
}

void AlgoCodeEdit::setCurrentFile(const QString &fileName)
{
    //取出路径中的.或..
    curFile = QFileInfo(fileName).canonicalFilePath();
    //
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}
