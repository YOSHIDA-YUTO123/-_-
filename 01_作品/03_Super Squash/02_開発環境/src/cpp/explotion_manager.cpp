//===================================================
//
// 煙、爆発のマネージャーの定義 [explotion_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "explotion_manager.h"
#include "factory_billboard.h"
#include "color_constants.hpp"
#include "effect_constants.hpp"
#include "effect_component.hpp"

//===================================================
// ボール着地時の煙
//===================================================
void ExplotionManager::BallLandingExplotion(entt::registry& registry, const D3DXVECTOR3& pos, const int nNum, const D3DXVECTOR3& move)
{
	// 総数分回す
	for (int nCnt = 0; nCnt <= nNum; nCnt++)
	{
		// 一周の割合を求める
		float fAngle = Const::TWO_PI / nNum * nCnt;

		D3DXVECTOR3 moveWk = Const::VEC3_NULL;

		// 移動量の設定
		moveWk.x = sinf(fAngle) * move.x;
		moveWk.y = move.y;
		moveWk.z = cosf(fAngle) * move.z;

		// エフェクトの生成
		auto effectID = FactoryBillboard::Create::Effect(registry, pos, Color::WHITE, 30.0f, EffectConstants::TEXTURE_SMOKE);
		FactoryBillboard::Build::Effect(registry, effectID, 30, moveWk, EffectComponent::FLAG_DEC_ALPHA | EffectComponent::FLAG_DEC_RADIUS);
		FactoryBillboard::Build::AnimationEffect(registry, effectID, 3, false, 4, 3);
	}
}