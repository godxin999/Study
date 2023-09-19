#include <deque>


template <typename T,typename Cont=std::deque<T>>
class Stack{
private:
    Cont elems;
public:
    void push(const T& elem){
        elems.push_back(elem);
    }
    void pop(){
        elems.pop_back();
    }
    const T& top()const{
        return elems[elems.size()-1];
    }
    bool empty()const{
        return elems.empty();
    }

    template <typename T2,typename Cont2>
    Stack<T,Cont>& operator=(const Stack<T2,Cont2>&);
    template <typename,typename> friend class Stack;//为了访问其他Stack实例的私有成员，将所有其他类型的Stack模板都声明为友元，因为参数名字不会被使用到，所以可以被忽略
};


template <typename T,typename Cont> template <typename T2,typename Cont2>
Stack<T,Cont>& Stack<T,Cont>::operator=(const Stack<T2,Cont2>& op2){//类型检查会在插入时进行
    /*Stack<T2> tmp(op2);
    elems.clear();
    while(!tmp.empty()){
        elems.push_front(tmp.top());
        tmp.pop();
    }*/
    elems.clear();
    elems.insert(elems.begin(),op2.elems.begin(),op2.elems.end());
    return *this;
}