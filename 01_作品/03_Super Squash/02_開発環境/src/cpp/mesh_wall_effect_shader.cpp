//===================================================
//
// メッシュの壁のシェーダーを適応する処理 [mesh_wall_effect_shader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_wall_effect_shader.h"
#include "texture_mt_manager.h"
#include "renderer.h"
#include "manager.h"
#include "renderer_mt_key_component.hpp"
#include "texture_manager.h"
#include "transform_component.hpp"
#include "light.h"

//===================================================
// コンストラクタ
//===================================================
CMeshWallEffectShader::CMeshWallEffectShader()
{
}

//===================================================
// デストラクタ
//===================================================
CMeshWallEffectShader::~CMeshWallEffectShader()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CMeshWallEffectShader::Init(const char* pShaderFile, const char* pTech)
{
	// 初期化処理
	if (FAILED(CShader::Init(pShaderFile, pTech)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// 適応処理
//===================================================
void CMeshWallEffectShader::Apply(const entt::registry& registry, const entt::entity entity)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャMTマネージャの取得
	CTextureMTManager* pTextureMTManager = pManager->GetTextureMTManager();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "MeshRenderer_GetRenderer");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	auto pRendererMTKeyComp = registry.try_get<RendererMTKeyComponent>(entity);

	D3DXMATRIX mtxProj, mtxView;

	// ビューマトリックス、プロジェクションマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// 空間情報の取得
	auto& transformComp = registry.get<Transform3DComponent>(entity);

	// ワールドマトリックス
	D3DXMATRIX WorldViewProjMatrix;
	WorldViewProjMatrix = transformComp.mtxWorld * mtxView * mtxProj;

	// ライトの取得
	CLight* pLight = pManager->GetLight();

	// ライトの情報の取得
	auto alightInfo = pLight->GetLight();

	D3DXVECTOR3 light_dir1 = alightInfo[0].Light.Direction;
	D3DXVECTOR3 light_dir2 = alightInfo[1].Light.Direction;
	D3DXVECTOR3 light_dir3 = alightInfo[2].Light.Direction;

	CShader::SetValue("g_LightDir001", &light_dir1, sizeof(D3DXVECTOR3));
	CShader::SetValue("g_LightDir002", &light_dir2, sizeof(D3DXVECTOR3));
	CShader::SetValue("g_LightDir003", &light_dir3, sizeof(D3DXVECTOR3));

	CShader::SetVector("g_LightColor001", D3DXVECTOR4(alightInfo[0].Light.Diffuse.r, alightInfo[0].Light.Diffuse.g, alightInfo[0].Light.Diffuse.b,1.0f));
	CShader::SetVector("g_LightColor002", D3DXVECTOR4(alightInfo[1].Light.Diffuse.r, alightInfo[0].Light.Diffuse.g, alightInfo[0].Light.Diffuse.b,1.0f));
	CShader::SetVector("g_LightColor003", D3DXVECTOR4(alightInfo[2].Light.Diffuse.r, alightInfo[0].Light.Diffuse.g, alightInfo[0].Light.Diffuse.b, 1.0f));

	CShader::SetMatrix("g_mtxWVP", WorldViewProjMatrix);

	// テクスチャの設定
	CShader::SetTexture("g_WallTexture", pTextureMTManager->GetAdress(pRendererMTKeyComp->nType));

	//CShader::CommitChange();
}