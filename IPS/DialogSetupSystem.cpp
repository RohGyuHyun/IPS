// DialogSetupSystem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "IPS.h"
#include "IPSDlg.h"
#include "DialogSetupSystem.h"
#include "afxdialogex.h"


// CDialogSetupSystem 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSetupSystem, CDialogEx)

CDialogSetupSystem::CDialogSetupSystem(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogSetupSystem::IDD, pParent)
	, m_edit_nPortNum(0)
	, m_edit_nDelete_day(0)
	, m_edit_nDelete_Hour(0)
	, m_edit_nDelete_Minute(0)
{

}

CDialogSetupSystem::~CDialogSetupSystem()
{
}

void CDialogSetupSystem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT_NUM, m_edit_nPortNum);
	DDX_Text(pDX, IDC_EDIT_DELETE_DAY, m_edit_nDelete_day);
	DDX_Text(pDX, IDC_EDIT_DELETE_HOUR, m_edit_nDelete_Hour);
	DDX_Text(pDX, IDC_EDIT_DELETE_MINUTE, m_edit_nDelete_Minute);
}


BEGIN_MESSAGE_MAP(CDialogSetupSystem, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogSetupSystem 메시지 처리기입니다.
int CDialogSetupSystem::GetPortNum(){return m_edit_nPortNum;}
	
void CDialogSetupSystem::Update(BOOL isUpdate)
{
	CIPSDlg *pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	typeSystemPara systemPara;

	systemPara = pdlg->GetSystemPara();
	if(isUpdate)
	{
		UpdateData(TRUE);
		
		systemPara.nPortNum = m_edit_nPortNum;
		systemPara.nDay = m_edit_nDelete_day;
		systemPara.nHour = m_edit_nDelete_Hour;
		systemPara.nMinute = m_edit_nDelete_Minute;

		pdlg->SetSystemPara(systemPara);
	}
	else
	{
		m_edit_nPortNum = systemPara.nPortNum;
		m_edit_nDelete_day = systemPara.nDay;
		m_edit_nDelete_Hour = systemPara.nHour;
		m_edit_nDelete_Minute = systemPara.nMinute;
		UpdateData(FALSE);
	}
}

void CDialogSetupSystem::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
