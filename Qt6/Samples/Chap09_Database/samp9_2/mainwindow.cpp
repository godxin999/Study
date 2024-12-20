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
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//行选择
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//单行选择
    ui->tableView->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actOpenDB_triggered()
{
    QString aFile=QFileDialog::getOpenFileName(this,"选择文件","","SQLite数据库(*.db3)");
    if(aFile.isEmpty()){
        return ;
    }
    DB=QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(aFile);
    if(DB.open()){
        selectData();
    }
    else{
        QMessageBox::warning(this,"错误","打开数据库失败");
    }
}

void MainWindow::selectData(){
    //创建数据模型，查询数据
    qryModel=new QSqlQueryModel(this);
    qryModel->setQuery("SELECT empNo,Name,Gender,Birthday,Province,Department,Salary FROM employee ORDER BY empNo ");
    if(qryModel->lastError().isValid()){
        QMessageBox::critical(this,"错误","数据表查询错误,错误信息\n"+qryModel->lastError().text());
        return ;
    }
    ui->statusbar->showMessage(QString("记录条数： %1").arg(qryModel->rowCount()));
    //设置字段显示标题
    QSqlRecord rec=qryModel->record();
    qryModel->setHeaderData(rec.indexOf("empNo"),Qt::Horizontal,"工号");
    qryModel->setHeaderData(rec.indexOf("Name"),Qt::Horizontal,"姓名");
    qryModel->setHeaderData(rec.indexOf("Gender"),Qt::Horizontal,"性别");
    qryModel->setHeaderData(rec.indexOf("Birthday"),Qt::Horizontal,"生日");
    qryModel->setHeaderData(rec.indexOf("Province"),Qt::Horizontal,"省份");
    qryModel->setHeaderData(rec.indexOf("Department"),Qt::Horizontal,"部门");
    qryModel->setHeaderData(rec.indexOf("Salary"),Qt::Horizontal,"工资");
    //创建选择模型
    selModel=new QItemSelectionModel(qryModel,this);
    connect(selModel,&QItemSelectionModel::currentRowChanged,this,&MainWindow::do_currentRowChanged);
    ui->tableView->setModel(qryModel);
    ui->tableView->setSelectionModel(selModel);
    //创建数据组件映射
    dataMapper=new QDataWidgetMapper(this);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    dataMapper->setModel(qryModel);
    //界面组件与模型的具体字段的映射
    dataMapper->addMapping(ui->dbSpinEmpNo,rec.indexOf("empNo"));
    dataMapper->addMapping(ui->dbEditName,rec.indexOf("Name"));
    dataMapper->addMapping(ui->dbComboSex,rec.indexOf("Gender"));
    dataMapper->addMapping(ui->dbEditBirth,rec.indexOf("Birthday"));
    dataMapper->addMapping(ui->dbComboProvince,rec.indexOf("Province"));
    dataMapper->addMapping(ui->dbComboDep,rec.indexOf("Department"));
    dataMapper->addMapping(ui->dbSpinSalary,rec.indexOf("Salary"));
    dataMapper->toFirst();
    ui->actOpenDB->setEnabled(false);

}

void MainWindow::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous){
    Q_UNUSED(previous);
    if(!current.isValid()){
        ui->dbLabPhoto->clear();
        ui->dbEditMemo->clear();
        return ;
    }
    dataMapper->setCurrentModelIndex(current);//设置当前行

    bool first=(current.row()==0);
    bool last=(current.row()==qryModel->rowCount()-1);
    ui->actRecFirst->setEnabled(!first);
    ui->actRecLast->setEnabled(!last);
    ui->actRecNext->setEnabled(!last);
    ui->actRecPrevious->setEnabled(!first);

    int curRecNo=selModel->currentIndex().row();
    QSqlRecord curRec=qryModel->record(curRecNo);//获取当前记录
    int empNo=curRec.value("EmpNo").toInt();//主键字段
    QSqlQuery query;
    query.prepare("SELECT EmpNo,Memo,Photo FROM employee WHERE empNo = :ID");
    query.bindValue(":ID",empNo);
    query.exec();
    query.first();

    QVariant va=query.value("Photo");
    if(!va.isValid()){
        ui->dbLabPhoto->clear();
    }
    else{
        QByteArray data=va.toByteArray();
        QPixmap pic;
        pic.loadFromData(data);
        ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->size().width()));
    }
    QVariant va2=query.value("Memo");
    ui->dbEditMemo->setPlainText(va2.toString());
}

void MainWindow::refreshTableView(){
    int index=dataMapper->currentIndex();//获取当前行号
    QModelIndex curIndex=qryModel->index(index,1);//为当前行创建模型索引
    selModel->clearSelection();
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置当前行
}

void MainWindow::on_actRecFirst_triggered()
{
    dataMapper->toFirst();
    refreshTableView();
}


void MainWindow::on_actRecLast_triggered()
{
    dataMapper->toLast();
    refreshTableView();
}


void MainWindow::on_actRecPrevious_triggered()
{
    dataMapper->toPrevious();
    refreshTableView();
}


void MainWindow::on_actRecNext_triggered()
{
    dataMapper->toNext();
    refreshTableView();
}

