#include "InputProcessorMgr.h"
#include "InputProcessor.h"

USING(Engine)

Engine::CInputProcessorMgr::CInputProcessorMgr()
	:
	//m_rGameWorld(_rGameWorld),
	m_bIsConfirmed(true)		// 반.드.시. 초기에는 true로 설정해주어야 함.
{
}

Engine::CInputProcessorMgr::~CInputProcessorMgr()
{
	//Release();
	//Free();
}

//LRESULT CInputProcessorMgr::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	if (!m_pCurInputProcessor) return 0;
//	return m_pCurInputProcessor->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
//}

bool Engine::CInputProcessorMgr::ConfirmValidInputProcessor(void)
{
	if (!m_bIsConfirmed) {				// 상태 변경 요청이 접수된 경우
		Safe_Release(m_pCurInputProcessor);		// 기존 상태를 지운다. 
		m_pCurInputProcessor = m_pNextInputProcessor;		// 기존 상태를 새로운 상태로 교체한다.
		m_pNextInputProcessor = nullptr;			// 새로운 상태 변수 무효화
		m_bIsConfirmed = true;			// 상태 변경 완료!
	}
	if (!m_pCurInputProcessor) return false;		// 그럼에도 현재 유효한 씬이 없다면 false를 반환

	return true;						// 계속 프로세스를 진행
}

void Engine::CInputProcessorMgr::SetNextInputProcessor(CInputProcessor * _pNextInputProcessor)
{
	// 다음 씬이 이미 세팅된 경우, 해당 함수를 무시한다.
	// => 다음 상태 변경까지 새로운 상태로 세팅할 수 없다.
	// for 상태를 동기화해주기 위함.
	if (!m_bIsConfirmed) {
		Safe_Release(_pNextInputProcessor);	// 해당 요청은 삭제한다.
		return;	// 이전에 상태 변경이 요청이 들어왔지만 아직 변경되지 않은 경우
	}

	m_pNextInputProcessor = _pNextInputProcessor;		// 새로이 상태 변경 요청이 들어온 경우.
	m_bIsConfirmed = false;			// 변경 요청이 들어옴.
}

_int Engine::CInputProcessorMgr::ProcessInput(const _float& _fDeltaTime)
{
	if (!ConfirmValidInputProcessor())
		return -1;

	return m_pCurInputProcessor->ProcessInput(_fDeltaTime);
}


CInputProcessorMgr * CInputProcessorMgr::Create(void)
{
	return new CInputProcessorMgr();
}

void Engine::CInputProcessorMgr::Free(void)
{
	Safe_Release(m_pCurInputProcessor);
	Safe_Release(m_pNextInputProcessor);
	m_bIsConfirmed = true;
}