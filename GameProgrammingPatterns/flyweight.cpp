//游戏中的森林有成千上万的树，每棵树都有上千的多边形，每棵树都有一系列与之相关的数据：
//1.树干、树叶和树叶形状的多边形网格
//2.树皮和树叶的纹理
//3.森林中树的位置和朝向
//4.大小和色彩之类的调节参数

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

//因为多边形网格和纹理的体积非常大，所以需要进行处理，因为森林中有很多长得一模一样的树，它们使用了相同的网格和纹理，所以我们可以通过显式地将对象切为两部分。
//可以将树公有的数据拿出来分离到另一个类中。

class TreeModel{
private:
    Mesh* mesh_;
    Texture bark_;
    Texture leaves_;
};

//游戏中每个树的实例只需要有一个对这个共享TreeModel的引用，留在Tree中的是哪些实例相关的数据

class Tree{
private:
    TreeModel* model_;
    Vector position_;
    double height_;
    double thickness_;
    Color barkTint_;
    Color leafTint_;
};

//把所有的东西都存在主存里没什么问题，但是这对渲染也毫无帮助。在森林到屏幕上之前，它得先到GPU。我们需要用显卡可以识别的方式共享数据。
//为了减少需要推送到GPU的数据量，需要将共享的数据TreeModel只发送一次。然后我们分别发送每个树独特的数据(位置，颜色，大小)。最后告诉GPU使用同一模型渲染每个实例。
//今日的图形接口和显卡正好支持这一点，Direct3D和OpenGL都可以做实例渲染。这些API需要提供两部分数据流。第一部分是一块需要渲染多次的共同数据。第二部分是实例的列表以及绘制时需要用的参数。然后调用一次渲染就可以绘制整个森林。

//实例渲染时，每棵树通过总线送到GPU消耗的更多是时间而非内存。
//享元模式通过将对象的数据分为两种来解决这个问题。第一种数据没有特定指明是哪个对象的实例，因此可以在它们间分享。Gof称之为固有状态(上下文无关部分)，数据的剩余部分是变化状态，那些每个实例独一无二的东西。

//这些树长出来的地方也需要在游戏中表示。这里可能有草，泥土，丘陵，湖泊，河流，以及其它任何你可以想到的地形。我们基于区块建立地表：世界的表面被划分为由微小区块组成的巨大网格，其中每个区块都由一种地形覆盖。
//每种地形类型都有一系列特性会影响游戏玩法：
//1.决定了玩家能够多快地穿过它的移动开销
//2.表明能否用船穿过的水域标识
//3.用来渲染它的纹理。
//游戏程序员偏执于效率，所以不会在每个区块中保存这些状态。一个通用的方式是为每种地形使用一个枚举。

enum Terrain{
    TERRAIN_GRASS,
    TERRAIN_HILL,
    TERRAIN_RIVER
};

class world{
private:
    Terrain tiles_[WIDTH][HEIGHT];//世界管理巨大的网格
};

//为了获取区块的实际的有用的数据，我们使用如下函数：

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

//这可行，但是很丑。移动开销和水域标识是区块的数据，但在这里它们散布在代码中。更糟的是，简单地形的数据被众多方法拆开了。
//然后我们设计了地形类：

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

//同样，我们不想为每一个区块都保存一个实例，显然上面的类中的所有状态都是固有的(上下文无关的)，我们不使用枚举，而是使用Terrain对象指针来表示地形：

class world{
private:
    Terrain* tiles_[WIDTH][HEIGHT];
    //...
};

//由于地形实例在很多地方使用，如果要动态分配，它们的声明周期会很复杂，所以我们直接在游戏世界中存储它们

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

//然后可以以如下方式来绘制地形：

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

//现在就不需要world中的方法来接触地形(switch-case)属性了，可以直接暴露出Terrain对象

const Terrain& world::getTile(int x, int y)const{
    return *tiles_[x][y];
}

//使用这种方式，world不再与地形的细节耦合了

int cost=world.getTile(x,y).getMovementCost();