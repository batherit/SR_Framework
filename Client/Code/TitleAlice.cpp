#include "pch.h"
#include "TitleAlice.h"

CTitleAlice::CTitleAlice(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CTitleAlice::CTitleAlice(const CTitleAlice & rhs)
	:
	CGameObject(rhs)
{
}

CTitleAlice::~CTitleAlice(void)
{
}

HRESULT CTitleAlice::Ready_Object(void)
{
	// Mesh
	//if(m_pMesh)
	//	m_mapComponent[Engine::ID_Dynamic].emplace(L"Com_Mesh", m_pMesh);

	// Renderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderID(Engine::RENDER_NONALPHA);
	//m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	return S_OK;
}

int CTitleAlice::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	m_pMesh->Play_Animation(_fDeltaTime);

	m_pRenderer->Update(_fDeltaTime);

	CGameObject::Update_Object(_fDeltaTime);
	return 0;
}

void CTitleAlice::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	m_pMesh->UpdateAnimation();
	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW())) {
		return;
	}
	if (!IsVisible())
		return;

	//_matrix matView, matProj;
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	//m_pRenderer->SetViewMatrix(matView);
	//m_pRenderer->SetProjMatirx(matProj);

	m_pRenderer->Render();
}

CTitleAlice * CTitleAlice::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTitleAlice*	pInstance = new CTitleAlice(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

_bool CTitleAlice::SetRenderInfo(const _tchar * _pMeshTag, Engine::RENDERID _eRenderID)
{
	if (!_pMeshTag || _eRenderID >= Engine::RENDER_END)
		return false;

	lstrcpy(m_tcMeshTag, _pMeshTag);
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, m_tcMeshTag));

	if (!m_pMesh)
		return false;

	Engine::CComponent* pComponent = m_mapComponent[Engine::ID_STATIC][Engine::CDynamicMesh::GetComponentTag()];
	if (pComponent) {
		// ���� ���õ� �޽��� �ִٸ� �����Ѵ�.
		Engine::Safe_Release(pComponent);
	}

	// ���ο� �޽÷� �����Ѵ�.
	m_mapComponent[Engine::ID_STATIC][Engine::CDynamicMesh::GetComponentTag()] = m_pMesh;
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	LoadCollidersInfo();

	return true;
}

void CTitleAlice::Free(void)
{
	CGameObject::Free();
}

_bool CTitleAlice::SaveInfo(HANDLE & _hfOut)
{
	DWORD dwByte = 0;

	// 1) �޽� �±�
	_int iStrLen = lstrlen(m_tcMeshTag);
	WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	WriteFile(_hfOut, &m_tcMeshTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	// 2) ���� ���̵�
	Engine::RENDERID eRenderID = m_pRenderer->GetRenderID();
	WriteFile(_hfOut, &eRenderID, sizeof(eRenderID), &dwByte, nullptr);

	// 3) Ʈ������ ���� ����
	_vec3 vPos = GetTransform()->GetPos();
	_vec3 vAngle = GetTransform()->GetAngle();
	_vec3 vScale = GetTransform()->GetScale();
	WriteFile(_hfOut, &vPos, sizeof(vPos), &dwByte, nullptr);
	WriteFile(_hfOut, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	WriteFile(_hfOut, &vScale, sizeof(vScale), &dwByte, nullptr);

	return true;
}

_bool CTitleAlice::LoadInfo(HANDLE & _hfIn)
{
	DWORD dwByte = 0;

	// 1) �޽� �±� �ε�
	_int iStrLen = 0;
	ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	ReadFile(_hfIn, &m_tcMeshTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	// 2) ���� ���̵�
	Engine::RENDERID eRenderID = Engine::RENDER_END;
	ReadFile(_hfIn, &eRenderID, sizeof(eRenderID), &dwByte, nullptr);

	// 3) Ʈ������ ���� ����
	_vec3 vPos, vAngle, vScale;
	ReadFile(_hfIn, &vPos, sizeof(vPos), &dwByte, nullptr);
	ReadFile(_hfIn, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	ReadFile(_hfIn, &vScale, sizeof(vScale), &dwByte, nullptr);
	GetTransform()->SetPos(vPos);
	GetTransform()->SetAngle(vAngle);
	GetTransform()->SetScale(vScale);

	return SetRenderInfo(m_tcMeshTag, eRenderID);
}

//_bool CTitleAlice::SaveCollidersInfo(HANDLE & _hfOut)
//{
//	DWORD dwByte = 0;
//
//	char cBoneName[MAX_PATH] = "";
//	_tchar tcColliderTag[MAX_PATH] = L"";
//	Engine::E_COLLIDER_TYPE eColliderType = Engine::TYPE_END;
//
//	_int iBoneNameSize = m_vecColliders.size();
//	_int iCollidersSize = 0;
//	_int iStrLen = 0;
//	Engine::CColliderObject* pCollider = nullptr;
//
//	WriteFile(_hfOut, &iBoneNameSize, sizeof(iBoneNameSize), &dwByte, nullptr);					// �� ���� ����
//	// �ݶ��̴��� ������ �� ������ŭ ��ȸ
//	for (_int i = 0; i < iBoneNameSize; ++i) {
//		strcpy_s(cBoneName, m_vecColliders[i].first.c_str());
//		iStrLen = strlen(cBoneName);
//		WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);							// �� �̸� ���� ����
//		WriteFile(_hfOut, &cBoneName, sizeof(char) * (iStrLen + 1), &dwByte, nullptr);			// �� �̸� ����
//
//		// �ϳ��� ���� ���� �ݶ��̴� ���� �����Ѵ�.
//		iCollidersSize = m_vecColliders[i].second.size();
//		WriteFile(_hfOut, &iCollidersSize, sizeof(iCollidersSize), &dwByte, nullptr);			// �ݶ��̴� �� ����
//
//		for (_int j = 0; j < iCollidersSize; ++j) {
//			pCollider = m_vecColliders[i].second[j];
//
//			lstrcpy(tcColliderTag, pCollider->GetColliderTag());
//			iStrLen = lstrlen(pCollider->GetColliderTag());
//			WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
//			WriteFile(_hfOut, &tcColliderTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);// �ݶ��̴� �±� ����
//			// � �ݶ��̴� Ÿ�������� �����Ѵ�.
//			eColliderType = pCollider->GetColliderType();
//			WriteFile(_hfOut, &eColliderType, sizeof(eColliderType), &dwByte, nullptr);			// �ݶ��̴� Ÿ�� ����
//
//			pCollider->SaveInfo(_hfOut);														// �ݶ��̴� Ʈ������ ����
//		}
//	}
//
//	return true;
//}
//
//_bool CTitleAlice::LoadCollidersInfo(HANDLE & _hfIn)
//{
//	ClearColliders();
//
//	DWORD dwByte = 0;
//
//	char cBoneName[MAX_PATH] = "";
//	_tchar tcColliderTag[MAX_PATH] = L"";
//	Engine::E_COLLIDER_TYPE eColliderType = Engine::TYPE_END;
//
//	_int iBoneNameSize = 0;
//	_int iCollidersSize = 0;
//	_int iStrLen = 0;
//	Engine::CColliderObject* pCollider = nullptr;
//
//	ReadFile(_hfIn, &iBoneNameSize, sizeof(iBoneNameSize), &dwByte, nullptr);						// �� ���� �б�
//	// �ݶ��̴��� ������ �� ������ŭ ��ȸ
//	for (_int i = 0; i < iBoneNameSize; ++i) {
//		ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);								// �� �̸� ���� �б�
//		ReadFile(_hfIn, &cBoneName, sizeof(char) * (iStrLen + 1), &dwByte, nullptr);				// �� �̸� �б�
//
//		// �ϳ��� ���� ���� �ݶ��̴� ���� �����Ѵ�.
//		ReadFile(_hfIn, &iCollidersSize, sizeof(iCollidersSize), &dwByte, nullptr);					// �ݶ��̴� �� �б�
//
//		for (_int j = 0; j < iCollidersSize; ++j) {
//			ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
//			ReadFile(_hfIn, &tcColliderTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);		// �ݶ��̴� �±� �б�
//
//			// � �ݶ��̴� Ÿ�������� �����Ѵ�.
//			ReadFile(_hfIn, &eColliderType, sizeof(eColliderType), &dwByte, nullptr);				// �ݶ��̴� Ÿ�� �б�
//
//			switch (eColliderType) {																// �ݶ��̴� Ʈ������ �б�
//			case Engine::TYPE_SPHERE:
//				pCollider = Engine::CColliderObject_Sphere::Create(m_pGraphicDev);
//				pCollider->LoadInfo(_hfIn);
//				if (!AddCollider(pCollider, cBoneName))
//					abort();
//				break;
//			case Engine::TYPE_AABB:
//				pCollider = Engine::CColliderObject_AABB::Create(m_pGraphicDev);
//				pCollider->LoadInfo(_hfIn);
//				if (!AddCollider(pCollider, cBoneName))
//					abort();
//				break;
//			case Engine::TYPE_OBB:
//				pCollider = Engine::CColliderObject_OBB::Create(m_pGraphicDev);
//				pCollider->LoadInfo(_hfIn);
//				if (!AddCollider(pCollider, cBoneName))
//					abort();
//				break;
//			}
//
//			pCollider->SetColliderTag(tcColliderTag);
//		}
//	}
//
//	m_pCullingSphere = dynamic_cast<Engine::CColliderObject_Sphere*>(GetColliderFromTag(L"CULL"));
//
//	return true;
//}

_bool CTitleAlice::LoadCollidersInfo()
{
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Colliders\\");

	lstrcat(szDataPath, m_tcMeshTag);
	lstrcat(szDataPath, L".col");

	HANDLE hFile = CreateFileW(szDataPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	CGameObject::LoadCollidersInfo(hFile);

	CloseHandle(hFile);
	return true;
}