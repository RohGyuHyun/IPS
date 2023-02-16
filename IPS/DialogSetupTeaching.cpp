// DialogSetupTeaching.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "IPS.h"
#include "IPSDlg.h"
#include "DialogSetupTeaching.h"
#include "afxdialogex.h"

// CDialogSetupTeaching 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSetupTeaching, CDialogEx)

CDialogSetupTeaching::CDialogSetupTeaching(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogSetupTeaching::IDD, pParent)
	, m_edit_8k_long_width(0)
	, m_edit_8k_long_height(0)
	, m_edit_8k_short_width(0)
	, m_edit_8k_short_height(0)
	, m_edit_16k_long_width(0)
	, m_edit_16k_long_height(0)
	, m_edit_16k_short_width(0)
	, m_edit_16k_short_height(0)
	, m_edit_8k_long_deretion_offset(0)
	, m_edit_8k_short_deretion_offset(0)
	, m_edit_16k_long_deretion_offset(0)
	, m_edit_16k_short_deretion_offset(0)
	, m_edit_8k_long_deafult_line_x(0)
	, m_edit_8k_long_deafult_line_y(0)
	, m_edit_16k_long_defult_line_x(0)
	, m_edit_16k_long_defult_line_y(0)
	, m_edit_thrsh0(0.0)
	, m_edit_8k_defect_size0(0)
	, m_edit_8k_inspection_cnt0(0)
	, m_edit_thrsh1(0)
	, m_edit_thrsh2(0)
	, m_edit_nNoiseSize0(0)
	, m_edit_nNoiseSize1(0)
	, m_edit_nNoiseSize2(0)
	, m_edit_dMargeDistance(0)
	, m_edit_dTest0(0)
	, m_edit_dTest1(0)
	, m_edit_dTest2(0)
	, m_edit_dTest3(0)
	, m_edit_dTest4(0)
	, m_edit_thrsh3(0)
	, m_edit_thrsh4(0)
	, m_edit_nMaxDefectCnt(0)
{
	
}

CDialogSetupTeaching::~CDialogSetupTeaching()
{
}

void CDialogSetupTeaching::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_8K_LONG_WIDTH, m_edit_8k_long_width);
	DDX_Text(pDX, IDC_EDIT_8K_LONG_HEIGHT, m_edit_8k_long_height);
	DDX_Text(pDX, IDC_EDIT_8K_SHORT_WIDTH, m_edit_8k_short_width);
	DDX_Text(pDX, IDC_EDIT_8K_SHORT_HEIGHT, m_edit_8k_short_height);

	DDX_Text(pDX, IDC_EDIT_16K_LONG_WIDTH, m_edit_16k_long_width);
	DDX_Text(pDX, IDC_EDIT_16K_LONG_HEIGHT, m_edit_16k_long_height);
	DDX_Text(pDX, IDC_EDIT_16K_SHORT_WIDTH, m_edit_16k_short_width);
	DDX_Text(pDX, IDC_EDIT_16K_SHORT_HEIGHT, m_edit_16k_short_height);

	DDX_Text(pDX, IDC_EDIT_8K_DERECTION_OFFET1, m_edit_8k_long_deretion_offset);
	DDX_Text(pDX, IDC_EDIT_8K_DERECTION_OFFET2, m_edit_8k_short_deretion_offset);

	DDX_Text(pDX, IDC_EDIT_16K_DERECTION_OFFET1, m_edit_16k_long_deretion_offset);
	DDX_Text(pDX, IDC_EDIT_16K_DERECTION_OFFET2, m_edit_16k_short_deretion_offset);

	DDX_Text(pDX, IDC_EDIT_8K_LONG_DEAFULT_LINE_X, m_edit_8k_long_deafult_line_x);
	DDX_Text(pDX, IDC_EDIT_8K_LONG_DEAFULT_LINE_Y, m_edit_8k_long_deafult_line_y);

	DDX_Text(pDX, IDC_EDIT_16K_LONG_DEAFULT_LINE_X, m_edit_16k_long_defult_line_x);
	DDX_Text(pDX, IDC_EDIT_16K_LONG_DEAFULT_LINE_Y, m_edit_16k_long_defult_line_y);


	DDX_Text(pDX, IDC_EDIT_8K_THRESH0, m_edit_thrsh0);

	DDX_Text(pDX, IDC_EDIT_8K_DEFECT_SIZE0, m_edit_8k_defect_size0);

	DDX_Text(pDX, IDC_EDIT_8K_INSPECTION_CNT0, m_edit_8k_inspection_cnt0);

	DDX_Text(pDX, IDC_EDIT_8K_THRESH1, m_edit_thrsh1);
	DDX_Text(pDX, IDC_EDIT_8K_THRESH2, m_edit_thrsh2);
	DDX_Text(pDX, IDC_EDIT_8K_NOISE_0, m_edit_nNoiseSize0);
	DDX_Text(pDX, IDC_EDIT_8K_NOISE_1, m_edit_nNoiseSize1);
	DDX_Text(pDX, IDC_EDIT_8K_NOISE_2, m_edit_nNoiseSize2);
	DDX_Text(pDX, IDC_EDIT_8K_MARGE_DISTANCE, m_edit_dMargeDistance);
	DDX_Text(pDX, IDC_EDIT_8K_INSPECTION_TEST0, m_edit_dTest0);
	DDX_Text(pDX, IDC_EDIT_8K_INSPECTION_TEST1, m_edit_dTest1);
	DDX_Text(pDX, IDC_EDIT_8K_INSPECTION_TEST2, m_edit_dTest2);
	DDX_Text(pDX, IDC_EDIT_8K_INSPECTION_TEST3, m_edit_dTest3);
	DDX_Text(pDX, IDC_EDIT_8K_INSPECTION_TEST4, m_edit_dTest4);
	DDX_Text(pDX, IDC_EDIT_8K_THRESH3, m_edit_thrsh3);
	DDX_Control(pDX, IDC_COMBO_SELECT_DEFECT, m_combo_SelectDefect);
	DDX_Text(pDX, IDC_EDIT_8K_THRESH4, m_edit_thrsh4);
	DDX_Text(pDX, IDC_EDIT_8K_DEFECT_MAX_CNT, m_edit_nMaxDefectCnt);
}


BEGIN_MESSAGE_MAP(CDialogSetupTeaching, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CBN_CLOSEUP(IDC_COMBO_SELECT_DEFECT, &CDialogSetupTeaching::OnCbnCloseupComboSelectDefect)
END_MESSAGE_MAP()


// CDialogSetupTeaching 메시지 처리기입니다.
BOOL CDialogSetupTeaching::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_combo_SelectDefect.SetCurSel(0);
	return TRUE;
}

BEGIN_EVENTSINK_MAP(CDialogSetupTeaching, CDialogEx)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_8K_LONG_SAVE, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrl8kLongSave, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_8K_SHORT_SAVE, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrl8kShortSave, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_16K_LONG_SAVE, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrl16kLongSave, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_16K_SHORT_SAVE, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrl16kShortSave, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_8K_TEST, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrl8kTest, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_8K_LONG_OFFSET_MOVE, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrl8kLongOffsetMove, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_TEACHING_IMAGE_LOAD, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrlTeachingImageLoad, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogSetupTeaching::ClickBtnenhctrl8kLongSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CIPSDlg * pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	CString strTemp, path;
	char temp[512];
	path.Format(_T("%s%s\\TeachingIMG"), pdlg->mRootDirectory, pdlg->m_ModelName);
	sprintf_s(temp, path.GetLength() + 1, "%S", path);
	MakeDirectories((LPCWSTR)temp);
	//MakeDirectories(path);
	strTemp.Format(_T("%s\\Cam_8K_LONG"), path);
	
	sprintf_s(temp, strTemp.GetLength() + 1, "%S", strTemp);
	pdlg->SaveImage(CAM_8K, ROTATE_GRAB_0, pdlg->GetScanIdx(), temp);
}


void CDialogSetupTeaching::ClickBtnenhctrl8kShortSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CIPSDlg * pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	CString strTemp, path;
	char temp[512];
	path.Format(_T("%s%s\\TeachingIMG"), pdlg->mRootDirectory, pdlg->m_ModelName);
	sprintf_s(temp, path.GetLength() + 1, "%S", path);
	MakeDirectories((LPCWSTR)temp);
	//MakeDirectories(path);
	strTemp.Format(_T("%s\\Cam_8K_SHORT"), path);
	
	sprintf_s(temp, strTemp.GetLength() + 1, "%S", strTemp);
	pdlg->SaveImage(CAM_8K, ROTATE_GRAB_90, pdlg->GetScanIdx(), temp);
}


void CDialogSetupTeaching::ClickBtnenhctrl16kLongSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CIPSDlg * pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	CString strTemp, path;
	char temp[512];
	path.Format(_T("%s%s\\TeachingIMG"), pdlg->mRootDirectory, pdlg->m_ModelName);
	sprintf_s(temp, path.GetLength() + 1, "%S", path);
	MakeDirectories((LPCWSTR)temp);
	//MakeDirectories(path);
	strTemp.Format(_T("%s\\Cam_16K_LONG"), path);
	
	sprintf_s(temp, strTemp.GetLength() + 1, "%S", strTemp);
	pdlg->SaveImage(CAM_16K, ROTATE_GRAB_0, pdlg->GetScanIdx(), temp);
}


void CDialogSetupTeaching::ClickBtnenhctrl16kShortSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CIPSDlg * pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	CString strTemp, path;
		char temp[512];
	path.Format(_T("%s%s\\TeachingIMG"), pdlg->mRootDirectory, pdlg->m_ModelName);
	sprintf_s(temp, path.GetLength() + 1, "%S", path);
	MakeDirectories((LPCWSTR)temp);
	//MakeDirectories(path);
	strTemp.Format(_T("%s\\Cam_16K_SHORT"), path);

	sprintf_s(temp, strTemp.GetLength() + 1, "%S", strTemp);
	pdlg->SaveImage(CAM_16K, ROTATE_GRAB_90, pdlg->GetScanIdx(), temp);
}


void CDialogSetupTeaching::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		//Update(FALSE);
	}
	else
	{

	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

int CDialogSetupTeaching::Get8KLongWidth(){return m_edit_8k_long_width;}
int CDialogSetupTeaching::Get8KLongHeight(){return m_edit_8k_long_height;}
int CDialogSetupTeaching::Get8KShortWidth(){return m_edit_8k_short_width;}
int CDialogSetupTeaching::Get8KShortHeight(){return m_edit_8k_short_height;}

int CDialogSetupTeaching::Get16KLongWidth(){return m_edit_16k_long_width;}
int CDialogSetupTeaching::Get16KLongHeight(){return m_edit_16k_long_height;}
int CDialogSetupTeaching::Get16KShortWidth(){return m_edit_16k_short_width;}
int CDialogSetupTeaching::Get16KShortHeight(){return m_edit_16k_short_height;}

int CDialogSetupTeaching::Get8KLongOffset(){return m_edit_8k_long_deretion_offset;}
int CDialogSetupTeaching::Get8KShortOffset(){return m_edit_8k_short_deretion_offset;}

int CDialogSetupTeaching::Get16KLongOffset(){return m_edit_16k_long_deretion_offset;}
int CDialogSetupTeaching::Get16KShortOffset(){return m_edit_16k_short_deretion_offset;}

int CDialogSetupTeaching::Get8KLongDeafultLineX(){return m_edit_8k_long_deafult_line_x;}
int CDialogSetupTeaching::Get8KLongDeafultLineY(){return m_edit_8k_long_deafult_line_y;}

int CDialogSetupTeaching::Get16KLongDeafultLineX(){return m_edit_16k_long_defult_line_x;}
int CDialogSetupTeaching::Get16KLongDeafultLineY(){return m_edit_16k_long_defult_line_y;}

double CDialogSetupTeaching::Get8KThresh0(int idx)
{
	double rslt;
	switch(idx)
	{
	case 0:
		rslt = m_edit_thrsh0;
		break;
	case 1:
		rslt = m_edit_thrsh1;
		break;
	case 2:
		rslt = m_edit_thrsh2;
		break;
	case 3:
		rslt = m_edit_thrsh3;
		break;
	case 4:
		rslt = m_edit_thrsh4;
		break;
	}

	return rslt;
}

int CDialogSetupTeaching::Get8KDefecSize0(){return m_edit_8k_defect_size0;}

int CDialogSetupTeaching::Get8KInspectionCnt0(){return m_edit_8k_inspection_cnt0;}

//20160318 ngh
int CDialogSetupTeaching::Get8KNoiseSize(int idx)
{
	int rslt = 0;

	switch(idx)
	{
	case 0:
		rslt = m_edit_nNoiseSize0;
		break;
	case 1:
		rslt = m_edit_nNoiseSize1;
		break;
	case 2:
		rslt = m_edit_nNoiseSize2;
		break;
	}

	return rslt;
}
//20160318 ngh
double CDialogSetupTeaching::Get8KMargeDistance()
{
	return m_edit_dMargeDistance;
}

void CDialogSetupTeaching::ClickBtnenhctrl8kTest()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CIPSDlg *pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	typeGrabPara grabPara;
	Hobject rslt_rgn, slct_rgn;
	CTime cTime;
	cTime = CTime::GetCurrentTime();

	pdlg->m_bAutoGrabInspectEnd = FALSE;

	grabPara.nCamIdx = pdlg->GetCamIdx();
	grabPara.nRotat[grabPara.nCamIdx] = pdlg->GetRotate();
	if(grabPara.nCamIdx == CAM_8K)
	{
		switch(grabPara.nRotat[grabPara.nCamIdx])
		{
		case 0:
			grabPara.nHeight[grabPara.nCamIdx] = Get8KLongHeight();
			if(grabPara.nDerection[grabPara.nCamIdx] == 1)
			{
				grabPara.nGrabOffset[grabPara.nCamIdx] = Get8KLongOffset();
			}
			break;
		case 1:
			grabPara.nHeight[grabPara.nCamIdx] = Get8KShortHeight();
			if(grabPara.nDerection[grabPara.nCamIdx] == 1)
			{
				grabPara.nGrabOffset[grabPara.nCamIdx] = Get8KShortOffset();
			}
			break;
		}
	}
	else if(grabPara.nCamIdx >= CAM_16K)
	{
		switch(grabPara.nRotat[grabPara.nCamIdx])
		{
		case 0:
			grabPara.nHeight[grabPara.nCamIdx] = Get16KLongHeight();
			if(grabPara.nDerection[grabPara.nCamIdx] == 1)
			{
				grabPara.nGrabOffset[grabPara.nCamIdx] = Get16KLongOffset();
			}
			break;
		case 1:
			grabPara.nHeight[grabPara.nCamIdx] = Get16KShortHeight();
			if(grabPara.nDerection[grabPara.nCamIdx] == 1)
			{
				grabPara.nGrabOffset[grabPara.nCamIdx] = Get16KShortOffset();
			}
			break;
		}
	}
	
	
	grabPara.nScanIdx[grabPara.nCamIdx] = pdlg->GetScanIdx();
	grabPara.strLotNo = _T("TEST");
	grabPara.strGlassID = _T("TEST1");
	if(grabPara.nCamIdx <= 1)
	{
		grabPara.nInspectionType = 0;
	}
	else
	{
		grabPara.nInspectionType = 1;
	}

	pdlg->m_strResultLotNoPath.Format(_T("%s\\%04d%02d%02d\\%s\\%s\\"), DEFAULT_RESULT_PATH, cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), grabPara.strLotNo, grabPara.strGlassID);
	if(grabPara.nCamIdx == 0)
	{
		pdlg->m_strResultDataPath.Format(_T("%s\\Data\\%d_%d_%d\\"), pdlg->m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx]);
		pdlg->m_strResultImagePath.Format(_T("%s\\Image\\%d_%d_%d\\"), pdlg->m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nScanIdx[grabPara.nCamIdx]);
	}
	else
	{
#ifdef PCL_TEST
		if(grabPara.nInspectionType == INSPECTION_TYPE_CVD)
		{
			pdlg->m_strResultDataPath.Format(_T("%s\\Data\\CVD\\%d_%d_%d\\"), pdlg->m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
			pdlg->m_strResultImagePath.Format(_T("%s\\Image\\CVD\\%d_%d_%d\\"), pdlg->m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
		}
		else if(grabPara.nInspectionType = INSPECTION_TYPE_PCL)
		{
			if(grabPara.nRotat[grabPara.nCamIdx] == ROTATE_GRAB_0)
			{
				pdlg->m_strResultDataPath.Format(_T("%s\\Data\\PCL_L\\%d_%d_%d\\"), pdlg->m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
				pdlg->m_strResultImagePath.Format(_T("%s\\Image\\PCL_L\\%d_%d_%d\\"), pdlg->m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
			}
			else if(grabPara.nRotat[grabPara.nCamIdx] == ROTATE_GRAB_90)
			{
				pdlg->m_strResultDataPath.Format(_T("%s\\Data\\PCL_S\\%d_%d_%d\\"), pdlg->m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
				pdlg->m_strResultImagePath.Format(_T("%s\\Image\\PCL_S\\%d_%d_%d\\"), pdlg->m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
			}				
		}
#else
		pdlg->m_strResultDataPath.Format(_T("%s\\Data\\%d_%d_%d\\"), pdlg->m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
		pdlg->m_strResultImagePath.Format(_T("%s\\Image\\%d_%d_%d\\"), pdlg->m_strResultLotNoPath, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
#endif
	}

	char temp[512];
	grabPara.strResultLotNoPath = pdlg->m_strResultLotNoPath;
	sprintf_s(temp, pdlg->m_strResultLotNoPath.GetLength() + 1,  "%S", pdlg->m_strResultLotNoPath);
	MakeDirectories((LPCWSTR)temp);
	grabPara.strResultDataPath = pdlg->m_strResultDataPath;
	sprintf_s(temp, pdlg->m_strResultDataPath.GetLength() + 1,  "%S", pdlg->m_strResultDataPath);
	MakeDirectories((LPCWSTR)temp);
	grabPara.strResultImagePath = pdlg->m_strResultImagePath;
	sprintf_s(temp, pdlg->m_strResultImagePath.GetLength() + 1,  "%S", pdlg->m_strResultImagePath);
	MakeDirectories((LPCWSTR)temp);

	pdlg->SetGrabPara(grabPara, grabPara.nScanIdx[grabPara.nCamIdx]);

	CString strTemp;
	strTemp.Format(_T("Cam %d Rotate %d Scan %d Inspection Test"), grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	pdlg->WriteLog(LOG_PATH, strTemp);

	pdlg->Inspection(grabPara);

	//pdlg->LoadInspPara(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]);
	//pdlg->SetInspectScan(pdlg->m_dlgImgRect[grabPara.nCamIdx]->GetView(), grabPara.nScanIdx[grabPara.nCamIdx],
	//	&pdlg->GetHalImage(grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx]), &rslt_num, &rslt_rgn);
}


void CDialogSetupTeaching::ClickBtnenhctrl8kLongOffsetMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CIPSDlg *pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	pdlg->ShiftImage();
	pdlg->DrawDeafultLine(pdlg->GetCamIdx());
}


void CDialogSetupTeaching::ClickBtnenhctrlTeachingImageLoad()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CIPSDlg *pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	HTuple wd, ht;
	Herror herr;
	CString strFile;
	char temp[512];


	for(int i = 0; i < 1; i++)
	{
		strFile.Format(_T("D:\\DATABASE\\test\\teachingIMG\\Cam_8K_LONG_%d.bmp"), i);
		sprintf_s(temp, strFile.GetLength() + 1, "%S", strFile);
		read_image(&pdlg->GetHalImage(pdlg->GetCamIdx(), 0, i), temp);
		//Hal_OpenHalImage(, strFile, FALSE);

		set_check("~give_error");
		herr = get_image_pointer1(pdlg->GetHalImage(pdlg->GetCamIdx(), 0, i), NULL, NULL, &wd, &ht);
		set_check("give_error");
		if(H_MSG_TRUE == herr)
		{
		}
		else
		{
			AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1!"));
			return;
		}

		disp_obj(pdlg->GetHalImage(pdlg->GetCamIdx(), 0, i), pdlg->m_dlgImgRect[pdlg->GetCamIdx()]->GetView());
		pdlg->m_dlgImgRect[pdlg->GetCamIdx()]->Display();
	}
}


void CDialogSetupTeaching::Update(BOOL isUpdate)
{
	CIPSDlg *pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	typeSetupPara setupPara;
	typeInspPara insPara;
	setupPara = pdlg->GetSetupPara();
	insPara = pdlg->GetInsPara(pdlg->GetCamIdx(), pdlg->GetScanIdx(), pdlg->GetRotate());
	if(isUpdate)
	{
		UpdateData(TRUE);
		setupPara.TeachingPara.n8K_LongWidth = Get8KLongWidth();//m_edit_8k_long_width
		setupPara.TeachingPara.n8K_LongHeight = Get8KLongHeight();//m_edit_8k_long_height
		setupPara.TeachingPara.n8K_ShortWidth = Get8KShortWidth();//m_edit_8k_short_width
		setupPara.TeachingPara.n8K_ShortHeight = Get8KShortHeight();//m_edit_8k_short_height

		setupPara.TeachingPara.n16K_LongWidth = Get16KLongWidth();//m_edit_16k_long_width
		setupPara.TeachingPara.n16K_LongHeight = Get16KLongHeight();//m_edit_16k_long_height
		setupPara.TeachingPara.n16K_ShortWidth = Get16KShortWidth();//m_edit_16k_short_width
		setupPara.TeachingPara.n16K_ShortHeight = Get16KShortHeight();//m_edit_16k_short_height

		setupPara.TeachingPara.n8K_LongOffset = Get8KLongOffset();//m_edit_8k_long_deretion_offset
		setupPara.TeachingPara.n8K_ShortOffset = Get8KShortOffset();//m_edit_8k_short_deretion_offset

		setupPara.TeachingPara.n16K_LongOffset = Get16KLongOffset();//m_edit_16k_long_deretion_offset
		setupPara.TeachingPara.n16K_ShortOffset = Get16KShortOffset();//m_edit_16k_short_deretion_offset

		setupPara.TeachingPara.n8K_LongDeafultX = Get8KLongDeafultLineX();//m_edit_8k_long_deafult_line_x
		setupPara.TeachingPara.n8K_ShortDeafultX = Get8KLongDeafultLineY();//m_edit_8k_long_deafult_line_y

		setupPara.TeachingPara.n16K_LongDeafultX = Get16KLongDeafultLineX();//m_edit_16k_long_defult_line_x
		setupPara.TeachingPara.n16K_ShortDeafultX = Get16KLongDeafultLineY();//m_edit_16k_long_defult_line_y
		setupPara.nMaxDefectCnt = m_edit_nMaxDefectCnt;
		pdlg->SetSetupPara(setupPara);

		insPara.dThreshHold[0] = Get8KThresh0(0);//m_edit_thrsh0
		insPara.dThreshHold[1] = Get8KThresh0(1);//m_edit_thrsh0
		insPara.dThreshHold[2] = Get8KThresh0(2);//m_edit_thrsh0
		insPara.dThreshHold[3] = Get8KThresh0(3);//m_edit_thrsh0
		insPara.dThreshHold[4] = Get8KThresh0(4);

		insPara.nDefectSize[0] = Get8KDefecSize0();

		//20160318 ngh
		insPara.nNoiseSize[0] = Get8KNoiseSize(0);
		insPara.nNoiseSize[1] = Get8KNoiseSize(1);
		insPara.nNoiseSize[2] = Get8KNoiseSize(2);

		insPara.dMargeDistance[0] = Get8KMargeDistance();

		insPara.nInspectionCnt = Get8KInspectionCnt0();;

		int sel_defect = m_combo_SelectDefect.GetCurSel();

		insPara.ImageFilterPara[sel_defect].dFactor = m_edit_dTest4;

		pdlg->SetInsPara(insPara, pdlg->GetCamIdx(), pdlg->GetScanIdx(), pdlg->GetRotate());
	}
	else
	{
		m_edit_8k_long_width = setupPara.TeachingPara.n8K_LongWidth;
		m_edit_8k_long_height = setupPara.TeachingPara.n8K_LongHeight;
		m_edit_8k_short_width= setupPara.TeachingPara.n8K_ShortWidth;
		m_edit_8k_short_height = setupPara.TeachingPara.n8K_ShortHeight;

		m_edit_16k_long_width = setupPara.TeachingPara.n16K_LongWidth;
		m_edit_16k_long_height = setupPara.TeachingPara.n16K_LongHeight;
		m_edit_16k_short_width = setupPara.TeachingPara.n16K_ShortWidth;
		m_edit_16k_short_height = setupPara.TeachingPara.n16K_ShortHeight;

		m_edit_8k_long_deretion_offset = setupPara.TeachingPara.n8K_LongOffset;
		m_edit_8k_short_deretion_offset = setupPara.TeachingPara.n8K_ShortOffset;

		m_edit_16k_long_deretion_offset = setupPara.TeachingPara.n16K_LongOffset;
		m_edit_16k_short_deretion_offset = setupPara.TeachingPara.n16K_ShortOffset;

		m_edit_8k_long_deafult_line_x = setupPara.TeachingPara.n8K_LongDeafultX;
		m_edit_8k_long_deafult_line_y = setupPara.TeachingPara.n8K_ShortDeafultX;

		m_edit_16k_long_defult_line_x = setupPara.TeachingPara.n16K_LongDeafultX;
		m_edit_16k_long_defult_line_y = setupPara.TeachingPara.n16K_ShortDeafultX;

		m_edit_nMaxDefectCnt = setupPara.nMaxDefectCnt;

		m_edit_thrsh0 = insPara.dThreshHold[0];
		m_edit_thrsh1 = insPara.dThreshHold[1];
		m_edit_thrsh2 = insPara.dThreshHold[2];
		m_edit_thrsh3 = insPara.dThreshHold[3];
		m_edit_thrsh4 = insPara.dThreshHold[4];

		m_edit_8k_defect_size0 = insPara.nDefectSize[0];

		//20160318 ngh
		m_edit_nNoiseSize0 = insPara.nNoiseSize[0];
		m_edit_nNoiseSize1 = insPara.nNoiseSize[1];
		m_edit_nNoiseSize2 = insPara.nNoiseSize[2];

		m_edit_dMargeDistance = insPara.dMargeDistance[0];

		m_edit_8k_inspection_cnt0 = insPara.nInspectionCnt;

		int sel_defect = m_combo_SelectDefect.GetCurSel();

		m_edit_dTest4 = insPara.ImageFilterPara[sel_defect].dFactor;

		UpdateData(FALSE);
	}
}

void CDialogSetupTeaching::OnCbnCloseupComboSelectDefect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CIPSDlg *pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;
	typeInspPara insPara;
	insPara = pdlg->GetInsPara(pdlg->GetCamIdx(), pdlg->GetScanIdx(), pdlg->GetRotate());

	int idx = m_combo_SelectDefect.GetCurSel();

	m_edit_dTest4 = insPara.ImageFilterPara[idx].dFactor;

	UpdateData(FALSE);
}
