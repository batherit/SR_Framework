#include "pch.h"
#include "UI_InGame.h"
#include "UI_HPGauge.h"
#include "UI_Targeting.h"
#include "UI_Image.h"
#include "UI_GunGauge.h"
#include "UI_FadeInOut.h"
#include "Attribute.h"
#include "AliceW.h"

CUI_InGame::CUI_InGame(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_InGame::CUI_InGame(const CUI_InGame & rhs)
	:
	CGameObject(rhs)
{
}

CUI_InGame::~CUI_InGame(void)
{
}

HRESULT CUI_InGame::Ready_Object(void)
{
	m_pAimImage = CUI_Image::Create(m_pGraphicDev);
	m_pAimImage->SetTexture(L"UI_Aim");
	m_pAimImage->SetPos(WINCX >> 1, WINCY >> 1);
	m_pAimImage->SetActivated(false);
	AddChild(m_pAimImage);

	m_pGunGauge = CUI_GunGauge::Create(m_pGraphicDev);
	m_pGunGauge->SetActivated(false);
	AddChild(m_pGunGauge);

	m_pHPGauge = CUI_HPGauge::Create(m_pGraphicDev);
	m_pHPGauge->SetPlayer(m_pPlayer);
	AddChild(m_pHPGauge);

	m_pTargeting = CUI_Targeting::Create(m_pGraphicDev);
	m_pTargeting->GetTransform()->SetPos(0.f, 10.f, 0.f);
	m_pTargeting->SetPlayer(m_pPlayer);
	AddChild(m_pTargeting);

	m_pFadeInOut = CUI_FadeInOut::Create(m_pGraphicDev);
	AddChild(m_pFadeInOut);

	return S_OK;
}

_int CUI_InGame::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CUI_InGame::Render_Object(void)
{
}

CUI_InGame * CUI_InGame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_InGame*	pInstance = new CUI_InGame(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_InGame::Free(void)
{
	CGameObject::Free();
}

void CUI_InGame::SetPlayer(Engine::CGameObject * _pPlayer)
{
	if (!_pPlayer)
		return;

	m_pPlayer = _pPlayer;
	m_pHPGauge->SetPlayer(m_pPlayer);
	m_pTargeting->SetPlayer(m_pPlayer);
}