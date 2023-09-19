#include <deque>

//模板参数模板
/*
template<typename T,template<typename Elem> class Cont=std::deque>//第二个模板参数被定义为一个类模板，从C++17开始才可以使用typename替换类模板的class
class Stack{
private:
    Cont<T> elems;
public:
    void push(const T& elem);    
    void pop(){
        elems.pop_back();
    }
    const T& top()const{
        return elems[elems.size()-1];
    }
    bool empty()const{
        return elems.empty();
    }
};

template <typename T,template<typename> class Cont>
void Stack<T,Cont>::push(const T& elem){
    elems.push_back(elem);
}
*/
//在C++17之前，template<typename elem> class Cont=std::deque中的模板参数需要和实际参数匹配，而std::deque有两个参数，第二个为默认参数allocator，这个参数也要匹配，所以应该将模板定义为如下形式

template <typename T,template<typename Elem,typename Alloc=std::allocator<Elem>> class Cont=std::deque>
class Stack{
private:
    Cont<T> elems;
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
    template <typename,template<typename,typename> class>
    friend class Stack;
    template <typename T2,template<typename Elem2,typename=std::allocator<Elem2>> class Cont2>
    Stack<T,Cont>& operator=(const Stack<T2,Cont2>&);
};

template <typename T,template<typename,typename> class Cont>
template <typename T2,template<typename,typename> class Cont2>
Stack<T,Cont>& Stack<T,Cont>::operator=(const Stack<T2,Cont2>& op2){
    elems.clear();
    elems.insert(elems.begin(),op2.elems.begin(),op2.elems.end());
    return *this;
}