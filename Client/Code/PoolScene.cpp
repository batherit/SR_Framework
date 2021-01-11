#include "pch.h"
#include "PoolScene.h"
#include "Terrain.h"
#include "AliceL.h"
#include "Monster.h"
#include "MadCapA.h"
#include "Stone.h"
#include "Sword.h"
#include "SphereRenderer.h"
#include "SkyBox.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "CameraController_Player.h"
#include "CameraController_Crowd.h"
#include "CameraController_Sliding.h"
#include "CameraController_Target.h"
#include "CameraController_Gun.h"
#include "UI_InGame.h"
#include "Map.h"
#include "HPFlower.h"
#include "JumpPad.h"
#include "Tooth.h"
#include "Snail.h"
#include "Valve.h"
#include "PopDomino.h"
#include "UI_FadeInOut.h"

#include "Attribute.h"

CPoolScene::CPoolScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CPoolScene::CPoolScene(const CPoolScene & rhs)
	:
	CScene(rhs)
{
}

CPoolScene::~CPoolScene(void)
{
}

//LRESULT CPoolScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	
//	return LRESULT();
//}

void CPoolScene::ResetScene(void)
{

}

HRESULT CPoolScene::Ready(void)
{


	return S_OK;
}

int CPoolScene::Update(const _float& fTimeDelta)
{
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_L)) {
		if (Engine::CCollisionMgr::GetInstance()->IsColliderVisible()) {
			Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);
		}
		else {
			Engine::CCollisionMgr::GetInstance()->SetColliderVisible(true);
		}
	}

	return CScene::Update(fTimeDelta);
}

//void CPoolScene::Render(void)
//{
//	Engine::Get_Renderer()->Render_GameObject();
//
//	//Engine::Render_Font(L"Font_Jinji", L"Text Test", &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
//	//CScene::Render();
//}

void CPoolScene::OnLoaded()
{
	Engine::CCollisionMgr::GetInstance()->ClearGameObjectList();
	Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);

	FAILED_CHECK_RETURN(Ready_Resource(Engine::RESOURCE_END), );
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), );
	FAILED_CHECK_RETURN(Ready_LightInfo(), );

	m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CPoolScene::OnExited()
{
}

CPoolScene * CPoolScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPoolScene*	pInstance = new CPoolScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

HRESULT CPoolScene::Ready_LightInfo()
{
	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPoolScene::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.emplace(pLayerTag, pLayer);

	// 맵 생성
	CMap* pMap = CMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pMap, E_FAIL);
	pMap->LoadMap(
		pLayer,
		L"../../Resource/Terrain/Terrain_Poor.trr",
		L"../../Resource/Navi/Navi_Pool.navi",
		L"../../Resource/Map/Map_Pool.map"
	);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pMap), E_FAIL);

	// 카메라 매니져 생성
	Engine::CCameraMgr* pCameraMgr = Engine::CCameraMgr::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraMgr, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraMgr", pCameraMgr), E_FAIL);

	// 카메라 생성
	m_pCamera = CStaticCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	//m_pCamera->SetParent(m_pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", m_pCamera), E_FAIL);
	pCameraMgr->SetCamera(m_pCamera);

	// 플레이어 생성
	m_pPlayer = CAliceL::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pPlayer), E_FAIL);
	m_pPlayer->GetTransform()->SetPos(pMap->GetCurSpawnPoint());

	// 플레이어 카메라 컨트롤러 생성(0)
	Engine::CCameraController* pCameraController = CCameraController_Player::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	static_cast<CCameraController_Player*>(pCameraController)->SetPlayer(m_pPlayer);
	pCameraMgr->AddCameraController(pCameraController);
	pCameraMgr->ChangeCameraController(0, 0.5f);

	// 커서 없애기
	ShowCursor(false);
	Engine::CDirectInputMgr::GetInstance()->SetMouseFixed(true);

	//CUI_InGame* pUIInGame = CUI_InGame::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pUIInGame, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_InGame", pUIInGame), E_FAIL);
	//pUIInGame->SetPlayer(m_pPlayer);
	//pUIInGame->GetFadeInOut()->StartFadeInOut(2.f, true);
	//m_pPlayer->SetInGameUI(pUIInGame);

	// 스카이 박스 생성
	m_pSkyBox = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pSkyBox, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", m_pSkyBox), E_FAIL);

	return S_OK;
}

HRESULT CPoolScene::Ready_Resource(Engine::RESOURCETYPE eType)
{
	return S_OK;
}

void CPoolScene::Free(void)
{
	//Engine::CCollisionMgr::GetInstance()->ClearGameObjectList();
	CScene::Free();
}
