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

void Widget::showBoolInfo(QString str, bool CheckValue){
    if(CheckValue){
        ui->plainTextEdit->appendPlainText(str+"= true");
    }
    else{
        ui->plainTextEdit->appendPlainText(str+"= false");
    }
}


void Widget::on_btnClearText_clicked()
{
    ui->plainTextEdit->clear();
}


void Widget::on_pushButton_5_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---isNull(),isEmpty()函数测试---");
    QString str1,str2="";
    ui->plainTextEdit->appendPlainText("QString str1, str2=""");
    showBoolInfo("str1.isNull()",str1.isNull());
    showBoolInfo("str1.isEmpty()",str1.isEmpty());
    showBoolInfo("\nstr2.isNull()",str2.isNull());
    showBoolInfo("str2.isEmpty()",str2.isEmpty());
    str2.clear();
    ui->plainTextEdit->appendPlainText("\nstr2.clear()后");
    showBoolInfo("\nstr2.isNull()",str2.isNull());
    showBoolInfo("str2.isEmpty()",str2.isEmpty());
}


void Widget::on_pushButton_3_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---first(),last()函数测试---");
    QString str1=ui->comboBox->currentText();
    QString str2=ui->comboBox_2->currentText();
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    ui->plainTextEdit->appendPlainText("str2= "+str2);
    int N=str1.lastIndexOf(str2);//查找str2在str1中最后出现的位置
    QString str3=str1.first(N+1);//前N+1个字符，这里是文件路径
    ui->plainTextEdit->appendPlainText(str3);
    str3=str1.last(str1.size()-N-1);//最后几个字符，这里是去除路径的文件名
    ui->plainTextEdit->appendPlainText(str3);
}


void Widget::on_pushButton_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---front(),back()函数测试---");
    QString str1="Hello,北京";
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    QChar ch1=str1.front();
    ui->plainTextEdit->appendPlainText("str1.front()= "+QString(ch1));
    QChar ch2=str1.back();
    ui->plainTextEdit->appendPlainText("str1.back()= "+QString(ch2));
}


void Widget::on_pushButton_2_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---left(),right()函数测试---");
    QString str1=ui->comboBox->currentText();
    QString str2=ui->comboBox_2->currentText();
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    ui->plainTextEdit->appendPlainText("str2= "+str2);
    int N=str1.lastIndexOf(str2);//查找str2在str1中最后出现的位置
    QString str3=str1.left(N+1);//前N+1个字符，这里是文件路径
    ui->plainTextEdit->appendPlainText(str3);
    str3=str1.right(str1.size()-N-1);//最后几个字符，这里是去除路径的文件名
    ui->plainTextEdit->appendPlainText(str3);
}

void Widget::on_pushButton_4_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---section()函数测试---");
    QString str1=ui->comboBox->currentText();
    QString str2=ui->comboBox_2->currentText();
    int N=ui->spinBox->value();//获取spinBox中的参数值
    QString str3;
    if(QString::compare("\\",str2,Qt::CaseInsensitive)==0){//判断str是否是"\"，且忽略大小写
        str3=str1.section("\\",N,N);//根据参数截取数据段
    }
    else{
        str3=str1.section(str2,N,N);
    }
    ui->plainTextEdit->appendPlainText(str3);
}


void Widget::on_pushButton_6_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---resize()函数测试---");
    QString str1;
    str1.resize(5,'0');
    ui->plainTextEdit->appendPlainText("str1.resize(5,'0')后");
    ui->plainTextEdit->appendPlainText("str1= "+str1);
}


void Widget::on_pushButton_7_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---size(),count(),length()函数测试---");
    QString str1="Hello";
    ui->plainTextEdit->appendPlainText("str1="+str1);
    ui->plainTextEdit->appendPlainText(QString::asprintf("str1.size()= %lld",str1.size()));
    ui->plainTextEdit->appendPlainText(QString::asprintf("str1.count()= %lld",str1.count()));
    ui->plainTextEdit->appendPlainText(QString::asprintf("str1.length()= %lld",str1.length()));
}


void Widget::on_pushButton_8_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---fill()函数测试---");
    QString str1="hello";
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    str1.fill('X');
    ui->plainTextEdit->appendPlainText("str1.fill('X')后");
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    str1.fill('A',2);
    ui->plainTextEdit->appendPlainText("str1.fill('A',2)后");
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    str1.fill(QChar(0x54C8),3);
    ui->plainTextEdit->appendPlainText("str1.fill(QChar(0x54C8),3)后");
    ui->plainTextEdit->appendPlainText("str1= "+str1);
}


void Widget::on_pushButton_10_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---contains()函数测试---");
    QString str1=ui->comboBox->currentText();
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    showBoolInfo("str1.contains(\".cpp\")",str1.contains(".cpp"));
    showBoolInfo("str1.contains(\".CPP\",Qt::CaseInsensitive)",str1.contains(".CPP",Qt::CaseInsensitive));
}


void Widget::on_pushButton_12_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---count()函数测试---");
    QString str1=ui->comboBox->currentText();
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    ui->plainTextEdit->appendPlainText(QString::asprintf("str1.count(\"QT\")= %lld",str1.count("QT")));
    ui->plainTextEdit->appendPlainText(QString::asprintf("str1.count(\"QT\",Qt::CaseInsensitive)= %lld",str1.count("QT",Qt::CaseInsensitive)));
}


void Widget::on_pushButton_9_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---indexOf(),lastIndexOf函数测试---");
    QString str1=ui->comboBox->currentText();
    QString str2=ui->comboBox_2->currentText();
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    ui->plainTextEdit->appendPlainText("str2= "+str2);
    ui->plainTextEdit->appendPlainText(QString::asprintf("str1.indexOf(str2)= %lld",str1.indexOf(str2)));
    ui->plainTextEdit->appendPlainText(QString::asprintf("str1.lastIndexOf(str2)= %lld",str1.lastIndexOf(str2)));
}


void Widget::on_pushButton_11_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---endsWith(),startsWith函数测试---");
    QString str1=ui->comboBox->currentText();
    QString str2=ui->comboBox_2->currentText();
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    ui->plainTextEdit->appendPlainText("str2= "+str2);
    showBoolInfo("str1.endsWith(str1)",str1.endsWith(str2));
    showBoolInfo("str1.startsWith(str1)",str1.startsWith(str2));
}


void Widget::on_pushButton_14_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---insert函数测试---");
    QString str1="It is great.";
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    str1.insert(str1.lastIndexOf(" "),"n't");
    ui->plainTextEdit->appendPlainText("str1.insert(str1.lastIndexOf(\" \"),\"n't\")= "+str1);
}


void Widget::on_pushButton_15_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---remove函数测试---");
    QString str1="你们,我们,它们";
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    QString Destr="们";
    ui->plainTextEdit->appendPlainText("Dechar= "+Destr);
    ui->plainTextEdit->appendPlainText("str1.remove(Dechar)后");
    QChar Dechar=QChar(Destr[0].unicode());
    str1.remove(Dechar);//去除指定的字符
    ui->plainTextEdit->appendPlainText("str1= "+str1);

    str1=ui->comboBox->currentText();
    QString str2=ui->comboBox_2->currentText();
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    ui->plainTextEdit->appendPlainText("str2= "+str2);
    str1.remove(str1.lastIndexOf(str2)+1,20);
    ui->plainTextEdit->appendPlainText("str1.remove(str1.lastIndexOf(str2)+1,20)后");
    ui->plainTextEdit->appendPlainText("str1= "+str1);

}


void Widget::on_pushButton_16_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---replace函数测试---");
    QString str1="Goooooogle";
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    str1.replace('o','e');
    ui->plainTextEdit->appendPlainText("str1.replace('o','e')后");
    ui->plainTextEdit->appendPlainText("str1= "+str1);

    str1="It is great.";
    QString str2="wonderful";
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    ui->plainTextEdit->appendPlainText("str2= "+str2);
    str1.replace(str1.lastIndexOf(" ")+1,5,str2);//第一个参数是开始替换的位置，第二个参数为替换字符的长度，第三个参数为替换的内容
    ui->plainTextEdit->appendPlainText("str1.replace(str1.lastIndexOf(\" \")+1,5,str2)后");
    ui->plainTextEdit->appendPlainText("str1= "+str1);
}


void Widget::on_pushButton_13_clicked()
{
    ui->plainTextEdit->appendPlainText("\n---trimmed,simplified函数测试---");
    QString str1=ui->comboBox->currentText();
    ui->plainTextEdit->appendPlainText("str1= "+str1);
    QString str2=str1.trimmed();//去除首尾的空白字符
    ui->plainTextEdit->appendPlainText("str1.trimmed()= "+str2);
    str2=str1.simplified();//去除所有空白字符
    ui->plainTextEdit->appendPlainText("str1.simplified()= "+str2);
}

