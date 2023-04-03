#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QLabel>
#include <QTreeWidgetItem>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    enum TreeItemType{itTopItem=1001,itGroupItem,itImageItem};//创建节点的时候用作type的参数，自定义节点类型数据必须大于1000
    enum TreeColNum{colItem=0,colItemType,colDate};//目录树列的序号
    QLabel *labFileName;//用于状态栏显示文件名
    QLabel *labNodeText;//用于状态栏显示节点标题
    QSpinBox *spinRatio;//用于状态栏显示图片缩放比例
    QPixmap m_pixmap;//当前的图片
    float m_ratio;//当前图片缩放比率
    void buildTreeHeader();//构建目录树表头
    void iniTree();//初始化目录树
    void addFolderItem(QTreeWidgetItem *parItem,QString dirName);//添加目录节点
    QString getFinalFolderName(const QString &fullPathName);//提取目录名称
    void addImageItem(QTreeWidgetItem *parItem,QString aFilename);//添加图片节点
    void displayImage(QTreeWidgetItem *item);//显示一个图片节点关联的图片
    void changeItemCaption(QTreeWidgetItem *item);//遍历改变节点标题
    void deleteItem(QTreeWidgetItem *parItem,QTreeWidgetItem *item);//删除一个节点
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actAddFolder_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
