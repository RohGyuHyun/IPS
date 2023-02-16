#pragma once
#include "afxwin.h"


// CDialogSetupTeaching 대화 상자입니다.

class CDialogSetupTeaching : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSetupTeaching)

public:
	CDialogSetupTeaching(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSetupTeaching();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_TEACHING };

	int Get8KLongWidth();
	int Get8KLongHeight();
	int Get8KShortWidth();
	int Get8KShortHeight();

	int Get16KLongWidth();
	int Get16KLongHeight();
	int Get16KShortWidth();
	int Get16KShortHeight();

	int Get8KLongOffset();
	int Get8KShortOffset();

	int Get16KLongOffset();
	int Get16KShortOffset();

	int Get8KLongDeafultLineX();
	int Get8KLongDeafultLineY();

	int Get16KLongDeafultLineX();
	int Get16KLongDeafultLineY();

	double Get8KThresh0(int idx);

	int Get8KDefecSize0();

	int Get8KInspectionCnt0();

	int Get8KNoiseSize(int idx);
	double Get8KMargeDistance();

	void Update(BOOL isUpdate);
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	long m_edit_8k_long_width;
	long m_edit_8k_long_height;
	long m_edit_8k_short_width;
	long m_edit_8k_short_height;

	long m_edit_16k_long_width;
	long m_edit_16k_long_height;
	long m_edit_16k_short_width;
	long m_edit_16k_short_height;

	int m_edit_8k_long_deretion_offset;
	int m_edit_8k_short_deretion_offset;

	int m_edit_16k_long_deretion_offset;
	int m_edit_16k_short_deretion_offset;

	int m_edit_8k_long_deafult_line_x;
	int m_edit_8k_long_deafult_line_y;

	int m_edit_16k_long_defult_line_x;
	int m_edit_16k_long_defult_line_y;

	double m_edit_thrsh0;

	int m_edit_8k_defect_size0;

	int m_edit_8k_inspection_cnt0;

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrl8kLongSave();
	void ClickBtnenhctrl8kShortSave();
	void ClickBtnenhctrl16kLongSave();
	void ClickBtnenhctrl16kShortSave();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ClickBtnenhctrl8kTest();
	void ClickBtnenhctrl8kLongOffsetMove();
	void ClickBtnenhctrlTeachingImageLoad();
	double m_edit_thrsh1;
	double m_edit_thrsh2;
	int m_edit_nNoiseSize0;
	int m_edit_nNoiseSize1;
	int m_edit_nNoiseSize2;
	double m_edit_dMargeDistance;
	double m_edit_dTest0;
	double m_edit_dTest1;
	double m_edit_dTest2;
	double m_edit_dTest3;
	double m_edit_dTest4;
	double m_edit_thrsh3;
	CComboBox m_combo_SelectDefect;
	afx_msg void OnCbnCloseupComboSelectDefect();
	double m_edit_thrsh4;
	int m_edit_nMaxDefectCnt;
};
