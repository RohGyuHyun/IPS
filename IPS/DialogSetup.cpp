// DialogSetup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "IPS.h"
#include "IPSDlg.h"
#include "DialogSetup.h"
#include "afxdialogex.h"


// CDialogSetup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSetup, CDialogEx)

CDialogSetup::CDialogSetup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogSetup::IDD, pParent)
{

}

CDialogSetup::~CDialogSetup()
{
	delete m_dlgSetupTeaching;
	delete m_dlgSetupSystem;
}

void CDialogSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SETUP, m_ctrSetupMenu);
}


BEGIN_MESSAGE_MAP(CDialogSetup, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETUP, &CDialogSetup::OnTcnSelchangeTabSetup)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogSetup 메시지 처리기입니다.
BOOL CDialogSetup::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	MoveWindow(SETUP_DLG_LEFT, SETUP_DLG_TOP, SETUP_DLG_RIGHT, SETUP_DLG_BOTTOM, FALSE);

	TC_ITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_TEXT;
	TabCtrlItem.pszText = _T("Teahing");
	m_ctrSetupMenu.InsertItem(0, &TabCtrlItem);
	TabCtrlItem.mask = TCIF_TEXT;
	TabCtrlItem.pszText = _T("System");
	m_ctrSetupMenu.InsertItem(1, &TabCtrlItem);

	m_dlgSetupTeaching = new CDialogSetupTeaching;
	m_dlgSetupTeaching->Create(IDD_DIALOG_SETUP_TEACHING, this);

	m_dlgSetupSystem = new CDialogSetupSystem;
	m_dlgSetupSystem->Create(IDD_DIALOG_SETUP_SYSTEM, this);

	CRect rect;
	m_ctrSetupMenu.GetWindowRect(rect);
	m_dlgSetupTeaching->MoveWindow(rect.left, rect.top + 20, rect.Width() - 2, rect.Height() - 20);
	m_dlgSetupSystem->MoveWindow(rect.left, rect.top + 20, rect.Width() - 2, rect.Height() - 20);

	return TRUE;
}

void CDialogSetup::OnTcnSelchangeTabSetup(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int select = m_ctrSetupMenu.GetCurSel();

	if((NULL != m_dlgSetupTeaching) && (NULL != m_dlgSetupSystem))
	{
		switch(select)
		{
			//Update(TRUE);
			case SETUP_TEACHING:
				m_dlgSetupTeaching->ShowWindow(SW_SHOW);
				m_dlgSetupSystem->ShowWindow(SW_HIDE);
				break;
			case 1:
				m_dlgSetupSystem->ShowWindow(SW_SHOW);
				m_dlgSetupTeaching->ShowWindow(SW_HIDE);
				break;
		}
	}
	*pResult = 0;
}
BEGIN_EVENTSINK_MAP(CDialogSetup, CDialogEx)
	ON_EVENT(CDialogSetup, IDC_BTNENHCTRL_APPLY, DISPID_CLICK, CDialogSetup::ClickBtnenhctrlApply, VTS_NONE)
	ON_EVENT(CDialogSetup, IDC_BTNENHCTRL_SAVE, DISPID_CLICK, CDialogSetup::ClickBtnenhctrlSave, VTS_NONE)
	ON_EVENT(CDialogSetup, IDC_BTNENHCTRL_CANCLE, DISPID_CLICK, CDialogSetup::ClickBtnenhctrlCancle, VTS_NONE)
END_EVENTSINK_MAP()

void hMirr(Hobject *src, Hobject *rslt)
{
	BYTE *by_source, *rslt_source;;
	Hlong hw,hh;
	char typ[128];
	int iw,ih;
	get_image_pointer1(*src, (Hlong*)&by_source, typ, &hw, &hh);
	iw = hw - 1;
	rslt_source = new BYTE[hw * hh];
	memset(rslt_source, NULL, sizeof(BYTE)*hw*hh);

	for(int h = 0; h < hh; h++)
	{
		iw = hw - 1;
		for(int w = 0; w < hw; w++)
		{
			rslt_source[(hw * h) + iw] = by_source[(hw * h) + w];
		}
	}

	gen_image1_extern(rslt, "byte", hw, hh, (Hlong)rslt_source, (Hlong)0);
	delete rslt_source;
}

#define pi 3.141592653589793238462643383279
void hRotate(Hobject *src, Hobject *rslt, double degree)
{
	BYTE *by_source, *rslt_source;;
	Hlong hw,hh;
	char typ[128];
	int iw,ih;
	get_image_pointer1(*src, (Hlong*)&by_source, typ, &hw, &hh);
	iw = hw - 1;
	rslt_source = new BYTE[hw * hh];
	memset(rslt_source, NULL, sizeof(BYTE)*hw*hh);
	double radian = degree*pi/180.0; // (1)
    double cc = cos(radian), ss = sin(-radian);
    double xcenter = (double)hw/2.0, ycenter = (double)hh/2.0;
	int orig_x, orig_y;

	for(int h = 0; h < hh; h++)
	{
		for(int w = 0; w < hw; w++)
		{
			orig_x = (int)(xcenter + ((double)h - ycenter)*ss + ((double)w - xcenter)*cc);
            orig_y = (int)(ycenter + ((double)h - ycenter)*cc - ((double)w - xcenter)*ss);
			int pixel = 0;

            if ((orig_y >= 0 && orig_y < hh) && (orig_x >= 0 && orig_x < hw))
                pixel = by_source[(hw * h) + w];

            rslt_source[(hw * h) + w] = pixel; // (6
		}
	}

	gen_image1_extern(rslt, "byte", hw, hh, (Hlong)rslt_source, (Hlong)0);
	delete rslt_source;
}

void CDialogSetup::ClickBtnenhctrlApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	/*CIPSDlg * pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	Update(TRUE);
	pdlg->SaveInspPara(pdlg->GetCamIdx(), pdlg->GetRotate(), pdlg->GetScanIdx());*/

	CIPSDlg * pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	pdlg->GetImage();
	Hobject hRsltImage, hRsltImage2, hRsltImage3, hRsltImage4;
	long sTime, eTime, tTime1, tTime2, tTime3, tTime4;

	sTime = GetCurrentTime();
	mirror_image(pdlg->GetImage(), &hRsltImage, "row");
	eTime = GetCurrentTime();
	tTime1 = eTime - sTime;

	sTime = GetCurrentTime();
	hMirr(&pdlg->GetImage(), &hRsltImage2);
	eTime = GetCurrentTime();
	tTime2 = eTime - sTime;


	sTime = GetCurrentTime();
	rotate_image(pdlg->GetImage(), &hRsltImage3, 180, "constant");
	eTime = GetCurrentTime();
	tTime3 = eTime - sTime;

	sTime = GetCurrentTime();
	hRotate(&pdlg->GetImage(), &hRsltImage4, 90.);
	eTime = GetCurrentTime();
	tTime4 = eTime - sTime;

	eTime = GetCurrentTime();
}


void CDialogSetup::ClickBtnenhctrlSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CIPSDlg * pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	pdlg->BufImageToHalImage(pdlg->GetCamIdx(), pdlg->GetRotate(), pdlg->GetScanIdx());
	Update(TRUE);
	
	pdlg->SaveInspPara(pdlg->GetCamIdx(), pdlg->GetRotate(), pdlg->GetScanIdx());
	pdlg->SaveSetupPara();
	pdlg->SaveSystemPara();
}


void CDialogSetup::ClickBtnenhctrlCancle()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
}

void CDialogSetup::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow) 
	{
		UpdateData(TRUE);
		int select = m_ctrSetupMenu.GetCurSel();
		Update(FALSE);

		if((NULL != m_dlgSetupTeaching) && (NULL != m_dlgSetupSystem))
		{
			switch(select)
			{
				case SETUP_TEACHING:
					m_dlgSetupTeaching->ShowWindow(SW_SHOW);
					m_dlgSetupSystem->ShowWindow(SW_HIDE);
					break;
				case 1:
					m_dlgSetupSystem->ShowWindow(SW_SHOW);
					m_dlgSetupTeaching->ShowWindow(SW_HIDE);
					break;
			}
		}
	}
	else
	{
		m_dlgSetupTeaching->ShowWindow(SW_HIDE);
		m_dlgSetupSystem->ShowWindow(SW_HIDE);
	}
}


void CDialogSetup::Update(BOOL isUpdate)
{
	m_dlgSetupTeaching->Update(isUpdate);
	m_dlgSetupSystem->Update(isUpdate);
}