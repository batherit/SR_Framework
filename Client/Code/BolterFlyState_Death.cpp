#include "pch.h"
#include "BolterFlyState_Death.h"
#include "StateMgr.h"
#include "BolterFly.h"


CBolterFlyState_Death::CBolterFlyState_Death(CBolterFly & _rOwner)
	:
	CState(_rOwner)
{
}

CBolterFlyState_Death::~CBolterFlyState_Death()
{
}

void CBolterFlyState_Death::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Attack_MeLee3);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
}

int CBolterFlyState_Death::Update(const _float& _fDeltaTime)
{
	if (!m_rOwner.GetDynamicMesh()->IsAnimStopping() && m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.8f) {
		m_rOwner.GetDynamicMesh()->SetAnimationStop(true);
	}
	else if ((m_fElapsedTime += _fDeltaTime) >= 3.f) {
		m_rOwner.SetValid(false);
	}

	return 0;
}

void CBolterFlyState_Death::OnExited(void)
{
}

void CBolterFlyState_Death::Free(void)
{
}
