#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QDate>
#include    <QTableWidgetItem>
#include    <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    labCellIndex=new QLabel("当前单元格坐标: ",this);
    labCellIndex->setMinimumWidth(250);
    labCellType=new QLabel("当前单元格类型: ",this);
    labCellType->setMinimumWidth(250);
    labStudID=new QLabel("学生ID: ",this);
    labStudID->setMinimumWidth(250);
    ui->statusbar->addWidget(labCellIndex);
    ui->statusbar->addWidget(labCellType);
    ui->statusbar->addWidget(labStudID);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnSetHeader_clicked()
{
    QStringList headerText;
    headerText<<"姓名"<<"性别"<<"出生日期"<<"民族"<<"分数"<<"是否党员";
    //ui->tableInfo->setHorizontalHeaderLabels(headerText);//只设置标题
    ui->tableInfo->setColumnCount(headerText.size());//设置表格列数
    for(int i=0;i<ui->tableInfo->columnCount();++i){
        QTableWidgetItem *headerItem=new QTableWidgetItem(headerText.at(i));//创建item
        QFont font=headerItem->font();
        font.setBold(true);
        font.setPointSize(11);//设置字体大小
        headerItem->setForeground(QBrush(Qt::red));//设置前景色为红色
        headerItem->setFont(font);
        ui->tableInfo->setHorizontalHeaderItem(i,headerItem);//设置表头单元格的item
    }
}


void MainWindow::on_btnIniData_clicked()
{
    QDate birth(2001,4,6);//初始化日期
    ui->tableInfo->clearContents();//清除表格内的数据，不清除表头
    for(int i=0;i<ui->tableInfo->rowCount();++i){
        QString strName=QString("学生%1").arg(i);
        QString strSex=((i%2)==0)?"男":"女";
        bool isParty=((i%2)==0)?false:true;
        int score=QRandomGenerator::global()->bounded(60,100);
        createItemsARow(i,strName,strSex,birth,"汉族",isParty,score);//为某一行创建item
        birth=birth.addDays(20);//日期加20天
    }
}


void MainWindow::createItemsARow(int rowNo, QString name, QString sex, QDate birth, QString nation, bool isPM, int score){
    uint studID=202105000;//学号奇数
    //姓名
    QTableWidgetItem *item=new QTableWidgetItem(name,MainWindow::ctName);//这里的枚举值是单元格的类型
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    studID+=rowNo;//学号=基数+行号
    item->setData(Qt::UserRole,QVariant(studID));//将学号设置为用户数据
    ui->tableInfo->setItem(rowNo,MainWindow::colName,item);//为单元格设置item，这里的枚举值相当于列号
    //性别
    QIcon icon;
    if(sex=="男"){
        icon.addFile(":/images/icons/boy.ico");
    }
    else{
        icon.addFile(":/images/icons/girl.ico");
    }
    item =new QTableWidgetItem(sex,MainWindow::ctSex);
    item->setIcon(icon);
    item->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    Qt::ItemFlags flags=Qt::ItemIsSelectable|Qt::ItemIsEnabled;//性别单元格不允许被编辑
    item->setFlags(flags);
    ui->tableInfo->setItem(rowNo,MainWindow::colSex,item);
    //出生日期
    QString str=birth.toString("yyyy-MM-dd");
    item=new QTableWidgetItem(str,MainWindow::ctBirth);
    item->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colBirth,item);
    //民族
    item=new QTableWidgetItem(nation,MainWindow::ctNation);
    item->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colNation,item);
    //是否党员
    item=new QTableWidgetItem("党员",MainWindow::ctPartyM);
    item->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    flags=Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled;//党员单元格不允许被编辑，但可以更改复选状态
    item->setFlags(flags);
    if(isPM){
        item->setCheckState(Qt::Checked);
    }
    else{
        item->setCheckState(Qt::Unchecked);
    }
    item->setBackground(QBrush(Qt::yellow));//设置背景色
    ui->tableInfo->setItem(rowNo,MainWindow::colPartyM,item);
    //分数
    str.setNum(score);
    item=new QTableWidgetItem(str,MainWindow::ctScore);
    item->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colScore,item);
}


void MainWindow::on_tableInfo_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    QTableWidgetItem *item=ui->tableInfo->item(currentRow,currentColumn);//获取当前单元格
    if(item==nullptr){
        return ;
    }
    labCellIndex->setText(QString::asprintf("当前单元格坐标: %d 行, %d 列",currentRow,currentColumn));
    int cellType=item->type();//获取单元格的类型
    labCellType->setText(QString::asprintf("当前单元格类型: %d",cellType));

    item=ui->tableInfo->item(currentRow,MainWindow::colName);//获取包含Name信息的单元格
    uint ID=item->data(Qt::UserRole).toUInt();//将其中存储的用户数据转换为uint类型
    labStudID->setText(QString::asprintf("学生ID: %d",ID));//设置为学生ID
}


void MainWindow::on_btnInsertRow_clicked()
{
    int curRow=ui->tableInfo->currentRow();//获取当前的行号
    ui->tableInfo->insertRow(curRow);//插入一行但是不自动为单元格创建item
    createItemsARow(curRow,"新学生","男",QDate::fromString("2002-10-1","yyyy-M-d"),"苗族",true,80);
}


void MainWindow::on_btnAppendRow_clicked()
{
    int curRow=ui->tableInfo->rowCount();
    ui->tableInfo->insertRow(curRow);//在表格末尾添加一行
    createItemsARow(curRow,"新生","女",QDate::fromString("2002-6-5","yyyy-M-d"),"满族",false,76);
}


void MainWindow::on_btnDelCurRow_clicked()
{
    int curRow=ui->tableInfo->currentRow();
    ui->tableInfo->removeRow(curRow);//删除当前行
}


void MainWindow::on_chkBoxTabEditable_clicked(bool checked)//设置是否可编辑和进入编辑模式的方式
{
    if(checked){
        ui->tableInfo->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);//允许编辑，双击或获取焦点后点击可进入编辑状态
    }
    else{
        ui->tableInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);//不允许编辑
    }
}


void MainWindow::on_chkBoxHeaderH_clicked(bool checked)
{
    ui->tableInfo->horizontalHeader()->setVisible(checked);
}


void MainWindow::on_chkBoxHeaderV_clicked(bool checked)
{
    ui->tableInfo->verticalHeader()->setVisible(checked);
}


void MainWindow::on_chkBoxRowColor_clicked(bool checked)
{
    ui->tableInfo->setAlternatingRowColors(checked);//设置行的底色交替采用不同颜色
}


void MainWindow::on_rBtnSelectRow_clicked()
{
    ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectRows);//行选择
}


void MainWindow::on_rBtnSelectItem_clicked()
{
    ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectItems);//单元格选择
}


void MainWindow::on_btnReadtoEdit_clicked()
{
    QTableWidgetItem *item;
    ui->textEdit->clear();
    for(int i=0;i<ui->tableInfo->rowCount();++i){
        QString str=QString::asprintf("第 %d 行: ",i+1);
        for(int j=0;j<ui->tableInfo->columnCount()-1;++j){
            item=ui->tableInfo->item(i,j);//获取当前行除最后一列的所有文本
            str=str+item->text()+"   ";
        }
        item=ui->tableInfo->item(i,colPartyM);//获取当前行最后一列党员的item
        if(item->checkState()==Qt::Checked){//根据复选状态确定增添的文本
            str=str+"党员";
        }
        else{
            str=str+"群众";
        }
        ui->textEdit->appendPlainText(str);
    }
}


void MainWindow::on_btnAutoHeight_clicked()
{
    ui->tableInfo->resizeRowsToContents();//自动调节行高
}


void MainWindow::on_btnAutoWidth_clicked()
{
    ui->tableInfo->resizeColumnsToContents();//自动调节行宽
}


void MainWindow::on_btnSetRows_clicked()
{
    ui->tableInfo->setRowCount(ui->spinRowCount->value());//设置行数
    ui->tableInfo->setAlternatingRowColors(ui->chkBoxRowColor->isChecked());//设置是否交替背景色
}

