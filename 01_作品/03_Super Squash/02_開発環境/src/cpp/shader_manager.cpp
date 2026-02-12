//===================================================
//
// シェーダーの初期化を行うs [shader_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shader_manager.h"
#include "manager.h"
#include "renderer.h"
#include "color_constants.hpp"
#include "shader_outline.h"
#include "mesh_wall_effect_shader.h"

//===================================================
// コンストラクタ
//===================================================
CShaderManager::CShaderManager()
{
}

//===================================================
// デストラクタ
//===================================================
CShaderManager::~CShaderManager()
{
    // シェーダー分回す
    for (auto& shader : m_apShader)
    {
        // メモリの破棄
        shader.reset();
    }
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CShaderManager> CShaderManager::Create(void)
{
    // インスタンスの生成
    auto pInstance = std::make_unique<CShaderManager>();

    // 初期化処理
    if (FAILED(pInstance->Init()))
    {
        pInstance.reset();
        return nullptr;
    }

    return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CShaderManager::Init(void)
{
    // アウトラインの生成
    m_apShader[TYPE_OUTLINE] = std::make_unique<CShaderOutLine>();

    // アウトラインの初期化
    if (FAILED(m_apShader[TYPE_OUTLINE]->Init("data/SHADER/OutLine.hlsl", "tech"))) return E_FAIL;

    return S_OK;
}
