//������ɫ��������Ϊ���ɽ�Ҫд�뵱ǰ��ȾĿ���������ɫ
//������ɫ���ӹ�դ���׶λ�ȡ��ֵ�����ɫ����ʹ��SV_Targetϵͳ���彫��ɫ�����Ψһ�󶨵���ȾĿ��


struct PixelShaderInput
{
    float4 Color : COLOR;
};

float4 main(PixelShaderInput IN) : SV_Target
{
    return IN.Color;
}