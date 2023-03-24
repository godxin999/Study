#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include    <QLabel>
#include    <QProgressBar>
#include    <QSpinBox>
#include    <QFontComboBox>
#include    <QActionGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLabel *labelFile;
    QLabel *labelInfo;
    QProgressBar *progressbar1;
    QSpinBox *spinFontSize;
    QFontComboBox *comboFontName;
    QActionGroup *actionGroup;
    void buildUI();
    void buildSignalSlots();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void do_fontSize_changed(int FontSize);

    void do_fontSelected(const QFont &font);

    void on_actFont_Bold_triggered(bool checked);

    void on_actFont_Italic_triggered(bool checked);

    void on_actFont_Underline_triggered(bool checked);

    void on_actFile_Save_triggered();

    void on_actSys_ToggleText_triggered(bool checked);

    void on_actFile_New_triggered();

    void on_actFile_Open_triggered();

    void on_TextEdit_selectionChanged();

    void on_TextEdit_copyAvailable(bool b);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
