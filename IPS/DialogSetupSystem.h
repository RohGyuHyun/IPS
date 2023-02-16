#pragma once


// CDialogSetupSystem ��ȭ �����Դϴ�.

class CDialogSetupSystem : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSetupSystem)

public:
	CDialogSetupSystem(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSetupSystem();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SETUP_SYSTEM };

public:
	int GetPortNum();
	void Update(BOOL isUpdate);

protected:
	int m_edit_nPortNum;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.


	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	int m_edit_nDelete_day;
	int m_edit_nDelete_Hour;
	int m_edit_nDelete_Minute;
};
