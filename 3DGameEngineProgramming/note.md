# 3D游戏引擎编程学习笔记

[toc]

## Math

### Convention

1. 矩阵为列主矩阵，矩阵可以看作由多个列向量组成，且不具体到API实现进行讨论，如DirectX使用行主序进行存储。
2. 使用列向量并采用左乘(pre-multiply)进行变换(行向量需要右乘(post-multiply)进行变换)。
3. 除少数例子外均使用左手坐标系。

### Coordinate System

DirectX默认使用左手系，OpenGL默认使用右手系。

根据左右手法则确定左右手系各轴方向。

左手系如图：

![left-hand-rule](Assets\left-hand-rule.jpg)

右手系如图：

<img src="Assets\right-hand-rule.jpg" alt="right-hand-rule"  />

#### Direction of Ratation

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

### Quaternion

四元数的一般形式为

$$
q=s+x \boldsymbol{i}+y \boldsymbol{j}+z \boldsymbol{k}\ s,x,y,z \in \mathbb{R}
$$

其中 $\boldsymbol{i},\boldsymbol{j},\boldsymbol{k}$ 具有如下性质

$$
\boldsymbol{i}^{2}=\boldsymbol{j}^{2}=\boldsymbol{k}^{2}=\boldsymbol{i}\boldsymbol{j}\boldsymbol{k}=-1
$$

并且

$$
\boldsymbol{ij}=\boldsymbol{k}\quad \boldsymbol{jk}=\boldsymbol{i}\quad \boldsymbol{ki}=\boldsymbol{j}\\
\boldsymbol{ji}=-\boldsymbol{k}\quad \boldsymbol{kj}=-\boldsymbol{i}\quad \boldsymbol{ik}=-\boldsymbol{j}\\
$$

我们还可以将四元数表示为一个标量和一个三维向量的有序对

$$
q=[s,\boldsymbol{v}]\ s \in \mathbb{R},\boldsymbol{V} \in \mathbb{R}^{3}
$$

其中向量部分 $\boldsymbol{v}$ 还可以由各个分量进行表示

$$
q=[s,x \boldsymbol{i}+y \boldsymbol{j}+z \boldsymbol{k}]\ s,x,y,z \in \mathbb{R}
$$

#### Quaternion Addition and Subtraction

四元数进行加减时，标量部分直接相加减，向量部分各分量进行加减。

$$
\begin{aligned}
q_{a}&=[s_{a},\boldsymbol{a}]\\
q_{b}&=[s_{b},\boldsymbol{b}]\\
q_{a}+q_{b}&=[s_{a}+s_{b},\boldsymbol{a}+\boldsymbol{b}]\\
q_{a}-q_{b}&=[s_{a}-s_{b},\boldsymbol{a}-\boldsymbol{b}]\\
\end{aligned}
$$

#### Quaternion product

我们可以将四元数乘以一个标量，其规则如下：

$$
\begin{aligned}
q&=[s,\boldsymbol{v}]\\
\lambda q&=\lambda [s,\boldsymbol{v}]\\
&=[\lambda s,\lambda \boldsymbol{v}]
\end{aligned}
$$

然后我们计算两个四元数的乘积

$$
\begin{aligned}
q_{a}&=[s_{a},\boldsymbol{a}]\\
q_{b}&=[s_{b},\boldsymbol{b}]\\
q_{a}q_{b}&=[s_{a},\boldsymbol{a}][s_{b},\boldsymbol{b}]\\
&=(s_{a}+x_{a}\boldsymbol{i}+y_{a}\boldsymbol{j}+z_{a}\boldsymbol{k})(s_{b}+x_{b}\boldsymbol{i}+y_{b}\boldsymbol{j}+z_{b}\boldsymbol{k})\\
&=(s_{a}s_{b}-x_{a}x_{b}-y_{a}y_{b}-z_{a}z_{b})\\
&\quad +(s_{a}x_{b}+s_{b}x_{a}+y_{a}z_{b}-y_{b}z_{a})\boldsymbol{i}\\
&\quad +(s_{a}y_{b}+s_{b}y_{a}+z_{a}x_{b}-z_{b}x_{a})\boldsymbol{j}\\
&\quad +(s_{a}z_{b}+s_{b}z_{a}+x_{a}y_{b}-x_{b}y_{a})\boldsymbol{k}\\
&=[s_{a}s_{b}-x_{a}x_{b}-y_{a}y_{b}-z_{a}z_{b},\\
&\quad \ s_{b}(x_{a}\boldsymbol{i}+y_{a}\boldsymbol{j}+z_{a}\boldsymbol{k})+s_{a}(x_{b}\boldsymbol{i}+y_{b}\boldsymbol{j}+z_{b}\boldsymbol{k})\\
&\quad +(y_{a}z_{b}-y_{b}z_{a})\boldsymbol{i}+(z_{a}x_{b}-z_{b}x_{a})\boldsymbol{j}+(x_{a}y_{b}-x_{b}y_{a})\boldsymbol{k}]\\
&=[s_{a}s_{b}-\boldsymbol{a}\cdot \boldsymbol{b},s_{a}\boldsymbol{b}+s_{b}\boldsymbol{a}+\boldsymbol{a}\times \boldsymbol{b}]
\end{aligned}
$$

#### Real Quaternion

实四元数为向量部分为 $0$ 的四元数。

$$
q=[s,\boldsymbol{0}]
$$

两个实四元数的乘积也是一个实四元数。

$$
\begin{aligned}
q_{a}&=[s_{a},\boldsymbol{0}]\\
q_{b}&=[s_{b},\boldsymbol{0}]\\
q_{a}q_{b}&=[s_{a}s_{b},\boldsymbol{0}]
\end{aligned}
$$

#### Pure Quaternion

纯四元数为标量部分为 $0$ 的四元数。

$$
q=[0,\boldsymbol{v}]
$$

两个纯四元数的乘积为：

$$
\begin{aligned}
q_{a}&=[0,\boldsymbol{a}]\\
q_{b}&=[0,\boldsymbol{b}]\\
q_{a}q_{b}&=[0,\boldsymbol{a}][0,\boldsymbol{b}]\\
&=[-\boldsymbol{a}\cdot \boldsymbol{b},\boldsymbol{a}\times \boldsymbol{b}]
\end{aligned}
$$

#### Additive Form of a Quaternion

我们可以将四元数表示为一个实四元数和一个纯四元数相加的形式。

$$
\begin{aligned}
q&=[s,\boldsymbol{v}]\\
&=[s,\boldsymbol{0}]+[0,\boldsymbol{v}]
\end{aligned}
$$

#### Quaternion Norm

和复数类似，四元数的范数定义为：

$$
\begin{aligned}
q&=[s,\boldsymbol{v}]\\
\left\vert \left\vert q \right\vert  \right\vert &= \sqrt{s^{2}+\left\vert \left\vert \boldsymbol{v} \right\vert  \right\vert ^{2}}
\end{aligned}
$$

#### Unit Quaternion

范数为 $1$ 的四元数为单位四元数。

#### Quaternion Normalization

四元数通过除以其范数进行标准化。

$$
q^{'}=\frac{q}{\left\vert \left\vert q \right\vert  \right\vert }
$$

#### Quaternion Conjugate

四元数的共轭通过对四元数的向量部分求反获得。

$$
\begin{aligned}
q&=[s,\boldsymbol{v}]\\
q^{*}&=[s,-\boldsymbol{v}]
\end{aligned}
$$

四元数与其共轭的乘积为：

$$
\begin{aligned}
qq^{*}&=[s,\boldsymbol{v}][s,-\boldsymbol{v}]\\
&=[s^{2}+\left\vert \left\vert \boldsymbol{v} \right\vert  \right\vert ^{2},\boldsymbol{0}]\\
&=\left\vert \left\vert q \right\vert  \right\vert ^{2}
\end{aligned}
$$

#### Quaternion Inverse

四元数的逆定义如下：

$$
q^{-1}=\frac{q^{*}}{\left\vert \left\vert q \right\vert  \right\vert ^{2 }}
$$

对于范数为 $1$ 的单位四元数，我们可以得到

$$
q^{-1}=q^{*}
$$

#### Quaternion Dot Product

和向量点积相似，我们可以通过以下方式来计算两个四元数的点积。

$$
\begin{aligned}
q_{a}&=[s_{a},x_{a}\boldsymbol{i}+y_{a}\boldsymbol{j}+z_{a}\boldsymbol{k}]\\
q_{b}&=[s_{b},x_{b}\boldsymbol{i}+y_{b}\boldsymbol{j}+z_{b}\boldsymbol{k}]\\
q_{a}\cdot q_{b}&=s_{a}s_{b}+x_{a}x_{b}+y_{a}y_{b}+z_{a}z_{b}
\end{aligned}
$$

我们还可以使用四元数点积来计算四元数之间的角度差：

$$
\cos \theta=\frac{q_{a}\cdot q_{b}}{\left\vert \left\vert q_{a} \right\vert  \right\vert \left\vert \left\vert q_{b} \right\vert  \right\vert }
$$

对于单位四元数，我们可以把方程简化为：

$$
\cos \theta = s_{a}s_{b}+x_{a}x_{b}+y_{a}y_{b}+z_{a}z_{b}
$$


#### Quaternion and Rotation 

对于三位空间中的向量 $\boldsymbol{v}$ ，设旋转轴为 $\boldsymbol{n}$ 且为单位向量，旋转角为 $\theta$，我们可以得到：

$$
\begin{aligned}
\boldsymbol{v}_{\parallel rot}&=\boldsymbol{v}_{\parallel}\\
\boldsymbol{v}_{\perp rot}&=\cos\theta\boldsymbol{v}_{\perp}+\sin\theta \boldsymbol{n}\times \boldsymbol{v}_{\perp}\\
\end{aligned}
$$

我们可以将这些向量定义为纯四元数。

$$
\begin{aligned}
v&=[0,\boldsymbol{v}]\,\quad\quad v_{rot}=[0,\boldsymbol{v}_{rot}]\\
v_{\perp}&=[0,\boldsymbol{v}_{\perp}]\quad v_{\perp rot}=[0,\boldsymbol{v}_{\perp rot}]\\
v_{\parallel}&=[0,\boldsymbol{v}_{\parallel}]\,\ \quad v_{\parallel rot}=[0,\boldsymbol{v}_{\parallel rot}]\\
n&=[0,\boldsymbol{n}]
\end{aligned}
$$

四元数同样满足

$$
v=v_{\parallel}+v_{\perp}\quad v_{rot}=v_{\parallel rot}+v_{\perp rot}
$$

##### Rotation of $v_{\perp}$

对于正交于旋转轴的分量 $\boldsymbol{v}_{\perp}$，其满足：

$$
\boldsymbol{v}_{\perp rot}=\cos\theta\boldsymbol{v}_{\perp}+\sin\theta \boldsymbol{n}\times \boldsymbol{v}_{\perp}\\
$$

对于纯四元数，$v_{\perp}=[0,\boldsymbol{v}_{\perp}],n=[0,\boldsymbol{n}]$，那么我们可以得到:

$$
\begin{aligned}
nv_{\perp}&=[-\boldsymbol{v}_{\perp}\cdot \boldsymbol{n},\boldsymbol{n}\times \boldsymbol{v}_{\perp}]\\
&=[0,\boldsymbol{n}\times \boldsymbol{v}_{\perp}]\\
&=\boldsymbol{n}\times \boldsymbol{v}_{\perp}
\end{aligned}
$$

将上述式子代入，得到

$$
v_{\perp rot}=\cos \theta v_{\perp}+\sin \theta  nv_{\perp}
$$

因为四元数的乘法遵守分配律，所以上式可变换为：

$$
v_{\perp rot}=(\cos \theta +\sin \theta  n)v_{\perp}
$$

此时如果把 $\cos \theta +\sin \theta  n$ 看作是一个四元数，我们就可以把旋转写成四元数的乘积，即：

$$
v_{\perp rot}=qv_{\perp}
$$

其中

$$
\begin{aligned}
q&=\cos \theta +\sin \theta n\\
&=[\cos \theta,\boldsymbol{0}]+[0,\sin \theta \boldsymbol{n}]\\
&=[\cos \theta,\sin \theta \boldsymbol{n}]\\
&=\cos \theta +\sin \theta n_{x}\boldsymbol{i}+\sin \theta n_{y}\boldsymbol{j}+\sin \theta n_{z}\boldsymbol{k}
\end{aligned}
$$

至此得到了正交情况下的四元数旋转公式。

此外四元数 $q$ 还是一个单位四元数

$$
\begin{aligned}
\left\vert \left\vert q \right\vert  \right\vert &=\sqrt{\cos ^{2} \theta +(\sin \theta \boldsymbol{n}\cdot \sin \theta \boldsymbol{n})}\\
&=\sqrt{\cos ^{2} \theta +\sin ^{2} \theta (\boldsymbol{n}\cdot  \boldsymbol{n})}\\
&=\sqrt{\cos ^{2} \theta +\sin ^{2} \theta (\left\vert \left\vert \boldsymbol{n} \right\vert  \right\vert )}\\
&=\sqrt{\cos ^{2} \theta +\sin ^{2} \theta}\\
&=1
\end{aligned}
$$

##### Rotation of $v_{\parallel}$

对于平行于旋转轴的分量 $v_{\parallel}$，其满足：

$$
\boldsymbol{v}_{\parallel rot}=\boldsymbol{v}_{\parallel}
$$

所以可以得到平行情况下的四元数旋转公式为：

$$
v_{\parallel rot}=v_{\parallel}
$$

##### Rotation of $v$

在以上推导的基础上，我们可以得到普通情况下的四元数旋转公式为：

$$
\begin{aligned}
v_{rot}&=v_{\perp rot}+v_{\parallel rot}\\
&=qv_{\perp}+v_{\parallel}\quad \quad (q=[\cos \theta,\sin \theta \boldsymbol{n}])
\end{aligned}
$$

我们将使用几个引理对这个公式进行化简。

> Lemma 1
> 如果 $q=[\cos \theta,\sin \theta \boldsymbol{n}]$ ，$\boldsymbol{n}$ 为单位向量，那么 $q^{2}=qq=[\cos 2\theta,\sin 2\theta \boldsymbol{n}]$

因为 $q=[\cos \theta,\sin \theta \boldsymbol{n}]$，所以

$$
\begin{aligned}
q^{2}=qq&=[\cos \theta,\sin \theta \boldsymbol{n}]\cdot [\cos \theta,\sin \theta \boldsymbol{n}]\\
&=[\cos^{2}\theta-\sin^{2}\theta,2\sin \theta \cos \theta \boldsymbol{n}]\\
&=[\cos 2\theta,\sin 2\theta \boldsymbol{n}]
\end{aligned}
$$

其几何意义为，如果绕着同一个轴 $\boldsymbol{n}$ 连续旋转 $\theta$ 度两次，那么所作出的变换等同于直接绕着轴 $\boldsymbol{n}$ 旋转 $2\theta$ 度。

此外，根据四元数逆的定义：

$$
qq^{-1}=1
$$

旋转公式可变化为：

$$
\begin{aligned}
v_{rot}&=qv_{\perp}+v_{\parallel}\quad \quad (q=[\cos \theta,\sin \theta \boldsymbol{n}])\\
&=qv_{\perp}+1\cdot v_{\parallel}\\
&=ppv_{\perp}+pp^{-1}v_{\parallel} \quad \quad (q=p^{2},p=[\cos \frac{\theta}{2},\sin \frac{\theta}{2}\boldsymbol{n}])
\end{aligned}
$$

和 $q$ 一样，四元数 $p$ 也是一个单位四元数，所以可以得到

$$
p^{-1}=p^{*}
$$

所以旋转公式变化为；

$$
\begin{aligned}
v_{rot}&=ppv_{\perp}+pp^{-1}v_{\parallel}\\
&=ppv_{\perp}+pp^{*}v_{\parallel}
\end{aligned}
$$

> Lemma 2
> 假设 $v_{\parallel}=[0,\boldsymbol{v}_{\parallel}]$ 是一个纯四元数，而 $q=[\alpha,\beta \boldsymbol{n}]$，其中 $\boldsymbol{n}$ 为单位向量，$\alpha,\beta \in \mathbb{R}$，在这种条件下，如果 $v_{\parallel}$ 平行于 $\boldsymbol{n}$ ，那么 $qv_{\parallel}=v_{\parallel}q$

因为 $\boldsymbol{v}_{\parallel}\parallel \boldsymbol{n}$，所以 $\boldsymbol{n}\times \boldsymbol{v}_{\parallel}=0$，所以等式左侧为：

$$
\begin{aligned}
lhs&=qv_{\parallel}\\
&=[\alpha,\beta \boldsymbol{n}][0,\boldsymbol{v}_{\parallel}]\\
&=[-\beta \boldsymbol{n}\cdot \boldsymbol{v}_{\parallel},\alpha \boldsymbol{v}_{\parallel}]
\end{aligned}
$$

然后计算等式右侧：

$$
\begin{aligned}
rhs&=v_{\parallel}q\\
&=[0,\boldsymbol{v}_{\parallel}][\alpha,\beta \boldsymbol{n}]\\
&=[-\beta \boldsymbol{n}\cdot \boldsymbol{v}_{\parallel},\alpha \boldsymbol{v}_{\parallel}]=lhs
\end{aligned}
$$

> Lemma 3
> 假设 $v_{\perp}=[0,\boldsymbol{v}_{\perp}]$ 是一个纯四元数，而 $q=[\alpha,\beta \boldsymbol{n}]$，其中 $\boldsymbol{n}$ 为单位向量，$\alpha,\beta \in \mathbb{R}$，在这种条件下，如果 $v_{\perp}$ 正交于 $\boldsymbol{n}$ ，那么 $qv_{\perp}=v_{\parallel}q^{*}$

等式左侧：

$$
\begin{aligned}
lhs&=qv_{\perp}\\
&=[\alpha,\beta \boldsymbol{n}][0,\boldsymbol{v}_{\perp}]\\
&=[0,\alpha \boldsymbol{v}_{\perp}+\beta \boldsymbol{n}\times \boldsymbol{v}_{\perp}]
\end{aligned}
$$

等式右侧：

$$
\begin{aligned}
lhs&=v_{\perp}q^{*}\\
&=[0,\boldsymbol{v}_{\perp}][\alpha,-\beta \boldsymbol{n}]\\
&=[0,\alpha \boldsymbol{v}_{\perp}+\beta \boldsymbol{n}\times \boldsymbol{v}_{\perp}]=lhs
\end{aligned}
$$

所以旋转公式最终可以变换为：

$$
\begin{aligned}
v_{rot}&=ppv_{\perp}+pp^{*}v_{\parallel}\\
&=pv_{\perp}p^{*}+pv_{\parallel}p^{*}\\
&=p(v_{\perp}+v_{\parallel})p^{*}\\
&=pvp^{*}
\end{aligned}
$$

即，当向量 $\boldsymbol{v}$ 绕旋转轴 $\boldsymbol{n}$ 旋转 $\theta$ 度，$\boldsymbol{n}$ 为单位向量，令 $v=[0,\boldsymbol{v}],q=[\cos \frac{\theta}{2},\sin \frac{\theta}{2} \boldsymbol{n}]$，则

$$
v_{rot}=qvq^{*}=qvq^{-1}
$$

因为所有旋转四元数的标量部分都是一个角度的余弦值，所以如果有一个单位四元数 $q=[a,\boldsymbol{b}]$，那么我们可以得到：

$$
\frac{\theta}{2}=\cos ^{-1} (a)
$$

如果想要得到旋转轴，则需要将 $\boldsymbol{b}$ 的每一项都除以 $\sin \frac{\theta}{2}$ 即可。

$$
\boldsymbol{n}=\frac{\boldsymbol{b}}{\sin(\cos^{-1}(a))}
$$

### Tansformation

#### Model Transformation

线性空间定义为两个向量空间$V$和$W$之间的变换，表示为$ T:V\rightarrow W$，其支持

1. $T(v_1+v_2)=T(v_1)+T(v_2)$ 对于任意向量 $v_1$、$v_2$
2. $T(\alpha v)=\alpha T(v)$ 对于任意标量(scalar) $\alpha$

这表示任何线性变换都会将零向量变换为零向量，所以平移不是线性变换，因为其将零向量变为了非零向量。

##### Rotation

绕$X$、$Y$、$Z$轴旋转的旋转公式如下，在计算时只需要根据坐标系确定旋转正方向，并将旋转角度带入即可。
$$
R_x(\alpha)=\begin{bmatrix}
1&0&0&0\\
0&\cos\alpha&-\sin\alpha&0\\
0&\sin\alpha&\cos\alpha&0\\
0&0&0&1\\
\end{bmatrix}\\
R_y(\alpha)=\begin{bmatrix}
\cos\alpha&0&\sin\alpha&0\\
0&1&0&0\\
-\sin\alpha&0&\cos\alpha&0\\
0&0&0&1\\
\end{bmatrix}\\
R_z(\alpha)=\begin{bmatrix}
\cos\alpha&-\sin\alpha&0&0\\
\sin\alpha&\cos\alpha&0&0\\
0&0&1&0\\
0&0&0&1\\
\end{bmatrix}
$$

###### Vector triple product

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

###### Rodrigues' rotation formula

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

所以可以以向量起点为圆心，$\left\vert \boldsymbol{v}_{\perp rot} \right\vert $为半径画圆，旋转轴 $\boldsymbol{k}$ 垂直于该圆。

下图为右手系下旋转公式的几何图示

<img src="Assets\rodrigues_rotation_formula.png" alt="Assets/Rodrigues rotation formula" style="zoom: 75%;" />

根据几何图示，我们可以得到：

$$
\begin{aligned}
\boldsymbol{v}_{\perp rot}&=\cos\theta\boldsymbol{v}_{\perp}+\sin\theta \boldsymbol{k}\times \boldsymbol{v}_{\perp}\\
&=\cos\theta\boldsymbol{v}_{\perp}+\sin\theta \boldsymbol{k}\times \boldsymbol{v}\\
\end{aligned}
$$

对于左手系 $\boldsymbol{k}\times \boldsymbol{v}$ 的方向相反，旋转角度为 $-\theta$ ，结果相同。

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

##### Scale

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


#### Camera Transformation

相机变换即将相机从相机的对象空间变换到世界空间的变换，其与普通的模型矩阵相同。相机变换可以拆解成旋转变换(基变换)和平移变换的复合，因为我们使用的是列向量，且应该先旋转再平移，故相机的变换矩阵为

$$
\boldsymbol{M}=\boldsymbol{T}\boldsymbol{R}
$$

#### View Transformation

视图矩阵用于将坐标从世界空间转换到视图空间，即从世界坐标系到相机坐标系的变换矩阵。相机通过相机变换矩阵从相机坐标系变换到了世界坐标系，即

$$
\boldsymbol{v}_{world}=\boldsymbol{M}\boldsymbol{v}_{camera}
$$

而视图矩阵正好相反，其为从世界坐标系到相机坐标系的变换矩阵，即

$$
\boldsymbol{v}_{camera}=\boldsymbol{V}\boldsymbol{v}_{world}
$$

其图示如下：

<img src="Assets\cam_xform.jpg" alt="View Transformation"  />

所以 

$$
\boldsymbol{V}=\boldsymbol{M}^{-1}=(\boldsymbol{T}\boldsymbol{R})^{-1}=\boldsymbol{R}^{-1}\boldsymbol{T}^{-1}
$$

因为求相机变换矩阵更方便，所以先求相机变换矩阵再求其逆矩阵，首先是旋转变换(基变换)，对于世界坐标系，其标准正交基为 $(1,0,0)^{\mathrm{T}},(0,1,0)^{\mathrm{T}},(0,0,1)^{\mathrm{T}}$，设相机坐标系的标准正交基为 $\boldsymbol{r},\boldsymbol{u},\boldsymbol{f}$，位置为 $\boldsymbol{p}$ 则

$$
(\boldsymbol{r},\boldsymbol{u},\boldsymbol{f},1)=
\begin{bmatrix} \boldsymbol{r}_{x} & \boldsymbol{u}_{x} & \boldsymbol{f}_{x} & 0 \\ \boldsymbol{r}_{y} & \boldsymbol{u}_{y} & \boldsymbol{f}_{y} & 0 \\ \boldsymbol{r}_{z} & \boldsymbol{u}_{z} & \boldsymbol{f}_{z} & 0 \\ 0 & 0 & 0 & 1 \\\end{bmatrix}
\begin{bmatrix} 1 & 0 & 0 & 0 \\ 0 & 1 & 0 & 0 \\ 0 & 0 & 1 & 0 \\ 0 & 0 & 0 & 1 \\\end{bmatrix}
$$

所以旋转矩阵(基变换矩阵)为

$$
\boldsymbol{R}=
\begin{bmatrix} \boldsymbol{r}_{x} & \boldsymbol{u}_{x} & \boldsymbol{f}_{x} & 0 \\ \boldsymbol{r}_{y} & \boldsymbol{u}_{y} & \boldsymbol{f}_{y} & 0 \\ \boldsymbol{r}_{z} & \boldsymbol{u}_{z} & \boldsymbol{f}_{z} & 0 \\ 0 & 0 & 0 & 1 \\\end{bmatrix}
$$

因为旋转矩阵都是正交矩阵，其逆矩阵等于其转置，故

$$
\boldsymbol{R}^{-1}=\boldsymbol{R}^{\mathrm{T}}=
\begin{bmatrix} \boldsymbol{r}_{x} & \boldsymbol{r}_{y} & \boldsymbol{r}_{z} & 0 \\ \boldsymbol{u}_{x} & \boldsymbol{u}_{y} & \boldsymbol{u}_{z} & 0 \\ \boldsymbol{f}_{x} & \boldsymbol{f}_{y} & \boldsymbol{f}_{z} & 0 \\ 0 & 0 & 0 & 1 \\\end{bmatrix}
$$

然后为平移矩阵，因为相机的位置为 $\boldsymbol{p}$，故把相机从原点平移到 $\boldsymbol{p}$ 的矩阵为

$$
\boldsymbol{T}=\begin{bmatrix} 1 & 0 & 0 & \boldsymbol{p}_{x} \\ 0 & 1 & 0 & \boldsymbol{p}_{y} \\ 0 & 0 & 1 & \boldsymbol{p}_{z} \\ 0 & 0 & 0 & 1 \\\end{bmatrix}
$$

故

$$
\boldsymbol{T}^{-1}=\begin{bmatrix} 1 & 0 & 0 & -\boldsymbol{p}_{x} \\ 0 & 1 & 0 & -\boldsymbol{p}_{y} \\ 0 & 0 & 1 &- \boldsymbol{p}_{z} \\ 0 & 0 & 0 & 1 \\\end{bmatrix}
$$

所以视图矩阵为

$$
\boldsymbol{V}=\boldsymbol{R}^{-1}\boldsymbol{T}^{-1}=
\begin{bmatrix} \boldsymbol{r}_{x} & \boldsymbol{r}_{y} & \boldsymbol{r}_{z} & -\boldsymbol{r}\cdot \boldsymbol{p} \\ \boldsymbol{u}_{x} & \boldsymbol{u}_{y} & \boldsymbol{u}_{z} & -\boldsymbol{u}\cdot \boldsymbol{p} \\ \boldsymbol{f}_{x} & \boldsymbol{f}_{y} & \boldsymbol{f}_{z} & -\boldsymbol{f}\cdot \boldsymbol{p} \\ 0 & 0 & 0 & 1 \\\end{bmatrix}
$$

##### Look At Camera

这种相机使用相机位置，目标点和世界坐标系向上的单位向量来构建视图矩阵。

设摄像机位置为 $\boldsymbol{e}$，目标点为 $\boldsymbol{t}$，向上的单位向量为 $\boldsymbol{u}$，因为相机看向目标点，所以相机的 $z$ 轴正方向(forward)即为 $\boldsymbol{t}-\boldsymbol{e}$，对应的基向量为 $\boldsymbol{f}=normal(\boldsymbol{t}-\boldsymbol{e})$。

然后叉乘向上的向量和相机 $z$ 轴得到相机的 $x$ 轴正方向(right)，对应的基向量为 $\boldsymbol{r}=normal(\boldsymbol{u}\times \boldsymbol{f})$。

随后叉乘相机 $z$ 轴正方向和相机 $x$ 轴正方向的基向量得到相机 $y$ 轴正方向的基向量为 $\boldsymbol{u}=\boldsymbol{f}\times \boldsymbol{r}$。

向量 $\boldsymbol{r},\boldsymbol{u},\boldsymbol{f}$ 构成标准正交基，然后只需要按照上面的视图矩阵的计算方式进行计算即可。注意这里使用的是左手坐标系，右手坐标系中需要将向量 $\boldsymbol{t}-\boldsymbol{e}$ 的取反，即将 $z$ 轴正方向(视线方向进行翻转)。传统的UVN相机坐标系使用的就是右手坐标系。

##### FPS Camera

对于FPS游戏，一般不会使用Look-At方式来计算视图矩阵，而是使用一组欧拉角来构造视图矩阵，我们一般也不希望相机绕 $z$ 轴进行旋转变换，所以翻滚角一般为零。

我们约定采用 $zxy$ 顺规，并使用外旋，则旋转矩阵可以写成

$$
\begin{aligned}
\boldsymbol{R}(\boldsymbol{\alpha},\boldsymbol{\beta},\boldsymbol{\gamma})&=\boldsymbol{R}_{y}(\boldsymbol{\alpha})\boldsymbol{R}_{x}(\boldsymbol{\beta})\boldsymbol{R}_{z}(\boldsymbol{\gamma})\\
&=\begin{bmatrix}
\cos\alpha&0&\sin\alpha&0\\
0&1&0&0\\
-\sin\alpha&0&\cos\alpha&0\\
0&0&0&1\\
\end{bmatrix}
\begin{bmatrix}
1&0&0&0\\
0&\cos\beta&-\sin\beta&0\\
0&\sin\beta&\cos\beta&0\\
0&0&0&1\\
\end
{bmatrix}
\begin{bmatrix}
{\cos\gamma}&{-\sin\gamma}&0&0\\
{\sin\gamma}&{\cos\gamma}&0&0\\
0&0&1&0\\
0&0&0&1\\
\end{bmatrix}
\end{aligned}
$$

因为滚转角 $\gamma$ 为零，所以旋转矩阵可化简为

$$
\begin{aligned}
\boldsymbol{R}(\boldsymbol{\alpha},\boldsymbol{\beta})
&=\begin{bmatrix}
\cos\alpha&0&\sin\alpha&0\\
0&1&0&0\\
-\sin\alpha&0&\cos\alpha&0\\
0&0&0&1\\
\end{bmatrix}
\begin{bmatrix}
1&0&0&0\\
0&\cos\beta&-\sin\beta&0\\
0&\sin\beta&\cos\beta&0\\
0&0&0&1\\
\end{bmatrix}\\
&=\begin{bmatrix}\cos \alpha & \sin \alpha \sin \beta & \sin \alpha \cos \beta & 0\\0 & \cos \beta & - \sin \beta & 0\\- \sin \alpha & \sin \beta \cos \alpha & \cos \alpha \cos \beta & 0\\0 & 0 & 0 & 1\end{bmatrix}
\end{aligned}
$$

因为旋转矩阵是正交矩阵，所以该旋转矩阵同时为世界坐标系到相机坐标系的基变换矩阵，其中矩阵的前三列构成标准正交基，之后只需要按照上面的视图矩阵的计算方式进行计算即可。

##### Arcball Orbit Camera










































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

