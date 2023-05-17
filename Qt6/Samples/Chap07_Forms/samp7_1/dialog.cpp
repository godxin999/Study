#include "dialog.h"
#include "ui_dialog.h"

#include <QDir>
#include <QFileDialog>
#include <QColorDialog>
#include <QPalette>
#include <QFont>
#include <QFontDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QElapsedTimer>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_btnOpen_clicked()
{
    QString curPath=QDir::currentPath();//获取系统当前目录
    QString dlgTitle="选择一个文件";//对话框标题
    QString filter="文本文件(*.txt);;图片文件(*.jpg *.gif *.png);;所有文件(*.*)";//过滤器，每组文件间用两个分号隔开，同组不同后缀用空格隔开
    QString fileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    if(!fileName.isEmpty()){
        ui->plainTextEdit->appendPlainText(fileName);
    }
}


void Dialog::on_btnOpenMulti_clicked()
{
    QString curPath=QDir::currentPath();//获取系统当前目录
    QString dlgTitle="选择多个文件";//对话框标题
    QString filter="文本文件(*.txt);;图片文件(*.jpg *.gif *.png);;所有文件(*.*)";//过滤器，每组文件间用两个分号隔开，同组不同后缀用空格隔开
    QStringList fileList=QFileDialog::getOpenFileNames(this,dlgTitle,curPath,filter);
    for(int i=0;i<fileList.size();++i){
        ui->plainTextEdit->appendPlainText(fileList.at(i));
    }
}

void Dialog::do_progress_canceled(){
    ui->plainTextEdit->appendPlainText("**进度对话框被取消了**");
}

void Dialog::on_btnSave_clicked()
{
    QString curPath=QCoreApplication::applicationDirPath();//可执行程序所在的目录
    QString dlgTitle="保存文件";
    QString filter="文本文件(*.txt);;h文件(*.h);;C++文件(*.cpp);;所有文件(*.*)";
    QString fileName=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
    if(!fileName.isEmpty()){
        ui->plainTextEdit->appendPlainText(fileName);
    }
}


void Dialog::on_btnSaveDir_clicked()
{
    QString curPath=QCoreApplication::applicationDirPath();
    QString dlgTitle="选择一个目录";
    QString selectDir=QFileDialog::getExistingDirectory(this,dlgTitle,curPath);//返回选择的目录的名称的字符串
    if(!selectDir.isEmpty()){
        ui->plainTextEdit->appendPlainText(selectDir);
    }
}


void Dialog::on_btnColor_clicked()
{
    QPalette pal=ui->plainTextEdit->palette();
    QColor iniColor=pal.color(QPalette::Text);//初始颜色
    QColor color=QColorDialog::getColor(iniColor,this,"选择颜色");
    if(color.isValid()){//选择有效
        pal.setColor(QPalette::Text,color);//设置选择的颜色
        ui->plainTextEdit->setPalette(pal);//设置Palette
    }
}


void Dialog::on_btnFont_clicked()
{
    QFont iniFont=ui->plainTextEdit->font();//获取文本框的字体
    bool ok=false;//作为返回值
    QFont font=QFontDialog::getFont(&ok,iniFont);//选择字体
    if(ok){
        ui->plainTextEdit->setFont(font);
    }
}


void Dialog::on_btnProgress_clicked()
{
    QString labText="正在复制文件...";//信息标签显示的文字
    QString btnText="取消";//取消按钮的标题
    int minV=0,maxV=200;
    QProgressDialog dlgProgress(labText,btnText,minV,maxV,this);
    connect(&dlgProgress,SIGNAL(canceled()),this,SLOT(do_progress_canceled()));
    dlgProgress.setWindowTitle("复制文件");
    dlgProgress.setWindowModality(Qt::WindowModal);//以模态方式显示对话框
    dlgProgress.setAutoReset(true);//value()到达最大值时自动调用reset()
    dlgProgress.setAutoClose(true);//调用reset()时隐藏窗口

    QElapsedTimer msCounter;//计时器
    for(int i=minV;i<=maxV;++i){//用循环表示操作进度
        dlgProgress.setValue(i);
        dlgProgress.setLabelText(QString::asprintf("正在复制文件，第 %d 个",i));
        msCounter.start();
        while(true){
            if(msCounter.elapsed()>30){//运行时间30ms
                break;
            }
        }
        if(dlgProgress.wasCanceled()){//中途取消
            break;
        }
    }
}


void Dialog::on_btnInputString_clicked()
{
    QString dlgTitle="输入文字对话框";
    QString txtLabel="请输入文件名";//对话框上的提示文字
    QString iniInput="新建文件.txt";//默认输入
    QLineEdit::EchoMode echoMode=QLineEdit::Normal;//正常文字输入
    bool ok=false;
    QString text=QInputDialog::getText(this,dlgTitle,txtLabel,echoMode,iniInput,&ok);
    if(ok&&!text.isEmpty()){
        ui->plainTextEdit->appendPlainText(text);
    }
}


void Dialog::on_btnInputInt_clicked()
{
    QString dlgTitle="输入整数对话框";
    QString txtLabel="设置文本框字体大小";
    int defaultValue=ui->plainTextEdit->font().pointSize();//现有字体大小作为默认值
    int minValue=6,maxValue=50,stepValue=1;//设置最大值、最小值和步长
    bool ok=false;
    int inputValue=QInputDialog::getInt(this,dlgTitle,txtLabel,defaultValue,minValue,maxValue,stepValue,&ok);

    if(ok){
        QString str=QString("文本框大小被设置为:%1").arg(inputValue);
        ui->plainTextEdit->appendPlainText(str);
        QFont font=ui->plainTextEdit->font();
        font.setPointSize(inputValue);
        ui->plainTextEdit->setFont(font);
    }
}


void Dialog::on_btnInputFloat_clicked()
{
    QString dlgTitle="输入浮点数对话框";
    QString txtLabel="输入一个浮点数";
    float defaultValue=3.13;
    float minValue=0,maxValue=10000;//范围
    int decimals=2;//小数点位数
    bool ok=false;
    float inputValue=QInputDialog::getDouble(this,dlgTitle,txtLabel,defaultValue,minValue,maxValue,decimals,&ok);

    if(ok){
        QString str=QString::asprintf("输入了一个浮点数:%.2f",inputValue);
        ui->plainTextEdit->appendPlainText(str);
    }
}

void Dialog::on_btnInputItem_clicked()
{
    QStringList items;
    items<<"优秀"<<"良好"<<"合格"<<"不合格";
    QString dlgTitle="条目选择对话框";
    QString txtLabel="请选择级别";
    int curIndex=0;//初始选择项
    bool editable=true;//是否可编辑
    bool ok=false;
    QString text=QInputDialog::getItem(this,dlgTitle,txtLabel,items,curIndex,editable,&ok);
    if(ok&&!text.isEmpty()){
        ui->plainTextEdit->appendPlainText(text);
    }
}


void Dialog::on_btnMsgInformation_clicked()
{
    QString dlgTitle="information消息框";
    QString strInfo="文件已经打开，请检查";//对话框需要显示的信息
    QMessageBox::information(this,dlgTitle,strInfo,QMessageBox::Ok,QMessageBox::NoButton);
}


void Dialog::on_btnMsgWarning_clicked()
{
    QString dlgTitle="warning消息框";
    QString strInfo="文件内容已被修改";
    QMessageBox::warning(this,dlgTitle,strInfo);
}


void Dialog::on_btnMsgCritical_clicked()
{
    QString dlgTitle="critical消息框";
    QString strInfo="有不明程序访问网络";
    QMessageBox::warning(this,dlgTitle,strInfo);
}


void Dialog::on_btnMsgAbout_clicked()
{
    QString dlgTitle="about消息框";
    QString strInfo="SEGY文件查看软件 V1.0 \nDesigned by zx";
    QMessageBox::warning(this,dlgTitle,strInfo);
}


void Dialog::on_btnMsgQuestion_clicked()
{
    QString dlgTitle="Question消息框";
    QString strInfo="文件已被修改，是否保存修改？";
    QMessageBox::StandardButton defaultBtn=QMessageBox::NoButton;//默认按钮
    QMessageBox::StandardButton result;//返回选择的按钮内容
    result=QMessageBox::question(this,dlgTitle,strInfo,QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,defaultBtn);

    if(result==QMessageBox::Yes){
        ui->plainTextEdit->appendPlainText("Question消息框: Yes 被选择");
    }
    else if(result==QMessageBox::No){
        ui->plainTextEdit->appendPlainText("Question消息框: No 被选择");
    }
    else if(result==QMessageBox::Cancel){
        ui->plainTextEdit->appendPlainText("Question消息框: Cancel 被选择");
    }
    else{
        ui->plainTextEdit->appendPlainText("Question消息框: 无选择");
    }
}


void Dialog::on_btnMsgAboutQt_clicked()
{
    QString dlgTitle="aboutQt消息框";
    QMessageBox::aboutQt(this,dlgTitle);
}

