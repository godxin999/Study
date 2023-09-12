# Quaternion

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

## Quaternion Addition and Subtraction

四元数进行加减时，标量部分直接相加减，向量部分各分量进行加减。

$$
\begin{aligned}
q_{a}&=[s_{a},\boldsymbol{a}]\\
q_{b}&=[s_{b},\boldsymbol{b}]\\
q_{a}+q_{b}&=[s_{a}+s_{b},\boldsymbol{a}+\boldsymbol{b}]\\
q_{a}-q_{b}&=[s_{a}-s_{b},\boldsymbol{a}-\boldsymbol{b}]\\
\end{aligned}
$$

## Quaternion product

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

## Real Quaternion

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

## Pure Quaternion

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

## Additive Form of a Quaternion

我们可以将四元数表示为一个实四元数和一个纯四元数相加的形式。

$$
\begin{aligned}
q&=[s,\boldsymbol{v}]\\
&=[s,\boldsymbol{0}]+[0,\boldsymbol{v}]
\end{aligned}
$$

## Quaternion Norm

和复数类似，四元数的范数定义为：

$$
\begin{aligned}
q&=[s,\boldsymbol{v}]\\
\left\vert \left\vert q \right\vert  \right\vert &= \sqrt{s^{2}+\left\vert \left\vert \boldsymbol{v} \right\vert  \right\vert ^{2}}
\end{aligned}
$$

## Unit Quaternion

范数为 $1$ 的四元数为单位四元数。

## Quaternion Normalization

四元数通过除以其范数进行标准化。

$$
q^{'}=\frac{q}{\left\vert \left\vert q \right\vert  \right\vert }
$$

## Quaternion Conjugate

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

## Quaternion Inverse

四元数的逆定义如下：

$$
q^{-1}=\frac{q^{*}}{\left\vert \left\vert q \right\vert  \right\vert ^{2 }}
$$

对于范数为 $1$ 的单位四元数，我们可以得到

$$
q^{-1}=q^{*}
$$

## Quaternion Dot Product

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


## Quaternion and Rotation 

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

### Rotation of $v_{\perp}$

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

### Rotation of $v_{\parallel}$

对于平行于旋转轴的分量 $v_{\parallel}$，其满足：

$$
\boldsymbol{v}_{\parallel rot}=\boldsymbol{v}_{\parallel}
$$

所以可以得到平行情况下的四元数旋转公式为：

$$
v_{\parallel rot}=v_{\parallel}
$$

### Rotation of $v$

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

### Quaternion Rotation Composition

对于两个表示沿不同轴旋转不同角度的四元数 $q_{1},q_{2}$ ，我们先对 $v$ 进行 $q_{1}$ 的变换。

$$
v^{'}=q_{1}vq_{1}^{*}
$$

然后进行 $q_{2}$ 的变换。

$$
\begin{aligned}
v^{''}&=q_{2}v^{'}q_{2}^{*}\\
&=q_{2}q_{1}vq_{1}^{*}q_{2}^{*}
\end{aligned}
$$

> Lemma 4
> 对于任意四元数 $q_{1}=[s,\boldsymbol{v}],q_{2}=[t,\boldsymbol{u}]$ ，则
> $$ 
> q_{1}^{*}q_{2}^{*}=(q_{2}q_{1})^{*}
> $$

首先计算等式左侧：

$$
\begin{aligned}
lhs&=[s,-\boldsymbol{v}][t,-\boldsymbol{u}]\\
&=[st-\boldsymbol{u}\cdot \boldsymbol{v},-s \boldsymbol{u}-t \boldsymbol{v}+\boldsymbol{v}\times \boldsymbol{u}]
\end{aligned}
$$

然后计算等式右侧：

$$
\begin{aligned}
rhs&=([t,\boldsymbol{u}][s,\boldsymbol{v}])^{*}\\
&=([st-\boldsymbol{u}\cdot \boldsymbol{v},s \boldsymbol{u}+t \boldsymbol{v}+\boldsymbol{u}\times \boldsymbol{v}])^{*}\\
&=[st-\boldsymbol{u}\cdot \boldsymbol{v},-s \boldsymbol{u}-t \boldsymbol{v}+\boldsymbol{v}\times \boldsymbol{u}]=lhs
\end{aligned}
$$

所以我们可以对这两个变换进行复合，得到等价形式为：

$$
v^{''}=q_{e}vq_{e}^{*}\quad \quad (q_{e}=(q_{1}q_{2}))
$$





































