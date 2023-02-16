#pragma once
#include "afxcmn.h"
#include "DialogSetupTeaching.h"
#include "DialogSetupSystem.h"

#define SETUP_TEACHING	0

// CDialogSetup 대화 상자입니다.

class CDialogSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSetup)

public:
	CDialogSetup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSetup();

	CDialogSetupTeaching *m_dlgSetupTeaching;
	CDialogSetupSystem *m_dlgSetupSystem;
	void Update(BOOL isUpdate);
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP };

protected:
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
