#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    UIPropertySetup();
    connect(ui->spinNum,SIGNAL(valueChanged(int)),this,SLOT(on_btnCal_clicked()));
    connect(ui->spinPrice,SIGNAL(valueChanged(double)),this,SLOT(on_btnCal_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::UIPropertySetup(){
    ui->spinPrice->setDecimals(2);//显式两位小数
    ui->spinTotal->setDecimals(2);

    ui->spinDec->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);//自适应步长
    ui->spinDec->setRange(0,65535);//设置范围
    ui->spinDec->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);//水平扩展

    ui->spinBin->setRange(0,65535);
    ui->spinBin->setDisplayIntegerBase(2);//二进制

    ui->spinHex->setRange(0,65535);
    ui->spinHex->setDisplayIntegerBase(16);//十六进制
}



void Widget::on_btnCal_clicked()
{
    int num=ui->spinNum->value();
    float price=ui->spinPrice->value();
    float total=num*price;
    ui->spinTotal->setValue(total);
}


void Widget::on_spinDec_valueChanged(int arg1)
{
    qDebug(ui->spinDec->cleanText().toLocal8Bit().data());
    ui->spinBin->setValue(arg1);//设置值，会根据设定好的进制属性进行显示
    ui->spinHex->setValue(arg1);
}


void Widget::on_spinBin_valueChanged(int arg1)
{
    qDebug(ui->spinDec->cleanText().toLocal8Bit().data());
    ui->spinDec->setValue(arg1);
    ui->spinHex->setValue(arg1);
}


void Widget::on_spinHex_valueChanged(int arg1)
{
    qDebug(ui->spinDec->cleanText().toLocal8Bit().data());
    ui->spinBin->setValue(arg1);
    ui->spinDec->setValue(arg1);
}




