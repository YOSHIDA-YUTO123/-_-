//===================================================
// 
// ボールが壁に当たった時にインクをつける [ball_ink.hlsl]
// Author : YUTO YOSHIDA
//
//===================================================

//***************************************************
// グローバル変数宣言
//***************************************************
float2 g_HitPosUV;  // 衝突点のUV座標
float4 g_HitColor;  // 衝突時の色

//===================================================
// インクのuvの設定
//===================================================
float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    // UV中心を飛び散りの中心に固定
    float2 uvCenter = g_HitPosUV;
    float2 diff = uv - uvCenter;
    diff.x *= 5.0f;
    
    float fDistance = length(diff);
    
    // ノイズを作成
    float fNoise_1 = frac(sin(dot(uv * 30.0f, float2(12.9898f, 78.2323f))) * 43758.5453f);
    float fNoise_2 = frac(sin(dot(uv * 80.0f, float2(93.9898f, 67.2323f))) * 43758.5453f);
    
    // ノイズで距離を変化させていびつにする
    fDistance *= (0.7f + 0.3f * fNoise_1 + 0.2f * fNoise_2);
    
    float fAlpha = saturate(1.0 - pow(fDistance, 1.5) * 25.0);
    
    fAlpha *= 0.8 + 0.2 * frac(sin(dot(uv * 100.0, float2(34.12, 78.99))) * 43758.5453);
           
    // 色を設定
    return float4(g_HitColor.r, g_HitColor.g, g_HitColor.b, fAlpha);
}

//===================================================
// テクニック宣言
//===================================================
technique tech
{
    pass P0
    {
        PixelShader = compile ps_3_0 main();
    }
}