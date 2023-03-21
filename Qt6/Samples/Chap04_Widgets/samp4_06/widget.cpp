#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //当三种组件的值改变时，发射改变进度条的信号
    connect(ui->slider,SIGNAL(valueChanged(int)),this,SLOT(do_valueChanged(int)));
    connect(ui->scrollBar,SIGNAL(valueChanged(int)),this,SLOT(do_valueChanged(int)));
    connect(ui->dial,SIGNAL(valueChanged(int)),this,SLOT(do_valueChanged(int)));
}

Widget::~Widget()
{
    delete ui;
}
void Widget::on_chkBox_Visable_clicked(bool checked)//进度条的进度(百分比或值)是否显示
{
    ui->progressBar->setTextVisible(checked);
}


void Widget::on_chkBox_Inverted_clicked(bool checked)//进度条切换起始位置(左/右)
{
    ui->progressBar->setInvertedAppearance(checked);
}


void Widget::on_radio_percent_clicked()
{
    ui->progressBar->setFormat("%p");//以百分比打印
}


void Widget::on_radio_Value_clicked()
{
    ui->progressBar->setFormat("%v");//以值打印
}

void Widget::do_valueChanged(int value){//自定义槽函数
    ui->progressBar->setValue(value);
}
