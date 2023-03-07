#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->radioBlack,SIGNAL(clicked()),this,SLOT(do_setFontColor()));
    connect(ui->radioRed,SIGNAL(clicked()),this,SLOT(do_setFontColor()));
    connect(ui->radioBlue,SIGNAL(clicked()),this,SLOT(do_setFontColor()));
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_checkBoxUnder_clicked(bool checked)
{
    QFont font=ui->plainTextEdit->font();
    font.setUnderline(checked);
    ui->plainTextEdit->setFont(font);
}


void Dialog::on_checkBoxItalic_clicked(bool checked)
{
    QFont font=ui->plainTextEdit->font();
    font.setItalic(checked);
    ui->plainTextEdit->setFont(font);
}


void Dialog::on_checkBoxBold_clicked(bool checked)
{
    QFont font=ui->plainTextEdit->font();
    font.setBold(checked);
    ui->plainTextEdit->setFont(font);
}

void Dialog::on_btnClear_clicked()
{
    ui->plainTextEdit->clear();
}

void Dialog::do_setFontColor(){
    QPalette plet=ui->plainTextEdit->palette();
    if(ui->radioBlue->isChecked()){
        plet.setColor(QPalette::Text,Qt::blue);
    }
    else if(ui->radioRed->isChecked()){
        plet.setColor(QPalette::Text,Qt::red);
    }
    else if(ui->radioBlack->isChecked()){
        plet.setColor(QPalette::Text,Qt::black);
    }
    else{
        plet.setColor(QPalette::Text,Qt::black);
    }
    ui->plainTextEdit->setPalette(plet);
}
