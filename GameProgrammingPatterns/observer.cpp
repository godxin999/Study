/*************************************************************************
	> File Name: observer.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/30 15:53:31
 ************************************************************************/

//要实现一个包含各种行为来解锁成就的系统是很有技巧的，如果不够小心，成就系统会缠绕在代码库的每个角落。
//成就会在游戏中的各个地方被解锁，解耦成就系统和其他部分是很重要的。
//观察者模式允许代码宣称时间发生而不用关心谁会收到通知。
//比如我们要实现一个从桥上掉落的成就

void Physics::updateEntity(Entity& entity){
	bool wasOnSurface = entity.isOnSurface();
	entity.accelerate(GRAVITY);
	entity.update();
	if(wasOnSurface && !entity.isOnSurface()){
		notify(entity,EVENT_START_FALL);//声称事件发生
	}
}

//成就系统注册自己为观察者，这样无论何时物理代码发送通知，成就系统都能收到通知，它会检测是否有成就被解锁而无需牵扯到物理代码。
//这样我们就可以改变成就的集合或者删除整个成就系统而不用改变物理代码。

//观察者定义如下：

class Observer{
public:
	virtual ~Observer(){}
	virtual void onNotify(const Entity& entity, Event event) = 0;
};

//对于成就系统可以这样实现：

class Achievements : public Observer{
public:
	//onNitify()的参数和游戏有关，典型参数为发送通知的对象和装入细节数据参数
	//可以在此处使用模板编程
	virtual void onNotify(const Entity& entity, Event event){
		//此处使用枚举硬编码
		switch(event){
			case EVENT_ENTITY_FELL:
				if(entity.isHero()&&heroIsOnBridge){
					unlock(ACHIEVEMENT_FELL_OFF_BRIDGE);
				}
				break;
			default:
				break;
		}
	}
private:
	void unlock(Achievement achievement){
		//...
	}
	bool heroIsOnBridge;
};

//被观察者拥有通知的函数，首先它有一个列表，用来保存等它通知的观察者。
//同时被观察者暴露了公开的接口，允许观察者注册和注销。
//观察者还有一个任务就是发送通知

class Subject{
private:
	//可以使用C++容器
	Observer* observers[MAX_OBSERVERS];
	int numObservers;
public:
	void addObserver(Observer* observer){
		//...
	}
	void removeObserver(Observer* observer){
		//...
	}
protected:
	//代码架设了观察者不会再onNotify()中注册或注销自己，需要有更加可靠的方法阻止或优雅的处理这种这样的并发修改
	void notify(const Entity& entity, Event event){
		for(int i = 0; i < numObservers; i++){
			observers[i]->onNotify(entity, event);
		}
	}
};

//现在我们只需要将物理引擎和这些挂钩，那么其就可以发送消息了，我们按照传统的设计模式方法实现，继承Subject类

class Physics : public Subject{
public:
	void updateEntity(Entity& entity){
		//...
	}
};

//但是在真实代码中，我们需要避免使用继承，而是让Physics类持有一个Subject类的实例，这样就不再是观察物理引擎本身而是观察独立的下落事件对象，观察者可以像下面的方式注册自己

physics.entityFell().addObserver(this);

//因为观察者模式是同步的，被观察者直接调用了观察者，这意味着直到所有观察者的通知方法返回后，被观察者才会继续自己的工作，观察者会阻塞被观察者的运行。
//当你有耗时的操作要执行时，将这些操作推到另一个线程或工作队列中去。
//你需要小心地在观察者中混合线程和锁，如果观察者试图获得被观察者拥有的锁，游戏就进入死锁了。
//在多线程引擎中，你最好使用事件队列来做异步通信。

//在真实的项目中中，观察者列表随着观察者的添加和删除而动态地增长和缩短。
//第一件需要注意的事情是只在观察者加入时分配内存，发送通知无需内存分配(只需一个方法调用)，如果你在游戏一开始就加入观察者而不乱动它们，分配的总量是很小的。

//我们可以使用链式观察者来避免动态内存分配，我们使用链表头指针来替代观察者指针的数组

class Subject{
private:
	Observer* head;
public:
	Subject() : head(nullptr){}
	//...
};

class Observer{
	friend class Subject;
public:
	Observer() : next(nullptr){}
	//...
private:
	Observer* next;
};

//因为被观察者拥有增删观察者的接口，但是链表在Observer内部管理，最简单的实现方式就是让被观察者成为友元类
//注册一个新的观察者可以使用头插法

void Subject::addObserver(Observer* observer){
	observer->next = head;
	head = observer;
}

//插入到头部很简单，但是在遍历链表时，我们需要从头部开始，这样就会导致观察者的通知顺序和注册顺序相反。
//最新注册的观察者最先接到通知，所以如果以A，B，C的顺序来注册观察者，它们会以C，B，A的顺序接到通知。
//在好的观察者设计中，观察同一被观察者的两个观察者互相之间不该有任何顺序相关，如果顺序确实有影响，这意味着这两个观察者有一些微妙的耦合，最终会出现问题。
//删除操作如下：

void Subject::removeObserver(Observer* observer){
	if(head == observer){
		head = observer->next;
		observer->next = nullptr;
		return;
	}
	Observer* current = head;
	while(current != nullptr){
		if(current->next == observer){
			current->next = observer->next;
			observer->next = nullptr;
			return;
		}
		current = current->next;
	}
}

//因为使用的是链表，所以我们需要遍历才能找到要删除的观察者，可以使用双向链表或者使用hlist_head来优化删除操作。
//然后是发送通知

void Subject::notify(const Entity& entity, Event event){
	Observer* observer = head;
	while(observer != nullptr){
		observer->onNotify(entity, event);
		observer = observer->next;
	}
}

//我们使用观察者对象作为链表节点，这暗示着观察者对象只能存在于一个观察者链表中，即一个观察者一次只能观察一个被观察者，但是在真实的项目中，一个观察者对象可以同时存在于多个观察者列表中

//我们可以使用链表节点池来解决这个问题

class Subject{
private:
	Node* head;
	//...
	class Node{
	public:
		//...
	private:
		Node* next;
		Observer* observer;
	};
};

//这样多个节点就可以指向同一个观察者对象了，即我们可以同时观察多个被观察者。
//使用这种方式避免动态内存分配的方式很简单，就是预先在对象池中分配它们。

//在被删除时取消注册是观察者的职责。多数情况下，观察者确实知道它在观察哪个被观察者，所以通常需要做的只是给它的析构器添加一个removeObserver()。
//如果在删除被观察者时，你不想让观察者处理问题，这也很好解决。只需要让被观察者在它被删除前发送一个死亡通知。这样任何观察者都可以接收到，然后做些合适的行为。
//更安全的方案是在每个被观察者销毁时，让观察者自动取消注册。这意味着每个观察者都需要有它在观察的被观察者的列表，最终维护一个双向指针。

//观察者模式是一个让不相关的代码块互相交流，而不必打包成更大的块的好方法，这在专注于一个特性或层面的单一代码块内不会太有用。
//这就是为什么它能很好地适应我们的例子：成就和物理是几乎完全不相干的领域，通常被不同的人实现。我们想要它们之间的交流最小化，这样无论在哪一个上工作都不需要另一个的太多信息。

//在观察者模式中，观察者通常是一个对象，它需要实现一个特定的接口或方法，以便在被观察的对象发生变化时接收通知。然而，在支持闭包的语言中，观察者可以简化为一个函数或方法的引用。这样，当被观察的对象发生变化时，就可以直接调用这个函数，而不需要去处理复杂的对象和接口。这种方法使得代码更加简洁，易于理解和维护。