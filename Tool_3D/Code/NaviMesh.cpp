#include "pch.h"
#include "NaviMesh.h"


CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CNaviMesh::CNaviMesh(const CNaviMesh & rhs)
	:
	CGameObject(rhs)
{
}

CNaviMesh::~CNaviMesh(void)
{
}


HRESULT CNaviMesh::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();
	m_pManualCol = AddComponent<Engine::CManualCol>();
	return S_OK;
}

int CNaviMesh::Update_Object(const _float & fTimeDelta)
{
	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CNaviMesh::Render_Object(void)
{
	DWORD dwCullMode;
	m_pGraphicDev->GetRenderState(D3DRS_CULLMODE, &dwCullMode);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	m_pGraphicDev->SetTexture(0, NULL);
	m_pManualCol->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, dwCullMode);



	// 삼각형 라인 그리기
	_matrix matWorld, matView, matProj;
	matWorld = m_pTransform->GetObjectMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	ID3DXLine *pLine;
	D3DXCreateLine(m_pGraphicDev, &pLine);
	pLine->SetWidth(2.f);
	pLine->SetAntialias(true);
	pLine->Begin();

	auto& vecVertices = GetNaviVertices();
	_int iVerticesSize = vecVertices.size();
	_vec3 vLastLine[2];
	for (_int i = 0; i < iVerticesSize / 3; ++i) {
		pLine->DrawTransform(&vecVertices[i * 3], 2, &(matWorld * matView * matProj), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		pLine->DrawTransform(&vecVertices[i * 3 + 1], 2, &(matWorld * matView * matProj), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

		vLastLine[0] = vecVertices[i * 3 + 2];
		vLastLine[1] = vecVertices[i * 3];
		pLine->DrawTransform(vLastLine, 2, &(matWorld * matView * matProj), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	}
	pLine->End();
	pLine->Release();

}

CNaviMesh * CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CNaviMesh::Free(void)
{
	CGameObject::Free();
}

_bool CNaviMesh::IsValidTriangleIndex(_int _iTriangleIndex)
{
	return m_pManualCol->IsValidTriangleIndex(_iTriangleIndex);
}

_bool CNaviMesh::IsValidVertexIndex(_int _iVertexIndex)
{
	return m_pManualCol->IsValidVertexIndex(_iVertexIndex);
}

_bool CNaviMesh::IsValidIndex(_int _iTriangleIndex, _int _iVertexIndex)
{
	return m_pManualCol->IsValidIndex(_iTriangleIndex, _iVertexIndex);
}

void CNaviMesh::PushTriangleVertices(_vec3 _vTriPos1, _vec3 _vTriPos2, _vec3 _vTriPos3)
{
	m_pManualCol->PushTriangleVertices(_vTriPos1, _vTriPos2, _vTriPos3);
}

void CNaviMesh::PopTriangleVertices(_int _iTriangleIndex)
{
	m_pManualCol->PopTriangleVertices(_iTriangleIndex);

	if (-1 == m_iMarkedTriangleIndex)
		return;

	if (_iTriangleIndex < m_iMarkedTriangleIndex) {
		--m_iMarkedTriangleIndex;
	}
	else if (_iTriangleIndex == m_iMarkedTriangleIndex) {
		m_iMarkedTriangleIndex = -1;
	}
}

void CNaviMesh::MarkTriangle(_int _iTriangleIndex)
{
	if (!IsValidTriangleIndex(_iTriangleIndex))
		return;

	// 기존에 마크된 트라이앵글이 있다면, 해제한다.
	if (-1 != m_iMarkedTriangleIndex)
		ReleaseMarkedTriangle();

	// 해당 인덱스의 트라이앵글을 마크한다.
	m_iMarkedTriangleIndex = _iTriangleIndex;
	m_pManualCol->SetTriangleColor(_iTriangleIndex, D3DCOLOR_ARGB(150, 255, 255, 50));
}

void CNaviMesh::ReleaseMarkedTriangle()
{
	if (-1 == m_iMarkedTriangleIndex)
		return;

	// 마크된 삼각형을 해제한다.
	m_pManualCol->SetTriangleColor(m_iMarkedTriangleIndex);
	m_iMarkedTriangleIndex = -1;
}

void CNaviMesh::SetTriangleVertexPosition(_int _iTriangleIndex, _int _iVertexIndex, const _vec3 & _vNewPosition)
{
	m_pManualCol->SetTriangleVertexPosition(_iTriangleIndex, _iVertexIndex, _vNewPosition);
}

_vec3 CNaviMesh::GetTriangleVertexPosition(_int _iTriangleIndex, _int _iVertexIndex) const
{
	return m_pManualCol->GetTriangleVertexPosition(_iTriangleIndex, _iVertexIndex);
}

vector<_vec3>& CNaviMesh::GetNaviVertices()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pManualCol->GetVertices();
}

void CNaviMesh::GenerateNewNaviMesh(vector<_vec3>& _rVertices)
{
	m_pManualCol->GenerateNewNaviMesh(_rVertices);
}

void CNaviMesh::SaveInfo(HANDLE & _hfOut)
{
	auto& rVertices = GetNaviVertices();
	_int iVerticesSize = static_cast<_int>(GetNaviVertices().size());
	WriteFile(_hfOut, &iVerticesSize, sizeof(_int), nullptr, nullptr);

	for (auto& rPos : GetNaviVertices()) {
		WriteFile(_hfOut, &rPos.x, sizeof(rPos.x), nullptr, nullptr);
		WriteFile(_hfOut, &rPos.y, sizeof(rPos.y), nullptr, nullptr);
		WriteFile(_hfOut, &rPos.z, sizeof(rPos.z), nullptr, nullptr);
	}
}

void CNaviMesh::LoadInfo(HANDLE & _hfIn)
{
	_int iVerticesSize = 0;
	ReadFile(_hfIn, &iVerticesSize, sizeof(_int), nullptr, nullptr);
	vector<_vec3> vecVertices;
	vecVertices.reserve(iVerticesSize + 10);

	_vec3 vPos;
	for (_int i = 0; i < iVerticesSize; ++i) {
		ReadFile(_hfIn, &vPos.x, sizeof(vPos.x), nullptr, nullptr);
		ReadFile(_hfIn, &vPos.y, sizeof(vPos.y), nullptr, nullptr);
		ReadFile(_hfIn, &vPos.z, sizeof(vPos.z), nullptr, nullptr);
		vecVertices.emplace_back(vPos);
	}

	GenerateNewNaviMesh(vecVertices);

	vecVertices.clear();
	vecVertices.shrink_to_fit();
}
