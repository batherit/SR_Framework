#ifndef BigGuyB_h__
#define BigGuyB_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CRoute;
class CUI_SpeechBubble;
template<typename T> class CStateMgr;
class CBigGuyB : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CBigGuyB(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CBigGuyB(const  CBigGuyB& rhs);
	virtual ~CBigGuyB(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CBigGuyB*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	_bool LoadRoutePosList(const _tchar* _pRouteFilePath);

	CStateMgr<CBigGuyB>* GetStateMgr() const { return m_pStateMgr; }
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }
	CRoute* GetRoute() const { return m_pRoute; }

	_bool IsTalking() const { return m_bIsTalking; }
	void ShoutOut(_float _fOffsetX, _float _fOffsetY, const wstring & _wstrText);
	//_bool IsMoving(const _float& _fDeltaTime, _vec3* _pDir = nullptr);
	//_bool IsRunOn(const _float&, _vec3* _pDir = nullptr);
	//CMap* GetMap() const { return m_pMap; }
	//_int GetCellIndex() const { return m_iCellIndex; }

private:
	//CMap* m_pMap = nullptr;
	CStateMgr<CBigGuyB>* m_pStateMgr = nullptr;
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	CRoute* m_pRoute = nullptr;

	_bool m_bIsTalking = false;
	CUI_SpeechBubble* m_pSpeechBubble = nullptr;
	//_int m_iCellIndex = -1;
};

END

#endif // !LogoObject_h__
