#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>

void MainWindow::setActionsForButton(){
    //关联每个QToolButton和对应的Action
    ui->tBtnListIni->setDefaultAction(ui->actListini);//初始化列表
    ui->tBtnListClear->setDefaultAction(ui->actListClear);//清除列表
    ui->tBtnListInsert->setDefaultAction(ui->actListInsert);//插入项
    ui->tBtnListAppend->setDefaultAction(ui->actListAppend);//添加项
    ui->tBtnListDelete->setDefaultAction(ui->actListDelete);//删除当前项
    ui->tBtnSelAll->setDefaultAction(ui->actSelAll);//全选
    ui->tBtnSelNone->setDefaultAction(ui->actSelNone);//空选
    ui->tBtnSelInvs->setDefaultAction(ui->actSelInvs);//反选
}

void MainWindow::createSelectionMenu(){
    //创建下拉菜单
    QMenu *menuSelction =new QMenu(this);
    //添加三种Action，并创建菜单项
    menuSelction->addAction(ui->actSelAll);//全选
    menuSelction->addAction(ui->actSelNone);//空选
    menuSelction->addAction(ui->actSelInvs);//反选
    //ListWidget上方的"项选项"按钮
    ui->tBtnSelItem->setPopupMode(QToolButton::MenuButtonPopup);//菜单弹出模式
    ui->tBtnSelItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->tBtnSelItem->setDefaultAction(ui->actSelPopMenu);//关联Action
    ui->tBtnSelItem->setMenu(menuSelction);//设置下拉菜单
    //工具栏上的"项选项"按钮，具有下拉菜单
    QToolButton *aBtn=new QToolButton(this);
    aBtn->setPopupMode(QToolButton::InstantPopup);//立刻显示下拉菜单
    aBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    aBtn->setDefaultAction(ui->actSelPopMenu);//关联Action
    aBtn->setMenu(menuSelction);//设置下拉菜单
    ui->toolBar->addWidget(aBtn);//在工具栏上添加该按钮
    //在工具栏上添加分隔条和"退出"按钮
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actQuit);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->splitter);//设置中心界面
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);//设置目录属性
    setActionsForButton();
    createSelectionMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actListini_triggered()
{
    QListWidgetItem *aItem;
    QIcon aIcon;
    aIcon.addFile(":/icons/icons/check2.ico");//添加图标
    bool chk=ui->chkBoxListEditable->isChecked();//是否可编辑

    ui->listWidget->clear();
    for(int i=0;i<10;++i){
        QString str=QString("Item %1").arg(i);
        aItem=new QListWidgetItem();
        aItem->setText(str);
        aItem->setIcon(aIcon);//设置图标
        aItem->setCheckState(Qt::Checked);//设置为checked状态
        if(chk){//设置item的属性
            aItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        }
        else{
            aItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        }
        ui->listWidget->addItem(aItem);//添加一个项
    }
}

void MainWindow::on_actListClear_triggered()
{
    ui->listWidget->clear();
}


void MainWindow::on_actListInsert_triggered()
{
    QIcon aIcon(":/icons/icons/check2.ico");//定义并直接赋值
    bool chk=ui->chkBoxListEditable->isChecked();
    QListWidgetItem *aItem =new QListWidgetItem("Insert Item");
    aItem->setIcon(aIcon);
    aItem->setCheckState(Qt::Checked);
    if(chk){
        aItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    }
    else{
        aItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    }
    ui->listWidget->insertItem(ui->listWidget->currentRow(),aItem);//在当前行前插入一个项
}


void MainWindow::on_actListAppend_triggered()
{
    QIcon aIcon(":/icons/icons/check2.ico");
    bool chk=ui->chkBoxListEditable->isChecked();
    QListWidgetItem *aItem =new QListWidgetItem("Added Item");
    aItem->setIcon(aIcon);
    aItem->setCheckState(Qt::Checked);
    if(chk){
        aItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    }
    else{
        aItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    }
    ui->listWidget->addItem(aItem);//添加一个项
}


void MainWindow::on_actListDelete_triggered()
{
    int row=ui->listWidget->currentRow();//获取当前行号
    QListWidgetItem *aItem=ui->listWidget->takeItem(row);//删除指定的项
    delete aItem;
}


void MainWindow::on_actSelInvs_triggered()
{
    QListWidgetItem *aItem;
    int cnt=ui->listWidget->count();
    for(int i=0;i<cnt;++i){
        aItem=ui->listWidget->item(i);
        if(aItem->checkState()!=Qt::Checked){
            aItem->setCheckState(Qt::Checked);
        }
        else{
            aItem->setCheckState(Qt::Unchecked);
        }
    }
}


void MainWindow::on_actSelAll_triggered()
{
    QListWidgetItem *aItem;
    int cnt=ui->listWidget->count();
    for(int i=0;i<cnt;++i){
        aItem=ui->listWidget->item(i);
        aItem->setCheckState(Qt::Checked);
    }
}


void MainWindow::on_actSelNone_triggered()
{
    QListWidgetItem *aItem;
    int cnt=ui->listWidget->count();
    for(int i=0;i<cnt;++i){
        aItem=ui->listWidget->item(i);
        aItem->setCheckState(Qt::Unchecked);
    }
}


void MainWindow::on_chkBoxListEditable_clicked(bool checked)
{
    QListWidgetItem *aItem;
    int cnt=ui->listWidget->count();
    for(int i=0;i<cnt;++i){
        aItem=ui->listWidget->item(i);
        if(checked){
            aItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        }
        else{
            aItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        }
    }
}


void MainWindow::on_checkBox_2_clicked(bool checked)
{
    ui->listWidget->setSortingEnabled(checked);//如果可以排序，那么addItem()和insertItem()新添加的项将会添加到列表的最前面
}


void MainWindow::on_tBtnSortAsc_clicked()
{
    ui->listWidget->sortItems(Qt::AscendingOrder);//升序排序
}


void MainWindow::on_tBtnSortDes_clicked()
{
    ui->listWidget->sortItems(Qt::DescendingOrder);//降序排序
}


void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)//当前项改变的信号
{
    QString str;
    if(current!=nullptr){//检测指针是否为空
        if(previous==nullptr){
            str="当前: "+current->text();
        }
        else{
            str="前一项: "+previous->text()+";当前项: "+current->text();
        }
        ui->editCutItemText->setText(str);
    }
    ui->plainTextEdit->appendPlainText("currentItemChanged()信号被发射");
}


void MainWindow::on_listWidget_currentRowChanged(int currentRow)//当前行改变的信号
{
    ui->plainTextEdit->appendPlainText(QString("currentRowChanged()信号被发射,currentRow=%1").arg(currentRow));
}


void MainWindow::on_listWidget_currentTextChanged(const QString &currentText)//当前文本改变的信号
{
    ui->plainTextEdit->appendPlainText(QString("currentTextChanged()信号被发射,currentText=")+currentText);
}


void MainWindow::on_listWidget_itemChanged(QListWidgetItem *item)//项属性改变的信号
{
    ui->plainTextEdit->appendPlainText(QString("itemChanged()信号被发射")+item->text());
}


void MainWindow::on_listWidget_itemSelectionChanged()//选择项发生改变的信号
{
    ui->plainTextEdit->appendPlainText(QString("itemSelectionChanged()信号被发射"));
}


void MainWindow::on_tBtnClearText_clicked()
{
    ui->plainTextEdit->clear();
}


void MainWindow::on_tBtnAddLine_clicked()
{
    ui->plainTextEdit->appendPlainText("");
}


void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)//设置菜单属性为customContextMenu，然后用户编程实现创建并显示快捷菜单
{
    Q_UNUSED(pos);
    QMenu *menuList=new QMenu(this);//创建菜单
    menuList->addAction(ui->actListini);//加入所有Action
    menuList->addAction(ui->actListClear);
    menuList->addAction(ui->actListInsert);
    menuList->addAction(ui->actListAppend);
    menuList->addAction(ui->actListDelete);
    menuList->addSeparator();
    menuList->addAction(ui->actSelAll);
    menuList->addAction(ui->actSelNone);
    menuList->addAction(ui->actSelInvs);

    menuList->exec(QCursor::pos());//在光标处创建菜单
    delete menuList;//菜单显示过后进行删除
}

