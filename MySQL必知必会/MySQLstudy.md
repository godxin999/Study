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