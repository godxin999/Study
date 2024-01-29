# 第七章

## 7.9 物理反射模型

### 7.9.1 BRDF(Bidirectional Reflectance Distribution Function)

单位时间内光源辐射出的能量或者物体表面吸收的能量称为光通(radiant flux)，单位为瓦特($W$)。单位面积、单位时间内光源辐射出的能量或者物体表面吸收的能量称为光通强度(irradiance)，单位为瓦特每平方米($W\cdot m^{-2}$)。物体表面发射出的光通强度称为表面的辐射强度，照射到表面的光通强度称为表面的辐照强度。

表面接收到的能量和光源发出的能量相同，但是根据朗博漫反射效应，表面接收到的光通强度和光源发出的光通强度是不一样的，设表面面积为 $A$，光源向该表面照射 $P$ 瓦特的能量。设 $\boldsymbol{N}$ 为表面的单位法向量，$\boldsymbol{L}$ 为指向光源的单位向量，则光线的面积为 $A(\boldsymbol{N}\cdot \boldsymbol{L})$，则光源的光通强度为

$$
\Phi_{E}=\frac{P}{A(\boldsymbol{N} \cdot \boldsymbol{L})}
$$

因为照射到物体表面的光通强度等于 $P/A$，所以

$$
\Phi_{I}=\Phi_{E}(\boldsymbol{N} \cdot \boldsymbol{L})
$$

光线照射物体表面的方向使用立体角表示，立体角为平面角的三维扩展，对于平面角，弧长$l=\theta r$，推广到三维，立体角对应球面面积 $A=\omega r^{2}$，因为半径为 $r$ 的球面积为 $4\pi r^{2}$，所以整个球面的立体角为 $4\pi$，单位为立体弧度(球面度) $sr$。

立体角由半径 $r$、方位角 $\theta$ 和极角 $\varphi$ 决定，立体角对应球面面积的微分 $\mathrm{d}A$ 可以理解为沿着极角和方位角方向弧长的微分的乘积。

![方位角](assets/note/%E6%96%B9%E4%BD%8D%E8%A7%92.png)

沿极角方向的弧长微分为 $r\mathrm{d}\varphi$，沿方位角方向的弧长所对应的圆的半径为 $r\sin\varphi$，所以沿方位角方向的弧长微分为 $r\sin\varphi\mathrm{d}\theta$，故

$$
\mathrm{d}A=r^{2}\sin\varphi\mathrm{d}\varphi\mathrm{d}\theta
$$

除以 $r^{2}$ 得到立体角微分为：

$$
\mathrm{d}\omega=\sin\varphi\mathrm{d}\varphi\mathrm{d}\theta
$$

辐射度(radiance)表示单位立体角辐射的光通强度，单位为瓦特每立体弧度($W\cdot m^{-2} \cdot sr^{-1}$)。

表面上一个微分面积 $\mathrm{d}A$ 上接收的光线辐照强度 $\Phi_{I}$ 等于该面积射手的辐射度的积分 $C_{I}(L)$，其中指向光源的方向 $\boldsymbol{L}$ 的取址范围为表面上的单位半球 $\Omega$，$\theta$ 和 $\varphi$ 分别为方向 $\boldsymbol{L}$ 方位角和极角。

$$
\begin{aligned}
\Phi_{I} &=\int_{\Omega} C_{I}(\boldsymbol{L}) \mathrm{d} \omega \\
&=\int_{0}^{2 \pi} \int_{0}^{\frac{\pi}{2}} C_{I}(\theta, \varphi) \sin \varphi \mathrm{d} \varphi \mathrm{d} \theta
\end{aligned}
$$

物体表面接收的辐射度 $C_{I}$ 和光源发出的辐射度 $C_{E}$ 满足

$$
C_{I}(\theta, \varphi)=C_{E}(\theta, \varphi)(\boldsymbol{N}\cdot \boldsymbol{L})=C_{E}(\theta, \varphi)\cos \varphi
$$

故

$$
\begin{aligned}
\Phi_{I} &=\int_{\Omega} C_{E}(\boldsymbol{L})(\boldsymbol{L} \cdot \boldsymbol{N}) \mathrm{d} \omega \\
&=\int_{0}^{2 \pi} \int_{0}^{\frac{\pi}{2}} C_{E}(\theta, \varphi) \cos \varphi \sin \varphi \mathrm{d} \varphi \mathrm{d} \theta \\
\end{aligned}
$$

表面上一点的双向反射率 $\rho(\boldsymbol{V},\boldsymbol{L})$ 为观察方向 $\boldsymbol{V}$ 和指向光源的方向 $\boldsymbol{L}$ 的函数，其定义为微分反射辐射度 $\mathrm{d}C_{R}$ 和微分入射辐照度 $\mathrm{d}\Phi_{I}$ 的比值，即

$$
\rho(\boldsymbol{V},\boldsymbol{L})=\frac{\mathrm{d} C_{R}}{\mathrm{d} \Phi_{I}}=\frac{\mathrm{d} C_{R}}{C_{E}(\boldsymbol{L}) (\boldsymbol{L} \cdot \boldsymbol{N})\mathrm{d}\omega}
$$

函数 $\rho(\boldsymbol{V},\boldsymbol{L})$ 即双向反射分布函数，可以使用该函数计算反射光沿某一方向的辐射度，即

$$
\mathrm{d}C_{R}=\rho(\boldsymbol{V},\boldsymbol{L}) C_{E}(\boldsymbol{L}) (\boldsymbol{L} \cdot \boldsymbol{N}) \mathrm{d} \omega
$$

平行光源、点光源和聚光灯均沿着同一方向照射物体表面，所以在计算某一表面相对 $n$ 个光源的反射光照到观察者的辐射度时，直接对离散的方向 $L_{i}$ 进行求和即可。

$$
C_{R}(\boldsymbol{V})=\sum_{i=1}^{n} \rho(\boldsymbol{V}, \boldsymbol{L}_{i}) C_{E}(\boldsymbol{L}_{i})\left(\boldsymbol{L}_{i} \cdot \boldsymbol{N}\right)
$$

