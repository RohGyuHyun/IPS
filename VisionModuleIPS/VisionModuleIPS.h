// VisionModuleIPS.h : VisionModuleIPS DLL의 기본 헤더 파일입니다.
//

#pragma once


#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include "hasp_api.h"
//#define CAMERA
//#define PCL_TEST

#define DAT_STR_LENG								512

#define CAM_8K						0
#define CAM_16K						1
#define MAX_BUF						2
#define PCL_IMAGE					1

#ifdef CAMERA
#define MAX_CAM						2
#define MAX_SCAN_CNT				10
#define MAX_ROTATE					2
#else
#define MAX_CAM						2
#define MAX_SCAN_CNT				1
#define MAX_ROTATE					2
#endif

#ifdef CAMERA
#define CAMERA_WIDTH_8K				8192
#define CAMERA_HEIGHT_8K			1000
#define MAX_CAMERA_WIDTH_8K			8192
#define MAX_HALCON_IMAGE_WIDTH_8K	MAX_CAMERA_WIDTH_8K + 16
#define MAX_CAMERA_HEIGHT_8K		63000
#define MAX_HALCON_IMAGE_HEIGHT_8K	MAX_CAMERA_HEIGHT_8K + 0

#define CAMERA_WIDTH_16K			16384
#define CAMERA_HEIGHT_16K			1000
#define MAX_CAMERA_WIDTH_16K		16384
#define MAX_HALCON_IMAGE_WIDTH_16K	MAX_CAMERA_WIDTH_16K + 16
#define MAX_CAMERA_HEIGHT_16K		190000//110000//
#define MAX_HALCON_IMAGE_HEIGHT_16K	MAX_CAMERA_HEIGHT_16K + 0
#else
#define CAMERA_WIDTH_8K				8192
#define CAMERA_HEIGHT_8K			1000
#define MAX_CAMERA_WIDTH_8K			8192
#define MAX_HALCON_IMAGE_WIDTH_8K	MAX_CAMERA_WIDTH_8K + 16
#define MAX_CAMERA_HEIGHT_8K		10
#define MAX_HALCON_IMAGE_HEIGHT_8K	MAX_CAMERA_HEIGHT_8K + 0

#define CAMERA_WIDTH_16K			16384
#define CAMERA_HEIGHT_16K			1000
#define MAX_CAMERA_WIDTH_16K		16384
#define MAX_HALCON_IMAGE_WIDTH_16K	MAX_CAMERA_WIDTH_16K + 16
#define MAX_CAMERA_HEIGHT_16K		10
#define MAX_HALCON_IMAGE_HEIGHT_16K	MAX_CAMERA_HEIGHT_16K + 0
#endif

#define MAX_DEFECT_CNT				30000

#define FORWARD_DERECTION			0
#define REVERSE_DERECTION			1

#define ROTATE_GRAB_0				0
#define ROTATE_GRAB_90				1

#define DCF_FILE_PATH_8K			_T("D:\\Project\\DCF\\Solios eV-CL Full 01.dcf")
#define DCF_FILE_PATH_16K			_T("D:\\Project\\DCF\\Solios eV-CL Full 02.dcf")

#define MAX_INS_RECT_CNT			1000
#define MAX_INS_PARA				5

#define SYSTEM_PARA_FILE_NAME		_T("System.ini")
#define	SETUP_PARA_FILE_NAME		_T("Setup.ini")
#define INSP_PARA_FILE_NAME			_T("Inspection.ini")

#define LOG_PATH					_T("D:\\DATABASE\\LOG")

#define INSPECTION_TYPE_CVD			0
#define INSPECTION_TYPE_PCL			1

typedef struct GrabPara
{
	CString strLotNo;
	CString strGlassID;
	int nCamIdx;
	int nRotat[MAX_CAM + PCL_IMAGE];
	int nScanIdx[MAX_CAM + PCL_IMAGE];
	int nDerection[MAX_CAM + PCL_IMAGE];
	int nHeight[MAX_CAM + PCL_IMAGE];
	int nGrabOffset[MAX_CAM + PCL_IMAGE];
	CString strResultLotNoPath;
	CString strResultDataPath;
	CString strResultImagePath;
	int nInspectionType;
}typeGrabPara;

typedef struct InspectionRect
{
	double dRectLeft;
	double dRectTop;
	double dRectRight;
	double dRectBottom;
}typeInsRect;

//#ifndef CAMERA
typedef struct ImageFilterPara
{
	/*double dMeen1;
	double dMeen2;
	double dEmpha1;
	double dEmpha2;*/
	double dFactor;
}typeImageFilter;
//#endif

typedef struct InspectionPara
{
	int nParaCnt[MAX_INS_PARA];
	int nInspectionCnt;
	typeInsRect InsRect[MAX_INS_RECT_CNT];
	int nInspectionCVDUpCnt;
	typeInsRect InsCVDUpRect[MAX_INS_RECT_CNT];
	int nInspectionCVDDwCnt;
	typeInsRect InsCVDDwRect[MAX_INS_RECT_CNT];
	double dThreshHold[MAX_INS_PARA];
	int nDefectSize[MAX_INS_PARA];
	//20160318 ngh
	int nNoiseSize[MAX_INS_PARA];
	double dMargeDistance[MAX_INS_PARA];
	double dPixelSize;
//#ifndef CAMERA
	typeImageFilter ImageFilterPara[3];
//#endif
}typeInspPara;

typedef struct SetupTeachingPara
{
	int n8K_LongWidth;
	int n8K_LongHeight;
	int n8K_ShortWidth;
	int n8K_ShortHeight;

	int n16K_LongWidth;
	int n16K_LongHeight;
	int n16K_ShortWidth;
	int n16K_ShortHeight;

	int n8K_LongDeafultX;
	int n8K_LongDeafultY;
	int n8K_ShortDeafultX;
	int n8K_ShortDeafultY;

	int n16K_LongDeafultX;
	int n16K_LongDeafultY;
	int n16K_ShortDeafultX;
	int n16K_ShortDeafultY;

	int n8K_LongOffset;
	int n8K_ShortOffset;
	int n16K_LongOffset;
	int n16K_ShortOffset;

	int n8KThreshHold0;
}tyypeSetupTeachingPara;

typedef struct SetupPara
{
	int nMaxDefectCnt;
	tyypeSetupTeachingPara TeachingPara;
}typeSetupPara;

typedef struct SystemPara
{
	int nPortNum;
	int nDay;
	int nHour;
	int nMinute;
}typeSystemPara;

typedef struct CVDRslt
{
	double dUp_rslt_x;
	double dUp_rslt_y;
	double dDw_rslt_x;
	double dDw_rslt_y;

	double dRowEdgeFirstLR[2];
	double dColumnEdgeFirstLR[2];
	double dRowEdgeSecondLR[2];
	double dColumnEdgeSecondLR[2];
}typeCVDRslt;

class AFX_EXT_CLASS CVisionModule
{
public:
	CVisionModule();
	~CVisionModule();
private:
	MIL_ID m_MilApplication;
	MIL_ID m_MilSystem[MAX_CAM];
	MIL_ID m_MilDigitizer[MAX_CAM];

	MIL_ID m_MilLargeGrabBuf[MAX_CAM];
	MIL_ID m_MilGrabBuf[MAX_CAM][MAX_BUF];

	BOOL m_bGrab[MAX_CAM];

	Hobject m_HalGrabImage[MAX_CAM];
	Hobject m_HalTransImage[MAX_CAM];
	
	hasp_status_t m_SafeNet_USB_status;
	hasp_feature_t m_SafeNet_USB_feature;
	hasp_handle_t m_SafeNet_USB_handle;

	BOOL SafeNet_Lock_LogIn();
	void SafeNet_Lock_LogOut();

	//20160524 ngh
	BOOL m_bGrabStop[MAX_CAM];
	int m_nGrabError[MAX_CAM];
public:
	BOOL Open(int cam_idx);
	BOOL Grab(int cam_idx, int derection, MIL_INT64 grab_height, int *grab_cnt);
	BOOL Close();

	BOOL GetGrab(int cam_idx);

	MIL_ID *GetMilSystem(int cam_idx);
	MIL_ID *GetMilDigitizer(int cam_idx);
	MIL_ID *GetMilLargeGrabBuf(int cam_idx);

	void ShiftImage(int cam_idx, int offset, BOOL derection, Hobject *rotate_halcon_object);

	//20160524 ngh
	void SetGrabStop(int cam_idx, BOOL bStop);
	BOOL GetGrabStop(int cam_idx);
	int GetGrabError(int cam_idx);
	//BOOL WriteLog(CString file_path, CString log, BOOL isList = FALSE);
	BOOL WriteLog(CString file_path, CString log, BOOL isList = FALSE);
};
