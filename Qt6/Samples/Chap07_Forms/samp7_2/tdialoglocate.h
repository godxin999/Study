#ifndef TDIALOGLOCATE_H
#define TDIALOGLOCATE_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
    class TDialogLocate;
    }

class TDialogLocate : public QDialog
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent *event);//对话框关闭事件处理函数
    void showEvent(QShowEvent *event);//对话框显示事件处理函数
public:
    explicit TDialogLocate(QWidget *parent = nullptr);
    ~TDialogLocate();
    void setSpinRange(int rowCount,int colCount);//设置最大值用于初始化
public slots:
    void setSpinValue(int rowNo,int colNo);//与主窗口的cellIndexChanged()信号关联
signals:
    void changeCellText(int row,int column,QString &text);//对话框发射的信号
    void changeActionEnable(bool en);//对话框发射的信号
private slots:
    void on_btnSetText_clicked();

private:
    Ui::TDialogLocate *ui;
};

#endif // TDIALOGLOCATE_H
