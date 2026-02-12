//===================================================
//
// タグのデータ [tag_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TAG_COMPONRNT_HPP_
#define _TAG_COMPONRNT_HPP_

//***************************************************
// タグの定義
//***************************************************
namespace Tag
{
	struct StencilPolygonTag {};
	struct SpriteTag {};
	struct SpriteSheetTag {};
	struct MeshQuadSheetTag {};
	struct BillboardTag {};
	struct MeshTag {};
	struct EffectMeshWallTag {};
	struct MeshQuadTag {};
	struct MeshFieldTag {};
	struct MeshDomeTag {};
	struct MeshSphereTag {};
	struct MeshWallTag {};
	struct CharacterTag {};
	struct PlayerTag {};
	struct TutorialPlayerTag {};
	struct EnemyTag {};
	struct BallTag {};
	struct EffectTag {};
	struct EffectPoolTag {};
	struct StencilTag {};
	struct ModelTag {};
	struct ParticleTag {};
	struct ParentSystemTag {};
	struct BallCollisionTag {};
	struct TargetTag {};
	struct HitStopTag {};
	struct TimerTag {};
	struct InkTag {};
	struct LocatorTag {};
	struct TitleMenuTag {};
	struct PauseMenuTag {};
	struct TutorialMRTTag {};
	struct ResultMRTTag {};
	struct TutorialTaskTag {};
	struct TutorialSuccessTag {};
	struct TitleMRTTag {};
	struct ResultScoreTag {};
}

//***************************************************
// コライダーのタグ
//***************************************************
namespace ColliderTag
{
	struct BallShotRange {};
}

//***************************************************
// 描画のタグの定義
//***************************************************
namespace RendererTag
{
	struct SpriteTag {};
	struct SpriteMRTTag {};
	struct ShaderSpriteTag {};
	struct MeshQuadTag {};
	struct ShaderMeshMTTag {};
	struct MeshMTTag {};
	struct CharacterModelTag{};
	struct OutLineSphereTag {};
	struct OutLineCharacterTag {};
	struct OutLineModelTag {};
	struct FieldEffectTag {};
	struct TutorialSpriteTag {};
	struct TutorialCharacterTag {};
	struct ShaderMeshWallTag {};
	struct TitleMRTTag {};
	struct MeshSphereMRT {};
	struct ModelMRT {};
}
#endif