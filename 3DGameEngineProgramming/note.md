# 3D游戏引擎编程学习笔记

[toc]

## Math

### Coordinate System

DirectX默认使用左手系，OpenGL默认使用右手系。

根据左右手法则确定左右手系各轴方向。

左手系如图：

![left-hand-rule](Assets\left-hand-rule.jpg)

右手系如图：

<img src="Assets\right-hand-rule.jpg" alt="right-hand-rule"  />

### Direction of Ratation

对于左手坐标系，确定一个旋转轴后，使用左手握紧拳头，拇指指向旋转轴的正方向，四指弯曲的方向即旋转的正方向。

以上图左手系为例，从正方向向负方向看，旋转正方向为顺时针(clockwise)，从负方向向正方向看，旋转正方向为逆时针(counter-clockwise)。

### Coordinate Space

在3D游戏引擎中，通常会处理几个不同的坐标空间，最常用的空间包括对象空间、世界空间、惯性空间(Inertial space)和相机空间(Camera space)。

#### Object Space

对象空间也称为局部空间(local space)或建模空间(modeling space)，对象的顶点是相对他们描述的对象来表示的。

对象空间示例如下：

<img src="Assets\LocalSpaceCoordinateSystem.png" alt="Object Space"  />

#### World Space

世界坐标系是描述所有其他对象空间的全局坐标空间。

世界空间示例如下：

<img src="Assets\WorldSpaceCoordinateSystem.png" alt="World Space"  />

#### Inertial Space

惯性空间是物体空间和世界空间之间的空间，惯性空间的和对象空间有相同原点，惯性空间的轴与世界空间轴平行。

将点从物体空间变换到惯性空间仅需要旋转，将点从惯性空间变换到世界空间仅需要平移。

惯性空间示例如下：

<img src="Assets\IntertialSpaceCoordinateSystem.png" alt="Intertial space"  />

#### Camera Space

相机空间是和观察者相关联的坐标空间，相机空间被认为是我们正在看向物体的原点，相机的坐标轴通常表现为$+X$轴指向右，$+Y$轴指向上，$+Z$轴指向前方(forward)。而在右手系中，$Z$轴相反，$-Z$轴指向前方。

### Linear Tansformation

线性空间定义为两个向量空间$V$和$W$之间的变换，表示为$ T:V\rightarrow W$，其支持

1. $T(v_1+v_2)=T(v_1)+T(v_2)$ 对于任意向量 $v_1$、$v_2$
2. $T(\alpha v)=\alpha T(v)$ 对于任意标量(scalar) $\alpha$

这表示任何线性变换都会将零向量变换为零向量，所以平移不是线性变换，因为其将零向量变为了非零向量。

#### Rotation

绕$X$、$Y$、$Z$轴旋转的旋转公式如下，在计算时只需要根据坐标系确定旋转正方向，并将旋转角度带入即可。
$$
R_x(\alpha)=\begin{bmatrix}
1&0&0&0\\
0&{\cos\alpha}&{-\sin\alpha}&0\\
0&{\sin\alpha}&{\cos\alpha}&0\\
0&0&0&1\\
\end{bmatrix}\\
R_y(\alpha)=\begin{bmatrix}
{\cos\alpha}&0&{\sin\alpha}&0\\
0&1&0&0\\
{-\sin\alpha}&0&{\cos\alpha}&0\\
0&0&0&1\\
\end{bmatrix}\\
R_z(\alpha)=\begin{bmatrix}
{\cos\alpha}&{-\sin\alpha}&0&0\\
{\sin\alpha}&{\cos\alpha}&0&0\\
0&0&1&0\\
0&0&0&1\\
\end{bmatrix}
$$

##### Vector triple product

向量三重积定义如下：
$$
\boldsymbol{a}\times(\boldsymbol{b}\times \boldsymbol{c})=(\boldsymbol{a}\cdot \boldsymbol{c})\cdot \boldsymbol{b}-(\boldsymbol{a}\cdot \boldsymbol{b})\cdot \boldsymbol{c} 
$$

下证明：

首先，叉积满足如下性质：
1. $\boldsymbol{i}\times \boldsymbol{j}=-\boldsymbol{j}\times \boldsymbol{i}$
2. $\boldsymbol{i}\times \boldsymbol{i}=0$
3. $\boldsymbol{a}\times (\boldsymbol{b}+\boldsymbol{c})=\boldsymbol{a}\times \boldsymbol{b}+\boldsymbol{a}\times \boldsymbol{c}$

在右手系中，三个坐标轴对应的单位向量为 $\boldsymbol{i}$、$\boldsymbol{j}$、$\boldsymbol{k}$，其满足
$$
\boldsymbol{i}\times \boldsymbol{j}=\boldsymbol{k}\\
\boldsymbol{j}\times \boldsymbol{k}=\boldsymbol{i}\\
\boldsymbol{k}\times \boldsymbol{i}=\boldsymbol{j}
$$

对任意一个三维向量，其可以表现为三个基向量的线性组合，如：
$$
\boldsymbol{a}=a_x \boldsymbol{i}+a_y \boldsymbol{j}+a_z \boldsymbol{k}
$$

故叉积可以表示为如下形式：
$$
\begin{aligned}
\boldsymbol{a}\times \boldsymbol{b}&=(a_x \boldsymbol{i}+a_y \boldsymbol{j}+a_z \boldsymbol{k})\times( b_x \boldsymbol{i}+b_y \boldsymbol{j}+b_z \boldsymbol{k})
\\&=(a_{y}b_{z}-a_{z}b_{y})\boldsymbol{i}+(a_{z}b_{x}-a_{x}b_{z})\boldsymbol{j}+(a_{x}b_{y}-a_{y}b_{x})\boldsymbol{k}\\
&=\begin{vmatrix} \boldsymbol{i} & \boldsymbol{j} & \boldsymbol{k} \\ a_{x} & a_{y} & a_{z} \\ b_{x} & b_{y} & b_{z} \\\end{vmatrix}
\end{aligned}
$$

进一步可以见叉积表示为矩阵和向量相乘的形式，因为 $\boldsymbol{a}\times \boldsymbol{b}=(a_{y}b_{z}-a_{z}b_{y},a_{z}b_{x}-a_{x}b_{z},a_{x}b_{y}-a_{y}b_{x})^{\mathrm{T}}$，故叉积可以表示为：
$$
\begin{aligned}
\boldsymbol{a}\times \boldsymbol{b}&=[\boldsymbol{a}]_{\times }\boldsymbol{b}=\begin{bmatrix} 0 & -a_{z} & a_{y} \\ a_{z} & 0 & -a_{x} \\ -a_{y} & a_{x} & 0 \\\end{bmatrix}\begin{bmatrix} b_{x} \\ b_{y} \\ b_{z} \\\end{bmatrix}
&=[\boldsymbol{b}]_{\times }\boldsymbol{a}=\begin{bmatrix} 0 & b_{z} & -b_{y} \\ -b_{z} & 0 & b_{x} \\ b_{y} & -b_{x} & 0 \\\end{bmatrix}\begin{bmatrix} a_{x} \\ a_{y} \\ a_{z} \\\end{bmatrix}
\end{aligned}=-\boldsymbol{b}\times \boldsymbol{a}
$$

由上式可得
$$
\begin{aligned}
\boldsymbol{a}\times (\boldsymbol{b}\times \boldsymbol{c})&=[\boldsymbol{a}]_{\times }([\boldsymbol{b}]_{\times }\boldsymbol{c})\\
&=\begin{bmatrix} 0 & -a_{z} & a_{y} \\ a_{z} & 0 & -a_{x} \\ -a_{y} & a_{x} & 0 \\\end{bmatrix}\begin{bmatrix} 0 & -b_{z} & b_{y} \\ b_{z} & 0 & -b_{x} \\ -b_{y} & b_{x} & 0 \\\end{bmatrix}\begin{bmatrix} c_{x} \\ c_{y} \\ c_{z} \\\end{bmatrix}\\
&=\begin{bmatrix}b_{x}(a_{y} c_{y} + a_{z} c_{z}) - c_{x} (a_{y} b_{y} + a_{z} b_{z})\\b_{y}(a_{x} c_{x} + a_{z} c_{z}) - c_{y} (a_{x} b_{x} + a_{z} b_{z})\\b_{z}(a_{x} c_{x} + a_{y} c_{y}) - c_{z} (a_{x} b_{x} + a_{y} b_{y})\end{bmatrix}\\
&=\begin{bmatrix}b_{x}(a_{x} c_{x} + a_{y} c_{y} + a_{z} c_{z}) - c_{x} (a_{x} b_{x} + a_{y} b_{y} + a_{z} b_{z})\\b_{y}(a_{x} c_{x} + a_{y} c_{y} + a_{z} c_{z}) - c_{y} (a_{x} b_{x} + a_{y} b_{y} + a_{z} b_{z})\\b_{z}(a_{x} c_{x} + a_{y} c_{y} + a_{z} c_{z}) - c_{z} (a_{x} b_{x} + a_{y} b_{y} + a_{z} b_{z})\end{bmatrix}\\
&=(\boldsymbol{a}\cdot \boldsymbol{c})\cdot \boldsymbol{b}-(\boldsymbol{a}\cdot \boldsymbol{b})\cdot \boldsymbol{c}
\end{aligned}
$$

##### Rodrigues' rotation formula

对于三位空间中的向量 $\boldsymbol{v}$ ，设旋转轴为 $\boldsymbol{k}$ 且为单位向量，旋转角为 $\theta$，则旋转后的向量 $\boldsymbol{v}_{rot}$为：
$$
\boldsymbol{v}_{rot}=\cos\theta\boldsymbol{v}+(1-\cos\theta)(\boldsymbol{k}\cdot \boldsymbol{v})\boldsymbol{k}+\sin\theta \boldsymbol{k}\times \boldsymbol{v}
$$

下证明：

对于向量 $\boldsymbol{v}$，我们可以将其分解为平行于 $\boldsymbol{k}$ 的分量 $\boldsymbol{v}_{\parallel}$ 和正交于 $\boldsymbol{k}$ 的分量 $\boldsymbol{v}_{\perp}$，因为 $\boldsymbol{k}$ 为单位向量，故可得：
$$
\boldsymbol{v}_{\parallel}=(\boldsymbol{v}\cdot \boldsymbol{k})\boldsymbol{k}\\
\boldsymbol{v}_{\perp}=\boldsymbol{v}-\boldsymbol{v}_{\parallel}=\boldsymbol{v}-(\boldsymbol{v}\cdot \boldsymbol{k})\boldsymbol{k}
$$

由向量三重积可得:
$$
\begin{aligned}
\boldsymbol{v}-(\boldsymbol{v}\cdot \boldsymbol{k})\boldsymbol{k}&=(\boldsymbol{k}\cdot \boldsymbol{k})\boldsymbol{v}-(\boldsymbol{v}\cdot \boldsymbol{k})\boldsymbol{k}\\
&=\boldsymbol{k}\times (\boldsymbol{v}\times \boldsymbol{k})\\
&=-\boldsymbol{k}\times (\boldsymbol{k}\times \boldsymbol{v})
\end{aligned}
$$

对于旋转后的向量 $\boldsymbol{v}_{rot}$，其平行于旋转轴的分量不会改变，其正交于旋转轴的分量大小不变，方向改变，即
$$
\boldsymbol{v}_{\parallel rot}=\boldsymbol{v}_{\parallel}\\
\left\vert \boldsymbol{v}_{\perp rot} \right\vert =\left\vert \boldsymbol{v}_{\perp} \right\vert 
$$

下图为旋转公式的几何图示

<img src="Assets\rodrigues_rotation_formula.png" alt="Assets/Rodrigues rotation formula" style="zoom: 75%;" />

根据几何图示，我们可以得到：
$$
\boldsymbol{v}_{\perp rot}=\cos\theta\boldsymbol{v}_{\perp}+\sin\theta \boldsymbol{k}\times \boldsymbol{v}
$$

故
$$
\begin{aligned}
\boldsymbol{v}_{rot}&=\boldsymbol{v}_{\parallel rot}+\boldsymbol{v}_{\perp rot}\\
&=(\boldsymbol{v}\cdot \boldsymbol{k})\boldsymbol{k}+\cos\theta(\boldsymbol{v}-(\boldsymbol{v}\cdot \boldsymbol{k})\boldsymbol{k})+\sin \theta \boldsymbol{k}\times \boldsymbol{v}\\
&=\cos\theta \boldsymbol{v}+(1-\cos\theta)(\boldsymbol{v}\cdot \boldsymbol{k})\boldsymbol{k}+\sin \theta \boldsymbol{k} \times \boldsymbol{v}
\end{aligned}
$$
 
罗德里格斯旋转公式还可以表示为矩阵形式，令 $\boldsymbol{v}_{rot}=\boldsymbol{R}\boldsymbol{v}$，其中 $\boldsymbol{R}$ 为旋转矩阵，两端同时乘以 $\boldsymbol{v}$ 的转置，则

$$
\begin{aligned}
\boldsymbol{R}\boldsymbol{v}\boldsymbol{v}^{\mathrm{T}}&=\cos\theta \boldsymbol{v}\boldsymbol{v}^{\mathrm{T}}+(1-\cos\theta)(\boldsymbol{v}\cdot \boldsymbol{k})\boldsymbol{k}\boldsymbol{v}^{\mathrm{T}}+\sin \theta \boldsymbol{k}\times \boldsymbol{v}\boldsymbol{v}^{\mathrm{T}}\\
&=\cos\theta \boldsymbol{v}\boldsymbol{v}^{\mathrm{T}}+(1-\cos\theta)\boldsymbol{k}\boldsymbol{k}^{\mathrm{T}}\boldsymbol{v}\boldsymbol{v}^{\mathrm{T}}+\sin \theta [\boldsymbol{k}]_{\times }\boldsymbol{v}\boldsymbol{v}^{\mathrm{T}}
\end{aligned}
$$

然后两侧同时乘以 $\boldsymbol{v}\boldsymbol{v}^{\mathrm{T}}$ 的逆，则
$$
\boldsymbol{R}=\cos\theta \boldsymbol{I}+(1-\cos\theta)\boldsymbol{k}\boldsymbol{k}^{\mathrm{T}}+\sin \theta [\boldsymbol{k}]_{\times }\\
[\boldsymbol{k}]_{\times }=\begin{bmatrix} 0 & -k_{z} & k_{y} \\ k_{z} & 0 & -k_{x} \\ -k_{y} & k_{x} & 0 \\\end{bmatrix}
$$

结合向量三重积，可得另一形式为：
$$
\begin{aligned}
\boldsymbol{v}_{rot}&=\boldsymbol{v}_{\parallel rot}+\boldsymbol{v}_{\perp rot}\\
&=\boldsymbol{v}_{\parallel}+\cos\theta\boldsymbol{v}_{\perp}+\sin \theta \boldsymbol{k}\times \boldsymbol{v}\\
&=\boldsymbol{v}-\boldsymbol{v}_{\perp}+\cos\theta\boldsymbol{v}_{\perp}+\sin \theta \boldsymbol{k}\times \boldsymbol{v}\\
&=\boldsymbol{v}+\sin \theta \boldsymbol{k}\times \boldsymbol{v}+(\cos \theta-1)\boldsymbol{v}_{\perp}\\
&=\boldsymbol{v}+\sin \theta \boldsymbol{k}\times \boldsymbol{v}+(1-\cos \theta)\boldsymbol{k}\times (\boldsymbol{k}\times \boldsymbol{v})\\
&=\boldsymbol{v}+\sin \theta [\boldsymbol{k}]_{\times }\boldsymbol{v}+(1-\cos \theta)[\boldsymbol{k}]_{\times }^{2}\boldsymbol{v}
\end{aligned}
$$

#### Scale

设伸缩因子为 $k$，沿任意方向 $\boldsymbol{n}$，其中 $\boldsymbol{n}$为单位向量的缩放矩阵的推导如下：

对于任意向量 $\boldsymbol{v}$，因为其沿着 $\boldsymbol{n}$ 进行伸缩，伸缩因子为 $k$，且 $\boldsymbol{v}$ 正交于 $\boldsymbol{n}$ 的分量不受缩放的影响，可得：

$$
\boldsymbol{v}_{\parallel scale}=k(\boldsymbol{v}\cdot \boldsymbol{n})\boldsymbol{n}\\
\boldsymbol{v}_{\perp scale}=\boldsymbol{v}_{\perp}
$$

又因为 $\boldsymbol{v}_{\perp}=\boldsymbol{v}-\boldsymbol{v}_{\parallel}$，故

$$
\begin{aligned}
\boldsymbol{v}_{scale}&=\boldsymbol{v}_{\parallel scale}+
\boldsymbol{v}_{\perp scale}\\
&=k(\boldsymbol{v}\cdot \boldsymbol{n})\boldsymbol{n}+\boldsymbol{v}-\boldsymbol{v}\cdot \boldsymbol{n}\\
&=\boldsymbol{v}+(k-1)(\boldsymbol{v}\cdot \boldsymbol{n})\boldsymbol{n}
\end{aligned}
$$

同理可推出矩阵形式为：

$$
\boldsymbol{S}=\boldsymbol{I}+(k-1)\boldsymbol{n}\boldsymbol{n}^{\mathrm{T}}
$$

### Matrix

#### Determinant

$$
\left\vert \boldsymbol{M} \right\vert=\sum_{j=1}^{n}m_{ij}(-1)^{i+j}M_{ij}=\sum_{j=1}^{n}m_{ij}C_{ij} 
$$

其中 $M_{ij}$表示去除了 $i$ 行和 $j$ 列的矩阵的行列式，即余子式(minor)，$C_{ij}$为矩阵的代数余子式(cofactor)，其由余子式计算而来。

#### Inverse

如果方阵的行列式等于零，那么就称方阵可逆。可逆矩阵也被称为非奇异(non-singular)矩阵。

求取矩阵逆的一般方程为：

$$
\boldsymbol{M}^{-1}=\frac{adj(\boldsymbol{M})}{\det(\boldsymbol{M}) }=\frac{\boldsymbol{A}^{*}}{\left\vert \boldsymbol{A} \right\vert }
$$

其中 $adj(\boldsymbol{M})$ 为矩阵的经典伴随(classical adjoint)，也称为矩阵的伴随(adjugate)。

伴随矩阵的计算方法为：

$$
adj(\boldsymbol{M})=\boldsymbol{C}^{\mathrm{T}}
$$

其中 $\boldsymbol{C}$ 为矩阵 $\boldsymbol{M}$ 的余子矩阵，其每个元素为矩阵 $\boldsymbol{M}$ 对应的代数余子式，记作 $cof(\boldsymbol{M})$。

#### Orthogonal Matrix

如果一个方阵是正交矩阵当且仅当 $\boldsymbol{M}\boldsymbol{M}^{\mathrm{T}}=\boldsymbol{I}$，故正交矩阵的逆矩阵等于其转置。

正交矩阵满足如下性质：
1. 矩阵的每列均为单位向量
2. 矩阵的列必须相互垂直

### Camera Transformation

相机变换即将相机从相机的对象空间变换到世界空间的变换，其与普通的模型矩阵相同。相机变换可以拆解成旋转变换和平移变换的复合，因为我们使用的是列向量，且应该先旋转再平移，故相机的变换矩阵为

$$
\boldsymbol{M}=\boldsymbol{T}\boldsymbol{R}
$$

### View Matrix

视图矩阵用于将坐标从世界空间转换到视图空间，即从世界坐标系到相机坐标系的基变换矩阵。相机通过相机变换矩阵从相机坐标系变换到了全局坐标系，即

$$
\boldsymbol{v}_{world}=\boldsymbol{M}\boldsymbol{v}_{camera}
$$

对于两个坐标系的标准正交基来说，相机变换矩阵 $\boldsymbol{M}$ 即为从相机坐标系变换到世界坐标系的基变换矩阵。

视图矩阵正好相反，其为从世界坐标系到相机坐标系的基变换矩阵，即

$$
\boldsymbol{v}_{camera}=\boldsymbol{V}\boldsymbol{v}_{world}
$$

所以

$$
\boldsymbol{V}=\boldsymbol{M}^{-1}=(\boldsymbol{T}\boldsymbol{R})^{-1}=\boldsymbol{R}^{-1}\boldsymbol{T}^{-1}
$$


#### Look At Camera


















































## DirectX12

### DirectX12 Graphics Pipeline

DirectX12图形管线由许多阶段组成，其中某些阶段是固定的，只能通过DirectX12 API 的函数来进行配置，其他阶段是可以通过着色器程序来编程控制。下图概述了DirectX12图形管线的各个阶段，其中箭头表示来自各阶段以及高速GPU内存中可用的内存资源的数据流，例如：缓冲区、纹理和常量缓冲区。

<img src="Assets\DirectX-11-Rendering-Pipeline.png" alt="DirectX 12 Graphics Pipeline" style="zoom: 50%;" />

其中蓝色矩形代表固定功能的阶段，绿色圆角矩形代表图形管线的可编程阶段。

#### Input-Assembler Stage

输入装配器阶段**IA**用于从用户定义的顶点缓冲区和索引缓冲区中读取图元(primitive)数据，并将这些数据装配为几何图元(geometric primitive)，例如：线列表、三角形列表、三角形带等

#### Vertex Shader Stage

顶点着色器阶段**VS**负责将顶点数据从对象空间(object-space)转化到(clip-space)。顶点着色器还可以用于执行(骨骼)动画或计算每个顶点的光照。顶点着色器阶段是定义有效管线状态对象绝对需要的唯一着色器阶段。

#### Hull Shader Stage

外壳着色器阶段**HS**是可选的着色器阶段，其负责确定输入的控制面片(control patch)在细分阶段的细分程度。

#### Tessellator Stage

曲面细分阶段是一个固定功能阶段，其根据外壳着色器阶段指定的曲面细分因子(tessellation factors)将面片图元细分为更小的图元。

#### Domain Shader Stage

域着色器阶段**DS**是可选的着色器阶段，其负责根据外壳着色器阶段输出的控制点和曲面细分阶段的插值坐标(interpolation coordinate)来计算最终的顶点属性。

#### Geometry Shader Stage

几何着色器阶段**GS**是可选的着色器阶段，他将单个几何图元作为输入，并可以丢弃(discard)图元、将图元转换为其他图元类型或者生成其他图元。

#### Stream Output Stage

流输出阶段**SO**是一个可选的固定功能阶段，其可以将图元数据送回GPU内存。这些数据可以循环回渲染管线并由另一组着色器进行处理，这对于在粒子效果(particle effect)中生成或终止粒子非常有用。

#### Rasterizer Stage

光栅化阶段**RS**是一个固定功能的阶段，其将图元裁剪到视锥体(frustum)中，并执行剔除(culling)操作。光栅化阶段还会沿着每个图元的面(across the face of each primitive)对每个顶点属性进行插值，并把插值结果传递给像素着色器。

#### Pixel Shader Stage

像素着色器阶段**PS**负责从光栅化阶段获取每个顶点的插值并产生一个（或多个）每像素颜色值。像素着色器对图元覆盖的每个像素都被调用（invoke）一次。

#### Output-Merger Stage

输出合并阶段**OM**将各个类型的输出数据与当前绑定的渲染目标结合在一起，生成最终的管线结果。

### GPU Synchronization

#### Fence、CommandQueue

<img src="Assets\GPU-Synchronization.png" alt="GPU synchronization" style="zoom: 80%;" />

在上图中，主线程发出了多条命令。对于该图，第一帧表示为 `Frame N`，命令列表在命令队列中执行。执行命令列表后，队列会立即调用`Signal`将栅栏值插入命令队列，值为`N`。当命令队列到达该点(`Signal(N)`)时，栅栏的完成值(completed value)将被设为`N`。

调用`Signal`后，会有一条`WaitForFenceValue`命令用于等待上一帧`Frame N-1`结束。由于`Frame N-1`的命令队列中没有任何命令，因此执行将继续进行，CPU 线程不会停滞(没有红色的等待部分)。

然后在 CPU 线程上构建`Frame N+1`，并在`DirectQueue`上执行。在这种情况下，CPU必须等待栅栏值到达`N`(标红的为等待部分)，这表示命令队列已使用完这些资源。

命令队列使用完`Frame N`的资源后，就可以在队列上构建并执行`Frame N+2`。如果队列仍在处理`Frame N+1`的命令，CPU就必须再次等待这些资源可用后才能继续。

在使用多个队列的情况下，应该确保每个命令队列都跟踪自己的栅栏对象和栅栏值。

<img src="Assets\GPU-Synchronization-2.png" alt="GPU Synchronization-2" style="zoom:80%;" />

从该图我们可以看到，CPU会执行`Frame N`的命令列表，并调用`Signal`向`DirectQueue`插入栅栏值`N`。同时，CPU 还会向`ComputeQueue`发出调度命令，并调用`Signal`插入栅栏值`N+1`。

如果`ComputeQueue`在`DirectQueue`到达`Signal(N)`之前到达`Signal(N+1)`，那么栅栏的完成值将设置为`N+1`。由于`N`小于`N+1`，栅栏的完成值减小，从而导致了错误。

