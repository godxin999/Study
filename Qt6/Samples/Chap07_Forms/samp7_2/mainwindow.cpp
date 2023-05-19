#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_model=new QStandardItemModel(6,4,this);//创建数据模型
    QStringList header;
    header<<"姓名"<<"性别"<<"学位"<<"部门";
    m_model->setHorizontalHeaderLabels(header);
    m_selection=new QItemSelectionModel(m_model);//创建选择模型
    connect(m_selection,&QItemSelectionModel::currentChanged,this,&MainWindow::do_model_currentChanged);

    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionModel(m_selection);
    setCentralWidget(ui->tableView);
    //创建状态栏组件
    labCellPos=new QLabel("当前单元格: ",this);
    labCellPos->setMinimumWidth(180);
    labCellPos->setAlignment(Qt::AlignHCenter);
    ui->statusbar->addWidget(labCellPos);
    labCellText=new QLabel("单元格内容: ",this);
    labCellText->setMinimumWidth(180);
    ui->statusbar->addWidget(labCellText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_model_currentChanged(const QModelIndex &current, const QModelIndex &previous){
    Q_UNUSED(previous);
    if(current.isValid()){
        labCellPos->setText(QString::asprintf("当前单元格: %d行, %d列",current.row(),current.column()));
        QStandardItem *aItem;
        aItem=m_model->itemFromIndex(current);//从模型索引获取Item
        this->labCellText->setText("单元格内容: "+aItem->text());
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton result;
    result=QMessageBox::question(this,"确认","确定要退出本程序吗?");
    if(result==QMessageBox::Yes){
        event->accept();//退出
    }
    else{
        event->ignore();//不退出
    }
}

void MainWindow::on_actTab_SetSize_triggered()
{
    TDialogSize *dlgTableSize=new TDialogSize(this);//创建对话框
    dlgTableSize->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);//固定大小
    dlgTableSize->setRowColumn(m_model->rowCount(),m_model->columnCount());
    int ret=dlgTableSize->exec();//以模态方式显示对话框
    if(ret==QDialog::Accepted){//获取对话框上的输入，设置表格行数和列数
        int cols=dlgTableSize->columnCount();
        m_model->setColumnCount(cols);
        int rows=dlgTableSize->rowCount();
        m_model->setRowCount(rows);
    }
    delete dlgTableSize;//删除对话框
}


void MainWindow::on_actTab_SetHeader_triggered()
{
    if(dlgSetHeaders==nullptr){//如果对象没有被创建
        dlgSetHeaders=new TDialogHeaders(this);//创建对象
    }
    if(dlgSetHeaders->headerList().size()!=m_model->columnCount()){
        QStringList strList;
        for(int i=0;i<m_model->columnCount();++i){//获取现有的表头标题
            strList.append(m_model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
        }
        dlgSetHeaders->setHeaderList(strList);//用于对话框初始化显示
    }
    int ret=dlgSetHeaders->exec();//以模态方式显示对话框
    if(ret==QDialog::Accepted){
        QStringList strList=dlgSetHeaders->headerList();//获取修改后的字符串列表
        m_model->setHorizontalHeaderLabels(strList);//设置模型的表头标题
    }
}

void MainWindow::do_setCellText(int row, int col, QString &text){
    QModelIndex index=m_model->index(row,col);//获取模型索引
    m_selection->clearSelection();//清除现有选择
    m_selection->setCurrentIndex(index,QItemSelectionModel::Select);//定位到单元格
    m_model->setData(index,text,Qt::DisplayRole);//设置单元格文字
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    emit cellIndexChanged(index.row(),index.column());
}


void MainWindow::on_actTab_Locate_triggered()
{
    TDialogLocate *dlgLocate=new TDialogLocate(this);
    dlgLocate->setAttribute(Qt::WA_DeleteOnClose);//对话框关闭时自动删除
    dlgLocate->setWindowFlag(Qt::WindowStaysOnTopHint);//设置对话框特性StayOnTop
    dlgLocate->setSpinRange(m_model->rowCount(),m_model->columnCount());
    QModelIndex curIndex=m_selection->currentIndex();
    if(curIndex.isValid()){
        dlgLocate->setSpinValue(curIndex.row(),curIndex.column());
    }
    connect(dlgLocate,&TDialogLocate::changeCellText,this,&MainWindow::do_setCellText);//对话框发射changeCellText()信号，用于定位单元格并设置文字
    connect(dlgLocate,&TDialogLocate::changeActionEnable,ui->actTab_Locate,&QAction::setEnabled);//对话框发射changeActionEnable()信号，用于设置action的enabled属性
    connect(this,&MainWindow::cellIndexChanged,dlgLocate,&TDialogLocate::setSpinValue);//主窗口发射cellIndexChanged()信号，用于修改对话框上的spinBox的值

    dlgLocate->setModal(false);
    dlgLocate->show();//以非模态方式显示对话框
}

