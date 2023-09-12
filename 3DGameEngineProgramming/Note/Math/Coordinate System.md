# Coordinate System

DirectX默认使用左手系，OpenGL默认使用右手系。

根据左右手法则确定左右手系各轴方向。

左手系如图：

![left-hand-rule](Assets\left-hand-rule.jpg)

右手系如图：

<img src="Assets\right-hand-rule.jpg" alt="right-hand-rule"  />

## Direction of Ratation

对于左手坐标系，确定一个旋转轴后，使用左手握紧拳头，拇指指向旋转轴的正方向，四指弯曲的方向即旋转的正方向。

以上图左手系为例，从正方向向负方向看，旋转正方向为顺时针(clockwise)，从负方向向正方向看，旋转正方向为逆时针(counter-clockwise)。

## Coordinate Space

在3D游戏引擎中，通常会处理几个不同的坐标空间，最常用的空间包括对象空间、世界空间、惯性空间(Inertial space)和相机空间(Camera space)。

### Object Space

对象空间也称为局部空间(local space)或建模空间(modeling space)，对象的顶点是相对他们描述的对象来表示的。

对象空间示例如下：

<img src="Assets\LocalSpaceCoordinateSystem.png" alt="Object Space"  />

### World Space

世界坐标系是描述所有其他对象空间的全局坐标空间。

世界空间示例如下：

<img src="Assets\WorldSpaceCoordinateSystem.png" alt="World Space"  />

### Inertial Space

惯性空间是物体空间和世界空间之间的空间，惯性空间的和对象空间有相同原点，惯性空间的轴与世界空间轴平行。

将点从物体空间变换到惯性空间仅需要旋转，将点从惯性空间变换到世界空间仅需要平移。

惯性空间示例如下：

<img src="Assets\IntertialSpaceCoordinateSystem.png" alt="Intertial space"  />

### Camera Space

相机空间是和观察者相关联的坐标空间，相机空间被认为是我们正在看向物体的原点，相机的坐标轴通常表现为$+X$轴指向右，$+Y$轴指向上，$+Z$轴指向前方(forward)。而在右手系中，$Z$轴相反，$-Z$轴指向前方。
