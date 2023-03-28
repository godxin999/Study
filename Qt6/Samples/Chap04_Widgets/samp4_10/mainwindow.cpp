#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    buildUI();//动态创建界面组件
    buildSignalSlots();
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->setCentralWidget(ui->TextEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildUI(){
    labelFile=new QLabel(this);//用于显示当前文件名
    labelFile->setMinimumWidth((150));
    labelFile->setText("文件名: ");
    ui->statusbar->addWidget(labelFile);//添加到状态栏，addWidget是添加到状态栏左侧

    progressbar1 =new QProgressBar;//状态栏上的进度条
    progressbar1->setMaximumWidth(200);
    progressbar1->setMinimum(5);
    progressbar1->setMaximum(50);
    progressbar1->setValue(ui->TextEdit->font().pointSize());//将进度条的值设置为字体大小
    ui->statusbar->addWidget(progressbar1);//添加到状态栏

    labelInfo=new QLabel(this);//用于显式字体名称
    labelInfo->setText("字体名称: ");
    ui->statusbar->addPermanentWidget(labelInfo);//添加到状态栏，addPermanentWidget是添加到状态栏右侧

    actionGroup=new QActionGroup(this);//实现中文英文的互斥选择
    actionGroup->addAction(ui->actLang_CN);
    actionGroup->addAction(ui->actLang_EN);
    actionGroup->setExclusive(true);//互斥选择
    ui->actLang_CN->setChecked(true);

    spinFontSize=new QSpinBox(this);//设置字体大小的spinbox
    spinFontSize->setMinimum(5);
    spinFontSize->setMaximum(50);
    spinFontSize->setValue(ui->TextEdit->font().pointSize());
    spinFontSize->setMinimumWidth(50);
    ui->mainToolBar->addWidget(spinFontSize);//添加到工具栏

    comboFontName=new QFontComboBox(this);//字体下拉列表
    comboFontName->setMinimumWidth(150);
    ui->mainToolBar->addWidget(comboFontName);//添加到工具栏
    ui->mainToolBar->addSeparator();//添加分隔条
    ui->mainToolBar->addAction(ui->actClose);//添加退出按钮
}

void MainWindow::do_fontSize_changed(int FontSize){
    QTextCharFormat fmt=ui->TextEdit->currentCharFormat();//获取当前格式
    fmt.setFontPointSize(FontSize);//设置新的字体大小
    ui->TextEdit->mergeCurrentCharFormat(fmt);//合并格式
    progressbar1->setValue(FontSize);
}

void MainWindow::do_fontSelected(const QFont &font){
    labelInfo->setText("字体名称: "+font.family());
    QTextCharFormat fmt=ui->TextEdit->currentCharFormat();
    fmt.setFont(font);//设置字体
    ui->TextEdit->mergeCurrentCharFormat(fmt);//合并格式
}

void MainWindow::on_actFont_Bold_triggered(bool checked)
{
    QTextCharFormat fmt=ui->TextEdit->currentCharFormat();
    if(checked){
        fmt.setFontWeight(QFont::Bold);//粗体模式
    }
    else{
        fmt.setFontWeight(QFont::Normal);//标准格式
    }
    ui->TextEdit->mergeCurrentCharFormat(fmt);
}


void MainWindow::on_actFont_Italic_triggered(bool checked)
{
    QTextCharFormat fmt=ui->TextEdit->currentCharFormat();
    fmt.setFontItalic(checked);//设置斜体
    ui->TextEdit->mergeCurrentCharFormat(fmt);
}


void MainWindow::on_actFont_Underline_triggered(bool checked)
{
    QTextCharFormat fmt=ui->TextEdit->currentCharFormat();
    fmt.setFontUnderline(checked);//设置下划线
    ui->TextEdit->mergeCurrentCharFormat(fmt);
}


void MainWindow::on_actFile_Save_triggered()
{
    ui->TextEdit->document()->setModified(false);//如果文件被保存，那么就设置是否修改选项为否
    labelFile->setText("文件已保存");
}


void MainWindow::on_actSys_ToggleText_triggered(bool checked)
{
    if(checked){
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//设置显示按钮标签在按钮下方
    }
    else{
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);//设置只显示按钮
    }
}

void MainWindow::buildSignalSlots(){
    connect(spinFontSize,SIGNAL(valueChanged(int)),this,SLOT(do_fontSize_changed(int)));
    connect(comboFontName,&QFontComboBox::currentFontChanged,this,&MainWindow::do_fontSelected);
}

void MainWindow::on_actFile_New_triggered()
{
    ui->TextEdit->clear();
    ui->TextEdit->document()->setModified(false);//如果新建文件，那么就设置是否修改选项为否
    labelFile->setText("新建了一个文件");
}


void MainWindow::on_actFile_Open_triggered()
{
    labelFile->setText("打开的文件");
}


void MainWindow::on_TextEdit_selectionChanged()
{
    QTextCharFormat fmt=ui->TextEdit->currentCharFormat();//获取选中的文本的格式
    ui->actFont_Bold->setChecked(fmt.font().bold());//根据格式设置粗体、斜体、下划线的选中状态
    ui->actFont_Italic->setChecked(fmt.fontItalic());
    ui->actFont_Underline->setChecked(fmt.fontUnderline());
}


void MainWindow::on_TextEdit_copyAvailable(bool b)
{
    ui->actEdit_Cut->setEnabled(b);//根据能否复制的信号来更新三个Actions的使能状态
    ui->actEdit_Copy->setEnabled(b);
    ui->actEdit_Paste->setEnabled(ui->TextEdit->canPaste());
}

