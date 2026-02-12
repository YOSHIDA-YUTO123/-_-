//===================================================
//
// タイトルメニューのアニメーションの更新処理 [titlemenu_animation_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "titlemenu_animation_system.h"
#include "titlemenu_animation_component.hpp"
#include "transform_component.hpp"

//===================================================
// 更新処理
//===================================================
void TitleMenuAnimationSystem::Update(entt::registry& registry)
{
	// コンポーネントの取得
	auto view = registry.view<TitleMenuAnimationComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& AnimComp = registry.get<TitleMenuAnimationComponent>(entity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);

		// 再生状態じゃないなら処理を飛ばす
		if (!AnimComp.bPlay)
		{
			// 0に徐々に近づける
			D3DXQuaternionSlerp(&transformComp.quaternion, &transformComp.quaternion, &Const::QUATERNION_IDENTITY, 0.1f);

			continue;
		}

		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &Const::VEC_U, 0.01f);

		// 回転させる
		transformComp.quaternion *= quat;
	}
}