#pragma once
#include "afxcmn.h"
#include "DialogSetupTeaching.h"
#include "DialogSetupSystem.h"

#define SETUP_TEACHING	0

// CDialogSetup ��ȭ �����Դϴ�.

class CDialogSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSetup)

public:
	CDialogSetup(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSetup();

	CDialogSetupTeaching *m_dlgSetupTeaching;
	CDialogSetupSystem *m_dlgSetupSystem;
	void Update(BOOL isUpdate);
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SETUP };

protected:
	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTabSetup(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_ctrSetupMenu;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlApply();
	void ClickBtnenhctrlSave();
	void ClickBtnenhctrlCancle();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
