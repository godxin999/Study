#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_btnGetTime_clicked()
{
    QDateTime curDateTime=QDateTime::currentDateTime();//获取当前日期和时间
    ui->timeEdit->setTime(curDateTime.time());//设置为当前的时间
    ui->editTime->setText(curDateTime.toString("hh:mm:ss"));//根据format打印时间的字符串
    ui->dateEdit->setDate(curDateTime.date());//设置当前日期
    ui->editDate->setText(curDateTime.toString("yyyy-MM-dd"));
    ui->dateTimeEdit->setDateTime(curDateTime);//设置日期和时间
    ui->editDateTime->setText(curDateTime.toString("yyyy-MM-dd hh:mm:ss"));
}


void Widget::on_btnSetTime_clicked()
{
    QString str=ui->editTime->text();//读取字符串所表示的时间
    str=str.trimmed();//去除可能存在的空格
    if(!str.isEmpty()){
        QTime tm=QTime::fromString(str,"hh:mm:ss");
        ui->timeEdit->setTime(tm);//根据字符串设置时间
    }
}


void Widget::on_btnSetDate_clicked()
{
    QString str=ui->editDate->text();
    str=str.trimmed();
    if(!str.isEmpty()){
        QDate dt=QDate::fromString(str,"yyyy-MM-dd");
        ui->dateEdit->setDate(dt);
    }
}


void Widget::on_btnSetDateTime_clicked()
{
    QString str=ui->editDateTime->text();
    str=str.trimmed();
    if(!str.isEmpty()){
        QDateTime datetime=QDateTime::fromString(str,"yyyy-MM-dd hh:mm:ss");
        ui->dateTimeEdit->setDateTime(datetime);
    }
}

void Widget::on_calendarWidget_selectionChanged(){
    QDate dt=ui->calendarWidget->selectedDate();//获取当前选择的日期
    QString str=dt.toString("yyyy年M月d日");//将时间转换为字符串
    ui->editCalendar->setText(str);//打印日期
}
