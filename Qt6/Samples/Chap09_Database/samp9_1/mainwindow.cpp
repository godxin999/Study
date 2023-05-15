#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->splitter);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);//项选择
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//单项选择
    ui->tableView->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actOpenDB_triggered()
{
    QString aFile=QFileDialog::getOpenFileName(this,"选择文件","","SQLite数据库(*.db3)");
    if(aFile.isEmpty())return ;
    DB=QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(aFile);
    /*
    DB=QSqlDatabase::addDatabase("QMYSQL");//添加数据库驱动
    DB.setHostName("127.0.0.1");
    DB.setPort(3306);
    DB.setUserName("root");
    DB.setPassword("root");
    DB.setDatabaseName("demodb");
    */
    if(!DB.open()){
        QMessageBox::warning(this,"错误","打开数据库失败");
    }
    else{
        openTable();//打开数据表
    }
}

void MainWindow::openTable(){
    //创建数据模型，打开数据表
    tabModel=new QSqlTableModel(this,DB);//数据模型
    tabModel->setTable("employee");//设置数据表
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//数据保存方式
    tabModel->setSort(tabModel->fieldIndex("empNo"),Qt::AscendingOrder);//升序排序
    if(!tabModel->select()){//查询数据失败
        QMessageBox::critical(this,"错误信息","打开数据表错误，错误信息:\n"+tabModel->lastError().text());
        return ;
    }
    showRecordCount();//显示记录条数
    //设置字段显示标题
    tabModel->setHeaderData(tabModel->fieldIndex("EmpNo"),Qt::Horizontal,"工号");
    tabModel->setHeaderData(tabModel->fieldIndex("Name"),Qt::Horizontal,"姓名");
    tabModel->setHeaderData(tabModel->fieldIndex("Gender"),Qt::Horizontal,"性别");
    tabModel->setHeaderData(tabModel->fieldIndex("Birthday"),Qt::Horizontal,"出生日期");
    tabModel->setHeaderData(tabModel->fieldIndex("Province"),Qt::Horizontal,"省份");
    tabModel->setHeaderData(tabModel->fieldIndex("Departmen"),Qt::Horizontal,"部门");
    tabModel->setHeaderData(tabModel->fieldIndex("Salary"),Qt::Horizontal,"工资");
    tabModel->setHeaderData(tabModel->fieldIndex("Memo"),Qt::Horizontal,"备注");
    tabModel->setHeaderData(tabModel->fieldIndex("Photo"),Qt::Horizontal,"照片");
    //创建选择模型
    selModel=new QItemSelectionModel(tabModel,this);
    connect(selModel,&QItemSelectionModel::currentChanged,this,&MainWindow::do_currentChanged);
    connect(selModel,&QItemSelectionModel::currentRowChanged,this,&MainWindow::do_currentRowChanged);
    //模型视图结构
    ui->tableView->setModel(tabModel);//设置数据模型
    ui->tableView->setSelectionModel(selModel);//设置选择模型
    ui->tableView->setColumnHidden(tabModel->fieldIndex("Memo"),true);//隐藏列
    ui->tableView->setColumnHidden(tabModel->fieldIndex("Photo"),true);//隐藏列
    //为tableView中的性别和部门两个字段设置自定义代理
    QStringList strList;
    strList<<"男"<<"女";
    bool isEditable=true;
    delegateSex.setItems(strList,isEditable);
    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("Gender"),&delegateSex);//设置代理

    strList.clear();
    strList<<"销售部"<<"技术部"<<"生产部"<<"行政部";
    isEditable=true;
    delegateDepart.setItems(strList,isEditable);
    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("Department"),&delegateDepart);//设置代理
    //创建界面组件与模型字段的数据映射
    dataMapper=new QDataWidgetMapper(this);
    dataMapper->setModel(tabModel);//设置数据模型
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    //界面组件与模型的具体字段的映射
    dataMapper->addMapping(ui->dbSpinEmpNo,tabModel->fieldIndex("empNo"));
    dataMapper->addMapping(ui->dbEditName,tabModel->fieldIndex("Name"));
    dataMapper->addMapping(ui->dbComboSex,tabModel->fieldIndex("Gender"));
    dataMapper->addMapping(ui->dbEditBirth,tabModel->fieldIndex("Birthday"));
    dataMapper->addMapping(ui->dbComboProvince,tabModel->fieldIndex("Province"));
    dataMapper->addMapping(ui->dbComboDep,tabModel->fieldIndex("Department"));
    dataMapper->addMapping(ui->dbSpinSalary,tabModel->fieldIndex("Salary"));
    dataMapper->addMapping(ui->dbEditMemo,tabModel->fieldIndex("Memo"));
    dataMapper->toFirst();//移动到首记录
    //获取字段名称列表，填充groupBoxSort
    getFieldNames();
    //更新action和界面组件的使能状态
    ui->actOpenDB->setEnabled(false);
    ui->actRecAppend->setEnabled(true);
    ui->actRecInsert->setEnabled(true);
    ui->actScan->setEnabled(true);
    ui->groupBoxSort->setEnabled(true);//排序分组框
    ui->groupBoxFilter->setEnabled(true);//过滤分组框
}

void MainWindow::showRecordCount(){
    ui->statusbar->showMessage(QString("记录条数: %1").arg(tabModel->rowCount()));
}

void MainWindow::getFieldNames(){
    QSqlRecord emptyRec=tabModel->record();//获取空记录，只有字段名
    for(int i=0;i<emptyRec.count();++i){
        ui->comboFields->addItem(emptyRec.fieldName(i));//填充到排序字段的下拉列表中
    }
}

void MainWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous){
    Q_UNUSED(previous);
    Q_UNUSED(current);
    ui->actSubmit->setEnabled(tabModel->isDirty());//有未更新到数据库的修改时可用
    ui->actRevert->setEnabled(tabModel->isDirty());
}

void MainWindow::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous){
    Q_UNUSED(previous);
    //行切换时的状态控制
    ui->actRecDelete->setEnabled(current.isValid());
    ui->actPhoto->setEnabled(current.isValid());
    ui->actPhotoClear->setEnabled(current.isValid());
    if(!current.isValid()){
        ui->dbLabPhoto->clear();
        return ;
    }

    dataMapper->setCurrentIndex(current.row());//更新数据映射的行号
    int curRecNo=current.row();
    QSqlRecord curRec=tabModel->record(curRecNo);//获取当前记录
    if(curRec.isNull("Photo")){
        ui->dbLabPhoto->clear();
    }
    else{
        QByteArray data=curRec.value("Photo").toByteArray();//将图片作为二进制数据读出
        QPixmap pic;
        pic.loadFromData(data);//将二进制数据转换为图片
        ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->size().width()));
    }
}

void MainWindow::on_actRecAppend_triggered()
{
    QSqlRecord rec=tabModel->record();//获取一条空记录，只有字段定义
    rec.setValue(tabModel->fieldIndex("empNo"),2000+tabModel->rowCount());
    rec.setValue(tabModel->fieldIndex("Gender"),"男");//设置数据
    tabModel->insertRecord(tabModel->rowCount(),rec);//插入到数据模型的最后

    selModel->clearSelection();
    QModelIndex curIndex=tabModel->index(tabModel->rowCount()-1,1);
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置当前行
}


void MainWindow::on_actRecInsert_triggered()
{
    QModelIndex curIndex=ui->tableView->currentIndex();//当前行的模型索引
    QSqlRecord rec=tabModel->record();
    tabModel->insertRecord(curIndex.row(),rec);//在当前行之前插入一条记录
    selModel->clearSelection();
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置当前行
    showRecordCount();
}


void MainWindow::on_actRecDelete_triggered()
{
    QModelIndex curIndex=selModel->currentIndex();
    tabModel->removeRow(curIndex.row());//删除当前行
    showRecordCount();
}


void MainWindow::on_actSubmit_triggered()
{
    bool res=tabModel->submitAll();
    if(!res){
        QMessageBox::information(this,"消息","数据保存错误,错误信息\n"+tabModel->lastError().text());
    }
    else{
        ui->actSubmit->setEnabled(false);
        ui->actRevert->setEnabled(false);
    }
    showRecordCount();
}


void MainWindow::on_actRevert_triggered()
{
    tabModel->revertAll();
    ui->actSubmit->setEnabled(false);
    ui->actRevert->setEnabled(false);
    showRecordCount();
}


void MainWindow::on_actPhoto_triggered()
{
    QString aFile=QFileDialog::getOpenFileName(this,"选择图片文件","","照片(*.jpg)");
    if(aFile.isEmpty()){
        return ;
    }
    QByteArray data;
    QFile *file=new QFile(aFile);
    file->open(QIODevice::ReadOnly);
    data=file->readAll();//读取图片数据为字节数据数组
    file->close();

    int curRecNo=selModel->currentIndex().row();
    QSqlRecord curRec=tabModel->record(curRecNo);//获取当前记录
    curRec.setValue("Photo",data);//设置字段数据
    tabModel->setRecord(curRecNo,curRec);//修改记录
    QPixmap pic;
    pic.loadFromData(data);
    ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->width()));
}


void MainWindow::on_actPhotoClear_triggered()
{
    int curRecNo=selModel->currentIndex().row();
    QSqlRecord curRec=tabModel->record(curRecNo);//获取当前记录
    curRec.setNull("Photo");//设置为空值
    tabModel->setRecord(curRecNo,curRec);//修改当前记录
    ui->dbLabPhoto->clear();
}


void MainWindow::on_actScan_triggered()
{
    if(tabModel->rowCount()==0){
        return ;
    }
    for(int i=0;i<tabModel->rowCount();++i){
        QSqlRecord aRec=tabModel->record(i);//获取每行的数据
        float salary=aRec.value("Salary").toFloat();
        salary=salary*1.1;
        aRec.setValue("Salary",salary);//更新字段数据
        tabModel->setRecord(i,aRec);//更新记录
    }
    if(tabModel->submitAll()){
        QMessageBox::information(this,"消息","涨工资数据计算完毕");
    }
}


void MainWindow::on_comboFields_currentIndexChanged(int index)
{
    if(ui->radioBtnAscend->isChecked()){
        tabModel->setSort(index,Qt::AscendingOrder);
    }
    else{
        tabModel->setSort(index,Qt::DescendingOrder);
    }
    tabModel->select();
}


void MainWindow::on_radioBtnAscend_clicked()
{
    tabModel->setSort(ui->comboFields->currentIndex(),Qt::AscendingOrder);
    tabModel->select();
}


void MainWindow::on_radioBtnDescend_clicked()
{
    tabModel->setSort(ui->comboFields->currentIndex(),Qt::DescendingOrder);
    tabModel->select();
}


void MainWindow::on_radioBtnMan_clicked()
{
    tabModel->setFilter(" Gender= '男' ");
    showRecordCount();
}


void MainWindow::on_radioBtnWoman_clicked()
{
    tabModel->setFilter(" Gender='女' ");
    showRecordCount();
}


void MainWindow::on_radioBtnBoth_clicked()
{
    tabModel->setFilter("");
    showRecordCount();
}

