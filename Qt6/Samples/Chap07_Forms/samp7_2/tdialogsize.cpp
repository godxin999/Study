#include "tdialogsize.h"
#include "ui_tdialogsize.h"

#include <QMessageBox>

TDialogSize::TDialogSize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialogSize)
{
    ui->setupUi(this);
}

TDialogSize::~TDialogSize()
{
    QMessageBox::information(this,"提示","TDialogSize对话框被删除");
    delete ui;
}

int TDialogSize::rowCount(){
    return ui->spinBoxRow->value();
}

int TDialogSize::columnCount(){
    return ui->spinBoxColumn->value();
}

void TDialogSize::setRowColumn(int row, int column){
    ui->spinBoxRow->setValue(row);
    ui->spinBoxColumn->setValue(column);
}
