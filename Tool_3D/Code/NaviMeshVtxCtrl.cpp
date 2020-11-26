#include "pch.h"
#include "NaviMeshVtxCtrl.h"
#include "NaviMesh.h"

CNaviMeshVtxCtrl::CNaviMeshVtxCtrl(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CNaviMeshVtxCtrl::CNaviMeshVtxCtrl(const CNaviMeshVtxCtrl & rhs)
	: CGameObject(rhs)
{
}

CNaviMeshVtxCtrl::~CNaviMeshVtxCtrl(void)
{
}

HRESULT CNaviMeshVtxCtrl::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();
	
	GetTransform()->SetScale(_vec3(10.f, 10.f, 10.f));
	return S_OK;
}

int CNaviMeshVtxCtrl::Update_Object(const _float & _fDeltaTime)
{
	if (!m_bIsActivated)
		return 0;

	if (!m_bIsPicking) {
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
			// 충돌된 평면을 찾는다.
			auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));
			m_ePlaneType = GetPlaneTypeByRay(stPickingRayInfo);
			if (m_ePlaneType != PLANE::TYPE_END) {
				m_bIsPicking = true;
			}
		}
	}
	else if (m_bIsPicking) {
		if(Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(Engine::DIM_LB)) {
			DragVertex();
		}
		else {
			// 드래그가 끝났다면, 픽킹 상태가 아닌 것이다.
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return 0;
}

void CNaviMeshVtxCtrl::Render_Object(void)
{
	_matrix matWorld, matView, matProj;
	matWorld = m_pTransform->GetObjectMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTexture(0, NULL);

	// 축 그리기
	ID3DXLine *pLine;
	D3DXCreateLine(m_pGraphicDev, &pLine);
	pLine->SetWidth(2.f);
	pLine->SetAntialias(true);
	pLine->Begin();

	for (_int i = 0; i < AXIS_NUM; ++i) {
		if (m_ePlaneType == PLANE::TYPE_XY && i == 2) continue;
		else if (m_ePlaneType == PLANE::TYPE_XZ && i == 1) continue;
		else if (m_ePlaneType == PLANE::TYPE_YZ && i == 0) continue;

		pLine->DrawTransform(m_vAxisVertices[i], 2, &(matWorld * matView * matProj), m_clAxisColor[i]);
	}
	pLine->End();
	pLine->Release();
	
}

CNaviMeshVtxCtrl * CNaviMeshVtxCtrl::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMeshVtxCtrl*	pInstance = new CNaviMeshVtxCtrl(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CNaviMeshVtxCtrl::Free(void)
{
	CGameObject::Free();
}

void CNaviMeshVtxCtrl::SetVertexInfo(CNaviMesh * _pNaviMesh, _int _iTriangleIndex, _int _iVertexIndex)
{
	if (!_pNaviMesh || !_pNaviMesh->IsValidIndex(_iTriangleIndex, _iVertexIndex))
		return;

	m_pNaviMesh = _pNaviMesh;
	m_iTriangleIndex = _iTriangleIndex;
	m_iVertexIndex = _iVertexIndex;

	// 정점 위치에 컨트롤을 일치시킨다.(동기화)
	GetTransform()->SetPos(_pNaviMesh->GetTriangleVertexPosition(_iTriangleIndex, _iVertexIndex));
}

PLANE::E_TYPE CNaviMeshVtxCtrl::GetPlaneTypeByRay(Engine::PICKINGRAYINFO & _rRayInfo)
{
	PLANE::E_TYPE ePlaneType = PLANE::TYPE_END;
	_float fU, fV, fDist;
	_vec3 vMinHitPos;
	_vec3 vTempHitPos;
	_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
	_matrix matWorld = GetTransform()->GetObjectMatrix();

	// 1) XY 평면
	for (_int i = 0; i < TRIANGLE_NUM; ++i) {
		// 삼각형 로컬 정점 좌표를 월드 좌표로 변환한다.
		for (_int j = 0; j < TRI_VERTEX_NUM; ++j) {
			D3DXVec3TransformCoord(&vWorldTriVtx[j], &XYTriLocal[i][j], &matWorld);
		}
		// 충돌했는지를 체크한다.
		if (D3DXIntersectTri(&vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2],
			&_rRayInfo.vRayPos, &_rRayInfo.vRayDir, &fU, &fV, &fDist)) {

			// 충돌했다면, 충돌 평면 타입과 광선 충돌 지점을 저장한다.
			vMinHitPos = Engine::GetHitPos(vWorldTriVtx[0], vWorldTriVtx[1], vWorldTriVtx[2], fU, fV);
			ePlaneType = PLANE::TYPE_XY;
			break;
		}
	}
	// 2) XZ 평면
	for (_int i = 0; i < TRIANGLE_NUM; ++i) {
		// 삼각형 로컬 정점 좌표를 월드 좌표로 변환한다.
		for (_int j = 0; j < TRI_VERTEX_NUM; ++j) {
			D3DXVec3TransformCoord(&vWorldTriVtx[j], &XZTriLocal[i][j], &matWorld);
		}
		// 충돌했는지를 체크한다.
		if (D3DXIntersectTri(&vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2],
			&_rRayInfo.vRayPos, &_rRayInfo.vRayDir, &fU, &fV, &fDist)) {

			// 충돌했다면, 충돌 평면 타입과 광선 충돌 지점을 저장한다.
			vTempHitPos = Engine::GetHitPos(vWorldTriVtx[0], vWorldTriVtx[1], vWorldTriVtx[2], fU, fV);

			// 기존 히트 포스와 거리를 비교하여 가까우면 해당 평면 타입으로 갱신한다.
			if (D3DXVec3LengthSq(&(vMinHitPos - _rRayInfo.vRayPos)) > D3DXVec3LengthSq(&(vTempHitPos - _rRayInfo.vRayPos))) {
				vMinHitPos = vTempHitPos;
				ePlaneType = PLANE::TYPE_XZ;
				break;
			}
		}
	}
	// 3) YZ 평면
	for (_int i = 0; i < TRIANGLE_NUM; ++i) {
		// 삼각형 로컬 정점 좌표를 월드 좌표로 변환한다.
		for (_int j = 0; j < TRI_VERTEX_NUM; ++j) {
			D3DXVec3TransformCoord(&vWorldTriVtx[j], &YZTriLocal[i][j], &matWorld);
		}
		// 충돌했는지를 체크한다.
		if (D3DXIntersectTri(&vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2],
			&_rRayInfo.vRayPos, &_rRayInfo.vRayDir, &fU, &fV, &fDist)) {

			// 충돌했다면, 충돌 평면 타입과 광선 충돌 지점을 저장한다.
			vTempHitPos = Engine::GetHitPos(vWorldTriVtx[0], vWorldTriVtx[1], vWorldTriVtx[2], fU, fV);

			// 기존 히트 포스와 거리를 비교하여 가까우면 해당 평면 타입으로 갱신한다.
			if (D3DXVec3LengthSq(&(vMinHitPos - _rRayInfo.vRayPos)) > D3DXVec3LengthSq(&(vTempHitPos - _rRayInfo.vRayPos))) {
				ePlaneType = PLANE::TYPE_YZ;
				break;
			}
		}
	}

	return ePlaneType;
}

void CNaviMeshVtxCtrl::DragVertex()
{
	switch (m_ePlaneType) {
	case PLANE::TYPE_XY:
		// TODO : XY 평면에 대한 이동 갱신
	{
		// XY 평면을 구한다.
		D3DXPLANE planeXY;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// 로컬 삼각형 정점들을 월드 좌표로 변환한다.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &XYTriLocal[0][i], &matWorld);
		}

		// 월드 XY 평면을 구한다
		D3DXPlaneFromPoints(&planeXY, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// 광선을 구한다.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// 광선과 평면이 충돌했는지 확인한다.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeXY, &fT)) {
			vHitPos = stPickingRayInfo.vRayPos + fT * stPickingRayInfo.vRayDir;
			// 충돌 지점으로 삼각형 정점과 컨트롤러를 이동시킨다.
			m_pNaviMesh->SetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex, vHitPos);
			GetTransform()->SetPos(vHitPos);
		}
		else {
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}
	break;
	case PLANE::TYPE_XZ:
		// TODO : XZ 평면에 대한 이동 갱신
	{
		D3DXPLANE planeXZ;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// 로컬 삼각형 정점들을 월드 좌표로 변환한다.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &XZTriLocal[0][i], &matWorld);
		}

		// 월드 XY 평면을 구한다
		D3DXPlaneFromPoints(&planeXZ, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// 광선을 구한다.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// 광선과 평면이 충돌했는지 확인한다.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeXZ, &fT)) {
			vHitPos = stPickingRayInfo.vRayPos + fT * stPickingRayInfo.vRayDir;
			// 충돌 지점으로 삼각형 정점과 컨트롤러를 이동시킨다.
			m_pNaviMesh->SetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex, vHitPos);
			GetTransform()->SetPos(vHitPos);
		}
		else {
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}
	break;
	case PLANE::TYPE_YZ:
		// TODO : YZ 평면에 대한 이동 갱신
	{
		D3DXPLANE planeYZ;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// 로컬 삼각형 정점들을 월드 좌표로 변환한다.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &YZTriLocal[0][i], &matWorld);
		}

		// 월드 XY 평면을 구한다
		D3DXPlaneFromPoints(&planeYZ, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// 광선을 구한다.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// 광선과 평면이 충돌했는지 확인한다.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeYZ, &fT)) {
			vHitPos = stPickingRayInfo.vRayPos + fT * stPickingRayInfo.vRayDir;
			// 충돌 지점으로 삼각형 정점과 컨트롤러를 이동시킨다.
			m_pNaviMesh->SetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex, vHitPos);
			GetTransform()->SetPos(vHitPos);
		}
		else {
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}
	break;
	default:
		m_ePlaneType = PLANE::TYPE_END;
		m_bIsPicking = false;
		break;
	}
}
