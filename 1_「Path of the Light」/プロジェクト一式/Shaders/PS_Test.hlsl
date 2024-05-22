
Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);


cbuffer Parameters : register(b0)
{
    float4 DiffuseColor             : packoffset(c0);
    float3 EmissiveColor            : packoffset(c1);
    float3 SpecularColor            : packoffset(c2);
    float  SpecularPower : packoffset(c2.w);

    float3 LightDirection[3]        : packoffset(c3);
    float3 LightDiffuseColor[3]     : packoffset(c6);
    float3 LightSpecularColor[3]    : packoffset(c9);

    float3 EyePosition              : packoffset(c12);

    float3 FogColor                 : packoffset(c13);
    float4 FogVector                : packoffset(c14);

    float4x4 World                  : packoffset(c15);
    float3x3 WorldInverseTranspose  : packoffset(c19);
    float4x4 WorldViewProj          : packoffset(c22);
};

cbuffer Parameters : register(b1)
{
    float Att0 : packoffset(c0.x);
    float Att1 : packoffset(c0.y);
    float Att2 : packoffset(c0.z);
    float3 LightPosition : packoffset(c1);
};

struct PSInputPixelLightingTx
{
    float2 TexCoord     : TEXCOORD0;
    float4 PositionWS   : TEXCOORD1;
    float3 NormalWS     : TEXCOORD2;
    float4 Diffuse      : COLOR0;
};

// Pixel shader: pixel lighting.
float4 PSBasicPixelLighting(PSInputPixelLightingTx pin) : SV_Target0
{
    // ���C�g�̕������Z�o����
    float3 lightDirection = pin.PositionWS.xyz - LightPosition;

    // ���C�g�܂ł̋������Z�o����
    float lightDistance = length(lightDirection);

    // ���C�g�̋����ɂ����̌����W�����Z�o����
    float atten = 1 / (Att0 + Att1 * lightDistance + Att2 * lightDistance * lightDistance);
    
    // �e�N�X�`���F �~ Vc���w�肳��Ă���ꍇ�͒��_�J���[ + �A���t�@
    float4 color = Texture.Sample(Sampler, pin.TexCoord) * pin.Diffuse;

    // �@���𐳋K��
    float3 worldNormal = normalize(pin.NormalWS);

    // ���̋�������ς���Z�o����
    float3 dotL = dot(-lightDirection, worldNormal);
    
    // �\�ʂ̏ꍇ�͂P�A���ʂ̏ꍇ��0
    float3 zeroL = step(0, dotL);
    
    // ���ʂ̏ꍇ�͍��ɂȂ�
    float3 diffuse = zeroL * dotL;
    
    // ���C�g�̋����ɂ�錸�����|����
    diffuse *= atten;
    
    // ���̉e�� �~ �}�e���A���̃f�B�t���[�Y�F �{ �A���r�G���g�F�i�G�~�b�V�u�F���܂ށj
    diffuse = diffuse * DiffuseColor.rgb + EmissiveColor;
    
    // ���̉e�����|����
    color.rgb *= diffuse;
    
    // �����x�N�g��
    float3 eyeVector = normalize(EyePosition - pin.PositionWS.xyz);

    // �n�[�t�x�N�g��
    float3 halfVector = normalize(eyeVector - lightDirection);

    // �X�y�L�����̉e����������ς��g�����߂�
    float dotH = dot(halfVector, worldNormal);

    // �X�y�L�����p���[���w���Ƃ��Ďg���n�C���C�g�̂������𒲐�
    float3 specular = pow(max(dotH, 0) * zeroL, SpecularPower) * dotL;

    // ���C�g�̃X�y�L�����F�ƃ}�e���A���̃X�y�L�����F�𔽉f
    specular = specular * LightSpecularColor[0] * SpecularColor;

    // �X�y�L������������
    color.rgb += specular * color.a;
  
    return color;
}
