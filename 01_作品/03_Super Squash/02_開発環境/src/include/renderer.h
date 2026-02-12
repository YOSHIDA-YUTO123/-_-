//===================================================
//
// DirectXの描画処理 [renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "memory"

//***************************************************
// 前方宣言
//***************************************************
class CScene;		// シーン
class CDebugProc;	// デバッグ情報
class CFade;		// フェード

//***************************************************
// レンダラークラスの定義
//***************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(CScene* pScene);
	void Draw(const int fps, CScene* pScene, CFade* pFade);

	LPDIRECT3DDEVICE9 GetDevice(void);

	void onWireFrame();
	void offWireFrame();
private:

	std::unique_ptr<CDebugProc> m_pDebugProc;	// デバッグ情報のクラス
	LPDIRECT3D9 m_pD3D;							// Directxデバイスへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;				// Directxデバイスへのポインタ
};

#endif