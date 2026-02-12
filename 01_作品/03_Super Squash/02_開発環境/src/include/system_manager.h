//===================================================
//
// systemのマネージャーの処理 [system_manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SYSTEM_MANAGER_H_
#define _SYSTEM_MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"
#include "renderer_system_base.h"
#include "mrt_type.hpp"
#include "renderer_mrt_system_base.h"

//***************************************************
// 前方宣言
//***************************************************
struct SystemBase;			// 処理の基底
struct RendererSystemBase;	// 描画処理の基底

//***************************************************
// systemのマネージャークラスの定義
//***************************************************
class CSystemManager
{
public:

	CSystemManager();
	~CSystemManager();

	/// <summary>
	/// 描画systemの追加
	/// </summary>
	/// <param name="描画systemのインスタンス"></param>
	void AddRendererSystem(std::unique_ptr<RendererSystemBase> pSystem) { m_apRendererSystems.push_back(std::move(pSystem)); }

	/// <summary>
	/// 更新処理の追加
	/// </summary>
	/// <param name="システムのインスタンス"></param>
	void AddSystem(std::unique_ptr<SystemBase> pSystem) { m_apSystems.push_back(std::move(pSystem)); }

	/// <summary>
	/// ポーズの影響の受けない更新処理の追加
	/// </summary>
	/// <param name="ポーズの影響の受けない更新処理のインスタンス"></param>
	void AddUnPauseSystem(std::unique_ptr<SystemBase> pSystem) { m_apUnPauseSystem.push_back(std::move(pSystem)); }

	/// <summary>
	/// MRTの描画処理
	/// </summary>
	/// <param name="インスタンス"></param>
	/// <param name="種類"></param>
	void AddRendererMRTSystem(std::unique_ptr<RendererMRTSystemBase> pSystem, const MRTType::TYPE type) { m_apRendererMRTSystem[type].push_back(std::move(pSystem)); }

	void ReleaseAll(void);
	void UpdateAll(entt::registry& registry);
	void UnPauseUpdateAll(entt::registry& registry);
	void DrawAll(entt::registry& registry);
private:
	std::vector<std::unique_ptr<RendererSystemBase>> m_apRendererSystems;										// 描画systemのリスト
	std::vector<std::unique_ptr<SystemBase>> m_apSystems;														// systemのリスト
	std::vector<std::unique_ptr<SystemBase>> m_apUnPauseSystem;													// ポーズの影響を受けない更新処理
	std::array<std::vector<std::unique_ptr<RendererMRTSystemBase>>,MRTType::TYPE_MAX> m_apRendererMRTSystem;	// ポーズの影響を受けない更新処理
};
#endif
