//===================================================
//
// テクスチャアニメーションの処理 [texture_animation_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// エンティティのプールクラスの定義
//***************************************************
#include "texture_animation_system.h"
#include "texture_animation_component.hpp"
#include "sheet_component.hpp"

//===================================================
// 更新処理
//===================================================
void TextureAnimationSystem::Update(entt::registry& registry)
{
	// テクスチャアニメーションの取得
	auto view = registry.view<TextureAnimationComponent>();

	// テクスチャアニメーションを持つエンティティ分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& textureAnimComp = registry.get<TextureAnimationComponent>(entity);
		auto& SheetComp = registry.get<SheetComponent>(entity);

		// カウンターを超えたら
		if (textureAnimComp.nCounter >= textureAnimComp.nSpeed)
		{
			// パターンを増やす
			textureAnimComp.nPattern++;

			// アニメーションのカウンターを進める
			textureAnimComp.nCounter = 0;

			float fPosTexU = 1.0f / SheetComp.nSegmentU;
			float fPosTexH = 1.0f / SheetComp.nSegmentV;

			// Y座標の割合
			int nRatePosY = textureAnimComp.nPattern / SheetComp.nSegmentU;

			// テクスチャ座標の計算
			float fNewTexPosU = fPosTexU * textureAnimComp.nPattern;
			float fNewTexPosV = nRatePosY * fPosTexH;

			// テクスチャ座標の設定
			SheetComp.aPosUV[0] = D3DXVECTOR2(fNewTexPosU, fNewTexPosV);
			SheetComp.aPosUV[1] = D3DXVECTOR2(fNewTexPosU + fPosTexU, fNewTexPosV);
			SheetComp.aPosUV[2] = D3DXVECTOR2(fNewTexPosU, fNewTexPosV + fPosTexH);
			SheetComp.aPosUV[3] = D3DXVECTOR2(fNewTexPosU + fPosTexU, fNewTexPosV + fPosTexH);

			// パターンが最大まで行ったか判定
			const bool bPatternMax = textureAnimComp.nPattern >= (SheetComp.nSegmentU * SheetComp.nSegmentV);

			// 最大まで行ったら
			if (bPatternMax)
			{
				continue;
			}

			// パターンを初期化
			textureAnimComp.nPattern = 0;

			// ループしないなら
			if (!textureAnimComp.bLoop)
			{
				// 破棄
				registry.destroy(entity);

				return;
			}

			textureAnimComp.nCounter++;
		}
	}
}