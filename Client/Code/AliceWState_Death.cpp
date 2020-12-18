#include "pch.h"
#include "AliceWState_Death.h"
#include "AliceWState_Run.h"
#include "AliceWState_Attack.h"
#include "AliceWState_Jump.h"
#include "StateMgr.h"
#include "AliceW.h"


CAliceWState_Death::CAliceWState_Death(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Death::~CAliceWState_Death()
{
}

void CAliceWState_Death::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::Alice_Death);
}

int CAliceWState_Death::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.99f) {
		m_rOwner.SetActivated(false);
		abort();
	}

	return 0;
}

void CAliceWState_Death::OnExited(void)
{
}

void CAliceWState_Death::Free(void)
{
}