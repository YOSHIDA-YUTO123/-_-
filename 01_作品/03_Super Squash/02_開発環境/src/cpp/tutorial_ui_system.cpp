//===================================================
//
// チュートリアルのUIの更新処理 [tutorial_ui_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "tutorial_ui_system.h"
#include "tag_component.hpp"
#include "tutorial_system_component.hpp"
#include "ui_fadeout_component.hpp"
#include "character_motion_spec.h"
#include "factory_2d.h"
#include "spec_combinators.h"
#include "factory_mesh.h"
#include "ball_constants.hpp"
#include "character_collision_spec.h"
#include "character_model_component.hpp"
#include "parent_component.hpp"
#include "sphere_collider_component.hpp"
#include "sphere_collider_component.hpp"
#include "collision.h"
#include "ball_spec.h"
#include "tutorial_ui_component.hpp"
#include "velocity_component.hpp"
#include "transform_component.hpp"
#include "mesh_wall_collision_component.hpp"
#include "factory_mesh_quad.h"
#include "factory_system_entity.h"
#include "entity_name_component.hpp"
#include "renderer_mt_key_component.hpp"
#include "sound.h"
#include "manager.h"
#include "input.h"
#include "minigame_player_component.hpp"
#include "texture_mt_manager.h"
#include "mrt_target_component.hpp"
#include "entity_name_component.hpp"
#include "ui_wall_controller.h"

//***************************************************
// チュートリアルの定数
//***************************************************
namespace TutorialUI_Const
{
	constexpr int INTERVAL = 180;	// タスクを進ませない時間
}

//===================================================
// 更新処理
//===================================================
void TutorialUISystem::Update(entt::registry& registry)
{
	// チュートリアルのシステムの取得
	const auto tutorial_system = registry.view<TutorialSystemComponent>().front();

	// 使えなかったら
	if (!registry.valid(tutorial_system))
	{
		return;
	}

	// チュートリアルの取得
	auto& tutorialComp = registry.get<TutorialSystemComponent>(tutorial_system);

	// 最初のUIの使用時間
	tutorialComp.nFirstuiDisplayTime--;

	// 状態が無しなら
	if (tutorialComp.currentState == TutorialSystemComponent::State::None && tutorialComp.nFirstuiDisplayTime <= 0)
	{
		tutorialComp.currentState = TutorialSystemComponent::State::Move;
		UIWallController::SetTutorialTask(registry,"data/TEXTURE/tutorial_move.png");
		tutorialComp.nInterval = TutorialUI_Const::INTERVAL;
	}

	tutorialComp.nInterval--;

	// 成功判定を受け付けない時間
	if (tutorialComp.nInterval >= 0)
	{
		return;
	}

	// プレイヤーの取得
	auto playerID = registry.view<Tag::PlayerTag>().front();

	// 使えなかったら
	if (!registry.valid(playerID))
	{
		return;
	}

	// 状態の遷移
	switch (tutorialComp.currentState)
	{
	case TutorialSystemComponent::State::Move:
		SetMoveTask(registry, playerID, tutorialComp);
		break;
	case TutorialSystemComponent::State::jetPack:
		SetJetPackTask(registry, playerID, tutorialComp);
		break;
	case TutorialSystemComponent::State::QuickMove:
		SetQuickTask(registry, playerID, tutorialComp);
		break;
	case TutorialSystemComponent::State::Shot:
		SetShotTask(registry, playerID, tutorialComp);
		break;
	case TutorialSystemComponent::State::BallShot_Small:
		SetBallSpeedSmallTask(registry, tutorialComp);
		break;
	case TutorialSystemComponent::State::BallShot_Medium:
		SetBallSpeedMediumTask(registry, tutorialComp);
		break;
	case TutorialSystemComponent::State::MinigameRule:
		SetRuleUI(registry, tutorialComp);
		break;
	case TutorialSystemComponent::State::MinigameStart:
		SetGameStartTask(registry, tutorialComp);
		break;
	default:
		break;
	}

	// スキップ処理
	Skip(registry, tutorialComp);
}

//===================================================
// タスクの設定処理
//===================================================
void TutorialUISystem::SetTask(entt::registry& registry, const char* pTexturePath, TutorialSystemComponent& tutorialComp)
{
	if (!tutorialComp.bSetTask)
	{
		// チュートリアルのテクスチャの設定
		UIWallController::SetTutorialTask(registry, pTexturePath);
		tutorialComp.bSetTask = true;
	}
}

//===================================================
// ボールの設定処理
//===================================================
void TutorialUISystem::SetBall(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	if (!tutorialComp.bSetBall)
	{
		// ボールの生成
		auto ball = FactoryMesh::Create::Ball(registry, { 0.0f,150.0f,0.0f }, BallConstants::RADIUS * 2.0f, BallConstants::TEXTURE_PATH, BallConstants::SEGMENT);
		FactoryMesh::Build::Ball(registry, ball, 0.98f);
		tutorialComp.bSetBall = true;
	}
}

//===================================================
// 成功UIの生成
//===================================================
void TutorialUISystem::SetSuccess(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	if (pSound != nullptr)
	{
		pSound->Play(CSound::LABEL_SE_TASK_OK);
	}

	UIWallController::SetTutorialTask(registry, "data/TEXTURE/tutorial_success.png");
	tutorialComp.nInterval = 180;
	tutorialComp.bSetTask = false;
}

//===================================================
// 移動が成功したら
//===================================================
void TutorialUISystem::SetMoveTask(entt::registry& registry, const entt::entity playerID, TutorialSystemComponent& tutorialComp)
{
	CharacterSpec::Motion::CMove is_move; // 移動モーションどうか

	if (tutorialComp.currentState == TutorialSystemComponent::State::Move &&
		is_move.IsSatisfiedBy(registry, playerID))
	{
		// 成功の設定
		SetSuccess(registry, tutorialComp);

		tutorialComp.currentState = TutorialSystemComponent::State::jetPack;
	}
}

//===================================================
// ジェットパックが成功したら
//===================================================
void TutorialUISystem::SetJetPackTask(entt::registry& registry, const entt::entity playerID, TutorialSystemComponent& tutorialComp)
{
	CharacterSpec::Motion::CJetPack is_jetpack; // ジェットパックモーションどうか

	if (tutorialComp.currentState == TutorialSystemComponent::State::jetPack)
	{
		// タスクの設定
		SetTask(registry, "data/TEXTURE/tutorial_jetpack.png", tutorialComp);

		if (is_jetpack.IsSatisfiedBy(registry, playerID))
		{
			// 成功の設定
			SetSuccess(registry, tutorialComp);

			tutorialComp.currentState = TutorialSystemComponent::State::QuickMove;
		}
	}
}

//===================================================
// ショットが成功したら
//===================================================
void TutorialUISystem::SetShotTask(entt::registry& registry, const entt::entity playerID, TutorialSystemComponent& tutorialComp)
{
	CharacterSpec::Motion::CCheckShot is_shot; // ショットモーションどうか

	if (tutorialComp.currentState == TutorialSystemComponent::State::Shot)
	{
		// ボールの設定処理
		SetBall(registry, tutorialComp);

		// タスクの設定
		SetTask(registry, "data/TEXTURE/tutorial_shot.png", tutorialComp);

		if (is_shot.IsSatisfiedBy(registry, playerID))
		{
			// 成功の設定
			SetSuccess(registry, tutorialComp);

			tutorialComp.currentState = TutorialSystemComponent::State::BallShot_Small;
		}
	}
}

//===================================================
// クイック移動が成功したら
//===================================================
void TutorialUISystem::SetQuickTask(entt::registry& registry, const entt::entity playerID, TutorialSystemComponent& tutorialComp)
{
	CharacterSpec::Motion::CJetPackMoveLeft is_quickMove_left;		// 左クイック移動
	CharacterSpec::Motion::CJetPackMoveRight is_quickMove_right;	// 右クイック移動

	if (tutorialComp.currentState == TutorialSystemComponent::State::QuickMove)
	{
		// タスクの設定
		SetTask(registry, "data/TEXTURE/tutorial_quick.png", tutorialComp);

		if (COrSpec(is_quickMove_left, is_quickMove_right).IsSatisfiedBy(registry, playerID))
		{
			// 成功の設定
			SetSuccess(registry, tutorialComp);

			tutorialComp.currentState = TutorialSystemComponent::State::Shot;
		}
	}
}

//===================================================
// ボールの速さが小以上に出来たら
//===================================================
void TutorialUISystem::SetBallSpeedSmallTask(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	// ボールのID
	const auto ballID = registry.view<Tag::BallTag>().front();

	// 使えないIDだったら
	if (!registry.valid(ballID))
	{
		return;
	}

	BallSpec::CSpeedLevelAboveSmall is_aboveSmall; // 速度の判定

	if (tutorialComp.currentState == TutorialSystemComponent::State::BallShot_Small)
	{
		// タスクの設定
		SetTask(registry, "data/TEXTURE/tutorial_small.png", tutorialComp);

		if (is_aboveSmall.IsSatisfiedBy(registry, ballID))
		{
			// 成功の設定
			SetSuccess(registry, tutorialComp);

			tutorialComp.currentState = TutorialSystemComponent::State::BallShot_Medium;
		}
	}
}

//===================================================
// ボールの速さが大以上に出来たら
//===================================================
void TutorialUISystem::SetBallSpeedMediumTask(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	// ボールのID
	const auto ballID = registry.view<Tag::BallTag>().front();

	// 使えないIDだったら
	if (!registry.valid(ballID))
	{
		return;
	}

	BallSpec::CSpeedLevelAboveMedium is_aboveMedium; // 速度の判定

	if (tutorialComp.currentState == TutorialSystemComponent::State::BallShot_Medium)
	{
		// タスクの設定
		SetTask(registry, "data/TEXTURE/tutorial_medium.png", tutorialComp);

		if (is_aboveMedium.IsSatisfiedBy(registry, ballID))
		{
			// 成功の設定
			SetSuccess(registry, tutorialComp);

			auto& transformComp = registry.get<Transform3DComponent>(ballID);
			auto& velocityComp = registry.get<VelocityComponent>(ballID);

			transformComp.pos = Const::VEC3_NULL;
			velocityComp.move = Const::VEC3_NULL;

			tutorialComp.currentState = TutorialSystemComponent::State::MinigameRule;
		}
	}
}

//===================================================
// ルールのUIの表示
//===================================================
void TutorialUISystem::SetRuleUI(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	// 現在の状態が一致したら
	if (tutorialComp.currentState == TutorialSystemComponent::State::MinigameRule)
	{
		// ルール説明まで来たらスキップをしたのと同じ状態にする
		tutorialComp.bSkip = true;

		if (!tutorialComp.bSetTask)
		{
			UIWallController::SetTutorialGameRule(registry);
			tutorialComp.bSetTask = true;
		}

		tutorialComp.nRuleDisplayTime--;

		if (tutorialComp.nRuleDisplayTime <= 0)
		{
			tutorialComp.currentState = TutorialSystemComponent::State::MinigameStart;

			UIWallController::SetTutotialGameStart(registry);

			tutorialComp.nInterval = TutorialUI_Const::INTERVAL;
		}
	}
}

//===================================================
// ゲーム開始の設定処理
//===================================================
void TutorialUISystem::SetGameStart(entt::registry& registry, UIFadeOutComponent& fadeOutComp, TutorialSystemComponent& tutorialComp)
{
	// ボールのID
	const auto ballID = registry.view<Tag::BallTag>().front();

	// 使えないIDだったら
	if (!registry.valid(ballID))
	{
		return;
	}

	auto& transformComp = registry.get<Transform3DComponent>(ballID);
	auto& velocityComp = registry.get<VelocityComponent>(ballID);
	velocityComp.move = Const::VEC3_NULL;
	transformComp.pos = Const::VEC3_NULL;

	tutorialComp.currentState = TutorialSystemComponent::State::MinigameStart;
	fadeOutComp.bPlay = true;
}

//===================================================
// ゲームスタートが成功したときの処理
//===================================================
void TutorialUISystem::SetGameStartTask(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	// ボールのID
	const auto ballID = registry.view<Tag::BallTag>().front();

	// 使えないIDだったら
	if (!registry.valid(ballID))
	{
		return;
	}

	if (tutorialComp.currentState != TutorialSystemComponent::State::MinigameStart) return;

	// メッシュウォールの当たり判定
	auto& meshWallCollisionComp = registry.get<MeshWallCollisionComponent>(ballID);

	// 当たってないなら
	if (!meshWallCollisionComp.bCollision) return;

	// パラメータ
	Factory2D::Param param;

	param.pos = { 1090.0f,360.0f };
	param.size = { 300.0f, 200.0f };
	param.texturePath = "data/TEXTURE/number001.png";

	// タイマーの生成
	Factory2D::Create::Timer(registry, param, 120);

	param.pos = Const::CENTER_POS_2D;
	param.size = { SCREEN_WIDTH,SCREEN_HEIGHT };
	param.texturePath = "";
	param.nPriority = -1;

	// 背景の生成
	Factory2D::Create::SpriteMRT(registry, param, MRTType::TYPE_TIMER);

	// スコアのエリアの生成
	FactorySystemEntity::CreateScoreArea(registry, { 0.0f,120.0f,330.0f });

	tutorialComp.currentState = TutorialSystemComponent::State::Max;

	// ゲーム開始のUI
	UIWallController::SetGameStart(registry);

	// ミニゲームのプレイヤーの取得
	auto minigame_playerID = registry.view<MinigamePlayerComponent>().front();

	if (registry.valid(minigame_playerID))
	{
		auto& minigameComp = registry.get<MinigamePlayerComponent>(minigame_playerID);
		minigameComp.nCombo = 0;
	}

	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	if (pSound != nullptr)
	{
		pSound->StopSound(CSound::LABEL_BGM_TUTORIAL);
		pSound->Play(CSound::LABEL_BGM_MINIGAME);
	}
}

//===================================================
// スキップを押したら
//===================================================
void TutorialUISystem::Skip(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoyPad = pManager->GetInputJoypad();

	// 取得できないなら処理しない
	if (pKeyboard == nullptr || pJoyPad == nullptr) return;

	// スキップしたなら
	if (tutorialComp.bSkip)
	{
		return;
	}

	if (pKeyboard->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_BAKC))
	{
		tutorialComp.bSkip = true;

		// ボールの設定
		SetBall(registry, tutorialComp);

		// チュートリアルのタスクの設定
		UIWallController::SetTutorialTask(registry, "data/TEXTURE/tutorial_dec_skip.png");

		// ステートを変更、インターバルを設定
		tutorialComp.currentState = TutorialSystemComponent::State::MinigameRule;
		tutorialComp.nInterval = TutorialUI_Const::INTERVAL;
		tutorialComp.bSetTask = false;
	}
}