//像素着色器的作用为生成将要写入当前渲染目标的最终颜色
//像素着色器从光栅化阶段获取插值后的颜色，并使用SV_Target系统语义将颜色输出到唯一绑定的渲染目标


struct PixelShaderInput
{
    float4 Color : COLOR;
};

float4 main(PixelShaderInput IN) : SV_Target
{
    return IN.Color;
}