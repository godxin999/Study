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


void Widget::on_btnGetChars_clicked()
{
    QString str=ui->editStr->text();//读取输入的字符串
    if(str.isEmpty()){//判空
        return ;
    }
    ui->plainTextEdit->clear();
    for(quint16 i=0;i<str.size();++i){
        QChar ch=str.at(i);//获取单个字符
        char16_t uniCode=ch.unicode();//获取Unicode(UTF-16)值
        QString chStr(ch);//将QChar转换为QString
        QString info=chStr+QString::asprintf("\t,Unicode编码=0x%X",uniCode);
        ui->plainTextEdit->appendPlainText(info);
    }
}


void Widget::on_btnCharJudge_clicked()
{
    QString str=ui->editChar->text();
    if(str.isEmpty()){
        return ;
    }
    QChar ch=str[0];
    char16_t uniCode=ch.unicode();
    QString chStr(ch);
    QString info=chStr+QString::asprintf("\t,Unicode编码=0x%X\n",uniCode);
    ui->plainTextEdit->appendPlainText(info);
    //使用QChar的接口来判断字符的属性
    ui->chkDigit->setChecked(ch.isDigit());//是否为数字
    ui->chkLetter->setChecked(ch.isLetter());//是否为字母
    ui->chkLetterOrNumber->setChecked(ch.isLetterOrNumber());//是否为字母或数字
    ui->chkUpper->setChecked(ch.isUpper());//是否为大写字母
    ui->chkLower->setChecked(ch.isLower());//是否为小写字母
    ui->chkMark->setChecked(ch.isMark());//是否为记号
    ui->chkSpace->setChecked(ch.isSpace());//是否为分隔符号
    ui->chkSymbol->setChecked(ch.isSymbol());//是否为符号
    ui->chkPunct->setChecked(ch.isPunct());//是否为标点
}


void Widget::on_btnConvLatin1_clicked()
{
    QString str="Dimple";
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(str);
    QChar chP=QChar::fromLatin1('P');
    str[0]=chP;
    ui->plainTextEdit->appendPlainText("\n"+str);
}


void Widget::on_btnConvUTF16_clicked()
{
    QString str="Hello,北京";
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(str);
    str[6]=QChar(0x9752);
    str[7]=QChar::fromUcs2(0x5C9B);
    ui->plainTextEdit->appendHtml("\n"+str);
}


void Widget::on_btnCompare_clicked()
{
    QString str="它们来自河南或河北";
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(str);
    QString HuStr="河to湖";
    QChar He=QChar::fromUcs2(HuStr[0].unicode());
    QChar Hu=QChar(HuStr[3].unicode());
    for(qint16 i=0;i<str.size();++i){
        if(str.at(i)==He){
            str[i]=Hu;
        }
    }
    ui->plainTextEdit->appendPlainText("\n"+str);
}


void Widget::on_btnClear_clicked()
{
    ui->plainTextEdit->clear();
}

