
// InputEventAnalyzerDlg.h : 头文件
//
#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <set>
using std::set;
#include "EventAnalyzer.h"
#include "DialogFilters.h"

#define UWM_UPDATE_EVENT		(WM_USER + 0x134)
#define UWM_UPDATE_EVENT_DETAIL	(WM_USER + 0x234)
#define UWM_UPDATE_PAUSE_TEXT	(WM_USER + 0x345)
#define UWM_UPDATE_PLAY_ITEMS	(WM_USER + 0x456)

enum UpdateListType {
	ULT_UPDATE_POINT_LIST = 0x01,
	ULT_UPDATE_EVENT_BOX = 0x02,
	ULT_UPDATE_ALL = ULT_UPDATE_POINT_LIST | ULT_UPDATE_EVENT_BOX,
};

class CPointPlayController
{
public:
	CPointPlayController() : cit_spnts(show_points.cbegin()),
		sleepms(100), index(-1), framebyframe(FALSE)
	{
		filter.Init();
	}

	void ResetIndex() {
		index = -1;
	}

	void NextFrame() {
		framebyframe = TRUE;
	}

	void SpeedMinus() {
		if (sleepms <= 5000) {
			sleepms += 50;
		}
	}
	void SpeedPlus() {
		if (sleepms > 50) {
			sleepms -= 50;
		}
		else {
			sleepms = 1;
		}
	}

	double GetSpeed() const {
		return 1000.0 / sleepms;
	}

	void ResetItems(CPointPlayController &ctrler);

	int GetNextTraceIndex();
	BOOL FilterPoint(int id) {
		return filter.IsFilterPnt(id);
	}
public:
	INPUT_FILTER_T filter;
	map<int, vector<TPTOUCH_T> > show_points;
	map<int, vector<TPTOUCH_T> >::const_iterator cit_spnts;
	EVENT_DEV_T evdev;
	BOOL framebyframe;
	int index;
	int sleepms;
};


inline BOOL IsUpdateEventBox(int type) 
{
	return !!(type & ULT_UPDATE_EVENT_BOX);
}

inline BOOL IsUpdatePointList(int type)
{
	return !!(type & ULT_UPDATE_POINT_LIST);
}

// CInputEventAnalyzerDlg 对话框
class CInputEventAnalyzerDlg : public CDialogEx
{
// 构造
public:
	CInputEventAnalyzerDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CInputEventAnalyzerDlg();

// 对话框数据
	enum { IDD = IDD_INPUTEVENTANALYZER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
	void SetPauseText(BOOL bPause);
	void DetectError();
	void EnterDrawPointsHalt();
	void ExitDrawPointsHalt();

public:
	void UpdateList(int dev_no);
	void UpdateCtrls();
	void UpdateDetailList(int dev_no, int item_no, int type = ULT_UPDATE_ALL);
	void UpdateDetailList(int dev_no, set<int>& item_nos, int type = ULT_UPDATE_ALL);
	void UpdateEventDetail(int type);
	void CheckListAll(BOOL bCheck);
	void EnableCtrls(BOOL bEnable);

	int CurSelToDevNo();
	void InitDraw();
	void DrawPoint(int index);
	

public:
	BOOL IsShowPointTrace() const {return m_bShowPointsTrace;}
	BOOL IsShowPointHalted() const { return m_bHaltShowPoints; }
	void ResetScreen();
	BOOL IsShowPointerLocation() const { return m_bShowPointerLocation;}
public:
	CPointPlayController m_ctrler;
	void UpdatePlayCtrler();

private:
	BOOL m_bCtrlNeedUpdate;
	CPointPlayController m_ctrlerTmp;
	BOOL m_bListUpdating;
	BOOL m_bShowPointsTrace;
	BOOL m_bHaltShowPoints;
	BOOL m_bShowPointerLocation;

	static const int POINT_RADIUS = 6;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBrowseEventLog();
	CEdit m_editEventLogPath;
	CListCtrl m_listEvents;
	CEdit m_editAppLog;

	LRESULT OnUpdatePlayItems(WPARAM wParam, LPARAM lParam);
	void SelectEventItem(int nItem);
	LRESULT OnUpdatePauseText(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateListCheck(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLvnColumnclickListEvents(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_cbEventDevs;
	afx_msg void OnLvnItemchangingListEvents(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox m_listBoxEventsDetail;
	afx_msg void OnHdnItemclickListEvents(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListEvents(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_chkEventLabel;
	afx_msg void OnBnClickedCheckEventLabel();
	CButton m_chkShowPoints;
	afx_msg void OnBnClickedCheckShowPoints();
	afx_msg void OnCbnSelchangeComboEventDevs();
	afx_msg void OnBnClickedCheckSelectAll();
	afx_msg void OnBnClickedBtnPrevious();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnNext();
	CButton m_chkSelectAll;
	CButton m_btnPlay;
	CStatic m_staticShowPoint;
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheckPointerLocation();
	CButton m_chkPointerLocation;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPlayopsPause();
	afx_msg void OnPlayopsPlaycheckeditems();
	afx_msg void OnBnClickedBtnPause();
	CButton m_btnPause;
	afx_msg void OnBnClickedBtnErrors();
	CButton m_btnErrorNext;
	afx_msg void OnBnClickedBtnExtraFilters();
	afx_msg void OnPlayopsNextframe();
	afx_msg void OnPlayopsPrevframe();
	afx_msg void OnPlayopsSpeedMinus();
	afx_msg void OnPlayopsSpeedPlus();
	CButton m_btnPrev;
	CButton m_btnNext;
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
