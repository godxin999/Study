# 游戏编程模式

## 重访设计模式

### 命令模式

每个游戏中都有一块代码读取用户的输入，然后将其转换为游戏中的动作。以下是一个简单的实现：

```cpp
void InputHandler::handleInput(){
	if (IsPressed(BUTTON_X))
		jump();
	else if (IsPressed(BUTTON_Y))
		fireGun();
	else if (IsPressed(BUTTON_A))
		swapWeapon();
	else if (IsPressed(BUTTON_B))
		lurchIneffectively();
}
```

因为许多游戏都允许玩家配置按键，为了支持这一点，我们要把jump和fire转化为可以变换的东西，我们定义了一个基类代表可以触发的游戏行为。

```cpp
class Command{
public:
	virtual ~Command(){}
	virtual void execute()=0;
};
```

然后为不同的游戏行为创建子类

```cpp
class JumpCommand:public Command{
public:
	void execute()override{
		jump();
	}
};

class FireCommand:public Command{
public:
	void execute()override{
		fireGun();
	}
};

class NoneCommand:public Command{
public:
	void execute()override{
		//为某些没有绑定的按键提供一个默认的命令
	}
};
```

在代码的输入处理部分，为每一个按键存储一个指向命令的指针，这样就可以在游戏中动态的改变按键的绑定。

```cpp
class InputHandler{
public:
	//输入处理
	void handleInput(){
		if (IsPressed(BUTTON_X))
			buttonX_->execute();
		else if (IsPressed(BUTTON_Y))
			buttonY_->execute();
		else if (IsPressed(BUTTON_A))
			buttonA_->execute();
		else if (IsPressed(BUTTON_B))
			buttonB_->execute();
	}
private:
	Command* buttonX_;
	Command* buttonY_;
	Command* buttonA_;
	Command* buttonB_;
};
```

在之前的代码中，我们假定顶层的jump和fireGun之类的函数可以找到玩家的角色并且执行相应的动作。这些假定的耦合限制了这些命令的用处，因为它们只能用于玩家角色。现在我们放松这个限制，不让函数去找玩家角色，而是将函数控制的对象传进去。

```cpp
class Command{
public:
	virtual ~Command(){}
	virtual void execute(GameActor& actor)=0;
};
```

这里的GameActor是代表游戏世界中角色的游戏对象类，我们将其传给execute函数，这样命令类的子类就可以调用所选游戏对象上的方法了。

```cpp
class JumpCommand:public Command{
public:
	void execute(GameActor& actor)override{
		actor.jump();
	}
};
```

现在就可以使用这个类让任何角色跳跃了，而不仅仅是玩家角色，但是现在还缺少一部分代码，第一我们需要修改HandleInput函数，让它可以返回一个命令。

```cpp
Command* InputHandler::handleInput(){
	if (IsPressed(BUTTON_X))
		return buttonX_;
	if (IsPressed(BUTTON_Y))
		return buttonY_;
	if (IsPressed(BUTTON_A))
		return buttonA_;
	if (IsPressed(BUTTON_B))
		return buttonB_;	
	return nullptr;
}
```

这里并不能立即执行，因为还不知道哪个角色会传递进来，这是命令模式的好处，延迟到调用执行时再知道。然后接收命令并作用在玩家上

```cpp
Command* command = inputHandler.handleInput();
if (command){
	command->execute(player);
}
```

这样我们就可以让玩家控制游戏中的任何角色，只需要向命令传入不同的角色就可以了。同理，我们可以在AI和角色之间使用相同的命令模式，AI代码只需要生成Command对象，然后传入AI角色就可以了。甚至可以给玩家角色加上AI，这样就可以进行自动演示了。

对于控制角色的命令，我们将其视为命令队列或命令流，一些代码（输入控制器或者AI）产生一系列命令放入流中。 另一些代码（调度器或者角色自身）调用并消耗命令。 通过在中间加入队列，可以对生产者和消费者进行解耦。如果将这些命令序列化，我们可以通过网络流传输它们。我们可以接受玩家的输入，将其通过网络发送到另外一台机器上，然后将其重现，这是网络多人游戏的基础。

如果一个命令可以做一件事，那么它亦可以撤销这件事，例如，移动一个单位的代码可能如下：

```cpp
class MoveUnitCommand:public Command{
public:
	MoveUnitCommand(Unit* unit, int x, int y):unit_(unit),x_(x),y_(y){}
	void execute()override{
		unit_->moveTo(x_,y_);
	}
private:
	Unit* unit_;
	int x_,y_;
};
```

这个例子不是通用的移动某物的命令，而是游戏回合中特殊的一次移动，这里的命令更加特殊，它们代表了特定时间点能做的特定事件，这意味着输入控制代码可以在玩家下决定时创建一个实例，就像这样。

```cpp
Command* handleInput(){
	Unit* unit=getSelectedUnit();

	if(isPressed(BUTTON_UP))
		return new MoveUnitCommand(unit,unit->x(),unit->y()-1);

	if(isPressed(BUTTON_DOWN))
		return new MoveUnitCommand(unit,unit->x(),unit->y()+1);

	//其他的移动命令
	return nullptr;
}
```

为了让指令可以被取消，我们为每个类定义另一个需要实现的方法：

```cpp
class Command{
public:
	virtual ~Command(){}
	virtual void execute()=0;
	virtual void undo()=0;
};
```

undo函数回滚了execute函数造成的游戏状态改变。

```cpp
class MoveUnitCommand:public Command{
public:
	MoveUnitCommand(Unit* unit, int x, int y):unit_(unit),xBefore_(0),yBefore_(0),x_(x),y_(y){}
	void execute()override{
		xBefore_=unit_->x();
		yBefore_=unit_->y();
		unit_->moveTo(x_,y_);
	}
	void undo()override{
		unit_->moveTo(xBefore_,yBefore_);
	}
private:
	Unit* unit_;
	int xBefore_,yBefore_;
	int x_,y_;
};
```

如果要撤销多个命令，我们就需要记录指令列表，让后用一个引用指向当前的指令，当玩家执行一个命令时，我们将其加入列表，然后将引用指向它，当玩家撤销时，我们就将代表当前的指针向后退，当他们选择重做时，我们就将指针向前移动，如果在撤销后选择了新的命令，那么就清除当前指针所指向的命令之后的所有命令

最终可能会得到很多不同的命令类。为了更容易实现这些类，可以定义一个具体的基类，包含一些能定义行为的高层方法，往往会有帮助。这将命令的主体execute函数转到子类沙箱之中。

在上面的例子中，我们明确地指定哪个角色会处理命令。但是在某些情况下，特别是当对象模型分层时，也可以不这么简单粗暴，对象可以响应命令，或者将命令交给它的从属对象。（职责链模式）

有些命令是无状态的纯粹行为，比如第一个例子中的JumpCommand。在这种情况下，有多个实例是在浪费内存，因为所有的实例是等价的，这时可以用享元模式。

### 享元模式

游戏中的森林有成千上万的树，每棵树都有上千的多边形，每棵树都有一系列与之相关的数据：

1. 树干、树叶和树叶形状的多边形网格
2. 树皮和树叶的纹理
3. 森林中树的位置和朝向
4. 大小和色彩之类的调节参数

```cpp
class Tree{
private:
    Mesh* mesh_;
    Texture bark_;
    Texture leaves_;
    Vector position_;
    double height_;
    double thickness_;
    Color barkTint_;
    Color leafTint_;
};
```

因为多边形网格和纹理的体积非常大，所以需要进行处理，因为森林中有很多长得一模一样的树，它们使用了相同的网格和纹理，所以我们可以通过显式地将对象切为两部分。首先将树公有的数据拿出来分离到另一个类中。

```cpp
class TreeModel{
private:
    Mesh* mesh_;
    Texture bark_;
    Texture leaves_;
};
```

游戏中每个树的实例只需要有一个对这个共享TreeModel的引用，留在Tree中的是哪些实例相关的数据。

```cpp
class Tree{
private:
    TreeModel* model_;
    Vector position_;
    double height_;
    double thickness_;
    Color barkTint_;
    Color leafTint_;
};
```

把所有的东西都存在主存里没什么问题，但是这对渲染也毫无帮助。在森林到屏幕上之前，它得先到GPU。我们需要用显卡可以识别的方式共享数据。为了减少需要推送到GPU的数据量，需要将共享的数据TreeModel只发送一次。然后我们分别发送每个树独特的数据(位置，颜色，大小)。最后告诉GPU使用同一模型渲染每个实例。

今日的图形接口和显卡正好支持这一点，Direct3D和OpenGL都可以做实例渲染。这些API需要提供两部分数据流。第一部分是一块需要渲染多次的共同数据。第二部分是实例的列表以及绘制时需要用的参数。然后调用一次渲染就可以绘制整个森林。

在实例渲染时，每棵树通过总线送到GPU消耗的更多是时间而非内存。享元模式通过将对象的数据分为两种来解决这个问题。第一种数据没有特定指明是哪个对象的实例，因此可以在它们间分享。Gof称之为固有状态(上下文无关部分)，数据的剩余部分是变化状态，那些每个实例独一无二的东西。

这些树长出来的地方也需要在游戏中表示。这里可能有草，泥土，丘陵，湖泊，河流，以及其它任何你可以想到的地形。我们基于区块建立地表：世界的表面被划分为由微小区块组成的巨大网格，其中每个区块都由一种地形覆盖。每种地形类型都有一系列特性会影响游戏玩法：

1. 决定了玩家能够多快地穿过它的移动开销
2. 表明能否用船穿过的水域标识
3. 用来渲染它的纹理。

游戏程序员偏执于效率，所以不会在每个区块中保存这些状态。一个通用的方式是为每种地形使用一个枚举。

```cpp
enum Terrain{
    TERRAIN_GRASS,
    TERRAIN_HILL,
    TERRAIN_RIVER
};

class world{
private:
    Terrain tiles_[WIDTH][HEIGHT];//世界管理巨大的网格
};
```

为了获取区块的实际的有用的数据，我们使用如下函数：

```cpp
int world::getMovementCost(int x, int y){
    switch(tiles_[x][y]){
        case TERRAIN_GRASS: return 1;
        case TERRAIN_HILL: return 3;
        case TERRAIN_RIVER: return 2;
    }
}

bool world::isWater(int x, int y){
    switch(tiles_[x][y]){
        case TERRAIN_GRASS: return false;
        case TERRAIN_HILL: return false;
        case TERRAIN_RIVER: return true;
    }
}
```

这可行，但是很丑。移动开销和水域标识是区块的数据，但在这里它们散布在代码中。更糟的是，简单地形的数据被众多方法拆开了。所以我们设计了地形类：

```cpp
class Terrain{
public:
    Terrain(int movementCost, bool isWater, Texture texture):
        movementCost_(movementCost),
        isWater_(isWater),
        texture_(texture)
    {}
    int getMovementCost()const{return movementCost_;}
    bool isWater()const{return isWater_;}
    const Texture& getTexture()const{return texture_;}
private:
    int movementCost_;
    bool isWater_;
    Texture texture_;
};
```

同样，我们不想为每一个区块都保存一个实例，显然上面的类中的所有状态都是固有的(上下文无关的)，我们不使用枚举，而是使用Terrain对象指针来表示地形：

```cpp
class world{
private:
    Terrain* tiles_[WIDTH][HEIGHT];
    //...
};
```

由于地形实例在很多地方使用，如果要动态分配，它们的声明周期会很复杂，所以我们直接在游戏世界中存储它们。

```cpp
class world{
public:
    world():
        grassTerrain_(1,false,GRASS_TEXTURE),
        hillTerrain_(3,false,HILL_TEXTURE),
        riverTerrain_(2,true,RIVER_TEXTURE)
    {}
private:
    //可以使用容器(对象池)来存储每种地形，在需要时首先从容器中查找，如果没有找到，就创建一个新的地形对象
    Terrain grassTerrain_;
    Terrain hillTerrain_;
    Terrain riverTerrain_;
    Terrain* tiles_[WIDTH][HEIGHT];
    //...
};
```

然后可以以如下方式来绘制地形：

```cpp
void world::generateTerrain(){
    for(int i=0;i<WIDTH;++i){
        for(int j=0;j<HEIGHT;++j){
            if(random(10)==0){//随机生成丘陵
                tiles_[i][j]=&hillTerrain_;
            }else{
                tiles_[i][j]=&grassTerrain_;
            }
        }
    }
    int x= random(WIDTH);//随机生成河流
    for(int y=0;y<HEIGHT;++y){
        tiles_[x][y]=&riverTerrain_;
    }
}
```

现在就不需要world中的方法来接触地形(switch-case)属性了，可以直接暴露出Terrain对象。

```cpp
const Terrain& world::getTile(int x, int y)const{
    return *tiles_[x][y];
}
```

使用这种方式，world不再与地形的细节耦合了。

```cpp
int cost=world.getTile(x,y).getMovementCost();
```

### 观察者模式

要实现一个包含各种行为来解锁成就的系统是很有技巧的，如果不够小心，成就系统会缠绕在代码库的每个角落。成就会在游戏中的各个地方被解锁，解耦成就系统和其他部分是很重要的。观察者模式允许代码宣称时间发生而不用关心谁会收到通知。比如我们要实现一个从桥上掉落的成就：

```cpp
void Physics::updateEntity(Entity& entity){
	bool wasOnSurface = entity.isOnSurface();
	entity.accelerate(GRAVITY);
	entity.update();
	if(wasOnSurface && !entity.isOnSurface()){
		notify(entity,EVENT_START_FALL);//声称事件发生
	}
}
```

成就系统注册自己为观察者，这样无论何时物理代码发送通知，成就系统都能收到通知，它会检测是否有成就被解锁而无需牵扯到物理代码。这样我们就可以改变成就的集合或者删除整个成就系统而不用改变物理代码。

观察者定义如下：

```cpp
class Observer{
public:
	virtual ~Observer(){}
	virtual void onNotify(const Entity& entity, Event event) = 0;
};
```

对于成就系统可以这样实现：

```cpp
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
```

被观察者拥有通知的函数，首先它有一个列表，用来保存等它通知的观察者。同时被观察者暴露了公开的接口，允许观察者注册和注销。此外观察者还有一个任务就是发送通知。

```cpp
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
```

现在我们只需要将物理引擎和这些挂钩，那么其就可以发送消息了，我们按照传统的设计模式方法实现，继承Subject类：

```cpp
class Physics : public Subject{
public:
	void updateEntity(Entity& entity){
		//...
	}
};
```

但是在真实代码中，我们需要避免使用继承，而是让Physics类持有一个Subject类的实例，这样就不再是观察物理引擎本身而是观察独立的下落事件对象，观察者可以像下面的方式注册自己

```cpp
physics.entityFell().addObserver(this);
```

因为观察者模式是同步的，被观察者直接调用了观察者，这意味着直到所有观察者的通知方法返回后，被观察者才会继续自己的工作，观察者会阻塞被观察者的运行。当你有耗时的操作要执行时，将这些操作推到另一个线程或工作队列中去。你需要小心地在观察者中混合线程和锁，如果观察者试图获得被观察者拥有的锁，游戏就进入死锁了。在多线程引擎中，你最好使用事件队列来做异步通信。

在真实的项目中中，观察者列表随着观察者的添加和删除而动态地增长和缩短。第一件需要注意的事情是只在观察者加入时分配内存，发送通知无需内存分配(只需一个方法调用)，如果你在游戏一开始就加入观察者而不乱动它们，分配的总量是很小的。

我们可以使用链式观察者来避免动态内存分配，我们使用链表头指针来替代观察者指针的数组

```cpp
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
```

因为被观察者拥有增删观察者的接口，但是链表在Observer内部管理，最简单的实现方式就是让被观察者成为友元类，注册一个新的观察者可以使用头插法：

```cpp
void Subject::addObserver(Observer* observer){
	observer->next = head;
	head = observer;
}
```

插入到头部很简单，但是在遍历链表时，我们需要从头部开始，这样就会导致观察者的通知顺序和注册顺序相反。最新注册的观察者最先接到通知，所以如果以A，B，C的顺序来注册观察者，它们会以C，B，A的顺序接到通知。在好的观察者设计中，观察同一被观察者的两个观察者互相之间不该有任何顺序相关，如果顺序确实有影响，这意味着这两个观察者有一些微妙的耦合，最终会出现问题。

删除操作如下：

```cpp
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
```

因为使用的是链表，所以我们需要遍历才能找到要删除的观察者，可以使用双向链表或者使用hlist_head来优化删除操作。

然后是发送通知：

```cpp
void Subject::notify(const Entity& entity, Event event){
	Observer* observer = head;
	while(observer != nullptr){
		observer->onNotify(entity, event);
		observer = observer->next;
	}
}
```

我们使用观察者对象作为链表节点，这暗示着观察者对象只能存在于一个观察者链表中，即一个观察者一次只能观察一个被观察者，但是在真实的项目中，一个观察者对象可以同时存在于多个观察者列表中。我们可以使用链表节点池来解决这个问题：

```cpp
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
```

这样多个节点就可以指向同一个观察者对象了，即我们可以同时观察多个被观察者。使用这种方式避免动态内存分配的方式很简单，就是预先在对象池中分配它们。

在被删除时取消注册是观察者的职责。多数情况下，观察者确实知道它在观察哪个被观察者，所以通常需要做的只是给它的析构器添加一个removeObserver()。如果在删除被观察者时，你不想让观察者处理问题，这也很好解决。只需要让被观察者在它被删除前发送一个死亡通知。这样任何观察者都可以接收到，然后做些合适的行为。

更安全的方案是在每个被观察者销毁时，让观察者自动取消注册。这意味着每个观察者都需要有它在观察的被观察者的列表，最终维护一个双向指针。

观察者模式是一个让不相关的代码块互相交流，而不必打包成更大的块的好方法，这在专注于一个特性或层面的单一代码块内不会太有用。这就是为什么它能很好地适应我们的例子：成就和物理是几乎完全不相干的领域，通常被不同的人实现。我们想要它们之间的交流最小化，这样无论在哪一个上工作都不需要另一个的太多信息。

在观察者模式中，观察者通常是一个对象，它需要实现一个特定的接口或方法，以便在被观察的对象发生变化时接收通知。然而，在支持闭包的语言中，观察者可以简化为一个函数或方法的引用。这样，当被观察的对象发生变化时，就可以直接调用这个函数，而不需要去处理复杂的对象和接口。这种方法使得代码更加简洁，易于理解和维护。

### 原型模式

假设我们游戏中存在Ghost，Demon，Sorcerer等怪物，这些怪物都有自己对应的类，生产者构造特定种类怪物的实例，为了在游戏中支持每种怪物，我们可以使用一种暴力的实现方式，让每个怪物类都有生产者类，实现出来的效果如下：

```cpp
class Monster{};
class Ghost:public Monster{};
class Demon:public Monster{};
class Sorcerer:public Monster{};

class Spawner{
public:
    virtual ~Spawner(){}
    virtual Monster* spawnMonster()=0;
};
class GhostSpawner:public Spawner{
public:
    Monster* spawnMonster()override{
        return new Ghost();
    }
};
class DemonSpawner:public Spawner{
public:
    Monster* spawnMonster()override{
        return new Demon();
    }
};
class SorcererSpawner:public Spawner{
public:
    Monster* spawnMonster()override{
        return new Sorcerer();
    }
};
```

这样的代码显得很笨重，原型模式提供了一个解决方案，一个对象可以产出和自己相近的对象，如果有一个怪物，那么就可以制造更多的怪物，还可以制造其他的怪物，任何怪物都可以被视为原型怪物，产出其他版本的自己。

为了实现这个功能，我们给基类Monster添加一个抽象的clone方法。

```cpp
class Monster{
public:
    virtual ~Monster(){}
    virtual Monster* clone()=0;
    //...
};
```

每个怪兽子类提供一个特定的实现，返回与它自己的类和状态都完全一样的新对象，例如：

```cpp
class Ghost:public Monster{
public:
    Ghost(int health, int speed):health_(health),speed_(speed){}
    Monster* clone()override{
        return new Ghost(health_,speed_);
    }

private:
    int health_;
    int speed_;
};
```

一旦所有的怪物都支持这个方法，我们就不再需要为每个怪物类创建生产者类，我们只需要定义一个类：

```cpp
class Spawner{
public:
    Spawner(Monster* prototype):prototype_(prototype){}
    Monster* spawnMonster(){
        return prototype_->clone();
    }
private:
    Monster* prototype_;
};
```

为了得到恶灵的生产者，我们创建一个恶灵的原型实例，然后创建拥有这个实例的生产者：

```cpp
Monster* ghostPrototype = new Ghost(15,3);
Spawner* ghostSpawner = new Spawner(ghostPrototype);
```

这个模式的灵巧之处在于它不但拷贝原型的类，也拷贝它的状态。这就意味着我们可以创建一个生产者，生产快速鬼魂，虚弱鬼魂，慢速鬼魂，而只需创建一个合适的原型鬼魂。

但是在实现clone()时，会遇见令人不快的语义漏洞，做深层拷贝还是浅层拷贝呢？换言之，如果恶魔拿着叉子，克隆恶魔也要克隆叉子吗？我们需要将每个怪物有独立的类作为前提条件，这绝对不是当今大多数游戏引擎运作的方法。这样庞杂的类层次管理起来很痛苦，这是我们为什么用组件模式和类型对象为不同的实体建模的原因(无需一一建构自己的类)。

哪怕我们确实需要为每个怪物创建不同的类，我们还可以使用生产函数来代替clone()方法。

```cpp
Monster* spawnGhost(){
    return new Ghost();
}

using SpawnCallback=Monster*(*)();

class Spawner{
public:
    Spawner(SpawnCallback spawn):spawn_(spawn){}
    Monster* spawnMonster(){
        return spawn_();
    }
private:
    SpawnCallback spawn_;
};
```

此时为了给恶灵创建生产者，我们只需要做：

```cpp
Spawner* ghostSpawner = new Spawner(spawnGhost);
```

生产者类需要为某类怪物构建实例，但是我们不想硬编码是哪类怪物。自然的解决方案是将它作为模板中的类型参数：

```cpp
class Spawner{
public:
    virtual ~Spawner(){}
    virtual Monster* spawnMonster()=0;
};

template <class T>
class SpawnFor:public Spawner{
public:
    Monster* spawnMonster()override{
        return new T();
    }
};
```

然后就可以像这样来使用：

```cpp
Spawner* ghostSpawner = new SpawnFor<Ghost>();
```

早期的游戏在程序中生成几乎所有东西，这样程序可以塞进磁盘和老式游戏卡带。在今日的游戏中，代码只是驱动游戏的引擎，游戏是完全由数据定义的。

当游戏数据达到一定规模时，需要考虑一些相似的方案：使用原型和委托来重用数据。

游戏设计者需要在很多文件中设定怪物和物品的属性。游戏中的哥布林也许有如下定义：

```json
{
  "name": "goblin grunt",
  "minHealth": 20,
  "maxHealth": 30,
  "resists": ["cold", "poison"],
  "weaknesses": ["fire", "light"]
}

{
  "name": "goblin wizard",
  "minHealth": 20,
  "maxHealth": 30,
  "resists": ["cold", "poison"],
  "weaknesses": ["fire", "light"],
  "spells": ["fire ball", "lightning bolt"]
}

{
  "name": "goblin archer",
  "minHealth": 20,
  "maxHealth": 30,
  "resists": ["cold", "poison"],
  "weaknesses": ["fire", "light"],
  "attacks": ["short bow"]
}
```

在哥布林实体间有很多的重复，如果这是代码，我们会为哥布林构建抽象，并在三个哥布林类型中重用。但是JSON没法这么做。所以让我们把它做得更加巧妙些。

我们可以为对象添加prototype字段，记录委托对象的名字。如果在此对象内没找到一个字段，那就去委托对象中查找。

这样我们就可以简化我们的Json文件：

```json
{
  "name": "goblin grunt",
  "minHealth": 20,
  "maxHealth": 30,
  "resists": ["cold", "poison"],
  "weaknesses": ["fire", "light"]
}

{
  "name": "goblin wizard",
  "prototype": "goblin grunt",
  "spells": ["fire ball", "lightning bolt"]
}

{
  "name": "goblin archer",
  "prototype": "goblin grunt",
  "attacks": ["short bow"]
}
```

由于弓箭手和术士都将grunt作为原型，我们就不需要在它们中重复血量，防御和弱点。在基于原型的系统中，对象可以克隆产生新对象是很自然的，我认为在这里也一样自然。这特别适合记录那些只有一处不同的实体的数据。

对于Boss和其他独特的事物，它们通常是更加常见事物的重新定义，原型委托是定义它们的好方法。例如断头魔剑，就是一把拥有加成的长剑，可以像下面这样表示：

```json
{
  "name": "Sword of Head-Detaching",
  "prototype": "longsword",
  "damageBonus": "20"
}
```

### 单例模式

设计模式中是这样描述单例模式的：保证一个类只有一个实例，并且提供了访问该实例的全局访问点。

单例的现代实现方案如下：

```cpp
class FileSystem{
public:
	static FileSystem& instance(){
		static FileSystem* instance = new FileSystem();
		return *instance;
	}
private:
	FileSystem(){}
};
```

我们使用单例的的原因如下：

- 如果没有人使用，那么就不必要创建实例
- 它在运行时实例化
- 可继承单例

假设我们需要跨平台的文件系统封装类。为了达到这一点，我们需要它变成文件系统抽象出来的接口，而子类为每个平台实现接口。基类的定义如下：

```cpp
class FileSystem{
public:
	virtual ~FileSystem(){}
	virtual char* readFile(char* path)=0;
	virtual void writeFile(char* path, char* contents)=0;
public:
	static FileSystem& instance();
protected:
	FileSystem(){}
};
```

然后为每个平台定义子类：

```cpp
class PS3FileSystem:public FileSystem{
public:
	char* readFile(char* path)override{
		//...
	}
	void writeFile(char* path, char* contents)override{
		//...
	}
};
class WiiFileSystem:public FileSystem{
public:
	char* readFile(char* path)override{
		//...
	}
	void writeFile(char* path, char* contents)override{
		//...
	}
};
```

灵巧之处在于如何创建实例：

```cpp
FileSystem& FileSystem::instance(){
#ifdef PLATFORM_PS3
	static PS3FileSystem* instance = new PS3FileSystem();
#elif PLATFORM_WII
	static WiiFileSystem* instance = new WiiFileSystem();
#endif
	return *instance;
}
```

这样整个代码库都可以使用FileSystem::instance()接触到文件系统，而无需和任何平台相关的代码耦合。耦合发生在为特定平台写的FileSystem类实现文件中。

因为单例模式是全局状态，所以其依旧会带来一些问题。对于日志类，大部分模块都能从记录诊断日志中获益。但是日志类显然不能作为参数传入需要它的函数，明显的解决方案是让Log类成为单例。这样每个函数都能从类那里获得一个实例，这样带来的后果是不能创建多个日志实例，所有的信息都倾泻到同一个文件中。我们想将日志分散到多个文件中来解决这点，但是单例模式不允许我们这么做。

对于惰性初始化的单例，其在运行时实例化。如果初始化音频系统消耗了几百个毫秒，那么我们需要控制它何时发生，因为如果在第一次声音播放时惰性初始化它自己，这可能发生在游戏的高潮部分，从而导致可见的掉帧和断续的游戏体验。同样，游戏通常需要严格管理在堆上分配的内存来避免碎片。如果音频系统在初始化时分配到了堆上，我们需要知道初始化在何时发生，这样我们可以控制内存待在堆的哪里。因为这两个原因，大多数游戏都不使用惰性初始化。相反，它们像这样实现单例模式：

```cpp
class FileSystem{
public:
	static FileSystem& instance(){
		return instance_;
	}
private:
	FileSystem(){}
	static FileSystem instance_;
};
```

这解决了惰性初始化的问题，但是损失了几个单例带来的好处，在静态实例中，我们不能使用多态，在静态初始化时，类也必须是可构建的，我们也不能在不需要这个实例的时候，释放实例所占的内存。

在使用单例之前，我们需要考虑是否真的需要单例类，在游戏代码中，有很多单例类都是“管理器”，它们的存在意义就是照顾其他对象，管理器类有时是有用的，但是有时我们可以通过将单例所有的行为都移动到单例管理(帮助)的类中。这样就可以避免多余的管理器类(单例)的产生。

便利的访问是我们使用单例的一个主要原因，这让我们在不同地方获取需要的对象更加容易。通用原则是在能完成工作的同时，将变量写得尽可能局部，对象影响的范围越小，所以在我们使用具有全局状态的单例之前，首先应考虑代码中其他获取对象的方式：

- 从参数中传入
- 从基类中获得

对于GameObject基类，游戏中的对象都继承它，我们可以利用这一点，将Log对象放在基类中，这样所有的派生类都能访问到它(子类沙箱)。

```cpp
class GameObject{
protected:
	Log& getLog(){
		return log;
	}
private:
	static Log& log;
};

class Enemy : public GameObject{
	void doSomething(){
	    getLog().write("I can log!");
	}
};
```

这保证任何GameObject之外的代码都不能接触Log对象，但是每个派生的实体都确实能使用getLog()，但是这也引出了一个新问题，GameObject是怎样获得Log实例的？一个简单的方案是，让基类创建并拥有静态实例。如果你不想要基类承担这些，你可以提供一个初始化函数传入Log实例，或使用服务定位器模式找到它。

- 从已经是全局的东西中获取

移除所有全局状态并不实际，大多数代码库仍有一些全局可用对象，比如一个代表了整个游戏状态的Game或World对象，我们可以让现有的全局对象捎带需要的东西，来减少全局变量类的数目，从而不让Log、FileSystem和AudioPlayer都变成单例。

```cpp
class Game{
public:
	static Game& instance(){
		return instance;
	}
	Log& getLog(){
		return *log;
	}
	FileSystem& getFileSystem(){
		return *fileSystem;
	}
	AudioPlayer& getAudioPlayer(){
		return *audioPlayer;
	}
private:
	static Game instance;
  	Log         *log;
	FileSystem  *fileSystem;
	AudioPlayer *audioPlayer;
};

```

这样只有Game是全局可见的，函数可以通过它访问其他系统。

```cpp
Game::instance().getAudioPlayer().play(VERY_LOUD_BANG);
```

如果未来架构被改为支持多个Game实例，Log、FileSystem和AudioPlayer都不会被影响到。这种方式的缺陷是更多的代码耦合到了Game中，如果一个类简单地需要播放声音，为了访问音频播放器，需要通过游戏对象来访问。我们可以通过混合方案解决这个问题，知道Game的代码可以直接从它那里访问AudioPlayer，而不知道Game的代码，我们用上面描述的其他选项来提供AudioPlayer。

- 从服务定位器中获得

目前为止，我们假设全局类是具体的类，比如Game。另一种方式是定义一个类，这个类存在的唯一目标就是为对象提供全局访问，这种常见的模式被称为服务定位器模式。

### 状态模式




