#pragma once

enum {
	IF_POINT0 = 0x0001,
	IF_POINT1 = 0x0002,
	IF_POINT2 = 0x0004,
	IF_POINT3 = 0x0008,
	IF_POINT4 = 0x0010,
	IF_POINT5 = 0x0020,
	IF_POINT6 = 0x0040,
	IF_POINT7 = 0x0080,
	IF_POINT8 = 0x0100,
	IF_POINT9 = 0x0200,
};

struct INPUT_FILTER_T
{
	int pfilter;

	BOOL IsFilterPnt(int id) {
		return !(pfilter & (0x0001 << id));
	}

	void SetPoint(int id, BOOL bShow) {
		if (bShow) {
			pfilter |= (0x0001 << id);
		}
		else {
			pfilter &= ~(0x0001 << id);
		}
	}

	void Init() {
		pfilter = IF_POINT0;
		pfilter |= IF_POINT1 | IF_POINT2 | IF_POINT3 | IF_POINT4;
		pfilter |= IF_POINT5 | IF_POINT6 | IF_POINT7 | IF_POINT8;
		pfilter |= IF_POINT9;
	}

};


// CDialogFilters 对话框

class CDialogFilters : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogFilters)

public:
	CDialogFilters(INPUT_FILTER_T *pift, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogFilters();

// 对话框数据
	enum { IDD = IDD_DLG_EXTRA_FILTERS };

private:
	INPUT_FILTER_T *m_pIft;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetCheckStatus(UINT id, BOOL bCheck);
	BOOL GetCheckStatus(UINT id);
	afx_msg void OnBnClickedCheckP0();
	afx_msg void OnBnClickedCheckP1();
	afx_msg void OnBnClickedCheckP2();
	afx_msg void OnBnClickedCheckP3();
	afx_msg void OnBnClickedCheckP4();
	afx_msg void OnBnClickedCheckP5();
	afx_msg void OnBnClickedCheckP6();
	afx_msg void OnBnClickedCheckP7();
	afx_msg void OnBnClickedCheckP8();
	afx_msg void OnBnClickedCheckP9();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnReset();
};
