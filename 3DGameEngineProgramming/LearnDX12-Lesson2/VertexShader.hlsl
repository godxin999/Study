//顶点着色器用于接收顶点数据，并对顶点属性执行变换，以便为渲染流水线的下一阶段生成顶点属性


struct VertexPosColor
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};

struct ModelViewProjection
{
    matrix MVP;
};

ConstantBuffer<ModelViewProjection> ModelViewProjectionCB : register(b0);//常量缓冲区

struct VertexShaderOutput
{
    float4 Color : COLOR;
    float4 Position : SV_Position;
};

VertexShaderOutput main(VertexPosColor IN)
{
    VertexShaderOutput OUT;
    /*
    如果矢量的最后一个分量为0，那么这个矢量将被旋转但不会被平移
    如果顶点属性是一个位置矢量(点，对应的为方向向量)，那么最后一个分量一定为1
    如果顶点属性是一个法向量，那么最后一个分量一定为0
    */
    OUT.Position = mul(ModelViewProjectionCB.MVP, float4(IN.Position, 1.0f));
    /*
    在HLSL中，可以使用行向量或列向量来表示顶点位置
    如果使用行向量，则应将其右乘模型视图投影矩阵，即mul(float4(IN.Position, 1.0f), ModelViewProjectionCB.MVP)
    如果您使用列向量，则应将其左乘模型视图投影矩阵，即mul(ModelViewProjectionCB.MVP, float4(IN.Position, 1.0f))
    这两种方法都可以得到相同的结果
    */
    
    OUT.Color = float4(IN.Color, 1.0f);//函数式类型转换
    /*
    颜色值的最后一个分量被称为alpha值，它表示颜色的不透明度
    */
    
    return OUT;
}