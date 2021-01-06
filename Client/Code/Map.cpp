#include "pch.h"
#include "Map.h"
#include "Terrain.h"
#include "StaticObject.h"
#include "DynamicObject.h"

// CustomObject
// HPFlower, JumpPad, Snail, Tooth, PopDomino, Valve + DeathBox, CheckPointBox, EventBox
#include "HPFlower.h"
#include "JumpPad.h"
#include "Snail.h"
#include "Tooth.h"
#include "PopDomino.h"
#include "Valve.h"


CMap::CMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CMap::CMap(const CMap & rhs)
	:
	CGameObject(rhs)
{
}

CMap::~CMap(void)
{
}


HRESULT CMap::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();
	m_pNaviMesh = Engine::CNaviMesh::Create(m_pGraphicDev);
	m_mapComponent[Engine::ID_STATIC].emplace(L"COM_NAVIMESH", m_pNaviMesh);

	return S_OK;
}

int CMap::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return 0;
}

void CMap::Render_Object(void)
{
	m_pNaviMesh->Render_NaviMeshes();
}

CMap * CMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMap*	pInstance = new CMap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CMap::Free(void)
{
	CGameObject::Free();
}

void CMap::LoadMap(Engine::CLayer* pLayer, const _tchar * _pTerrainFilePath, const _tchar * _pNaviFilePath, const _tchar * _pObjectsFilePath)
{
	LoadTerrain(_pTerrainFilePath);
	LoadNaviMesh(_pNaviFilePath);
	LoadObjects(pLayer, _pObjectsFilePath);
}

void CMap::LoadTerrain( const _tchar * _pFilePath)
{
	if (!_pFilePath)
		return;

	// TODO : 터레인 파일을 읽기 위한 코드를 작성합니다.
	HANDLE hFile = CreateFile(_pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	ReleaseChild(m_pTerrain);
	Engine::Safe_Release(m_pTerrain);
	m_pTerrain = CTerrain::Create(m_pGraphicDev);
	AddChild(m_pTerrain);

	m_pTerrain->LoadInfo(hFile);

	CloseHandle(hFile);

	
}

void CMap::LoadNaviMesh(const _tchar * _pFilePath)
{
	if (!_pFilePath)
		return;

	// TODO : 네비메쉬 파일을 읽기 위한 코드를 작성합니다.
	// TODO : 터레인 파일을 읽기 위한 코드를 작성합니다.
	HANDLE hFile = CreateFile(_pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	//if (m_pNaviMesh) {
		Engine::Safe_Release(m_pNaviMesh);
		m_pNaviMesh = Engine::CNaviMesh::Create(m_pGraphicDev);
		m_mapComponent[Engine::ID_STATIC][L"COM_NAVIMESH"] = m_pNaviMesh;
	//}

	DWORD dwByte = 0;
	_int iVerticesSize = 0;
	_int iTriangleSize = 0;
	ReadFile(hFile, &iVerticesSize, sizeof(_int), &dwByte, nullptr);
	iTriangleSize = iVerticesSize / 3;

	_vec3 vPos[3];
	_int iTriangleTagIndex = 0;
	for (_int i = 0; i < iTriangleSize; ++i) {
		for (_int j = 0; j < 3; ++j) {
			ReadFile(hFile, &vPos[j], sizeof(vPos[j]), &dwByte, nullptr);
		}
		ReadFile(hFile, &iTriangleTagIndex, sizeof(iTriangleTagIndex), &dwByte, nullptr);
		m_pNaviMesh->AddCell(vPos[0], vPos[1], vPos[2], iTriangleTagIndex);
	};
	m_pNaviMesh->Link_Cell();

	CloseHandle(hFile);
}

void CMap::LoadObjects(Engine::CLayer* pLayer, const _tchar * _pFilePath)
{
	if (!_pFilePath)
		return;

	// TODO : 정적 객체 파일을 읽기 위한 코드를 작성합니다.
	HANDLE hFile = CreateFile(_pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	/*if (m_vecChildList.size() > 1) {
		for_each(m_vecChildList.begin() + 1, m_vecChildList.end(), Engine::CDeleteObj());
		m_vecChildList.erase(remove_if(
			m_vecChildList.begin() + 1, m_vecChildList.end(),
			[](auto& rObj) { return rObj == nullptr; }),
			m_vecChildList.end());
	}*/

	for (auto& rObj : pLayer->GetLayerList(L"MapObjects")) {
		rObj->SetValid(false);
	}

	DWORD dwByte = 0;
	// Static
	_int iVecSize = 0;
	ReadFile(hFile, &iVecSize, sizeof(iVecSize), &dwByte, nullptr);			// vector 사이즈 로드

	_bool bIsCustomed = false;
	_int iStrLength = 0;
	_tchar tcFactor[50] = L"";
	Engine::CGameObject* pCustomObject = nullptr;
	CStaticObject* pStaticObject = nullptr;
	for (_int i = 0; i < iVecSize; ++i) {
		pStaticObject = CStaticObject::Create(m_pGraphicDev);
		if (!pStaticObject->LoadInfo(hFile))
			abort();
			//Engine::Safe_Release(pStaticObject);
		else {
			pLayer->Add_GameObject(L"MapObjects", pStaticObject);
		}

		ReadFile(hFile, &bIsCustomed, sizeof(bIsCustomed), &dwByte, nullptr);		// 커스텀 여부 저장
		//pStaticObject->SetCustomed(bIsCustomed);
		if (bIsCustomed) {
			for (_int i = 0; i < 6; ++i) {
				ReadFile(hFile, &iStrLength, sizeof(iStrLength), &dwByte, nullptr);
				ReadFile(hFile, tcFactor, sizeof(_tchar) * (iStrLength + 1), &dwByte, nullptr);
				// TODO : 팩터 해석을 작성합니다.
				//pStaticObject->GetFactorRef(i) = tcFactor;
			}

			if (lstrcmp(L"Snail", pStaticObject->GetMeshTag()) == 0) {
				pCustomObject = CSnail::Create(m_pGraphicDev);
				// TODO : 팩터 해석을 작성합니다.
			}
			else if (lstrcmp(L"Tooth", pStaticObject->GetMeshTag()) == 0) {
				pCustomObject = CTooth::Create(m_pGraphicDev);
			}
			else if (lstrcmp(L"Valve", pStaticObject->GetMeshTag()) == 0) {
				pCustomObject = CValve::Create(m_pGraphicDev);
			}
			else if (lstrcmp(L"Domino", pStaticObject->GetMeshTag()) == 0) {
				pCustomObject = CPopDomino::Create(m_pGraphicDev);
			}

			pCustomObject->GetTransform()->SetPos(pStaticObject->GetTransform()->GetPos());
			pCustomObject->GetTransform()->SetScale(pStaticObject->GetTransform()->GetScale());
			pCustomObject->GetTransform()->SetAngle(pStaticObject->GetTransform()->GetAngle());
			pLayer->Add_GameObject(L"MapObjects", pCustomObject);
			pStaticObject->SetValid(false);
			pStaticObject->SetActivated(false);
			//Engine::Safe_Release(pStaticObject);
		}
		else 
			pLayer->Add_GameObject(L"MapObjects", pStaticObject);
	}

	// Dynamic
	iVecSize = 0;
	ReadFile(hFile, &iVecSize, sizeof(iVecSize), &dwByte, nullptr);
	CDynamicObject* pDynamicObject = nullptr;
	for (_int i = 0; i < iVecSize; ++i) {
		pDynamicObject = CDynamicObject::Create(m_pGraphicDev);
		if (!pDynamicObject->LoadInfo(hFile))
			abort();
			//Engine::Safe_Release(pDynamicObject);
		else {
			pLayer->Add_GameObject(L"MapObjects", pDynamicObject);
			//m_vecDynamicObjects.emplace_back(pDynamicObject);
		}

		ReadFile(hFile, &bIsCustomed, sizeof(bIsCustomed), &dwByte, nullptr);		// 커스텀 여부 저장
		//pDynamicObject->SetCustomed(bIsCustomed);
		if (bIsCustomed) {
			for (_int i = 0; i < 6; ++i) {
				ReadFile(hFile, &iStrLength, sizeof(iStrLength), &dwByte, nullptr);
				ReadFile(hFile, tcFactor, sizeof(_tchar) * (iStrLength + 1), &dwByte, nullptr);
				// TODO : 팩터 해석을 작성합니다.
				//pDynamicObject->GetFactorRef(i) = tcFactor;
			}
		
			if (lstrcmp(L"HPFlower", pDynamicObject->GetMeshTag()) == 0) {
				pCustomObject = CHPFlower::Create(m_pGraphicDev);
				// TODO : 팩터 해석을 작성합니다.
			}
			else if (lstrcmp(L"JumpPad", pDynamicObject->GetMeshTag()) == 0) {
				pCustomObject = CJumpPad::Create(m_pGraphicDev);
			}

			pCustomObject->GetTransform()->SetPos(pDynamicObject->GetTransform()->GetPos());
			pCustomObject->GetTransform()->SetScale(pDynamicObject->GetTransform()->GetScale());
			pCustomObject->GetTransform()->SetAngle(pDynamicObject->GetTransform()->GetAngle());
			pLayer->Add_GameObject(L"MapObjects", pCustomObject);
			pDynamicObject->SetValid(false);
			pDynamicObject->SetActivated(false);
			//Engine::Safe_Release(pDynamicObject);
		}
		else
			pLayer->Add_GameObject(L"MapObjects", pDynamicObject);
	}

	CloseHandle(hFile);
}
