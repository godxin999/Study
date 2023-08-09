/*************************************************************************
        > File Name: test.cpp
        > Author: godxin999
        > Mail: A996570107@163.com
        > Created Time: 2023/7/22 13:45:21
 ************************************************************************/

#include <algorithm>
#include <cstring>
#include <iostream>
#include <type_traits>

using namespace std;
//使用默认模板参数作为返回类型
template <typename T1, typename T2,
          typename RT = std::decay_t<decltype(true ? T1() : T2())>>
RT max(T1 a, T2 b) {
  return b > a ? b : a;
}
//利用类型萃取 std::common_type<>作为返回类型的默认值
template <typename T1, typename T2, typename RT = std::common_type_t<T1, T2>>
RT max2(T1 a, T2 b) {
  return b > a ? b : a;
}
//返回类型作为第一个模板参数，并且依然能够从其它两个模板参数推断出它的类型
template <typename RT = long, typename T1, typename T2>
RT max3(T1 a, T2 b) {
  return b > a ? b : a;
}


int main() {
  int a = 100;
  double b = 200.0;
  cout << max3(a, b) << endl;

  return 0;
}
