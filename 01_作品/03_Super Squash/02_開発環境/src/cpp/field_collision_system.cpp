//===================================================
//
// フィールドの当たり判定 [field_collision_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "field_collision_system.h"
#include "field_collision_component.hpp"
#include "tag_component.hpp"
#include "mesh_vtx_component.hpp"
#include "size_component.hpp"
#include "transform_component.hpp"
#include "vertex_buffer_component.hpp"

//===================================================
// 更新処理
//===================================================
void FieldCollisionSystem::Update(entt::registry& registry)
{
	// 地面の当たり判定のコンポーネントを持つエンティティの取得
	auto CollisionEntity = registry.view<FieldCollisionComponent>();

	// 地面のコンポーネントを持つエンティティの取得(front())
	auto FieldEntity = registry.view<Tag::MeshFieldTag>().front();

	// 当たり判定
	for (auto entity : CollisionEntity)
	{
		// コンポーネントの取得
		auto& VtxComp = registry.get<MeshVtxComponent>(FieldEntity);
		auto& sizeComp = registry.get<Size3DComponent>(FieldEntity);
		auto& FieldTransformComp = registry.get<Transform3DComponent>(FieldEntity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& vtxBufferComp = registry.get<VertexBufferComponent>(FieldEntity);
		auto& collision = registry.get<FieldCollisionComponent>(entity);

		collision.bLanding = false;

		// 頂点の分割数の設定
		int nSegH = VtxComp.nSegmentU;
		int nSegV = VtxComp.nSegmentV;

		// 1マスのサイズ
		float GridSizeX = sizeComp.size.x / (float)nSegH;
		float GridSizeZ = sizeComp.size.z / (float)nSegV;

		float X = transformComp.pos.x + (sizeComp.size.x * 0.5f);
		float Z = (sizeComp.size.z * 0.5f) - transformComp.pos.z;

		// 何番目のポリゴンか
		int polyX = (int)(X / GridSizeX);
		int polyZ = (int)(Z / GridSizeZ);

		// 現在のポリゴンのインデックス番号
		int polyIndex = ((polyZ * (nSegH - 1) + polyX) * 2) + (polyZ * 6);

		// ポリゴン数の設定
		int nNumPolygon = ((nSegH * nSegV) * 2) + (4 * (nSegV - 1));

		// インデックス数の設定
		int nNumIndex = nNumPolygon + 2;

		VERTEX_3D* pVtx = nullptr;

		// 頂点バッファをロック
		vtxBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// インデックスバッファへのポインタ
		WORD* pIdx;

		// インデックスバッファのロック
		vtxBufferComp.pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

		const int NUM_POLYGON = 2;

		for (int nCnt = 0; nCnt < NUM_POLYGON; nCnt++)
		{
			// 頂点のインデックス
			int nCntVertex = (polyIndex + nCnt);

			// マイナスだったら
			if (nCntVertex < 0 || nCntVertex > nNumIndex - 2) continue;

			// インデックスを取得
			int nIdx0 = pIdx[nCntVertex];
			int nIdx1 = pIdx[nCntVertex + 1];
			int nIdx2 = pIdx[nCntVertex + 2];

			//// オーバーしていたら
			//if (nIdx0 >= nCntVertex || nIdx1 >= nCntVertex || nIdx2 >= nCntVertex) continue;

			// 頂点を取得
			D3DXVECTOR3 vtx0 = pVtx[nIdx0].pos;
			D3DXVECTOR3 vtx1 = pVtx[nIdx1].pos;
			D3DXVECTOR3 vtx2 = pVtx[nIdx2].pos;

			D3DXVECTOR3 edge0 = vtx1 - vtx0; // 辺ベクトル0
			D3DXVECTOR3 edge1 = vtx2 - vtx1; // 辺ベクトル1
			D3DXVECTOR3 edge2 = vtx0 - vtx2; // 辺ベクトル2

			D3DXVECTOR3 Normal = {};

			if (nCnt % 2 == 0)
			{
				// 偶数番目の三角形
				D3DXVec3Cross(&Normal, &edge0, &edge1);
			}
			else
			{
				// 奇数番目の三角形（順序が逆になっている）
				D3DXVec3Cross(&Normal, &edge1, &edge0);
			}

			D3DXVec3Normalize(&Normal, &Normal);

			D3DXVECTOR3 PlayerVec0 = transformComp.pos - vtx0;
			D3DXVECTOR3 PlayerVec1 = transformComp.pos - vtx1;
			D3DXVECTOR3 PlayerVec2 = transformComp.pos - vtx2;

			D3DXVECTOR3 Cross0 = {};
			D3DXVECTOR3 Cross1 = {};
			D3DXVECTOR3 Cross2 = {};

			if (nCnt % 2 == 0)
			{
				// 三角形の頂点から外積
				D3DXVec3Cross(&Cross0, &edge0, &PlayerVec0);
				D3DXVec3Normalize(&Cross0, &Cross0);

				D3DXVec3Cross(&Cross1, &edge1, &PlayerVec1);
				D3DXVec3Normalize(&Cross1, &Cross1);

				D3DXVec3Cross(&Cross2, &edge2, &PlayerVec2);
				D3DXVec3Normalize(&Cross2, &Cross2);
			}
			else
			{
				// 三角形の頂点から外積
				D3DXVec3Cross(&Cross0, &PlayerVec0, &edge0);
				D3DXVec3Normalize(&Cross0, &Cross0);

				D3DXVec3Cross(&Cross1, &PlayerVec1, &edge1);
				D3DXVec3Normalize(&Cross1, &Cross1);

				D3DXVec3Cross(&Cross2, &PlayerVec2, &edge2);
				D3DXVec3Normalize(&Cross2, &Cross2);
			}

			if (Cross0.y >= 0.0f && Cross1.y >= 0.0f && Cross2.y >= 0.0f)
			{
				//float xz = ((pPos->x - vtx0.x) * Normal.x + (pPos->z - pPos->z) * Normal.z);

				//float fHeight = vtx0.y - (xz / Normal.y);

				// 平面の方程式のDを計算
				float D = -(Normal.x * vtx0.x + Normal.y * vtx0.y + Normal.z * vtx0.z);

				// プレイヤーの位置に基づいて、プレイヤーのY位置を計算
				float PosY = (Normal.x * transformComp.pos.x + Normal.z * transformComp.pos.z + D) / -Normal.y;

				D3DXVECTOR3 vec = vtx0 - transformComp.pos;
				D3DXVec3Normalize(&vec, &vec);

				// プレイヤーがポリゴンの裏側かどうかの判定
				float dot = D3DXVec3Dot(&Normal, &vec); // 法線とプレイヤー方向との内積
				
				if (dot >= 0.0f)
				{
					collision.normal = Normal;

					// 位置の設定
					transformComp.pos.y = FieldTransformComp.pos.y + PosY;

					collision.bLanding = true;

					collision.meshID = FieldEntity;

					break;
				}
			}
		}

		//インデックスバッファのアンロック
		vtxBufferComp.pIdxBuffer->Unlock();

		// 頂点バッファをアンロック
		vtxBufferComp.pVtxBuffer->Unlock();
	}
}
