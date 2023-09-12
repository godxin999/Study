# Matrix

## Determinant

$$
\left\vert \boldsymbol{M} \right\vert=\sum_{j=1}^{n}m_{ij}(-1)^{i+j}M_{ij}=\sum_{j=1}^{n}m_{ij}C_{ij} 
$$

其中 $M_{ij}$表示去除了 $i$ 行和 $j$ 列的矩阵的行列式，即余子式(minor)，$C_{ij}$为矩阵的代数余子式(cofactor)，其由余子式计算而来。

## Inverse

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

## Orthogonal Matrix

如果一个方阵是正交矩阵当且仅当 $\boldsymbol{M}\boldsymbol{M}^{\mathrm{T}}=\boldsymbol{I}$，故正交矩阵的逆矩阵等于其转置。

正交矩阵满足如下性质：
1. 矩阵的每列均为单位向量
2. 矩阵的列必须相互垂直
