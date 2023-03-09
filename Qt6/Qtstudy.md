# Qtstudy

### 3.4 容器类
#### 3.4.1 顺序容器类
Qt的顺序容器有QList、QVector、QStack和QQueue，其中QVector就是QList。
1. QList类

```C++
QList<float> list;
QList<int>={1,2,3,4,5};
QList<int> list(100);
QList<QString> strList(10,"pass");
```
QList向列表末端添加数据操作较快，可以使用<<运算符或者函数append()来添加数据，并可以通过[]和at函数来访问元素
```C++
QList<QString> list;
list<<"Monday"<<"Tuesday";
list.append("Wednesday");
QString str1=list[1];
QString str2=list.at(2);
```
常用函数如下：
- append()
- prepend()：向列表始端添加元素
- insert()：在某个位置插入元素
- replace()：替换某个位置的元素
- at()
- clear():清除列表中的所有元素，元素个数变为0
- size()
- count():统计某个数据出现的次数
- resize()
- reverse()
- isEmpty()
- remove()、removeAt()、removeAll()、removeFirst()、removeLast()：从列表移除数据
- takeAt()、takeFirst()、takeLast()：从列表移除数据，并返回被移除的数据

Qt中很多函数的参数或者返回值是QList列表，比如QObject::findChildren()函数的返回值，有时还会把某种QList用typedef定义成一种等效的类型，比如
```C++
typedef QList<QObject*> QObjectList;
```
函数QObject::children()函数的返回值就是QObjectList。

2. QStack类

QStack是QList的子类，push()和pop()为其主要的接口函数
```C++
QStack<int> stack;
stack.push(10);
stack.push(20);
while(!stack.isEmpty()){
    cout<<stack.pop()<<Qt::endl;
}
```
3. QQueue类

QQueue是QList的子类，enqueue()和dequeue是其主要的操作函数
```C++
QQueue<int> queue;
queue.enqueue(10);
queue.enqueue(20);
while(!queue.isEmpty()){
    cout<<queue.dequeue()<<Qt::endl;
}
```
#### 3.4.2 关联容器类
Qt提供的关联容器有QSet、QMap、QMultiMap、QHash、QMultiHash。QHash类和QMultiHash类使用哈希函数进行查找，速度更快。
1. Qset类

Qset是基于哈希表的集合模板类，其内部由QHash实现。QSet输入数据的操作如下：
```C++
QSet<QString> set;
set<<"dog"<<"cat";
```
QSet测试一个数据是否在集合内的操作如下：
```C++
if(!set.contains("cat")){
    //...
}
```
2. QMap类

QMap<key,T>定义字典(关联数组)，一个键映射到一个值。QMap按照键的顺序存储数据，如果不在意存储顺序，使用QHash更快。QMap的定义以及赋值操作如下：
```C++
QMap<QString,int> map;
map["one"]=1;
map["two"]=2;
map["three"]=3;
```
可以使用insert()函数赋值，也可以使用remove()函数移除键值对
```C++
map.insert("four",4);
map.remove("two");
```
要查找值可以使用运算符[]或者value()函数
```C++
int num1=map["one"];
int num2=map.value("two");
```
如果在映射表中没有找到指定的键，会返回默认的构造值，在使用value()函数查找值的时候，可以指定一个默认的返回值
```C++
int timeout=map.value("TIMEOUT",30);
```
如果value()函数找到了键"TIMEOUT"，就返回关联的值，否则返回30。
3. QMultiMap类

QMultiMap<key,T>定义一个多值映射表，一个键可以对应多个值，其基本操作如下：
```C++
QMultiMap<QString,int> mp1,mp2,mp3;
mp1.insert("plenty",100);
mp1.insert("plenty",200);//mp1.size()=2
mp2.insert("plenty",500);//mp2.size()=1
mp3=mp1+mp2;//mp3.size()=3
```
QMultiMap不提供[]运算符，可以使用value()函数访问最新插入键的单个值，如果要获取一个键的所有值，可以使用values()函数来获取，该函数的返回值是QList&lt;T&gt;类型。
```C++
QList<int> values=mp1.values("plenty");
for(int i=0;i<values.size();++i){
    cout<<values.at(i)<<Qt::endl;
}
```
4. QHash类

QHash基于哈希表实现，查找QHash<key,T>存储的键值对的速度非常快。QHash和QMap的功能和用法相似，区别如下：
- QHash比QMap的查找速度快
- QMap按照键的顺序存储数据，QHash的数据是任意排序的
- QMap的键必须提供<运算符，QHash的键必须提供==运算符和一个名为QHash()的全局哈希函数

#### 3.4.3 遍历容器的数据
1. 顺序容器类的迭代器用法

```C++
QList<QString> list;
List<<"A"<<"B"<<"C"<<"D";
QList<QString>::const_iterator i;
for(i=list.constBegin();i!=list.constEnd();++i){
    qDebug()<<*i;
}
```
函数constBegin()和constEnd()用于只读迭代器，若要反向读写迭代器并将list的数据全改为小写，代码如下：
```C++
QList<QString>::reverse_iterator i;
for(i=list.rbegin();i!=list.rend();++i){
    *i=i->toLower();
}
```
函数rbegin()和rend()用于反向迭代器，迭代器变量i是一个指向QString的指针，所以我们可以使用QString的接口函数toLower()。
2. 关联容器类的迭代器用法

对于关联容器类QMap和QHash，解引用符*返回数据项的值，如果要返回键，则需要使用key()函数，对应地，可以使用value()函数返回一个项的值。
```C++
QMap<int,int> mp;
QMap<int,int>::const_iterator i;
for(int i=mp.constBegin();i!=mp.constEnd();++i){
    qDebug()<<i.key()<<":"<<i.value();
}
```
Qt中很多函数的返回值是QList或QStringList类型，要遍历这些返回的容器类，必须先复制，因为Qt使用隐式共享(不拷贝对象数据，仅传递给使用者一个该对象的指针，只有修改数据时才会实际复制共享对象给使用者)，所以不会产生较大开销。
```C++
const QList<int> sizes=splitter->sizes();
QList<int>::const_iterator i;
for(i=sizes.begin();i!=sizes.end();++i){
    //...
}
```
3. 使用foreach遍历容器数据

如果只想遍历容器中的所有项，可以使用宏foreach，这是&lt;QtGlobal&gt;头文件中定义的一个宏，其用法如下：
```C++
foreach(variable,container)//variable的类型必须是容器container的元素类型
```
使用foreach的代码比使用迭代器更简洁，使用foreach遍历QList&lt;QString&gt;的代码如下：
```C++
QList<QString> list;
QString str;
foreach(str,list){
    qDebug()<<str;
}
```
用于迭代的变量也可以在foreach语句中定义，还可以使用break退出迭代。
```C++
QList<QString> list;
foreach(const QString str,list){
    if(str.isEmpty()){
        break;
    }
    qDebug()<<str;
}
```
对于关联容器会自动访问键值对中的值，所以不需要调用values()函数，如果需要访问键则可以使用keys()函数。
```C++
QMap<QString,int> mp;
foreach(const Qstring str,mp.keys()){
    qDebug()<<str<<":"<<mp.value(str);
}
```
对于多值映射，可以使用两重foreach语句。
```C++
QMultiMap<QString,int>mp;
foreach(const QString str,mp.uniqueKeys()){
    foreach(int i,mp.values(str)){
        qDebug()<<str<<":"<<i;
    }
}
```
### 3.5 常用基础类
#### 3.5.1 QVariant类
QVariant是Qt中的一种万能数据类型，它可以存储任何类型的数据。Qt类库中有很多函数的返回值是QVariant类型的，比如QObject::property()函数，其定义如下：
```C++
QVariant QObject::property(const char *name)
```
property()函数通过属性名称返回属性值，而各种属性的数据类型是不同的，所以需要QVariant作为这个函数的返回值类型。
一个QVariant变量在任何时间都只能存储一个值，可以利用toBool()、toDouble()、toStringList()等函数将数据转换为具体类型的数据，还可以使用value()函数返回某种类型的数据。
QVariant变量可以在定义时通过构造函数来赋初值，也可以使用setValue()函数赋值。
```C++
QVariant var(173);//使用构造函数赋初值
QString str=var.toString();//转换为字符串，str="173"
int val=var.value<int>();//使用value()函数获取int类型的数据
QStringList strList;
strList<<"one"<<"two"<<"three";
var.setValue(strList);//使用setValue()函数赋值
QStringList value=var.toStringList();//使用toT()函数转换
```
对于Qt GUI模块中的一些类型，QVariant没有相对应的toT()函数，比如没有toColor()、toFont()，但是这些类型的值可以赋值给QVariant变量，之后通过QVariant::value()函数来得到指定类型的值。
```C++
QFont font=this->font();//获取窗口的字体
QVariant var=font;//赋值给QVariant变量
QFont font2=var.value<QFont>();//转换为QFont类型
```
#### 3.5.2 QFlags类
QFlags&lt;Enum&gt;是一个模板类，其中Enum是枚举类型，常用于定义枚举值的或运算组合，比如QLabel的alignment属性。Qt::Alignment是QFlags&lt;Qt::AlignmentFlag&gt;类型，因为alignment属性值是一个或者多个Qt::AlignmentFlag类型枚举值的组合，所以我们称Qt::Alignment为枚举类型Qt::AlignmentFlag的标志类型。
给窗口上的QLabel组件设置对其方式可以使用如下代码：
```C++
ui->label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
```
QFlags类支持位运算，所以有如下代码
```C++
QFlags<Qt::AlignmentFlag> flags=ui->label->alignment();//获取属性值
flags=flags|Qt::AlignVCenter;//增加垂直对其属性
ui->label->setAlignment(flags);//设置alignment属性值
```
我们还可以使用testFlag()函数来测试某个枚举值是否在此标志变量中，比如
```C++
QFlags<Qt::AlignmentFlag> flags=ui->label->alignment();
bool isLeft=flags.testFlag(Qt::AlignLeft);//测试是否含有AlignLeft属性
```
#### QRandomGenerator类



