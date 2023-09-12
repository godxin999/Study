# Tansformation

## Model Transformation

线性空间定义为两个向量空间$V$和$W$之间的变换，表示为$ T:V\rightarrow W$，其支持

1. $T(v_1+v_2)=T(v_1)+T(v_2)$ 对于任意向量 $v_1$、$v_2$
2. $T(\alpha v)=\alpha T(v)$ 对于任意标量(scalar) $\alpha$

这表示任何线性变换都会将零向量变换为零向量，所以平移不是线性变换，因为其将零向量变为了非零向量。

### Rotation

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

#### Vector triple product

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

#### Rodrigues' rotation formula

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

### Scale

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


## Camera Transformation

相机变换即将相机从相机的对象空间变换到世界空间的变换，其与普通的模型矩阵相同。相机变换可以拆解成旋转变换(基变换)和平移变换的复合，因为我们使用的是列向量，且应该先旋转再平移，故相机的变换矩阵为

$$
\boldsymbol{M}=\boldsymbol{T}\boldsymbol{R}
$$

## View Transformation

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

### Look At Camera

这种相机使用相机位置，目标点和世界坐标系向上的单位向量来构建视图矩阵。

设摄像机位置为 $\boldsymbol{e}$，目标点为 $\boldsymbol{t}$，向上的单位向量为 $\boldsymbol{u}$，因为相机看向目标点，所以相机的 $z$ 轴正方向(forward)即为 $\boldsymbol{t}-\boldsymbol{e}$，对应的基向量为 $\boldsymbol{f}=normal(\boldsymbol{t}-\boldsymbol{e})$。

然后叉乘向上的向量和相机 $z$ 轴得到相机的 $x$ 轴正方向(right)，对应的基向量为 $\boldsymbol{r}=normal(\boldsymbol{u}\times \boldsymbol{f})$。

随后叉乘相机 $z$ 轴正方向和相机 $x$ 轴正方向的基向量得到相机 $y$ 轴正方向的基向量为 $\boldsymbol{u}=\boldsymbol{f}\times \boldsymbol{r}$。

向量 $\boldsymbol{r},\boldsymbol{u},\boldsymbol{f}$ 构成标准正交基，然后只需要按照上面的视图矩阵的计算方式进行计算即可。注意这里使用的是左手坐标系，右手坐标系中需要将向量 $\boldsymbol{t}-\boldsymbol{e}$ 的取反，即将 $z$ 轴正方向(视线方向进行翻转)。传统的UVN相机坐标系使用的就是右手坐标系。

### FPS Camera

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

### Arcball Orbit Camera

