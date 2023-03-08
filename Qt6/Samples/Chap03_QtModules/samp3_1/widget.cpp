#include "widget.h"
#include "ui_widget.h"
#include <QMetaProperty>
#include <QCheckBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //创建对象时使用this作为父对象，this为窗口对象，boy被加入了窗口的对象树，如果不传递this为父对象，那么Tperson就不会被显式删除，需要在Widget类的析构函数中显式delete
    boy=new Tperson("王小明",this);
    boy->setProperty("score",95);
    boy->setProperty("age",10);
    boy->setProperty("sex","Boy");//sex没有在Tperson类中定义，所以这个属性是动态属性
    connect(boy,SIGNAL(ageChanged(int)),this,SLOT(do_ageChanged(int)));

    girl=new Tperson("张小丽",this);
    girl->setProperty("score",81);
    girl->setProperty("age",20);
    girl->setProperty("sex","Girl");
    connect(girl,&Tperson::ageChanged,this,&Widget::do_ageChanged);//两种connect方式均可

    ui->spinBoy->setProperty("isBoy",true);//为页面的组件spinBoy和spinGirl各设置一个动态属性isBoy，分别赋值为true和false
    ui->spinGirl->setProperty("isBoy",false);
    connect(ui->spinGirl,SIGNAL(valueChanged(int)),this,SLOT(do_spinChanged(int)));//关联信号和槽函数
    connect(ui->spinBoy,&QSpinBox::valueChanged,this,&Widget::do_spinChanged);
    ui->spinBoy->setValue(boy->age());
    ui->spinGirl->setValue(girl->age());

}

Widget::~Widget()
{
    delete ui;
}

void Widget::do_ageChanged(int value){
    Q_UNUSED(value);
    Tperson *person=qobject_cast<Tperson*>(sender());//获取信号发射者
    QString hisName=person->property("name").toString();//获取姓名
    QString hisSex=person->property("sex").toString();//获取动态属性sex
    int hisAge=person->age();//从接口获得年龄
    QString str=QString("%1,%2,年龄=%3").arg(hisName).arg(hisSex).arg(hisAge);
    ui->textEdit->appendPlainText(str);
}

void Widget::do_spinChanged(int arg1){
    QSpinBox *spinBox=qobject_cast<QSpinBox*>(sender());//获取信号发送者
    if(spinBox->property("isBoy").toBool()){//根据动态属性判断是哪个spinBox
        boy->setAge(arg1);
    }
    else{
        girl->setAge(arg1);
    }
}

void Widget::on_btnClassInfo_clicked(){
    QObject *obj=boy;
    const QMetaObject *meta=obj->metaObject();
    ui->textEdit->clear();
    ui->textEdit->appendPlainText(QString("类名称：%1\n").arg(meta->className()));
    ui->textEdit->appendPlainText("property");
    for(int i=meta->propertyOffset();i<meta->propertyCount();++i){
        const char *propName=meta->property(i).name();
        QString propValue=obj->property(propName).toString();
        QString str=QString("属性名称=%1，属性值=%2").arg(propName).arg(propValue);
        ui->textEdit->appendPlainText(str);
    }
    ui->textEdit->appendPlainText("");
    ui->textEdit->appendPlainText("ClassInfo");
    for(int i=meta->classInfoOffset();i<meta->classInfoCount();++i){
        QMetaClassInfo classInfo=meta->classInfo(i);
        ui->textEdit->appendPlainText(
                    QString("Name=%1；Value=%2").arg(classInfo.name()).arg(classInfo.value()));
    }

}

void Widget::on_btnClear_clicked(){
    ui->textEdit->clear();
}

void Widget::on_btnBoyInc_clicked(){
    boy->incAge();
}

void Widget::on_btnGirlInc_clicked(){
    girl->incAge();
}
