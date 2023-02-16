
// IPSDlg.h : 헤더 파일
//


#pragma once
#include "include/VisionModuleIPS.h"
#include "include/SVERSOK.H"
#include "Include/_CtrlImageRect.h"
#include "include/_FuncModuleHal.h"
#include "include/_FuncModuleBasic.h"
#include "DialogSetup.h"
#include "afxwin.h"
#include "btnenhctrl.h"
#include "btnenh.h"
#include "_Font.h"

//#define _TEST

#define USER_MSG_MAIN							WM_USER+10 

#define	IMAGE_RECT_DIALOG						10000
#define IMAGE_DISPLAY							0
#define INS_RSLT_IMAGE_DISPLAY					10

#define SERVER_PORT_NUM							3011

#define SETUP_DLG_LEFT							6
#define SETUP_DLG_TOP							620
#define SETUP_DLG_RIGHT							1700
#define SETUP_DLG_BOTTOM						410

#define DEFECT_DATA_MAX_NUM						30000

#define DEFAULT_RESULT_PATH						_T("D:\\RESULT")
#define DEFAULT_DATABASE_PATH					_T("D:\\DATABASE")

//Receive Packet
#define PACKET_RECEIVE_GRAB_READY				_T("0001")
#define PACKET_RECEIVE_GRAB_STOP				_T("0002")
#define PACKET_RECEIVE_AUTO_START				_T("0003")
#define PACKET_RECEIVE_AUTO_STOP				_T("0004")
#define PACKET_RECEIVE_AUTORUN					_T("6000")


//Send Packet
#define PACKET_SEND_GRAB_READY_COMP				_T("1001")
#define PACKET_SEND_GRAB_COMP					_T("1002")
#define PACKET_SEND_GRAB_STOP					_T("1004")

#define	PACKET_SEND_AUTORUN						_T("5000")
#define	PACKET_SEND_MANUAL						_T("5001")

#define GRAB_TIMEOUT_SEC						1000

#define MAIN_GRAB_READY							0
#define MAIN_GRAB_COMP							1
#define MAIN_GRAB_TIMEOUT						2
#define MAIN_AUTO_START							3
#define MAIN_AUTO_STOP							4

#define MAX_STRING_LINE							30000

#define WHITE_NG_IMAGE_HEIGHT					100
#define WHITE_NG_IMAGE_WIDTH					100

#define PAT_NG_IMAGE_HEIGHT						50
#define PAT_NG_IMAGE_WIDTH						50

 
// CIPSDlg 대화 상자
class CIPSDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(CIPSDlg);
// 생성입니다.
public:
	CIPSDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	virtual ~CIPSDlg();

	int m_Test;

	C_CtrlImageRect *m_dlgImgRect[MAX_CAM + PCL_IMAGE];
	int m_JobID;
	int m_LotID;
	int m_GlassID;

	CString mRootDirectory;
	CString m_ModelName;

	typeGrabPara GetGrabPara(int scan_id);
	void SetGrabPara(typeGrabPara grabPara, int scan_idx);
	void SetTitle(CString tile, int scan_idx);
	void SetGrabReady(BOOL ready, int scan_idx);
	void SelectView(int cam_idx);

	void SaveImage(int cam_idx, int rotate, int scan_idx, char* filename, char* image_type = "bmp");
	Hobject GetHalImage(int cam_idx, int rotate, int scan_idx);
	Hobject GetImage(){return GetHalImage(GetCamIdx(), GetRotate(), GetScanIdx());};
	int GetCamIdx();
	int GetRotate();
	int GetScanIdx();

	BOOL Save_Log(BOOL disp, CString log, BOOL error, BOOL bFileSave);

	void SetServerStatus(BOOL isOnoff);

	void DrawDeafultLine(int cam_idx);

	BOOL SetInspectScan(long disp, int scan_idx, Hobject *image, long *rslt_num, Hobject *rslt_rgn);
	BOOL SetInspectScan(long disp, int scan_idx, Hobject *image, long *rslt_num, Hobject *rslt_rgn, long *pat_rslt_num, Hobject *pat_rslt_rgn);
	BOOL SetInspectScanCVD(long disp, Hobject *image, BOOL up_direct, int scan_idx, double pixel_size, int thresh, long *rslt_num, double *rslt_x, double *rslt_y);
	BOOL SetInspectScanPCL(long disp, Hobject *image, BOOL up_direct, int scan_idx, double pixel_size, int thresh, long *rslt_num, double *rslt_x, double *rslt_y);

	CString GetResultImagePath();
	CString GetResultDataPath();

	BOOL Inspection(typeGrabPara grabPara);
	BOOL Inspection_8K(typeGrabPara grabPara);

	BOOL WriteInsResultFile(CString file_path, CString result_data, int scan_Idx);

	void ShiftImage();

	void BufImageToHalImage(int cam_idx, int rotate, int scan_id);

	void DefectImageave(typeGrabPara grabPara, int defect_idx);

	void SetSetupPara(typeSetupPara setupPara);
	//void SetInsPara(typeInspPara insPara, int scan_Idx);
	void SetInsPara(typeInspPara insPara, int cam_idx, int scan_idx, int rotate);
	void SetSystemPara(typeSystemPara systemPara);
	typeSetupPara GetSetupPara();
	//typeInspPara GetInsPara(int scan_Idx);
	typeInspPara GetInsPara(int cam_idx, int scan_idx, int rotate);
	typeSystemPara GetSystemPara();

	BOOL SaveSetupPara();
	BOOL LoadSetupPara();
	BOOL SaveInspPara(int cam_idx, int rotate, int scan_idx);
	BOOL LoadInspPara(int cam_idx, int rotate, int scan_idx);
	BOOL SaveSystemPara();
	BOOL LoadSystemPara();

	BOOL GetInspection(int scan_idx);
	void SetInspection(BOOL isInspection, int scan_idx);

	BOOL GetGrabComp(int scan_idx);
	void SetGrabComp(BOOL isComp, int scan_idx);

	void InitInspectionPara(int cam_idx, int scan_idx, int rotate);

	BOOL WriteLog(CString file_path, CString log, BOOL isList = FALSE);

	CString m_strResultLotNoPath;
	CString m_strResultDataPath;
	CString m_strResultImagePath;



	//20160526 ngh
	////////////////////////////////////////////////
	BOOL m_bDelete;
	int m_iDel_Year;
	int m_iDel_Month;
	int m_iDel_Day;
	int m_iDel_RecentDay;
	CString m_strImgPath;

	BOOL StartAutoDelete_Img();
	BOOL CheckDeleteDate();
	BOOL DeleteFolder(CString strFolder);
	BOOL DeleteFolderSearch(CString path);

	CWinThread *m_pAutoDeleteThread;
	static UINT AutoDeleteThread(LPVOID pParam);
	BOOL m_bAutoDeleteStart;
	BOOL m_bAutoDeleteEnd;
	////////////////////////////////////////////////

	BOOL m_bAutoGrabInspectEnd;

	BOOL GetAutoStartState();
//	m_btnAutoStart
// 대화 상자 데이터입니다.
	enum { IDD = IDD_IPS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	Hobject m_RsltRegion[MAX_CAM][MAX_ROTATE][MAX_SCAN_CNT];
	int m_RsltNum[MAX_CAM][MAX_ROTATE][MAX_SCAN_CNT];

	Hobject m_RsltPatRegion[MAX_CAM][MAX_ROTATE][MAX_SCAN_CNT];
	int m_RsltPatNum[MAX_CAM][MAX_ROTATE][MAX_SCAN_CNT];

	//20160608 ngh
	typeCVDRslt *m_CVDRslt[MAX_ROTATE][MAX_SCAN_CNT];
	int m_nUpRsltNum[MAX_ROTATE][MAX_SCAN_CNT];
	int m_nDwRsltNum[MAX_ROTATE][MAX_SCAN_CNT];

	//20160609 ngh
	typeCVDRslt *m_PCLRslt[MAX_ROTATE][MAX_SCAN_CNT];
	int m_nUpPCLRsltNum[MAX_ROTATE][MAX_SCAN_CNT];
	int m_nDwPCLRsltNum[MAX_ROTATE][MAX_SCAN_CNT];

	BOOL m_bInspection[MAX_SCAN_CNT];

	CSeverSock *m_Server;
	CClientSock *m_Client;
	BOOL m_bServerEnd;
	BOOL ServerOpen();
	BOOL TCPIPSendMCS(CString packet);

	typeInspPara *m_InsPara;//[MAX_SCAN_CNT];
	typeSetupPara m_SetupPara;
	typeSystemPara m_SystemPara;

	CDialogSetup *m_dlgSetup;

	CVisionModule *m_VisMod;

	UINT static InspectionThread(LPVOID);
	CWinThread *m_pThreadInspection;
	BOOL m_bInspectionEnd;
	int m_nInspection_idx;

	BOOL m_bEMIControlConnection;

	UINT static GrabThread(LPVOID pParam);
	CWinThread *m_pThreadGrab;
	int m_nGrabIdx;
	BOOL m_bAutoStart;
	BOOL m_bAutoEnd;
	BOOL m_bGrabReady[MAX_SCAN_CNT];
	BOOL m_bGrabComp[MAX_SCAN_CNT];

#ifdef PCL_TEST
	Hobject m_HalImage[MAX_CAM + PCL_IMAGE][MAX_ROTATE][MAX_SCAN_CNT];
	Hobject m_HalBufImage[MAX_CAM + PCL_IMAGE];
#else
	Hobject m_HalImage[MAX_CAM][MAX_ROTATE][MAX_SCAN_CNT];
	Hobject m_HalBufImage[MAX_CAM];
#endif
	int m_nSelectScanIdx[MAX_CAM + PCL_IMAGE];
	int m_nDerection[MAX_CAM + PCL_IMAGE];
	int m_nRotate[MAX_CAM + PCL_IMAGE];
	typeGrabPara m_GrabPara[MAX_SCAN_CNT];

	void SelectCam(int cam_idx);
	
	void GrabReady(typeGrabPara grabPara);

	void SelectScanIdx(int cam_idx, int rotate, int image_ix);
	LRESULT OnMainMessage(WPARAM para0, LPARAM para1);

	BOOL ImageRectInit(int cnt);
	

	int HandleAllMessage();
	LRESULT OnImageRectMessage(WPARAM para0, LPARAM para1);

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnAccept(WPARAM,LPARAM);
	afx_msg LRESULT OnClose(WPARAM,LPARAM);
	afx_msg LRESULT OnReceive(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
	CStatic m_staticMain;
	//int m_nCamIdx;
	afx_msg void OnBnClickedRadioCam1();
	afx_msg void OnBnClickedRadioCam2();
	
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickBtnenhctrlGrab();
	void ClickBtnenhctrlDerection();
	CBtnEnh m_btnDerection;
	int m_nImageIdx;
	afx_msg void OnBnClickedRadioImage1();
	afx_msg void OnBnClickedRadioImage2();
	afx_msg void OnBnClickedRadioImage3();
	afx_msg void OnBnClickedRadioImage4();
	afx_msg void OnBnClickedRadioImage5();
	CBtnEnh m_btnAutoStart;
	void ClickBtnenhctrlAuto();
	CBtnEnh m_btnRotate;
	void ClickBtnenhctrlRotate();
	CBtnEnh m_btnStateEMI;
public:
	afx_msg void OnBnClickedRadioImage6();
	void ClickBtnenhctrlSave();
	void ClickBtnenhctrlSetup();
	afx_msg void OnBnClickedRadioImage7();
	afx_msg void OnBnClickedRadioImage8();
	afx_msg void OnBnClickedRadioImage9();
	afx_msg void OnBnClickedRadioImage10();
	afx_msg void OnBnClickedRadioCam3();
	BOOL m_nCamIdx;
	void ClickBtnenhctrlGrabStop();
};