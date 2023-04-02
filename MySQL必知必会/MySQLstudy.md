# MySQLstudy

## 使用MySQL
获取帮助
```sql
help;--获取帮助
help show;--获取使用show语句的帮助
```
登录到本地
```sql
mysql -u root -p
```
使用use关键字选择数据库
```sql
use mydb;--使用mydb数据库
```
获取数据库信息
```sql
show databases;
```
获取数据库内表的列表
```sql
show tables;
```
使用show来显示表列
```sql
show columns from tablename;
describe tablename;--MySQL only
```
其余show语句如下:
```sql
show status;--显示服务器状态信息
show create database databasename;--显示创建特定数据库的MySQL语句
show create table tablename;--显示创建特定表的MySQL语句
show grants;--显示授予用户的安全权限
show errors;--显示服务器错误消息
show warnings;--显示服务器警告消息
```

## 检索数据
使用select检索单个列
```sql
--从websites表中检索名为name的列
select name 
from websites;
```
使用select检索多个列
```sql
--从websites表中检索名为name、id、url的列
select id,name,url 
from websites;
```
使用select检索所有列
```sql
--从websites表中检索所有列
select * 
from websites;
```
使用select检索不同的行
```sql
--检索country列中不同的值
select distinct country 
from websites;
```
使用limit子句限制检索的行数列数
```sql
--限定MySQL返回不多于5行
select id 
from websites
limit 3;
--指定MySQL返回从行2开始的3行
select id 
from websites
limit 2,3;
--MySQL5 only,功能同上
select id
from websites
limit 3 offset 2;
```
使用完全限定的表名进行检索
```sql
--使用完全限定的表名
select websites.id 
from websites;
--使用完全限定的表名和数据库名
select websites.id 
from mydb.websites;
```

## 排序检索数据
使用order by子句进行排序
```sql
--使url列以字母排序
select url 
from websites
order by url;
```
按多个列进行排序
```sql
--选择url、alexa、sal列并按照alexa、sal列依次进行排序
select url,alexa,sal 
from websites
order by alexa,sal;
```
使用desc关键字进行降序排序
```sql
--选择url、alexa、sal列并按照sal列进行降序排列
select url,alexa,sal 
from websites
order by sal desc;
--选择url、alexa、sal列并先按照sal列进行降序排列再按照alexa列进行升序排列
select url,alexa,sal 
from websites
order by sal desc,alexa;
```
利用order by子句和limit子句进行筛选
```sql
--选出sal列中值最大的一项
select sal 
from websites
order by sal desc
limit 1;
```

## 过滤数据
使用where子句指定搜索条件
```sql
--选择url、id列，并指定返回id=2的行
select url,id 
from websites
where id=2;
```
使用where子句检查单个值
```sql
--选择url、id列并过滤掉id大于等于4的行
select url,id 
from websites
where id<4;
```
使用where子句进行不匹配检查
```sql
--选择url、id列并过滤掉id不等于3的行
select url,id 
from websites
where id<>3;
```
使用between关键字进行范围值检查
```sql
--选择url、id列，并返回id为1到3的数据
select url,id 
from websites
where id between 1 and 3;
```
使用is null子句进行空值检查
```sql
--选中url、id列，并返回id为null的行
select url,id 
from websites
where id is null;
```

## 数据过滤
使用and操作符使用多个过滤条件
```sql
--选中url、id列，并返回国家为中国且sal小于等于50的行
select url,id 
from websites
where country='CN' and sal<=50;
```
使用or操作符检索满足任一条件的行
```sql
--选中url、id列，并返回满足sal等于50或500的行
select url,id 
from websites
where sal=50 or sal=500;
```
同时使用and和or操作符，并使用圆括号保证优先级
```sql
--选中url、id列，并返回满足id等于1或2且alexa小于等于10的行
select url,id 
from websites
where (id=1 or id=2) and alexa<=10;
```
使用in操作符指定条件范围
```sql
--选中name、url列并返回按照name排序id为3和5的行
select name,url 
from websites
where id in (3,5)
order by name;
```
使用not操作符不匹配条件
```sql
--选中name、url列并返回按照name排序id不为2和4的行
select name,url 
from websites
where id not in (2,4)
order by name;
```

## 使用通配符进行过滤
- %通配符可以匹配0或多个字符，但是不能匹配NULL
- _通配符只能匹配一个字符


使用like操作符和%通配符进行过滤
```sql
--选中name、url列，并返回以url以https开头的行
select name,url 
from websites
where url like 'https%';
```
使用多个通配符
```sql
--选中name、url列并返回url中包含google的行
select name,url 
from websites
where url like '%google%';
```
使用like操作符和_通配符进行过滤
```sql
--选中所有列并返回满足name中任意两个字符后接ogle的行
select *
from websites
where name like __ogle;
```
使用通配符有以下几点注意事项:
- 不要过度使用通配符，如果其他操作符也能达到目的，那么应该使用其他操作符
- 除非必要，不然不要把通配符用在搜索模式的开始处，这样的效率最慢


## 用正则表达式进行搜索

使用正则表达式进行基础字符匹配
```sql
--选中url、id列，并返回按id排序的url中包含google的行
select url,id 
from websites
where url regexp 'google'
order by id;
--选中url、id列，并返回按id排序的url中包含任一字符+oogle的行
select url,id 
from websites
where url regexp '.oogle'
order by id;
```
使用|进行or匹配
```sql
--选中url、id列，并返回按id排序的url中包含com或or的行
select url,id 
from websites
where url regexp 'com|cn'
order by id;
```
使用[]匹配几个字符之一
```sql
--选中url、id列，并返回按id排序的url中包含cn或cm的行
select url,id 
from websites
where url regexp 'c[nm]'
order by id;
```
使用^和[]不匹配几个字符之一
```sql
--选中url、id列，并返回按id排序的url中不包含cn或cm的行
select url,id 
from websites
where url regexp 'c[^nm]'
order by id;
```
使用[]匹配一个范围
```sql
--选中url、id列，并返回按id排序的id中包含1-9的行
select url,id 
from websites
where id regexp '[1-9]'
order by id;
```
使用\ \转义来匹配特殊字符，如.、[]、-、|、\
```sql
--选中url、id列，并返回按id排序的url中包含.的行
select url,id 
from websites
where url regexp '\\.'
order by id;
```
使用预定义的字符类进行匹配
- [:alnum:] 匹配任意字母和数字
- [:alpha:] 匹配任意字符
- [:blank:] 匹配空格和制表符
- [:cntrl:] 匹配控制字符
- [:digit:] 匹配任意数字
- [:graph:] 匹配任意可打印字符除空格
- [:lower:] 匹配任意小写字母
- [:print:] 匹配任意可打印字符
- [:punct:] 匹配任意不在[:alnum:]和[:cntrl:]中的字符
- [:space:] 匹配包括空格在内的任意空白字符
- [:upper:] 匹配任意大写字母
- [:xdigit:] 匹配任意十六进制数字

使用重复元字符?进行可选的匹配
```sql
--选中url、id列，并返回按id排序的url中包含http(s可选)的行
select url,id 
from websites
where url regexp 'https?'
order by id;
```
使用重复元字符{n}进行指定数目的匹配
```sql
--选中url、id、sal列，并返回sal中按id排序的包含连续四个数字的行
select url,id,sal 
from websites
where sal regexp '[[:digit:]]{4}'
order by id;
```
其余的重复元字符如下:
- \* 匹配0个或多个匹配
- \+ 匹配一个或多个匹配
- {n,} 匹配不少于指定数目的匹配
- {n,m} 匹配数目的范围

使用定位元字符^进行从文本开始的匹配
```sql
--选中url、id、sal列，并返回sal中按id排序的以一个数开头的行
select url,id,sal
from websites
where sal regexp '^[0-9]\\.'
order by id;
```

其余的定位元字符如下:
- $ 定位文本的末尾
- \[[:<:]] 定位词的开始
- \[[:>:]] 定位词的结尾

## 创建计算字段

