#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    enum CellType{ctName=1000,ctSex,ctBirth,ctNation,ctPartyM,ctScore};//单元格的类型，在创建单元格时用于设置type
    enum FieldColNum{colName=0,colSex,colBirth,colNation,colScore,colPartyM};//各字段在表中的列号
    QLabel *labCellIndex;//在状态栏上显示单元格的行号和列号
    QLabel *labCellType;//在状态栏上显示单元格的type
    QLabel *labStudID;//在状态栏上显示单元格的data
    void createItemsARow(int rowNo,QString name,QString sex,QDate birth,QString nation,bool isPM,int score);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSetHeader_clicked();

    void on_btnIniData_clicked();

    void on_tableInfo_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_btnInsertRow_clicked();

    void on_btnAppendRow_clicked();

    void on_btnDelCurRow_clicked();

    void on_chkBoxTabEditable_clicked(bool checked);

    void on_chkBoxHeaderH_clicked(bool checked);

    void on_chkBoxHeaderV_clicked(bool checked);

    void on_chkBoxRowColor_clicked(bool checked);

    void on_rBtnSelectRow_clicked();

    void on_rBtnSelectItem_clicked();

    void on_btnReadtoEdit_clicked();

    void on_btnAutoHeight_clicked();

    void on_btnAutoWidth_clicked();

    void on_btnSetRows_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
