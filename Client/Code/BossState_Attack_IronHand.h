#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBoss;
class CBossState_Attack_IronHand :
	public CState<CBoss>
{
public:
	CBossState_Attack_IronHand(CBoss& _rOwner);
	virtual ~CBossState_Attack_IronHand();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	_bool m_bIsAttack = false;
	_float m_fTickTime = 0.f;
	_int m_iAttackCount = 0;
};

END
