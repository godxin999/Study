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

void Widget::on_btnDebugTime_clicked()
{
    QTime TM1(9,47,40);//初始化时间
    QString str=TM1.toString();
    qDebug("Original time= %s",str.toLocal8Bit().data());

    QTime TM2=TM1.addSecs(150);//延后150s
    str=TM2.toString();
    qDebug("150s later,time= %s",str.toLocal8Bit().data());

    TM2=QTime::currentTime();//获取当前时间
    str=TM2.toString("HH:mm:ss zzz");
    qDebug("Current time= %s",str.toLocal8Bit().data());
    qDebug("Hour= %d",TM2.hour());
    qDebug("Minute= %d",TM2.minute());
    qDebug("Second= %d",TM2.second());
    qDebug("MSecond= %d",TM2.msec());
}


void Widget::on_btnDebugDate_clicked()
{
    QDate DT1(2023,3,1);//初始化日期
    QString str=DT1.toString("yyyy-MM-dd");
    qDebug("DT1= %s",str.toLocal8Bit().data());

    QDate DT2;
    DT2.setDate(2023,3,15);//设置日期
    str=DT2.toString("yyyy-MM-dd");
    qDebug("DT2= %s",str.toLocal8Bit().data());
    qDebug("Days between DT2 and DT1= %lld",DT2.daysTo(DT1));

    DT2=QDate::currentDate();//获取当前日期
    str=DT2.toString("yyyy-MM-dd");
    qDebug("Current date= %s",str.toLocal8Bit().data());
    qDebug("Year= %d",DT2.year());
    qDebug("Month= %d",DT2.month());
    qDebug("Day= %d",DT2.day());
    qDebug("Day of week= %d",DT2.dayOfWeek());//获取今天是周几

}


void Widget::on_btnDebugDateTime_clicked()
{
    QDateTime DT1=QDateTime::currentDateTime();//获取当前日期时间
    QString str=DT1.toString();
    qDebug("DT1= %s",str.toLocal8Bit().data());

    QDate dt=DT1.date();
    str=dt.toString("yyyy-MM-dd");
    qDebug("DT1.date()= %s",str.toLocal8Bit().data());

    QTime tm=DT1.time();
    str=tm.toString("hh:mm:ss zzz");
    qDebug("DT1.time()= %s",str.toLocal8Bit().data());

    qint64 MS=DT1.toSecsSinceEpoch();//计算从Unix时间到现在的秒数
    qDebug("DT1.toSecsSinceEpoch()= %lld",MS);

    MS+=120;
    DT1.setSecsSinceEpoch(MS);//根据秒数设置时间
    str=DT1.toString("yyyy-MM-dd hh:mm:ss");
    qDebug("DT1+120s= %s",str.toLocal8Bit().data());
}

