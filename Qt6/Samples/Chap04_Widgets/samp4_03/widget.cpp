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


void Widget::on_btnCal_clicked()
{
    int num=ui->editNum->text().toInt();//获取字符串并转换为对应类型
    float price=ui->editPrice->text().toFloat();
    float total=num*price;
    QString str;
    str=str.setNum(total,'f',2);//setNum()函数把整型或浮点型转换为字符串，第二个参数是格式字符，这里表示使用自然计数法，第三个位精度位数
    ui->editTotal->setText(str);
}

void Widget::on_btnDec_clicked()
{
    int val=ui->editDec->text().toInt();
    QString str=QString::number(val,16);//使用静态函数number()将val转换位16进制字符串
    str=str.toUpper();//转换为大写
    ui->editHex->setText(str);
    str=QString::number(val,2);//转换为2进制字符串
    ui->editBin->setText(str);
}


void Widget::on_btnBin_clicked()
{
    bool ok;
    int val=ui->editBin->text().toInt(&ok,2);//第一个参数ok为判断转换是否成功的变量，第二个参数表示读取二进制字符串
    QString str=QString::number(val,10);
    ui->editDec->setText(str);
    str=QString::number(val,16);
    str=str.toUpper();
    ui->editHex->setText(str);
}


void Widget::on_btnHex_clicked()
{
    bool ok;
    int val=ui->editHex->text().toInt(&ok,16);//读取16进制字符串
    QString str=QString::number(val,10);
    ui->editDec->setText(str);
    str=QString::number(val,2);
    ui->editBin->setText(str);
}


void Widget::on_btnDebug_clicked()
{
    QString str;
    double num=1245.2783;
    qDebug("num= %f",num);
    str.setNum(num,'f',5);//将num转换为字符串存在str中
    qDebug("str= %s",str.toLocal8Bit().data());//使用toLocal8Bit函数将QString转换为char*类型字符串进行输出
    str.setNum(num,'E',5);//E表示使用科学计数法，使用大写的E
    qDebug("str= %s",str.toLocal8Bit().data());
    str.setNum(num,'g',5);//g表示选择两种计数法中较简洁的输出
    qDebug("str= %s",str.toLocal8Bit().data());
    str.setNum(num,'g',3);
    qDebug("str= %s",str.toLocal8Bit().data());
}

