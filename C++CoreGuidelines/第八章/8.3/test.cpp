/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/8 15:58:57
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//ES.40 避免复杂的表达式
/*
void func(){
	while((c=getc())!=-1);//不好，赋值语句隐藏在子表达式中
	while((cin>>c1,cin>>c2),c1==c2);//不好，在一个子表达式中为两个非本地变量赋值
	for(char c1,c2;cin>>c1>>c2&&c1==c2;);//仍然很复杂
	int x= ++i + ++j;//好，如果i和j不是别名(相同数据的名称)
	v[i]=v[j]+v[k];//好，如果i!=j且i!=k
	x=a+(b=f())+(c=g())*7;//不好，多个赋值隐藏在子表达式中	
	x=a&b+c*d&&e^f==7;//不好，依赖于经常被误解的优先级规则
	x=x++ + x++ + ++x;//不好，未定义行为
}
*/
//ES.41 如果运算符优先级不确定，那就使用括号
//ES.42 保持指针的使用简单明了
void f(int* p,int cnt){//充满指针操纵，如果数组长度不正确则会出现错误
	if(cnt<2)return;
	int *q=p+1;
	int n=*p++;
}
#include <span>
void f2(std::span<int> p){
	if(p.size()<2)return ;
	auto q=p.subspan(1);
	int n=p[0];
	std::cout<<q[0]<<" "<<n<<std::endl;
}
void test1(){
	int a[10]={1,2,3,4,5,6,7,8,9,0};
	f2(a);
}
//ES.45 避免魔法常量，采用符号常量
const std::string m[12]={
	"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};
void func2(){
	for(int i=1;i<12;++i){//不好使用了魔法常量1和12
		std::cout<<m[i]<<std::endl;
	}
	constexpr int start_month=1;
	constexpr int end_month=12;
	for(int i=start_month;i<end_month;++i){//好，使用了符号常量
		std::cout<<m[i]<<std::endl;
	}

}
//ES.55 避免对范围检查的需要(能使用std::accumulate就不要使用for循环)

//ES.47 使用nullptr而不是NULL或0
//ES.61 使用delete[] 删除数组对象，使用delete删除非数组对象
//ES.65 不要对无效指针解引用(使用智能指针)

//ES.43 避免有未定义求值顺序的表达式
void test2(){
	int v[10]={0};
	for(int i=0;i<1;++i){
		v[i]=++i;//C++14为未定义行为，C++17保证赋值语句从右到左进行求值
	}
	for(int i=0;i<10;++i){
		std::cout<<v[i]<<" ";//0 1 0 0 0 0 0 0 0 0 
	}
	//C++17还保证移位运算符是从左到右进行求值，如:a<<b，a先求值，然后b求值，然后a左移b位
	//后缀表达式的求值顺序为从左到右，如：a.b、a->b、a[b]、a(b)
	//f1()->m(f2());//从左到右
	//std::cout<<f1()<<f2();//从左到右
	//f1()=f(2);//从右到左
}
//ES.44 不要依赖函数参数的求值顺序
void func3(int a,int b){//函数的求值顺序不是从左到右的
	std::cout<<a<<" "<<b<<std::endl;
}
void test3(){
	int i=0;
	func3(i++,i++);
}

//ES.48 避免转型
//ES.49 如果必须使用转型，请使用具名转型
//ES.50 不要使用转型去除const


int main(){
	//test1();
	//test2();
	test3();


	return 0;
}