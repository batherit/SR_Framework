#ifndef ColliderObject_AABB_h__
#define	ColliderObject_AABB_h__

#include "Engine_Define.h"
#include "ColliderObject.h"

BEGIN(Engine)
class CCubeCol;
class CPolygonRenderer;
class ENGINE_DLL CColliderObject_AABB : public CColliderObject
{
protected:
	explicit CColliderObject_AABB(LPDIRECT3DDEVICE9	pGraphicDev);	// 일반 생성자
	explicit CColliderObject_AABB(const CColliderObject_AABB& rhs);			// 복사 생성자
	virtual ~CColliderObject_AABB();

public:
	virtual HRESULT Ready_Object(void);
	virtual _int Update_Object(const _float& _fDeltaTime);
	virtual void Render_Object(void);

public:
	// 파일 입출력 관련
	virtual _bool SaveInfo(FILE* _fpOut) { return true; };
	virtual _bool LoadInfo(FILE* _fpIn) { return true; };
	virtual _bool SaveInfo(HANDLE& _hfOut);
	virtual _bool LoadInfo(HANDLE& _hfIn);

	virtual void Free(void) override;
	static CColliderObject_AABB* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void GetCollisionInfo(_vec3& _vMin, _vec3& _vMax);
	BOXINFO GetBoxInfo();

	void SetAutoGenerating(_bool _bIsAutoGenerating) { m_bIsAutoGenerating = _bIsAutoGenerating; if (_bIsAutoGenerating) GenerateAABB(); }
	_bool IsAutoGenerating() const { return m_bIsAutoGenerating; }
	_bool GenerateAABB();	// 콜라이더의 부모가 세팅된 이후 호출해야 유효하다.

private:
	CCubeCol* m_pCubeBuffer = nullptr;
	CPolygonRenderer* m_pRenderer = nullptr;
	_bool m_bIsAutoGenerating = false;
	_vec3 m_vMinW = _vec3(-0.5f, -0.5f, -0.5f);
	_vec3 m_vMaxW = _vec3(0.5f, 0.5f, 0.5f);
};

END

#endif