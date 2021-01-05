﻿#pragma once


// CMapTab 대화 상자
BEGIN(Client)
class CStaticObject;
END

class CMapTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMapTab)

public:
	CMapTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMapTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAP_TAB };
#endif

public:
	CTreeCtrl m_treeObjectList;
	CTreeCtrl m_treeAddedObject;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnNMClickTreeObjectToAdd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTreeAddedObject(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void UpdateAddedTree(const vector<CStaticObject*>& rStaticObjects);
	void UpdatePos(const _vec3& vPos);
	void UpdateAngle(const _vec3& vAngle);
	void UpdateScale(const _vec3& vScale);
	void SetSelectedObject(CStaticObject* _pStaticObject);
	_bool AddStaticObject(const _tchar* _pMeshTag);
	void RegisterMeshTag(Engine::MESHTYPE _eMeshType, const _tchar * _pMeshTag);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	HTREEITEM m_hSelectedTreeItem = nullptr;
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnEnChangeEditPosZ();
	afx_msg void OnEnChangeEditRotX();
	afx_msg void OnEnChangeEditRotY();
	afx_msg void OnEnChangeEditRotZ();
	afx_msg void OnEnChangeEditScaleX();
	afx_msg void OnEnChangeEditScaleY();
	afx_msg void OnEnChangeEditScaleZ();
	afx_msg void OnBnClickedRadioPosition();
	afx_msg void OnBnClickedRadioRotation();
	afx_msg void OnBnClickedRadioScale();

	HTREEITEM m_itemStaticL = NULL;
	HTREEITEM m_itemCustomL = NULL;
	HTREEITEM m_itemStaticR = NULL;
	HTREEITEM m_itemCustomR = NULL;

	CStaticObject* m_pSelectedStaticObject = nullptr;
	_int m_iSelectedStaticObjectIndex = -1;

	CEdit m_editPosX;
	CEdit m_editPosY;
	CEdit m_editPosZ;
	CEdit m_editRotX;
	CEdit m_editRotY;
	CEdit m_editRotZ;
	CEdit m_editScaleX;
	CEdit m_editScaleY;
	CEdit m_editScaleZ;

	CButton m_btnAdd;
	CButton m_btnDelete;
	CButton m_rbtnPosition;
	CButton m_rbtnRotation;
	CButton m_rbtnScale;
	
	
	
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	CString m_cstrPosX;
	CString m_cstrPosY;
	CString m_cstrPosZ;
	CString m_cstrRotX;
	CString m_cstrRotY;
	CString m_cstrRotZ;
	CString m_cstrScaleX;
	CString m_cstrScaleY;
	CString m_cstrScaleZ;
	CString m_cstrFactor0;
	CString m_cstrFactor1;
	CString m_cstrFactor2;
	CString m_cstrFactor3;
	CString m_cstrFactor4;
	CString m_cstrFactor5;
	afx_msg void OnEnChangeEditFactor0();
	afx_msg void OnEnChangeEditFactor1();
	afx_msg void OnEnChangeEditFactor2();
	afx_msg void OnEnChangeEditFactor3();
	afx_msg void OnEnChangeEditFactor4();
	afx_msg void OnEnChangeEditFactor5();
};
