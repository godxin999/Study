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
    //QString aFile=QFileDialog::getOpenFileName(this,"选择文件","","SQLite数据库(*.db3)");
    //if(aFile.isEmpty())return ;
    DB=QSqlDatabase::addDatabase("QMYSQL");//添加数据库驱动
    DB.setHostName("127.0.0.1");
    DB.setPort(3306);
    DB.setUserName("root");
    DB.setPassword("root");
    DB.setDatabaseName("demodb");
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
    tabModel->setHeaderData(tabModel->fieldIndex("Text"),Qt::Horizontal,"备注");
    tabModel->setHeaderData(tabModel->fieldIndex("Photo"),Qt::Horizontal,"照片");
    //创建选择模型
    selModel=new QItemSelectionModel(tabModel,this);
    connect(selModel,&QItemSelectionModel::currentChanged,this,&MainWindow::do_currentChanged);
    connect(selModel,&QItemSelectionModel::currentRowChanged,this,&MainWindow::do_currentRowChanged);
    //模型视图结构
    ui->tableView->setModel(tabModel);//设置数据模型
    ui->tableView->setSelectionModel(selModel);//设置选择模型
    ui->tableView->setColumnHidden(tabModel->fieldIndex("Text"),true);//隐藏列
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
    dataMapper->addMapping(ui->dbEditText,tabModel->fieldIndex("Text"));
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

}

void MainWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous){

}

void MainWindow::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous){

}
