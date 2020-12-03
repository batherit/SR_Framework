﻿// CTerrainTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CTerrainTab.h"
#include "Terrain.h"
#include "afxdialogex.h"
#include "EditScene.h"


// CTerrainTab 대화 상자

IMPLEMENT_DYNAMIC(CTerrainTab, CDialogEx)

CTerrainTab::CTerrainTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TERRAIN_TAB, pParent)
	, m_fSizeWidth(0)
	, m_fSizeDepth(0)
	, m_uiVtxNumWidth(0)
	, m_uiVtxNumHeight(0)
	, m_fU(0)
	, m_fV(0)
{

}

CTerrainTab::~CTerrainTab()
{
}

void CTerrainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_vPos.x);
	DDX_Text(pDX, IDC_EDIT2, m_vPos.y);
	DDX_Text(pDX, IDC_EDIT3, m_vPos.z);
	DDX_Text(pDX, IDC_EDIT4, m_vAngle.x);
	DDX_Text(pDX, IDC_EDIT5, m_vAngle.y);
	DDX_Text(pDX, IDC_EDIT6, m_vAngle.z);
	DDX_Text(pDX, IDC_EDIT7, m_vScale.x);
	DDX_Text(pDX, IDC_EDIT8, m_vScale.y);
	DDX_Text(pDX, IDC_EDIT9, m_vScale.z);
	DDX_Control(pDX, IDC_LIST2, m_lbxTexture);
	DDX_Text(pDX, IDC_EDIT11, m_fSizeWidth);
	DDX_Text(pDX, IDC_EDIT12, m_fSizeDepth);
	DDX_Text(pDX, IDC_EDIT14, m_uiVtxNumWidth);
	DDX_Text(pDX, IDC_EDIT15, m_uiVtxNumHeight);
	DDX_Text(pDX, IDC_EDIT16, m_fU);
	DDX_Text(pDX, IDC_EDIT17, m_fV);
}

BEGIN_MESSAGE_MAP(CTerrainTab, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CTerrainTab::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT2, &CTerrainTab::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT3, &CTerrainTab::OnEnChangeEditPosZ)
	ON_EN_CHANGE(IDC_EDIT4, &CTerrainTab::OnEnChangeEditRotX)
	ON_EN_CHANGE(IDC_EDIT5, &CTerrainTab::OnEnChangeEditRotY)
	ON_EN_CHANGE(IDC_EDIT6, &CTerrainTab::OnEnChangeEditRotZ)
	ON_EN_CHANGE(IDC_EDIT7, &CTerrainTab::OnEnChangeEditScaleX)
	ON_EN_CHANGE(IDC_EDIT8, &CTerrainTab::OnEnChangeEditScaleX)
	ON_EN_CHANGE(IDC_EDIT9, &CTerrainTab::OnEnChangeEditScaleX)
	ON_EN_CHANGE(IDC_EDIT11, &CTerrainTab::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT12, &CTerrainTab::OnEnChangeEditDepth)
	ON_EN_CHANGE(IDC_EDIT14, &CTerrainTab::OnEnChangeEditVtxNumWidth)
	ON_EN_CHANGE(IDC_EDIT15, &CTerrainTab::OnEnChangeEditVtxNumDepth)
	ON_EN_CHANGE(IDC_EDIT16, &CTerrainTab::OnEnChangeEditU)
	ON_EN_CHANGE(IDC_EDIT17, &CTerrainTab::OnEnChangeEditV)
	ON_BN_CLICKED(IDC_BUTTON9, &CTerrainTab::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CTerrainTab::OnBnClickedButtonLoad)
END_MESSAGE_MAP()


// CTerrainTab 메시지 처리기
void CTerrainTab::OnEnChangeEditPosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain)
		pTerrain->GetTransform()->SetPos(m_vPos);
	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain)
		pTerrain->GetTransform()->SetPos(m_vPos);
	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain)
		pTerrain->GetTransform()->SetPos(m_vPos);
	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditRotX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		_vec3 vRadian = _vec3(D3DXToRadian(m_vAngle.x), D3DXToRadian(m_vAngle.y), D3DXToRadian(m_vAngle.z));
		pTerrain->GetTransform()->SetAngle(vRadian);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditRotY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		_vec3 vRadian = _vec3(D3DXToRadian(m_vAngle.x), D3DXToRadian(m_vAngle.y), D3DXToRadian(m_vAngle.z));
		pTerrain->GetTransform()->SetAngle(vRadian);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditRotZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		_vec3 vRadian = _vec3(D3DXToRadian(m_vAngle.x), D3DXToRadian(m_vAngle.y), D3DXToRadian(m_vAngle.z));
		pTerrain->GetTransform()->SetAngle(vRadian);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditScaleX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		pTerrain->GetTransform()->SetScale(m_vScale);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		pTerrain->GetTransform()->SetScale(m_vScale);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditScaleZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		pTerrain->GetTransform()->SetScale(m_vScale);
	}

	UpdateData(FALSE);
}

void CTerrainTab::OnEnChangeEditWidth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTerrainTab::OnEnChangeEditDepth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTerrainTab::OnEnChangeEditVtxNumWidth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTerrainTab::OnEnChangeEditVtxNumDepth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTerrainTab::OnEnChangeEditU()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTerrainTab::OnEnChangeEditV()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTerrainTab::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTerrainTab::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
