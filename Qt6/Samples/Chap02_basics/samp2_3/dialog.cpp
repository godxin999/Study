#include "dialog.h"
//#include "ui_dialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    /*, ui(new Ui::Dialog)*/
{
    //ui->setupUi(this);
    initUI();//界面创建和布局
    initSignalSlots();//信号与槽的关联
    setWindowTitle("手工创建UI");
}

Dialog::~Dialog()
{
    //delete ui;
}

void Dialog::do_chkBoxUnder(bool checked){
    QFont font=txtEdit->font();
    font.setUnderline(checked);
    txtEdit->setFont(font);
}

void Dialog::do_chkBoxItalic(bool checked){
    QFont font=txtEdit->font();
    font.setItalic(checked);
    txtEdit->setFont(font);
}

void Dialog::do_chkBoxBold(bool checked){
    QFont font=txtEdit->font();
    font.setBold(checked);
    txtEdit->setFont(font);
}

void Dialog::do_setFontColor(){
    QPalette plet=txtEdit->palette();
    if(radioBlue->isChecked()){
        plet.setColor(QPalette::Text,Qt::blue);
    }
    else if(radioBlack->isChecked()){
        plet.setColor(QPalette::Text,Qt::black);
    }
    else if(radioRed->isChecked()){
        plet.setColor(QPalette::Text,Qt::red);
    }
    else{
        plet.setColor(QPalette::Text,Qt::black);
    }
    txtEdit->setPalette(plet);
}

void Dialog::initSignalSlots(){
//关联三个设置颜色的QRadioButton
    connect(radioBlue,SIGNAL(clicked()),this,SLOT(do_setFontColor()));
    connect(radioBlack,SIGNAL(clicked()),this,SLOT(do_setFontColor()));
    connect(radioRed,SIGNAL(clicked()),this,SLOT(do_setFontColor()));
//关联三个设置字体的QCheckBox
    connect(chkBoxUnder,SIGNAL(clicked(bool)),this,SLOT(do_chkBoxUnder(bool)));
    connect(chkBoxItalic,SIGNAL(clicked(bool)),this,SLOT(do_chkBoxItalic(bool)));
    connect(chkBoxBold,SIGNAL(clicked(bool)),this,SLOT(do_chkBoxBold(bool)));
//关联三个按钮与窗口的槽函数
    connect(btnOK,SIGNAL(clicked()),this,SLOT(accept()));
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
    connect(btnClose,SIGNAL(clicked()),this,SLOT(close()));
}

void Dialog::initUI(){
//创建Underline，Italic，Bold三个CheckBox，并水平布局
    chkBoxUnder=new QCheckBox("Underline");
    chkBoxItalic=new QCheckBox("Italic");
    chkBoxBold=new QCheckBox("Bold");
    QHBoxLayout *HLay1=new QHBoxLayout();
    HLay1->addWidget(chkBoxUnder);
    HLay1->addWidget(chkBoxItalic);
    HLay1->addWidget(chkBoxBold);
//创建Black，Red，Blue三个RadioButton，并水平布局
    radioBlack=new QRadioButton("Black");
    radioBlack->setChecked(true);
    radioRed=new QRadioButton("Red");
    radioBlue=new QRadioButton("Blue");
    QHBoxLayout *HLay2=new QHBoxLayout();
    HLay2->addWidget(radioBlack);
    HLay2->addWidget(radioRed);
    HLay2->addWidget(radioBlue);
//创建确定，取消，退出三个PushButton，并水平布局
    btnOK=new QPushButton("确定");
    btnCancel=new QPushButton("取消");
    btnClose=new QPushButton("退出");
    QHBoxLayout *HLay3=new QHBoxLayout();
    HLay3->addStretch();
    HLay3->addWidget(btnOK);
    HLay3->addWidget(btnCancel);
    HLay3->addStretch();
    HLay3->addWidget(btnClose);
//创建文本框并设置初始字体
    txtEdit=new QPlainTextEdit;
    txtEdit->setPlainText("Hello world\n手工创建");
    QFont font=txtEdit->font();
    font.setPointSize(20);
    txtEdit->setFont(font);
//创建垂直布局，并设置为主布局
    QVBoxLayout *VLay=new QVBoxLayout(this);
    VLay->addLayout(HLay1);
    VLay->addLayout(HLay2);
    VLay->addWidget(txtEdit);
    VLay->addLayout(HLay3);
    setLayout(VLay);
}


