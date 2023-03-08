#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "tperson.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
private:
    Tperson *boy;
    Tperson *girl;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void do_ageChanged(int value);
    void do_spinChanged(int arg1);

    void on_btnClear_clicked();
    void on_btnBoyInc_clicked();
    void on_btnGirlInc_clicked();
    void on_btnClassInfo_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
