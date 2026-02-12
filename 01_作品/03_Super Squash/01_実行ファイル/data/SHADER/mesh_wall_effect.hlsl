//===================================================
// 
// 壁のエフェクトのシェーダー [mesh_wall_effect.hlsl]
// Author : YUTO YOSHIDA
//
//===================================================

//***************************************************
// グローバル変数宣言
//***************************************************
texture g_WallTexture;  // 壁のテクスチャ
float4x4 g_mtxWVP;      // ワールド×ビュー×プロジェクションのマトリックス
float3 g_LightDir001;   // ライトの方向1
float3 g_LightDir002;   // ライトの方向2
float3 g_LightDir003;   // ライトの方向3

float3 g_LightColor001; // ライトの色1
float3 g_LightColor002; // ライトの色2
float3 g_LightColor003; // ライトの色3

float g_Time;           // 時間

//***************************************************
// 入力頂点シェーダー
//***************************************************
struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float4 col : COLOR;
    float2 uv : TEXCOORD0;
};

//***************************************************
// 出力頂点シェーダー
//***************************************************
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

//***************************************************
// サンプラーステートの設定
//***************************************************
sampler WallSampler = sampler_state
{
    Texture = <g_WallTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

//===================================================
// 頂点を計算する
//===================================================
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // カメラから見たワールド座標にする
    output.pos = mul(float4(input.pos, 1), g_mtxWVP);
    output.uv = input.uv;
    output.normal = input.normal;
    output.col = input.col;
    
    return output;
}

//===================================================
// ランダム関数
//===================================================
float2 random(float2 co)
{
    return frac(sin(dot(co, float2(12.9898f, 78.2323))) * 43758.5453);
}

//===================================================
// 画面に出力
//===================================================
float4 main(VS_OUTPUT input) : SV_TARGET
{   
    // 影を求める
    float fNdotL_1 = saturate(dot(normalize(input.normal), g_LightDir001));
    float fNdotL_2 = saturate(dot(normalize(input.normal), g_LightDir002));
    float fNdotL_3 = saturate(dot(normalize(input.normal), g_LightDir003));

    float3 fNDotL = fNdotL_1 * g_LightColor001 +
                   fNdotL_2 * g_LightColor002 +
                   fNdotL_3 * g_LightColor003;
    
    // 範囲制限
    fNDotL = saturate(fNDotL);
    
    // テクスチャの色を取得
    float4 texColor = tex2D(WallSampler, input.uv);
    
    if (texColor.a < 0.01f)
    {
        texColor = float4(0.2f, 0.2f, 0.2f, input.col.a);
    }
    else
    {
        texColor.rgb *= fNDotL;

        texColor.a *= input.col.a; // アルファ反映
    }
    

    return texColor;
}

//===================================================
// テクニック宣言
//===================================================
technique tech
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 main();
    }
}