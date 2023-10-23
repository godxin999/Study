# Engine Support Systems

## 6.2 Memory Management

内存影响性能的两个方式：

1. 动态内存分配很慢，使用定制内存分配器/完全避免动态内存分配(不现实)
2. 内存访问模式(内存布局)主导性能，CPU可以更有效地操作位于小的连续的内存块中的数据(数据不能分散到很多内存地址中，影响性能)

### 6.2.1 Optimizing Dynamic Memory Allocation

**经验法则**：将堆分配保持在最低限度，永远不要在紧密的循环中进行堆分配

#### 6.2.1.1 Stack-Based Allocators

当一个游戏关卡被加载，都会为其分配内存，加载关卡后，几乎不会发生内存分配，在关卡结束时，将数据卸载并把其分配的所有内存释放。

















## Words

be dictated by 由...决定

utilize 利用

altogether 完全地

custom 定制的

be dominated by 由...主导

contiguous 邻近的

utmost 极度的

be brought to one's knees 由于某种压力/困难而变得无法工作

be attributed to 归因于

inherently 天生的

extraordinarily 格外地

rule of thumb 经验法则

conclusion 结尾

maintain 维护















