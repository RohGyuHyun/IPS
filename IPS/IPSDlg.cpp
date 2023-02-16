
// IPSDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "IPS.h"
#include "Include/_CtrlImageRect.h"
#include "include/_FuncModuleHal.h"
#include "include/_FuncModuleBasic.h"
#include "IPSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL FileDelete(CString FilePath, CString FileName)
{
	CFileFind ff;
	CString strTarget = FilePath;
	CStringArray strFileArray;
	BOOL rslt = FALSE;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}
	
	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				continue;
			}
			if(ff.GetFileName() == FileName)
			{
				DeleteFile(Filename);
				break;
			}
		}
	}
	ff.Close();
	
	return rslt;
}


BOOL FileSearch(CString FilePath, CString FileName)
{
	CFileFind ff;
	CString strTarget = FilePath;
	CStringArray strFileArray;
	BOOL rslt = FALSE;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}
	
	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				continue;
			}
			if(ff.GetFileName() == FileName)
			{
				rslt = TRUE;
				break;
			}
		}
	}
	ff.Close();
	
	return rslt;
}


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIPSDlg 대화 상자
CIPSDlg::CIPSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIPSDlg::IDD, pParent)
	//, m_nCamIdx(0)
	, m_nImageIdx(0)
	, m_nCamIdx(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for(int i = 0; i < (MAX_CAM + PCL_IMAGE); i++)
	{
		m_dlgImgRect[i] = NULL;
		m_dlgImgRect[i] = new C_CtrlImageRect;
		m_nDerection[i] = 0;
		m_nSelectScanIdx[i] = 0;
		m_nRotate[i] = 0;
	}
	for(int j = 0; j < MAX_SCAN_CNT; j++)
	{
		m_GrabPara[j].nCamIdx = 0;
		for(int i = 0; i < MAX_CAM + PCL_IMAGE; i++)
		{
			m_GrabPara[j].nScanIdx[i] = 0;
			m_GrabPara[j].nDerection[i] = 0;
			m_GrabPara[j].nHeight[i] = 0;
			m_GrabPara[j].nRotat[i] = 0;
			m_GrabPara[j].strResultDataPath.Empty();
			m_GrabPara[j].strResultImagePath.Empty();
			m_GrabPara[j].strResultLotNoPath.Empty();
		}
		m_bInspection[j] = FALSE;
		m_bGrabReady[j] = FALSE;
		
	}

	m_InsPara = new typeInspPara[MAX_SCAN_CNT * (MAX_CAM ) * MAX_ROTATE];

	for(int i = 0; i < (MAX_SCAN_CNT * (MAX_CAM ) * MAX_ROTATE); i++)
	{
		memset(&m_InsPara[i], NULL, sizeof(typeInspPara));
	}
	//memset(m_InsPara, NULL, sizeof(typeInspPara) * MAX_SCAN_CNT * (MAX_CAM + PCL_IMAGE) * MAX_ROTATE);
	memset(&m_SetupPara, NULL, sizeof(typeSetupPara));

	m_bAutoStart = FALSE;
	m_bAutoEnd = FALSE;
	m_VisMod = NULL;
	
	m_bInspectionEnd = FALSE;

	mRootDirectory.Format(_T("D:\\DATABASE\\"));
	m_ModelName.Format(_T("test"));
	m_bAutoGrabInspectEnd = FALSE;
	m_nInspection_idx = 0;
	m_nGrabIdx = 0;
	m_bEMIControlConnection = FALSE;
	m_bServerEnd = FALSE;

	for(int i = 0; i <  MAX_SCAN_CNT; i++)
	{
		for(int j = 0; j < MAX_ROTATE; j++)
		{
			m_CVDRslt[j][i] = new typeCVDRslt[MAX_DEFECT_CNT];
			memset(m_CVDRslt[j][i], NULL, sizeof(typeCVDRslt) * MAX_DEFECT_CNT);

			m_PCLRslt[j][i] = new typeCVDRslt[MAX_DEFECT_CNT];
			memset(m_PCLRslt[j][i], NULL, sizeof(typeCVDRslt) * MAX_DEFECT_CNT);

			memset(m_nUpRsltNum[j], NULL, sizeof(int));
			memset(m_nDwRsltNum[j], NULL, sizeof(int));

			memset(m_nUpPCLRsltNum[j], NULL, sizeof(int));
			memset(m_nDwPCLRsltNum[j], NULL, sizeof(int));
		}
	}
}

CIPSDlg::~CIPSDlg()
{

}

void CIPSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAIN, m_staticMain);
	//DDX_Radio(pDX, IDC_RADIO_CAM1, m_nCamIdx);
	DDX_Control(pDX, IDC_BTNENHCTRL_DERECTION, m_btnDerection);
	DDX_Radio(pDX, IDC_RADIO_IMAGE1, m_nImageIdx);
	DDX_Control(pDX, IDC_BTNENHCTRL_AUTO, m_btnAutoStart);
	DDX_Control(pDX, IDC_BTNENHCTRL_ROTATE, m_btnRotate);
	DDX_Control(pDX, IDC_BTNENHCTRL_STATE_EMI, m_btnStateEMI);
	DDX_Radio(pDX, IDC_RADIO_CAM_1, m_nCamIdx);
}

BEGIN_MESSAGE_MAP(CIPSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(USER_MSG_MAIN, OnMainMessage)
	ON_MESSAGE(USER_MSG_IMG_RECT, OnImageRectMessage)
	ON_BN_CLICKED(IDC_RADIO_CAM_1, &CIPSDlg::OnBnClickedRadioCam1)
	ON_BN_CLICKED(IDC_RADIO_CAM_2, &CIPSDlg::OnBnClickedRadioCam2)
	ON_BN_CLICKED(IDC_RADIO_CAM_3, &CIPSDlg::OnBnClickedRadioCam3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_IMAGE1, &CIPSDlg::OnBnClickedRadioImage1)
	ON_BN_CLICKED(IDC_RADIO_IMAGE2, &CIPSDlg::OnBnClickedRadioImage2)
	ON_BN_CLICKED(IDC_RADIO_IMAGE3, &CIPSDlg::OnBnClickedRadioImage3)
	ON_BN_CLICKED(IDC_RADIO_IMAGE4, &CIPSDlg::OnBnClickedRadioImage4)
	ON_BN_CLICKED(IDC_RADIO_IMAGE5, &CIPSDlg::OnBnClickedRadioImage5)
	ON_BN_CLICKED(IDC_RADIO_IMAGE6, &CIPSDlg::OnBnClickedRadioImage6)
	ON_MESSAGE(WM_ACCEPT_SOCKET,OnAccept)
	ON_MESSAGE(WM_RECEIVE_DATA,OnReceive)
	ON_MESSAGE(WM_CLOSE_SOCK,OnClose)
	ON_BN_CLICKED(IDC_RADIO_IMAGE7, &CIPSDlg::OnBnClickedRadioImage7)
	ON_BN_CLICKED(IDC_RADIO_IMAGE8, &CIPSDlg::OnBnClickedRadioImage8)
	ON_BN_CLICKED(IDC_RADIO_IMAGE9, &CIPSDlg::OnBnClickedRadioImage9)
	ON_BN_CLICKED(IDC_RADIO_IMAGE10, &CIPSDlg::OnBnClickedRadioImage10)
	
END_MESSAGE_MAP()

// CIPSDlg 메시지 처리기
BOOL CIPSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CString strTemp;
#ifdef CAMERA
	m_VisMod = new CVisionModule();
	for(int i = 0; i < MAX_CAM; i++)
	{
		m_VisMod->Open(i);
	}
#endif
	WriteLog(LOG_PATH, _T("Vision Cam Init"));
	m_dlgSetup = new CDialogSetup;
	m_dlgSetup->Create(IDD_DIALOG_SETUP, this);
	WriteLog(LOG_PATH, _T("Setup Dialog Init"));
	//Inspection_8K(m_GrabPara);
	
#ifdef PCL_TEST
	ImageRectInit(MAX_CAM + PCL_IMAGE);
#else
	ImageRectInit(MAX_CAM);
#endif

#ifdef CAMERA
	SelectView(m_nCamIdx);
#else
	m_dlgImgRect[0]->ShowWindow(SW_SHOW);
#endif
	SendMessage(USER_MSG_IMG_RECT, m_nCamIdx, IMAGE_DISPLAY);

	LoadSetupPara();
	LoadSystemPara();
	
#ifdef PCL_TEST
	for(int j = 0; j < (MAX_CAM + PCL_IMAGE); j++)
	{
#else
	for(int j = 0; j < MAX_CAM; j++)
	{
#endif
		for(int i = 0; i < MAX_SCAN_CNT; i++)
		{
			for(int k = 0; k < MAX_ROTATE; k++)
			{
				LoadInspPara(j, k, i);
			}	
		}
	}
	m_dlgSetup->Update(FALSE);

	m_Server = new CSeverSock;
	ServerOpen();

	m_bAutoDeleteStart = TRUE;
	m_bAutoDeleteEnd = FALSE;

	m_pAutoDeleteThread = AfxBeginThread(AutoDeleteThread, this, THREAD_PRIORITY_NORMAL);
	if(m_pAutoDeleteThread)
		m_pAutoDeleteThread->m_bAutoDelete = TRUE;


#ifndef PCL_TEST
	GetDlgItem(IDC_RADIO_CAM_3)->ShowWindow(SW_HIDE);
#endif

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BOOL CIPSDlg::ImageRectInit(int cnt)
{
	CRect rect, title;
	CString strTemp;
	Herror herr;	
	int i, j, k;
	HTuple wd, ht;
	long width, height;
	GetDlgItem(IDC_STATIC_MAIN)->GetClientRect(rect);
	GetDlgItem(IDC_STATIC_MAIN)->ClientToScreen(rect);
	
	for(i = 0; i < cnt; i++)
	{
		if(i == 0)
		{
			gen_image_const(&m_HalBufImage[i], HTuple("byte"), MAX_HALCON_IMAGE_HEIGHT_8K, MAX_HALCON_IMAGE_WIDTH_8K);
		}
		else if(i >= 1)
		{
			gen_image_const(&m_HalBufImage[i], HTuple("byte"), MAX_HALCON_IMAGE_HEIGHT_16K, MAX_HALCON_IMAGE_WIDTH_16K);
		}
		for(j = 0; j < MAX_ROTATE; j++)
		{
			for(k = 0; k < MAX_SCAN_CNT; k++)
			{
				if(i == 0)
				{
					gen_image_const(&m_HalImage[i][j][k], HTuple("byte"), MAX_HALCON_IMAGE_HEIGHT_8K, MAX_HALCON_IMAGE_WIDTH_8K);
				}
				else if(i >= 1)
				{
					gen_image_const(&m_HalImage[i][j][k], HTuple("byte"), MAX_HALCON_IMAGE_HEIGHT_16K, MAX_HALCON_IMAGE_WIDTH_16K);
				}
				//Halcon image initialize
				set_check("~give_error");
					herr = get_image_pointer1(m_HalImage[i][j][k], NULL, NULL, &wd, &ht);
				set_check("give_error");

				if(H_MSG_TRUE == herr)
				{
				}
				else
				{
					AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1_00!"));
					return FALSE;
				}
				strTemp.Format(_T("Cam %d, Rotate %d Image %d Init"), i, j, k);
				WriteLog(LOG_PATH, strTemp);
				InitInspectionPara(i, k, j);
				strTemp.Format(_T("Cam %d, Rotate %d Image %d Parameter Init"), i, j, k);
				WriteLog(LOG_PATH, strTemp);
			}
		}		

		width = long((Hlong)wd[0]);
		height = long((Hlong)ht[0]);
		if(i == CAM_8K)
		{
			strTemp.Format(_T("Cam 1번 8K Rotate 0 1번 Image"));
		}
		else if(i == CAM_16K)
		{
			strTemp.Format(_T("Cam 2번 16K Rotate 0 1번 CVD Image"));
		}
		else if(i == CAM_16K + PCL_IMAGE)
		{
			strTemp.Format(_T("Cam 2번 16K Rotate 0 1번 PCL Image"));
		}

		m_dlgImgRect[i]->Create(IMAGE_RECT_DIALOG, this);
		m_dlgImgRect[i]->MoveWindow(rect.left + 3, rect.top + 3, rect.Width(), rect.Height(), FALSE);
		m_dlgImgRect[i]->m_Title.GetClientRect(title);
		m_dlgImgRect[i]->m_Title.MoveWindow(rect.left - 3, rect.top - 25, rect.Width(), title.Height());
		m_dlgImgRect[i]->m_Title.SetTextCaptionAlignment(1);
		m_dlgImgRect[i]->SetCamIndex(i, strTemp, this->m_hWnd);
		m_dlgImgRect[i]->ShowWindow(SW_SHOW);
		
		//GetDlgItem(IDC_STATIC_MAIN)->GetWindowRect(rect);
		//zoom = (double)(rect.Width() / (int)width);
		////height = (long)((double)height / zoom);
		////m_dlgImgRect[i]->m_
		//if(i == 0)
		//{
		//	m_dlgImgRect[i]->SetZoom(zoom, (long)MAX_HALCON_IMAGE_HEIGHT_8K, (long)MAX_HALCON_IMAGE_WIDTH_8K / zoom);
		//}
		//else
		//{
		//	m_dlgImgRect[i]->SetZoom(zoom, (long)MAX_HALCON_IMAGE_HEIGHT_16K, (long)MAX_HALCON_IMAGE_WIDTH_16K / zoom);
		//}

		m_dlgImgRect[i]->SetZoom(0, (long)width, (long)height * 1.6);
			
		set_font(m_dlgImgRect[i]->GetView(), "-Arial-12-*-*-*-*-*-1-");
		set_draw(m_dlgImgRect[i]->GetView(), HTuple("margin"));
	
		m_dlgImgRect[i]->ShowWindow(SW_HIDE);
		if(cnt < 3)
		{
			GetDlgItem(IDC_RADIO_CAM2)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CAM3)->EnableWindow(FALSE);
		}
		strTemp.Format(_T("View %d Init"), i);
		WriteLog(LOG_PATH, strTemp);
	}

	return TRUE;
}

void CIPSDlg::SetServerStatus(BOOL isOnOff)
{
	if(isOnOff)
	{
		m_btnStateEMI.SetBackColor(RGB(0, 255, 0));
	}
	else
	{
		m_btnStateEMI.SetBackColor(RGB(255, 0, 0));

	}}

void CIPSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.
void CIPSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CIPSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CIPSDlg::OnClose(WPARAM wParam, LPARAM lParam)
{
	//접속 중인 클라이언트가 해제 되면 메시지 출력
	//기존 서버 소켓을 닫는다.
	m_Server->Close();
	//delete m_Client;

	m_btnStateEMI.SetBackColor(RGB(255, 0, 0));
	m_bEMIControlConnection = FALSE;

	CString strTemp;
	strTemp.Format(_T("Server Close"));
	WriteLog(LOG_PATH, strTemp);
	//AfxMessageBox(_T("ServerClose"));

	ServerOpen();

	return TRUE;
}

//새로운 클라이언트가 접속했을때
LRESULT CIPSDlg::OnAccept(WPARAM wParam, LPARAM lParam)
{
	//새로운 클라이언트와 연결할 소켓을 만들고

	m_Client = new CClientSock;

	//서버 소켓으로부터 현재 연결된 소켓을
	//받는다.
	m_Client = m_Server->GetAcceptSocket();
	
	//hwnd설정
	m_Client->SetWnd(this->m_hWnd);

	m_btnStateEMI.SetBackColor(RGB(0, 255, 0));
	m_bEMIControlConnection = TRUE;
	CString strTemp;
	strTemp.Format(_T("TCP/IP client connected!"));
	WriteLog(LOG_PATH, strTemp);
	//AfxMessageBox(str);

	return TRUE;
}

LRESULT CIPSDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	CString rcv_dat, strCmd;
	rcv_dat.Empty();
	strCmd.Empty();
	CString strLotNo, strCamIdx, strDerection, strGrabHeight, strScanIdx, strRotate, strInspType, strParaCnt1, strParaCnt2, strPara1, strPara2,
			strInsCnt, strRectLeft, strRectRight, strRectTop, strRectBottom, strNoiseSize, strMargeDistance; 
	char temp[PACKET_MAX_NUM];
	memset(temp, NULL, PACKET_MAX_NUM);
	typeInspPara insPara;
	memset(&insPara, NULL, sizeof(typeInspPara));
	int subIdx = 0, noise_cnt = 0;;
	CTime cTime;
	typeGrabPara grabPara;
	CString strTemp;
	CString LogPath;
	CString packet;
	LogPath.Format(_T("D:\\DATABASE\\"));
	//소켓으로 부터 데이타를 받은다음
	m_Client->Receive((LPSTR)temp, PACKET_MAX_NUM);

	cTime = CTime::GetCurrentTime();
		
	rcv_dat.Format(_T("%S"), temp);
	rcv_dat.Delete(0, 1);
	rcv_dat.Delete(rcv_dat.GetLength() - 1, 1);
	if(rcv_dat.GetLength() > 30)
	{
		strTemp.Format(_T("Receive Packet -> %s...."), rcv_dat.Mid(0, 30));
	}
	else
	{
		strTemp.Format(_T("Receive Packet -> %s"), rcv_dat);
	}
	WriteLog(LOG_PATH, strTemp);

	AfxExtractSubString(strCmd, rcv_dat, subIdx++, ',');

	if(strCmd == _T("6000"))
	{
		if(GetAutoStartState())
		{
			packet.Format(_T("%c%s%c"),PACKET_CHAR_STX, PACKET_SEND_AUTORUN, PACKET_CHAR_ETX);
			TCPIPSendMCS(packet);
		}
		else
		{
			packet.Format(_T("%c%s%c"),PACKET_CHAR_STX, PACKET_SEND_MANUAL, PACKET_CHAR_ETX);
			TCPIPSendMCS(packet);
		}
	}

	//20160808 ngh
	if(strCmd == PACKET_RECEIVE_AUTO_START)
	{ 
		if(!GetAutoStartState())
		{
			m_btnAutoStart.SetValue(1);
			ClickBtnenhctrlAuto();
		}
		Sleep(1000);
		packet.Format(_T("%c%s%c"),PACKET_CHAR_STX, PACKET_SEND_AUTORUN, PACKET_CHAR_ETX);
		TCPIPSendMCS(packet);
	}
		
	if(m_btnAutoStart.GetValue())
	{
		if(strCmd == PACKET_RECEIVE_GRAB_READY)
		{ 
			
			AfxExtractSubString(grabPara.strLotNo, rcv_dat, subIdx++,',');
			AfxExtractSubString(grabPara.strGlassID, rcv_dat, subIdx++,',');
			AfxExtractSubString(strCamIdx, rcv_dat, subIdx++, ',');

			grabPara.nCamIdx = _wtoi(strCamIdx);
			if(grabPara.nCamIdx == CAM_8K)
			{
				subIdx++;
				grabPara.nInspectionType = 0;
			}
			else
			{
#ifdef PCL_TEST
				AfxExtractSubString(strInspType, rcv_dat, subIdx++, ',');
				grabPara.nInspectionType = _wtoi(strInspType);
				grabPara.nCamIdx += grabPara.nInspectionType;
#else
				grabPara.nInspectionType = 0;
				subIdx++;
#endif
			}

			AfxExtractSubString(strRotate, rcv_dat, subIdx++, ',');
#ifdef CAMERA
			AfxExtractSubString(strScanIdx, rcv_dat, subIdx++, ',');
#else
			strScanIdx.Format(_T("0"));
			subIdx++;
#endif

			AfxExtractSubString(strDerection, rcv_dat, subIdx++, ',');
			AfxExtractSubString(strGrabHeight, rcv_dat, subIdx++, ',');
			
			
			grabPara.nRotat[grabPara.nCamIdx] = _wtoi(strRotate);
			grabPara.nScanIdx[grabPara.nCamIdx] = _wtoi(strScanIdx);
			grabPara.nDerection[grabPara.nCamIdx] = _wtoi(strDerection);
			grabPara.nHeight[grabPara.nCamIdx] = _wtoi(strGrabHeight);
			m_nCamIdx = grabPara.nCamIdx;
			m_nRotate[m_nCamIdx] = grabPara.nRotat[grabPara.nCamIdx];
			m_nSelectScanIdx[m_nCamIdx] = grabPara.nScanIdx[grabPara.nCamIdx];
			m_nDerection[m_nCamIdx] = grabPara.nDerection[grabPara.nCamIdx];
			m_nGrabIdx = grabPara.nScanIdx[grabPara.nCamIdx];

			

			strTemp.Format(_T("Cam %d, Rotate %d, ScanIdx %d, Derection %d, GrabHeight %d GrabReady Packet Receive"), 
				grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], grabPara.nDerection[grabPara.nCamIdx], grabPara.nHeight[grabPara.nCamIdx]);
			WriteLog(LOG_PATH, strTemp);

#ifdef CAMERA
			m_VisMod->SetGrabStop(grabPara.nCamIdx, FALSE);
#endif
			if(grabPara.nCamIdx == CAM_8K)
			{
				AfxExtractSubString(strParaCnt1, rcv_dat, subIdx++, ',');
				insPara.nParaCnt[0] = _wtoi(strParaCnt1);
				for(int i = 0; i < insPara.nParaCnt[0]; i++)
				{
					AfxExtractSubString(strPara1, rcv_dat, subIdx++, ',');
					insPara.dThreshHold[i] = _wtoi(strPara1);
				}

				AfxExtractSubString(strParaCnt2, rcv_dat, subIdx++, ',');
				insPara.nParaCnt[1] = _wtoi(strParaCnt2);
				for(int i = 0; i < insPara.nParaCnt[1]; i++)
				{
					AfxExtractSubString(strPara2, rcv_dat, subIdx++, ',');
					if(i < 2)
					{
						insPara.ImageFilterPara[i].dFactor = _wtof(strPara2);
					}
				}

				AfxExtractSubString(strNoiseSize, rcv_dat, subIdx++, ',');
				noise_cnt = _wtoi(strNoiseSize);
				for(int i = 0; i < noise_cnt; i++)
				{
					AfxExtractSubString(strNoiseSize, rcv_dat, subIdx++, ',');
					insPara.nNoiseSize[i] = _wtoi(strNoiseSize);
				}

				AfxExtractSubString(strMargeDistance, rcv_dat, subIdx++, ',');
				AfxExtractSubString(strMargeDistance, rcv_dat, subIdx++, ',');
				insPara.dMargeDistance[0] = _wtof(strMargeDistance);

				AfxExtractSubString(strInsCnt, rcv_dat, subIdx++, ',');
				insPara.nInspectionCnt = _wtoi(strInsCnt);

				for(int i = 0; i < insPara.nInspectionCnt; i++)
				{
					AfxExtractSubString(strRectLeft, rcv_dat, subIdx++, ',');
					insPara.InsRect[i].dRectLeft = _wtoi(strRectLeft);
					AfxExtractSubString(strRectTop, rcv_dat, subIdx++, ',');
					insPara.InsRect[i].dRectTop = _wtoi(strRectTop);
					AfxExtractSubString(strRectRight, rcv_dat, subIdx++, ',');
					insPara.InsRect[i].dRectRight = _wtoi(strRectRight);
					AfxExtractSubString(strRectBottom, rcv_dat, subIdx++, ',');
					insPara.InsRect[i].dRectBottom = _wtoi(strRectBottom);
				}
			}
			else if(grabPara.nCamIdx >= CAM_16K)
			{
				AfxExtractSubString(strParaCnt1, rcv_dat, subIdx++, ',');
				insPara.nParaCnt[0] = _wtoi(strParaCnt1);
				for(int i = 0; i < insPara.nParaCnt[0]; i++)
				{
					AfxExtractSubString(strPara1, rcv_dat, subIdx++, ',');
					if(i == 0)
					{
						insPara.dPixelSize = _wtof(strPara1);
					}
					else
					{
						insPara.dThreshHold[i - 1] = _wtof(strPara1);
					}
				}

				AfxExtractSubString(strParaCnt2, rcv_dat, subIdx++, ',');
				insPara.nParaCnt[1] = _wtoi(strParaCnt2);
				for(int i = 0; i < insPara.nParaCnt[1]; i++)
				{
					AfxExtractSubString(strPara2, rcv_dat, subIdx++, ',');
					insPara.nDefectSize[i] = _wtoi(strPara2);
					if(insPara.nDefectSize[i] <= 2)
					{
						insPara.nDefectSize[i] = 3;
					}
				}

				AfxExtractSubString(strInsCnt, rcv_dat, subIdx++, ',');
				insPara.nInspectionCnt = _wtoi(strInsCnt);

				for(int i = 0; i < insPara.nInspectionCnt; i++)
				{
					AfxExtractSubString(strRectLeft, rcv_dat, subIdx++, ',');
					insPara.InsRect[i].dRectLeft = _wtoi(strRectLeft);
					AfxExtractSubString(strRectTop, rcv_dat, subIdx++, ',');
					insPara.InsRect[i].dRectTop = _wtoi(strRectTop);
					AfxExtractSubString(strRectRight, rcv_dat, subIdx++, ',');
					insPara.InsRect[i].dRectRight = _wtoi(strRectRight);
					AfxExtractSubString(strRectBottom, rcv_dat, subIdx++, ',');
					insPara.InsRect[i].dRectBottom = _wtoi(strRectBottom);
				}

				AfxExtractSubString(strInsCnt, rcv_dat, subIdx++, ',');
				insPara.nInspectionCVDUpCnt = _wtoi(strInsCnt);

				for(int i = 0; i < insPara.nInspectionCVDUpCnt; i++)
				{
					AfxExtractSubString(strRectLeft, rcv_dat, subIdx++, ',');
					insPara.InsCVDUpRect[i].dRectLeft = _wtoi(strRectLeft);
					AfxExtractSubString(strRectTop, rcv_dat, subIdx++, ',');
					insPara.InsCVDUpRect[i].dRectTop = _wtoi(strRectTop);
					AfxExtractSubString(strRectRight, rcv_dat, subIdx++, ',');
					insPara.InsCVDUpRect[i].dRectRight = _wtoi(strRectRight);
					AfxExtractSubString(strRectBottom, rcv_dat, subIdx++, ',');
					insPara.InsCVDUpRect[i].dRectBottom = _wtoi(strRectBottom);
				}

				AfxExtractSubString(strInsCnt, rcv_dat, subIdx++, ',');
				insPara.nInspectionCVDDwCnt = _wtoi(strInsCnt);

				for(int i = 0; i < insPara.nInspectionCVDDwCnt; i++)
				{
					AfxExtractSubString(strRectLeft, rcv_dat, subIdx++, ',');
					insPara.InsCVDDwRect[i].dRectLeft = _wtoi(strRectLeft);
					AfxExtractSubString(strRectTop, rcv_dat, subIdx++, ',');
					insPara.InsCVDDwRect[i].dRectTop = _wtoi(strRectTop);
					AfxExtractSubString(strRectRight, rcv_dat, subIdx++, ',');
					insPara.InsCVDDwRect[i].dRectRight = _wtoi(strRectRight);
					AfxExtractSubString(strRectBottom, rcv_dat, subIdx++, ',');
					insPara.InsCVDDwRect[i].dRectBottom = _wtoi(strRectBottom);
				}
			}

			if(grabPara.nCamIdx == CAM_8K)
			{
				switch(grabPara.nRotat[grabPara.nCamIdx])
				{
				case 0:
					if(grabPara.nDerection[grabPara.nCamIdx] == 1)
					{
						grabPara.nGrabOffset[grabPara.nCamIdx] = m_dlgSetup->m_dlgSetupTeaching->Get8KLongOffset();
					}
					break;
				case 1:
					if(grabPara.nDerection[grabPara.nCamIdx] == 1)
					{
						grabPara.nGrabOffset[grabPara.nCamIdx] = m_dlgSetup->m_dlgSetupTeaching->Get8KShortOffset();
					}
					break;
				}
			}
			else if(grabPara.nCamIdx >= CAM_16K)
			{
				switch(grabPara.nRotat[grabPara.nCamIdx])
				{
				case 0:
					if(grabPara.nDerection[grabPara.nCamIdx] == 1)
					{
						grabPara.nGrabOffset[grabPara.nCamIdx] = m_dlgSetup->m_dlgSetupTeaching->Get16KLongOffset();
					}
					break;
				case 1:
					if(grabPara.nDerection[grabPara.nCamIdx] == 1)
					{
						grabPara.nGrabOffset[grabPara.nCamIdx] = m_dlgSetup->m_dlgSetupTeaching->Get16KShortOffset();
					}
					break;
				}
			}

#ifdef CAMERA
			SelectView(grabPara.nCamIdx);
#endif

			if(grabPara.nCamIdx == CAM_8K)
			{
				if(grabPara.nRotat[grabPara.nCamIdx] == ROTATE_GRAB_0)
				{
					strTemp.Format(_T("Cam 1번 8K Rotate 0 %d번 Image"), grabPara.nScanIdx[grabPara.nCamIdx] + 1);
				}
				else if(grabPara.nRotat[grabPara.nCamIdx] == ROTATE_GRAB_90)
				{
					strTemp.Format(_T("Cam 1번 8K Rotate 90 %d번 Image"), grabPara.nScanIdx[grabPara.nCamIdx] + 1);
				}
			}
			else if(grabPara.nCamIdx >= CAM_16K)
			{
				//20160614 ngh 수정 필요
				if(grabPara.nRotat[grabPara.nCamIdx] == ROTATE_GRAB_0)
				{
					strTemp.Format(_T("Cam %d번 16K Rotate 0 %d번 Image"), grabPara.nCamIdx + 1, grabPara.nScanIdx[grabPara.nCamIdx] + 1);
				}
				else if(grabPara.nRotat[grabPara.nCamIdx] == ROTATE_GRAB_90)
				{
					strTemp.Format(_T("Cam %d번 16K Rotate 90 %d번 Image"), grabPara.nCamIdx + 1, grabPara.nScanIdx[grabPara.nCamIdx] + 1);
				}
			}
			m_dlgImgRect[grabPara.nCamIdx]->m_Title.SetWindowTextW(strTemp);
			
			m_strResultLotNoPath.Format(_T("%s\\%04d%02d%02d\\%s\\%s\\"), DEFAULT_RESULT_PATH, cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), grabPara.strLotNo, grabPara.strGlassID);
			
			if(grabPara.nCamIdx == CAM_8K)
			{
				m_strResultDataPath.Format(_T("%s\\Data\\%d_%d_%d\\"), m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
				m_strResultImagePath.Format(_T("%s\\Image\\%d_%d_%d\\"), m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
			}
			else
			{
#ifdef PCL_TEST
				if(grabPara.nInspectionType == INSPECTION_TYPE_CVD)
				{
					m_strResultDataPath.Format(_T("%s\\Data\\CVD\\%d_%d_%d\\"), m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
					m_strResultImagePath.Format(_T("%s\\Image\\CVD\\%d_%d_%d\\"), m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
				}
				else if(grabPara.nInspectionType = INSPECTION_TYPE_PCL)
				{
					if(grabPara.nRotat[grabPara.nCamIdx] == ROTATE_GRAB_0)
					{
						m_strResultDataPath.Format(_T("%s\\Data\\PCL_L\\%d_%d_%d\\"), m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
						m_strResultImagePath.Format(_T("%s\\Image\\PCL_L\\%d_%d_%d\\"), m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
					}
					else if(grabPara.nRotat[grabPara.nCamIdx] == ROTATE_GRAB_90)
					{
						m_strResultDataPath.Format(_T("%s\\Data\\PCL_S\\%d_%d_%d\\"), m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
						m_strResultImagePath.Format(_T("%s\\Image\\PCL_S\\%d_%d_%d\\"), m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
					}			
				}
#else
				m_strResultDataPath.Format(_T("%s\\Data\\%d_%d_%d\\"), m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
				m_strResultImagePath.Format(_T("%s\\Image\\%d_%d_%d\\"), m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
#endif
			}
			

			grabPara.strResultLotNoPath = m_strResultLotNoPath;
			sprintf_s(temp, m_strResultLotNoPath.GetLength() + 1,  "%S", m_strResultLotNoPath);
			MakeDirectories((LPCWSTR)temp);
			grabPara.strResultDataPath = m_strResultDataPath;
			sprintf_s(temp, m_strResultDataPath.GetLength() + 1,  "%S", m_strResultDataPath);
			MakeDirectories((LPCWSTR)temp);
			grabPara.strResultImagePath = m_strResultImagePath; 
			sprintf_s(temp, m_strResultImagePath.GetLength() + 1,  "%S", m_strResultImagePath);
			MakeDirectories((LPCWSTR)temp);

			if(grabPara.nScanIdx[grabPara.nCamIdx] == 0)
			{
				for(int i = 0; i < MAX_SCAN_CNT; i++)
				{
					SetGrabComp(FALSE, i);
					SetInspection(FALSE, i);
					SetGrabReady(FALSE, i);
				}
				m_nInspection_idx = 0;
			}

			SetInsPara(insPara, grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx], grabPara.nRotat[grabPara.nCamIdx]);

			SaveInspPara(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
			
			SetGrabPara(grabPara, grabPara.nScanIdx[grabPara.nCamIdx]);
#ifdef CAMERA
			SetGrabReady(TRUE, grabPara.nScanIdx[grabPara.nCamIdx]);
#endif
		}
	}
	
	return TRUE;
}

BOOL CIPSDlg::TCPIPSendMCS(CString packet)
{
	CString strTemp;
	int size = 0;
	char temp[PACKET_MAX_NUM];
	memset(temp, NULL, PACKET_MAX_NUM);
	
	sprintf_s(temp, packet.GetLength() + 1, "%S", packet);
	size = (int)strlen(temp);

	if(m_bEMIControlConnection)
	{
		m_Client->Send((LPSTR)temp, size);
		strTemp.Format(_T("Send Packet -> %S"), temp);
		WriteLog(LOG_PATH, strTemp);
	}
	else
	{
		strTemp.Format(_T("MCS Not Connection, Not Send Packet -> %S"), temp);
		WriteLog(LOG_PATH, strTemp);
		return FALSE;
	}

	return TRUE;
}

BOOL CIPSDlg::ServerOpen()
{
	BOOL chk;
	chk = TRUE;
	CString strTemp,packet;
	delete m_Server;
	//서버 소켓을 만든다.
	m_Server = new CSeverSock;
	AfxSocketInit();
	if( !m_Server->Create(GetSystemPara().nPortNum) )
	{
		chk = FALSE;
		strTemp.Format(_T("Error : TCP/IP server create fail!"));
		WriteLog(LOG_PATH, strTemp);
	}
	
	m_Server->SetWnd( this->m_hWnd );
	//클라이언트가 오기를 기다린다.
	if( !m_Server->Listen() )
	{
		chk = FALSE;
		strTemp.Format(_T("Error : TCP/IP server listen fail!"));
		WriteLog(LOG_PATH, strTemp);
	}

	if(chk)
	{
		strTemp.Format(_T("TCP/IP server open OK!"));
		WriteLog(LOG_PATH, strTemp);
		if(m_btnAutoStart.GetValue())
		{
			packet.Format(_T("%c%s%c"),PACKET_CHAR_STX, PACKET_SEND_AUTORUN, PACKET_CHAR_ETX);
			TCPIPSendMCS(packet);
		}
		else
		{
			packet.Format(_T("%c%s%c"),PACKET_CHAR_STX, PACKET_SEND_MANUAL, PACKET_CHAR_ETX);
			TCPIPSendMCS(packet);
		}
	}
	
	//m_Client = new CClientSock;

	return chk;
}

void CIPSDlg::SetGrabPara(typeGrabPara grabPara, int scan_idx)
{
	m_GrabPara[scan_idx] = grabPara;
}

void CIPSDlg::SetTitle(CString tile, int scan_idx)
{
	m_dlgImgRect[m_GrabPara[scan_idx].nCamIdx]->m_Title.SetWindowTextW(tile);
}

void CIPSDlg::SetGrabReady(BOOL ready, int scan_idx)
{
	m_bGrabReady[scan_idx] = ready;
}

int CIPSDlg::HandleAllMessage()
{
	int returnValue;
	MSG Mess;

	do {
		returnValue=::PeekMessage(&Mess, NULL, 0, 0, PM_REMOVE);
		if (returnValue)
		{
			::TranslateMessage(&Mess);
			::DispatchMessage (&Mess);
		}
	} while(returnValue);
	
	return returnValue;
}

LRESULT CIPSDlg::OnMainMessage(WPARAM para0, LPARAM para1)
{
	// TODO: Add your control notification handler code here
	CString status;
	int cam_idx;
	cam_idx = (int)para0;

	//임시
	CString strTemp;

	switch((int)para1)
	{
	case MAIN_GRAB_READY:
		//GrabReady(m_GrabPara);
		break;
	case MAIN_GRAB_COMP:

		break;
	case MAIN_GRAB_TIMEOUT:

		break;
	case MAIN_AUTO_START:
		m_btnAutoStart.SetValue(TRUE);
		strTemp.Format(_T("Auto Mode"));
		WriteLog(LOG_PATH, strTemp);
		m_bAutoGrabInspectEnd = FALSE;
		m_bAutoEnd = FALSE;
		m_bAutoStart = TRUE;
		m_btnAutoStart.SetWindowTextW(_T("AUTO RUN"));
		m_pThreadGrab = AfxBeginThread(GrabThread, this, THREAD_PRIORITY_NORMAL);
		m_pThreadGrab->m_bAutoDelete = TRUE;

		m_bInspectionEnd = FALSE;
		m_pThreadInspection = AfxBeginThread(InspectionThread, this, THREAD_PRIORITY_NORMAL);
		m_pThreadInspection->m_bAutoDelete = TRUE;
		break;
	case MAIN_AUTO_STOP:
		m_btnAutoStart.SetValue(FALSE);
		strTemp.Format(_T("Manual Mode"));
		WriteLog(LOG_PATH, strTemp);
#ifdef CAMERA
		MdigHalt(*m_VisMod->GetMilDigitizer(GetCamIdx()));
#endif
		m_bAutoGrabInspectEnd = TRUE;
		m_bAutoEnd = TRUE;
		m_bAutoStart = FALSE;
		m_bInspectionEnd = TRUE;
		m_btnAutoStart.SetWindowTextW(_T("MANUAL RUN"));
		break;
	}

	return 0;
}

LRESULT CIPSDlg::OnImageRectMessage(WPARAM para0, LPARAM para1)
{
	// TODO: Add your control notification handler code here
	HTuple wd, ht;
	Herror herr;
	CString status;
	int cam_idx;
	typeInspPara insPara;
	typeGrabPara grabPara;
	CRect rect;
	Hobject sub_ins_rgn, ins_rgn, ins_rgn1, ins_rgn2;
	Hobject rslt_rgn, slct_rgn;
	long rslt_num;
	HTuple Num, area, row0, col0, row1, col1, ccol, crow;
	cam_idx = (int)para0;

	//임시
	CFileDialog dlgFile(FALSE,_T("Image"),_T("*.bmp"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("BMP Image(*.bmp)|*.bmp|JPEG(*.jpg)|*.jpg||"));;
	CString strTemp;
	CString tmpPath_Name = _T("");
	char temp[512];
	wchar_t *tmpfilename = NULL;


	switch((int)para1)
	{
	//Display
	case 0:
		disp_obj(m_HalImage[GetCamIdx()][GetRotate()][GetScanIdx()], m_dlgImgRect[GetCamIdx()]->GetView());
		m_dlgImgRect[GetCamIdx()]->Display();
		DrawDeafultLine(GetCamIdx());
		if(m_dlgSetup->IsWindowVisible())
		{
			SendMessage(USER_MSG_IMG_RECT, GetCamIdx(), INS_RSLT_IMAGE_DISPLAY);
		}
		break;
	//Live on
	case 1:
		//HandleAllMessage();
		break;
	//Live off
	case 2:
		HandleAllMessage();
		break;
	//Image load
	case 3:
		Hal_OpenHalImage(&m_HalImage[GetCamIdx()][GetRotate()][GetScanIdx()], CString(""), FALSE);

		set_check("~give_error");
		herr = get_image_pointer1(m_HalImage[GetCamIdx()][GetRotate()][GetScanIdx()], NULL, NULL, &wd, &ht);
		set_check("give_error");
		if(H_MSG_TRUE == herr)
		{
		}
		else
		{
			AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
			return 0;
		}

		disp_obj(m_HalImage[GetCamIdx()][GetRotate()][GetScanIdx()], m_dlgImgRect[GetCamIdx()]->GetView());
		m_dlgImgRect[cam_idx]->Display();
		

		break;
	//Image save
	case 4:
		//8bit : BMP, JPEG
		if(dlgFile.DoModal() == IDOK)
		{
			tmpPath_Name = dlgFile.GetPathName();
			tmpfilename = tmpPath_Name.GetBuffer(tmpPath_Name.GetLength());

			// BMP or JPG
			strTemp = tmpPath_Name.Right(3);

			if(_T("bmp") == strTemp || _T("BMP") == strTemp)
			{
				//char temp[100];
				sprintf_s(temp, tmpPath_Name.GetLength() + 1, "%S", tmpPath_Name);
				write_image(m_HalImage[GetCamIdx()][GetRotate()][GetScanIdx()], HTuple("bmp"), NULL, temp);
			}
			else if(_T("jpg") == strTemp || _T("JPG") == strTemp)
			{
				//char temp[100];
				sprintf_s(temp, tmpPath_Name.GetLength() + 1, "%S", tmpPath_Name);
				write_image(m_HalImage[GetCamIdx()][GetRotate()][GetScanIdx()], HTuple("jpeg"), NULL, temp);
			}
			else
			{
			
			}
		}
		//Hal_SaveHalImage(&m_HalImage[cam_idx][m_nRotate[cam_idx]][m_nSelectScanIdx[cam_idx]], CString(""), CString("bmp"));		
		break;
	//Gray value
	case 5:

		break;
	//Region confirm
	case 6:
			
		break;
	//Test
	case 7:
		
		break;
	//결과 출력
	case 10:
		insPara = GetInsPara(GetCamIdx(), GetScanIdx(), GetRotate());
		grabPara = GetGrabPara(GetScanIdx());
#ifndef CAMERA
		grabPara.nCamIdx = GetCamIdx();
		grabPara.nScanIdx[grabPara.nCamIdx] = GetScanIdx();
		grabPara.nRotat[grabPara.nCamIdx] = GetRotate();
		grabPara.strLotNo.Format(_T("TEST"));
		grabPara.strGlassID.Format(_T("TEST1"));
#endif

		for(int i = 0; i < insPara.nInspectionCnt; i++)
		{
			rect.left = (LONG)insPara.InsRect[i].dRectLeft;
			rect.top = (LONG)insPara.InsRect[i].dRectTop;
			rect.right = (LONG)insPara.InsRect[i].dRectRight;
			rect.bottom = (LONG)insPara.InsRect[i].dRectBottom;
			gen_rectangle1(&sub_ins_rgn, rect.top, rect.left, rect.bottom, rect.right);

			union2(sub_ins_rgn, ins_rgn, &ins_rgn);
				
			set_color(m_dlgImgRect[grabPara.nCamIdx]->GetView(), HTuple("blue"));
			disp_obj(ins_rgn, m_dlgImgRect[grabPara.nCamIdx]->GetView());
		}

		if(grabPara.nCamIdx >= CAM_16K)
		{
			for(int i = 0; i < insPara.nInspectionCVDUpCnt; i++)
			{
				rect.left = (LONG)insPara.InsCVDUpRect[i].dRectLeft;
				rect.top = (LONG)insPara.InsCVDUpRect[i].dRectTop;
				rect.right = (LONG)insPara.InsCVDUpRect[i].dRectRight;
				rect.bottom = (LONG)insPara.InsCVDUpRect[i].dRectBottom;
				gen_rectangle1(&sub_ins_rgn, rect.top, rect.left, rect.bottom, rect.right);

				union2(sub_ins_rgn, ins_rgn1, &ins_rgn1);
			}
			for(int i = 0; i < insPara.nInspectionCVDDwCnt; i++)
			{
				rect.left = (LONG)insPara.InsCVDDwRect[i].dRectLeft;
				rect.top = (LONG)insPara.InsCVDDwRect[i].dRectTop;
				rect.right = (LONG)insPara.InsCVDDwRect[i].dRectRight;
				rect.bottom = (LONG)insPara.InsCVDDwRect[i].dRectBottom;
				gen_rectangle1(&sub_ins_rgn, rect.top, rect.left, rect.bottom, rect.right);

				union2(sub_ins_rgn, ins_rgn1, &ins_rgn1);
			}				

			set_color(m_dlgImgRect[grabPara.nCamIdx]->GetView(), HTuple("yellow"));
			disp_obj(ins_rgn1, m_dlgImgRect[grabPara.nCamIdx]->GetView());

			if(cam_idx >= CAM_16K)
			{
				if(grabPara.nInspectionType == 0)
				{
					for(int i = 0; i < m_nUpRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]; i++)
					{
						set_color(m_dlgImgRect[grabPara.nCamIdx]->GetView(), HTuple("red"));
						set_draw(m_dlgImgRect[grabPara.nCamIdx]->GetView(), HTuple("margin"));

						if(m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_y >= 5 && m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_x >= 5)
						{
							disp_line(m_dlgImgRect[grabPara.nCamIdx]->GetView(), m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_y - 5, m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_x, m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_y + 5, m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_x);
							disp_line(m_dlgImgRect[grabPara.nCamIdx]->GetView(), m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_y, m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_x - 5, m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_y, m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_x + 5);
						}
					}

					for(int i = 0; i < m_nDwRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]; i++)
					{
						set_color(m_dlgImgRect[grabPara.nCamIdx]->GetView(), HTuple("red"));
						set_draw(m_dlgImgRect[grabPara.nCamIdx]->GetView(), HTuple("margin"));

						if(m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_y >= 5 && m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_x >= 5)
						{
							disp_line(m_dlgImgRect[grabPara.nCamIdx]->GetView(), m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_y - 5, m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_x, m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_y + 5, m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_x);
							disp_line(m_dlgImgRect[grabPara.nCamIdx]->GetView(), m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_y, m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_x - 5, m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_y, m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_x + 5);
						}
					}
				}
				else
				{
					for(int i = 0; i < m_nUpPCLRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]; i++)
					{
						set_color(m_dlgImgRect[grabPara.nCamIdx]->GetView(), HTuple("red"));
						set_draw(m_dlgImgRect[grabPara.nCamIdx]->GetView(), HTuple("margin"));

						if(m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_y >= 5 && m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_x >= 5)
						{
							disp_line(m_dlgImgRect[grabPara.nCamIdx]->GetView(), m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_y - 5, m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_x, m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_y + 5, m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_x);
							disp_line(m_dlgImgRect[grabPara.nCamIdx]->GetView(), m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_y, m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_x - 5, m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_y, m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dUp_rslt_x + 5);
						}
					}

					for(int i = 0; i < m_nDwPCLRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]; i++)
					{
						set_color(m_dlgImgRect[grabPara.nCamIdx]->GetView(), HTuple("red"));
						set_draw(m_dlgImgRect[grabPara.nCamIdx]->GetView(), HTuple("margin"));

						if(m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_y >= 5 && m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_x >= 5)
						{
							disp_line(m_dlgImgRect[grabPara.nCamIdx]->GetView(), m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_y - 5, m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_x, m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_y + 5, m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_x);
							disp_line(m_dlgImgRect[grabPara.nCamIdx]->GetView(), m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_y, m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_x - 5, m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_y, m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][i].dDw_rslt_x + 5);
						}
					}
				}
			}
			else
			{

			}
		}
		else if(grabPara.nCamIdx == CAM_8K)
		{
			if(m_RsltNum[grabPara.nCamIdx][grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]] > 0)
			{
				set_color(m_dlgImgRect[grabPara.nCamIdx]->GetView(), HTuple("red"));
				disp_obj(m_RsltRegion[grabPara.nCamIdx][grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]], m_dlgImgRect[cam_idx]->GetView());
			}
			if(m_RsltPatNum[grabPara.nCamIdx][grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]] > 0)
			{
				set_color(m_dlgImgRect[grabPara.nCamIdx]->GetView(), HTuple("yellow"));
				disp_obj(m_RsltPatRegion[grabPara.nCamIdx][grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]], m_dlgImgRect[cam_idx]->GetView());
			}
		}

		break;
	default:
		AfxMessageBox(_T("Error : invalid index!"));
		break;
	}

	return 0;
}

void CIPSDlg::OnBnClickedRadioCam1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(MAX_CAM >= 2)
	{
		SelectCam(m_nCamIdx);
	}
	else
	{
		m_nCamIdx = 0;
		UpdateData(FALSE);
	}
}

void CIPSDlg::OnBnClickedRadioCam2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(MAX_CAM >= 2)
	{
		SelectCam(m_nCamIdx);
	}
	else
	{
		m_nCamIdx = 0;
		UpdateData(FALSE);
	}
}

void CIPSDlg::OnBnClickedRadioCam3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(MAX_CAM >= 2)
	{
		m_nCamIdx = 2;
		SelectCam(m_nCamIdx);
	}
	else
	{
		m_nCamIdx = 0;
		UpdateData(FALSE);
	}
}


void CIPSDlg::SelectCam(int cam_idx)
{
	SelectView(cam_idx);
	m_btnDerection.SetValue(m_nDerection[cam_idx]);
	ClickBtnenhctrlDerection();
	m_nImageIdx = m_nSelectScanIdx[cam_idx];
	UpdateData(FALSE);
	ClickBtnenhctrlDerection();
	SelectScanIdx(cam_idx, GetRotate(), m_nImageIdx);
	SendMessage(USER_MSG_IMG_RECT, cam_idx, IMAGE_DISPLAY);
}

void CIPSDlg::SelectView(int cam_idx)
{
	if(cam_idx == CAM_8K)
	{
		m_dlgImgRect[0]->ShowWindow(SW_SHOW);
		m_dlgImgRect[1]->ShowWindow(SW_HIDE);
#ifdef PCL_TEST
		m_dlgImgRect[2]->ShowWindow(SW_HIDE);
#endif
	}
	else if(cam_idx == CAM_16K)
	{
		m_dlgImgRect[0]->ShowWindow(SW_HIDE);
		m_dlgImgRect[1]->ShowWindow(SW_SHOW);
#ifdef PCL_TEST
		m_dlgImgRect[2]->ShowWindow(SW_HIDE);
#endif
	}
	else if(cam_idx == (CAM_16K + PCL_IMAGE))
	{
		m_dlgImgRect[0]->ShowWindow(SW_HIDE);
		m_dlgImgRect[1]->ShowWindow(SW_HIDE);
#ifdef PCL_TEST
		m_dlgImgRect[2]->ShowWindow(SW_SHOW);
#endif
	}
}

BEGIN_EVENTSINK_MAP(CIPSDlg, CDialog)
	ON_EVENT(CIPSDlg, IDC_BTNENHCTRL_EXIT, DISPID_CLICK, CIPSDlg::ClickBtnenhctrlExit, VTS_NONE)
	ON_EVENT(CIPSDlg, IDC_BTNENHCTRL_GRAB, DISPID_CLICK, CIPSDlg::ClickBtnenhctrlGrab, VTS_NONE)
	ON_EVENT(CIPSDlg, IDC_BTNENHCTRL_DERECTION, DISPID_CLICK, CIPSDlg::ClickBtnenhctrlDerection, VTS_NONE)
	ON_EVENT(CIPSDlg, IDC_BTNENHCTRL_AUTO, DISPID_CLICK, CIPSDlg::ClickBtnenhctrlAuto, VTS_NONE)
	ON_EVENT(CIPSDlg, IDC_BTNENHCTRL_ROTATE, DISPID_CLICK, CIPSDlg::ClickBtnenhctrlRotate, VTS_NONE)
	ON_EVENT(CIPSDlg, IDC_BTNENHCTRL_SAVE, DISPID_CLICK, CIPSDlg::ClickBtnenhctrlSave, VTS_NONE)
	ON_EVENT(CIPSDlg, IDC_BTNENHCTRL_SETUP, DISPID_CLICK, CIPSDlg::ClickBtnenhctrlSetup, VTS_NONE)
	ON_EVENT(CIPSDlg, IDC_BTNENHCTRL_GRAB_STOP, DISPID_CLICK, CIPSDlg::ClickBtnenhctrlGrabStop, VTS_NONE)
END_EVENTSINK_MAP()

void CIPSDlg::ClickBtnenhctrlExit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bAutoDeleteStart = FALSE;
	m_bAutoDeleteEnd = TRUE;

	m_bServerEnd = TRUE;
	m_Server->Close();
	m_bAutoStart = FALSE;
	m_bAutoEnd = TRUE;
	m_bInspectionEnd = TRUE;
	Sleep(1000);
#ifdef CAMERA
	for(int i = 0; i < MAX_CAM; i++)
	{
		MdigHalt(*m_VisMod->GetMilDigitizer(i));
	}
#endif
	if(m_btnAutoStart.GetValue())
	{
		m_btnAutoStart.SetValue(0);
	}

	if(m_pThreadGrab == NULL)
	{
		delete m_pThreadGrab;
	}

	if(m_pThreadInspection == NULL)
	{
		delete m_pThreadInspection;
	}
	
	for(int i = 0; i < MAX_CAM; i++)
	{
		delete m_dlgImgRect[i];
	}
#ifdef CAMERA
	m_VisMod->Close();
	delete m_VisMod;

	delete m_InsPara;

	for(int i = 0; i <  MAX_SCAN_CNT; i++)
	{
		for(int j = 0; j < MAX_ROTATE; j++)
		{
			delete m_CVDRslt[j][i];
		}
	}
#endif
	delete m_dlgSetup;

	delete m_Server;

	CString strTemp;
	strTemp.Format(_T("Application End"));
	WriteLog(LOG_PATH, strTemp);

	EndDialog(0);
}

void CIPSDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nEvent = (int)nIDEvent;
	CString strTemp;
	int nIdx;
	if(m_nCamIdx == CAM_8K)
	{
		nIdx = 8;
	}
	else if(m_nCamIdx == CAM_16K)
	{
		nIdx = 16;
	}

	switch(nEvent)
	{
	case 0:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CIPSDlg::ClickBtnenhctrlGrab()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	int rslt = 0;
	int grab_height = 0;
	int grab_offset = 0;
#ifdef CAMERA
	if(m_nCamIdx == CAM_8K)
	{
		switch(m_nRotate[m_nCamIdx])
		{
		case 0:
			grab_height = m_dlgSetup->m_dlgSetupTeaching->Get8KLongWidth();
			if(m_nDerection[m_nCamIdx] == 1)
			{
				grab_offset = m_dlgSetup->m_dlgSetupTeaching->Get8KLongOffset();
			}
			break;
		case 1:
			grab_height = m_dlgSetup->m_dlgSetupTeaching->Get8KShortWidth();
			if(m_nDerection[m_nCamIdx] == 1)
			{
				grab_offset = m_dlgSetup->m_dlgSetupTeaching->Get8KShortOffset();
			}
			break;
		}
	}
	else if(m_nCamIdx == CAM_16K)
	{
		switch(m_nRotate[m_nCamIdx])
		{
		case 0:
			grab_height = m_dlgSetup->m_dlgSetupTeaching->Get16KLongWidth();
			if(m_nDerection[m_nCamIdx] == 1)
			{
				grab_offset = m_dlgSetup->m_dlgSetupTeaching->Get16KLongOffset();
			}
			break;
		case 1:
			grab_height = m_dlgSetup->m_dlgSetupTeaching->Get16KShortWidth();
			if(m_nDerection[m_nCamIdx] == 1)
			{
				grab_offset = m_dlgSetup->m_dlgSetupTeaching->Get16KShortOffset();
			}
			break;
		}
	}
	CString strTemp;
	strTemp.Format(_T("Cam %d Rotate %d Image %d Test Grab Start"), m_nCamIdx, m_nRotate[m_nCamIdx], m_nSelectScanIdx[m_nCamIdx]);
	WriteLog(LOG_PATH, strTemp);
	int grab_cnt;
	rslt = m_VisMod->Grab(m_nCamIdx, m_nDerection[m_nCamIdx], grab_height, &grab_cnt);
	m_VisMod->ShiftImage(m_nCamIdx, 0, m_nDerection[m_nCamIdx], &m_HalImage[m_nCamIdx][m_nRotate[m_nCamIdx]][m_nSelectScanIdx[m_nCamIdx]]);
	
	strTemp.Format(_T("Cam %d Rotate %d Image %d Test Grab End"), m_nCamIdx, m_nRotate[m_nCamIdx], m_nSelectScanIdx[m_nCamIdx]);
	WriteLog(LOG_PATH, strTemp);
#endif
	SendMessage(USER_MSG_IMG_RECT, m_nCamIdx, IMAGE_DISPLAY);
}

void CIPSDlg::ClickBtnenhctrlDerection()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_btnDerection.GetValue())
	{
		m_btnDerection.SetWindowTextW(_T("역방향"));
	}
	else
	{
		m_btnDerection.SetWindowTextW(_T("정방향"));
	}
	m_nDerection[m_nCamIdx] = m_btnDerection.GetValue();
	UpdateData(FALSE);
}

void CIPSDlg::OnBnClickedRadioImage1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SelectScanIdx(m_nCamIdx, GetRotate(), m_nImageIdx);
}

void CIPSDlg::OnBnClickedRadioImage2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SelectScanIdx(m_nCamIdx, GetRotate(), m_nImageIdx);
}

void CIPSDlg::OnBnClickedRadioImage3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SelectScanIdx(m_nCamIdx, GetRotate(), m_nImageIdx);
}

void CIPSDlg::OnBnClickedRadioImage4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SelectScanIdx(m_nCamIdx, GetRotate(), m_nImageIdx);
}

void CIPSDlg::OnBnClickedRadioImage5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SelectScanIdx(m_nCamIdx, GetRotate(), m_nImageIdx);
}

void CIPSDlg::SelectScanIdx(int cam_idx, int rotate, int image_idx)
{
	CString strTemp;
	m_nSelectScanIdx[cam_idx] = image_idx;
	clear_window(m_dlgImgRect[cam_idx]->GetView());
	if(cam_idx == CAM_8K)
	{
		if(rotate == ROTATE_GRAB_0)
		{
			strTemp.Format(_T("Cam 1번 8K Rotate 0 %d번 Image"), image_idx + 1);
		}
		else if(rotate == ROTATE_GRAB_90)
		{
			strTemp.Format(_T("Cam 1번 8K Rotate 90 %d번 Image"), image_idx + 1);
		}
	}
	else if( cam_idx == CAM_16K)
	{
		if(rotate == ROTATE_GRAB_0)
		{
			strTemp.Format(_T("Cam 2번 16K Rotate 0 %d번 CVD Image"), image_idx + 1);
		}
		else if(rotate == ROTATE_GRAB_90)
		{
			strTemp.Format(_T("Cam 2번 16K Rotate 90 %d번 CVD Image"), image_idx + 1);
		}
	}
#ifdef PCL_TEST
	else if( cam_idx == (CAM_16K + PCL_IMAGE))
	{
		if(rotate == ROTATE_GRAB_0)
		{
			strTemp.Format(_T("Cam 2번 16K Rotate 0 %d번 PCL Image"), image_idx + 1);
		}
		else if(rotate == ROTATE_GRAB_90)
		{
			strTemp.Format(_T("Cam 2번 16K Rotate 90 %d번 PCL Image"), image_idx + 1);
		}
	}
#endif
	LoadInspPara(cam_idx, rotate, image_idx);
	if(m_dlgSetup->IsWindowVisible())
	{
		m_dlgSetup->Update(FALSE);
	}
	m_dlgImgRect[cam_idx]->m_Title.SetWindowTextW(strTemp);
	SendMessage(USER_MSG_IMG_RECT, cam_idx, IMAGE_DISPLAY);
}

void CIPSDlg::ClickBtnenhctrlAuto()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strTemp,packet;
	if(m_btnAutoStart.GetValue())
	{
		strTemp.Format(_T("Auto Mode"));
		WriteLog(LOG_PATH, strTemp);
		m_bAutoGrabInspectEnd = FALSE;
		m_bAutoEnd = FALSE;
		m_bAutoStart = TRUE;
		m_btnAutoStart.SetWindowTextW(_T("AUTO RUN"));
		m_pThreadGrab = AfxBeginThread(GrabThread, this, THREAD_PRIORITY_NORMAL);
		m_pThreadGrab->m_bAutoDelete = TRUE;

		m_bInspectionEnd = FALSE;
		m_pThreadInspection = AfxBeginThread(InspectionThread, this, THREAD_PRIORITY_NORMAL);
		m_pThreadInspection->m_bAutoDelete = TRUE;
		packet.Format(_T("%c%s%c"),PACKET_CHAR_STX, PACKET_SEND_AUTORUN, PACKET_CHAR_ETX);
		TCPIPSendMCS(packet);
	}
	else
	{
		strTemp.Format(_T("Manual Mode"));
		WriteLog(LOG_PATH, strTemp);
#ifdef CAMERA
		MdigHalt(*m_VisMod->GetMilDigitizer(GetCamIdx()));
#endif
		m_bAutoGrabInspectEnd = TRUE;
		m_bAutoEnd = TRUE;
		m_bAutoStart = FALSE;
		m_bInspectionEnd = TRUE;
		m_btnAutoStart.SetWindowTextW(_T("MANUAL RUN"));
		packet.Format(_T("%c%s%c"),PACKET_CHAR_STX, PACKET_SEND_MANUAL, PACKET_CHAR_ETX);
		TCPIPSendMCS(packet);
	}
}


BOOL CIPSDlg::GetAutoStartState()
{
	return m_btnAutoStart.GetValue();
}
void CIPSDlg::GrabReady(typeGrabPara grabPara)
{
	CString strSendDat, strTemp;
	strSendDat.Empty();
	typeSetupPara setupPara;
	setupPara = GetSetupPara();

	strTemp.Format(_T("Cam %d, Rotate %d, Image %d Width %d GrabReady Start"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], grabPara.nHeight[grabPara.nCamIdx]);
	WriteLog(LOG_PATH, strTemp);

	SetGrabReady(FALSE, grabPara.nScanIdx[grabPara.nCamIdx]);
	SetGrabComp(FALSE, grabPara.nScanIdx[grabPara.nCamIdx]);
	SetInspection(FALSE, grabPara.nScanIdx[grabPara.nCamIdx]);

	if((grabPara.nCamIdx > MAX_CAM) ||
		(grabPara.nHeight[grabPara.nCamIdx] % 1000) ||
		(grabPara.nDerection[grabPara.nCamIdx] > 2) ||
		(grabPara.nRotat[grabPara.nCamIdx] > 2))
	{
		strSendDat.Format(_T("%c%s,NG,%d,%d,%d%c"), PACKET_CHAR_STX, PACKET_SEND_GRAB_READY_COMP, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], PACKET_CHAR_ETX);
		TCPIPSendMCS(strSendDat);
		strTemp.Format(_T("Cam %d, Rotate %d, Image %d GrabReady Error"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
		WriteLog(LOG_PATH, strTemp);
		return;
	}
	grabPara.nGrabOffset[grabPara.nCamIdx] = 0;

	if(grabPara.nCamIdx == CAM_8K)
	{
		if(grabPara.nRotat[grabPara.nCamIdx] == ROTATE_GRAB_0)
		{
			grabPara.nGrabOffset[grabPara.nCamIdx] = setupPara.TeachingPara.n8K_LongOffset;
		}
		else
		{
			grabPara.nGrabOffset[grabPara.nCamIdx] = setupPara.TeachingPara.n8K_ShortOffset;
		}
	}
	else
	{
		if(grabPara.nRotat[grabPara.nCamIdx] == ROTATE_GRAB_0)
		{
			grabPara.nGrabOffset[grabPara.nCamIdx] = setupPara.TeachingPara.n16K_LongOffset;
		}
		else
		{
			grabPara.nGrabOffset[grabPara.nCamIdx] = setupPara.TeachingPara.n16K_ShortOffset;
		}
	}
	strSendDat.Format(_T("%c%s,OK,%d,%d,%d%c"), PACKET_CHAR_STX, PACKET_SEND_GRAB_READY_COMP, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], PACKET_CHAR_ETX);
	TCPIPSendMCS(strSendDat);
	strTemp.Format(_T("Cam %d, Rotate %d, Image %d GrabReady OK"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	WriteLog(LOG_PATH, strTemp);//pkh

	
#ifdef CAMERA
	strTemp.Format(_T("Cam %d, Rotate %d, Image %d Grab Start"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	WriteLog(LOG_PATH, strTemp);
	/*if(grabPara.nCamIdx == CAM_16K)
	{
		grabPara.nHeight[grabPara.nCamIdx] -= 1000;
	}*/
	int grab_cnt = 0;
	if(!m_VisMod->Grab(grabPara.nCamIdx, grabPara.nDerection[grabPara.nCamIdx], (grabPara.nHeight[grabPara.nCamIdx]), &grab_cnt))
	{
		m_nInspection_idx++;
		if(m_nInspection_idx == MAX_SCAN_CNT)
		{
			m_nInspection_idx = 0;
		}
		strSendDat.Format(_T("%c%s,NG,%d,%d,%d%c"), PACKET_CHAR_STX, PACKET_SEND_GRAB_COMP, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], PACKET_CHAR_ETX);
		SendMessage(USER_MSG_IMG_RECT, grabPara.nCamIdx, IMAGE_DISPLAY);
		TCPIPSendMCS(strSendDat);
		if(m_VisMod->GetGrabError(grabPara.nCamIdx) == 1)
		{
			strTemp.Format(_T("Cam %d, Rotate %d, Image %d Grab Stop"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
		}
		else if(m_VisMod->GetGrabError(grabPara.nCamIdx) == 2)
		{
			strTemp.Format(_T("Cam %d, Rotate %d, Image %d Grab Error, GrabCnt %d"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], grab_cnt);
		}
		WriteLog(LOG_PATH, strTemp);
		//AfxMessageBox(strTemp);
		return;
	}
	strTemp.Format(_T("Cam %d, Rotate %d, Image %d Grab End, GrabCnt %d"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], grab_cnt);
	WriteLog(LOG_PATH, strTemp);
	
	m_VisMod->ShiftImage(grabPara.nCamIdx, grabPara.nGrabOffset[grabPara.nCamIdx], grabPara.nDerection[grabPara.nCamIdx], &m_HalImage[grabPara.nCamIdx][grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]);
	
	strTemp.Format(_T("Cam %d, Rotate %d, Image %d Shift Move End"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	WriteLog(LOG_PATH, strTemp);

	if(grabPara.nScanIdx[grabPara.nCamIdx] == 0)
	{
		m_nInspection_idx = 0;
	}
	
	SetGrabComp(TRUE, grabPara.nScanIdx[grabPara.nCamIdx]);
	if(grabPara.nCamIdx == CAM_8K)
	{
		SetInspection(TRUE, grabPara.nScanIdx[grabPara.nCamIdx]);
	}
	else if(grabPara.nCamIdx >= CAM_16K)
	{
		SetInspection(TRUE, grabPara.nScanIdx[grabPara.nCamIdx]);
	}
	strSendDat.Format(_T("%c%s,OK,%d,%d,%d%c"), PACKET_CHAR_STX, PACKET_SEND_GRAB_COMP, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], PACKET_CHAR_ETX);
	SendMessage(USER_MSG_IMG_RECT, grabPara.nCamIdx, IMAGE_DISPLAY);
	TCPIPSendMCS(strSendDat);
	strTemp.Format(_T("Cam %d, Rotate %d, Image %d GrabReady End"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	WriteLog(LOG_PATH, strTemp);
#else
	HTuple wd, ht;
	Herror herr;

	//SaveInspPara(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	//LoadInspPara(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);

	if(grabPara.nScanIdx[grabPara.nCamIdx] == 0)
	{
		m_nInspection_idx = 0;
	}
	
	SetGrabComp(TRUE, grabPara.nScanIdx[grabPara.nCamIdx]);
	if(grabPara.nCamIdx == CAM_8K)
	{
		SetInspection(TRUE, grabPara.nScanIdx[grabPara.nCamIdx]);
	}
	else if(grabPara.nCamIdx == CAM_16K)
	{
		SetInspection(TRUE, grabPara.nScanIdx[grabPara.nCamIdx]);
	}

	disp_obj(m_HalImage[grabPara.nCamIdx][m_nRotate[grabPara.nCamIdx]][m_nSelectScanIdx[grabPara.nCamIdx]], m_dlgImgRect[grabPara.nCamIdx]->GetView());
	m_dlgImgRect[grabPara.nCamIdx]->Display();
	strSendDat.Format(_T("%c%s,OK,%d,%d,%d%c"), PACKET_CHAR_STX, PACKET_SEND_GRAB_COMP, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], PACKET_CHAR_ETX);
	SendMessage(USER_MSG_IMG_RECT, grabPara.nCamIdx, IMAGE_DISPLAY);
	TCPIPSendMCS(strSendDat);
	strTemp.Format(_T("Cam %d, Rotate %d, Image %d GrabReady End"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	WriteLog(LOG_PATH, strTemp);
#endif	
}

void CIPSDlg::ShiftImage()
{
	int offset = 0;
	if(m_nCamIdx == CAM_8K)
	{
		if(m_nRotate[m_nCamIdx] == ROTATE_GRAB_0)
		{
			offset = m_dlgSetup->m_dlgSetupTeaching->Get8KLongOffset();
		}
		else
		{
			offset = m_dlgSetup->m_dlgSetupTeaching->Get8KShortOffset();
		}
	}
	else
	{
		if(m_nRotate[m_nCamIdx] == ROTATE_GRAB_0)
		{
			offset = m_dlgSetup->m_dlgSetupTeaching->Get16KLongOffset();
		}
		else
		{
			offset = m_dlgSetup->m_dlgSetupTeaching->Get16KShortOffset();
		}
	}
	clear_window(m_dlgImgRect[m_nCamIdx]->GetView());
	m_HalBufImage[m_nCamIdx] = m_HalImage[m_nCamIdx][m_nRotate[m_nCamIdx]][m_nSelectScanIdx[m_nCamIdx]];
	m_VisMod->ShiftImage(m_nCamIdx, offset, m_nDerection[m_nCamIdx], &m_HalBufImage[m_nCamIdx]);
	disp_obj(m_HalBufImage[m_nCamIdx], m_dlgImgRect[m_nCamIdx]->GetView());
}

void CIPSDlg::BufImageToHalImage(int cam_idx, int rotate, int scan_idx)
{
	m_HalImage[cam_idx][rotate][scan_idx] = m_HalBufImage[cam_idx];
}

UINT CIPSDlg::GrabThread(LPVOID pParam)
{
	CIPSDlg *pdlg = (CIPSDlg *)pParam;
	typeGrabPara grabPara;
	int i = 0;
	/*grabPara.nCamIdx = 0;
	grabPara.nDerection[0] = 0;
	grabPara.nGrabOffset[0] = 0;
	grabPara.nHeight[0] = 10000;
	grabPara.nRotat[0] = 0;
	grabPara.nScanIdx[0] = 0;
	grabPara.strLotNo = _T("TEST");
	grabPara.strResultDataPath = _T("D:\\");
	grabPara.strResultImagePath = _T("D:\\");
	grabPara.strResultLotNoPath = _T("D:\\");
	pdlg->GrabReady(grabPara);*/
	while(TRUE)
	{
		if(pdlg->m_bAutoStart)
		{
			if(pdlg->m_bGrabReady[pdlg->m_nGrabIdx])// && !pdlg->GetGrabComp(pdlg->m_nGrabIdx))// && !pdlg->m_VisMod->GetGrab(pdlg->m_GrabPara.nCamIdx))
			{
				pdlg->m_bGrabReady[pdlg->m_nGrabIdx] = FALSE;
				grabPara = pdlg->GetGrabPara(pdlg->m_nGrabIdx);
				pdlg->GrabReady(grabPara);
			}
			Sleep(1);
		}
		else
		{
			Sleep(1);
		}

		if(pdlg->m_bAutoEnd)
		{
			break;
		}

		Sleep(1);
	}
	return 0;
}

UINT CIPSDlg::InspectionThread(LPVOID pParam)
{
	CIPSDlg *pdlg = (CIPSDlg *)pParam;
	typeGrabPara grabPara;
	int i = 0;
	while(TRUE)
	{
		if(pdlg->m_bAutoStart)
		{
			if(pdlg->GetInspection(pdlg->m_nInspection_idx) && pdlg->GetGrabComp(pdlg->m_nInspection_idx))
			{
				grabPara = pdlg->GetGrabPara(pdlg->m_nInspection_idx);
				pdlg->Inspection(grabPara);
				pdlg->m_nInspection_idx++;
				if(pdlg->m_nInspection_idx == MAX_SCAN_CNT)
				{
					pdlg->m_nInspection_idx = 0;
					for(i = 0; i < MAX_SCAN_CNT; i++)
					{
						pdlg->SetGrabComp(FALSE, 0);
						pdlg->SetGrabReady(FALSE, 0);
						pdlg->SetInspection(FALSE, 0);
					}
				}
			}
			Sleep(1);
		}
		else
		{
			Sleep(1);
		}

		if(pdlg->m_bInspectionEnd)
		{
			break;
		}

		Sleep(1);
	}
	return 0;
}

//Inspection_8K(grabPara)

void CIPSDlg::ClickBtnenhctrlRotate()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_btnRotate.GetValue())
	{
		m_btnRotate.SetWindowTextW(_T("90도"));
	}
	else
	{
		m_btnRotate.SetWindowTextW(_T("0도"));
	}
	SelectScanIdx(GetCamIdx(), GetRotate(), GetScanIdx());
}


void CIPSDlg::OnBnClickedRadioImage6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SelectScanIdx(m_nCamIdx, GetRotate(), m_nImageIdx);
}

void CIPSDlg::OnBnClickedRadioImage7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SelectScanIdx(m_nCamIdx, GetRotate(), m_nImageIdx);
}

void CIPSDlg::OnBnClickedRadioImage8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SelectScanIdx(m_nCamIdx, GetRotate(), m_nImageIdx);
}

void CIPSDlg::OnBnClickedRadioImage9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SelectScanIdx(m_nCamIdx, GetRotate(), m_nImageIdx);
}

void CIPSDlg::OnBnClickedRadioImage10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SelectScanIdx(m_nCamIdx, GetRotate(), m_nImageIdx);
}

void CIPSDlg::ClickBtnenhctrlSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//Hal_SaveHalImage(&m_HalImage[cam_idx][m_nRotate[cam_idx]][m_nSelectScanIdx[cam_idx]], CString(""), CString("bmp"));
	UpdateData(TRUE);
	//임시
	int nIdx;
	CString strTemp;
	if(m_nCamIdx == CAM_8K)
	{
		nIdx = 8;
	}
	else if(m_nCamIdx == CAM_16K)
	{
		nIdx = 16;
	}
	strTemp.Format(_T("D:\\Cam_%dK_%d_%d_%d.bmp"), nIdx, m_nRotate[m_nCamIdx], m_nDerection[m_nCamIdx], m_nSelectScanIdx[m_nCamIdx]);
	char temp[100];
	sprintf_s(temp, strTemp.GetLength() + 1, "%S", strTemp);
	write_image(m_HalImage[m_nCamIdx][m_nRotate[m_nCamIdx]][m_nSelectScanIdx[m_nCamIdx]], HTuple("bmp"), NULL, temp);
}

void CIPSDlg::ClickBtnenhctrlSetup()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	LoadSetupPara();
#ifdef PCL_TEST
	for(int i = 0; i < (MAX_CAM + PCL_IMAGE); i++)
	{
#else
	for(int i = 0; i < MAX_CAM; i++)
	{
#endif
		for(int j = 0; j < MAX_SCAN_CNT; j++)
		{
			for(int k = 0; k < MAX_ROTATE; k++)
			{
				LoadInspPara(i, j, k);
			}
		}
	}
	m_dlgSetup->ShowWindow(SW_SHOW);
}

BOOL CIPSDlg::Save_Log(BOOL disp, CString log, BOOL error, BOOL bFileSave)
{	
	CSize sz;
	CStdioFile file;
	CString path, str;	
	int year, month, day;
	int hour, minute, second;
	
	CString szTemp = _T("");	
	CTime	pTime = CTime::GetCurrentTime();
	CString sTime;
	
	year = pTime.GetYear();
	month = pTime.GetMonth();
	day = pTime.GetDay();
    hour = pTime.GetHour();
	minute = pTime.GetMinute();
	second = pTime.GetSecond();
	
	//sTime.Format("%04d/%02d/%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
	sTime.Format(_T("%02d:%02d:%02d > "), hour, minute, second);
		
	if ( bFileSave)
	{
		//Create_Dir(FALSE);
		path.Format(_T("%s\\Log\\%04d\\%04d-%02d\\%04d-%02d-%02d\\%04d%02d%02d.log"), mRootDirectory,
			year, year,  month, 	year, month, day,	year, month, day); 
		str.Format(_T("%02d:%02d:%02d >\t%s\n"),  hour, minute, second, log);
		
		if(file.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate) == 0)
			return FALSE;
		
		file.SeekToEnd();
		file.WriteString(str);
		file.Close();
	}
	
	if (error)
	{
		//Create_Dir(FALSE);
		path.Format(_T("%s\\Err\\%04d\\%04d-%02d\\%04d-%02d-%02d\\%04d%02d%02d.err"), mRootDirectory,
			year, year,  month, 	year, month, day,	year, month, day); 
		str.Format(_T("%02d:%02d:%02d >\t%s\n"), hour, minute, second, log);
		
		if(file.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate) == 0)
			return FALSE;
		
		file.SeekToEnd();
		file.WriteString(str);
		file.Close();
	}
	
	return TRUE;
}

void CIPSDlg::SaveImage(int cam_idx, int rotate, int scan_idx, char* filename, char* image_type)
{
	char temp[512];
	//temp.Format(_T("%S.%s"), filename, image_type);
	sprintf(temp, "%s.%s", filename, image_type);
	write_image(GetHalImage(cam_idx, rotate, scan_idx), HTuple(image_type), NULL, temp);
}

Hobject CIPSDlg::GetHalImage(int cam_idx, int rotate, int scan_idx)
{
	return m_HalImage[cam_idx][rotate][scan_idx];
}

int CIPSDlg::GetCamIdx()
{
	return m_nCamIdx;
}

int CIPSDlg::GetRotate()
{
	return m_btnRotate.GetValue();
}

int CIPSDlg::GetScanIdx()
{
	return m_nImageIdx;
}

void CIPSDlg::DrawDeafultLine(int cam_idx)
{
	//SendMessage(USER_MSG_IMG_RECT, m_nCamIdx, IMAGE_DISPLAY);
	Hobject line;
	set_color(m_dlgImgRect[cam_idx]->GetView(), "red");
	set_draw(m_dlgImgRect[cam_idx]->GetView(), HTuple("margin"));
	set_line_width(m_dlgImgRect[cam_idx]->GetView(), 2);
	int deafult_line_y = 0;
	int deafult_line_x = 0;
	m_dlgSetup->m_dlgSetupTeaching->UpdateData(TRUE);
	if(cam_idx == CAM_8K)
	{
		deafult_line_y = MAX_HALCON_IMAGE_WIDTH_8K;
		if(!GetRotate())
		{
			deafult_line_x = this->m_dlgSetup->m_dlgSetupTeaching->Get8KLongDeafultLineX();
		}
		else
		{
			deafult_line_x = this->m_dlgSetup->m_dlgSetupTeaching->Get8KLongDeafultLineY();
		}
	}
	else if(cam_idx == CAM_16K)
	{
		deafult_line_y = MAX_HALCON_IMAGE_WIDTH_16K;
		if(!GetRotate())
		{
			deafult_line_x = this->m_dlgSetup->m_dlgSetupTeaching->Get16KLongDeafultLineX();
		}
		else
		{
			deafult_line_x = this->m_dlgSetup->m_dlgSetupTeaching->Get16KLongDeafultLineY();
		}
	}

	gen_region_line(&line, (Hlong)(0), (Hlong)(deafult_line_x), 
					(Hlong)(deafult_line_y), (Hlong)(deafult_line_x));

	disp_obj(line, m_dlgImgRect[cam_idx]->GetView());
}

CString CIPSDlg::GetResultImagePath(){return m_strResultImagePath;}
CString CIPSDlg::GetResultDataPath(){return m_strResultDataPath;}
typeGrabPara CIPSDlg::GetGrabPara(int scan_idx){return m_GrabPara[scan_idx];}

BOOL CIPSDlg::WriteInsResultFile(CString file_path, CString result_data, int scan_Idx)
{
	FILE*	stream;
	char	strFile[MAX_PATH], strToFile[MAX_PATH];
    long	result, line;
	char	szTime[DAT_STR_LENG]; 
	CString bak_file, path, filePath;
	char buffer[DAT_STR_LENG];
	CTime time;
	time = CTime::GetCurrentTime();
	typeGrabPara grabPara = GetGrabPara(scan_Idx);
	int limit_line = 0;
	
	path.Empty();	

	//MakeDirectories(path);
	 
	sprintf_s(strFile, file_path.GetLength() + 1, "%S", file_path);

	if((stream = _fsopen( strFile, "a+" , _SH_DENYNO)) == NULL)
	{
		HANDLE fd = CreateFile( (LPCWSTR)(LPCSTR)strFile,
							GENERIC_READ|GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
		stream = _fsopen( strFile, "a+" , 0x04);
		if(stream == NULL)
		{
			CloseHandle(fd);
			return FALSE;
		}

		CloseHandle(fd);
	}

	//Check first time
    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}
	line = 0;
	while(fgets(buffer, MAX_STRING_LINE, stream)) 
	{
		line++;
		if(1 < line)
		{
			break;
		}
	}

    result = fseek(stream, 0L, SEEK_END);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	memset(szTime, NULL, sizeof(szTime));
	sprintf_s(szTime, result_data.GetLength() + 1, "%S", result_data);
    fputs(szTime, stream);
	fputs("\n", stream);

    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	//Check limit line
	line = 0;
	if(limit_line)
	{
		while(fgets(buffer, MAX_STRING_LINE, stream)) line++;
		if(line > limit_line)
		{
			sprintf_s(strToFile, filePath.GetLength() + bak_file.GetLength() + 2 + 1, "%S\\%S", path, bak_file);
			CopyFile((LPCWSTR)(LPCSTR)strFile, (LPCWSTR)(LPCSTR)strToFile, FALSE);
			fclose(stream);
			DeleteFile((LPCWSTR)(LPCSTR)strFile);
		}
		else
		{
			fclose(stream);
		}
	}
	else
	{
		fclose(stream);
	}

	return TRUE;
}


//20160302 ngh
BOOL CIPSDlg::Inspection(typeGrabPara grabPara)
{
	BOOL rslt = TRUE, bBlackWhite;
	Hobject rslt_rgn, slct_rgn;
	long rslt_num = 0, num = 0, i;
	long up_rslt_num = 0, dw_rslt_num = 0;
	HTuple Num, area, row0, col0, row1, col1, ccol, crow;
	HTuple gray_min, gray_max, gray_range;		// added, gray min, max
	HTuple gray_mean , gray_deviation;  //gray 평균값, 편차값
	CRect rslt_rect, rNGrect;
	CRect rslt_up_rect, rslt_dw_rect;
	CString strTemp, strDataFileName, strObjName, strData, strFileName, strSendDat;
	char temp[512];
	typeInspPara insPara;
	double *up_rslt_x, *up_rslt_y, *dw_rslt_x, *dw_rslt_y;

	LoadInspPara(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);

	insPara =  GetInsPara(grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx], grabPara.nRotat[grabPara.nCamIdx]);
	SetInspection(FALSE, grabPara.nScanIdx[grabPara.nCamIdx]);
	
	strTemp.Format(_T("Cam %d, Rotate %d, Image %d Inspection Start"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	WriteLog(LOG_PATH, strTemp);
	if(grabPara.nCamIdx == CAM_8K)
	{
		strTemp.Format(_T("Inspection Rect Cnt %d"), insPara.nInspectionCnt);
		WriteLog(LOG_PATH, strTemp);
	}
	else if(grabPara.nCamIdx == CAM_16K)
	{
		if(grabPara.nInspectionType == INSPECTION_TYPE_CVD)
		{
			strTemp.Format(_T("Inspection Rect Cnt %d, CVD Point Up Cnt %d, CVD Point Dw Cnt %d"), insPara.nInspectionCnt, insPara.nInspectionCVDUpCnt, insPara.nInspectionCVDDwCnt);
			WriteLog(LOG_PATH, strTemp);
		}
		else if(grabPara.nInspectionType == INSPECTION_TYPE_PCL)
		{
			strTemp.Format(_T("Inspection Rect Cnt %d, PCL Point Up Cnt %d, PCL Point Dw Cnt %d"), insPara.nInspectionCnt, insPara.nInspectionCVDUpCnt, insPara.nInspectionCVDDwCnt);
			WriteLog(LOG_PATH, strTemp);
		}
	}
	
	strFileName.Format(_T("%d_%d_%d_%s.csv"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], grabPara.strGlassID);	
	strDataFileName.Format(_T("%s\\%s"), grabPara.strResultDataPath, strFileName);
	
	if(FileSearch(grabPara.strResultDataPath, strFileName))
	{
		FileDelete(grabPara.strResultDataPath, strFileName);
	}

	if(grabPara.nCamIdx == CAM_8K)
	{
		strData.Format(_T("Defect_idx,Width,Height,CenterPoint.X,CenterPoint.Y,Area,Gray_min,Gray_max,Gray_mean,Gray_deviation,BlackWhite"));
		WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
		Hobject pat_rslt_rgn;
		long pat_rslt_num = 0;
		long white_num = 0;
		rslt_num = 0;
		BOOL bWhiteInspection = TRUE, bPatInspection = TRUE;
		
		//ngh_test
		if(!SetInspectScan(m_dlgImgRect[grabPara.nCamIdx]->GetView(), grabPara.nScanIdx[grabPara.nCamIdx],
			&GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), &rslt_num, &rslt_rgn, &pat_rslt_num, &pat_rslt_rgn))
		{
			pat_rslt_num = 0;
			rslt_num = -1;
			strTemp.Format(_T("Cam %d, Rotate %d, Image %d Inspection Error"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
			WriteLog(LOG_PATH, strTemp);
		}
		else
		{
			m_RsltRegion[grabPara.nCamIdx][grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]] = rslt_rgn;
			m_RsltNum[grabPara.nCamIdx][grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]] = rslt_num;

			m_RsltPatRegion[grabPara.nCamIdx][grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]] = pat_rslt_rgn;
			m_RsltPatNum[grabPara.nCamIdx][grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]] = pat_rslt_num;
		}

		if(rslt_num > 0)
		{
			//불량의 width, height얻기
			connection(rslt_rgn, &rslt_rgn);
			select_shape(rslt_rgn, &rslt_rgn, HTuple("area"), HTuple("and"), HTuple(2),  HTuple("max"));
			count_obj(rslt_rgn, &Num);
			Hobject rslt_image;
			bBlackWhite = TRUE;
		
			num = (Hlong)Num[0];
			white_num = num;
			for(i = 0; i < num; i++)
			{
				select_obj(rslt_rgn, &slct_rgn, i + 1);
				int sNum = 0;
				count_obj(slct_rgn, &Num);

				sNum = Num[0];

				area_center(slct_rgn, &area, &crow, &ccol);
				smallest_rectangle1(slct_rgn, &row0, &col0, &row1, &col1);
				rslt_rect = CRect((Hlong)col0[0], (Hlong)row0[0], (Hlong)col1[0], (Hlong)row1[0]);

	//			strData.Format(_T("%06d,%d,%d,%d,%d,%d"), i, rslt_rect.Width(), rslt_rect.Height(), rslt_rect.CenterPoint().x, rslt_rect.CenterPoint().y,(Hlong) area[0]);
	//sh			WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
			
				rNGrect.left = (rslt_rect.left - WHITE_NG_IMAGE_HEIGHT < 0) ? 0 : rslt_rect.left - WHITE_NG_IMAGE_HEIGHT;
				rNGrect.top = (rslt_rect.top - WHITE_NG_IMAGE_WIDTH < 0) ? 0 : rslt_rect.top - WHITE_NG_IMAGE_WIDTH;
				rNGrect.right = (rslt_rect.right + WHITE_NG_IMAGE_HEIGHT > MAX_HALCON_IMAGE_HEIGHT_8K) ? MAX_HALCON_IMAGE_HEIGHT_8K : rslt_rect.right + WHITE_NG_IMAGE_HEIGHT;
				rNGrect.bottom = (rslt_rect.bottom + WHITE_NG_IMAGE_WIDTH > MAX_HALCON_IMAGE_WIDTH_8K) ? MAX_HALCON_IMAGE_WIDTH_8K : rslt_rect.bottom + WHITE_NG_IMAGE_WIDTH;

				crop_rectangle1(GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), &rslt_image, (Hlong)rNGrect.top, (Hlong)rNGrect.left, (Hlong)rNGrect.bottom, (Hlong)rNGrect.right);
			
				//------------------------------------------------ gray min, max 추가
				//min_max_gray(slct_rgn, rslt_image, (Hlong)0, &gray_min, &gray_max, &gray_range); // 결함 slct_rgn은 이미지 전체에 대한 영역, rslt_image는 crop된 이미지 이므로 원본으로 변경해야 함
				min_max_gray(slct_rgn, GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), (Hlong)0, &gray_min, &gray_max, &gray_range);	//비추
				intensity(slct_rgn, GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), &gray_mean, &gray_deviation);

		//		gray_min1 = (Hlong)gray_min[0];
		//		gray_max1 = (Hlong)gray_max[0];
				//------------------------------------------------

				//if(/*((double)gray_min[0] >= (insPara.dThreshHold[0] / 2.5)) && */((double)gray_max[0] >= insPara.dThreshHold[1]/*(insPara.dThreshHold[0] * 2.0)*/)
				//	&& ((double)gray_deviation[0] >= (double)(int)(255 / (insPara.dThreshHold[0] / 8))))
				//{
				//	bBlackWhite = TRUE;
				//}
				//else
				//{
				//	bBlackWhite = FALSE;
				//}


				//20160717 ngh
				/////////////////////////////////////////////////////////////////////////////////////////////////
				BYTE *by_source;
				Hlong hw,hh;
				char typ[128];
				int iw,ih;
				get_image_pointer1(rslt_image, (Hlong*)&by_source, typ, &hw, &hh);
				
				iw = rNGrect.Width();
				ih = rNGrect.Height();

				for(int a = 0; a < hh; a++)
				{
					if((((hh / 2) - ((ih - 200) / 2)) <= a) && (a <= ((hh / 2) + ((ih - 200) / 2))))
					{
						by_source[(a * hw) + ((hw / 2) - ((iw - 200) / 2))] = 255;
						by_source[(a * hw) + ((hw / 2) + ((iw - 200) / 2))] = 255;
					}
					for(int b = 0; b < hw; b++)
					{
						if((((hw / 2) - ((iw - 200) / 2)) <= b) && (b <= ((hw / 2) + ((iw - 200) / 2))))
						{
							by_source[(((hh / 2) - (ih - 200) / 2) * hw) + b] = 255;
							by_source[(((hh / 2) + (ih - 200) / 2) * hw) + b] = 255;
						}
					}
				}
				/////////////////////////////////////////////////////////////////////////////////////////////////

				strData.Format(_T("%06d,%d,%d,%d,%d,%.2f,%d,%d,%.2f,%.2f,%d"), 
					i + 1, 
					rslt_rect.Width(), 
					rslt_rect.Height(), 
					rslt_rect.CenterPoint().x, 
					rslt_rect.CenterPoint().y, 
					(double)area[0],
					(Hlong)gray_min[0],
					(Hlong)gray_max[0],
					(double)gray_mean[0], 
					(double)gray_deviation[0],
					bBlackWhite);

				WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
			
				if(grabPara.nRotat[grabPara.nCamIdx])
				{
					rotate_image(rslt_image, &rslt_image, 90, "constant");
				}
				CString strImageName;
				strImageName.Format(_T("%s\\%d_%d_%d_%04d.jpg"), grabPara.strResultImagePath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], i + 1);
				sprintf_s(temp, strImageName.GetLength() + 1, "%S", strImageName);
				write_image(rslt_image, HTuple("jpeg"), NULL, temp);

				//strObjName.Format(_T("%s\\%d_%d_%04d"), grabPara.strResultImagePath, grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx], i);
				//sprintf_s(temp, strObjName.GetLength() + 1, "%S", strObjName);
				//write_object(slct_rgn, temp);
			}
		}
		if(pat_rslt_num > 0)
		{
			//불량의 width, height얻기
			connection(pat_rslt_rgn, &pat_rslt_rgn);
			select_shape(pat_rslt_rgn, &pat_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(2),  HTuple("max"));
			count_obj(pat_rslt_rgn, &Num);
			Hobject rslt_image;
			bBlackWhite = FALSE;
		
			num = (Hlong)Num[0];

			for(i = 0; i < num; i++)
			{
				select_obj(pat_rslt_rgn, &slct_rgn, i + 1);
				int sNum = 0;
				count_obj(slct_rgn, &Num);

				sNum = Num[0];

				area_center(slct_rgn, &area, &crow, &ccol);
				smallest_rectangle1(slct_rgn, &row0, &col0, &row1, &col1);
				rslt_rect = CRect((Hlong)col0[0], (Hlong)row0[0], (Hlong)col1[0], (Hlong)row1[0]);

	//			strData.Format(_T("%06d,%d,%d,%d,%d,%d"), i, rslt_rect.Width(), rslt_rect.Height(), rslt_rect.CenterPoint().x, rslt_rect.CenterPoint().y,(Hlong) area[0]);
	//sh			WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
			
				rNGrect.left = (rslt_rect.left - PAT_NG_IMAGE_HEIGHT < 0) ? 0 : rslt_rect.left - PAT_NG_IMAGE_HEIGHT;
				rNGrect.top = (rslt_rect.top - PAT_NG_IMAGE_WIDTH < 0) ? 0 : rslt_rect.top - PAT_NG_IMAGE_WIDTH;
				rNGrect.right = (rslt_rect.right + PAT_NG_IMAGE_HEIGHT > MAX_HALCON_IMAGE_HEIGHT_8K) ? MAX_HALCON_IMAGE_HEIGHT_8K : rslt_rect.right + PAT_NG_IMAGE_HEIGHT;
				rNGrect.bottom = (rslt_rect.bottom + PAT_NG_IMAGE_WIDTH > MAX_HALCON_IMAGE_WIDTH_8K) ? MAX_HALCON_IMAGE_WIDTH_8K : rslt_rect.bottom + PAT_NG_IMAGE_WIDTH;

				crop_rectangle1(GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), &rslt_image, (Hlong)rNGrect.top, (Hlong)rNGrect.left, (Hlong)rNGrect.bottom, (Hlong)rNGrect.right);
			
				//------------------------------------------------ gray min, max 추가
				//min_max_gray(slct_rgn, rslt_image, (Hlong)0, &gray_min, &gray_max, &gray_range); // 결함 slct_rgn은 이미지 전체에 대한 영역, rslt_image는 crop된 이미지 이므로 원본으로 변경해야 함
				min_max_gray(slct_rgn, GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), (Hlong)0, &gray_min, &gray_max, &gray_range);	//비추
				intensity(slct_rgn, GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), &gray_mean, &gray_deviation);

		//		gray_min1 = (Hlong)gray_min[0];
		//		gray_max1 = (Hlong)gray_max[0];
				//------------------------------------------------

				BYTE *by_source;
				Hlong hw,hh;
				char typ[128];
				int iw,ih;
				get_image_pointer1(rslt_image, (Hlong*)&by_source, typ, &hw, &hh);
				
				iw = rNGrect.Width();
				ih = rNGrect.Height();

				for(int a = 0; a < hh; a++)
				{
					if((((hh / 2) - ((ih - 100) / 2)) <= a) && (a <= ((hh / 2) + ((ih - 100) / 2))))
					{
						by_source[(a * hw) + ((hw / 2) - ((iw - 100) / 2))] = 255;
						by_source[(a * hw) + ((hw / 2) + ((iw - 100) / 2))] = 255;
					}
					for(int b = 0; b < hw; b++)
					{
						if((((hw / 2) - ((iw - 100) / 2)) <= b) && (b <= ((hw / 2) + ((iw - 100) / 2))))
						{
							by_source[(((hh / 2) - (ih - 100) / 2) * hw) + b] = 255;
							by_source[(((hh / 2) + (ih - 100) / 2) * hw) + b] = 255;
						}
					}
				}
			
				strData.Format(_T("%06d,%d,%d,%d,%d,%.2f,%d,%d,%.2f,%.2f,%d"), 
					i + 1 + white_num, 
					rslt_rect.Width(), 
					rslt_rect.Height(), 
					rslt_rect.CenterPoint().x, 
					rslt_rect.CenterPoint().y, 
					(double)area[0],
					(Hlong)gray_min[0],
					(Hlong)gray_max[0],
					(double)gray_mean[0], 
					(double)gray_deviation[0],
					bBlackWhite);

				WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
			
				if(grabPara.nRotat[grabPara.nCamIdx])
				{
					rotate_image(rslt_image, &rslt_image, 90, "constant");
				}
				CString strImageName;
				strImageName.Format(_T("%s\\%d_%d_%d_%04d.jpg"), grabPara.strResultImagePath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], i + 1 + white_num);
				sprintf_s(temp, strImageName.GetLength() + 1, "%S", strImageName);
				write_image(rslt_image, HTuple("jpeg"), NULL, temp);

				//strObjName.Format(_T("%s\\%d_%d_%04d"), grabPara.strResultImagePath, grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx], i);
				//sprintf_s(temp, strObjName.GetLength() + 1, "%S", strObjName);
				//write_object(slct_rgn, temp);
			}
		}
		num = pat_rslt_num + rslt_num;
		/*if(bPatInspection && bWhiteInspection)
		{
			num = pat_rslt_num + rslt_num;
		}
		else
		{
			num = -1;
		}*/

		SaveInspPara(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	}
	else if(grabPara.nCamIdx >= CAM_16K)
	{
		//20160608 ngh
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//불량의 width, height얻기
		Hobject rslt_image;
		CString strImageName;
		int idx = 0;
		up_rslt_x = new double[MAX_DEFECT_CNT];
		up_rslt_y = new double[MAX_DEFECT_CNT];
		dw_rslt_x = new double[MAX_DEFECT_CNT];
		dw_rslt_y = new double[MAX_DEFECT_CNT];
		memset(up_rslt_x, NULL, sizeof(double) * MAX_DEFECT_CNT);
		memset(up_rslt_y, NULL, sizeof(double) * MAX_DEFECT_CNT);
		memset(dw_rslt_x, NULL, sizeof(double) * MAX_DEFECT_CNT);
		memset(dw_rslt_y, NULL, sizeof(double) * MAX_DEFECT_CNT);

		strData.Format(_T("index,Up&Down,result_x(pixel),result_y(pixel)"));
		WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
	
		if(grabPara.nInspectionType == INSPECTION_TYPE_CVD)
		{
			SetInspectScanCVD(m_dlgImgRect[grabPara.nCamIdx]->GetView(), &GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), TRUE, grabPara.nScanIdx[grabPara.nCamIdx], 0.005228, insPara.dThreshHold[0], &up_rslt_num, up_rslt_x, up_rslt_y);
			SetInspectScanCVD(m_dlgImgRect[grabPara.nCamIdx]->GetView(), &GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), FALSE, grabPara.nScanIdx[grabPara.nCamIdx], 0.005228, insPara.dThreshHold[0], &dw_rslt_num, dw_rslt_x, dw_rslt_y);
			/*if(!SetInspectScanCVD(m_dlgImgRect[grabPara.nCamIdx]->GetView(), &GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), TRUE, grabPara.nScanIdx[grabPara.nCamIdx], 0.005228, insPara.dThreshHold[0], &up_rslt_num, up_rslt_x, up_rslt_y) ||
				!SetInspectScanCVD(m_dlgImgRect[grabPara.nCamIdx]->GetView(), &GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), FALSE, grabPara.nScanIdx[grabPara.nCamIdx], 0.005228, insPara.dThreshHold[0], &dw_rslt_num, dw_rslt_x, dw_rslt_y))
			{
				up_rslt_num = 0;
				dw_rslt_num = -1;
			}*/
		}
		else if(grabPara.nInspectionType == INSPECTION_TYPE_PCL)
		{
			if(!SetInspectScanPCL(m_dlgImgRect[grabPara.nCamIdx]->GetView(), &GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), TRUE, grabPara.nScanIdx[grabPara.nCamIdx], 0.005228, insPara.dThreshHold[0], &up_rslt_num, up_rslt_x, up_rslt_y) ||
				!SetInspectScanPCL(m_dlgImgRect[grabPara.nCamIdx]->GetView(), &GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), FALSE, grabPara.nScanIdx[grabPara.nCamIdx], 0.005228, insPara.dThreshHold[0], &dw_rslt_num, dw_rslt_x, dw_rslt_y))
			{
				up_rslt_num = 0;
				dw_rslt_num = -1;
			}
		}
	
		m_nUpRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]] = 0;
		m_nDwRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]] = 0;
		m_nUpPCLRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]] = 0;
		m_nDwPCLRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]] = 0;

		/*m_dUp_rslt_x = up_rslt_x;
		m_dUp_rslt_y = up_rslt_y;
		m_dDw_rslt_x = dw_rslt_x;
		m_dDw_rslt_y = dw_rslt_y;*/
		for(i = 0; (i < up_rslt_num) && (i < dw_rslt_num); i++)
		{
			rslt_up_rect = CRect(up_rslt_x[i] - 200, up_rslt_y[i] - 200, up_rslt_x[i] + 200, up_rslt_y[i] + 200);
			rslt_dw_rect = CRect(dw_rslt_x[i] - 200, dw_rslt_y[i] - 200, dw_rslt_x[i] + 200, dw_rslt_y[i] + 200);


			//Up
			rNGrect.left = (rslt_up_rect.left < 0) ? 0 : rslt_up_rect.left;
			rNGrect.top = (rslt_up_rect.top < 0) ? 0 : rslt_up_rect.top;
			rNGrect.right = (rslt_up_rect.right > MAX_HALCON_IMAGE_HEIGHT_16K) ? MAX_HALCON_IMAGE_HEIGHT_16K : rslt_up_rect.right;
			rNGrect.bottom = (rslt_up_rect.bottom > MAX_HALCON_IMAGE_WIDTH_16K) ? MAX_HALCON_IMAGE_WIDTH_16K : rslt_up_rect.bottom;

			crop_rectangle1(GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), &rslt_image, (Hlong)rNGrect.top, (Hlong)rNGrect.left, (Hlong)rNGrect.bottom, (Hlong)rNGrect.right);

			//20160609 ngh
			if(rNGrect.left > 0 && rNGrect.top > 0 && rNGrect.right < MAX_HALCON_IMAGE_HEIGHT_16K && rNGrect.bottom < MAX_HALCON_IMAGE_WIDTH_16K)
			{
				strData.Format(_T("%d,Up,%.3f,%.3f"), idx, up_rslt_x[i], up_rslt_y[i]);
				
				if(grabPara.nInspectionType == 0)
				{
					WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
					strImageName.Format(_T("%s\\%d_%d_%d_%06d_T.jpg"), grabPara.strResultImagePath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], idx);
					sprintf_s(temp, strImageName.GetLength() + 1, "%S", strImageName);
					write_image(rslt_image, HTuple("jpeg"), NULL, temp);

					m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][m_nUpRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]].dUp_rslt_x = up_rslt_x[i];
					m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][m_nUpRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]].dUp_rslt_y = up_rslt_y[i];
					m_nUpRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]++;
				}
				else
				{
					WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
					strImageName.Format(_T("%s\\%d_%d_%d_%06d_T.jpg"), grabPara.strResultImagePath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], idx);
					sprintf_s(temp, strImageName.GetLength() + 1, "%S", strImageName);
					write_image(rslt_image, HTuple("jpeg"), NULL, temp);

					m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][m_nUpPCLRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]].dUp_rslt_x = up_rslt_x[i];
					m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][m_nUpPCLRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]].dUp_rslt_y = up_rslt_y[i];
					m_nUpPCLRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]++;
				}
			}


			//Down
			rNGrect.left = (rslt_dw_rect.left < 0) ? 0 : rslt_dw_rect.left;
			rNGrect.top = (rslt_dw_rect.top < 0) ? 0 : rslt_dw_rect.top;
			rNGrect.right = (rslt_dw_rect.right > MAX_HALCON_IMAGE_HEIGHT_16K) ? MAX_HALCON_IMAGE_HEIGHT_16K : rslt_dw_rect.right;
			rNGrect.bottom = (rslt_dw_rect.bottom > MAX_HALCON_IMAGE_WIDTH_16K) ? MAX_HALCON_IMAGE_WIDTH_16K : rslt_dw_rect.bottom;

			crop_rectangle1(GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), &rslt_image, (Hlong)rNGrect.top, (Hlong)rNGrect.left, (Hlong)rNGrect.bottom, (Hlong)rNGrect.right);
			
			//20160609 ngh
			if(rNGrect.left > 0 && rNGrect.top > 0 && rNGrect.right < MAX_HALCON_IMAGE_HEIGHT_16K && rNGrect.bottom < MAX_HALCON_IMAGE_WIDTH_16K)
			{
				strData.Format(_T("%d,Down,%.3f,%.3f"), idx + 1, dw_rslt_x[i], dw_rslt_y[i]);
				
				if(grabPara.nInspectionType == 0)
				{
					WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
					strImageName.Format(_T("%s\\%d_%d_%d_%06d_B.jpg"), grabPara.strResultImagePath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], idx + 1);
					sprintf_s(temp, strImageName.GetLength() + 1, "%S", strImageName);
					write_image(rslt_image, HTuple("jpeg"), NULL, temp);

					m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][m_nDwRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]].dDw_rslt_x = dw_rslt_x[i];
					m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][m_nDwRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]].dDw_rslt_y = dw_rslt_y[i];
					m_nDwRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]++;
				}
				else
				{
					WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
					strImageName.Format(_T("%s\\%d_%d_%d_%06d_B.jpg"), grabPara.strResultImagePath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], idx + 1);
					sprintf_s(temp, strImageName.GetLength() + 1, "%S", strImageName);
					write_image(rslt_image, HTuple("jpeg"), NULL, temp);

					m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][m_nDwPCLRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]].dDw_rslt_x = dw_rslt_x[i];
					m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][m_nDwPCLRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]].dDw_rslt_y = dw_rslt_y[i];
					m_nDwPCLRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]]++;
				}
			}
			idx+=2;

		}	
		num = m_nUpRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]] + m_nDwRsltNum[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]];
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		delete up_rslt_x ;
		delete up_rslt_y;
		delete dw_rslt_x;
		delete dw_rslt_y;
	}

//#ifdef CAMERA
	strSendDat.Format(_T("%c1003,%d,%d,%d,%d,%s,%s%c"), PACKET_CHAR_STX, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], num, grabPara.strLotNo, grabPara.strGlassID, PACKET_CHAR_ETX);
	SendMessage(USER_MSG_IMG_RECT, grabPara.nCamIdx, IMAGE_DISPLAY);
	TCPIPSendMCS(strSendDat);
//#endif

	strTemp.Format(_T("Cam %d, Rotate %d, Image %d Inspection End"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	WriteLog(LOG_PATH, strTemp);

	return rslt;
}


BOOL CIPSDlg::Inspection_8K(typeGrabPara grabPara)
{
	BOOL rslt = TRUE;
	Hobject rslt_rgn, slct_rgn;
	long rslt_num = 0, num = 0, i;
	HTuple Num, area, row0, col0, row1, col1, ccol, crow;
	HTuple gray_min, gray_max, gray_range;		// added, gray min, max
	HTuple gray_mean , gray_deviation;  //gray 평균값, 편차값
	CRect rslt_rect, rNGrect;
	CString strTemp, strDataFileName, strObjName, strData, strFileName, strSendDat;
	char temp[512];
	typeInspPara insPara;

	insPara =  GetInsPara(grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx], grabPara.nRotat[grabPara.nCamIdx]);
	SetInspection(FALSE, grabPara.nScanIdx[grabPara.nCamIdx]);
	
	strTemp.Format(_T("Cam %d, Rotate %d, Image %d Inspection Start"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	WriteLog(LOG_PATH, strTemp);
	if(grabPara.nCamIdx == CAM_8K)
	{
		strTemp.Format(_T("Inspection Rect Cnt %d"), insPara.nInspectionCnt);
		WriteLog(LOG_PATH, strTemp);
	}
	else if(grabPara.nCamIdx == CAM_16K)
	{

	}
	
	strFileName.Format(_T("%d_%d_%d_%s.csv"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], grabPara.strGlassID);	
	strDataFileName.Format(_T("%s\\%s"), grabPara.strResultDataPath, strFileName);
	
	if(FileSearch(grabPara.strResultDataPath, strFileName))
	{
		FileDelete(grabPara.strResultDataPath, strFileName);
	}

	strData.Format(_T("Defect_idx,Width,Height ,CenterPoint.X,CenterPoint.Y,Area,Gray_min,Gray_max,Gray_mean,Gray_deviation"));
	WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
/*
	if(insPara.nInspectionCnt < 1)
	{
		strSendDat.Format(_T("%c1003,%d,%d,%d,-1,%sc"), PACKET_CHAR_STX, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], num, grabPara.strLotNo, PACKET_CHAR_ETX);
		SendMessage(USER_MSG_IMG_RECT, grabPara.nCamIdx, IMAGE_DISPLAY);
		TCPIPSendMCS(strSendDat);
		return FALSE;
	}*/

	SetInspectScan(m_dlgImgRect[grabPara.nCamIdx]->GetView(), grabPara.nScanIdx[grabPara.nCamIdx], 
		&GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]),
		&rslt_num, &rslt_rgn);
	
	/*if(m_bAutoGrabInspectEnd)
	{
		rslt = FALSE;
		return rslt;
	}*/
	m_RsltRegion[grabPara.nCamIdx][grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]] = rslt_rgn;
	if(rslt_num)
	{
		//불량의 width, height얻기
		connection(rslt_rgn, &rslt_rgn);
		select_shape(rslt_rgn, &rslt_rgn, HTuple("area"), HTuple("and"), HTuple(2),  HTuple("max"));
		count_obj(rslt_rgn, &Num);
		Hobject rslt_image;
		
		num = (Hlong)Num[0];

		for(i = 0; i < num; i++)
		{
			select_obj(rslt_rgn, &slct_rgn, i + 1);

			area_center(slct_rgn, &area, &crow, &ccol);
			smallest_rectangle1(slct_rgn, &row0, &col0, &row1, &col1);
			rslt_rect = CRect((Hlong)col0[0], (Hlong)row0[0], (Hlong)col1[0], (Hlong)row1[0]);

//			strData.Format(_T("%06d,%d,%d,%d,%d,%d"), i, rslt_rect.Width(), rslt_rect.Height(), rslt_rect.CenterPoint().x, rslt_rect.CenterPoint().y,(Hlong) area[0]);
//sh			WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
			
			rNGrect.left = (rslt_rect.left - WHITE_NG_IMAGE_HEIGHT < 0) ? 0 : rslt_rect.left - WHITE_NG_IMAGE_HEIGHT;
			rNGrect.top = (rslt_rect.top - WHITE_NG_IMAGE_WIDTH < 0) ? 0 : rslt_rect.top - WHITE_NG_IMAGE_WIDTH;
			rNGrect.right = (rslt_rect.right + WHITE_NG_IMAGE_HEIGHT > MAX_HALCON_IMAGE_HEIGHT_8K) ?  MAX_HALCON_IMAGE_HEIGHT_8K : rslt_rect.right + WHITE_NG_IMAGE_HEIGHT;
			rNGrect.bottom = (rslt_rect.bottom + WHITE_NG_IMAGE_WIDTH > MAX_HALCON_IMAGE_WIDTH_8K) ? MAX_HALCON_IMAGE_WIDTH_8K : rslt_rect.bottom + WHITE_NG_IMAGE_WIDTH;

			crop_rectangle1(GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), &rslt_image, (Hlong)rNGrect.top, (Hlong)rNGrect.left, (Hlong)rNGrect.bottom, (Hlong)rNGrect.right);
			
			//------------------------------------------------ gray min, max 추가
			//min_max_gray(slct_rgn, rslt_image, (Hlong)0, &gray_min, &gray_max, &gray_range); // 결함 slct_rgn은 이미지 전체에 대한 영역, rslt_image는 crop된 이미지 이므로 원본으로 변경해야 함
			min_max_gray(slct_rgn, GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), (Hlong)0, &gray_min, &gray_max, &gray_range);	//비추
			intensity(slct_rgn, GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), &gray_mean, &gray_deviation);

	//		gray_min1 = (Hlong)gray_min[0];
	//		gray_max1 = (Hlong)gray_max[0];
			//------------------------------------------------
			
			strData.Format(_T("%06d,%d,%d,%d,%d,%.2f,%d,%d,%.2f,%.2f"), 
				i, 
				rslt_rect.Width(), 
				rslt_rect.Height(), 
				rslt_rect.CenterPoint().x, 
				rslt_rect.CenterPoint().y, 
				(double)area[0],
				(Hlong)gray_min[0],
				(Hlong)gray_max[0],
				(double)gray_mean[0], 
				(double)gray_deviation[0]);

			WriteInsResultFile(strDataFileName, strData, grabPara.nScanIdx[grabPara.nCamIdx]);
			
			CString strImageName;
			strImageName.Format(_T("%s\\%d_%d_%d_%04d.jpg"), grabPara.strResultImagePath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], i);
			sprintf_s(temp, strImageName.GetLength() + 1, "%S", strImageName);
			write_image(rslt_image, HTuple("jpeg"), NULL, temp);

			//strObjName.Format(_T("%s\\%d_%d_%04d"), grabPara.strResultImagePath, grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx], i);
			//sprintf_s(temp, strObjName.GetLength() + 1, "%S", strObjName);
			//write_object(slct_rgn, temp);
		}
	}

	SaveInspPara(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);

	strSendDat.Format(_T("%c1003,%d,%d,%d,%d,%s%c"), PACKET_CHAR_STX, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], num, grabPara.strGlassID, PACKET_CHAR_ETX);
	SendMessage(USER_MSG_IMG_RECT, grabPara.nCamIdx, IMAGE_DISPLAY);
	TCPIPSendMCS(strSendDat);
	strTemp.Format(_T("Cam %d, Rotate %d, Image %d Inspection End"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	WriteLog(LOG_PATH, strTemp);

	return rslt;
}

void CIPSDlg::DefectImageave(typeGrabPara grabPara, int defect_idx)
{
	CString strImageName;
	char temp[512];
	strImageName.Format(_T("%s\\%d_%d_%04d.jpg"), GetResultImagePath(), grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx], defect_idx);
	sprintf_s(temp, strImageName.GetLength() + 1, "%S", strImageName);

}
//20160318 ngh
BOOL CIPSDlg::SetInspectScan(long disp, int scan_idx, Hobject *image, long *rslt_num, Hobject *rslt_rgn)
{
	BOOL rslt = TRUE;
	int cam_idx = 0;
	long num;
	typeInspPara insPara;
	typeGrabPara grabPara;

	grabPara = GetGrabPara(scan_idx);
	insPara = GetInsPara(grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx], grabPara.nRotat[grabPara.nCamIdx]);

	int noise[3];
	long black_rslt_num, white_rslt_num;
	Hobject black_rslt_rgn, white_rslt_rgn;
	Hobject pat_black_rslt_rgn, pat_white_rslt_rgn, pat_rslt_rgn;;
	int thresh[5];
	double distance = insPara.dMargeDistance[0] / 0.012;
	HTuple bwNum;
	HTuple bNum, wNum, pNum, Area, row, col;
	long lwNum, lpNum;
	Hobject sel_black_rgn, sel_white_rgn, dil_black, inter_rgn, sub_tot_rgn, sel_pat_rgn, dil_pat, sel_black_white_rgn, dil_white;
	Hobject dil_rgn, sel_white_rgn2, sum_white_rslt_rgn, sum_pat_rslt_rgn, sel_pat_rgn2;
	BOOL bSum;
#ifndef CAMERA
	//typeImageFilter imageFilter;
	//imageFilter = m_dlgSetup->m_dlgSetupTeaching->GetImageFilter();
#endif
	for(int i = 0; i < 5; i++)
	{
		thresh[i] = insPara.dThreshHold[i];
	}

	noise[0] = insPara.nNoiseSize[0];
	noise[1] = insPara.nNoiseSize[1];
	noise[2] = insPara.nNoiseSize[2];


	RECT rect;
	CRect ins_rect[512];
	Hobject sub_ins_rgn, Region, ins_rgn;
	Hobject ImageReduced, EmphaImage, MeanImage;
	Herror herr;
	HTuple Num;
	Hobject slct_rgn, slct_rgn2, sum_rgn, temp_rgn, temp_rgn2, black_white_rgn;
	HTuple area, row0, col0, row1, col1, ccol, crow;
	CRect rslt_rect, rslt_rect2;

	if(disp)
	{
		//disp_obj(*image, disp);
	}

	gen_empty_obj(&ins_rgn);
	gen_empty_obj(rslt_rgn);
	gen_empty_obj(&Region);
	*rslt_num = 0;
	black_rslt_num = 0;
	white_rslt_num = 0;
	//이물
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for(int i = 0; i < insPara.nInspectionCnt; i++)
	{
		rect.left = (LONG)insPara.InsRect[i].dRectLeft;
		rect.top = (LONG)insPara.InsRect[i].dRectTop;
		rect.right = (LONG)insPara.InsRect[i].dRectRight;
		rect.bottom = (LONG)insPara.InsRect[i].dRectBottom;
		gen_rectangle1(&sub_ins_rgn, rect.top, rect.left, rect.bottom, rect.right);
		ins_rect[i].SetRect(rect.left, rect.top, rect.right, rect.bottom);
		union2(sub_ins_rgn, ins_rgn, &ins_rgn);
	}
	
#ifdef CAMERA
	//mean_image(ImageReduced, &MeanImage, m_ImageFilter.dMeen1, m_ImageFilter.dMeen2);
	//emphasize(MeanImage, &EmphaImage, m_ImageFilter.dEmpha1, m_ImageFilter.dEmpha2, m_ImageFilter.Factro);
	mean_image(ImageReduced, &MeanImage, 2, 2);
	emphasize(MeanImage, &EmphaImage, 15, 15, 3);
#else
	//mean_image(*image, &MeanImage, insPara.ImageFilterPara[1].dMeen1, insPara.ImageFilterPara[1].dMeen2);
	//emphasize(MeanImage, &EmphaImage, insPara.ImageFilterPara[1].dEmpha1, insPara.ImageFilterPara[1].dEmpha2, insPara.ImageFilterPara[1].dFactor);

	mean_image(*image, &MeanImage, 2, 2);
	emphasize(MeanImage, &EmphaImage, 15, 15, 3);
#endif

	reduce_domain(EmphaImage, ins_rgn, &ImageReduced);

	set_check("~give_error");
	herr = threshold(ImageReduced, &Region, 0, thresh[2]);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		//Error log

		rslt = FALSE;
		return rslt;
	}	

	connection(Region, &pat_black_rslt_rgn);

	//작은 노이즈 없에기
	select_shape(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

	dilation_circle(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple(distance));
	closing_circle(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple(distance));

	gen_empty_obj(&sum_rgn);

	connection(pat_black_rslt_rgn, &sum_rgn);

	count_obj(sum_rgn, &Num);

	num = 0;
	num = (Hlong)Num[0];

	for(int i = 0; i < num - 1; i++)
	{
		gen_empty_obj(&temp_rgn);
		gen_empty_obj(&sub_tot_rgn);
		gen_empty_obj(&inter_rgn);

		bSum = FALSE;
		select_obj(pat_black_rslt_rgn, &sel_pat_rgn, i + 1);
		dilation_circle(sel_pat_rgn, &dil_rgn, distance);
		
		for(int j = 1; j < num; j++)
		{
			select_obj(pat_black_rslt_rgn, &sel_pat_rgn2, j + 1);
			intersection(dil_rgn, sel_pat_rgn2, &inter_rgn);
			area_center(inter_rgn, &Area, &row, &col);

			if(1 < (Hlong)Area[0])
			{
				bSum = TRUE;
				union2(sel_pat_rgn, sel_pat_rgn2, &sub_tot_rgn);
				union2(temp_rgn, sub_tot_rgn, &temp_rgn);
			}
		}
		
		if(bSum)
		{
			closing_circle(temp_rgn, &temp_rgn, HTuple(distance));
			union2(temp_rgn, sum_pat_rslt_rgn, &sum_pat_rslt_rgn);
		}
		else
		{
			copy_obj(sel_pat_rgn, &temp_rgn, 1, -1);
			union2(temp_rgn, sum_pat_rslt_rgn, &sum_pat_rslt_rgn);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//높이(주름)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	gen_empty_obj(&ins_rgn);
	gen_empty_obj(rslt_rgn);
	gen_empty_obj(&Region);
	*rslt_num = 0;
	black_rslt_num = 0;
	white_rslt_num = 0;

	for(int i = 0; i < insPara.nInspectionCnt; i++)
	{
		rect.left = (LONG)insPara.InsRect[i].dRectLeft;
		rect.top = (LONG)insPara.InsRect[i].dRectTop;
		rect.right = (LONG)insPara.InsRect[i].dRectRight;
		rect.bottom = (LONG)insPara.InsRect[i].dRectBottom;
		gen_rectangle1(&sub_ins_rgn, rect.top, rect.left, rect.bottom, rect.right);
		ins_rect[i].SetRect(rect.left, rect.top, rect.right, rect.bottom);
		union2(sub_ins_rgn, ins_rgn, &ins_rgn);
	}

#ifdef CAMERA
	//mean_image(ImageReduced, &MeanImage, m_ImageFilter.dMeen1, m_ImageFilter.dMeen2);
	//emphasize(MeanImage, &EmphaImage, m_ImageFilter.dEmpha1, m_ImageFilter.dEmpha2, m_ImageFilter.Factro);
	mean_image(ImageReduced, &MeanImage, 20, 20);
	emphasize(MeanImage, &EmphaImage, 100, 100, insPara.ImageFilterPara[0].dFactor);
#else
	//mean_image(*image, &MeanImage, insPara.ImageFilterPara[0].dMeen1, insPara.ImageFilterPara[0].dMeen2);
	//emphasize(MeanImage, &EmphaImage, insPara.ImageFilterPara[0].dEmpha1, insPara.ImageFilterPara[0].dEmpha2, insPara.ImageFilterPara[0].dFactor);

	mean_image(*image, &MeanImage, 20, 20);
	emphasize(MeanImage, &EmphaImage, 100, 100, insPara.ImageFilterPara[0].dFactor);
#endif

	reduce_domain(EmphaImage, ins_rgn, &ImageReduced);

	set_check("~give_error");
	herr = threshold(ImageReduced, &Region, thresh[0], thresh[1]);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		//Error log

		rslt = FALSE;
		return rslt;
	}	

	connection(Region, &white_rslt_rgn);

	//작은 노이즈 없에기
	select_shape(white_rslt_rgn, &white_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(white_rslt_rgn, &white_rslt_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(white_rslt_rgn, &white_rslt_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

	dilation_circle(white_rslt_rgn, &white_rslt_rgn, HTuple(distance));
	closing_circle(white_rslt_rgn, &white_rslt_rgn, HTuple(distance));

	gen_empty_obj(&sum_rgn);
	connection(white_rslt_rgn, &sum_rgn);

	count_obj(sum_rgn, &Num);

	num = 0;
	num = (Hlong)Num[0];

	for(int i = 0; i < num - 1; i++)
	{
		gen_empty_obj(&temp_rgn);
		gen_empty_obj(&sub_tot_rgn);
		gen_empty_obj(&inter_rgn);

		bSum = FALSE;
		select_obj(white_rslt_rgn, &sel_white_rgn, i + 1);
		dilation_circle(sel_white_rgn, &dil_rgn, distance);
		
		for(int j = 1; j < num; j++)
		{
			select_obj(white_rslt_rgn, &sel_white_rgn2, j + 1);
			intersection(dil_rgn, sel_white_rgn2, &inter_rgn);
			area_center(inter_rgn, &Area, &row, &col);

			if(1 < (Hlong)Area[0])
			{
				bSum = TRUE;
				union2(sel_white_rgn, sel_white_rgn2, &sub_tot_rgn);
				union2(temp_rgn, sub_tot_rgn, &temp_rgn);
			}
		}
		
		if(bSum)
		{
			closing_circle(temp_rgn, &temp_rgn, HTuple(distance));
			union2(temp_rgn, sum_white_rslt_rgn, &sum_white_rslt_rgn);
		}
		else
		{
			copy_obj(sel_white_rgn, &temp_rgn, 1, -1);
			union2(temp_rgn, sum_white_rslt_rgn, &sum_white_rslt_rgn);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//이물&높이
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Hobject sum_rslt_rgn;
	lpNum = 0;
	lwNum = 0;
	count_obj(sum_pat_rslt_rgn, &Num);
	lpNum = (Hlong)Num[0];
	count_obj(sum_white_rslt_rgn, &Num);
	lwNum = (Hlong)Num[0];


	for(int i = 0; i < lpNum; i++)
	{
		gen_empty_obj(&temp_rgn);
		gen_empty_obj(&sub_tot_rgn);
		gen_empty_obj(&inter_rgn);

		bSum = FALSE;
		select_obj(sum_pat_rslt_rgn, &sel_pat_rgn, i + 1);
		
		for(int j = 1; j < lwNum; j++)
		{
			select_obj(sum_white_rslt_rgn, &sel_white_rgn, j + 1);
			intersection(sel_pat_rgn, sel_white_rgn, &inter_rgn);
			area_center(inter_rgn, &Area, &row, &col);

			if(1 < (Hlong)Area[0])
			{
				bSum = TRUE;
				union2(sel_pat_rgn, sel_white_rgn, &sub_tot_rgn);
				union2(temp_rgn, sub_tot_rgn, &temp_rgn);
			}
		}
		
		if(bSum)
		{
			closing_circle(temp_rgn, &temp_rgn, HTuple(distance));
			union2(temp_rgn, sum_rslt_rgn, &sum_rslt_rgn);
		}
		else
		{
			copy_obj(sel_pat_rgn, &temp_rgn, 1, -1);
			union2(temp_rgn, sum_rslt_rgn, &sum_rslt_rgn);
		}
	}

	connection(sum_rslt_rgn, &sum_rslt_rgn);

	count_obj(sum_rslt_rgn, &Num);
	num = 0;
	num = (Hlong)Num[0];
	count_obj(sum_white_rslt_rgn, &Num);
	lwNum = (Hlong)Num[0];

	Hobject sel_sum_rgn, sel_sum_rgn2, tot_sum_rslt_rgn;

	copy_obj(sum_rslt_rgn, &tot_sum_rslt_rgn, 1, -1);

	for(int i = 0; i < lwNum - 1; i++)
	{
		gen_empty_obj(&temp_rgn);
		gen_empty_obj(&sub_tot_rgn);
		gen_empty_obj(&inter_rgn);

		bSum = FALSE;
		select_obj(sum_white_rslt_rgn, &sel_sum_rgn, i + 1);

		for(int j = 1; j < num; j++)
		{
			select_obj(sum_rslt_rgn, &sel_sum_rgn2, j + 1);
			intersection(sel_sum_rgn, sel_sum_rgn2, &inter_rgn);
			area_center(inter_rgn, &Area, &row, &col);

			if(1 < (Hlong)Area[0])
			{
				bSum = TRUE;
			}
		}

		if(!bSum)
		{
			copy_obj(sel_sum_rgn, &temp_rgn, 1, -1);
			union2(temp_rgn, tot_sum_rslt_rgn, &tot_sum_rslt_rgn);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	gen_empty_obj(&sum_rgn);
	erosion_circle(sum_white_rslt_rgn, &sum_rgn, HTuple(distance));

	connection(sum_rgn, &sum_rgn);

	count_obj(sum_rgn, &Num);

	num = 0;
	num = (Hlong)Num[0];
	
	if(0 < num)
	{
		if(disp)
		{
			set_color(disp, HTuple("red"));
			set_draw(disp, HTuple("margin"));
			disp_obj(sum_rgn, disp);
		}

		*rslt_num = num;
		copy_obj(sum_rgn, rslt_rgn, 1, -1);
		connection(*rslt_rgn, rslt_rgn);
	}
	else
	{

	}

	if(disp)
	{
		set_color(disp, HTuple("blue"));
		set_draw(disp, HTuple("margin"));
#ifdef CAMERA
		disp_obj(ins_rgn, disp);
#else
		disp_obj(sum_pat_rslt_rgn, disp);

		set_color(disp, HTuple("yellow"));
		set_draw(disp, HTuple("margin"));
		disp_obj(sum_white_rslt_rgn, disp);

		set_color(disp, HTuple("red"));
		set_draw(disp, HTuple("margin"));
		disp_obj(sum_rgn, disp);
#endif
	}

	return rslt;
}

//2160401 ngh
BOOL CIPSDlg::SetInspectScan(long disp, int scan_idx, Hobject *image, long *rslt_num, Hobject *rslt_rgn, long *pat_rslt_num, Hobject *pat_rslt_rgn)
{
	BOOL rslt = TRUE;
	int cam_idx = 0;
	long num;
	typeInspPara insPara;
	typeGrabPara grabPara;
	typeSetupPara setupPara;
	
	grabPara = GetGrabPara(scan_idx);
	insPara = GetInsPara(grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx], grabPara.nRotat[grabPara.nCamIdx]);
	
	setupPara = GetSetupPara();
	int noise[3];
	long black_rslt_num, white_rslt_num;
	Hobject black_rslt_rgn, white_rslt_rgn;
	Hobject pat_black_rslt_rgn, pat_white_rslt_rgn, pat_rslt_rgn1;
	int thresh[5];
	double distance = insPara.dMargeDistance[0] / 0.012;
	HTuple bwNum;
	HTuple bNum, wNum, pNum, Area, row, col;
	long lwNum, lpNum;
	Hobject sel_black_rgn, sel_white_rgn, dil_black, inter_rgn, sub_tot_rgn, sel_pat_rgn, dil_pat, sel_black_white_rgn, dil_white;
	Hobject dil_rgn, dil_rgn2, sel_white_rgn2, sum_white_rslt_rgn, sum_pat_rslt_rgn, sel_pat_rgn2;
	BOOL bSum;
	BOOL bPatResult = TRUE, bWhiteResult = TRUE;

	for(int i = 0; i < 5; i++)
	{
		thresh[i] = insPara.dThreshHold[i];
	}

	noise[0] = insPara.nNoiseSize[0];
	noise[1] = insPara.nNoiseSize[1];
	noise[2] = insPara.nNoiseSize[2];

	RECT rect;
	CRect ins_rect[512];
	Hobject sub_ins_rgn, Region, ins_rgn;
	Hobject ImageReduced, EmphaImage, MeanImage;
	Herror herr;
	HTuple Num;
	Hobject slct_rgn, slct_rgn2, sum_rgn, temp_rgn, temp_rgn2, black_white_rgn;
	HTuple area, row0, col0, row1, col1, ccol, crow;
	CRect rslt_rect, rslt_rect2;

	if(disp)
	{
		//disp_obj(*image, disp);
	}

	gen_empty_obj(&ins_rgn);
	gen_empty_obj(rslt_rgn);
	gen_empty_obj(&Region);
	*rslt_num = 0;
	black_rslt_num = 0;
	white_rslt_num = 0;
	//이물
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for(int i = 0; i < insPara.nInspectionCnt; i++)
	{
		rect.left = (LONG)insPara.InsRect[i].dRectLeft;
		rect.top = (LONG)insPara.InsRect[i].dRectTop;
		rect.right = (LONG)insPara.InsRect[i].dRectRight;
		rect.bottom = (LONG)insPara.InsRect[i].dRectBottom;
		gen_rectangle1(&sub_ins_rgn, rect.top, rect.left, rect.bottom, rect.right);
		ins_rect[i].SetRect(rect.left, rect.top, rect.right, rect.bottom);
		union2(sub_ins_rgn, ins_rgn, &ins_rgn);
	}

	mean_image(*image, &MeanImage, 2, 2);
	emphasize(MeanImage, &EmphaImage, 15, 15, insPara.ImageFilterPara[1].dFactor);

	reduce_domain(EmphaImage, ins_rgn, &ImageReduced);

	set_check("~give_error");
	herr = threshold(ImageReduced, &Region, 0, thresh[2]);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		//Error log

		rslt = FALSE;
		return rslt;
	}	

	connection(Region, &pat_black_rslt_rgn);

	//작은 노이즈 없에기
	select_shape(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

	dilation_circle(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple(distance));
	closing_circle(pat_black_rslt_rgn, &pat_black_rslt_rgn, HTuple(distance));

	gen_empty_obj(&sum_rgn);

	connection(pat_black_rslt_rgn, &sum_rgn);

	count_obj(sum_rgn, &Num);

	num = 0;
	num = (Hlong)Num[0];

	if(num > setupPara.nMaxDefectCnt)
	{
		bPatResult = FALSE;
	}

	if(m_bAutoGrabInspectEnd)
	{
		return FALSE;
	}

	lpNum = 0;
	if(bPatResult)
	{
		for(int i = 0; i < num - 1; i++)
		{
			gen_empty_obj(&temp_rgn);
			gen_empty_obj(&sub_tot_rgn);
			gen_empty_obj(&inter_rgn);

			bSum = FALSE;
			select_obj(pat_black_rslt_rgn, &sel_pat_rgn, i + 1);
			dilation_circle(sel_pat_rgn, &dil_rgn, distance);
		
			for(int j = 1; j < num; j++)
			{
				if(m_bAutoGrabInspectEnd)
				{
					return FALSE;
				}

				select_obj(pat_black_rslt_rgn, &sel_pat_rgn2, j + 1);
				intersection(dil_rgn, sel_pat_rgn2, &inter_rgn);
				area_center(inter_rgn, &Area, &row, &col);

				if(1 < (Hlong)Area[0])
				{
					bSum = TRUE;
					union2(sel_pat_rgn, sel_pat_rgn2, &sub_tot_rgn);
					union2(temp_rgn, sub_tot_rgn, &temp_rgn);
				}
			}
		
			if(bSum)
			{
				closing_circle(temp_rgn, &temp_rgn, HTuple(distance));
				union2(temp_rgn, sum_pat_rslt_rgn, &sum_pat_rslt_rgn);
			}
			else
			{
				copy_obj(sel_pat_rgn, &temp_rgn, 1, -1);
				union2(temp_rgn, sum_pat_rslt_rgn, &sum_pat_rslt_rgn);
			}
		}

		erosion_circle(sum_pat_rslt_rgn, &sum_pat_rslt_rgn, HTuple(distance));

		//작은 노이즈 없에기
		select_shape(sum_pat_rslt_rgn, &sum_pat_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
		select_shape(sum_pat_rslt_rgn, &sum_pat_rslt_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
		select_shape(sum_pat_rslt_rgn, &sum_pat_rslt_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

		connection(sum_pat_rslt_rgn, &sum_pat_rslt_rgn);

		count_obj(sum_pat_rslt_rgn, &Num);
		lpNum = (Hlong)Num[0];
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//높이(주름)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	gen_empty_obj(&ins_rgn);
	gen_empty_obj(rslt_rgn);
	gen_empty_obj(&Region);
	*rslt_num = 0;
	black_rslt_num = 0;
	white_rslt_num = 0;

	for(int i = 0; i < insPara.nInspectionCnt; i++)
	{
		rect.left = (LONG)insPara.InsRect[i].dRectLeft;
		rect.top = (LONG)insPara.InsRect[i].dRectTop;
		rect.right = (LONG)insPara.InsRect[i].dRectRight;
		rect.bottom = (LONG)insPara.InsRect[i].dRectBottom;
		gen_rectangle1(&sub_ins_rgn, rect.top, rect.left, rect.bottom, rect.right);
		ins_rect[i].SetRect(rect.left, rect.top, rect.right, rect.bottom);
		union2(sub_ins_rgn, ins_rgn, &ins_rgn);
	}

	mean_image(*image, &MeanImage, 20, 20);
	emphasize(MeanImage, &EmphaImage, 100, 100, insPara.ImageFilterPara[0].dFactor);

	reduce_domain(EmphaImage, ins_rgn, &ImageReduced);

	set_check("~give_error");
	herr = threshold(ImageReduced, &Region, thresh[0], thresh[1]);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		//Error log

		rslt = FALSE;
		return rslt;
	}	

	connection(Region, &white_rslt_rgn);

	//작은 노이즈 없에기
	select_shape(white_rslt_rgn, &white_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
	select_shape(white_rslt_rgn, &white_rslt_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
	select_shape(white_rslt_rgn, &white_rslt_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

	dilation_circle(white_rslt_rgn, &white_rslt_rgn, HTuple(distance));
	closing_circle(white_rslt_rgn, &white_rslt_rgn, HTuple(distance));

	gen_empty_obj(&sum_rgn);
	connection(white_rslt_rgn, &sum_rgn);

	count_obj(sum_rgn, &Num);

	num = 0;
	num = (Hlong)Num[0];

	if(num > setupPara.nMaxDefectCnt)
	{
		bWhiteResult = FALSE;
	}

	if(m_bAutoGrabInspectEnd)
	{
		return FALSE;
	}

	lwNum = 0;
	if(bWhiteResult)
	{
		for(int i = 0; i < num - 1; i++)
		{
			gen_empty_obj(&temp_rgn);
			gen_empty_obj(&sub_tot_rgn);
			gen_empty_obj(&inter_rgn);

			bSum = FALSE;
			select_obj(white_rslt_rgn, &sel_white_rgn, i + 1);
			dilation_circle(sel_white_rgn, &dil_rgn, distance);
		
			for(int j = 1; j < num; j++)
			{
				if(m_bAutoGrabInspectEnd)
				{
					return FALSE;
				}

				select_obj(white_rslt_rgn, &sel_white_rgn2, j + 1);
				intersection(dil_rgn, sel_white_rgn2, &inter_rgn);
				area_center(inter_rgn, &Area, &row, &col);

				if(1 < (Hlong)Area[0])
				{
					bSum = TRUE;
					union2(sel_white_rgn, sel_white_rgn2, &sub_tot_rgn);
					union2(temp_rgn, sub_tot_rgn, &temp_rgn);
				}
			}
		
			if(bSum)
			{
				closing_circle(temp_rgn, &temp_rgn, HTuple(distance));
				union2(temp_rgn, sum_white_rslt_rgn, &sum_white_rslt_rgn);
			}
			else
			{
				copy_obj(sel_white_rgn, &temp_rgn, 1, -1);
				union2(temp_rgn, sum_white_rslt_rgn, &sum_white_rslt_rgn);
			}
		}

		//작은 노이즈 없에기
		select_shape(sum_white_rslt_rgn, &sum_white_rslt_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
		select_shape(sum_white_rslt_rgn, &sum_white_rslt_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
		select_shape(sum_white_rslt_rgn, &sum_white_rslt_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

		connection(sum_white_rslt_rgn, &sum_white_rslt_rgn);

		count_obj(sum_white_rslt_rgn, &Num);
		lwNum = (Hlong)Num[0];
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Hobject sel_sum_rgn, sel_sum_rgn2, tot_sum_rslt_rgn;

	if(bPatResult)
	{
		for(int i = 0; i < lpNum; i++)
		{
			gen_empty_obj(&temp_rgn);
			gen_empty_obj(&sub_tot_rgn);
			gen_empty_obj(&inter_rgn);

			bSum = FALSE;
			select_obj(sum_pat_rslt_rgn, &sel_pat_rgn, i + 1);
			dilation_circle(sel_pat_rgn, &dil_rgn, distance);
		
			for(int j = 0; j < lwNum; j++)
			{
				if(m_bAutoGrabInspectEnd)
				{
					return FALSE;
				}

				select_obj(sum_white_rslt_rgn, &sel_sum_rgn, j + 1);
				//dilation_circle(sel_sum_rgn, &dil_rgn2, distance);
				intersection(dil_rgn, sel_sum_rgn, &inter_rgn);
				area_center(inter_rgn, &Area, &row, &col);

				if(1 < (Hlong)Area[0])
				{
					bSum = TRUE;
				}
			}

			if(!bSum)
			{
				copy_obj(sel_pat_rgn, &temp_rgn, 1, -1);
				union2(temp_rgn, tot_sum_rslt_rgn, &tot_sum_rslt_rgn);
			}
		}
		gen_empty_obj(&sum_rgn);
		connection(tot_sum_rslt_rgn, &sum_rgn);

		//이물 Display
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//작은 노이즈 없에기
		select_shape(sum_rgn, &sum_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
		select_shape(sum_rgn, &sum_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
		select_shape(sum_rgn, &sum_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

		connection(sum_rgn, &sum_rgn);

		count_obj(sum_rgn, &Num);

		num = 0;
		num = (Hlong)Num[0];
	
		if(0 < num)
		{
			if(disp)
			{
				set_color(disp, HTuple("yellow"));
				set_draw(disp, HTuple("margin"));
				disp_obj(sum_rgn, disp);
			}

			*pat_rslt_num = num;
			copy_obj(sum_rgn, pat_rslt_rgn, 1, -1);
			connection(*pat_rslt_rgn, pat_rslt_rgn);
		}
		else
		{

		}
	}
	else
	{
		*pat_rslt_num = 0;
	}

	if(bWhiteResult)
	{
		//주름 Display
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		gen_empty_obj(&sum_rgn);
		erosion_circle(sum_white_rslt_rgn, &sum_rgn, HTuple(distance));

		connection(sum_rgn, &sum_rgn);

		//작은 노이즈 없에기
		select_shape(sum_rgn, &sum_rgn, HTuple("area"), HTuple("and"), HTuple(noise[2]),  HTuple("max"));
		select_shape(sum_rgn, &sum_rgn, HTuple("width"), HTuple("and"), HTuple(noise[0]),  HTuple("max"));
		select_shape(sum_rgn, &sum_rgn, HTuple("height"), HTuple("and"), HTuple(noise[1]),  HTuple("max"));

		connection(sum_rgn, &sum_rgn);

		count_obj(sum_rgn, &Num);

		num = 0;
		num = (Hlong)Num[0];
	
		if(0 < num)
		{
			if(disp)
			{
				set_color(disp, HTuple("red"));
				set_draw(disp, HTuple("margin"));
				disp_obj(sum_rgn, disp);
			}

			*rslt_num = num;
			copy_obj(sum_rgn, rslt_rgn, 1, -1);
			connection(*rslt_rgn, rslt_rgn);
		}
		else
		{

		}

		if(disp)
		{
			set_color(disp, HTuple("blue"));
			set_draw(disp, HTuple("margin"));
			disp_obj(ins_rgn, disp);
		}
	}
	else
	{
		*rslt_num = 0;
	}

	/*if(!bPatResult || !bWhiteResult)
	{
		rslt = FALSE;
	}*/

	return rslt;
}

//20160302 ngh
BOOL CIPSDlg::SetInspectScanCVD(long disp, Hobject *image, BOOL up_direct, int scan_idx, double pixel_size, int thresh, long *rslt_num, double *rslt_x, double *rslt_y)
{
	BOOL rslt = TRUE;
	int cam_idx = 0;
	int i, j;

	typeInspPara insPara;
	typeGrabPara grabPara;

	grabPara = GetGrabPara(scan_idx);
	insPara = GetInsPara(grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx], grabPara.nRotat[grabPara.nCamIdx]);

	//insPara.dPixelSize = 0.005228;

	int pitch;

	HTuple RowEdgeFirstLR, ColumnEdgeFirstLR, RowEdgeSecondLR, ColumnEdgeSecondLR, RowEdgeFirstUD, ColumnEdgeFirstUD, RowEdgeSecondUD, ColumnEdgeSecondUD; 
	HTuple AmplitudeSecond, MeasureHandle, IntraDistance, InterDistance, AmplitudeFirst;
	HTuple wd, ht, row, col, leng;
	Hlong edgeCnt;

	RECT rect;
	Hobject sub_ins_rgn, Region;
	Hobject ImageReduced, EmphaImage, MeanImage;
	Herror herr;
	HTuple Num;

	if(disp)
	{
		//disp_obj(*image, disp);
	}

	*rslt_num = 0;
	
	if(0 >= insPara.nInspectionCVDDwCnt || 0>= insPara.nInspectionCVDUpCnt)
	{
		return FALSE;
	}

	set_check("~give_error");
	herr = get_image_pointer1(*image, NULL, NULL, &wd, &ht);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
		return FALSE;
	}

	int rect_cnt = 0;
	
	if(up_direct)
	{
		rect_cnt = insPara.nInspectionCVDUpCnt;
	}
	else
	{
		rect_cnt = insPara.nInspectionCVDDwCnt;
	}

	for(i = 0; i < rect_cnt; i++)
	{
		if(up_direct)
		{
			rect.left = insPara.InsCVDUpRect[i].dRectLeft;
			rect.top = insPara.InsCVDUpRect[i].dRectTop;
			rect.right = insPara.InsCVDUpRect[i].dRectRight;
			rect.bottom = insPara.InsCVDUpRect[i].dRectBottom;
		}
		else
		{
			rect.left = insPara.InsCVDDwRect[i].dRectLeft;
			rect.top = insPara.InsCVDDwRect[i].dRectTop;
			rect.right = insPara.InsCVDDwRect[i].dRectRight;
			rect.bottom = insPara.InsCVDDwRect[i].dRectBottom;
		}

		//LR
		row = (Hlong)(rect.top);
		col = (Hlong)((rect.right + rect.left) * 0.5);

		leng = (rect.right - rect.left) * 0.5;
		gen_measure_rectangle2(row,col,HTuple(0),leng,50,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);


		if(0 < pixel_size)
		{
			pitch = 1. / pixel_size;
		}
		else
		{
			pitch = 1;
		}

		for(j = 0; (j < rect.bottom) && ((row + pitch) < rect.bottom); j++)
		{

			if(0 < pixel_size)
			{
				pitch = (1. * j) / pixel_size;
			}
			else
			{
				pitch = j;
			}
		
			translate_measure(MeasureHandle, row + pitch, col);

			measure_pairs(*image, MeasureHandle, HTuple(3), HTuple(thresh), HTuple("negative_strongest"), HTuple("all"),
				&RowEdgeFirstLR,&ColumnEdgeFirstLR,&AmplitudeFirst,  
				&RowEdgeSecondLR,&ColumnEdgeSecondLR,&AmplitudeSecond,
				&IntraDistance, &InterDistance);

			edgeCnt = RowEdgeFirstLR.Num();
			if(0 < edgeCnt)
			{
				set_color(disp, HTuple("green"));
				set_draw(disp, HTuple("margin"));

				disp_line(disp, RowEdgeFirstLR[0], ColumnEdgeFirstLR[0], RowEdgeSecondLR[0], ColumnEdgeSecondLR[0]);


				m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][*rslt_num].dRowEdgeFirstLR[!up_direct] = RowEdgeFirstLR[0];
				m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][*rslt_num].dColumnEdgeFirstLR[!up_direct] = ColumnEdgeFirstLR[0];
				m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][*rslt_num].dRowEdgeSecondLR[!up_direct] = RowEdgeSecondLR[0];
				m_CVDRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][*rslt_num].dColumnEdgeSecondLR[!up_direct] = ColumnEdgeSecondLR[0];

				//20160608 ngh
				/*m_dRowEdgeFirstLR[!up_direct][*rslt_num] = RowEdgeFirstLR[0];
				m_dColumnEdgeFirstLR[!up_direct][*rslt_num] = ColumnEdgeFirstLR[0];
				m_RowEdgeSecondLR[!up_direct][*rslt_num] = RowEdgeSecondLR[0];
				m_ColumnEdgeSecondLR[!up_direct][*rslt_num] = ColumnEdgeSecondLR[0];*/

				set_color(disp, HTuple("red"));
				set_draw(disp, HTuple("margin"));

				if(up_direct)
				{
					//위에서서 아래(up) 이미지 상에서는 왼쪽에서 오른쪽
					//RowEdgeFirstLR[0], ColumnEdgeFirstLR[0]

					rslt_x[*rslt_num] = ColumnEdgeFirstLR[0];
					rslt_y[*rslt_num] = RowEdgeFirstLR[0];

					disp_line(disp, rslt_y[*rslt_num] - 5, rslt_x[*rslt_num], rslt_y[*rslt_num] + 5, rslt_x[*rslt_num]);
					disp_line(disp, rslt_y[*rslt_num], rslt_x[*rslt_num] - 5, rslt_y[*rslt_num], rslt_x[*rslt_num] + 5);
				}
				else
				{
					//아래에서 위(down) 이미지 상에서는 오른쪽에서 왼쪽
					//RowEdgeSecondLR[0], ColumnEdgeSecondLR[0]

					rslt_x[*rslt_num] = ColumnEdgeSecondLR[0];
					rslt_y[*rslt_num] = RowEdgeSecondLR[0];

					disp_line(disp, rslt_y[*rslt_num] - 5, rslt_x[*rslt_num], rslt_y[*rslt_num] + 5, rslt_x[*rslt_num]);
					disp_line(disp, rslt_y[*rslt_num], rslt_x[*rslt_num] - 5, rslt_y[*rslt_num], rslt_x[*rslt_num] + 5);
				}

				if(DEFECT_DATA_MAX_NUM < *rslt_num)
				{
					break;
				}
			}
			else
			{
				rslt_x[*rslt_num] = 0.;
				rslt_y[*rslt_num] = 0.;

				rslt = FALSE;
			}

			(*rslt_num)++;
		}
	}
	close_measure(MeasureHandle);

	return rslt;
}

//20160302 ngh
BOOL CIPSDlg::SetInspectScanPCL(long disp, Hobject *image, BOOL up_direct, int scan_idx, double pixel_size, int thresh, long *rslt_num, double *rslt_x, double *rslt_y)
{
	BOOL rslt = TRUE;
	int cam_idx = 0;
	int i, j;

	typeInspPara insPara;
	typeGrabPara grabPara;

	grabPara = GetGrabPara(scan_idx);
	insPara = GetInsPara(grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx], grabPara.nRotat[grabPara.nCamIdx]);

	//insPara.dPixelSize = 0.005228;

	int pitch;

	HTuple RowEdgeFirstLR, ColumnEdgeFirstLR, RowEdgeSecondLR, ColumnEdgeSecondLR, RowEdgeFirstUD, ColumnEdgeFirstUD, RowEdgeSecondUD, ColumnEdgeSecondUD; 
	HTuple AmplitudeSecond, MeasureHandle, IntraDistance, InterDistance, AmplitudeFirst;
	HTuple wd, ht, row, col, leng;
	Hlong edgeCnt;

	RECT rect;
	Hobject sub_ins_rgn, Region;
	Hobject ImageReduced, EmphaImage, MeanImage;
	Herror herr;
	HTuple Num;

	if(disp)
	{
		//disp_obj(*image, disp);
	}

	*rslt_num = 0;
	
	if(0 >= insPara.nInspectionCVDDwCnt || 0>= insPara.nInspectionCVDUpCnt)
	{
		return FALSE;
	}

	set_check("~give_error");
	herr = get_image_pointer1(*image, NULL, NULL, &wd, &ht);
	set_check("give_error");
	if(H_MSG_TRUE == herr)
	{
	}
	else
	{
		AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
		return FALSE;
	}

	int rect_cnt = 0;
	
	if(up_direct)
	{
		rect_cnt = insPara.nInspectionCVDUpCnt;
	}
	else
	{
		rect_cnt = insPara.nInspectionCVDDwCnt;
	}

	for(i = 0; i < rect_cnt; i++)
	{
		if(up_direct)
		{
			rect.left = insPara.InsCVDUpRect[i].dRectLeft;
			rect.top = insPara.InsCVDUpRect[i].dRectTop;
			rect.right = insPara.InsCVDUpRect[i].dRectRight;
			rect.bottom = insPara.InsCVDUpRect[i].dRectBottom;
		}
		else
		{
			rect.left = insPara.InsCVDDwRect[i].dRectLeft;
			rect.top = insPara.InsCVDDwRect[i].dRectTop;
			rect.right = insPara.InsCVDDwRect[i].dRectRight;
			rect.bottom = insPara.InsCVDDwRect[i].dRectBottom;
		}

		//LR
		row = (Hlong)(rect.top);
		col = (Hlong)((rect.right + rect.left) * 0.5);

		leng = (rect.right - rect.left) * 0.5;
		gen_measure_rectangle2(row,col,HTuple(0),leng,50,wd,ht,HTuple("nearest_neighbor"),&MeasureHandle);


		if(0 < pixel_size)
		{
			pitch = 1. / pixel_size;
		}
		else
		{
			pitch = 1;
		}

		for(j = 0; (j < rect.bottom) && ((row + pitch) < rect.bottom); j++)
		{

			if(0 < pixel_size)
			{
				pitch = (1. * j) / pixel_size;
			}
			else
			{
				pitch = j;
			}
		
			translate_measure(MeasureHandle, row + pitch, col);

			measure_pairs(*image, MeasureHandle, HTuple(3), HTuple(thresh), HTuple("negative_strongest"), HTuple("all"),
				&RowEdgeFirstLR,&ColumnEdgeFirstLR,&AmplitudeFirst,  
				&RowEdgeSecondLR,&ColumnEdgeSecondLR,&AmplitudeSecond,
				&IntraDistance, &InterDistance);

			edgeCnt = RowEdgeFirstLR.Num();
			if(0 < edgeCnt)
			{
				set_color(disp, HTuple("green"));
				set_draw(disp, HTuple("margin"));

				disp_line(disp, RowEdgeFirstLR[0], ColumnEdgeFirstLR[0], RowEdgeSecondLR[0], ColumnEdgeSecondLR[0]);


				m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][*rslt_num].dRowEdgeFirstLR[!up_direct] = RowEdgeFirstLR[0];
				m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][*rslt_num].dColumnEdgeFirstLR[!up_direct] = ColumnEdgeFirstLR[0];
				m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][*rslt_num].dRowEdgeSecondLR[!up_direct] = RowEdgeSecondLR[0];
				m_PCLRslt[grabPara.nRotat[grabPara.nCamIdx]][grabPara.nScanIdx[grabPara.nCamIdx]][*rslt_num].dColumnEdgeSecondLR[!up_direct] = ColumnEdgeSecondLR[0];

				//20160608 ngh
				/*m_dRowEdgeFirstLR[!up_direct][*rslt_num] = RowEdgeFirstLR[0];
				m_dColumnEdgeFirstLR[!up_direct][*rslt_num] = ColumnEdgeFirstLR[0];
				m_RowEdgeSecondLR[!up_direct][*rslt_num] = RowEdgeSecondLR[0];
				m_ColumnEdgeSecondLR[!up_direct][*rslt_num] = ColumnEdgeSecondLR[0];*/

				set_color(disp, HTuple("red"));
				set_draw(disp, HTuple("margin"));

				if(up_direct)
				{
					//위에서서 아래(up) 이미지 상에서는 왼쪽에서 오른쪽
					//RowEdgeFirstLR[0], ColumnEdgeFirstLR[0]

					rslt_x[*rslt_num] = ColumnEdgeFirstLR[0];
					rslt_y[*rslt_num] = RowEdgeFirstLR[0];

					disp_line(disp, rslt_y[*rslt_num] - 5, rslt_x[*rslt_num], rslt_y[*rslt_num] + 5, rslt_x[*rslt_num]);
					disp_line(disp, rslt_y[*rslt_num], rslt_x[*rslt_num] - 5, rslt_y[*rslt_num], rslt_x[*rslt_num] + 5);
				}
				else
				{
					//아래에서 위(down) 이미지 상에서는 오른쪽에서 왼쪽
					//RowEdgeSecondLR[0], ColumnEdgeSecondLR[0]

					rslt_x[*rslt_num] = ColumnEdgeSecondLR[0];
					rslt_y[*rslt_num] = RowEdgeSecondLR[0];

					disp_line(disp, rslt_y[*rslt_num] - 5, rslt_x[*rslt_num], rslt_y[*rslt_num] + 5, rslt_x[*rslt_num]);
					disp_line(disp, rslt_y[*rslt_num], rslt_x[*rslt_num] - 5, rslt_y[*rslt_num], rslt_x[*rslt_num] + 5);
				}

				if(DEFECT_DATA_MAX_NUM < *rslt_num)
				{
					break;
				}
			}
			else
			{
				rslt_x[*rslt_num] = 0.;
				rslt_y[*rslt_num] = 0.;

				//rslt = FALSE;
			}

			(*rslt_num)++;
		}
	}
	close_measure(MeasureHandle);

	return rslt;
}


void CIPSDlg::SetSetupPara(typeSetupPara setupPara){m_SetupPara = setupPara;}
void CIPSDlg::SetSystemPara(typeSystemPara systemPara){m_SystemPara = systemPara;}
typeSetupPara CIPSDlg::GetSetupPara(){return m_SetupPara;}
typeSystemPara CIPSDlg::GetSystemPara(){return m_SystemPara;}

void CIPSDlg::SetInsPara(typeInspPara insPara, int cam_idx, int scan_idx, int rotate)
{
	m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1] = insPara;
}

typeInspPara CIPSDlg::GetInsPara(int cam_idx, int scan_idx, int rotate)
{
	return m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1];
}

BOOL CIPSDlg::GetInspection(int scan_idx){return m_bInspection[scan_idx];}
void CIPSDlg::SetInspection(BOOL isInspection, int scan_idx){m_bInspection[scan_idx] = isInspection;}

BOOL CIPSDlg::GetGrabComp(int scan_idx){return m_bGrabComp[scan_idx];}
void CIPSDlg::SetGrabComp(BOOL isComp, int scan_idx){m_bGrabComp[scan_idx] = isComp;}

BOOL CIPSDlg::SaveSystemPara()
{
	CString file_path, temp_path;
	BOOL rslt = TRUE;
	CString key, dat, strTemp;

	strTemp.Format(_T("Save System Parameter"));
	WriteLog(LOG_PATH, strTemp);
	
	//temp_path.Format("%sVT_%02d\\", path, device + 1);
	temp_path.Format(_T("%s"), mRootDirectory);
	//Make director
	MakeDirectories(temp_path);
	
	file_path.Format(_T("%s\\%s"), temp_path, SYSTEM_PARA_FILE_NAME);
	
	//file 존재 확인 
	HANDLE fd=CreateFile(   file_path,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);
		
		//write INI
		key.Empty();
		key.Format(_T("PORT_NUM"));
		dat.Empty();
		dat.Format(_T("%d"), m_SystemPara.nPortNum);
		WritePrivateProfileString(_T("SYSTEM_PARA"), key, dat, file_path);

		key.Empty();
		key.Format(_T("DELETE_DAY"));
		dat.Empty();
		dat.Format(_T("%d"), m_SystemPara.nDay);
		WritePrivateProfileString(_T("SYSTEM_PARA"), key, dat, file_path);

		key.Empty();
		key.Format(_T("DELETE_HOUR"));
		dat.Empty();
		dat.Format(_T("%d"), m_SystemPara.nHour);
		WritePrivateProfileString(_T("SYSTEM_PARA"), key, dat, file_path);

		key.Empty();
		key.Format(_T("DELETE_MINUTE"));
		dat.Empty();
		dat.Format(_T("%d"), m_SystemPara.nMinute);
		WritePrivateProfileString(_T("SYSTEM_PARA"), key, dat, file_path);
	}
	fd = 0;
	return TRUE;
}

BOOL CIPSDlg::LoadSystemPara()
{
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path, temp_path;
	temp_path.Format(_T("%s"), mRootDirectory);
	file_path.Format(_T("%s\\%s"), temp_path, SYSTEM_PARA_FILE_NAME);
	WriteLog(LOG_PATH, _T("Load System Parameter"));
	
	//file 존재 확인 
	HANDLE fd = CreateFile( file_path,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if(((LONG)fd)<0)
	{
		return FALSE;
	}
	else
	{
		CloseHandle(fd);

		key.Empty();
		key.Format(_T("PORT_NUM"));
		m_SystemPara.nPortNum = GetPrivateProfileInt(_T("SYSTEM_PARA"), key, 0, file_path);

		key.Empty();
		key.Format(_T("DELETE_DAY"));
		m_SystemPara.nDay = GetPrivateProfileInt(_T("SYSTEM_PARA"), key, 0, file_path);

		key.Empty();
		key.Format(_T("DELETE_HOUR"));
		m_SystemPara.nHour = GetPrivateProfileInt(_T("SYSTEM_PARA"), key, 0, file_path);

		key.Empty();
		key.Format(_T("DELETE_MINUTE"));
		m_SystemPara.nMinute = GetPrivateProfileInt(_T("SYSTEM_PARA"), key, 0, file_path);
	}
	fd = 0;
	return TRUE;
}

BOOL CIPSDlg::SaveSetupPara()
{
	CString file_path, temp_path;
	BOOL rslt = TRUE;
	CString key, dat, strTemp;

	strTemp.Format(_T("Save Setup Parameter"));
	WriteLog(LOG_PATH, strTemp);
	
	//temp_path.Format("%sVT_%02d\\", path, device + 1);
	temp_path.Format(_T("%s\\%s"), mRootDirectory, m_ModelName);
	//Make director
	MakeDirectories(temp_path);
	
	file_path.Format(_T("%s\\%s"), temp_path, SETUP_PARA_FILE_NAME);
	
	//file 존재 확인 
	HANDLE fd=CreateFile(   file_path,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);
		
		//write INI
		//SetupPara
		key.Empty();
		key.Format(_T("MAX_DEFECT_CNT"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.nMaxDefectCnt);
		WritePrivateProfileString(_T("SETUP_PARA"), key, dat, file_path);

		//8K Long
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_LONG_WIDTH"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n8K_LongWidth);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("8K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_LONG_HEIGHT"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n8K_LongHeight);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("8K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_LONG_DEAFULT_X"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n8K_LongDeafultX);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("8K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_LONG_DEAFULT_Y"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n8K_LongDeafultY);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("8K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_LONG_OFFSET"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n8K_LongOffset);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("8K_SETUP_PARA"), key, dat, file_path);

		//8K short
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_SHORT_WIDTH"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n8K_ShortWidth);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("8K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_SHORT_HEIGHT"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n8K_ShortHeight);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("8K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_SHORT_DEAFULT_X"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n8K_ShortDeafultX);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("8K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_SHORT_DEAFULT_Y"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n8K_ShortDeafultY);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("8K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_SHORT_OFFSET"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n8K_ShortOffset);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("8K_SETUP_PARA"), key, dat, file_path);

		//16K Long
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_LONG_WIDTH"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n16K_LongWidth);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("16K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_LONG_HEIGHT"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n16K_LongHeight);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("16K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_LONG_DEAFULT_X"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n16K_LongDeafultX);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("16K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_LONG_DEAFULT_Y"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n16K_LongDeafultY);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("16K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_LONG_OFFSET"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n16K_LongOffset);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("16K_SETUP_PARA"), key, dat, file_path);

		//16K short
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_SHORT_WIDTH"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n16K_ShortWidth);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("16K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_SHORT_HEIGHT"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n16K_ShortHeight);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("16K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_SHORT_DEAFULT_X"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n16K_ShortDeafultX);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("16K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_SHORT_DEAFULT_Y"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n16K_ShortDeafultY);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("16K_SETUP_PARA"), key, dat, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_SHORT_OFFSET"));
		dat.Empty();
		dat.Format(_T("%d"), m_SetupPara.TeachingPara.n16K_ShortOffset);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(_T("16K_SETUP_PARA"), key, dat, file_path);
	}
	fd=0;

	return TRUE;
}

BOOL CIPSDlg::LoadSetupPara()
{
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path, temp_path;
	temp_path.Format(_T("%s\\%s"), mRootDirectory, m_ModelName);
	file_path.Format(_T("%s\\%s"), temp_path, SETUP_PARA_FILE_NAME);
	WriteLog(LOG_PATH, _T("Load Setup Parameter"));
	
	//file 존재 확인 
	HANDLE fd = CreateFile( file_path,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if(((LONG)fd)<0)
	{
		return FALSE;
	}
	else
	{
		CloseHandle(fd);

		key.Empty();
		key.Format(_T("MAX_DEFECT_CNT"));
		m_SetupPara.nMaxDefectCnt = GetPrivateProfileInt(_T("SETUP_PARA"), key, 0, file_path);

		//8K Long
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_LONG_WIDTH"));
		m_SetupPara.TeachingPara.n8K_LongWidth = GetPrivateProfileInt(_T("8K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_LONG_HEIGHT"));
		m_SetupPara.TeachingPara.n8K_LongHeight = GetPrivateProfileInt(_T("8K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_LONG_DEAFULT_X"));
		m_SetupPara.TeachingPara.n8K_LongDeafultX = GetPrivateProfileInt(_T("8K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_LONG_DEAFULT_Y"));
		m_SetupPara.TeachingPara.n8K_LongDeafultY = GetPrivateProfileInt(_T("8K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_LONG_OFFSET"));
		m_SetupPara.TeachingPara.n8K_LongOffset = GetPrivateProfileInt(_T("8K_SETUP_PARA"), key, 0, file_path);
		//8K Short
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_SHORT_WIDTH"));
		m_SetupPara.TeachingPara.n8K_ShortWidth = GetPrivateProfileInt(_T("8K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_SHORT_HEIGHT"));
		m_SetupPara.TeachingPara.n8K_ShortHeight = GetPrivateProfileInt(_T("8K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_SHORT_DEAFULT_X"));
		m_SetupPara.TeachingPara.n8K_ShortDeafultX = GetPrivateProfileInt(_T("8K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_SHORT_DEAFULT_Y"));
		m_SetupPara.TeachingPara.n8K_ShortDeafultY = GetPrivateProfileInt(_T("8K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("8K_SETUP_TEACHING_PARA_SHORT_OFFSET"));
		m_SetupPara.TeachingPara.n8K_ShortOffset = GetPrivateProfileInt(_T("8K_SETUP_PARA"), key, 0, file_path);

		//16K Long
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_LONG_WIDTH"));
		m_SetupPara.TeachingPara.n16K_LongWidth = GetPrivateProfileInt(_T("16K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_LONG_HEIGHT"));
		m_SetupPara.TeachingPara.n16K_LongHeight = GetPrivateProfileInt(_T("16K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_LONG_DEAFULT_X"));
		m_SetupPara.TeachingPara.n16K_LongDeafultX = GetPrivateProfileInt(_T("16K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_LONG_DEAFULT_Y"));
		m_SetupPara.TeachingPara.n16K_LongDeafultY = GetPrivateProfileInt(_T("16K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_LONG_OFFSET"));
		m_SetupPara.TeachingPara.n16K_LongOffset = GetPrivateProfileInt(_T("16K_SETUP_PARA"), key, 0, file_path);
		//16K Short
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_SHORT_WIDTH"));
		m_SetupPara.TeachingPara.n16K_ShortWidth = GetPrivateProfileInt(_T("16K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_SHORT_HEIGHT"));
		m_SetupPara.TeachingPara.n16K_ShortHeight = GetPrivateProfileInt(_T("16K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_SHORT_DEAFULT_X"));
		m_SetupPara.TeachingPara.n16K_ShortDeafultX = GetPrivateProfileInt(_T("16K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_SHORT_DEAFULT_Y"));
		m_SetupPara.TeachingPara.n16K_ShortDeafultY = GetPrivateProfileInt(_T("16K_SETUP_PARA"), key, 0, file_path);
		key.Empty();
		key.Format(_T("16K_SETUP_TEACHING_PARA_SHORT_OFFSET"));
		m_SetupPara.TeachingPara.n16K_ShortOffset = GetPrivateProfileInt(_T("16K_SETUP_PARA"), key, 0, file_path);
	}
	fd = 0;
	return TRUE;
}

BOOL CIPSDlg::SaveInspPara(int cam_idx, int rotate, int scan_idx)
{
	BOOL rslt = TRUE;
	CString key, dat;
	CString strTemp, file_path, temp_path, strCam, strApp;
	int cnt = 0;
	temp_path.Format(_T("%s\\%s"), mRootDirectory, m_ModelName);
	MakeDirectories(temp_path);
	
	if(cam_idx == CAM_8K)
	{
		file_path.Format(_T("%s\\8K_%d_%d_%s"), temp_path, rotate, scan_idx, INSP_PARA_FILE_NAME);
		strCam.Format(_T("8K"));
		strApp.Format(_T("8K_INSP_PARA"));
	}
	else
	{
#ifdef PCL_TEST
		if(cam_idx == CAM_16K)
		{
			file_path.Format(_T("%s\\16K_%d_%d_CVD_%s"), temp_path, rotate, scan_idx, INSP_PARA_FILE_NAME);
		}
		else
		{
			file_path.Format(_T("%s\\16K_%d_%d_PCL_%s"), temp_path, rotate, scan_idx, INSP_PARA_FILE_NAME);
		}
#else
		file_path.Format(_T("%s\\16K_%d_%d_%s"), temp_path, rotate, scan_idx, INSP_PARA_FILE_NAME);
#endif
		
		strCam.Format(_T("16K"));
		strApp.Format(_T("16K_INSP_PARA"));
	}

	if(cam_idx > CAM_8K)
	{
		switch(cam_idx)
		{
		case 1:
			strTemp.Format(_T("Cam %d Rotate %d Scan %d CVD Inspection Parameter Save"), cam_idx, rotate, scan_idx);
			break;
		case 2:
			strTemp.Format(_T("Cam %d Rotate %d Scan %d PCL Inspection Parameter Save"), cam_idx, rotate, scan_idx);
			break;
		}
	}
	else
	{
		strTemp.Format(_T("Cam %d Rotate %d Scan %d Inspection Parameter Save"), cam_idx, rotate, scan_idx);
	}
	WriteLog(LOG_PATH, strTemp);

//file 존재 확인 
	HANDLE fd=CreateFile(   file_path,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);
		
		//write INI
		//8K
		for(int i = 0; i < MAX_INS_PARA; i++)
		{
			key.Empty();
			key.Format(_T("%s_INSP_PARA_THRESH_HOLD_%d"), strCam, i);
			dat.Empty();
			dat.Format(_T("%.4f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].dThreshHold[i]);//pdlg->m_cImageView[device]->m_bOnLineMode);
			WritePrivateProfileString(strApp, key, dat, file_path);

			key.Empty();
			key.Format(_T("%s_INSP_PARA_DEFECT_SIZE_%d"), strCam, i);
			dat.Empty();
			dat.Format(_T("%d"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nDefectSize[i]);//pdlg->m_cImageView[device]->m_bOnLineMode);
			WritePrivateProfileString(strApp, key, dat, file_path);
		}
		//20160318 ngh
		for(int i = 0; i < MAX_INS_PARA; i++)
		{
			key.Empty();
			key.Format(_T("%s_INSP_PARA_NOISE_SIZE_%d"), strCam, i);
			dat.Empty();
			dat.Format(_T("%d"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nNoiseSize[i]);//pdlg->m_cImageView[device]->m_bOnLineMode);
			WritePrivateProfileString(strApp, key, dat, file_path);
		}
		//20160318 ngh
		key.Empty();
		key.Format(_T("%s_INSP_PARA_MARGE_DISTANCE"), strCam);
		dat.Empty();
		dat.Format(_T("%f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].dMargeDistance[0]);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(strApp, key, dat, file_path);

		key.Empty();
		key.Format(_T("%s_INSP_PARA_INSPECTION_CNT"), strCam);
		dat.Empty();
		dat.Format(_T("%d"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCnt);//pdlg->m_cImageView[device]->m_bOnLineMode);
		WritePrivateProfileString(strApp, key, dat, file_path);

		for(int i = 0; i < m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCnt; i++)
		{
			key.Empty();
			key.Format(_T("%s_INSP_PARA_RECT_LEFT_%d"), strCam, i);
			dat.Empty();
			dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[i].dRectLeft);//pdlg->m_cImageView[device]->m_bOnLineMode);
			WritePrivateProfileString(strApp, key, dat, file_path);

			key.Empty();
			key.Format(_T("%s_INSP_PARA_RECT_TOP_%d"), strCam, i);
			dat.Empty();
			dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[i].dRectTop);//pdlg->m_cImageView[device]->m_bOnLineMode);
			WritePrivateProfileString(strApp, key, dat, file_path);

			key.Empty();
			key.Format(_T("%s_INSP_PARA_RECT_RIGHT_%d"), strCam, i);
			dat.Empty();
			dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[i].dRectRight);//pdlg->m_cImageView[device]->m_bOnLineMode);
			WritePrivateProfileString(strApp, key, dat, file_path);

			key.Empty();
			key.Format(_T("%s_INSP_PARA_RECT_BOTTOM_%d"), strCam, i);
			dat.Empty();
			dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[i].dRectBottom);//pdlg->m_cImageView[device]->m_bOnLineMode);
			WritePrivateProfileString(strApp, key, dat, file_path);
			cnt++;
			if((m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[i].dRectLeft == 0) &&
				(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[i].dRectTop == 0) &&
				(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[i].dRectRight == 0) &&
				(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[i].dRectBottom == 0))
			{
				break;
			}
		}

		for(int i = 0; i < MAX_INS_PARA; i++)
		{
			//key.Empty();
			//key.Format(_T("%s_INSP_PARA_TEST0_%d"), strCam, i);
			//dat.Empty();
			//dat.Format(_T("%f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].ImageFilterPara[i].dMeen1);//pdlg->m_cImageView[device]->m_bOnLineMode);
			//WritePrivateProfileString(strApp, key, dat, file_path);

			//key.Empty();
			//key.Format(_T("%s_INSP_PARA_TEST1_%d"), strCam, i);
			//dat.Empty();
			//dat.Format(_T("%f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].ImageFilterPara[i].dMeen2);//pdlg->m_cImageView[device]->m_bOnLineMode);
			//WritePrivateProfileString(strApp, key, dat, file_path);

			//key.Empty();
			//key.Format(_T("%s_INSP_PARA_TEST2_%d"), strCam, i);
			//dat.Empty();
			//dat.Format(_T("%f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].ImageFilterPara[i].dEmpha1);//pdlg->m_cImageView[device]->m_bOnLineMode);
			//WritePrivateProfileString(strApp, key, dat, file_path);

			//key.Empty();
			//key.Format(_T("%s_INSP_PARA_TEST3_%d"), strCam, i);
			//dat.Empty();
			//dat.Format(_T("%f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].ImageFilterPara[i].dEmpha2);//pdlg->m_cImageView[device]->m_bOnLineMode);
			//WritePrivateProfileString(strApp, key, dat, file_path);

			key.Empty();
			key.Format(_T("%s_INSP_PARA_FACTOR_%d"), strCam, i);
			dat.Empty();
			dat.Format(_T("%f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].ImageFilterPara[i].dFactor);//pdlg->m_cImageView[device]->m_bOnLineMode);
			WritePrivateProfileString(strApp, key, dat, file_path);
		}


		if(cam_idx >= CAM_16K)
		{
			key.Empty();
			key.Format(_T("%s_INSP_PARA_PIXEL_SIZE"), strCam);
			dat.Empty();
			dat.Format(_T("%.f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].dPixelSize);//pdlg->m_cImageView[device]->m_bOnLineMode);
			WritePrivateProfileString(strApp, key, dat, file_path);

			if(cam_idx == CAM_16K)
			{
				key.Empty();
				key.Format(_T("%s_INSP_PARA_INSPECTION_CVD_UP_CNT"), strCam);
				dat.Empty();
				dat.Format(_T("%d"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDUpCnt);//pdlg->m_cImageView[device]->m_bOnLineMode);
				WritePrivateProfileString(strApp, key, dat, file_path);

				for(int i = 0; i < m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDUpCnt; i++)
				{
					key.Empty();
					key.Format(_T("%s_INSP_CVD_UP_PARA_RECT_LEFT_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectLeft);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);

					key.Empty();
					key.Format(_T("%s_INSP_CVD_UP_PARA_RECT_TOP_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectTop);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);

					key.Empty();
					key.Format(_T("%s_INSP_CVD_UP_PARA_RECT_RIGHT_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectRight);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);

					key.Empty();
					key.Format(_T("%s_INSP_CVD_UP_PARA_RECT_BOTTOM_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectBottom);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);
					cnt++;
					if((m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectLeft == 0) &&
						(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectTop == 0) &&
						(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectRight == 0) &&
						(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectBottom == 0))
					{
						break;
					}
				}

				key.Empty();
				key.Format(_T("%s_INSP_PARA_INSPECTION_CVD_DW_CNT"), strCam);
				dat.Empty();
				dat.Format(_T("%d"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDDwCnt);//pdlg->m_cImageView[device]->m_bOnLineMode);
				WritePrivateProfileString(strApp, key, dat, file_path);

				for(int i = 0; i < m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDDwCnt; i++)
				{
					key.Empty();
					key.Format(_T("%s_INSP_CVD_DW_PARA_RECT_LEFT_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectLeft);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);

					key.Empty();
					key.Format(_T("%s_INSP_CVD_DW_PARA_RECT_TOP_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectTop);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);

					key.Empty();
					key.Format(_T("%s_INSP_CVD_DW_PARA_RECT_RIGHT_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectRight);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);

					key.Empty();
					key.Format(_T("%s_INSP_CVD_DW_PARA_RECT_BOTTOM_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectBottom);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);
					cnt++;
					if((m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectLeft == 0) &&
						(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectTop == 0) &&
						(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectRight == 0) &&
						(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectBottom == 0))
					{
						break;
					}
				}
			}
			else
			{
				key.Empty();
				key.Format(_T("%s_INSP_PARA_INSPECTION_PCL_UP_CNT"), strCam);
				dat.Empty();
				dat.Format(_T("%d"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDUpCnt);//pdlg->m_cImageView[device]->m_bOnLineMode);
				WritePrivateProfileString(strApp, key, dat, file_path);

				for(int i = 0; i < m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDUpCnt; i++)
				{
					key.Empty();
					key.Format(_T("%s_INSP_PCL_UP_PARA_RECT_LEFT_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectLeft);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);

					key.Empty();
					key.Format(_T("%s_INSP_PCL_UP_PARA_RECT_TOP_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectTop);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);

					key.Empty();
					key.Format(_T("%s_INSP_PCL_UP_PARA_RECT_RIGHT_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectRight);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);

					key.Empty();
					key.Format(_T("%s_INSP_PCL_UP_PARA_RECT_BOTTOM_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectBottom);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);
					cnt++;
					if((m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectLeft == 0) &&
						(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectTop == 0) &&
						(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectRight == 0) &&
						(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectBottom == 0))
					{
						break;
					}
				}

				key.Empty();
				key.Format(_T("%s_INSP_PARA_INSPECTION_PCL_DW_CNT"), strCam);
				dat.Empty();
				dat.Format(_T("%d"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDDwCnt);//pdlg->m_cImageView[device]->m_bOnLineMode);
				WritePrivateProfileString(strApp, key, dat, file_path);

				for(int i = 0; i < m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDDwCnt; i++)
				{
					key.Empty();
					key.Format(_T("%s_INSP_PCL_DW_PARA_RECT_LEFT_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectLeft);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);

					key.Empty();
					key.Format(_T("%s_INSP_PCL_DW_PARA_RECT_TOP_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectTop);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);

					key.Empty();
					key.Format(_T("%s_INSP_PCL_DW_PARA_RECT_RIGHT_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectRight);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);

					key.Empty();
					key.Format(_T("%s_INSP_PCL_DW_PARA_RECT_BOTTOM_%d"), strCam, i);
					dat.Empty();
					dat.Format(_T("%.0f"), m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectBottom);//pdlg->m_cImageView[device]->m_bOnLineMode);
					WritePrivateProfileString(strApp, key, dat, file_path);
					cnt++;
					if((m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectLeft == 0) &&
						(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectTop == 0) &&
						(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectRight == 0) &&
						(m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectBottom == 0))
					{
						break;
					}
				}
			}
		}			
	}
	fd = 0;

	return TRUE;
}

BOOL CIPSDlg::LoadInspPara(int cam_idx, int rotate, int scan_idx)
{
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path, temp_path, strCam, strApp;
	char temp[256];
	CString strTemp;
	temp_path.Format(_T("%s\\%s"), mRootDirectory, m_ModelName);
	if(cam_idx == CAM_8K)
	{
		file_path.Format(_T("%s\\8K_%d_%d_%s"), temp_path, rotate, scan_idx, INSP_PARA_FILE_NAME);
		strCam.Format(_T("8K"));
		strApp.Format(_T("8K_INSP_PARA"));
	}
	else
	{
#ifdef PCL_TEST
		if(cam_idx == CAM_16K)
		{
			file_path.Format(_T("%s\\16K_%d_%d_CVD_%s"), temp_path, rotate, scan_idx, INSP_PARA_FILE_NAME);
		}
		else
		{
			file_path.Format(_T("%s\\16K_%d_%d_PCL_%s"), temp_path, rotate, scan_idx, INSP_PARA_FILE_NAME);
		}
#else
		file_path.Format(_T("%s\\16K_%d_%d_%s"), temp_path, rotate, scan_idx, INSP_PARA_FILE_NAME);
#endif
		
		strCam.Format(_T("16K"));
		strApp.Format(_T("16K_INSP_PARA"));
	}

	if(cam_idx > CAM_8K)
	{
		switch(cam_idx)
		{
		case 1:
			strTemp.Format(_T("Cam %d Rotate %d Scan %d CVD Inspection Parameter Load"), cam_idx, rotate, scan_idx);
			break;
		case 2:
			strTemp.Format(_T("Cam %d Rotate %d Scan %d PCL Inspection Parameter Load"), cam_idx, rotate, scan_idx);
			break;
		}
	}
	else
	{
		strTemp.Format(_T("Cam %d Rotate %d Scan %d Inspection Parameter Load"), cam_idx, rotate, scan_idx);
	}
	WriteLog(LOG_PATH, strTemp);
	

	//file 존재 확인 
	HANDLE fd = CreateFile( file_path,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if(((LONG)fd)<0)
	{
		return FALSE;
	}
	else
	{
		CloseHandle(fd);

		for(int i = 0; i < MAX_INS_PARA; i++)
		{
			//8K Long
			key.Empty();
			key.Format(_T("%s_INSP_PARA_THRESH_HOLD_%d"), strCam, i);
			GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
			dat.Format(_T("%s"), temp);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].dThreshHold[i] = _wtof(dat);

			key.Empty();
			key.Format(_T("%s_INSP_PARA_DEFECT_SIZE_%d"), strCam, i);
			GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
			dat.Format(_T("%s"), temp);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nDefectSize[i] = _wtoi(dat);
		}
		//20160318 ngh
		for(int i = 0; i < MAX_INS_PARA; i++)
		{
			//8K Long
			key.Empty();
			key.Format(_T("%s_INSP_PARA_NOISE_SIZE_%d"), strCam, i);
			GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
			dat.Format(_T("%s"), temp);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nNoiseSize[i] = _wtoi(dat);
		}
		//20160318 ngh
		key.Empty();
		key.Format(_T("%s_INSP_PARA_MARGE_DISTANCE"), strCam);
		GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
		dat.Format(_T("%s"), temp);
		m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].dMargeDistance[0] = _wtof(dat);

		key.Empty();
		key.Format(_T("%s_INSP_PARA_INSPECTION_CNT"), strCam);
		GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
		dat.Format(_T("%s"), temp);
		m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCnt = _wtoi(dat);

		for(int i = 0; i < m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCnt; i++)
		{
			key.Empty();
			key.Format(_T("%s_INSP_PARA_RECT_LEFT_%d"), strCam, i);
			GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
			dat.Format(_T("%s"), temp);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[i].dRectLeft = _wtof(dat);

			key.Empty();
			key.Format(_T("%s_INSP_PARA_RECT_TOP_%d"), strCam, i);
			GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
			dat.Format(_T("%s"), temp);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[i].dRectTop = _wtof(dat);

			key.Empty();
			key.Format(_T("%s_INSP_PARA_RECT_RIGHT_%d"), strCam, i);
			GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
			dat.Format(_T("%s"), temp);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[i].dRectRight = _wtof(dat);

			key.Empty();
			key.Format(_T("%s_INSP_PARA_RECT_BOTTOM_%d"), strCam, i);
			GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
			dat.Format(_T("%s"), temp);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[i].dRectBottom = _wtof(dat);
		}

		for(int i = 0; i < MAX_INS_PARA; i++)
		{
			/*key.Empty();
			key.Format(_T("%s_INSP_PARA_TEST0_%d"), strCam, i);
			GetPrivateProfileString(strApp, key, _T("0.0"), (LPWSTR)temp, 256, file_path);
			dat.Format(_T("%s"), temp);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].ImageFilterPara[i].dMeen1 = _wtof(dat);

			key.Empty();
			key.Format(_T("%s_INSP_PARA_TEST1_%d"), strCam, i);
			GetPrivateProfileString(strApp, key, _T("0.0"), (LPWSTR)temp, 256, file_path);
			dat.Format(_T("%s"), temp);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].ImageFilterPara[i].dMeen2 = _wtof(dat);

			key.Empty();
			key.Format(_T("%s_INSP_PARA_TEST2_%d"), strCam, i);
			GetPrivateProfileString(strApp, key, _T("0.0"), (LPWSTR)temp, 256, file_path);
			dat.Format(_T("%s"), temp);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].ImageFilterPara[i].dEmpha1 = _wtof(dat);

			key.Empty();
			key.Format(_T("%s_INSP_PARA_TEST3_%d"), strCam, i);
			GetPrivateProfileString(strApp, key, _T("0.0"), (LPWSTR)temp, 256, file_path);
			dat.Format(_T("%s"), temp);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].ImageFilterPara[i].dEmpha2 = _wtof(dat);*/

			key.Empty();
			key.Format(_T("%s_INSP_PARA_FACTOR_%d"), strCam, i);
			GetPrivateProfileString(strApp, key, _T("0.0"), (LPWSTR)temp, 256, file_path);
			dat.Format(_T("%s"), temp);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].ImageFilterPara[i].dFactor = _wtof(dat);
		}

		if(cam_idx >= CAM_16K)
		{
			key.Empty();
			key.Format(_T("%s_INSP_PARA_PIXEL_SIZE"), strCam);
			GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
			
			dat.Format(_T("%s"), temp);//pdlg->m_cImageView[device]->m_bOnLineMode);
			m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].dPixelSize = _wtof(dat);

			if(cam_idx == CAM_16K)
			{
				key.Empty();
				key.Format(_T("%s_INSP_PARA_INSPECTION_CVD_UP_CNT"), strCam);
				GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
				dat.Format(_T("%s"), temp);
				m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDUpCnt = _wtoi(dat);

				for(int i = 0; i < m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDUpCnt; i++)
				{
					key.Empty();
					key.Format(_T("%s_INSP_CVD_UP_PARA_RECT_LEFT_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectLeft = _wtof(dat);

					key.Empty();
					key.Format(_T("%s_INSP_CVD_UP_PARA_RECT_TOP_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectTop = _wtof(dat);

					key.Empty();
					key.Format(_T("%s_INSP_CVD_UP_PARA_RECT_RIGHT_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectRight = _wtof(dat);

					key.Empty();
					key.Format(_T("%s_INSP_CVD_UP_PARA_RECT_BOTTOM_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectBottom = _wtof(dat);
				}

				key.Empty();
				key.Format(_T("%s_INSP_PARA_INSPECTION_CVD_DW_CNT"), strCam);
				GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
				dat.Format(_T("%s"), temp);
				m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDDwCnt = _wtoi(dat);

				for(int i = 0; i < m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDDwCnt; i++)
				{
					key.Empty();
					key.Format(_T("%s_INSP_CVD_DW_PARA_RECT_LEFT_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectLeft = _wtof(dat);

					key.Empty();
					key.Format(_T("%s_INSP_CVD_DW_PARA_RECT_TOP_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectTop = _wtof(dat);

					key.Empty();
					key.Format(_T("%s_INSP_CVD_DW_PARA_RECT_RIGHT_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectRight = _wtof(dat);

					key.Empty();
					key.Format(_T("%s_INSP_CVD_DW_PARA_RECT_BOTTOM_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectBottom = _wtof(dat);
				}
			}
			else
			{
				key.Empty();
				key.Format(_T("%s_INSP_PARA_INSPECTION_PCL_UP_CNT"), strCam);
				GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
				dat.Format(_T("%s"), temp);
				m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDUpCnt = _wtoi(dat);

				for(int i = 0; i < m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDUpCnt; i++)
				{
					key.Empty();
					key.Format(_T("%s_INSP_PCL_UP_PARA_RECT_LEFT_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectLeft = _wtof(dat);

					key.Empty();
					key.Format(_T("%s_INSP_PCL_UP_PARA_RECT_TOP_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectTop = _wtof(dat);

					key.Empty();
					key.Format(_T("%s_INSP_PCL_UP_PARA_RECT_RIGHT_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectRight = _wtof(dat);

					key.Empty();
					key.Format(_T("%s_INSP_PCL_UP_PARA_RECT_BOTTOM_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDUpRect[i].dRectBottom = _wtof(dat);
				}

				key.Empty();
				key.Format(_T("%s_INSP_PARA_INSPECTION_PCL_DW_CNT"), strCam);
				GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
				dat.Format(_T("%s"), temp);
				m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDDwCnt = _wtoi(dat);

				for(int i = 0; i < m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCVDDwCnt; i++)
				{
					key.Empty();
					key.Format(_T("%s_INSP_PCL_DW_PARA_RECT_LEFT_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectLeft = _wtof(dat);

					key.Empty();
					key.Format(_T("%s_INSP_PCL_DW_PARA_RECT_TOP_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectTop = _wtof(dat);

					key.Empty();
					key.Format(_T("%s_INSP_PCL_DW_PARA_RECT_RIGHT_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectRight = _wtof(dat);

					key.Empty();
					key.Format(_T("%s_INSP_PCL_DW_PARA_RECT_BOTTOM_%d"), strCam, i);
					GetPrivateProfileString(strApp, key, _T("0"), (LPWSTR)temp, 256, file_path);
					dat.Format(_T("%s"), temp);
					m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsCVDDwRect[i].dRectBottom = _wtof(dat);
				}
			}
		}
	}
	fd = 0;

	return TRUE;
}

void CIPSDlg::InitInspectionPara(int cam_idx, int scan_idx, int rotate)
{
	m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nInspectionCnt = 0;
	
	for(int i = 0; i < MAX_INS_PARA; i++)
	{
		m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nParaCnt[i] = 5;
		m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].dThreshHold[i] = 0;
		m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].nDefectSize[i] = 0;
	}
	for(int j = 0; j < MAX_INS_RECT_CNT; j++)
	{
		m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[j].dRectLeft = 0.;
		m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[j].dRectTop = 0.;
		m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[j].dRectRight = 0.;
		m_InsPara[((scan_idx + 1) * (cam_idx + 1) * (rotate + 1)) - 1].InsRect[j].dRectBottom = 0.;
	}
}

BOOL CIPSDlg::WriteLog(CString path, CString log, BOOL isList)
{
	FILE*	stream;
	char	strFile[MAX_PATH], strToFile[MAX_PATH];
    long	result, line;
	char	szTime[DAT_STR_LENG]; 
	CString bak_file, filePath, fileName, log_data;
	char buffer[DAT_STR_LENG];
	char temp[DAT_STR_LENG];
	CTime time;
	time = CTime::GetCurrentTime();
	int limit_line = 0;
	
	//path.Empty();	
	bak_file.Format(_T("Log.bak"));

	log_data.Format(_T("%02d:%02d:%02d - %s"), time.GetHour(), time.GetMinute(), time.GetSecond(), log);
	sprintf_s(temp, path.GetLength() + 1,  "%S", path);
	MakeDirectories((LPCWSTR)temp);

	fileName.Format(_T("%04d%02d%02d.log"), time.GetYear(), time.GetMonth(), time.GetDay());
	filePath.Format(_T("%s\\%s"),path, fileName);
	 
	sprintf_s(strFile, filePath.GetLength() + 1, "%S", filePath);

	if((stream = _fsopen( strFile, "a+" , _SH_DENYNO)) == NULL)
	{
		HANDLE fd = CreateFile( (LPCWSTR)(LPCSTR)strFile,
							GENERIC_READ|GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
		stream = _fsopen( strFile, "a+" , _SH_DENYNO);
		if(stream == NULL)
		{
			return FALSE;
		}

		CloseHandle(fd);
	}

	//Check first time
    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}
	line = 0;
	while(fgets(buffer, MAX_STRING_LINE, stream)) 
	{
		line++;
		if(1 < line)
		{
			break;
		}
	}

    result = fseek(stream, 0L, SEEK_END);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	memset(szTime, NULL, sizeof(szTime));
	sprintf_s(szTime, log_data.GetLength() + 1, "%S", log_data);
    fputs(szTime, stream);
	fputs("\n", stream);

    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	//Check limit line
	line = 0;
	if(limit_line)
	{
		while(fgets(buffer, MAX_STRING_LINE, stream)) line++;
		if(line > limit_line)
		{
			sprintf_s(strToFile, filePath.GetLength() + bak_file.GetLength() + 2 + 1, "%S\\%S", path, bak_file);
			CopyFile((LPCWSTR)(LPCSTR)strFile, (LPCWSTR)(LPCSTR)strToFile, FALSE);
			fclose(stream);
			DeleteFile((LPCWSTR)(LPCSTR)strFile);
		}
		else
		{
			fclose(stream);
		}
	}
	else
	{
		fclose(stream);
	}

	return TRUE;
}

//20160526 ngh
BOOL CIPSDlg::CheckDeleteDate()
{
	CString str;
	CTime now_time = CTime::GetCurrentTime();
	int temp_year, temp_month, temp_day;
	CTime delete_time;
	int TempDeleteDay=0;
	int i=0;
	temp_year=0;
	temp_month=0;
	temp_day=0;
	typeSystemPara systemPara;

	temp_day = now_time.GetDay();
	temp_month = now_time.GetMonth();
	temp_year = now_time.GetYear();

	systemPara = GetSystemPara();
 
	int save_day = systemPara.nDay;
	if( (temp_day - save_day) < 0)
	{
		if(now_time.GetMonth() == 2 || now_time.GetMonth() == 4 || now_time.GetMonth() == 6 ||
			now_time.GetMonth() == 8 || now_time.GetMonth() == 9 || now_time.GetMonth() == 11)
		{
			m_iDel_Year = now_time.GetYear();
			m_iDel_Month = now_time.GetMonth() - 1;
			m_iDel_RecentDay = 31 - (save_day - temp_day);
		}
		else if(now_time.GetMonth() == 5 || now_time.GetMonth() == 7 || now_time.GetMonth() == 10 || now_time.GetMonth() == 12)
		{
			m_iDel_Year = now_time.GetYear();
			m_iDel_Month = now_time.GetMonth() - 1;
			m_iDel_RecentDay = 30 - (save_day - temp_day);
		}
		else if(now_time.GetMonth() == 3)
		{
			m_iDel_Year = now_time.GetYear();
			m_iDel_Month = now_time.GetMonth() - 1;
			m_iDel_RecentDay = 28 - (save_day - temp_day);
		}
		else if(now_time.GetMonth() == 1)
		{
			m_iDel_Year = now_time.GetYear() - 1;
			m_iDel_Month = 12;
			m_iDel_RecentDay = 31 - (save_day - temp_day);
		}
	}
	else if( (temp_day - save_day) == 0)
	{
		if(now_time.GetMonth() == 1)
		{
			m_iDel_Year = now_time.GetYear() - 1;
			m_iDel_Month = 12;
			m_iDel_RecentDay = 0;
		}
		else
		{
			m_iDel_Year = now_time.GetYear();
			m_iDel_Month = now_time.GetMonth() - 1;
			m_iDel_RecentDay = 0;
		}
	}
	else
	{
		m_iDel_Year = now_time.GetYear();
		m_iDel_Month = now_time.GetMonth();
		m_iDel_RecentDay = temp_day - save_day;
	}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  저장 기간을 개월로 했을 경우
	int save_month = m_dlgSetup->m_SysPara.m_iPara[0];
	if( (temp_month - save_month) < 0)
	{
		m_iDel_Year = now_time.GetYear() - 1;

		for(i = 0; i <= (save_month - temp_month); i++)
		{
			if(temp_month == i+1)
			{
				m_iDel_Month = 12 - (save_month - temp_month);
			}
		}
	}
	else if( (temp_month - save_month) == 0)
	{
		m_iDel_Year = now_time.GetYear() - 1;

		m_iDel_Month = 12;
	}
	else
	{
		m_iDel_Year = now_time.GetYear();

		m_iDel_Month = temp_month - save_month;
	}
*/// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/*  //저장 기간 3개월 일 경우
	if( (temp_month - 2) <= 0)
	{
		m_iDel_Year = now_time.GetYear() - 1;

		if(temp_month == 1)
			m_iDel_Month = 10;
		else if(temp_month == 2)
			m_iDel_Month = 11;
	}
	else
	{
		m_iDel_Year = now_time.GetYear();

		if(temp_month == 3)
		{
			m_iDel_Year = now_time.GetYear() - 1;
			m_iDel_Month = 12;
		}
		else
			m_iDel_Month = temp_month - 3;
	}
	*/
	return TRUE;
}

BOOL CIPSDlg::DeleteFolderSearch(CString strFolder)
{
	CFileFind ff;
	CString strTarget = strFolder;
	CStringArray strFileArray;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}
	
	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				return FALSE;
			}
		}
	}
	ff.Close();

	return TRUE;

}

BOOL CIPSDlg::DeleteFolder(CString strFolder)
{
	CFileFind ff;
	CString strTarget = strFolder;
	CStringArray strFileArray;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}
	
	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				DeleteFolder(Filename);
				RemoveDirectory(Filename);
			}
			else
			{
				DeleteFile(Filename);
			}
		}
	}
	ff.Close();

	return TRUE;
}

BOOL CIPSDlg::StartAutoDelete_Img()
{
	CString Delete_Img_dir,Delete_Data_dir,Delete_Ng_Data_dir,Delete_Tab_Data_dir;
	CString Delete_Product_dir,Delete_Spc_dir;
	CTime now_time = CTime::GetCurrentTime();
	BOOL bDelFolder[31];
	typeSystemPara systemPara;
	systemPara = GetSystemPara();

	m_strImgPath.Format(_T("%s"), DEFAULT_RESULT_PATH);;
	
	CheckDeleteDate();

	int nYear = m_iDel_Year;
	int nDelYear = now_time.GetYear();

	if(1 == now_time.GetMonth() && now_time.GetDay() < m_iDel_RecentDay)
	{
		nDelYear = m_iDel_Year;
	}
	else if(1 == now_time.GetMonth() && now_time.GetDay() > m_iDel_RecentDay)
	{
		nDelYear = m_iDel_Year - 1;
	}

	for(int nYear = m_iDel_Year; nYear >= nDelYear; nYear--)		// 110930 ytlee
	{
		if(nYear != now_time.GetYear())
		{
			m_iDel_Month = 12;
			m_iDel_Day = m_iDel_RecentDay;		// 120104 ytlee
		}

		for(int nMonth = m_iDel_Month; nMonth > 0; nMonth--)
		{
			if(nMonth == 1 || nMonth == 3 || nMonth == 5 ||
				nMonth == 7 || nMonth == 8 || nMonth == 10 || nMonth == 12)		// 120104 ytlee
			{
				m_iDel_Day = 31;
			}
			else if(nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11)
			{
				m_iDel_Day = 30;
			}
			else if(nMonth == 2)
			{
				m_iDel_Day = 28;
			}
			if(nYear == now_time.GetYear() && (nMonth == now_time.GetMonth() || systemPara.nDay > now_time.GetDay()))		// 110901 ytlee
			{
				m_iDel_Day = m_iDel_RecentDay;
			}
			else
			{
				m_iDel_Day = 31;
			}

			for(int i = 0; i < 31; i++)
			{
				bDelFolder[i] = FALSE;
			}


			for(int nDay = 1; nDay <= m_iDel_Day; nDay++)
			{
				if(nMonth>9 && nDay>9)
					Delete_Img_dir.Format(_T("%s\\%d%d%d"),m_strImgPath, nYear, nMonth, nDay);
				else if(nMonth<10 && nDay>9)
					Delete_Img_dir.Format(_T("%s\\%d0%d%d"),m_strImgPath, nYear, nMonth, nDay);
				else if(nMonth>9 && nDay<10)
					Delete_Img_dir.Format(_T("%s\\%d%d0%d"),m_strImgPath, nYear, nMonth, nDay);
				else if(nMonth<10 && nDay<10)
					Delete_Img_dir.Format(_T("%s\\%d0%d0%d"),m_strImgPath, nYear, nMonth, nDay);

				CFileFind dbfile;

				if(dbfile.FindFile(Delete_Img_dir, 0))
				{
					if(DeleteFolder(Delete_Img_dir))
						RemoveDirectory(Delete_Img_dir);
				}

				if(DeleteFolderSearch(Delete_Img_dir))
				{
					bDelFolder[nDay - 1] = TRUE;
				}
				else
				{
					bDelFolder[nDay - 1] = FALSE;
				}
			}
		}
	}

	return TRUE;
}

UINT CIPSDlg::AutoDeleteThread(LPVOID pParam)
{
	CIPSDlg *pdlg = (CIPSDlg *)pParam;
	SYSTEMTIME	lpSystemTime;
	typeSystemPara systemPara;
	
	while(TRUE)
	{
		if(pdlg->m_bAutoDeleteStart && !pdlg->m_dlgSetup->IsWindowVisible())
		{
			GetLocalTime(&lpSystemTime);
			systemPara = pdlg->GetSystemPara();
			if((systemPara.nHour == lpSystemTime.wHour) && (systemPara.nMinute == lpSystemTime.wMinute) && !pdlg->m_bDelete)
			{
				pdlg->StartAutoDelete_Img();
				pdlg->m_bDelete = TRUE;
			}
			else if((systemPara.nMinute != lpSystemTime.wMinute) && pdlg->m_bDelete)
			{
				pdlg->m_bDelete = FALSE;
			}
		}

		if(pdlg->m_bAutoDeleteEnd)
		{
			pdlg->m_bAutoDeleteStart = FALSE;
			pdlg->m_bAutoDeleteEnd = FALSE;
			break;
		}
		Sleep(1);
	}

	return 0;
}

void TestFun(int *test)
{
	*test++;
}

void CIPSDlg::ClickBtnenhctrlGrabStop()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int test = 0;
	TestFun(&test);

}



