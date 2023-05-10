#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStandardItemModel>
#include <QItemSelectionModel>

#include "tcomboboxdelegate.h"
#include "tfloatspindelegate.h"
#include "tspinboxdelegate.h"

#define FixedColumnCount 6

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLabel *labCurFile;//当前文件
    QLabel *labCellPos;//当前单元格行列号
    QLabel *labCellText;//当前单元格内容
    QStandardItemModel *m_model;//数据模型
    QItemSelectionModel *m_selection;//选择模型
    void iniModelData(QStringList &aFileContent);//初始化模型内容的函数
    TSpinBoxDelegate *intSpinDelegata;//用于编辑整数
    TFloatSpinDelegate *floatSpinDelegate;//用于编辑浮点数
    TComboBoxDelegate *comboDelegate;//用于列表选择
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void do_currentChanged(const QModelIndex &current,const QModelIndex &previous);
    void on_actionOpen_triggered();

    void on_actionAppend_triggered();

    void on_actionDelete_triggered();

    void on_actionAlignLedft_triggered();

    void on_actionAlignCenter_triggered();

    void on_actionAlignRight_triggered();


    void on_actionFontBold_triggered(bool checked);

    void on_actionModelData_triggered();

    void on_actionInsert_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
