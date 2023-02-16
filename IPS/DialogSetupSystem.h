#pragma once


// CDialogSetupSystem 대화 상자입니다.

class CDialogSetupSystem : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSetupSystem)

public:
	CDialogSetupSystem(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSetupSystem();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_SYSTEM };

public:
	int GetPortNum();
	void Update(BOOL isUpdate);

protected:
	int m_edit_nPortNum;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	int m_edit_nDelete_day;
	int m_edit_nDelete_Hour;
	int m_edit_nDelete_Minute;
};
