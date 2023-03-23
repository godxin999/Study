#include "widget.h"
#include "ui_widget.h"
#include <QMenu>
#include <QVariant>
#include <QMap>

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


void Widget::on_btnInitItems_clicked()
{
    QIcon icon;//图标类
    icon.addFile(":/icons/icons/aim.ico");//在资源文件中找到图标
    ui->comboBox->clear();
    for(int i=0;i<20;++i){
        ui->comboBox->addItem(icon,QString("Item %1").arg(i));//添加带有图标的对象
        ui->comboBox->addItem(QString("Item %1").arg(i));
    }
}


void Widget::on_btnIni2_clicked()
{
    QMap<QString,int> City_Zone;
    City_Zone.insert("北京",10);
    City_Zone.insert("上海",21);
    City_Zone.insert("天津",22);
    City_Zone.insert("大连",411);
    City_Zone.insert("锦州",416);
    City_Zone.insert("徐州",516);
    City_Zone.insert("福州",591);
    City_Zone.insert("青岛",532);
    ui->comboBox_2->clear();
    foreach(const QString &str,City_Zone.keys()){
        ui->comboBox_2->addItem(str,City_Zone.value(str));//城市名称作为列表显示的文字，区号作为用户数据，在界面上只能看到城市名称
    }
}


void Widget::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->plainTextEdit->appendPlainText(arg1);
}


void Widget::on_comboBox_2_currentIndexChanged(int index)
{
    Q_UNUSED(index);//不使用这个参数
    QString city=ui->comboBox_2->currentText();
    QString zone=ui->comboBox_2->currentData().toString();//获取数据
    ui->plainTextEdit->appendPlainText(city+": 区号 = "+zone);
}


void Widget::on_btnClearText_clicked()
{
    ui->plainTextEdit->clear();
}


void Widget::on_btnClearItems_clicked()
{
    ui->comboBox->clear();
}


void Widget::on_cnkBoxEditable_clicked(bool checked)
{
    ui->comboBox->setEditable(checked);
}


void Widget::on_plainTextEdit_customContextMenuRequested(const QPoint &pos)//创建并显示标准弹出菜单
{
    QMenu* menu=ui->plainTextEdit->createStandardContextMenu();
    menu->exec(pos);
}

