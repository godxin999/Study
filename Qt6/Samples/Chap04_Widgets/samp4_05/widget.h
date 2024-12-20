#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnAlign_Left_clicked();

    void on_btnAlign_Center_clicked();

    void on_btnAlign_Right_clicked();

    void on_btnBold_clicked(bool checked);

    void on_btnItalic_clicked(bool checked);

    void on_btnUnderline_clicked(bool checked);

    void on_chkReadonly_clicked(bool checked);

    void on_chkEnabled_clicked(bool checked);

    void on_chkClearButtonEnabled_clicked(bool checked);

    void on_radioBlack_clicked();

    void on_radioRed_clicked();

    void on_radioBlue_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
