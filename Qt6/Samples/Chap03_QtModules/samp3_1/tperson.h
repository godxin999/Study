#ifndef TPERSON_H
#define TPERSON_H

#include <QObject>

class Tperson : public QObject
{
    Q_OBJECT//使Tperson获得元对象系统支持，可以使用槽与信号、属性等功能
    Q_CLASSINFO("author","Zhang")//定义附加类信息
    Q_CLASSINFO("version","1.0.0")
    Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)//定义属性age，READ和WRITE关键字表示可以通过接口获取该属性
    Q_PROPERTY(QString name MEMBER m_name)//定义属性name，MEMBER关键字表示只能通过property函数读取，setProperty函数修改
    Q_PROPERTY(int score MEMBER m_score)//定义属性score
private:
    int m_age=10;
    int m_score=79;
    QString m_name;
public:
    explicit Tperson(QString aName,QObject *parent = nullptr);
    ~Tperson();
    int age();
    void setAge(int value);
    void incAge();
signals:
    void ageChanged(int value);
public slots:
};

#endif // TPERSON_H
