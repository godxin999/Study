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

void MainWindow::on_actAddFiles_triggered()
{
    QStringList files=QFileDialog::getOpenFileNames(this,"选择文件","","images(*.jpg)");//可选取多个jpg图片文件
    if(files.isEmpty()){//如果一个文件都没选
        return ;
    }
    QTreeWidgetItem *parItem,*item;
    item=ui->treeFiles->currentItem();//取当前选中节点
    if(item==nullptr){//如果当前节点为空
        item=ui->treeFiles->topLevelItem(0);//取顶层节点
    }
    if(item->type()==itImageItem){//如果节点类型为图片节点，则取其父节点作为将添加图片节点的父节点
        parItem=item->parent();
    }
    else{
        parItem=item;//否则父节点就是当前节点
    }
    for(int i=0;i<files.size();++i){
        QString aFilename =files.at(i);
        addImageItem(parItem,aFilename);//添加每个图片节点
    }
    parItem->setExpanded(true);//展开父节点
}

void MainWindow::addImageItem(QTreeWidgetItem *parItem, QString aFilename){
    QIcon icon(":/icons/icons/31.ico");
    QFileInfo fileInfo(aFilename);//QFileInfo用于获取文件信息
    QString NodeText =fileInfo.fileName();//不带有路径的文件名
    QDateTime birthDate=fileInfo.lastModified();//获取文件的最后修改时间

    QTreeWidgetItem *item;
    item=new QTreeWidgetItem(MainWindow::itImageItem);//构造图片节点
    item->setIcon(colItem,icon);
    item->setText(colItem,NodeText);//设置第一列的文字
    item->setText(colItemType,"Image Item");//设置第二列的文字
    item->setText(colDate,birthDate.toString("yyyy-MM-dd"));//设置第三列的文字
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    item->setCheckState(colItem,Qt::Checked);
    item->setData(colItem,Qt::UserRole,QVariant(aFilename));//设置用户数据为完整的文件名
    parItem->addChild(item);
}


void MainWindow::on_treeFiles_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    qDebug("currentItemChanged() is emitted");
    if(current==nullptr){
        return ;
    }
    if(current==previous){
        return ;
    }
    int var=current->type();
    switch(var){
        case itTopItem:
            ui->actAddFolder->setEnabled(true);
            ui->actAddFiles->setEnabled(true);
            ui->actDeleteItem->setEnabled(false);
            break;
        case itGroupItem:
            ui->actAddFolder->setEnabled(true);
            ui->actAddFiles->setEnabled(true);
            ui->actDeleteItem->setEnabled(true);
            break;
        case itImageItem:
            ui->actAddFolder->setEnabled(false);
            ui->actAddFiles->setEnabled(true);
            ui->actDeleteItem->setEnabled(true);
            displayImage(current);
            break;
    }
}


void MainWindow::on_actDeleteItem_triggered()
{
    QTreeWidgetItem *item=ui->treeFiles->currentItem();//获取当前节点
    if(item==nullptr){
        return ;
    }
    QTreeWidgetItem *parItem=item->parent();//获取当前节点的父节点
    deleteItem(parItem,item);
}

void MainWindow::deleteItem(QTreeWidgetItem *parItem, QTreeWidgetItem *item){
    if(item->childCount()>0){//如果有子节点，先删除所有子节点
        int cnt=item->childCount();//子节点数量
        QTreeWidgetItem *tempParItem=item;//临时父节点
        for(int i=cnt-1;~i;--i){
            deleteItem(tempParItem,tempParItem->child(i));//递归删除子节点
        }
    }
    parItem->removeChild(item);//移除节点
    delete item;//从内存中删除对象
}

void MainWindow::on_actScanItems_triggered()
{
    for(int i=0;i<ui->treeFiles->topLevelItemCount();++i){
        QTreeWidgetItem *item=ui->treeFiles->topLevelItem(i);
        changeItemCaption(item);
    }
}

void MainWindow::changeItemCaption(QTreeWidgetItem *item){
    QString str="*"+item->text(colItem);//在节点标题前加*
    item->setText(colItem,str);
    if(item->childCount()>0){//如果有子节点
        for(int i=0;i<item->childCount();++i){
            changeItemCaption(item->child(i));//递归调用遍历节点
        }
    }
}

void MainWindow::displayImage(QTreeWidgetItem *item){
    QString filename=item->data(colItem,Qt::UserRole).toString();//获取节点存储的文件名
    labFileName->setText(filename);//显示文件名
    labNodeText->setText(item->text(colItem));//显示节点标题
    m_pixmap.load(filename);//加载图片
    ui->actZoomFitH->trigger();//出发trigger()信号，运行其关联的槽函数
    ui->actZoomFitH->setEnabled(true);
    ui->actZoomFitW->setEnabled(true);
    ui->actZoomIn->setEnabled(true);
    ui->actZoomOut->setEnabled(true);
    ui->actZoomRealSize->setEnabled(true);
}

void MainWindow::on_actZoomFitH_triggered()
{
    int H=ui->scrollArea->height();
    int realH=m_pixmap.height();
    m_ratio=float(H)/realH;//计算缩放比例
    spinRatio->setValue(100*m_ratio);//显示缩放比例
    QPixmap pix=m_pixmap.scaledToHeight(H-30);//缩放到指定高度
    ui->labPic->setPixmap(pix);
}


void MainWindow::on_actZoomIn_triggered()
{
    m_ratio=m_ratio*1.2;//在当前比例上乘1.2
    spinRatio->setValue(100*m_ratio);//显示缩放百分比
    int w=m_ratio*m_pixmap.width();
    int h=m_ratio*m_pixmap.height();
    QPixmap pix=m_pixmap.scaled(w,h);//缩放到指定宽高
    ui->labPic->setPixmap(pix);//显示缩放后图片
}


void MainWindow::on_actZoomOut_triggered()
{
    m_ratio=m_ratio*0.8;
    spinRatio->setValue(100*m_ratio);
    int w=m_ratio*m_pixmap.width();
    int h=m_ratio*m_pixmap.height();
    QPixmap pix=m_pixmap.scaled(w,h);
    ui->labPic->setPixmap(pix);
}

//以实际大小显示
void MainWindow::on_actZoomRealSize_triggered()
{
    m_ratio=1;
    spinRatio->setValue(100);
    ui->labPic->setPixmap(m_pixmap);
}


void MainWindow::on_actZoomFitW_triggered()
{
    int W=ui->scrollArea->width();
    int realW=m_pixmap.width();
    m_ratio=float(W)/realW;//计算缩放比例
    spinRatio->setValue(100*m_ratio);//显示缩放比例
    QPixmap pix=m_pixmap.scaledToWidth(W-30);//缩放到指定高度
    ui->labPic->setPixmap(pix);
}

//根据复选属性设置是否浮动
void MainWindow::on_actDockFloat_triggered(bool checked)
{
    ui->dockWidget->setFloating(checked);
}

//根据复选属性设置是否可见
void MainWindow::on_actDockVisible_triggered(bool checked)
{
    ui->dockWidget->setVisible(checked);
}

//根据浮动性是否改变设置对应action的复选属性
void MainWindow::on_dockWidget_topLevelChanged(bool topLevel)
{
    ui->actDockFloat->setChecked(topLevel);
}

//根据可见性是否改变设置对应action的复选属性
void MainWindow::on_dockWidget_visibilityChanged(bool visible)
{
    ui->actDockVisible->setChecked(visible);
}



