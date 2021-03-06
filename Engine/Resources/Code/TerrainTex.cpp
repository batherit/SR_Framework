#include "TerrainTex.h"

USING(Engine)
Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
	: CVIBuffer(rhs),
	m_iNumOfVerticesW(rhs.m_iNumOfVerticesW),
	m_iNumOfVerticesH(rhs.m_iNumOfVerticesH),
	m_fWidth(rhs.m_fWidth),
	m_fHeight(rhs.m_fHeight),
	m_fU(rhs.m_fU),
	m_fV(rhs.m_fV),
	m_vecVertices(rhs.m_vecVertices),
	m_vecIndexes(rhs.m_vecIndexes)
{

}

Engine::CTerrainTex::~CTerrainTex(void)
{

}

HRESULT Engine::CTerrainTex::Ready_Buffer(void)
{
	// default
	return SetTerrainInfo();
}

void CTerrainTex::Render_Buffer(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXTEX));
	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CTerrainTex::SetTerrainInfo(_uint _iNumOfVerticesW, _uint _iNumOfVerticesH, _float _fWidth, _float _fHeight, _float _fU, _float _fV, const _tchar* _szHeightMapFileName)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	m_iBitmapSizeW = _iNumOfVerticesW;
	m_iBitmapSizeH = _iNumOfVerticesH;

	// 높이맵 로드하기
	LoadHeightMap(_szHeightMapFileName);

	//m_vStartPos = _vStartPos;
	m_iNumOfVerticesW = _iNumOfVerticesW;
	m_iNumOfVerticesH = _iNumOfVerticesH;
	m_fWidth = _fWidth;
	m_fHeight = _fHeight;
	m_fU = _fU;
	m_fV = _fV;

	m_dwFVF = FVF_TEX;
	m_dwTriCnt = (_iNumOfVerticesW - 1) * (_iNumOfVerticesH - 1) * 2;	
	m_dwVtxCnt = _iNumOfVerticesW * _iNumOfVerticesH;		//4;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	// 버퍼 생성
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	// 1인자 : 어디서부터 잠글 것인가
	// 2인자 : 숫자가 0이면 전체 영역을 잠근다.
	// 3인자 : 인자값을 통해서 버텍스 버퍼 내 정점 중 첫번째 주소를 얻어온다.
	// 4인자 : 잠그는 형태를 묻는 인자, 정적 버퍼인 경우 

	// 시스템 메모리에 둘 정점 버퍼
	m_vecVertices.resize(_iNumOfVerticesW * _iNumOfVerticesH);

	_float fCU = m_fU / (_iNumOfVerticesW - 1);
	_float fCV = m_fV / (_iNumOfVerticesH - 1);
	_float fCX = _fWidth / (_iNumOfVerticesW - 1);
	_float fCY = _fHeight / (_iNumOfVerticesH - 1);
	_int iIndexVB = 0;
	_int iIndexPixel = 0;
	for (_uint i = 0; i < _iNumOfVerticesH; ++i) {
		for (_uint j = 0; j < _iNumOfVerticesW; ++j) {
			iIndexVB = _iNumOfVerticesW * i + j;
			iIndexPixel = static_cast<_int>(m_iBitmapSizeW * static_cast<_int>(i * static_cast<_float>(m_iBitmapSizeH - 1) 
				/ (_iNumOfVerticesH - 1)) + j * static_cast<_float>(m_iBitmapSizeW - 1) / (_iNumOfVerticesW - 1));
			m_vecVertices[iIndexVB] = pVertex[iIndexVB].vPos = _vec3(j * fCX, (m_pHeightMapData ? (m_pHeightMapData[m_stBitmapInfoHeader.biBitCount/CHAR_BIT * iIndexPixel] * 1.f) : 0.f), (i * fCY));
			pVertex[iIndexVB].vNormal = _vec3(0.f, 0.f, 0.f);		// 인덱스 버퍼를 생성할 때 노멀 벡터를 생성한다.
			pVertex[iIndexVB].vTexUV = _vec2(fCU * j, 1.f - fCV * i);
		}
	}

	//vector<INDEX16> indices;
	m_vecIndexes.resize(2 * (_iNumOfVerticesW - 1) * (_iNumOfVerticesH - 1));
	m_vecIndexes.clear();

	INDEX16 Indexes;
	_ulong	dwTriIdx = 0;
	_vec3	vDest, vSour, vNormal;
	for (_uint i = 1; i < _iNumOfVerticesH; ++i) {
		for (_uint j = 0; j < _iNumOfVerticesW - 1; ++j) {
			// 좌상 우상 우하
			Indexes = INDEX16{
				static_cast<_ushort>(0 + i * _iNumOfVerticesW + j),
				static_cast<_ushort>(1 + i * _iNumOfVerticesW + j),
				static_cast<_ushort>(1 + (i - 1) * _iNumOfVerticesW + j) };
			m_vecIndexes.emplace_back(Indexes);
			//indices.emplace_back(Indexes);

			// 법선 벡터 구하기
			vDest = pVertex[m_vecIndexes[dwTriIdx]._1].vPos - pVertex[m_vecIndexes[dwTriIdx]._0].vPos;
			vSour = pVertex[m_vecIndexes[dwTriIdx]._2].vPos - pVertex[m_vecIndexes[dwTriIdx]._1].vPos;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			pVertex[m_vecIndexes[dwTriIdx]._0].vNormal += vNormal;
			pVertex[m_vecIndexes[dwTriIdx]._1].vNormal += vNormal;
			pVertex[m_vecIndexes[dwTriIdx]._2].vNormal += vNormal;
			++dwTriIdx;

			// 우하 좌하 좌상
			Indexes = INDEX16{
				static_cast<_ushort>(1 + (i - 1) * _iNumOfVerticesW + j),
				static_cast<_ushort>(0 + (i - 1) * _iNumOfVerticesW + j),
				static_cast<_ushort>(0 + i * _iNumOfVerticesW + j) };
			m_vecIndexes.emplace_back(Indexes);
			//indices.emplace_back(Indexes);

			// 법선 벡터 구하기
			vDest = pVertex[m_vecIndexes[dwTriIdx]._1].vPos - pVertex[m_vecIndexes[dwTriIdx]._0].vPos;
			vSour = pVertex[m_vecIndexes[dwTriIdx]._2].vPos - pVertex[m_vecIndexes[dwTriIdx]._1].vPos;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			pVertex[m_vecIndexes[dwTriIdx]._0].vNormal += vNormal;
			pVertex[m_vecIndexes[dwTriIdx]._1].vNormal += vNormal;
			pVertex[m_vecIndexes[dwTriIdx]._2].vNormal += vNormal;
			++dwTriIdx;
		}
	}

	// 합해진 노멀벡터는 단위벡터가 아닐 수 있으므로 단위벡터로 만들어 준다.
	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);

	VOID* pIndices;
	m_pIB->Lock(0, sizeof(INDEX16) * m_vecIndexes.size(), (void**)&pIndices, 0);
	memcpy(pIndices, m_vecIndexes.data(), sizeof(INDEX16) * m_vecIndexes.size());

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

_float Engine::CTerrainTex::GetHeight(const _vec3& _vPos) {
	_vec3 vIndexPos = _vPos /*- m_vStartPos*/;

	if (vIndexPos.x < 0.f || vIndexPos.x > m_fWidth) return 0.f;
	if (vIndexPos.z < 0.f || vIndexPos.z > m_fHeight) return 0.f;

	_float fCX = m_fWidth / (m_iNumOfVerticesW - 1);
	_float fCY = m_fHeight / (m_iNumOfVerticesH - 1);

	_float fRow = vIndexPos.z / fCY + 1;
	_float fCol = vIndexPos.x / fCX;
	_int iRow = static_cast<_int>(fRow);
	_int iCol = static_cast<_int>(fCol);

	//// 우상단 인덱스 구하기
	//_vec3 vRightTopPos = m_vecVertices[(iRow + 1) * m_iNumOfVerticesW + iCol + 1];
	//_vec3 vLeftBottomPos = m_vecVertices[iRow * m_iNumOfVerticesW + iCol];
	D3DXPLANE plane;
	if (fRow - static_cast<_float>(iRow) > fCol - static_cast<_float>(iCol)) {
		// 좌하단에 더 가깝다.
		D3DXPlaneFromPoints(&plane,
			&m_vecVertices[iRow * m_iNumOfVerticesW + iCol],
			&m_vecVertices[iRow * m_iNumOfVerticesW + iCol + 1],
			&m_vecVertices[(iRow - 1) * m_iNumOfVerticesW + iCol + 1]);
	}
	else {
		// 우상단에 더 가깝다.
		D3DXPlaneFromPoints(&plane,
			&m_vecVertices[iRow * m_iNumOfVerticesW + iCol],
			&m_vecVertices[(iRow - 1) * m_iNumOfVerticesW + iCol + 1],
			&m_vecVertices[(iRow - 1) * m_iNumOfVerticesW + iCol]);
	}

	return (plane.a * _vPos.x + plane.c * _vPos.z  + plane.d) / -plane.b;

	// 선형 보간으로도 구할 수 있다.
}


Engine::CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrainTex*	pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))		// 버퍼 생성하고 정점 정보를 세팅한다.
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTerrainTex::Clone(void)
{
	return new CTerrainTex(*this);
}

_bool CTerrainTex::LoadHeightMap(const _tchar * _szHeightMapFileName)
{
	if (!_szHeightMapFileName) {
		delete m_pHeightMapData;
		m_pHeightMapData = nullptr;
		return false;
	}
		
	// 바이너리 모드로 높이맵 파일 열기
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, _szHeightMapFileName, L"rb");
	
	if (!err) {
		// 파일 헤더를 읽기
		BITMAPFILEHEADER bmpFileHeader;
		if (fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp) != 1) {
			if (fp) fclose(fp);
			return false;
		}

		// 비트맵 정보 헤더 읽기
		//BITMAPINFOHEADER bmpInfoHeader;
		if (fread(&m_stBitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp) != 1) {
			if (fp) fclose(fp);
			return false;
		}

		//// 비트맵 크기/픽셀 비트를 저장하기
		m_iBitmapSizeW = m_stBitmapInfoHeader.biWidth;
		m_iBitmapSizeH = m_stBitmapInfoHeader.biHeight;

		// 비트맵 이미지 데이터의 크기를 계산하기
		_int iImageSize = (m_stBitmapInfoHeader.biBitCount / CHAR_BIT) * m_stBitmapInfoHeader.biWidth * m_stBitmapInfoHeader.biHeight;

		// 비트맵 이미지 데이터를 위한 메모리 할당하기
		if (m_pHeightMapData) {
			delete m_pHeightMapData;
			m_pHeightMapData = nullptr;
		}
		
		m_pHeightMapData = new _ubyte[iImageSize];

		// 비트맵 데이터의 시작 부분으로 이동하기
		fseek(fp, bmpFileHeader.bfOffBits, SEEK_SET);

		// 비트맵 이미지 로드하기
		if (fread(m_pHeightMapData, 1, iImageSize, fp) != iImageSize) {
			if (m_pHeightMapData) {
				delete m_pHeightMapData;
				m_pHeightMapData = nullptr;
			}
			if (fp) 
				fclose(fp);
			return false;
		}
	}
	else if(m_pHeightMapData){
		delete m_pHeightMapData;
		m_pHeightMapData = nullptr;
		return false;
	}

	// 파일 닫기
	if (fp) fclose(fp);
	
	return true;
}

void Engine::CTerrainTex::Free(void)
{
	m_vecVertices.clear();
	m_vecVertices.shrink_to_fit();
	m_vecIndexes.clear();
	m_vecIndexes.shrink_to_fit();
	Safe_Delete(m_pHeightMapData);
	CVIBuffer::Free();
}


//VTXTEX*		pVertex = NULL;
	//m_pVB->Lock(0, 0, (void**)&pVertex, NULL);

	//// 우상단 인덱스 구하기
	//vRightTopPos = pVertex[(iRow + 1) * m_iNumOfVerticesW + iCol + 1].vPos;
	//vLeftBottomPos = pVertex[iRow * m_iNumOfVerticesW + iCol].vPos;

	//if (D3DXVec3LengthSq(&(vRightTopPos - _vPos)) > D3DXVec3LengthSq(&(vLeftBottomPos - _vPos))) {
	//	// 좌하단에 더 가깝다.
	//	D3DXPlaneFromPoints(&plane,
	//		&pVertex[iRow * m_iNumOfVerticesW + iCol + 1].vPos,
	//		&pVertex[iRow * m_iNumOfVerticesW + iCol].vPos,
	//		&pVertex[(iRow + 1) * m_iNumOfVerticesW + iCol].vPos);
	//}
	//else {
	//	// 우상단에 더 가깝다.
	//	D3DXPlaneFromPoints(&plane,
	//		&pVertex[(iRow + 1) * m_iNumOfVerticesW + iCol].vPos,
	//		&pVertex[(iRow + 1) * m_iNumOfVerticesW + iCol + 1].vPos,
	//		&pVertex[iRow * m_iNumOfVerticesW + iCol + 1].vPos);
	//}
	//m_pVB->Unlock();