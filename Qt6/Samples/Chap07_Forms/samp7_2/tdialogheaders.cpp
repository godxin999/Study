#include "tdialogheaders.h"
#include "ui_tdialogheaders.h"

#include <QMessageBox>

TDialogHeaders::TDialogHeaders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialogHeaders)
{
    ui->setupUi(this);
    m_model=new QStringListModel(this);
    ui->listView->setModel(m_model);
}

TDialogHeaders::~TDialogHeaders()
{
    QMessageBox::information(this,"提示","TDialogHeaders对话框被删除");
    delete ui;
}

void TDialogHeaders::setHeaderList(QStringList &headers){
    m_model->setStringList(headers);
}

QStringList TDialogHeaders::headerList(){
    return m_model->stringList();
}
