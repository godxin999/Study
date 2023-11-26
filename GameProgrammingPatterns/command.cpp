/*************************************************************************
	> File Name: command.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/26 21:22:47
 ************************************************************************/

#include <iostream>
#include <algorithm>

//每个游戏中都有一块代码读取用户的输入，然后将其转换为游戏中的动作。
//以下是一个简单的实现
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

//因为许多游戏都允许玩家配置按键，为了支持这一点，我们要把jump和fire转化为可以变换的东西
//我们定义了一个基类代表可以触发的游戏行为

class Command{
public:
	virtual ~Command(){}
	virtual void execute()=0;
};

//然后为不同的游戏行为创建子类

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

//在代码的输入处理部分，为每一个按键存储一个指向命令的指针，这样就可以在游戏中动态的改变按键的绑定

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

//之前的代码中，我们假定顶层的jump和fireGun之类的函数可以找到玩家的角色并且执行相应的动作。
//这些假定的耦合限制了这些命令的用处，因为它们只能用于玩家角色。
//我们放松这个限制，不让函数去找玩家角色，而是将函数控制的对象传进去

class Command{
public:
	virtual ~Command(){}
	virtual void execute(GameActor& actor)=0;
};

//这里的GameActor是代表游戏世界中角色的游戏对象类，我们将其传给execute函数，这样命令类的子类就可以调用所选游戏对象上的方法了

class JumpCommand:public Command{
public:
	void execute(GameActor& actor)override{
		actor.jump();
	}
};

//现在就可以使用这个类让任何角色跳跃了，而不仅仅是玩家角色
//现在还缺少一部分代码，第一我们需要修改HandleInput函数，让它可以返回一个命令

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

//这里并不能立即执行，因为还不知道哪个角色会传递进来，这是命令模式的好处，延迟到调用执行时再知道
//然后接收命令并作用在玩家上

Command* command = inputHandler.handleInput();
if (command){
	command->execute(player);
}

//这样我们就可以让玩家控制游戏中的任何角色，只需要向命令传入不同的角色就可以了
//同理，我们可以在AI和角色之间使用相同的命令模式，AI代码只需要生成Command对象，然后传入AI角色就可以了
//甚至可以给玩家角色加上AI，这样就可以进行自动演示了
//对于控制角色的命令，我们将其视为命令队列或命令流，一些代码（输入控制器或者AI）产生一系列命令放入流中。 另一些代码（调度器或者角色自身）调用并消耗命令。 通过在中间加入队列，可以对生产者和消费者进行解耦
//如果将这些命令序列化，我们可以通过网络流传输它们。我们可以接受玩家的输入，将其通过网络发送到另外一台机器上，然后将其重现，这是网络多人游戏的基础。

//如果一个命令可以做一件事，那么它亦可以撤销这件事，例如，移动一个单位的代码可能如下：

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

//这个例子不是通用的移动某物的命令，而是游戏回合中特殊的一次移动
//这里的命令更加特殊，它们代表了特定时间点能做的特定事件，这意味着输入控制代码可以在玩家下决定时创建一个实例，就像这样

Command* handleInput(){
	Unit* unit=getSelectedUnit();

	if(isPressed(BUTTON_UP))
		return new MoveUnitCommand(unit,unit->x(),unit->y()-1);

	if(isPressed(BUTTON_DOWN))
		return new MoveUnitCommand(unit,unit->x(),unit->y()+1);

	//其他的移动命令
	return nullptr;
}

//为了让指令可以被取消，我们为每个类定义另一个需要实现的方法：

class Command{
public:
	virtual ~Command(){}
	virtual void execute()=0;
	virtual void undo()=0;
};

//undo函数回滚了execute函数造成的游戏状态改变

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

//如果要撤销多个命令，我们就需要记录指令列表，让后用一个引用指向当前的指令，当玩家执行一个命令时，我们将其加入列表，然后将引用指向它，当玩家撤销时，我们就将代表当前的指针向后退，当他们选择重做时，我们就将指针向前移动，如果在撤销后选择了新的命令，那么就清除当前指针所指向的命令之后的所有命令

//最终可能会得到很多不同的命令类。为了更容易实现这些类，可以定义一个具体的基类，包含一些能定义行为的高层方法，往往会有帮助。这将命令的主体execute函数转到子类沙箱之中。
//在上面的例子中，我们明确地指定哪个角色会处理命令。但是在某些情况下，特别是当对象模型分层时，也可以不这么简单粗暴，对象可以响应命令，或者将命令交给它的从属对象。（职责链模式）
//有些命令是无状态的纯粹行为，比如第一个例子中的JumpCommand。在这种情况下，有多个实例是在浪费内存，因为所有的实例是等价的，这时可以用享元模式。