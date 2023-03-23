#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
private:
    QTimer *m_timer;
    QElapsedTimer m_counter;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void do_timer_timeout();

    void do_timer_shot();

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_btnOneShot_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
