#include <cstdint>
#include <utility>
#include <iostream>
#include <vector>

template <typename T,std::uint32_t size_bytes=4096>
class PoolAllocator{
public:
    struct FreeElement{
        char data[sizeof(T)];
        FreeElement* next{nullptr};
    };
    struct FreeList{
        FreeElement* head;
        std::uint32_t size{size_bytes/sizeof(FreeElement)};
    };
public:
    PoolAllocator(){
        pool=new FreeElement[size_bytes/sizeof(FreeElement)];
        for(int i=0;i<size_bytes/sizeof(FreeElement);++i){
            pool[i].next=free_list.head;
            free_list.head=&pool[i];
        }
    }
    PoolAllocator(const PoolAllocator&)=delete;
    PoolAllocator& operator=(const PoolAllocator&)=delete;
    ~PoolAllocator(){
        delete[] pool;
    }
public:
    T* allocate(){
        if(free_list.size==0){
            return nullptr;
        }
        FreeElement* block=free_list.head;
        free_list.head=block->next;
        --free_list.size;
        std::cout<<"allocate"<<std::endl;
        return reinterpret_cast<T*>(block);
    }
    void deallocate(T* p){
        FreeElement* block=reinterpret_cast<FreeElement*>(p);
        block->next=free_list.head;
        free_list.head=block;
        ++free_list.size;
        std::cout<<"deallocate"<<std::endl;
    }
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args){
        new(p) U(std::forward<Args>(args)...);
        std::cout<<"construct"<<std::endl;
    }
    void destroy(T* p){
        p->~T();
        std::cout<<"destroy"<<std::endl;
    }
private:
    FreeElement* pool{nullptr};
    FreeList free_list;
};

struct st{
    int a{0};
    int b{0};
};


int main(){
    PoolAllocator<st,32> pool;
    std::vector<st*> v;
    for(int i=0;i<4;++i){
        st* p=pool.allocate();
        if(!p){
            std::cout<<"allocate failed"<<std::endl;
            break;
        }
        pool.construct(p);
        v.push_back(p);
    }
    std::cout<<v.size()<<std::endl;
    for(auto it=v.begin();it!=v.end();){
        pool.destroy(*it);
        pool.deallocate(*it);
        it=v.erase(it);
    }
    std::cout<<v.size()<<std::endl;
    
    return 0;
}