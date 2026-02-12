//===================================================
//
// メッシュの壁のシェーダーを適応する処理 [mesh_wall_effect_shader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_WALL_EFFECT_SHADER_H_
#define _MESH_WALL_EFFECT_SHADER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "shader.h"

//***************************************************
// メッシュの壁のシェーダーを適応するクラスの定義
//***************************************************
class CMeshWallEffectShader : public CShader
{
public:
	CMeshWallEffectShader();
	~CMeshWallEffectShader();

	HRESULT Init(const char* pShaderFile, const char* pTech) override;
	void Apply(const entt::registry& registry, const entt::entity entity) override;
private:
};
#endif