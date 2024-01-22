/*************************************************************************
	> File Name: stl.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/22 12:17:44
 ************************************************************************/

#include <functional>
#include <ios>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>

template <typename Cont>
void print(const Cont& c){
    for(const auto& i:c){
        std::cout<<i<<" ";
    }
    std::cout<<std::endl;
}
//std::make_heap、std::push_heap、std::pop_heap、std::sort_heap
void test1(){
    std::vector<int> v{0,1,2,3,4,5,6,7,8,9};
    std::make_heap(v.begin(),v.end());//将一个无序的区间转换为一个大根堆
    print(v);
    
    v.push_back(100);
    std::push_heap(v.begin(),v.end());//将一个元素加入到大根堆中
    print(v);
    
    std::pop_heap(v.begin(),v.end());//将大根堆的根节点放到最后一个位置，并且将剩余的元素重新调整为大根堆
    v.pop_back();//删除最后一个元素
    print(v);
    
    std::sort_heap(v.begin(),v.end());//将一个大根堆转换为有序的区间，默认为升序
    print(v);
    //可以传入std::greater<>()，将大根堆转换为小根堆
}
//std::sort、std::stable_sort、std::partial_sort、std::nth_element、std::inplace_merge
void test2(){
    std::vector<int> v{0,8,6,5,4,9,3,2,7,1};
    std::vector<int> temp{v};

    std::sort(v.begin(),v.end());
    print(v);

    std::stable_sort(v.begin(),v.end());//稳定排序，保证相等元素的相对顺序不变
    print(v);
    
    v=temp;
    std::partial_sort(v.begin(),v.begin()+3,v.end());//将前三个元素排序，剩余的元素不保证有序
    print(v);
    
    v=temp;
    std::nth_element(v.begin(),v.begin()+3,v.end());
    std::cout<<v[3]<<std::endl;
    
    v=temp;
    std::partial_sort(v.begin(),v.begin()+3,v.end());
    std::partial_sort(v.begin()+3,v.end(),v.end());
    std::inplace_merge(v.begin(),v.begin()+3,v.end());//将两个有序的区间合并为一个有序的区间，内存充足时为O(n)的merge，否则为O(nlogn)的原地归并算法
    print(v);
}
//std::partion、std::stable_partion、std::partition_point
void test3(){
    std::vector<int> v{1,8,6,4,5,9,3,2,7,0};
    std::vector<int> temp{v};

    auto it1=std::partition(v.begin(),v.end(),[](int i){return i%2==0;});//将区间中的元素按照谓词划分为两个部分，前半部分为true，后半部分为false，返回划分点的迭代器
    print(v);
    std::cout<<*it1<<std::endl;
    
    v=temp;
    auto it2=std::stable_partition(v.begin(),v.end(),[](int i){return i%2==0;});//将区间中的元素按照谓词划分为两个部分，前半部分为true，后半部分为false，保证相对顺序不变，返回划分点的迭代器
    print(v);
    std::cout<<*it2<<std::endl;

    auto it3=std::partition_point(v.begin(),v.end(),[](int i){return i%2==0;});//在已分区的范围中找到分区点，返回分区点的迭代器，若没有分区则返回第一个不满足谓词的迭代器
    std::cout<<*it3<<std::endl;
}
//std::rotate、std::shuffle、std::next_permutation、std::prev_permutation、std::reverse
void test4(){
    std::vector<int> v{0,1,2,3,4,5,6,7,8,9};
    std::vector<int> temp{v};

    auto it=std::rotate(v.begin(),v.begin()+3,v.end());//将区间中的元素循环右移3个位置
    print(v);
    std::cout<<*it<<std::endl;//返回旋转后的第一个元素的迭代器

    v=temp;
    std::shuffle(v.begin(),v.end(),std::default_random_engine());//将区间中的元素随机打乱
    print(v);
    
    v=temp;
    std::next_permutation(v.begin(),v.end());//如果有存在比其大的排列，其重新排列为下一个排列并返回true，否则返回false(只根据字典序进行判断)
    print(v);
    
    auto b1=std::prev_permutation(v.begin(),v.end());//如果有存在比其小的排列，其重新排列为上一个排列并返回true，否则返回false(只根据字典序进行判断)
    print(v);
    auto b2=std::prev_permutation(v.begin(),v.end());
    std::cout<<b1<<" "<<b2<<std::endl;//1 0
    
    v=temp;
    std::reverse(v.begin(),v.end());//将区间中的元素反转
    print(v);
}
//std::is_sorted、std::is_heap、std::is_partitioned、std::is_permutation、std::is_sorted_until、std::is_heap_until、std::is_partitioned_until
void test5(){
    std::vector<int> v{0,1,2,3,4,5,6,7,9,8};
    std::vector<int> temp{v};
    std::next_permutation(temp.begin(), temp.end());

    std::cout<<std::boolalpha;
    std::cout<<std::is_sorted(v.begin(),v.end())<<std::endl;//判断区间是否有序
    std::cout<<std::is_heap(v.begin(),v.end())<<std::endl;//判断区间是否为大根堆
    std::cout<<std::is_partitioned(v.begin(),v.end(),[](int i){return i%2==0;})<<std::endl;//判断区间是否已经分区
    std::cout<<std::is_permutation(v.begin(),v.end(),temp.begin())<<std::endl;//判断第二个区间是否为第一个区间的一个排列
    std::cout<<std::distance(v.begin(),std::is_sorted_until(v.begin(),v.end()))<<std::endl;//返回区间中第一个不满足有序的元素的迭代器
    std::cout<<std::distance(v.begin(),std::is_heap_until(v.begin(),v.end()))<<std::endl;//返回区间中第一个不满足大根堆的元素的迭代器
}
//std::count、accumulate、std::inner_product、std::partial_sum、std::adjacent_difference、std::sample
void test6(){
    std::vector<int> v{0,1,2,3,4,5,6,7,8,9};
    std::vector<int> temp{v};

    std::cout<<std::count(v.begin(),v.end(),1)<<std::endl;//计算区间中等于1的元素的个数

    std::cout<<std::accumulate(v.begin(),v.end(),0)<<std::endl;//计算区间中元素的和

    std::cout<<std::inner_product(v.begin(),v.end(),temp.begin(),0)<<std::endl;//计算两个区间的内积
    
    std::partial_sum(v.begin(),v.end(),v.begin());//计算区间中元素的部分和，结果保存在第三个参数为起始位置的区间中
    print(v);

    v=temp;
    std::adjacent_difference(v.begin(),v.end(),v.begin());//计算区间中前一元素的差值，结果保存在第三个参数为起始位置的区间中，第一个元素默认不变，默认的操作是std::minus<>()，可以传入其他谓词
    print(v);

    v=temp;
    std::vector<int> res;
    std::sample(v.begin(),v.end(),std::back_inserter(res),5,std::default_random_engine());//从区间中不重复地选择count个元素，使得每个样本拥有相同的出现概率
    print(res);
}
//std::reduce、std::transform_reduce、std::inclusive_scan、std::exclusive_scan、std::transform_inclusive_scan、std::transform_exclusive_scan
void test7(){
    std::vector<int> v{1,1,2,3,1,5,1,7,8,9};
    std::vector<int> temp{v};
    std::vector<int> result(v.size());

    std::cout<<std::reduce(v.begin(),v.end())<<std::endl;//计算区间中元素的和

    std::cout<<std::transform_reduce(v.begin(),v.end(),0,std::plus<>(),[](int i){return i&1;})<<std::endl;//计算区间中元素的和，其对每个元素使用transform操作对应谓词，再对结果使用reduce操作对应谓词

    v=temp;
    std::inclusive_scan(v.begin(),v.end(),v.begin());//计算区间中元素的部分和，结果保存在第三个参数为起始位置的区间中，包含当前元素
    print(v);

    v=temp;
    std::exclusive_scan(v.begin(),v.end(),v.begin(),0);//计算区间中元素的部分和，结果保存在第三个参数为起始位置的区间中，不包含当前元素
    print(v);

    v=temp;
    std::transform_inclusive_scan(v.begin(),v.end(),v.begin(),std::plus<>(),[](int i){return i&1;});//计算区间中元素的部分和，结果保存在第三个参数为起始位置的区间中，包含当前元素，transform操作意义同上
    print(v);
    
    v=temp;
    std::transform_exclusive_scan(v.begin(),v.end(),v.begin(),0,std::plus<>(),[](int i){return i&1;});//计算区间中元素的部分和，结果保存在第三个参数为起始位置的区间中，不包含当前元素，transform操作意义同上
    print(v);
}
//std::all_of、std::any_of、std::none_of
void test8(){
    std::vector<int> v{0,1,2,3,4,5,6,7,8,9};
    std::vector<int> empty;

    std::cout<<std::boolalpha;
    std::cout<<std::all_of(v.begin(),v.end(),[](int i){return i>0;})<<std::endl;//判断区间中的元素是否全部满足谓词

    std::cout<<std::any_of(v.begin(),v.end(),[](int i){return i>0;})<<std::endl;//判断区间中的元素是否有一个满足谓词

    std::cout<<std::none_of(v.begin(),v.end(),[](int i){return i>0;})<<std::endl;//判断区间中的元素是否全部不满足谓词

    std::cout<<std::all_of(empty.begin(),empty.end(),[](int i){return i>0;})<<std::endl;//空集合的所有元素都满足谓词

    std::cout<<std::any_of(empty.begin(),empty.end(),[](int i){return i>0;})<<std::endl;//空集合的任意元素都不满足谓词

    std::cout<<std::none_of(empty.begin(),empty.end(),[](int i){return i>0;})<<std::endl;//空集合的所有元素都不满足谓词
}
//std::equal、std::lexicographical_compare、std::mismatch
void test9(){
    std::vector<int> v1{0,1,2,3,4,5,6,7,8,9};
    std::vector<int> v2{0,1,2,3,4,5,6,7,8,9};
    std::vector<int> v3{0,1,2,3,4,5,6,7,8,0};
    std::vector<int> v4{0,1,2,3,4,5,6,7,8,0,1};

    std::cout<<std::boolalpha;
    std::cout<<std::equal(v1.begin(),v1.end(),v2.begin())<<std::endl;//判断两个区间是否相等

    std::cout<<std::lexicographical_compare(v1.begin(),v1.end(),v3.begin(),v3.end())<<std::endl;//判断两个区间的字典序，如果第一个区间小于第二个区间返回true，否则返回false

    const auto& [it1,it2]=std::mismatch(v1.begin(),v1.end(),v4.begin());
    std::cout<<*it1<<" "<<*it2<<std::endl;//返回两个区间中第一个不相等的元素的迭代器
}
//std::find、std::adjacent_find
void test10(){
    std::vector<int> v{0,1,2,3,3,5,6,7,8,9};

    std::cout<<std::distance(v.begin(),std::find(v.begin(),v.end(),5))<<std::endl;//返回区间中第一个等于5的元素的迭代器

    std::cout<<std::distance(v.begin(),std::adjacent_find(v.begin(),v.end()))<<std::endl;//返回区间中第一个相邻的相等的元素的迭代器
}
//std::equal_range、std::lower_bound、std::upper_bound、std::binary_search
void test11(){
    std::vector<int> v{0,1,2,3,3,5,6,7,8,9};

    const auto& [it1,it2]=std::equal_range(v.begin(),v.end(),3);//返回区间中等于3的元素的范围
    std::cout<<std::distance(v.begin(),it1)<<" "<<std::distance(v.begin(),it2)<<std::endl;//3 5

    std::cout<<std::distance(v.begin(),std::lower_bound(v.begin(),v.end(),3))<<std::endl;//返回区间中第一个大于等于3的元素的迭代器

    std::cout<<std::distance(v.begin(),std::upper_bound(v.begin(),v.end(),3))<<std::endl;//返回区间中第一个大于3的元素的迭代器

    std::cout<<std::boolalpha<<std::binary_search(v.begin(),v.end(),3)<<std::endl;//判断区间中是否存在等于3的元素
}
//std::search、std::find_end、std::find_first_of
void test12(){
    std::vector<int> v1{0,1,2,3,4,5,6,3,4,5};
    std::vector<int> v2{3,4,5};
    //传入的第二个区间为子序列，第一个区间为被搜索的区间
    std::cout<<std::distance(v1.begin(),std::search(v1.begin(),v1.end(),v2.begin(),v2.end()))<<std::endl;//返回区间中第一个子序列的迭代器

    std::cout<<std::distance(v1.begin(),std::find_end(v1.begin(),v1.end(),v2.begin(),v2.end()))<<std::endl;//返回区间中最后一个子序列的迭代器

    std::cout<<std::distance(v1.begin(),std::find_first_of(v1.begin(),v1.end(),v2.begin(),v2.end()))<<std::endl;//返回区间中第一个与子序列中任意一个元素相等的元素的迭代器
}
//std::max_element、std::min_element、std::minmax_element
void test13(){
    std::vector<int> v{0,1,2,3,4,5,6,7,8,9};

    std::cout<<*std::max_element(v.begin(),v.end())<<std::endl;//返回区间中最大的元素的迭代器

    std::cout<<*std::min_element(v.begin(),v.end())<<std::endl;//返回区间中最小的元素的迭代器

    const auto& [it1,it2]=std::minmax_element(v.begin(),v.end());//返回区间中最小和最大的元素的迭代器
    std::cout<<*it1<<" "<<*it2<<std::endl;
}



int main(){
    //test1();
    //test2();
    //test3();
    //test4();
    //test5();
    //test6();
    //test7();
    //test8();
    //test9();
    //test10();
    //test11();
    //test12();
    //test13();

    return 0;
}