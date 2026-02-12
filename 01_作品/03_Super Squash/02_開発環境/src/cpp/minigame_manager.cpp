//===================================================
//
// ミニゲームの補助 [minigame_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "minigame_manager.h"
#include "timer_component.hpp"
#include "score_area_component.hpp"
#include "utility_math.h"
#include "manager.h"
#include "score_component.hpp"
#include "fstream"
#include "scene_controller.h"
#include "minigame_result_state.h"
#include "camera_animation_system.h"
#include "ranking_component.hpp"
#include "minigame_player_component.hpp"
#include "light.h"
#include "color_constants.hpp"
#include "scene.h"
#include "ui_wall_controller.h" 

//===================================================
// コンストラクタ
//===================================================
CMiniGameManager::CMiniGameManager(entt::registry& registry) :
	m_registry(registry),
	m_nScore(NULL),
	m_nComb(NULL),
	m_state(State::Normal),
	m_bChangeScene(false)
{
}

//===================================================
// デストラクタ
//===================================================
CMiniGameManager::~CMiniGameManager()
{
	// スコア情報のセーブ
	SaveInfo();

	// ランキングのセーブ
	SaveRanking();
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CMiniGameManager> CMiniGameManager::Create(entt::registry& registry)
{
	// 自分自身の生成
	auto pInstance = std::make_unique<CMiniGameManager>(registry);

	return pInstance;
}

//===================================================
// タイマーの取得
//===================================================
int CMiniGameManager::GetTimer(void)
{
	// タイマーの取得(1つなのでfrontを取得)
	auto timerID = m_registry.view<TimerComponent>().front();

	// 取得できないなら
	if (!m_registry.valid(timerID))
	{
		int nError = TIMER_NO_FIND;
		return nError;
	}

	// タイマーのデータの取得
	auto& timerComp = m_registry.get<TimerComponent>(timerID);

	return timerComp.nTime;
}

//===================================================
// 更新処理
//===================================================
void CMiniGameManager::Update(void)
{
	// 現在のゲームの時間の取得
	int nTime = GetTimer();

	// ミニゲームのプレイヤーの取得
	auto minigame_player = m_registry.view<MinigamePlayerComponent>().front();

	// 使用できるなら
	if (m_registry.valid(minigame_player))
	{
		// ミニゲームのプレイヤーのコンポーネントの取得
		auto& minigamePlayerComp = m_registry.get<MinigamePlayerComponent>(minigame_player);

		// コンボ数の取得
		m_nComb = minigamePlayerComp.nCombo;
	}
	// タイマーが見つからないなら処理しない
	if (nTime == TIMER_NO_FIND) return;

	CManager *pManager = CManager::GetInstance();

	// タイマーが0になったら
	if (nTime <= 0 && !m_bChangeScene)
	{
		m_bChangeScene = true;

		// シーンの取得
		CScene* pScene = pManager->GetScene();

		CSceneController* pSceneController = pScene->GetController();

		pSceneController->ChangeScene(std::make_shared<CMinigameResultState>(), CCameraAnimationSystem::TYPE_GAME_TO_RESULT, 120);
	}

	// 残り60秒なら
	if (nTime == 60)
	{
		// 残り60秒
		UIWallController::SetTimeLimit60Second(m_registry);
	}

	if (nTime == 57)
	{
		// 残り60秒
		UIWallController::SetSecondFaze(m_registry);
	}
	if (nTime <= 10)
	{
		// ライトの取得
		CLight* pLight = pManager->GetLight();

		for (auto& lights : pLight->GetLight())
		{
			// 使用されていないならスキップ
			if (!lights.bUse) continue;

			lights.Light.Diffuse = Color::RED;
		}
	}
}

//===================================================
// 情報のセーブ
//===================================================
void CMiniGameManager::SaveInfo(void)
{
	// バイナリファイルを開く
	std::ofstream file("data/SYSTEM/score.bin", std::ios::binary);

	// ファイルが開けないなら
	if (!file.is_open())
	{
		return;
	}

	// 書き出す
	file.write(reinterpret_cast<const char*>(&m_nScore), sizeof(m_nScore));

	file.close();
	file.clear();
}

//===================================================
// ランキングのセーブ
//===================================================
void CMiniGameManager::SaveRanking(void)
{
	// バイナリファイルを開く
	std::fstream in_file("data/SYSTEM/ranking.bin", std::ios::binary | std::ios::in);

	// ファイルが開けないなら
	if (!in_file.is_open())
	{
		return;
	}

	// ランキングを格納する変数
	int nRanking[RankingComponent::MAX_RANK] = { -1 };

	// 値を読み込む
	in_file.read(reinterpret_cast<char*>(nRanking), sizeof(nRanking));

	// ファイルをクリア
	in_file.close();
	in_file.clear();

	// ランクインしていたら
	if (nRanking[RankingComponent::MAX_RANK - 1] <= m_nScore)
	{
		nRanking[RankingComponent::MAX_RANK - 1] = m_nScore;
	}
	else
	{
		// ランクインしてないなら処理をしない
		return;
	}

	// ソート処理
	std::sort(nRanking, nRanking + RankingComponent::MAX_RANK,std::greater<int>());

	// バイナリファイルを開く
	std::ofstream out_file("data/SYSTEM/ranking.bin", std::ios::binary);

	// ファイルが開けないなら
	if (!out_file.is_open())
	{
		return;
	}

	// 書き出す
	out_file.write(reinterpret_cast<const char*>(nRanking), sizeof(nRanking));

	// ファイルをクリア
	out_file.close();
	out_file.clear();
}