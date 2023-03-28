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

