# DirectX12 学习笔记

[toc]

## DirectX12 Graphics Pipeline

DirectX12图形管线由许多阶段组成，其中某些阶段是固定的，只能通过DirectX12 API 的函数来进行配置，其他阶段是可以通过着色器程序来编程控制。下图概述了DirectX12图形管线的各个阶段，其中箭头表示来自各阶段以及高速GPU内存中可用的内存资源的数据流，例如：缓冲区、纹理和常量缓冲区。

<img src="Assets\DirectX-11-Rendering-Pipeline.png" alt="DirectX 12 Graphics Pipeline" style="zoom: 50%;" />

其中蓝色矩形代表固定功能的阶段，绿色圆角矩形代表图形管线的可编程阶段。

### Input-Assembler Stage

输入装配器阶段**IA**用于从用户定义的顶点缓冲区和索引缓冲区中读取图元(primitive)数据，并将这些数据装配为几何图元(geometric primitive)，例如：线列表、三角形列表、三角形带等

### Vertex Shader Stage

顶点着色器阶段**VS**负责将顶点数据从对象空间(object-space)转化到(clip-space)。顶点着色器还可以用于执行(骨骼)动画或计算每个顶点的光照。顶点着色器阶段是定义有效管线状态对象绝对需要的唯一着色器阶段。

### Hull Shader Stage

外壳着色器阶段**HS**是可选的着色器阶段，其负责确定输入的控制面片(control patch)在细分阶段的细分程度。

### Tessellator Stage

曲面细分阶段是一个固定功能阶段，其根据外壳着色器阶段指定的曲面细分因子(tessellation factors)将面片图元细分为更小的图元。

### Domain Shader Stage

域着色器阶段**DS**是可选的着色器阶段，其负责根据外壳着色器阶段输出的控制点和曲面细分阶段的插值坐标(interpolation coordinate)来计算最终的顶点属性。

### Geometry Shader Stage

几何着色器阶段**GS**是可选的着色器阶段，他将单个几何图元作为输入，并可以丢弃(discard)图元、将图元转换为其他图元类型或者生成其他图元。

### Stream Output Stage

流输出阶段**SO**是一个可选的固定功能阶段，其可以将图元数据送回GPU内存。这些数据可以循环回渲染管线并由另一组着色器进行处理，这对于在粒子效果(particle effect)中生成或终止粒子非常有用。

### Rasterizer Stage

光栅化阶段**RS**是一个固定功能的阶段，其将图元裁剪到视锥体(frustum)中，并执行剔除(culling)操作。光栅化阶段还会沿着每个图元的面(across the face of each primitive)对每个顶点属性进行插值，并把插值结果传递给像素着色器。

### Pixel Shader Stage

像素着色器阶段**PS**负责从光栅化阶段获取每个顶点的插值并产生一个（或多个）每像素颜色值。像素着色器对图元覆盖的每个像素都被调用（invoke）一次。

### Output-Merger Stage

输出合并阶段**OM**将各个类型的输出数据与当前绑定的渲染目标结合在一起，生成最终的管线结果。

## GPU Synchronization

### Fence、CommandQueue

<img src="Assets\GPU-Synchronization.png" alt="GPU synchronization" style="zoom: 80%;" />

在上图中，主线程发出了多条命令。对于该图，第一帧表示为 `Frame N`，命令列表在命令队列中执行。执行命令列表后，队列会立即调用`Signal`将栅栏值插入命令队列，值为`N`。当命令队列到达该点(`Signal(N)`)时，栅栏的完成值(completed value)将被设为`N`。

调用`Signal`后，会有一条`WaitForFenceValue`命令用于等待上一帧`Frame N-1`结束。由于`Frame N-1`的命令队列中没有任何命令，因此执行将继续进行，CPU 线程不会停滞(没有红色的等待部分)。

然后在 CPU 线程上构建`Frame N+1`，并在`DirectQueue`上执行。在这种情况下，CPU必须等待栅栏值到达`N`(标红的为等待部分)，这表示命令队列已使用完这些资源。

命令队列使用完`Frame N`的资源后，就可以在队列上构建并执行`Frame N+2`。如果队列仍在处理`Frame N+1`的命令，CPU就必须再次等待这些资源可用后才能继续。

在使用多个队列的情况下，应该确保每个命令队列都跟踪自己的栅栏对象和栅栏值。

<img src="Assets\GPU-Synchronization-2.png" alt="GPU Synchronization-2" style="zoom:80%;" />

从该图我们可以看到，CPU会执行`Frame N`的命令列表，并调用`Signal`向`DirectQueue`插入栅栏值`N`。同时，CPU 还会向`ComputeQueue`发出调度命令，并调用`Signal`插入栅栏值`N+1`。如果`ComputeQueue`在`DirectQueue`到达`Signal(N)`之前到达`Signal(N+1)`，那么栅栏的完成值将设置为`N+1`。由于`N`小于`N+1`，栅栏的完成值减小，从而导致了错误。

