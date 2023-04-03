#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    labNodeText =new QLabel("节点标题",this);
    labNodeText->setMinimumWidth(200);
    ui->statusbar->addWidget(labNodeText);
    spinRatio=new QSpinBox(this);//用于显示图片缩放比例的QSpinBox组件
    spinRatio->setRange(0,2000);
    spinRatio->setValue(100);
    spinRatio->setSuffix(" %");
    spinRatio->setReadOnly(true);
    spinRatio->setButtonSymbols(QAbstractSpinBox::NoButtons);//不显示右侧调节按钮
    ui->statusbar->addPermanentWidget(spinRatio);
    labFileName=new QLabel("文件名",this);
    ui->statusbar->addPermanentWidget(labFileName);

    buildTreeHeader();//重新构建目录树表头
    iniTree();//初始化目录树
    setCentralWidget(ui->scrollArea);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//重新构建treeFiles的表头
void MainWindow::buildTreeHeader(){
    ui->treeFiles->clear();//清除所有节点，但是不修改表头
    QTreeWidgetItem *header=new QTreeWidgetItem();//创建节点
    header->setText(MainWindow::colItem,"目录和文件");//根据我们自定义的枚举类型设置表头文本
    header->setText(MainWindow::colItemType,"节点类型");
    header->setText(MainWindow::colDate,"最后修改日期");
    header->setTextAlignment(colItem,Qt::AlignHCenter|Qt::AlignVCenter);
    header->setTextAlignment(colItemType,Qt::AlignHCenter|Qt::AlignVCenter);
    ui->treeFiles->setHeaderItem(header);//设置表头结点
}
//初始化目录树，创建一个顶层节点
void MainWindow::iniTree(){
    QIcon icon(":/icons/icons/15.ico");
    QTreeWidgetItem *item=new QTreeWidgetItem(MainWindow::itTopItem);//itTopItem是我们自定义类型的一个枚举值，表示顶层节点
    item->setIcon(MainWindow::colItem,icon);//设置第一列的图标
    item->setText(MainWindow::colItem,"图片");//设置第一列的文字
    item->setText(MainWindow::colItemType,"Top Item");//设置第二列的文字
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    item->setCheckState(MainWindow::colItem,Qt::Checked);//设置选中状态
    ui->treeFiles->addTopLevelItem(item);//添加顶层节点
}

void MainWindow::on_actAddFolder_triggered()
{
    QString dirName=QFileDialog::getExistingDirectory();//选择目录
    if(dirName.isEmpty()){//目录名称为空
        return;
    }
    QTreeWidgetItem *parItem=ui->treeFiles->currentItem();
    if(parItem==nullptr){//当前节点为空
        return ;
    }
    if(parItem->type()!=itImageItem){//图片节点不能添加分组节点
        addFolderItem(parItem,dirName);//在父节点下添加一个分组节点
    }
}

void MainWindow::addFolderItem(QTreeWidgetItem *parItem, QString dirName){
    QIcon icon(":/icons/icons/open3.bmp");
    QString NodeText=getFinalFolderName(dirName);//获取最后的文件夹的名称
    QTreeWidgetItem *item=new QTreeWidgetItem(MainWindow::itGroupItem);
    item->setIcon(colItem,icon);//设置图标
    item->setText(colItem,NodeText);//最后的文件夹的名称，设置第一列文字
    item->setText(colItemType,"Group Item");//设置第二列文字
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    item->setCheckState(colItem,Qt::Checked);
    item->setData(colItem,Qt::UserRole,QVariant(dirName));//设置用户数据，存储完整目录名称
    parItem->addChild(item);//添加节点
}

QString MainWindow::getFinalFolderName(const QString &fullPathName){
    int cnt=fullPathName.length();
    int i=fullPathName.lastIndexOf("/");//最后一次出现'/'的尾置
    QString str=fullPathName.right(cnt-i-1);//获取最后的文件夹的名称
    return str;
}
