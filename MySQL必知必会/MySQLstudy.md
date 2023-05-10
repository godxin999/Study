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
使用`use`关键字选择数据库
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
使用`show`来显示表列
```sql
show columns from tablename;
describe tablename;--MySQL only
```
其余`show`语句如下:
```sql
show status;--显示服务器状态信息
show create database databasename;--显示创建特定数据库的MySQL语句
show create table tablename;--显示创建特定表的MySQL语句
show grants;--显示授予用户的安全权限
show errors;--显示服务器错误消息
show warnings;--显示服务器警告消息
```

## 检索数据
使用`select`检索单个列
```sql
--从websites表中检索名为name的列
select name 
from websites;
```
使用`select`检索多个列
```sql
--从websites表中检索名为name、id、url的列
select id,name,url 
from websites;
```
使用`select`检索所有列
```sql
--从websites表中检索所有列
select * 
from websites;
```
使用`select`检索不同的行
```sql
--检索country列中不同的值
select distinct country 
from websites;
```
使用`limit`子句限制检索的行数列数
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
使用`order by`子句进行排序
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
使用`desc`关键字进行降序排序
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
利用`order by`子句和`limit`子句进行筛选
```sql
--选出sal列中值最大的一项
select sal 
from websites
order by sal desc
limit 1;
```

## 过滤数据
使用`where`子句指定搜索条件
```sql
--选择url、id列，并指定返回id=2的行
select url,id 
from websites
where id=2;
```
使用`where`子句检查单个值
```sql
--选择url、id列并过滤掉id大于等于4的行
select url,id 
from websites
where id<4;
```
使用`where`子句进行不匹配检查
```sql
--选择url、id列并过滤掉id不等于3的行
select url,id 
from websites
where id<>3;
```
使用`between`关键字进行范围值检查
```sql
--选择url、id列，并返回id为1到3的数据
select url,id 
from websites
where id between 1 and 3;
```
使用`is null`子句进行空值检查
```sql
--选中url、id列，并返回id为null的行
select url,id 
from websites
where id is null;
```

## 数据过滤
使用`and`操作符使用多个过滤条件
```sql
--选中url、id列，并返回国家为中国且sal小于等于50的行
select url,id 
from websites
where country='CN' and sal<=50;
```
使用`or`操作符检索满足任一条件的行
```sql
--选中url、id列，并返回满足sal等于50或500的行
select url,id 
from websites
where sal=50 or sal=500;
```
同时使用`and`和`or`操作符，并使用圆括号保证优先级
```sql
--选中url、id列，并返回满足id等于1或2且alexa小于等于10的行
select url,id 
from websites
where (id=1 or id=2) and alexa<=10;
```
使用`in`操作符指定条件范围
```sql
--选中name、url列并返回按照name排序id为3和5的行
select name,url 
from websites
where id in (3,5)
order by name;
```
使用`not`操作符不匹配条件
```sql
--选中name、url列并返回按照name排序id不为2和4的行
select name,url 
from websites
where id not in (2,4)
order by name;
```

## 使用通配符进行过滤
- `%`通配符可以匹配0或多个字符，但是不能匹配NULL
- `_`通配符只能匹配一个字符


使用`like`操作符和`%`通配符进行过滤
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
使用`like`操作符和`_`通配符进行过滤
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
使用`|`进行`or`匹配
```sql
--选中url、id列，并返回按id排序的url中包含com或or的行
select url,id 
from websites
where url regexp 'com|cn'
order by id;
```
使用`[]`匹配几个字符之一
```sql
--选中url、id列，并返回按id排序的url中包含cn或cm的行
select url,id 
from websites
where url regexp 'c[nm]'
order by id;
```
使用`^`和`[]`不匹配几个字符之一
```sql
--选中url、id列，并返回按id排序的url中不包含cn或cm的行
select url,id 
from websites
where url regexp 'c[^nm]'
order by id;
```
使用`[]`匹配一个范围
```sql
--选中url、id列，并返回按id排序的id中包含1-9的行
select url,id 
from websites
where id regexp '[1-9]'
order by id;
```
使用`\\`转义来匹配特殊字符，如`.`、`[]`、`-`、`|`、`\`
```sql
--选中url、id列，并返回按id排序的url中包含.的行
select url,id 
from websites
where url regexp '\\.'
order by id;
```
使用预定义的字符类进行匹配
- `[:alnum:]` 匹配任意字母和数字
- `[:alpha:]` 匹配任意字符
- `[:blank:]` 匹配空格和制表符
- `[:cntrl:]` 匹配控制字符
- `[:digit:]` 匹配任意数字
- `[:graph:]` 匹配任意可打印字符除空格
- `[:lower:]` 匹配任意小写字母
- `[:print:]` 匹配任意可打印字符
- `[:punct:]` 匹配任意不在[:alnum:]和[:cntrl:]中的字符
- `[:space:]` 匹配包括空格在内的任意空白字符
- `[:upper:]` 匹配任意大写字母
- `[:xdigit:]` 匹配任意十六进制数字

使用重复元字符`?`进行可选的匹配
```sql
--选中url、id列，并返回按id排序的url中包含http(s可选)的行
select url,id 
from websites
where url regexp 'https?'
order by id;
```
使用重复元字符`{n}`进行指定数目的匹配
```sql
--选中url、id、sal列，并返回sal中按id排序的包含连续四个数字的行
select url,id,sal 
from websites
where sal regexp '[[:digit:]]{4}'
order by id;
```
其余的重复元字符如下:
- `*` 匹配0个或多个匹配
- `+` 匹配一个或多个匹配
- `{n,}` 匹配不少于指定数目的匹配
- `{n,m}` 匹配数目的范围

使用定位元字符`^`进行从文本开始的匹配
```sql
--选中url、id、sal列，并返回sal中按id排序的以一个数开头的行
select url,id,sal
from websites
where sal regexp '^[0-9]\\.'
order by id;
```

其余的定位元字符如下:
- `$` 定位文本的末尾
- `[[:<:]]` 定位词的开始
- `[[:>:]]` 定位词的结尾

## 创建计算字段
使用MySQL的`Concat()`函数拼接字段
```sql
--拼接vend_name和vend_country字段，并把vend_country使用括号括起来
select Concat(vend_name,'(',vend_country,')')
from vendors
order by vend_name;
```
使用MySQL的`(R/L)Trim()`函数串两侧多余的空格
```sql
--使用RTrim()函数去除串右侧多余的空格
select Concat(RTrim(vend_name),'(',RTrim(vend_country),')')
from vendors
order by vend_name; 
```
使用`as`关键字给拼接出的字段赋予列别名
```sql
--将拼接出的字段命名为vend_title
select Concat(RTrim(vend_name),'(',RTrim(vend_country),')') as vend_title
from vendors
order by vend_name;
```
使用基础算数操作符进行算数操作
```sql
--使用乘法运算符计算出总价
select prod_id,quantity,item_price,quantity*item_price as expanded_price
from orderitems
where order_num = 20005;
```

## 使用数据处理函数
使用`Upper()`函数将文本转换为大写
```sql
--将vend_name列转换为大写并取别名为vend_name_upcase
select vend_name,Upper(vend_name) as vend_name_upcase
from vendors
order by vend_name;
```
使用`Soundex()`函数进行发音匹配
```sql
--在cust_contact列中搜索发音类似Y Lie的联系名
select cust_name,cust_contact
from customers
where Soundex(cust_contact) = Soundex('Y Lie');
```

其余常用的文本处理函数如下:
- `Length(str)` 返回串的长度
- `Locate(haystack,needle)` 找出needle串在haystack串中第一次出现的位置
- `SubString(str,pos,length)` 返回从pos开始长度为length的str的子串
- `SubString(str,pos)` 返回从pos的str的子串，其中pos可以为负，表示从倒数位置开始截取
- `Lower(str)` 将串转换为小写
- `Left(str,length)` 返回串左边指定数量的字符
- `Right(str,length)` 返回串右边指定数量的字符
- `LTrim(str)` 去除左边的空格
- `RTrim(str)` 去除右边的空格

进行简单的日期比较，日期的标准格式必须为yyyy-mm-dd
```sql
--搜索日期时间为2005-09-01 00:00:00的订单记录
select cust_id,order_num
from orders
where order_date='2005-09-01';
```
order_date的数据类型为datetime，当时间不匹配时，上述检索就会失效，应使用`Date()`函数指示MySQL提取列的日期部分
```sql
--搜索日期为2005-09-01的订单记录，忽略具体的时间
select cust_id,order_num
from orders
where Date(order_date)='2005-09-01';
```
需要匹配月份中天数的日期比较
```sql
--匹配2005年九月的全部订单记录，但是需要知道该月有多少天
select cust_id,order_num
from orders
where Date(order_date) between '2005-09-01' and '2005-09-30';
--匹配2005年九月的全部订单记录，无需知道改月有多少天
select cust_id,order_num
from orders
where Year(order_date) = 2005 and Month(order_date) = 9;   
```
其余的日期和时间处理函数如下:
- `Now()` 返回当前日期时间
- `CurDate()` 返回当前日期
- `CurTime()` 返回当前时间
- `Day()`
- `Hour()`
- `Minute()`
- `Second()`
- `DayOfWeek()` 返回一个日期对应的星期几
- `DateDiff(date1,date2)` 计算两个日期之差
- `Date_Add(datetime,interval expr time)` 返回一个日期时间值加上一个时间间隔expr之后的时间值

还有一部分数值处理函数如下:
- `Abs(x)`
- `Cos(x)`
- `Exp(x)` 返回e的x次方的值
- `Mod(x,y)` 返回x%y
- `Pi()` 返回圆周率
- `Rand()` 返回一个随机数
- `Sin(x)`
- `Sqrt(x)`
- `Tan(x)`

## 汇总数据
使用`AVG()`函数计算特定列的平均值，`AVG()`函数会自动忽略列值为NULL的行
```sql
--计算所有产品的价格的平均值
select AVG(prod_price) as avg_price
from products;
--计算vend_id为1003的产品的价格的平均值
select AVG(prod_price) as avg_price
from products
where vend_id = 1003;
```
使用`COUNT()`函数统计表中行的数目
```sql
--统计customers表的行数，不忽略NULL值
select COUNT(*) as num_cast
from customers;
--统计customers表中有邮件信息的行数，忽略NULL值
select COUNT(cust_email) as num_cast
from customers;
```
使用`MAX()`函数获取列中的最大值
```sql
--获取prod_price列中的最大值
select MAX(prod_price) as max_price
from products;
```
使用`MIN()`函数获取列中的最小值
```sql
--获取prod_price列中的最小值
select MIN(prod_price) as max_price 
from products;
```
使用`SUM()`函数返回指定列值的和
```sql
--统计order_num为20005的所有物品的总量
select SUM(quantity) as item_ordered
from orderitems
where order_num = 20005;
--计算出order_num为20005的所有物品的总价格
select SUM(quantity*item_price) as total_price 
from orderitems
where order_num = 20005;
```
使用`distinct`参数指定列名后，在使用聚集函数时就只统计不同的值
```sql
--计算vend_id为1003的不同产品的价格的平均值
select AVG(distinct prod_price) as avg_price
from products
where vend_id = 1003;
```
可以组合使用聚集函数
```sql
--统计物品的数目的同时统计物品价格的最大值、最小值和平均值
select COUNT(*) as num_items,       
MIN(prod_price) as price_min,
MAX(prod_price) as price_max, 
AVG(prod_price) as price_avg 
from products;
```

## 分组数据
使用`group by`子句建立分组，其使用要求如下:
- `group by`子句可以包含任意数目的列
- `group by`子句中列出的每个列都必须是检索列或者有效的表达式(不能为聚集函数或别名)
- 除了聚集计算语句外，`select`语句中的每个列都必须在`group by`子句中给出
- 如果分组列中包含NULL值，那么NULL将作为一个分组返回，如果列中包含多行NULL值，那么它们将分为一组
- `group by`子句必须出现在`where`子句之后，`order by`子句之前

```sql
--按照vend_id进行分组，并计算每组中的产品总数
select vend_id,COUNT(*) as num_prods
from products
group by vend_id;
```
使用`with rollup`关键字在分组时得到汇总(汇总对应的名称为NULL)
```sql
--按照vend_id进行分组，计算每组中的产品总数并汇总
select vend_id,COUNT(*) as num_prods
from products
group by vend_id with rollup;
```
使用`having`子句对分组进行过滤
```sql
--按照cust_id进行分组，计算订单的总数并过滤出订单总数大于等于2的分组
select cust_id,COUNT(*) as orders
from orders
group by cust_id
having COUNT(*) >= 2;
```
同时使用`having`子句和`where`子句
```sql
--首先过滤掉prod_price小于10的行，然后按照vend_id进行分组和排序，计算产品的总数并过滤出产品总数大于等于2的分组
select vend_id,COUNT(*) as num_prods
from products
where prod_price >= 10
group by vend_id
having COUNT(*) >= 2;
```
使用`group by`子句分组并使用`order by`子句进行排序
```sql
--按照order_num进行分组，计算出物品的总价并过滤出总价大于等于50的分组，最后按照总价大小进行排序
select order_num,SUM(quantity*item_price) as ordertotal
from orderitems
group by order_num
having SUM(quantity*item_price) >= 50
order by ordertotal;
```
至此回顾一下`select`语句中子句的顺序
|子句|说明|是否必须使用|
|:-:|:-:|:-:|
|select|要返回的列或表达式|是|
|from|从中检索数据的表|仅在从表中选择数据时使用|
|where|行级过滤|否|
|group by|分组说明|仅在按组计算聚集时使用|
|having|组级过滤|否|
|order by|输出排序顺序|否|
|limit|要检索的行数|否|

## 使用子查询
使用子查询将多个查询组合成一条语句
```sql
--首先进行最内层的查询得到产品ID为TNT2的订单号列表，然后进行中层的查询得到订购该种产品的客户ID列表，然后进行最外层的查询得到订购该种产品的客户的信息
select cust_name,cust_contact
from customers
where cust_id in (select cust_id
                  from orders
                  where order_num in (select order_num
                                      from orderitems 
                                      where prod_id = 'TNT2'));
```
将计算字段作为一个子查询
```sql
--首先查询每个客户在orders表中的订单数目，将结果作为一列返回，然后再按照cust_name进行排序
select cust_name,cust_state,(select COUNT(*)
                             from orders
                             where orders.cust_id = customers.cust_id) as orders
from customers
order by cust_name;
```
上述例子的子查询涉及外部查询，被成为相关子查询，这时候列名可能存在多义性，需要使用完全限定的列名

## 联结表
使用联结从多个表中检索数据
```sql
--从两个表中选择列，并使用where子句联结两表，指示MySQL匹配vendors表和products表中的vend_id
select vend_name,prod_name,prod_price
from vendors,products
where vendors.vend_id = products.vend_id
order by vend_name,prod_name;
```
如果没有`where`子句建立联结关系，那么就会返回两个表的笛卡尔积，即第一个表的每个行都会和第二个表的每个行配对，为了防止忘记联结条件可以使用`inner join`和`on`子句
```sql
--联结vendors表和products表并匹配两表中的vend_id
select vend_name,prod_name,prod_price
from vendors inner join products
on vendors.vend_id = prodcts.vend_id;
```
使用`where`子句创建多个联结条件
```sql
--查询订单号为20005的产品名、供应商、价格和数量
select prod_name,vend_name,prod_price,quantity
from orderitems,products,vendors
where products.vend_id = vendors.vend_id and orderitems.prod_id = products.prod_id and order_num = 20005;
```
使用联结替换上一章的子查询
```sql
--查询购买TNT2的客户的信息
select cust_name,cust_contact
from customers,orders,orderitems
where customers.cust_id = orders.cust_id and orderitems.order_num = orders.order_num and prod_id = 'TNT2';
```

## 创建高级联结
使用表别名缩短SQL语句
```sql
select cust_name,cust_contact
from customers as c,orders as o,orderitems as oi
where c.cust_id = o.cust_id and oi.order_num = o.order_num and prod_id = 'TNT2';
```
使用自联结替换子查询
```sql
--将products表分别设置别名为p1、p2以防止二义性，然后进行自联结，并按照第二个表中的prod_id过滤数据
select p1.prod_id,p1.prod_name
from products as p1,products as p2
where p1.vend_id = p2.vend_id and p2.prod_id = 'DTNTR';
```
使用自然连接排除多次出现的列，使每个列只返回一次
```sql
--只对第一个表使用通配符，其余的列明确列出，所以没有重复的列被检索出来
select c.*,o.order_num,o.order_date,oi.prod_id,oi.quantity,oi.item_price  
from customers as c,orders as o,orderitems as oi
where c.cust_id = o.cust_id and oi.order_num = o.order_num and prod_id = 'FB';
```
使用外部联结可以包含没有关联行的那些行，在使用`outer join`子句时使用`left`关键字指定包括左表的所有行
```sql
--包含左侧表(customers)中的所有行，检索所有客户的订单号，包括从未下过订单的客户
select customers.cust_id,orders.order_num 
from customers left outer join orders
on customers.cust_id = orders.cust_id;
```
同理使用`right`关键字指定包括右表的所有行
```sql
--包含右侧表(orders)中的所有行，检索所有订单的订单号
select customers.cust_id,orders.order_num
from customers right outer join orders
on customers.cust_id = orders.cust_id;
```
使用带聚集函数的联结
```sql
--使用内部联结，检索每个客户和每个客户的订单数
select customers.cust_name,customers.cust_id,COUNT(orders.order_num) as num_ord
from customers inner join orders
on customers.cust_id = orders.cust_id 
group by customers.cust_id;
--使用外部联结包含左侧表中的所有行，检索每个客户和每个客户的订单数，包括从未下过订单的客户
select customers.cust_name,customers.cust_id,COUNT(orders.order_num) as num_ord
from customers left outer join orders 
on customers.cust_id = orders.cust_id
group by customers.cust_id; 
```

## 组合查询
使用`union`关键字来创建组合查询
```sql
--检索价格小于等于五的物品或者供应商id为1001或1002的物品
select vend_id,prod_id,prod_price
from products
where prod_price <= 5 
union
select vend_id,prod_id,prod_price
from products
where vend_id in (1001,1002);
```
`union`关键字会自动对查询结果进行去重，这是可以使用`union all`关键字来返回所有匹配行
```sql
--不对检索结果进行去重
select vend_id,prod_id,prod_price 
from products
where prod_price <= 5
union all
select vend_id,prod_id,prod_price 
from products
where vend_id in (1001,1002);
```
使用`union`进行组合查询时，只能使用一条`order by`语句进行排序，且需位于最后一条`select`语句之后
```sql
--对组合查询的结果进行排序
select vend_id,prod_id,prod_price
from products
where prod_price <= 5
union
select vend_id,prod_id,prod_price
from products
where vend_id in (1001,1002) 
order by vend_id,prod_price;
```

## 全文本搜索
并非所有的引擎都支持全文本搜索，比如InnoDB引擎不支持全文本搜索，而MyISAM引擎则支持，在创建表的时候需要根据需求选择对应的引擎。
在创建表的时候，为了进行全文本搜索，MySQL根据子句`fulltext`的指示对列进行索引，在定义之后，MySQL会自动维护该索引，`fulltext`可以在创建表的时候指定，也可以在稍后指定。
使用`Match()`函数和`Against()`函数进行全文本搜索，其中`Match()`函数用于指定被搜索的列，`Against()`函数用于指定要使用的搜索表达式
```sql
--检索列note_text并指定rabbit为搜索文本
select note_text 
from productnotes
where Match(note_text) Against('rabbit');
```
全文本搜索会对结果进行排序，高优先级的行先返回
```sql
--rank列会显示出全文本搜索的优先级，不包含rabbit的rank值为0，文本中词靠前的rank值比词靠后的rank值高
select note_text,
Match(note_text) Against('rabbit') as `rank`
from productnotes;
```
使用查询扩展可以找出可能相关的结果，即使它们并不精确的包含所查找的词
```sql
--对anvils使用查询扩展
select note_text 
from productnotes
where Match(note_text) Against('anvils' with query expansion);
```
使用布尔文本搜索可以指定要排斥的词和词的优先级，还可以进行表达式分组，且布尔文本搜索在没有`fulltext`索引的情况下也可以使用
```sql
--匹配包含heavy但是不包含任意以rope开始的词的行
select note_text
from productnotes
where Match(note_text) Against('heavy -rope*' in boolean mode);
```
全文本布尔操作符及其说明如下:
- `+` 包含，词必须存在
- `-` 排除 词必须不出现
- `>` 包含，且增加优先级
- `<` 包含，且减少优先级
- `()` 把词组成子表达式(允许这些子表达式作为一个组被包含、排除、排列)
- `~` 取消一个词的排序值
- `*` 词尾的通配符
- `""` 定义一个短语(与单个词的列表不同，它匹配整个短语以包含或排除这个短语)

下面给出几个例子来说明一些操作符的使用方式
```sql
--匹配包含词rabbit和bait的行
select note_text
from productnotes
where Match(note_text) Against('+rabbit +bait' in boolean mode);
--匹配包含词rabbit和bait中至少一个词的行
select note_text  
from productnotes
where Match(note_text) Against('rabbit bait' in boolean mode); 
--匹配短语rabbit bait
select note_text  
from productnotes
where Match(note_text) Against('"rabbit bait"' in boolean mode); 
--匹配rabbit和carrot并增加前者的优先级和降低后者的优先级
select note_text  
from productnotes
where Match(note_text) Against('>rabbit <carrot' in boolean mode); 
--匹配safe和combination并降低后者的优先级
select note_text  
from productnotes
where Match(note_text) Against('+safe +(<combination)' in boolean mode); 
```
全文本搜索有以下的使用规则
- 短词(三个或三个字符以下的词)会被忽略且从索引中删除
- MySQL内带有的非用词列表在索引时总是被忽略
- 当一个词在50%以上的行都出现时，则将其作为一个非用词忽略，该条不适用于布尔文本搜索
- 如果表中的行数少于3行，那么全文本搜索将不返回结果
- 词中的单引号会被忽略，比如don't索引为dont

## 插入数据
使用`insert`语句插入一个行
```sql
--向customers表中插入一行
insert into customers
values(NULL,
'Pep E. LaPew',
'100 Main Street',
'Los Angeles',
'CA',
'90046',
'USA',
NULL,
NULL);
```
上面的SQL语句高度依赖于表中列的定义次序，所以应该使用下述更安全的`insert`语句
```sql
--明确地给出列名，在插入行时，MySQL会将values中的相应值填入表中的对应项
insert into customers(cust_name,
cust_address,
cust_city,
cust_state,
cust_zip,
cust_country,
cust_contact,
cust_email)
values('Pep E. LaPew',
'100 Main Street', 
'Los Angeles',
'CA',
'90046',
'USA',
NULL,
NULL);
```
如果表的定义允许，则可以在`insert`操作中省略某些列，省略的列必须满足以下某个条件
- 该列定义为允许`NULL`值
- 在表定义中给出默认值，即不给出值时使用默认值

为了提高性能，可以使用`insert low_priority into`语句，这样MySQL会延迟进行该条语句，直到没有客户端从表中读取数据为止

可以一次提交多个`insert`语句以插入多个行，如果每条insert语句中的列名和次序相同时，可以组合各条`insert`语句
```sql
--向customers表中插入两行
insert into customers(cust_name,
cust_address,
cust_city,
cust_state,
cust_zip,
cust_country)
values('Pep E. LaPew',
'100 Main Street',
'Los Angeles',
'CA',
'90046',
'USA'),
('M. Martain',
'42 Galaxy Way',
'New York',
'NY',
'11213',
'USA'); 
```
使用`select`语句配合`insert`语句可以插入检索出的数据
```sql
--将customers表中检索出的数据插入到customers表中
--注意不能在无法保证主键不重复的情况下导入主键列，这里为cust_id
insert into customers(        
cust_contact,
cust_email,
cust_name,
cust_address,
cust_city,
cust_state,
cust_zip,
cust_country)
select         
cust_contact,          
cust_email,
cust_name,
cust_address,
cust_city,
cust_state,
cust_zip,
cust_country
from customers;
```
`insert select`并不要求列名匹配，其使用select中的第一列来填充表中指定的第一个列，其余列同理。

## 更新和删除数据
使用`update`语句更新(修改)表中数据
```sql
--更新客户10005的电子邮件地址
update customers
set cust_email = 'elmer@fudd.com'
where cust_id = 10005;
--更新客户10005的客户名和电子邮件地址
update customers
set cust_name = 'The Fudds',     
cust_email = 'elmer@fudd.com'
where cust_id = 10005;
--使用NULL值去除客户10005的电子邮件地址
update customers
set cust_email = NULL            
where cust_id = 10005;
```
使用`delete`语句删除表中数据
```sql
--删除客户10020的所有数据
delete from customers
where cust_id = 10020;
```
如果想要从表中删除所有的行，可以使用`truncate table`语句，该语句删除原有的表然后重新创建一个表，速度较`delete`语句快

## 创建和操纵表
使用`create table`创建表的时候，需要给出以下信息:
- 新表的名字，在关键字`create table`之后给出
- 表列的名字和定义，使用逗号分隔

```sql
--创建customers表并以cust_id为主键
create table customers
(
cust_id int not NULL auto_increment,
cust_name char(50) not NULL,
cust_address char(50) NULL,
cust_city char(50) NULL,    
cust_state char(5) NULL,  
cust_zip char(10) NULL,  
cust_country char(50) NULL, 
cust_contact char(50) NULL, 
cust_email char(255) NULL,  
primary key (cust_id)
)
engine = InnoDB;
```
在创建新表时，指定的表名必须不存在，否则将会出错，如果想仅在一个表不存在时创建它，那么可以使用`create table if not exists`创建表

在创建表的时候，可以规定列是否接受`NULL`值，不允许`NULL`值的列在插入或更新行时必须有值，此外主键只能使用不允许`NULL`值的列
```sql
--订单号、订单时间、客户ID都需要，所以指定为not NULL
create table orders
(
order_num int not NULL auto_increment,
order_date datetime not NULL,
cust_id int not NULL,
primary key (order_num)
)
engine = InnoDB;
--供应商ID和供应商名是需要的，指定为not NULL，其余指定为NULL
create table vendors
(
vend_id int not NULL auto_increment,
vend_name char(50) not NULL,
vend_address char(50) NULL,
vend_state char(5) NULL,    
vend_zip char(10) NULL,   
vend_country char(50) NULL, 
primary key (vend_id)
)
engine = InnoDB;
```
可以使用多个列作为主键，前提为这些列的组合值必须唯一
```sql
--使用订单号和订单物品作为主键
create table orderitems
(
order_num int not NULL,
order_item int not NULL, 
prod_id char(10) not NULL,
quantity int not NULL,
item_price decimal(8,2) not NULL,
primary key (order_num,order_item)
)
engine = InnoDB;
```
使用`auto_increment`可以在执行`insert`操作时指示MySQL自动对该列增量，从而为该列赋予下一个可用的值，每个表只允许一个`auto_increment`列，且其必须被索引(例如使其成为主键)
```sql
cust_id int not NULL auto_increment
```
如果在插入行的时候没有给出值，MySQL允许指定此时使用的默认值，默认值使用`default`关键字在创建表时指定
```sql
--quantity列默认值为1
create table orderitems
(                                  
order_num int not NULL,
order_item int not NULL,
prod_id char(10) not NULL,
quantity int not NULL default 1,
item_price decimal(8,2) not NULL,  
primary key (order_num,order_item)
)
engine = InnoDB;
```
可以使用`engine=`语句来指定MySQL使用该引擎来创建表，当你使用`select`等语句时，该引擎在内部处理你的请求，常用的几个引擎如下:
- InnoDB为一个可靠的事务处理引擎，但不支持全文本搜索
- MEMORY在功能上等同于MyISAM，但是数据存储在内存中，速度快，适用于临时表
- MyISAM是一个性能极高的引擎，其支持全文本搜索，但不支持事务处理

引擎类型可以混用，但具有一个缺陷，就是外键不能跨引擎，即使用一个引擎的表不能引用具有使用不同引擎的表的外键

如果需要更新表的定义，则可以使用`alter table`语句，其在使用时要求给出下面的信息:
- 在`alter table`后给出要更改的表名(该表必须存在，否则将产生错误)
- 所做更改的列表

```sql
--向vendors表中添加一列
alter table vendors
add vend_phone char(20);
--删除刚刚添加的列
alter table vendors      
drop column vend_phone; 
```
可以使用`alter table`来定义外键
```sql
--添加外键order_item
alter table orderitems
add constraint fk_orderitems_orders
foreign key (order_num) references orders (order_num);
```
可以使用`drop table`删除表
```sql
--删除orderitems表
drop table orderitems;
```
可以使用`rename table`重命名表
```sql
--将customers表重命名为c
rename table customers to c;
--重命名多个表
rename table a1 to a,b1 to b,c1 to c;
```

## 使用视图
视图是虚拟的表，与包含数据的表不一样，视图只包含使用时动态数据的查询
```sql
--创建视图
create view productcustomers as       
select cust_name,cust_contact,prod_id
from customers,orders,orderitems
where customers.cust_id = orders.cust_id and orderitems.order_num = orders.order_num; 
--使用视图进行查询
select cust_name,cust_contact
from productcustomers
where prod_id = 'TNT2';
```
在视图创建之后，可以用与表基本相同的方式来利用它们，可以使用`select`操作，过滤和排序数据，或者将视图联结到其他视图或表，甚至添加和更新数据，但是使用视图有如下限制: 
- 视图的名字必须唯一
- 创建视图需要有足够的访问权限
- `order by`可以用在视图中，如果从该视图检索数据`select`中也含有`order by`，那么视图中的`order by`将被覆盖
- 视图不能索引，也不能有关联的触发器活默认值

视图的基本使用如下: 
- 视图使用`create view`语句来创建
- 可以通过`show create view viewname`来查看创建视图的语句
- 可以使用`drop view viewname`来删除视图
- 更新视图时，可以先用`drop`再用`create`，也可以直接使用`create or replace view`

可以使用视图来重新格式化检索出的数据
```sql
--创建视图
create view vendorlocations as
select Concat(RTrim(vend_name),'(',RTrim(vend_country),')') as vend_title
from vendors
order by vend_name;
--获取格式化后的数据
select * 
from vendorlocations;
```
可以使用视图来过滤不想要的数据
```sql
--创建视图
create view customeremaillist as
select cust_id,cust_name,cust_email
from customers
where cust_email is not null;
--获取过滤后的数据
select *
from customeremaillist;
```
使用视图和计算字段简化数据处理
```sql
--创建视图
create view orderitemsexpanded as
select order_num,prod_id,quantity,item_price,quantity*item_price as expanded_price
from orderitems;
--获取处理后的数据
select *                  
from orderitemsexpanded
where order_num = 20005;  
```
通常情况下，视图是可更新的，即可以对它们使用`insert`、`update`和`delete`操作，但是如果视图包含以下操作，则其是不可更新的
- 分组(使用`group by`和`having`)
- 联结
- 子查询
- 并
- 聚集函数
- `distinct`
- 导出列

## 使用存储过程
使用`call`语句执行存储过程，`call`接受存储过程的名字以及需要传递给它的任意参数
```sql
--执行名为productpricing的存储过程，它计算并返回产品的最低、最高和平均价格
call productpricing(@pricelow,@privehigh,@priceaverage);
```
使用`create procedure`语句创建存储过程
```sql
--因为创建存储过程中存在;所以先把分隔符更换为//，在创建后再更改回来，该存储过程是一个返回产品平均价格的存储过程
delimiter //
create procedure productpricing()
begin
select Avg(prod_price) as priceaverage
from products;
end //
delimiter ;
--调用该存储过程
call productpricing();
```
使用`drop procedure`语句删除创建的存储过程，如果指定的过程不存在，则会发生错误，这是可以使用`drop procedure if exists`语句进行删除
```sql
--删除存储过程productpricing
drop procedure productpricing;
drop procedure if exists productpricing; 
```
一般的存储过程不会显示结果，而是把结果返回给你指定的变量
```sql
delimiter //
create procedure productpricing(
out pl decimal(8,2),--定义输出参数pl，表示最低价格，decimal(8,2)表示最大为8位数字，小数点后共两位
out ph decimal(8,2),--定义输出参数ph，表示最高价格
out pa decimal(8,2)--定义输出参数pa，表示平均价格
)
begin
select Min(prod_price)
into pl
from products;
select Max(prod_price)
into ph
from products;
select Avg(prod_price)
into pa
from products;
end //
delimiter ;
--调用该存储过程
call productpricing(@pricelow,@privehigh,@priceaverage);
```
可以使用`select`语句显示存储过程中的变量
```sql
--显示存储过程中的变量
select @pricehigh,@pricelow,@priceaverage;
```
定义可以传入参数的存储过程
```sql
delimiter //
create procedure ordertotal(
in onumber int,--定义输入参数
out ototal decimal(8,2)
)
begin 
select Sum(item_price*quantity)
from orderitems
where order_num = onumber
into ototal;
end //
delimiter ;
--调用该存储过程，计算订单号为20005的合计
call ordertotal(20005,@total);
--以另外的参数调用存储过程
call ordertotal(20009,@total);
```
建立智能的存储过程
```sql
delimiter //
create procedure ordertotal(
in onumber int,
in taxable boolean,
out ototal decimal(8,2)
)comment 'obtain order total,optionally adding tax'--注释
begin
declare total decimal(8,2);--声明变量
declare taxrate int default 6;--声明变量
select Sum(item_price*quantity)
from orderitems
where order_num = onumber
into total;
if taxable then--条件语句
select total+(total/100*taxrate) into total;--计算税后价格
end if;
select total into ototal;--将总价保存到输出参数ototal中
end //
delimiter ;
```
可以使用`show create procedure`语句来查看显示创建存储过程的`create`语句，如果需要更详细的信息，可以使用`show procedure status`语句
```sql
--检查存储过程
show create procedure ordertotal;
show procedure status like 'ordertotal';
```

## 使用游标
使用游标可以使应用程序根据需要滚动或浏览结果集中的数据，可以使用`declare`语句创建游标，并定义对应的`select`语句
```sql
delimiter //
create procedure processorders()
begin
declare ordernumbers cursor--创建一个游标
for
select order_num from orders;
end //
delimiter ;
```
可以使用`open`和`close`语句打开和关闭游标，如果没有明确关闭游标，那么将会在到达`end`语句时自动关闭
```sql

delimiter //
create procedure processorders()
begin
declare ordernumbers cursor
for
select order_num from orders;
open ordernumbers;--打开游标
close ordernumbers;--关闭游标
end //
delimiter ;
```
可以使用`fetch`语句访问某一行数据
```sql
delimiter //
create procedure processorders()
begin
declare o int;--创建一个局部变量
declare ordernumbers cursor--定义游标
for
select order_num from orders;
open ordernumbers;
fetch ordernumbers into o;--检索当前行的order_num列，并将其存入局部变量o中
close ordernumbers;
end //
delimiter ;
```
可以使用`repeat`语句来循环检索数据(对游标进行循环)
```sql
delimiter //
create procedure processorders()
begin
declare done boolean default 0;
declare o int;
declare ordernumbers cursor
for
select order_num from orders;
declare continue handler for sqlstate '02000' set done = 1;--continue handler定义在条件出现时被执行的代码
--当sqlstate'02000'出现时，把done设置为1，sqlstate'02000'是一个未找到条件，当repeat没有更多的行供循环时，出现该条件
open ordernumbers;
repeat--开始循环
fetch ordernumbers into o; 
until done end repeat;--直到done结束循环
close ordernumbers;
end //
delimiter ;
```
下面对取出的数据进行某种实际的处理
```sql
delimiter //
create procedure processorders()
begin
declare done boolean default 0;
declare o int;--存储订单号
declare t decimal(8,2);--存储每个订单的合计
declare ordernumbers cursor--创建游标
for 
select order_num from orders;
declare continue handler for sqlstate '02000' set done = 1;
create table if not exists ordertotals(--创建表
order_num int,
total decimal(8,2)
);
open ordernumbers;
repeat 
fetch ordernumbers into o;
call ordertotal(o,1,t);--调用存储过程，计算待税的合计
insert into ordertotals(order_num,total)--将数据插入新创建的表中
values(o,t);
until done end repeat;
close ordernumbers;
end //
delimiter ;
```

## 使用触发器
如果在某个表发生改变时需要自动进行处理，那么就需要触发器，触发器是MySQL响应以下任意语句而自动执行的一条MySQL语句
- `delete`
- `insert`
- `update`

其他的语句不支持触发器，在创建触发器时，需要给出四条信息:
- 唯一的触发器名
- 触发器关联的表
- 触发器应响应的活动
- 触发器应何时执行

触发器使用`create trigger`语句创建，每个表最多支持6个触发器(每条`insert`、`update`和`delete`之前和之后)，单一触发器不能和多个事件或多个表关联。

下面对应介绍这三种语句的触发器，首先是`insert`触发器，对于`insert`触发器需要了解以下几点:
- 在`insert`触发器的代码中，可以引用一个名为`new`的虚拟表，访问被插入的行
- 在`before insert`触发器中，`new`中的值也可以被更新
- 对于`auto_increment`的列，`new`在`insert`执行前包含0，在`insert`执行之后包含新的自动生成值。
```sql
--创建insert触发器
create trigger neworder after insert on orders
for each row select new.order_num into @new_order_num;
--向orders表中插入行
insert into orders(order_date,cust_id)
values(Now(),10001);
--获取新得到的订单号
select @new_order_num;
```
这个触发器在orders表发生插入后执行，对于每个新插入的行，触发器从new表中得到新生成的订单号的值，并将其存入new_order_num中，对于orders的每次插入使用这个触发器将返回新的订单号。

然后是`delete`触发器，对于`delete`触发器需要了解以下几点:
- 在`delete`触发器的代码中，可以引用一个名为`old`的虚拟表，访问被删除的行
- `old`表中的值是只读的，不能更新
```sql
--创建delete触发器
delimiter //
create trigger deleteorder before delete on orders
for each row
begin
insert into archive_orders(order_num,order_date,cust_id)  
values(old.order_num,old.order_date,old.cust_id);          
end //
delimiter ;
```
这个触发器在orders表发生删除前执行，对于每个待删除的行，触发器从old表中读出数据，并将其保存到名为archive_orders的存档表中，使用`before delete`触发器的优点为，如果由于某种原因，订单不能存档，那么`delete`本身将被抛弃

最后是`update`触发器，对于`update`触发器需要了解以下几点:
- 在`update`触发器的代码中，可以引用一个名为`old`的虚拟表，访问更新前的值，还可以引用一个名为`new`的虚拟表，访问新更新的值
- 在`before update`触发器中，`new`中的值也可以被更新
- `old`表中的值是只读的，不能更新

```sql
--创建update触发器
create trigger updatevendor before update on vendors 
for each row set new.vend_state = Upper(new.vend_state);
```
这个触发器vendors表发生更新前执行，对于每个要更新的行，触发器将`new`表中的国家名替换为大写的

## 管理事务处理
事务处理可以用来维护数据库的完整性，它保证成批的MySQL操作要么执行，要么完全不执行，最常用的MyISAM和InnoDB两种引擎中只有InnoDB引擎支持事务管理，MySQL使用下面的语句来标识事务的开始:
```sql
--开始事务处理
start transaction
```
MySQL使用`rollback`指令来回退MySQL语句
```sql
--使用rollback回退了删除表中所有行的操作
delimiter //
select * from ordertotals;
start transaction;
delete from ordertotals;
select * from ordertotals;
rollback;
select * from ordertotals //
delimiter ;
```
此外`rollback`语句不能回退`select`语句，也不能回退`create`和`drop`操作

在事务处理块中，提交不会隐含地进行，为了进行明确地提交，可以使用`commit`语句
```sql
--使用comiit提交
delimiter // 
start transaction; 
delete from orderitems where order_num = 20010;
delete from orders where order_num = 20010;
commit //
delimiter ;
```
在`rollback`和`commit`语句执行后，事务会自动关闭

为了支持部分事务处理地回退，可以在事务处理块中放置占位符(保留点)
```sql
--创建一个保留点
save delete1;
```
在事务处理块中可以使用`rollback`回退到已经设置地保留点
```sql
--回退到对应的保留点
rollback to delete1;
```
默认的MySQL行为是自动提交所有更改，为了指示MySQL不自动提交更改，可以使用以下语句:
```sql
--设置不自动提交
set autocommit = 0;
```

## 全球化和本地化
使用`show`语句可以查看所有可用的字符集和支持校对的完整列表
```sql
--查看所有可用的字符集
show character set;
--查看所有可用的校对
show collation;
```
为了确认系统所用的字符集和校对，可以使用以下语句:
```sql
--使用通配符确定所用的字符集和校对
show variables like 'character%';
show variables like 'collation%';
```
为了给表指定字符集和校对，可以使用带子句的`create table`
```sql
--指定表使用的字符集和校对
create table mytable
(                
col1 int,
col2 varchar(10)
)default character set hebrew
collate hebrew_general_ci;
```
MySQL还允许对每个列设置字符集和校对
```sql
--指定col3列的字符集和校对
create table mytable       
( 
col1 int,
col2 varchar(10),
col3 varchar(10) character set latin1 collate latin1_general_ci
)default character set hebrew
collate hebrew_general_ci;
```
可以在`select`语句中指定特定的校对顺序来进行排序
```sql
--按对应的校对顺序进行排序
select * from customers
order by lastname,firstname collate latin1_general_cs;
```

## 安全管理
如果需要获得所有用户账号，则需要直接访问mysql数据库和表
```sql
--获取所有用户账号
use mysql;
select user from user;
```
可以使用`create user`语句创建一个用户
```sql
--创建名为ben的账号，口令为123456
create user ben identified by '123456';
```
可以使用`rename user`语句来重命名账号名称
```sql
--将账号ben重命名为bforta
rename user ben to bforta;
```
可以使用`drop user`语句来删除用户账号
```sql
--删除账号bforta
drop user bforta;
```
可以使用`show grants for`来查看账号的权限
```sql
--查看账号bforta的权限
show grants for bforta;
--输出GRANT USAGE ON *.* TO `bforta`@`%`表示根本没有权限
```
为了设置权限，可以使用`grant`语句，并且至少给出以下信息:
- 要授予的权限
- 被授予访问的数据库或表
- 用户名

```sql
--授予bforta对crashcourse所有表的select权限
grant select on crashcourse.* to bforta;
--授予bforta对crashcourse所有表的select和insert权限
grant select,insert on crashcourse.* to bforta;
```
可以使用`revoke`语句来撤销特定的权限
```sql
--撤销bforta对crashcourse所有表的select权限
revoke select on crashcourse.* from bforta;
```
可以使用`alter`语句来更改用户的密码
```sql
--更改bforta账户的密码
alter user bforta identified by '654321';
```

## 数据库维护
可以使用mysqldump来备份数据库的数据
```sql
--备份book数据库
mysqldump -u root -p book > C:\Users\admin\Desktop\Study\MySQL必知必会\book.sql
--备份所有数据库
mysqldump -u root -p --opt --all-databases > C:\Users\admin\Desktop\Study\MySQL必知必会\all-data.sql
```
可以使用`analyze table`语句来检查表键是否正确
```sql
--检查orders表
analyze table orders;
```
可以使用`check table`语句来发现和修复问题
```sql
--检查orders和orderitems表
check table orders,orderitems;
```
如果MyISAM表访问产生不正确和不一致的结果，可能需要使用`repair table`语句来修复响应的表，如果从一个表中删除大量数据，应该使用`optimize table`语句来收回所用的空间

如果需要排除系统启动问题，可以在命令行执行`mysqld`启动，下面是几个重要的命令行参数
- --help 显示帮助
- --safe-mode 装在减去某些最佳配置的服务器
- --verbose 显示全文本消息
- --version 显示版本信息

MySQL有以下几种日志:
- hostname.err 错误日志，包含启动和关闭问题以及关键错误的细节
- hostname.log 查询日志，记录所有的MySQL活动，但因为日志增长过快不应该长期使用
- hostname-bin 二进制日志，记录更新过数据的所有语句
- hostname-slow.log 缓慢查询日志，在确定数据库何处需要优化时很有用

上述日志均位于data目录中在使用日志时，可以使用`flush logs`语句来刷新和重新开始所有日志文件

## 改善性能
以下要点可能会影响MySQL的性能:
- 关键的DBMS应该运行在自己的专用服务器上
- MySQL是一个多用户多线程的DBMS，如果遇到显著的性能不良，可以使用`show processlist`显示所有活动进程(以及它们的线程ID和执行时间)，还可以使用`kill`命令结束莫格特定的进程
- 总是有不止一种方法编写同一条`select`语句，应该试验联结、并、子查询等，找出最佳的方法
- 使用`explain`语句让MySQL解释它将如何执行一条`select`语句
- 一般来说，存储过程比一条一条执行其中的各条语句快
- 不要使用`select *`
- 在导入数据时，应该关闭自动提交
- 必须索引数据库表以改善数据检索的性能
- 如果`select`语句中有复杂的`or`条件，则可以使用`union`语句来提升性能
- 如果一些表收集数据且不常被搜索，则在有必要之前不要索引它们
- `like`很慢，一般来说最好使用`fulltext`而不是`like`
- 每条规则在某些条件下都会被打破

