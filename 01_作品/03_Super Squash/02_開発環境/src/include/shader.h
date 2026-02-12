//===================================================
//
// シェーダーのクラスの基底クラス [shader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SHADER_H_
#define _SHADER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "variant"

//***************************************************
// シェーダーのクラスの定義
//***************************************************
class CShader
{
public:

	// 型の設定
	using Parameter = std::variant<float, D3DXVECTOR4, D3DXMATRIX>;

	CShader();
	virtual ~CShader();

	virtual HRESULT Init(const char* pShaderFile, const char* pTech);
	virtual void Apply(const entt::registry& registry, const entt::entity entity) = 0;

	void Begin(void);
	void BeginPass(void);
	void End(void);
	void EndPass(void);

protected:
	void SetValue(const char* pKey, const void* pData, size_t size);
	void SetFloat(const char* pKey, const float fParam);
	void SetFloatArray(const D3DXHANDLE& hHandle,const FLOAT* pParam, const UINT count);
	void SetVector(const char* pKey, const D3DXVECTOR4& vector4);
	void SetMatrix(const char* pKey, const D3DXMATRIX& matrix);
	void SetTexture(const char* pKey, const int nTextureID);
	void SetTexture(const char* pKey, const LPDIRECT3DTEXTURE9& pTextureID);
	D3DXHANDLE GetHandle(const char* pKey) const;
	void CommitChange(void);
private:
	//LPDIRECT3DVERTEXDECLARATION9 m_pVertexDecl;		// 頂点バッファの構造をDirectxに伝える変数
	LPD3DXEFFECT m_pEffect;								// HLSLのエフェクト
	std::unordered_map<std::string, Parameter> m_Param;	// パラメータ
};
#endif