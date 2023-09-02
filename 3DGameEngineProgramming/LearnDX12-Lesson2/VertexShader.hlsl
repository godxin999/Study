//������ɫ�����ڽ��ն������ݣ����Զ�������ִ�б任���Ա�Ϊ��Ⱦ��ˮ�ߵ���һ�׶����ɶ�������


struct VertexPosColor
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};

struct ModelViewProjection
{
    matrix MVP;
};

ConstantBuffer<ModelViewProjection> ModelViewProjectionCB : register(b0);//����������

struct VertexShaderOutput
{
    float4 Color : COLOR;
    float4 Position : SV_Position;
};

VertexShaderOutput main(VertexPosColor IN)
{
    VertexShaderOutput OUT;
    /*
    ���ʸ�������һ������Ϊ0����ô���ʸ��������ת�����ᱻƽ��
    �������������һ��λ��ʸ��(�㣬��Ӧ��Ϊ��������)����ô���һ������һ��Ϊ1
    �������������һ������������ô���һ������һ��Ϊ0
    */
    OUT.Position = mul(ModelViewProjectionCB.MVP, float4(IN.Position, 1.0f));
    /*
    ��HLSL�У�����ʹ��������������������ʾ����λ��
    ���ʹ������������Ӧ�����ҳ�ģ����ͼͶӰ���󣬼�mul(float4(IN.Position, 1.0f), ModelViewProjectionCB.MVP)
    �����ʹ������������Ӧ�������ģ����ͼͶӰ���󣬼�mul(ModelViewProjectionCB.MVP, float4(IN.Position, 1.0f))
    �����ַ��������Եõ���ͬ�Ľ��
    */
    
    OUT.Color = float4(IN.Color, 1.0f);//����ʽ����ת��
    /*
    ��ɫֵ�����һ����������Ϊalphaֵ������ʾ��ɫ�Ĳ�͸����
    */
    
    return OUT;
}