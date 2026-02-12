//===================================================
// 
// 地面のプレイヤーがいる場所にエフェクトを出すシェーダー [field_effect.hlsl]
// Author : YUTO YOSHIDA
//
//===================================================

//***************************************************
// グローバル変数宣言
//***************************************************
texture g_BaseTexture;  // 元のテクスチャ
float2 g_UVPos;         // UV座標
float g_Time;

//***************************************************
// サンプラ
//***************************************************
sampler BaseSampler = sampler_state
{
    Texture = <g_BaseTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

//***************************************************
// シェーダーに渡すための情報
//***************************************************
struct VS_INPUT
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float4 col : COLOR0;
};

//***************************************************
// 出力用変数
//***************************************************
struct VS_OUTPUT
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float4 col : COLOR0;
};

//***************************************************
// 頂点シェーダーの出力処理
//***************************************************
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output; // 出力用変数
    
    output.pos = input.pos;
    output.uv = input.uv;
    output.col = input.col;
    
    return output;
};

//===================================================
// ピクセルシェーダーを設定
//===================================================
float4 VP_Main(VS_OUTPUT output) : COLOR0
{
    // 距離を求める
    float fDistance = distance(output.uv, g_UVPos);
    float fRadius = 0.06f;
    float fInfluence = saturate(1.0f - fDistance / fRadius);
    
    // サインカーブで波を作る
    float fEffect = sin(g_Time * 12.0f + fDistance * 30.0f);
    fEffect = fEffect * 0.5f + 0.5f;
    
    // ベースの色
    float3 fBase = tex2D(BaseSampler, output.uv).rgb;
    float3 fWhite = float3(1.0f, 1.0f, 1.0f);
    
    // 色の強さ
    float fStrength = fEffect * fInfluence;
    
    // 色を設定
    float3 fColor = lerp(fBase, fWhite, fStrength);
    
    fColor = saturate(fColor + 0.5f);
    
    return float4(fColor, 1.0f);
}

//===================================================
// テクニック宣言
//===================================================
technique tech
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 VP_Main();
    }
}