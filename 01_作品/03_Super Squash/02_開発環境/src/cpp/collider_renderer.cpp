//===================================================
//
// デバッグ表示用コライダーの描画処理 [collider_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "collider_renderer.h"
#include "box_collider_component.hpp"
#include "capsule_collider_component.hpp"
#include "sphere_collider_component.hpp"
#include "transform_component.hpp"
#include "color_constants.hpp"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//===================================================
// 描画処理
//===================================================
void ColliderRenderer::Renderer(entt::registry& registry)
{
	// 表示するかどうか
	static bool bShow = true;

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pKeyboard->GetTrigger(DIK_F3))
	{
		bShow = bShow ? false : true;
	}

	if (!bShow)
	{
		return;
	}
	// 矩形コライダーの取得
	auto aabb_view = registry.view<BoxColliderComponent>();

	// 取得した分回す
	for (auto entity : aabb_view)
	{
		auto& aabbComp = registry.get<BoxColliderComponent>(entity);

		SetBoxVertex(registry, aabbComp);

	}

	// カプセルコライダーの取得
	auto capsule_view = registry.view<CapsuleColliderComponent>();

	// 取得した分回す
	for (auto entity : capsule_view)
	{
		auto& capsuleComp = registry.get<CapsuleColliderComponent>(entity);

		SetCapsuleVertex(registry, capsuleComp);
	}
}

//===================================================
// 描画処理
//===================================================
void ColliderRenderer::Draw(std::vector<LINE_LIST>& lineList)
{
	CManager* pManager = CManager::GetInstance();

	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ライティングOFF
	//pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			// ZバッファをOFF

	// SetTransformのマトリックスのリセット用変数
	D3DXMATRIX Identity;
	D3DXMatrixIdentity(&Identity);
	pDevice->SetTransform(D3DTS_WORLD, &Identity);

	// 頂点フォーマットの設定
	pDevice->SetFVF(D3DFVF_VERTEX);
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, lineList.size() / 2, lineList.data(), sizeof(LINE_LIST));

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ライティングON
	//pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);			// ZバッファをON
}

//===================================================
// ボックスの頂点座標の設定
//===================================================
void ColliderRenderer::SetBoxVertex(entt::registry& registry, BoxColliderComponent& aabb)
{
	std::vector<LINE_LIST> line_vertex;

	// 配列の確保
	line_vertex.resize(24);

	// 空間情報の取得
	auto& transform = registry.get<Transform3DComponent>(aabb.ownerID);

	D3DXVECTOR3 pos = transform.pos;
	D3DXVECTOR3 size = aabb.size * 0.5f;
	D3DXCOLOR col = Color::GREENYELLOW;

	line_vertex[0] = { pos + D3DXVECTOR3(-size.x,-size.y,-size.z),col };
	line_vertex[1] = { pos + D3DXVECTOR3( size.x,-size.y,-size.z),col };
	line_vertex[2] = { pos + D3DXVECTOR3( size.x,-size.y,-size.z),col };
	line_vertex[3] = { pos + D3DXVECTOR3( size.x, size.y,-size.z),col };
	line_vertex[4] = { pos + D3DXVECTOR3( size.x, size.y,-size.z),col };
	line_vertex[5] = { pos + D3DXVECTOR3(-size.x, size.y,-size.z),col };
	line_vertex[6] = { pos + D3DXVECTOR3(-size.x, size.y,-size.z),col };
	line_vertex[7] = { pos + D3DXVECTOR3(-size.x,-size.y,-size.z),col };

	line_vertex[8] = { pos + D3DXVECTOR3(-size.x,-size.y,size.z),col };
	line_vertex[9] = { pos + D3DXVECTOR3( size.x,-size.y,size.z),col };
	line_vertex[10] = { pos + D3DXVECTOR3( size.x,-size.y,size.z),col };
	line_vertex[11] = { pos + D3DXVECTOR3( size.x, size.y,size.z),col };
	line_vertex[12] = { pos + D3DXVECTOR3( size.x, size.y,size.z),col };
	line_vertex[13] = { pos + D3DXVECTOR3(-size.x, size.y,size.z),col };
	line_vertex[14] = { pos + D3DXVECTOR3(-size.x, size.y,size.z),col };
	line_vertex[15] = { pos + D3DXVECTOR3(-size.x,-size.y,size.z),col };

	line_vertex[16] = { pos + D3DXVECTOR3(-size.x,-size.y,-size.z),col };
	line_vertex[17] = { pos + D3DXVECTOR3(-size.x,-size.y,size.z),col };

	line_vertex[18] = { pos + D3DXVECTOR3(size.x,-size.y,-size.z),col };
	line_vertex[19] = { pos + D3DXVECTOR3(size.x, -size.y,size.z),col };

	line_vertex[20] = { pos + D3DXVECTOR3(size.x, size.y,-size.z),col };
	line_vertex[21] = { pos + D3DXVECTOR3(size.x, size.y,size.z),col };

	line_vertex[22] = { pos + D3DXVECTOR3(-size.x, size.y,-size.z),col };
	line_vertex[23] = { pos + D3DXVECTOR3(-size.x, size.y,size.z),col };

	// 描画処理
	Draw(line_vertex);
}

//===================================================
// カプセルの頂点座標の設定
//===================================================
void ColliderRenderer::SetCapsuleVertex(entt::registry& registry, CapsuleColliderComponent& capsuleComp)
{
	std::vector<LINE_LIST> line_vertex;

	D3DXCOLOR col = Color::CRIMSON;

	// 空間情報の取得
	auto& transformComp = registry.get<Transform3DComponent>(capsuleComp.ownerID);

	// 高さを求める
	float fHeight = capsuleComp.endPosLocal.y - capsuleComp.startPosLocal.y;

	// 分割数
	const int nSegment = 16;

	// 分割数分回す
	for (int nCntVtx = 0; nCntVtx < nSegment; nCntVtx++)
	{
		float fTheta = (Const::TWO_PI / nSegment) * nCntVtx;
		float fNextTheta = (Const::TWO_PI / nSegment) * (nCntVtx + 1);

		// 下の円
		D3DXVECTOR3 currentPos0 = D3DXVECTOR3(cosf(fTheta) * capsuleComp.fRadius, 0.0f, sinf(fTheta) * capsuleComp.fRadius);
		D3DXVECTOR3 nextPos0 = D3DXVECTOR3(cosf(fNextTheta) * capsuleComp.fRadius, 0.0f, sinf(fNextTheta) * capsuleComp.fRadius);

		D3DXVec3TransformCoord(&currentPos0, &currentPos0, &transformComp.mtxWorld);
		D3DXVec3TransformCoord(&nextPos0, &nextPos0, &transformComp.mtxWorld);

		// 要素の追加
		line_vertex.push_back({ currentPos0 ,col });
		line_vertex.push_back({ nextPos0 ,col });

		// 上の円
		D3DXVECTOR3 currentPos1 = D3DXVECTOR3(cosf(fTheta) * capsuleComp.fRadius, fHeight, sinf(fTheta) * capsuleComp.fRadius);
		D3DXVECTOR3 nextPos1 = D3DXVECTOR3(cosf(fNextTheta) * capsuleComp.fRadius, fHeight, sinf(fNextTheta) * capsuleComp.fRadius);

		D3DXVec3TransformCoord(&currentPos1, &currentPos1, &transformComp.mtxWorld);
		D3DXVec3TransformCoord(&nextPos1, &nextPos1, &transformComp.mtxWorld);

		// 要素の追加
		line_vertex.push_back({ currentPos1 ,col });
		line_vertex.push_back({ nextPos1 ,col });

		// 側面線
		line_vertex.push_back({ currentPos0, col });
		line_vertex.push_back({ nextPos1, col });
	}

	const int nSphereSeg = 4; // 半球の分割数

	// 上の半球の作成
	
	// 縦の分割数
	for (int nCntV = 0; nCntV < nSphereSeg; nCntV++)
	{
		float fThetaV = static_cast<float>(nCntV) / nSphereSeg;
		float fNextThetaV = static_cast<float>(nCntV + 1) / nSphereSeg;

		float fPosY = fHeight + sinf(fThetaV * D3DX_PI * 0.5f) * capsuleComp.fRadius;
		float fNextPosY = fHeight + sinf(fNextThetaV * D3DX_PI * 0.5f) * capsuleComp.fRadius;

		float fRadius = cosf(fThetaV * D3DX_PI * 0.5f) * capsuleComp.fRadius;
		float fNextRadius = cosf(fNextThetaV * D3DX_PI * 0.5f) * capsuleComp.fRadius;

		// 横の分割数分回す
		for (int nCntU = 0; nCntU < nSegment; nCntU++)
		{
			float fThetaU = (Const::TWO_PI / nSegment) * nCntU;
			float fNextThetaU = (Const::TWO_PI / nSegment) * (nCntU + 1);

			D3DXVECTOR3 p0(cosf(fThetaU) * fRadius, fPosY, sinf(fThetaU) * fRadius);
			D3DXVECTOR3 p1(cosf(fNextThetaU) * fRadius, fPosY, sinf(fNextThetaU) * fRadius);
			D3DXVECTOR3 p2(cosf(fThetaU) * fNextRadius, fNextPosY, sinf(fThetaU) * fNextRadius);

			D3DXVec3TransformCoord(&p0, &p0, &transformComp.mtxWorld);
			D3DXVec3TransformCoord(&p1, &p1, &transformComp.mtxWorld);
			D3DXVec3TransformCoord(&p2, &p2, &transformComp.mtxWorld);

			// 水平円弧ライン
			line_vertex.push_back({ p0, col });
			line_vertex.push_back({ p1, col });

			// 縦ライン
			line_vertex.push_back({ p0, col });
			line_vertex.push_back({ p2, col });
		}
	}

	// 下の半球の作成
	
	// 縦の分割数
	for (int nCntV = 0; nCntV < nSphereSeg; nCntV++)
	{
		float fThetaV = static_cast<float>(nCntV) / nSphereSeg;
		float fNextThetaV = static_cast<float>(nCntV + 1) / nSphereSeg;

		float fPosY = -sinf(fThetaV * D3DX_PI * 0.5f) * capsuleComp.fRadius;
		float fNextPosY = -sinf(fNextThetaV * D3DX_PI * 0.5f) * capsuleComp.fRadius;

		float fRadius = cosf(fThetaV * D3DX_PI * 0.5f) * capsuleComp.fRadius;
		float fNextRadius = cosf(fNextThetaV * D3DX_PI * 0.5f) * capsuleComp.fRadius;

		// 横の分割数分回す
		for (int nCntU = 0; nCntU < nSegment; nCntU++)
		{
			float fThetaU = (Const::TWO_PI / nSegment) * nCntU;
			float fNextThetaU = (Const::TWO_PI / nSegment) * (nCntU + 1);

			D3DXVECTOR3 p0(cosf(fThetaU) * fRadius, fPosY, sinf(fThetaU) * fRadius);
			D3DXVECTOR3 p1(cosf(fNextThetaU) * fRadius, fPosY, sinf(fNextThetaU) * fRadius);
			D3DXVECTOR3 p2(cosf(fThetaU) * fNextRadius, fNextPosY, sinf(fThetaU) * fNextRadius);

			D3DXVec3TransformCoord(&p0, &p0, &transformComp.mtxWorld);
			D3DXVec3TransformCoord(&p1, &p1, &transformComp.mtxWorld);
			D3DXVec3TransformCoord(&p2, &p2, &transformComp.mtxWorld);

			// 水平円弧ライン
			line_vertex.push_back({ p0, col });
			line_vertex.push_back({ p1, col });

			// 縦ライン
			line_vertex.push_back({ p0, col });
			line_vertex.push_back({ p2, col });
		}
	}

	// 描画処理
	Draw(line_vertex);
}
