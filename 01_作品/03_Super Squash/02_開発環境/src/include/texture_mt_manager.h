//===================================================
//
// マルチレンダーターゲットのテクスチャの管理 [texture_mt_manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TEXTURE_MT_MANAGER_H_
#define _TEXTURE_MT_MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "mrt_type.hpp"

//***************************************************
// マルチレンダーターゲットのテクスチャのマネージャーの定義
//***************************************************
class CTextureMTManager
{
public:

	struct MRTDec
	{
		MRTType::TYPE type;	// レンダーターゲットの種類
		D3DXVECTOR3 posV;	// 視点
		D3DXVECTOR3 posR;	// 注視点
		int nWidth;			// 横幅
		int nHeight;		// 縦幅
		bool bClear;		// クリアするかどうか
	};

	// 初期設定
	const MRTDec CreateTable[MRTType::TYPE_MAX] =
	{
		{ MRTType::TYPE_TUTORIAL,		D3DXVECTOR3(-83.0f, 210.0f, -220.0f),	D3DXVECTOR3(-79.0f, 210.0f, 10.0f)	,SCREEN_WIDTH,SCREEN_HEIGHT,true },
		{ MRTType::TYPE_TITLE,			D3DXVECTOR3(0.0f, 50.0f, -300.0f),		D3DXVECTOR3(0.0f, 50.0f, 0.0f)		,SCREEN_WIDTH,SCREEN_HEIGHT,true },
		{ MRTType::TYPE_TIMER,			D3DXVECTOR3(-83.0f, 210.0f, -220.0f),	D3DXVECTOR3(-79.0f, 210.0f, 10.0f)	,SCREEN_WIDTH,SCREEN_HEIGHT,true },
		{ MRTType::TYPE_RESULT,			D3DXVECTOR3(-83.0f, 210.0f, -220.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f)		,SCREEN_WIDTH,SCREEN_HEIGHT,true },
		{ MRTType::TYPE_RANKING,		D3DXVECTOR3(0.0f,0.0f,0.0f)			,	D3DXVECTOR3(0.0f,0.0f,0.0f)			,SCREEN_WIDTH,SCREEN_HEIGHT,true },
	};

	// テクスチャの情報
	struct Info
	{
		LPDIRECT3DTEXTURE9 pTextureMT;	// レンダリングターゲット用テクスチャ
		LPDIRECT3DSURFACE9 pRenderMT;	// レンダリングターゲット用インターフェース
		LPDIRECT3DSURFACE9 pZBuffMT;	// レンダリングターゲット用Zバッファ
		D3DVIEWPORT9 viewport;			// テクスチャレンダリング用ビューポート
		int nType;						// 自分の種類の番号
	};

	CTextureMTManager();
	~CTextureMTManager();

	LPDIRECT3DTEXTURE9 GetAdress(const int nType);

	HRESULT Init(void);
	void Release(void);
	void Release(const int nIdx);

	void SetDefaultTarget(void);
	void ChangeRenderTarget(const int nIdx);
	void ResetRenderTarget(void);
private:
	std::array<Info, MRTType::TYPE_MAX> m_aInfo;	// マルチレンダーターゲットの情報
	Info m_defInfo;						// デフォルトの描画情報の保存用変数
	D3DXMATRIX m_mtxViewDef;			// ビューマトリックスの保存用変数
	D3DXMATRIX m_mtxProjDef;			// プロジェクションマトリックスの保存用変数
};
#endif