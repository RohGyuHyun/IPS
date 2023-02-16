#include "stdafx.h"
#include "VisionModuleIPS.h"
//


//USB key 코드
unsigned char m_SafeNet_USB_vendor_code[] =
"6LCHxDCsAh5qK0OZUDIfbf4aLTYbXbz0bWitlU0/K24xtYoDMTgLEY6I+zwlqUqTW7gooSghysUj8YdV"
"NpgQUcDCBprT2tGa+DPInMWVq3aXeBcJMQyeumBk4JhXnB8WMWbc1QEF2q5S+WD7OtDpvLIk8EBEOChT"
"xRtyF+HbeEAcDFfJnmoBz95nzWI7GRwdLYHyIXXHqKPTczKzN3K03SmewwS/3JOyUa9dpfJIcwl362Sj"
"4IaCNQLAC/wycEGmJ4ADX/FlZA4cCNgHiK5Qllwl/0tl33J/L8wIP8VBh6xUk/bDckAOuB0X6f5ZQ4e/"
"4S6kyCIquM84+OmJHpjo670oH/kDGspjsQzhZR6YaMJuPGAhCaD11AcjW7gwgxnIvjwaL6oe3mVYpHbI"
"GKoKKbH1dWOmuwgEW61lv0a7DSrvaCHYQAYVOWIn8cZ8IeSyxvnCSr3jGFSnbN3bxWa7jDk/qFcCCma3"
"X+T4Ok49iQbrEMnatE/yWpcBEErGBGFSeMDoQbucdxyoZll1HucNJ/MdLzabon4qeyC9FcL7v+anKH+F"
"7pUy4XlFFFWVJ1itujg5HwqUK8zBgssGRPykGjxnclA1PrzgH8mrEaLyhuSWi0xejtJsIMdcovM1xjr6"
"TUSyyToTxCFA05+15CHGEfR5uaNDy8DTivtiv8Fg+UuGcW2aKTcRhzbsstwYUfcPl8tZ/hv8wP6J2I7y"
"qCO8GRfQTQU+L2jQfO2xVMn2gAm9n4zJTvRLXye2nP4+V7BhZrabOG75zwjJUf5E+u1xcqAlkxLea/40"
"ld2d68BkE8Cy1gdrlkh5t9TCS/EQYZNSQtA/jM5DnOfl1PycCGhOJqW2GbNjt4bTmmm5edbO60VWFRiP"
"kyWPhTW00uVQfaq+NVXSnxXAjzxCj+GnJEE4K0t80zB+KLG1ly0FotDfOaluef8t4xYBUr8VHoE6Vi9Q"
"zo/W0nFU91uV6k3yPr4nsA==";

CVisionModule::CVisionModule()
{

}

CVisionModule::~CVisionModule()
{
	
}

BOOL CVisionModule::Open(int cam_idx)
{
	/*if(!SafeNet_Lock_LogIn())
	{
		exit(0);
	}*/

	MIL_INT64 width, height;
	Herror herr;
	CString dcf_path;
	HTuple wd, ht;

	if(cam_idx == CAM_8K)
	{
		MappAlloc(M_DEFAULT, &m_MilApplication);
	}	

	if(cam_idx == CAM_8K)
	{
		MsysAlloc(M_SYSTEM_SOLIOS, M_DEV0, M_DEFAULT, &m_MilSystem[cam_idx]);
		dcf_path = DCF_FILE_PATH_8K;
	}
	else if(cam_idx == CAM_16K)
	{
		MsysAlloc(M_SYSTEM_SOLIOS, M_DEV1, M_DEFAULT, &m_MilSystem[cam_idx]);
		dcf_path = DCF_FILE_PATH_16K;
	}
	else
	{
		return FALSE;
	}

	m_bGrab[cam_idx] = FALSE;

	MdigAlloc(m_MilSystem[cam_idx], 0, dcf_path, M_DEFAULT, &m_MilDigitizer[cam_idx]);

	MdigControl(m_MilDigitizer[cam_idx], M_GRAB_MODE, M_ASYNCHRONOUS);
	MdigControl(m_MilDigitizer[cam_idx], M_GRAB_TIMEOUT, 50000);//M_INFINITE);
	MdigControl(m_MilDigitizer[cam_idx], M_GRAB_TRIGGER, M_ACTIVATE);
	MdigControl(m_MilDigitizer[cam_idx], M_GRAB_TRIGGER_SOURCE, 35L);


	MdigInquire(m_MilDigitizer[cam_idx], M_SIZE_X, &width);
	MdigInquire(m_MilDigitizer[cam_idx], M_SIZE_Y, &height);

	MdigHalt(m_MilDigitizer[cam_idx]);
	
	for(int i = 0; i < MAX_BUF; i++)
	{
		//20160524 ngh
		m_bGrabStop[i] = FALSE;

		if(cam_idx == CAM_8K)
		{
			MbufAlloc2d(m_MilSystem[cam_idx], CAMERA_WIDTH_8K, CAMERA_HEIGHT_8K, 8 + M_UNSIGNED, M_IMAGE + M_GRAB, &m_MilGrabBuf[cam_idx][i]);
			MbufClear(m_MilGrabBuf[cam_idx][i], 0x00);

			if(i == 0)
			{
				gen_image_const(&m_HalGrabImage[cam_idx], HTuple("byte"), MAX_HALCON_IMAGE_WIDTH_8K, MAX_HALCON_IMAGE_HEIGHT_8K);
				gen_image_const(&m_HalTransImage[cam_idx], HTuple("byte"), MAX_HALCON_IMAGE_HEIGHT_8K, MAX_HALCON_IMAGE_WIDTH_8K);

				set_check("~give_error");
					herr = get_image_pointer1(m_HalGrabImage[i], NULL, NULL, &wd, &ht);
				set_check("give_error");

				if(H_MSG_TRUE == herr)
				{
				}
				else
				{
					AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1_00!"));
					return FALSE;
				}	
			}
		}
		else if(cam_idx == CAM_16K)
		{
			MbufAlloc2d(m_MilSystem[cam_idx], CAMERA_WIDTH_16K, CAMERA_HEIGHT_16K, 8 + M_UNSIGNED, M_IMAGE + M_GRAB, &m_MilGrabBuf[cam_idx][i]);
			MbufClear(m_MilGrabBuf[cam_idx][i], 0x00);
			if(i == 0)
			{
				gen_image_const(&m_HalGrabImage[cam_idx], HTuple("byte"), MAX_HALCON_IMAGE_WIDTH_16K, MAX_HALCON_IMAGE_HEIGHT_16K);
				gen_image_const(&m_HalTransImage[cam_idx], HTuple("byte"), MAX_HALCON_IMAGE_HEIGHT_16K, MAX_HALCON_IMAGE_WIDTH_16K);

				set_check("~give_error");
					herr = get_image_pointer1(m_HalGrabImage[i], NULL, NULL, &wd, &ht);
				set_check("give_error");

				if(H_MSG_TRUE == herr)
				{
				}
				else
				{
					AfxMessageBox(_T("Halcon Error Occured at get_image_pointer1_00!"));
					return FALSE;
				}	
			}
		}
	}			

	return TRUE;
}

BOOL CVisionModule::Close()
{
	SafeNet_Lock_LogOut();
	for(int i = 0; i < MAX_CAM; i++)
	{
		for(int j = 0; j < MAX_BUF; j++)
		{
			MbufFree(m_MilGrabBuf[i][j]);
		}
		MdigFree(m_MilDigitizer[i]);
		MsysFree(m_MilSystem[i]);
	}
	MappFree(m_MilApplication);
	return TRUE;
}

BOOL CVisionModule::Grab(int cam_idx, int derection, MIL_INT64 grab_height, int *_grab_cnt)
{
#ifdef CAMERA
	MIL_INT64 width, height;
	MIL_INT64 mil_width, mil_height;
	BYTE *image_ptr;
	m_nGrabError[cam_idx] = 0;
	long image_length = 0;
	MIL_INT64 i = 0, x = 0, cnt;
	int grab_cnt = 0;
	if(cam_idx == 2)
	{
		cam_idx = 1;
	}
	//CIPSDlg * pdlg = (CIPSDlg *)AfxGetApp()->m_pMainWnd;  //pkh
	//CVisionModule*pdlg = (CVisionModule *)AfxGetApp()->m_pMainWnd;
	//CIPSDlg *pdlg =(CIPSDlg *)AfxGetApp()->m_pMainWnd;

	MdigControl(m_MilDigitizer[cam_idx], M_GRAB_ABORT, M_DEFAULT);	
	
	if(cam_idx == CAM_8K)
	{
		image_length = CAMERA_HEIGHT_8K;
		cnt = grab_height / image_length;
		
		width = MAX_HALCON_IMAGE_WIDTH_8K;
		height = MAX_HALCON_IMAGE_HEIGHT_8K;
		mil_width = CAMERA_WIDTH_8K;
		mil_height = height;
	}
	else if(cam_idx >= CAM_16K)
	{
		image_length = CAMERA_HEIGHT_16K;
		cnt = grab_height / image_length;
		
		width = MAX_HALCON_IMAGE_WIDTH_16K;
		height = MAX_HALCON_IMAGE_HEIGHT_16K;
		mil_width = CAMERA_WIDTH_16K;
		mil_height = height;
		cam_idx = CAM_16K;
	}

	MbufAlloc2d(m_MilSystem[cam_idx], mil_width, grab_height, 8 + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilLargeGrabBuf[cam_idx]);
	MbufClear(m_MilLargeGrabBuf[cam_idx], 0x00);
	MbufClear(m_MilGrabBuf[cam_idx][0], 0x00);
	MbufClear(m_MilGrabBuf[cam_idx][1], 0x00);
	m_bGrab[cam_idx] = TRUE;

	MappControl(M_ERROR, M_PRINT_DISABLE);
	CString strTemp;
	strTemp.Format(_T("Grab Bug Test1"));
	WriteLog(LOG_PATH, strTemp);

	MdigGrab(m_MilDigitizer[cam_idx], m_MilGrabBuf[cam_idx][0]);
	MdigGrabWait ( m_MilDigitizer[cam_idx], M_GRAB_END);  //pkh
	//CString strTemp;
	strTemp.Format(_T("Grab Bug Test2"));
	WriteLog(LOG_PATH, strTemp);

	//CString strSendDat, strTemp;
	//strSendDat.Format(_T("%c%s,OK,%d,%d,%d%c"), PACKET_CHAR_STX, PACKET_SEND_GRAB_READY_COMP, grabPara.nCamIdx, grabPara.nRotat[grabPara.nCamIdx], grabPara.nScanIdx[grabPara.nCamIdx], PACKET_CHAR_ETX);
	//TCPIPSendMCS(strSendDat);
	//strTemp.Format(_T("Test Image GrabReady OK"));
	//WriteLog(LOG_PATH, strTemp); //pkh

	//20160524 ngh
	if(m_bGrabStop[cam_idx])
	{
		MbufFree(m_MilLargeGrabBuf[cam_idx]);
		m_bGrab[cam_idx] = FALSE;
		m_nGrabError[cam_idx] = 1;
		return FALSE;
	}

	if(MappGetError(M_GLOBAL, M_NULL))
	{
		m_bGrab[cam_idx] = FALSE;
		MbufCopyClip(m_MilGrabBuf[cam_idx][0], m_MilLargeGrabBuf[cam_idx], 0, (image_length));
		m_nGrabError[cam_idx] = 2;
	}

	grab_cnt++;
	*_grab_cnt = grab_cnt - 1;    //20160803 pkh
 
	if(m_bGrab[cam_idx])
	{
		//Grab
		for(i = 0;i < (cnt); i++)
		{	
			x = 1 - (i % 2);
			//Grab
			MdigGrab(m_MilDigitizer[cam_idx], m_MilGrabBuf[cam_idx][x]);
			MdigGrabWait ( m_MilDigitizer[cam_idx], M_GRAB_END);  //pkh
			//20160524 ngh
			if(m_bGrabStop[cam_idx])
			{
				MbufFree(m_MilLargeGrabBuf[cam_idx]);
				m_bGrab[cam_idx] = FALSE;
				m_nGrabError[cam_idx] = 1;
				return FALSE;
			}

			if(MappGetError(M_GLOBAL, M_NULL))
			{
				//MbufFree(m_MilLargeGrabBuf[cam_idx]);
				m_bGrab[cam_idx] = FALSE;
				MbufCopyClip(m_MilGrabBuf[cam_idx][i%2], m_MilLargeGrabBuf[cam_idx], 0, (image_length * i));
				m_nGrabError[cam_idx] = 2;
				break;
				//return FALSE;
			}

			MbufCopyClip(m_MilGrabBuf[cam_idx][i%2], m_MilLargeGrabBuf[cam_idx], 0, (image_length * i));
			
			grab_cnt++;
			*_grab_cnt = grab_cnt - 1;
		}
	}

	MdigControl(m_MilDigitizer[cam_idx], M_GRAB_ABORT, M_DEFAULT);	

	MbufInquire(m_MilLargeGrabBuf[cam_idx], M_HOST_ADDRESS, &image_ptr);

	MappControl(M_ERROR, M_PRINT_ENABLE);

	gen_image1_extern(&m_HalGrabImage[cam_idx], "byte", width, grab_height, (Hlong)image_ptr, (Hlong)0);
	rotate_image(m_HalGrabImage[cam_idx], &m_HalTransImage[cam_idx], 90, "constant");
	if(cam_idx == CAM_8K)
	{
		if(derection == FORWARD_DERECTION)
		{
			rotate_image(m_HalTransImage[cam_idx], &m_HalTransImage[cam_idx], 180, "constant");
		}
		else if(derection == REVERSE_DERECTION)
		{
			rotate_image(m_HalTransImage[cam_idx], &m_HalTransImage[cam_idx], 180, "constant");
			mirror_image(m_HalTransImage[cam_idx], &m_HalTransImage[cam_idx], "column");
		}
		else
		{
			MbufFree(m_MilLargeGrabBuf[cam_idx]);
			m_bGrab[cam_idx] = FALSE;
			m_nGrabError[cam_idx] = 2;
			return FALSE;
		}
	}
	else if(cam_idx == CAM_16K)
	{
		if(derection == FORWARD_DERECTION)
		{
			
		}
		else if(derection == REVERSE_DERECTION)
		{
			mirror_image(m_HalTransImage[cam_idx], &m_HalTransImage[cam_idx], "column");
		}
		else
		{
			m_nGrabError[cam_idx] = 2;
			MbufFree(m_MilLargeGrabBuf[cam_idx]);
			m_bGrab[cam_idx] = FALSE;
			return FALSE;
		}
	}
	
	MbufFree(m_MilLargeGrabBuf[cam_idx]);
	//m_bGrab[cam_idx] = FALSE;

	*_grab_cnt = grab_cnt;
#endif
	
	return m_bGrab[cam_idx];
}

int CVisionModule::GetGrabError(int cam_idx)
{
	if(cam_idx == 2)
	{
		cam_idx = 1;
	}
	return m_nGrabError[cam_idx];
}

void CVisionModule::ShiftImage(int cam_idx, int offset, BOOL derection, Hobject *rotate_halcon_object)
{
	HTuple HomMat2DIdentity, HomMat2DTrans, HomMat2DScale;
	if(cam_idx == 2)
	{
		cam_idx = 1;
	}

	if(derection)
	{
		//Image Shift 이동///////////////////////////////////////////////////////////////
		hom_mat2d_identity(&HomMat2DIdentity);
		hom_mat2d_translate(HomMat2DIdentity, 0, offset, &HomMat2DTrans);
		affine_trans_image(m_HalTransImage[cam_idx],
			rotate_halcon_object, 
			HomMat2DTrans, HTuple("constant"),HTuple("false"));
	}
	else
	{
		*rotate_halcon_object = m_HalTransImage[cam_idx];
	}
}

MIL_ID *CVisionModule::GetMilSystem(int cam_idx)
{
	if(cam_idx == 2)
	{
		cam_idx = 1;
	}
	return &m_MilSystem[cam_idx];
}

MIL_ID *CVisionModule::GetMilDigitizer(int cam_idx)
{
	if(cam_idx == 2)
	{
		cam_idx = 1;
	}
	return &m_MilDigitizer[cam_idx];
}

MIL_ID *CVisionModule::GetMilLargeGrabBuf(int cam_idx)
{
	if(cam_idx == 2)
	{
		cam_idx = 1;
	}
	return &m_MilLargeGrabBuf[cam_idx];
}

//20140630 ngh SafeNet USB LockKey login
BOOL CVisionModule::SafeNet_Lock_LogIn()
{
	m_SafeNet_USB_feature = HASP_DEFAULT_FID;
	m_SafeNet_USB_handle = HASP_INVALID_HANDLE_VALUE;

	m_SafeNet_USB_status = hasp_login(m_SafeNet_USB_feature, m_SafeNet_USB_vendor_code, &m_SafeNet_USB_handle);

	if (m_SafeNet_USB_status != HASP_STATUS_OK)
	{
		AfxMessageBox(_T("USB LockKey를 확인해주세요"));
		return FALSE;
	}
	return TRUE;
}

//20140630 ngh SafeNet USB LockKey logout
void CVisionModule::SafeNet_Lock_LogOut()
{
	m_SafeNet_USB_status = hasp_logout(m_SafeNet_USB_handle);

	if (m_SafeNet_USB_status != HASP_STATUS_OK)
	{
		switch (m_SafeNet_USB_status)
		{
			case HASP_INV_HND:
				break;
			default:
				break;
		}
	}
}

BOOL CVisionModule::GetGrab(int cam_idx)
{
	if(cam_idx == 2)
	{
		cam_idx = 1;
	}
	return m_bGrab[cam_idx];
}

//20160524 ngh
void CVisionModule::SetGrabStop(int cam_idx, BOOL bStop)
{
	if(cam_idx == 2)
	{
		cam_idx = 1;
	}
	m_bGrabStop[cam_idx] = bStop;
}

BOOL CVisionModule::GetGrabStop(int cam_idx)
{
	if(cam_idx == 2)
	{
		cam_idx = 1;
	}
	return m_bGrabStop[cam_idx];
}
//#define _SH_DENYNO      0x40    /* deny none mode */
//#define MAX_STRING_LINE							30000
//BOOL CVisionModule::WriteLog(CString path, CString log, BOOL isList)
//{
//	FILE*	stream;
//	char	strFile[MAX_PATH], strToFile[MAX_PATH];
//    long	result, line;
//	char	szTime[DAT_STR_LENG]; 
//	CString bak_file, filePath, fileName, log_data;
//	char buffer[DAT_STR_LENG];
//	char temp[DAT_STR_LENG];
//	CTime time;
//	time = CTime::GetCurrentTime();
//	int limit_line = 0;
//	
//	path.Empty();	
//	bak_file.Format(_T("Log.bak"));
//
//	log_data.Format(_T("%02d:%02d:%02d - %s"), time.GetHour(), time.GetMinute(), time.GetSecond(), log);
//	sprintf_s(temp, path.GetLength() + 1,  "%S", path);
//	MakeDirectories((LPCWSTR)temp);
//
//	fileName.Format(_T("%04d%02d%02d.log"), time.GetYear(), time.GetMonth(), time.GetDay());
//	filePath.Format(_T("%s\\%s"),path, fileName);
//	 
//	sprintf_s(strFile, filePath.GetLength() + 1, "%S", filePath);
//
//	if((stream = _fsopen( strFile, "a+" , _SH_DENYNO)) == NULL)
//	{
//		HANDLE fd = CreateFile( (LPCWSTR)(LPCSTR)strFile,
//							GENERIC_READ|GENERIC_WRITE,
//							FILE_SHARE_READ|FILE_SHARE_WRITE,
//							NULL,
//							OPEN_ALWAYS,
//							FILE_ATTRIBUTE_NORMAL,
//							NULL
//							);
//		stream = _fsopen( strFile, "a+" , _SH_DENYNO);
//		if(stream == NULL)
//		{
//			return FALSE;
//		}
//
//		CloseHandle(fd);
//	}
//
//	Check first time
//    result = fseek(stream, 0L, SEEK_SET);
//    if(result)
//	{
//		fclose(stream);
//		return FALSE;
//	}
//	line = 0;
//	while(fgets(buffer, MAX_STRING_LINE, stream)) 
//	{
//		line++;
//		if(1 < line)
//		{
//			break;
//		}
//	}
//
//    result = fseek(stream, 0L, SEEK_END);
//    if(result)
//	{
//		fclose(stream);
//		return FALSE;
//	}
//
//	memset(szTime, NULL, sizeof(szTime));
//	sprintf_s(szTime, log_data.GetLength() + 1, "%S", log_data);
//    fputs(szTime, stream);
//	fputs("\n", stream);
//
//    result = fseek(stream, 0L, SEEK_SET);
//    if(result)
//	{
//		fclose(stream);
//		return FALSE;
//	}
//
//	Check limit line
//	line = 0;
//	if(limit_line)
//	{
//		while(fgets(buffer, MAX_STRING_LINE, stream)) line++;
//		if(line > limit_line)
//		{
//			sprintf_s(strToFile, filePath.GetLength() + bak_file.GetLength() + 2 + 1, "%S\\%S", path, bak_file);
//			CopyFile((LPCWSTR)(LPCSTR)strFile, (LPCWSTR)(LPCSTR)strToFile, FALSE);
//			fclose(stream);
//			DeleteFile((LPCWSTR)(LPCSTR)strFile);
//		}
//		else
//		{
//			fclose(stream);
//		}
//	}
//	else
//	{
//		fclose(stream);
//	}
//
//	return TRUE;
//}

BOOL CVisionModule::WriteLog(CString path, CString log, BOOL isList)
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
	//MakeDirectories((LPCWSTR)temp);

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
	while(fgets(buffer, 30000, stream)) 
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
		while(fgets(buffer, 30000, stream)) line++;
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