#ifndef TDIALOGSIZE_H
#define TDIALOGSIZE_H

#include <QDialog>

namespace Ui {
    class TDialogSize;
    }

class TDialogSize : public QDialog
{
    Q_OBJECT

public:
    explicit TDialogSize(QWidget *parent = nullptr);
    ~TDialogSize();
    //因为对话框上的组件都是私有成员，只能通过接口函数访问，所以声明为公有的
    int rowCount();//获取对话框输入的行数
    int columnCount();//获取对话框输入的列数
    void setRowColumn(int row,int column);//设置对话框上两个SpinBox的值
private:
    Ui::TDialogSize *ui;
};

#endif // TDIALOGSIZE_H
