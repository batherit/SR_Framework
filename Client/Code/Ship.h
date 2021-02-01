#ifndef Ship_h__
#define Ship_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAttribute;
class CUI_HPBar;
class CShip : public Engine::CGameObject
{

private: // ������, �Ҹ���
	explicit  CShip(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CShip(const  CShip& rhs);
	virtual ~CShip(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CShip*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetHPBar(CUI_HPBar* _pHPBar);
	void Bang();
	void Attacked();
	_bool IsAttacked() const { return m_bIsAttacked; }
	void IncreaseToothNum(_int _iAmount) { m_iToothNum += _iAmount; }
	_int GetToothNum() const { return m_iToothNum; }
	CAttribute* GetAttribute() const { return m_pAttribute; }

private:
	_bool m_bIsAttacked = false;
	_float m_fRecoveryTime = 0.f;
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	Engine::CTexture* m_pTexture = nullptr;
	CAttribute* m_pAttribute = nullptr;

	CUI_HPBar* m_pHPBar = nullptr;

	_float m_fDissolveAmount = 0.f;
	_float m_fDeathAnimTime = 2.f;
	_bool m_bIsDeathInit = false;

	_int m_iToothNum = 0;
};

END

#endif // !LogoObject_h__