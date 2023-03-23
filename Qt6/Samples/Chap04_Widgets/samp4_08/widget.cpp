#include "widget.h"
#include "ui_widget.h"
#include <QTime>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->groupBox->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);//设置尺寸属性
    ui->groupBox_2->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_timer=new QTimer(this);//新建定时器
    m_timer->stop();//停止定时器
    m_timer->setTimerType(Qt::CoarseTimer);//设置定时器精度
    ui->radioCoarse->setChecked(true);
    ui->radioOneShot->setChecked(true);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(do_timer_timeout()));//将超时信号关联到槽函数
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_btnStart_clicked()
{
    m_timer->setInterval(ui->spinBoxIntv->value());//设置时间周期
    if(ui->radioContinue->isChecked()){
        m_timer->setSingleShot(false);//连续计时
    }
    else{
        m_timer->setSingleShot(true);//单次计时
    }
    if(ui->radioPrecise->isChecked()){//设置精度
        m_timer->setTimerType(Qt::PreciseTimer);
    }
    else if(ui->radioCoarse->isChecked()){
        m_timer->setTimerType(Qt::CoarseTimer);
    }
    else{
        m_timer->setTimerType(Qt::VeryCoarseTimer);
    }
    m_timer->start();//启动定时器
    m_counter.start();//启动计时器
    ui->btnStart->setEnabled(false);//控制按钮属性
    ui->btnOneShot->setEnabled(false);
    ui->btnStop->setEnabled(true);
}


void Widget::on_btnStop_clicked()
{
    m_timer->stop();//暂停定时器
    int tmMsec=m_counter.elapsed();//获取过去的时间，单位为毫秒
    int ms=tmMsec%1000;
    int sec=tmMsec/1000;
    QString str=QString("流逝的时间：%1秒，%2毫秒").arg(sec).arg(ms,3,10,QChar('0'));//第二个参数为宽度，第三个参数为进制，第四个参数为填充的字符
    ui->labElapsedTime->setText(str);
    ui->btnStart->setEnabled(true);
    ui->btnOneShot->setEnabled(true);
    ui->btnStop->setEnabled(false);
}

void Widget::do_timer_timeout(){
    //QApplication::beep();
    QTime curTime=QTime::currentTime();
    ui->LCDHour->display(curTime.hour());//将当前时间显示在LCD上
    ui->LCDMin->display(curTime.minute());
    ui->LCDSec->display(curTime.second());
    if(m_timer->isSingleShot()){//如果时单次计时就显示流逝的时间
        int tmMsec=m_counter.elapsed();
        QString str=QString("流逝的时间：%1毫秒").arg(tmMsec);
        ui->labElapsedTime->setText(str);
        ui->btnStart->setEnabled(true);
        ui->btnOneShot->setEnabled(true);
        ui->btnStop->setEnabled(false);
    }
}

void Widget::on_btnOneShot_clicked()//动态单次定时的函数
{
    int intv=ui->spinBoxIntv->value();//设置周期
    QTimer::singleShot(intv,Qt::PreciseTimer,this,&Widget::do_timer_shot);//调用单次定时
    m_counter.start();//计时开始
    ui->btnOneShot->setEnabled(false);
}

void Widget::do_timer_shot(){
    //QApplication::beep();
    int tmMsec=m_counter.elapsed();//获取流逝的时间
    QString str=QString("流逝的时间：%1毫秒").arg(tmMsec);
    ui->labElapsedTime->setText(str);
    ui->btnOneShot->setEnabled(true);
}
