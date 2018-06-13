// InputEventAnalyzerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "InputEventAnalyzer.h"
#include "InputEventAnalyzerDlg.h"
#include "afxdialogex.h"
//#include "color256.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CRITICAL_SECTION g_crtsec;
CCriticalSection g_cs;
CInputEventAnalyzerDlg *g_pMainDlg;
HWND g_hMainWnd;

#define CS_LOCK() {\
	/*TRACE("%s: lock\n", __FUNCTION__);*/\
	TryEnterCriticalSection(&g_crtsec);\
	/*g_cs.Lock();*/\
}

#define CS_UNLOCK() {\
	/*TRACE("%s: unlock\n", __FUNCTION__);*/\
	LeaveCriticalSection(&g_crtsec);\
	/*g_cs.Unlock();*/\
}

CString g_strToolDate;
CString g_strCopyRight;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CInputEventAnalyzerDlg 对话框

void SetCompileDateTime()
{
	const char *szEnglishMonth[12]= {
		"Jan","Feb","Mar","Apr","May","Jun",
		"Jul","Aug","Sep","Oct","Nov","Dec",
	};
	char szTmpDate[100]={0}; 
	char szTmpTime[100]={0}; 
	char szMonth[10]={0}; 
	wchar_t szDateTime[250] = {0};
	int iYear, iMonth, iDay;
	int iHour,iMin,iSec;

	//获取编译日期、时间 
	sprintf_s(szTmpDate,"%s", __DATE__); //"Sep 18 2010" 
	sprintf_s(szTmpTime,"%s", __TIME__); //"10:59:19" 

	memcpy(szMonth, szTmpDate, 3);
	for (int i=0; i < 12; i++) 
	{ 
		if (strncmp(szMonth, szEnglishMonth[i],3) == 0) 
		{ 
			iMonth=i+1; 
			break; 
		} 
	} 
	memset(szMonth, 0, 10);
	memcpy(szMonth, szTmpDate+4, 2);
	iDay = atoi(szMonth);

	memset(szMonth, 0, 10);
	memcpy(szMonth, szTmpDate+7, 4);
	iYear = atoi(szMonth);


	memset(szMonth, 0, 10);
	memcpy(szMonth, szTmpTime, 2);
	iHour = atoi(szMonth);

	memset(szMonth, 0, 10);
	memcpy(szMonth, szTmpTime+3, 2);
	iMin = atoi(szMonth);


	memset(szMonth, 0, 10);
	memcpy(szMonth, szTmpTime+6, 2);
	iSec = atoi(szMonth);


	wsprintf(szDateTime,TEXT("Build@%4d%02d%02d_%02d%02d%02d"),
		iYear,iMonth,iDay,iHour,iMin,iSec); 

	TRACE(szDateTime);
	TRACE("\n");
	g_strToolDate = szDateTime;


	sprintf_s(szTmpDate, "FocalTech Copyright(C) %d", iYear);
	g_strCopyRight = szTmpDate;

	//g_strToolDate = TEXT("AdbTool, V1.0");
}

CInputEventAnalyzerDlg::CInputEventAnalyzerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputEventAnalyzerDlg::IDD, pParent), 
	m_bShowPointsTrace(FALSE),
	m_bShowPointerLocation(FALSE),
	m_bHaltShowPoints(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CInputEventAnalyzerDlg::~CInputEventAnalyzerDlg()
{
	CS_LOCK();
	CS_UNLOCK();
	DeleteCriticalSection(&g_crtsec);
}

void CInputEventAnalyzerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_EVENTLOG_PATH, m_editEventLogPath);
	DDX_Control(pDX, IDC_LIST_EVENTS, m_listEvents);
	DDX_Control(pDX, IDC_EDIT_APP_LOG, m_editAppLog);
	DDX_Control(pDX, IDC_COMBO_EVENT_DEVS, m_cbEventDevs);
	DDX_Control(pDX, IDC_LIST_EVENTS_DETAIL, m_listBoxEventsDetail);
	DDX_Control(pDX, IDC_CHECK_EVENT_LABEL, m_chkEventLabel);
	DDX_Control(pDX, IDC_CHECK_SHOW_POINTS, m_chkShowPoints);
	DDX_Control(pDX, IDC_CHECK_SELECT_ALL, m_chkSelectAll);
	DDX_Control(pDX, IDC_BTN_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_STATIC_SCREEN, m_staticShowPoint);
	DDX_Control(pDX, IDC_CHECK_POINTER_LOCATION, m_chkPointerLocation);
	DDX_Control(pDX, IDC_BTN_PAUSE, m_btnPause);
	DDX_Control(pDX, IDC_BTN_ERRORS, m_btnErrorNext);
	DDX_Control(pDX, IDC_BTN_PREVIOUS, m_btnPrev);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
}

BEGIN_MESSAGE_MAP(CInputEventAnalyzerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UWM_UPDATE_PAUSE_TEXT, &CInputEventAnalyzerDlg::OnUpdatePauseText)
	ON_MESSAGE(UWM_UPDATE_EVENT_DETAIL, &CInputEventAnalyzerDlg::OnUpdateListCheck)
	ON_MESSAGE(UWM_UPDATE_EVENT, &CInputEventAnalyzerDlg::OnUpdateEvent)
	ON_BN_CLICKED(IDC_BTN_BROWSE_EVENT_LOG, &CInputEventAnalyzerDlg::OnBnClickedBtnBrowseEventLog)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_EVENTS, &CInputEventAnalyzerDlg::OnLvnColumnclickListEvents)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_EVENTS, &CInputEventAnalyzerDlg::OnLvnItemchangingListEvents)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CInputEventAnalyzerDlg::OnHdnItemclickListEvents)
	ON_NOTIFY(NM_CLICK, IDC_LIST_EVENTS, &CInputEventAnalyzerDlg::OnNMClickListEvents)
	ON_BN_CLICKED(IDC_CHECK_EVENT_LABEL, &CInputEventAnalyzerDlg::OnBnClickedCheckEventLabel)
	ON_BN_CLICKED(IDC_CHECK_SHOW_POINTS, &CInputEventAnalyzerDlg::OnBnClickedCheckShowPoints)
	ON_CBN_SELCHANGE(IDC_COMBO_EVENT_DEVS, &CInputEventAnalyzerDlg::OnCbnSelchangeComboEventDevs)
	ON_BN_CLICKED(IDC_CHECK_SELECT_ALL, &CInputEventAnalyzerDlg::OnBnClickedCheckSelectAll)
	ON_BN_CLICKED(IDC_BTN_PREVIOUS, &CInputEventAnalyzerDlg::OnBnClickedBtnPrevious)
	ON_BN_CLICKED(IDC_BTN_PLAY, &CInputEventAnalyzerDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CInputEventAnalyzerDlg::OnBnClickedBtnNext)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_POINTER_LOCATION, &CInputEventAnalyzerDlg::OnBnClickedCheckPointerLocation)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_PLAYOPS_PAUSE, &CInputEventAnalyzerDlg::OnPlayopsPause)
	ON_COMMAND(ID_PLAYOPS_PLAYCHECKEDITEMS, &CInputEventAnalyzerDlg::OnPlayopsPlaycheckeditems)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CInputEventAnalyzerDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_ERRORS, &CInputEventAnalyzerDlg::OnBnClickedBtnErrors)
	ON_BN_CLICKED(IDC_BTN_EXTRA_FILTERS, &CInputEventAnalyzerDlg::OnBnClickedBtnExtraFilters)
	ON_COMMAND(ID_PLAYOPS_NEXTFRAME, &CInputEventAnalyzerDlg::OnPlayopsNextframe)
	ON_COMMAND(ID_PLAYOPS_PREVFRAME, &CInputEventAnalyzerDlg::OnPlayopsPrevframe)
	ON_COMMAND(ID_PLAYOPS_SPEED_MINUS, &CInputEventAnalyzerDlg::OnPlayopsSpeedMinus)
	ON_COMMAND(ID_PLAYOPS_SPEED_PLUS, &CInputEventAnalyzerDlg::OnPlayopsSpeedPlus)
	ON_MESSAGE(UWM_UPDATE_PLAY_ITEMS, &CInputEventAnalyzerDlg::OnUpdatePlayItems)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CInputEventAnalyzerDlg 消息处理程序

BOOL CInputEventAnalyzerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetCompileDateTime();
	
	CString strTitle(TEXT("InputEventAnalyzer "));
	strTitle += g_strToolDate;
	SetWindowTextW(strTitle);

	InitializeCriticalSection(&g_crtsec);

	m_chkEventLabel.SetCheck(BST_CHECKED);
	m_chkShowPoints.SetCheck(BST_CHECKED);

	m_listEvents.InsertColumn(0, TEXT("No."));
	m_listEvents.InsertColumn(1, TEXT("Down"));
	m_listEvents.InsertColumn(2, TEXT("Up"));
	m_listEvents.InsertColumn(3, TEXT("Flag"));

	m_listEvents.SetColumnWidth(0, 50);
	m_listEvents.SetColumnWidth(1, 100);
	m_listEvents.SetColumnWidth(2, 100);
	m_listEvents.SetColumnWidth(3, 40);

	DWORD dwStyle = m_listEvents.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_listEvents.SetExtendedStyle(dwStyle); //设置扩展风格

	CEventPair::InitEventPairs();
	CEventDiagnoser::InitSymbolicErrors();

	g_hMainWnd = GetSafeHwnd();
	g_pMainDlg = this;

	m_ctrler.filter.Init();
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInputEventAnalyzerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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



void CInputEventAnalyzerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CInputEventAnalyzerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CInputEventAnalyzerDlg::UpdateList(int dev_no)
{
	map<int, vector<TPIRQ_T> >::const_iterator cit = Analyzer.m_mapDevTpirqs.begin();

	m_listEvents.DeleteAllItems();
	m_listBoxEventsDetail.ResetContent();
	for (; cit != Analyzer.m_mapDevTpirqs.end(); cit++)
	{
		if (cit->first != dev_no)
		{
			continue;
		}
		m_bListUpdating = TRUE;
		vector<TPIRQ_T>::const_iterator irqit = cit->second.begin();
		for (int i = 1; irqit != cit->second.end(); irqit++, i++)
		{
			CString strNo, strDown, strUp;
			strNo.Format(TEXT("%03d"), i);

			vector<TPTOUCH_T>::const_iterator tchit = irqit->touches.begin();
			for (; tchit != irqit->touches.end(); tchit++)
			{
				if (tchit->type == TPTYPE_KEY)
				{
					if (tchit->tracking_id == 1)
					{
						strDown.AppendFormat(TEXT("%s, "), CEventPair::U16ToKey(tchit->key));
					}
					else 
					{
						strUp.AppendFormat(TEXT("%s, "), CEventPair::U16ToKey(tchit->key));
					}
				}
				else if (tchit->type == TPTYPE_TOUCH)
				{
					if (irqit->protocol == MT_PROTOCOL_B)
					{
						if (!TPIRQ_T::IsSlotActive(tchit->slot, tchit->tracking_id))
						{
							strUp.AppendFormat(TEXT("P%d,"), tchit->slot);
						}
						else {
							strDown.AppendFormat(TEXT("P%d,"), tchit->slot);
						}
					}
					else
					{
						strDown.AppendFormat(TEXT("P%d,"), tchit->tracking_id);
					}
				}

			}

			if (strDown.IsEmpty())
			{
				strDown = TEXT("-");
			}
			if (strUp.IsEmpty())
				strUp = TEXT("-");

			int count = m_listEvents.GetItemCount();
			int nRow = m_listEvents.InsertItem(count, strNo);
			m_listEvents.SetItemText(nRow, 1 , strDown);
			m_listEvents.SetItemText(nRow, 2, strUp);

			CString strFlag;
			if (irqit->dgnflag > ME_ERROR_NULL)
			{
				strFlag.Format(TEXT("%04X"), irqit->dgnflag);
			}
			m_listEvents.SetItemText(nRow, 3, strFlag);
		}

		m_bListUpdating = FALSE;
		UpdateDetailList(dev_no, 0);
		break;
	}

}

void CPointPlayController::ResetItems(CPointPlayController &ctrler)
{
	this->show_points = ctrler.show_points;
	this->cit_spnts = this->show_points.cbegin();
	this->evdev = ctrler.evdev;
	this->framebyframe = this->show_points.size() == 1;

	if ((this->index != -1))
	{
		if (ctrler.show_points[this->index].empty())
		{
			return;
		}
		this->cit_spnts = this->show_points.find(this->index);
	}
}

LRESULT CInputEventAnalyzerDlg::OnUpdatePlayItems(WPARAM wParam, LPARAM lParam)
{
	m_bCtrlNeedUpdate = TRUE;
	ExitDrawPointsHalt();
	return 0;
}

void CInputEventAnalyzerDlg::UpdateCtrls()
{
	m_cbEventDevs.ResetContent();

	map<int, EVENT_DEV_T>::const_iterator cit = Analyzer.m_mapEventDevices.begin();

	int ts_no = 0;
	UINT maxEvent = 0;
	int sel = 0;
	for(int i = 0; cit != Analyzer.m_mapEventDevices.end(); cit++, i++)
	{
		CString strItem;
		strItem.Format(TEXT("event%d(%s%s)"), cit->first, 
			(cit->second.protocol == MT_PROTOCOL_B) ? TEXT("[B]") : TEXT(""),
			cit->second.name );

		m_cbEventDevs.AddString(strItem);
		if (cit->second.abses.size() > maxEvent)
		{
			maxEvent = cit->second.abses.size();
			ts_no = cit->first;
			sel = i;
		}
	}
	m_cbEventDevs.SetCurSel(sel);

	UpdateList(CurSelToDevNo());
}

LRESULT CInputEventAnalyzerDlg::OnUpdateListCheck(WPARAM wParam, LPARAM lParam)
{
	int type = (int)wParam;
	UpdateEventDetail(type);

	return 0;
}

LRESULT CInputEventAnalyzerDlg::OnUpdateEvent(WPARAM wParam, LPARAM lParam)
{
	CString strInfo;
	strInfo = Analyzer.GetAnalysisResult();
	m_editAppLog.SetWindowTextW(strInfo);

	UpdateCtrls();
	return 0;
}

#define COLOR_YELLOW 0xFFFF00FF
#define COLOR_PURPLE 0x800080FF
#define COLOR_RED 0xFF0000FF
#define COLOR_ORANGE 0xFFA500FF
#define COLOR_GRAY 0x808080FF
#define COLOR_CYAN 0x00FFFFFF
#define COLOR_BROWN 0xA52A2AFF
#define COLOR_BLUE 0x0000FFFF
#define COLOR_GREEN 0x008000FF
#define COLOR_AQUA 0x00FFFFFF
#define COLOR_ANTIQUEWHITE 0xFAEBD7FF

#define _NTP(x) (DWORD)(x >> 8)  // no transparency

void CInputEventAnalyzerDlg::DrawPoint(int index)
{
	static COLORREF DRAW_CLR[10] = {
		_NTP(COLOR_RED), _NTP(COLOR_GREEN), _NTP(COLOR_YELLOW),
		_NTP(COLOR_BLUE), _NTP(COLOR_GRAY), _NTP(COLOR_AQUA),
		_NTP(COLOR_ANTIQUEWHITE), _NTP(COLOR_BROWN),
		_NTP(COLOR_CYAN), _NTP(COLOR_HIGHLIGHT),
	};

	CDC *pDC = GetDC();
	CRect rc;
	CDC dcMem;
	double xProp;
	double yProp;
	m_staticShowPoint.GetWindowRect(&rc);
	CBitmap bmp; //内存中承载临时图象的位图

	ScreenToClient(&rc);

	rc.top++;
	rc.left++;
	rc.bottom--;
	rc.right--;

	// 创建兼容DC
	dcMem.CreateCompatibleDC(pDC); 
	bmp.CreateCompatibleBitmap(pDC,rc.Width(),rc.Height());
	CBitmap *pOldBit=dcMem.SelectObject(&bmp);
	dcMem.FillSolidRect(rc,RGB(0,0,0));
	dcMem.SetTextColor(_NTP(COLOR_ANTIQUEWHITE));
	
	CFont ft;
	LOGFONT logft;
	memset(&logft, 0, sizeof(LOGFONT));  
	CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT))->GetLogFont(&logft);
	logft.lfHeight = 12;
	ft.CreateFontIndirect(&logft);
	CFont *pOldFont = dcMem.SelectObject(&ft);

	map<int, vector<TPTOUCH_T> >::const_iterator cit = m_ctrler.show_points.find(index);
	if ((m_ctrler.evdev.x_res != 0) && (cit != m_ctrler.show_points.end()))
	{
		CString strPntNo;
		CPoint pntTxt;
		CPoint pntDraw;
		CRect rcText;
		int nFollowDis;

		pntTxt.x = 4;
		pntTxt.y = 4;

		xProp = rc.Width() / (m_ctrler.evdev.x_res * 1.0);
		yProp = rc.Height() / (m_ctrler.evdev.y_res * 1.0);


		CString text;
		text.Format(TEXT("[%d]count: %d (%.1f f/s)"), index+1, cit->second.size(), m_ctrler.GetSpeed());
		dcMem.TextOutW(pntTxt.x, pntTxt.y, text);

		TEXTMETRIC tm;
		dcMem.GetTextMetrics(&tm);

		if (IsShowPointerLocation())
		{
			map<int, vector<TPTOUCH_T> >::const_iterator citEnd = cit;
			map<int, vector<TPTOUCH_T> >::const_iterator citBeg = m_ctrler.show_points.begin();
			for (; citBeg != citEnd; citBeg++)
			{
				vector<TPTOUCH_T>::const_iterator pit = citBeg->second.begin();
				for (; pit != citBeg->second.end(); pit++)
				{
					if (pit->type == TPTYPE_TOUCH)
					{
						int id = pit->tracking_id % 10;
						int point_id = id;
						if (m_ctrler.evdev.protocol == MT_PROTOCOL_B)
						{
							point_id = pit->slot;
						}
						if (!m_ctrler.filter.IsFilterPnt(point_id)) 
						{
							pntDraw.x = (LONG)(xProp * pit->x);
							pntDraw.y = (LONG)(yProp * pit->y);
							dcMem.SetPixel(pntDraw, _NTP(COLOR_YELLOW));
						}
					}
				}
			}
		}

		vector<TPTOUCH_T>::const_iterator pit = cit->second.begin();
		for (; pit != cit->second.end(); pit++)
		{
			int id = pit->tracking_id % 10;
			int point_id = id;
			dcMem.SetDCBrushColor(DRAW_CLR[id]);

			if (pit->type == TPTYPE_KEY)
			{
				text.Format(TEXT("Key: %s %s"), CEventPair::U16ToKey(pit->key),
					(pit->tracking_id == 1) ? TEXT("DOWN") : TEXT("UP"));
			}
			else
			{
				pntDraw.x = (LONG)(xProp * pit->x);
				pntDraw.y = (LONG)(yProp * pit->y);

				if (m_ctrler.evdev.protocol == MT_PROTOCOL_B)
				{
					point_id = pit->slot;
					if (pit->tracking_id == -1)
					{
						text.Format(TEXT("P%d UP"), pit->slot);

						pntTxt.y += tm.tmHeight + 1;
						dcMem.TextOutW(pntTxt.x, pntTxt.y, text);
						continue;
					}
					else
					{
						strPntNo.Format(TEXT("p%d"), pit->slot);
						text.Format(TEXT("P%d(%d,%d)(p:%d,tm:%d) DOWN"), pit->slot, 
							pit->x, pit->y, pit->pressure, pit->touch_major);
					}
				}
				else
				{
					point_id = pit->tracking_id;
					strPntNo.Format(TEXT("p%d"), pit->tracking_id);
					text.Format(TEXT("P%d(%d,%d)(p:%d,tm:%d) DOWN"), pit->tracking_id, 
						pit->x, pit->y, pit->pressure, pit->touch_major);
				}

				/*dcMem.Ellipse(pntDraw.x - POINT_RADIUS, pntDraw.y - POINT_RADIUS,
				pntDraw.x + POINT_RADIUS, pntDraw.y + POINT_RADIUS);*/

				if (m_ctrler.filter.IsFilterPnt(point_id)) {
					continue;
				}
				
				rcText.left = pntDraw.x - POINT_RADIUS;
				rcText.top = pntDraw.y - POINT_RADIUS;
				rcText.bottom = pntDraw.y + POINT_RADIUS;
				rcText.right = pntDraw.x + POINT_RADIUS;

				dcMem.Ellipse(&rcText);

				nFollowDis = 2 * POINT_RADIUS + 5;
				nFollowDis *= (pntDraw.x < (rc.Width()/5)) ? (1) : (-1);
				rcText.left += nFollowDis;
				rcText.right += nFollowDis;

				nFollowDis = 2 * POINT_RADIUS + 5;
				nFollowDis *= (pntDraw.y < (rc.Height()/5)) ? (1) : (-1);
				rcText.top += nFollowDis;
				rcText.bottom += nFollowDis;
				
				dcMem.DrawText(strPntNo, &rcText, DT_CENTER | DT_SINGLELINE);

			}
			pntTxt.y += tm.tmHeight + 1;
			dcMem.TextOutW(pntTxt.x, pntTxt.y, text);
		}
	
	}

	pDC->BitBlt(rc.left,rc.top,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);

	//将内存DC上的图象拷贝到前台
	//绘图完成后的清理
	dcMem.SelectObject(pOldFont);
	dcMem.DeleteDC();     //删除DC
	bmp.DeleteObject(); //删除位图

	return;
}

void CInputEventAnalyzerDlg::EnableCtrls(BOOL bEnable)
{
	static UINT CTRLS[] = {
		IDC_BTN_BROWSE_EVENT_LOG, IDC_COMBO_EVENT_DEVS, IDC_EDIT_EVENTLOG_PATH,
		IDC_CHECK_EVENT_LABEL, IDC_CHECK_SHOW_POINTS,
		IDC_CHECK_POINTER_LOCATION, IDC_BTN_NEXT, IDC_BTN_PLAY,
		IDC_BTN_PREVIOUS, IDC_CHECK_SELECT_ALL, IDC_BTN_PAUSE,
		IDC_BTN_ERRORS,IDC_BTN_EXTRA_FILTERS, IDC_COMBO_EVENT_DEVS, IDC_LIST_EVENTS, 
		IDC_LIST_EVENTS_DETAIL,
	};

	int count = sizeof(CTRLS) / sizeof(UINT);
	for (int i = 0; i < count; i++)
	{
		CWnd *pWnd = GetDlgItem(CTRLS[i]);
		if (pWnd)
		{
			pWnd->EnableWindow(bEnable);
		}
	}
	if (!bEnable)
	{
		CButton *pChkSelectAll = (CButton*)(GetDlgItem(IDC_CHECK_SELECT_ALL));
		pChkSelectAll->SetCheck(FALSE);
	}
}

unsigned WINAPI ThreadParseEventLog(LPVOID lP)
{
	CString *pPath = (CString *)lP;

	g_pMainDlg->EnableCtrls(FALSE);

	Analyzer.OpenGeteventLog(*pPath);
	delete pPath;
	PostMessage(g_hMainWnd, UWM_UPDATE_EVENT, 0, 0);

	g_pMainDlg->EnableCtrls(TRUE);
	return 0;
}

void CInputEventAnalyzerDlg::UpdateDetailList(int dev_no, int item_no, int type)
{
	set<int> item_nos;
	item_nos.insert(item_no);

	UpdateDetailList(dev_no, item_nos, type);
}

void CInputEventAnalyzerDlg::UpdateDetailList(int dev_no, set<int>& item_nos, int type)
{
	map<int, vector<TPIRQ_T> >::const_iterator cit = Analyzer.m_mapDevTpirqs.find(dev_no);

	map<int, vector<EVENT_T> > mapEventStacks;
	
	map<int, vector<EVENT_T> >::const_iterator stackit;

	set<int>::const_iterator noit = item_nos.begin();
	for (; noit != item_nos.end(); noit++)
	{
		const vector<EVENT_T> &evStack = (cit->second).at(*noit).event_stack;
		
		mapEventStacks[*noit] = evStack;
	}

	stackit = mapEventStacks.begin();
	if (IsUpdateEventBox(type)) 
	{
		m_listBoxEventsDetail.ResetContent();
	}

	m_ctrlerTmp.show_points.clear();
	for (; stackit != mapEventStacks.end(); stackit++)
	{
		CString strItem;
		vector<EVENT_T>::const_iterator evit = stackit->second.begin();

		if (IsUpdateEventBox(type))
		{
			strItem.Format(TEXT("------------ %d/%d ------------"), stackit->first+1,
						cit->second.size());

			m_listBoxEventsDetail.AddString(strItem);

			if (m_chkEventLabel.GetCheck() == BST_CHECKED)
			{

				for (; evit != stackit->second.end(); evit++)
				{
					strItem.Format(TEXT("[%d] %s %s %s"), evit->dev_no, 
						evit->stype, evit->scode, evit->svalue);

					if (evit->extra == EXTRA_EVENT_ADD)
					{
						strItem.Append(TEXT(" (+)"));
					}
					m_listBoxEventsDetail.AddString(strItem);
				}
			}
			else if (m_chkEventLabel.GetCheck() == BST_INDETERMINATE)	// tri-state
			{
				for (; evit != stackit->second.end(); evit++)
				{
					strItem.Format(TEXT("[%d] %04x %04x %08x"), evit->dev_no,
						evit->type, evit->code, evit->value);

					if (evit->extra == EXTRA_EVENT_ADD)
					{
						strItem.Append(TEXT(" (+)"));
					}
					m_listBoxEventsDetail.AddString(strItem);
				}
			}
		}

		if (m_chkShowPoints.GetCheck() == BST_CHECKED)
		{
			strItem.Format(TEXT("--- Touches ----"));
			m_listBoxEventsDetail.AddString(strItem);
			
			const TPIRQ_T &tpirq = (cit->second).at(stackit->first);

			vector<TPTOUCH_T>::const_iterator pntit = tpirq.touches.begin();
			BOOL bAddIRQ = FALSE;

			for (; pntit != tpirq.touches.end(); pntit++)
			{
				if (pntit->type == TPTYPE_KEY)
				{
					strItem.Format(TEXT("%s %s"), CEventPair::U16ToKey(pntit->key),
						(pntit->tracking_id == 1) ? TEXT("DOWN") : TEXT("UP"));
				}
				else
				{
					if (tpirq.protocol == MT_PROTOCOL_B)
					{
						if (!m_ctrler.filter.IsFilterPnt(pntit->slot)) {
							bAddIRQ = TRUE;
						}

						if (TPIRQ_T::IsSlotActive(pntit->slot, pntit->tracking_id))
						{
							strItem.Format(TEXT("[B]P%d(%d,%d)[p:%d,tm:%d] DOWN"), 
								pntit->slot, pntit->x, pntit->y, pntit->pressure, pntit->touch_major);
						}
						else 
						{
							strItem.Format(TEXT("[B]P%d UP"), pntit->slot);
						}
					}
					else
					{
						if (!m_ctrler.filter.IsFilterPnt(pntit->tracking_id)) {
							bAddIRQ = TRUE;
						}
						strItem.Format(TEXT("[A]P%d(%d,%d)[p:%d,tm:%d] DOWN"), 
							pntit->tracking_id, pntit->x, pntit->y, pntit->pressure,
							pntit->touch_major);
					}
				}
				if (IsUpdateEventBox(type)) 
				{
					m_listBoxEventsDetail.AddString(strItem);
				}
			}
			if (tpirq.dgnflag > ME_ERROR_NULL)
			{
				strItem.Empty();
				m_listBoxEventsDetail.AddString(strItem);
				strItem.Format(TEXT("###### Error Info ######"));
				m_listBoxEventsDetail.AddString(strItem);
				strItem.Format(TEXT("[%d]%s"), stackit->first+1, Analyzer.GetDgnErrorInfo(dev_no, stackit->first, tpirq.dgnflag));
				m_listBoxEventsDetail.AddString(strItem);
				strItem.Format(TEXT("###################"));
				m_listBoxEventsDetail.AddString(strItem);

			}
			if (bAddIRQ) 
			{
				m_ctrlerTmp.show_points[stackit->first] = tpirq.touches;
			}

		}

	}
	m_ctrlerTmp.cit_spnts = m_ctrlerTmp.show_points.cbegin();
	PostMessage(UWM_UPDATE_PLAY_ITEMS, 0, 0);
}



void CInputEventAnalyzerDlg::OnBnClickedBtnBrowseEventLog()
{
	CFileDialog fileLog(TRUE, TEXT("getevent log"),
		NULL, OFN_OVERWRITEPROMPT |OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, 
		TEXT("Event|*.txt|All|*.*||"), this);

	if (fileLog.DoModal() != IDOK) {
		return;
	}

	CString strPath = fileLog.GetPathName();

	m_editEventLogPath.SetWindowTextW(strPath);
	m_editEventLogPath.SetSel(-1);

	_beginthreadex(NULL, 0, ThreadParseEventLog, new CString(strPath), 0, NULL);
	
}


void CInputEventAnalyzerDlg::OnLvnColumnclickListEvents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CInputEventAnalyzerDlg::OnLvnItemchangingListEvents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

}
void CInputEventAnalyzerDlg::CheckListAll(BOOL bCheck)
{
	int count = m_listEvents.GetItemCount();

	for (int i = 0; i < count; i++)
	{
		m_listEvents.SetCheck(i, bCheck);
	}
}


void CInputEventAnalyzerDlg::OnHdnItemclickListEvents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if (phdr->iItem == 0)
	{
		static BOOL bCheckAll = TRUE;
		CheckListAll(bCheckAll);
		bCheckAll = !bCheckAll;

		PostMessage(UWM_UPDATE_EVENT_DETAIL, 0, 0);
	}
}

void CInputEventAnalyzerDlg::UpdateEventDetail(int type)
{
	int count = m_listEvents.GetItemCount();
	
	set<int> chkItems;
	for (int i = 0; i < count; i++)
	{
		if((type == 1) && (m_listEvents.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED))
		{
			chkItems.insert(i);
		}
		else if (m_listEvents.GetCheck(i))
		{
			chkItems.insert(i);
		}
	}

	UpdateDetailList(CurSelToDevNo(), chkItems);
}

void CInputEventAnalyzerDlg::OnNMClickListEvents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;

	if (m_bListUpdating)
	{
		return;
	}
	DWORD dwPos = GetMessagePos();
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

	m_listEvents.ScreenToClient(&point);

	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;

	UINT nFlag;
	int nItem = m_listEvents.HitTest(point, &nFlag);
	//判断是否点在checkbox上
	if(nFlag == LVHT_ONITEMSTATEICON)
	{
		PostMessage(UWM_UPDATE_EVENT_DETAIL, 0, 0);
	} 
	else 
	{
		int itemNo = pNMItemActivate->iItem;
		if (itemNo == -1)
		{
			return;
		}
		UpdateDetailList(CurSelToDevNo(), itemNo);
	}
}


void CInputEventAnalyzerDlg::OnBnClickedCheckEventLabel()
{
	PostMessage(UWM_UPDATE_EVENT_DETAIL, 1, 0);
}


void CInputEventAnalyzerDlg::OnBnClickedCheckShowPoints()
{
	PostMessage(UWM_UPDATE_EVENT_DETAIL, 1, 0);
}

int CInputEventAnalyzerDlg::CurSelToDevNo() 
{
	int sel = m_cbEventDevs.GetCurSel();

	map<int, EVENT_DEV_T>::const_iterator cit = Analyzer.m_mapEventDevices.begin();

	for(int i = 0; cit != Analyzer.m_mapEventDevices.end(); cit++, i++)
	{
		if (i == sel)
		{
			m_ctrlerTmp.evdev = cit->second;
			return cit->first;
		}
	}
	m_ctrlerTmp.evdev.x_res = 0;
	return -1;
}

void CInputEventAnalyzerDlg::OnCbnSelchangeComboEventDevs()
{
	UpdateList(CurSelToDevNo());
}


void CInputEventAnalyzerDlg::OnBnClickedCheckSelectAll()
{
	BOOL bChecked = (m_chkSelectAll.GetCheck() == BST_CHECKED);

	EnterDrawPointsHalt();
	CheckListAll(bChecked);

	PostMessage(UWM_UPDATE_EVENT_DETAIL, 0, 0);
}


void CInputEventAnalyzerDlg::OnBnClickedBtnPrevious()
{
	if (m_listEvents.GetItemCount() > 0)
	{
		EnterDrawPointsHalt();
		int nItem = m_listEvents.GetNextItem(-1, LVNI_SELECTED);
		if ((nItem == -1))
		{
			nItem = 0;
		}
		else if (nItem == 0)
		{	
			m_listEvents.SetItemState(nItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
			nItem = m_listEvents.GetItemCount()-1;
		}
		else
		{
			m_listEvents.SetItemState(nItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
			nItem--;
		}
		m_listEvents.SetItemState(nItem, 
			LVIS_SELECTED | LVIS_FOCUSED | LVIS_ACTIVATING | LVIS_OVERLAYMASK, 
			LVIS_SELECTED | LVIS_FOCUSED | LVIS_ACTIVATING | LVIS_OVERLAYMASK);

		m_listEvents.EnsureVisible(nItem, TRUE);
		m_listEvents.SetFocus();


		UpdateDetailList(CurSelToDevNo(), nItem);
		ExitDrawPointsHalt();
	}
}

int CPointPlayController::GetNextTraceIndex() 
{
	int index = -1;

	if (cit_spnts == show_points.cend())
	{
		cit_spnts = show_points.cbegin();
	}

	if (cit_spnts != show_points.cend())
	{
		index = cit_spnts->first;
		cit_spnts++;
	}
	
	return index;
}

void CInputEventAnalyzerDlg::ResetScreen()
{
	CRect lRect;
	m_staticShowPoint.GetClientRect(&lRect);
	lRect.left++;
	lRect.bottom--;
	lRect.top++;
	lRect.right--;

	m_staticShowPoint.GetDC()->FillSolidRect(lRect.left ,
		lRect.top ,lRect.Width(),lRect.Height(), RGB(240, 240, 240));

	m_editAppLog.ShowWindow(SW_SHOW);
}

void CInputEventAnalyzerDlg::UpdatePlayCtrler()
{
	if (m_bCtrlNeedUpdate)
	{
		m_ctrler.ResetItems(m_ctrlerTmp);
		m_bCtrlNeedUpdate = FALSE;
	}
}

unsigned WINAPI ThreadDrawPoints(LPVOID lP)
{
	CInputEventAnalyzerDlg *pDlg = (CInputEventAnalyzerDlg *)lP;
	CPointPlayController &ctrler = pDlg->m_ctrler;
	ctrler.ResetIndex();
	while (pDlg->IsShowPointTrace())
	{
		Sleep(5);
		if (pDlg->IsShowPointHalted())
		{
			continue;
		}
		pDlg->UpdatePlayCtrler();
		Sleep(ctrler.sleepms);
		CS_LOCK();
		ctrler.index = ctrler.GetNextTraceIndex();
		pDlg->DrawPoint(ctrler.index);
		if (!ctrler.framebyframe && !pDlg->IsShowPointHalted()) {
			pDlg->SelectEventItem(ctrler.index);
		}
		CS_UNLOCK();
	}
	pDlg->ResetScreen();

	return 0;
}

void CInputEventAnalyzerDlg::OnBnClickedBtnPlay()
{
	CString text;
	
	m_btnPlay.GetWindowText(text);

	if (!text.CompareNoCase(TEXT("Play")))
	{
		m_editAppLog.ShowWindow(SW_HIDE);
		m_btnPlay.SetWindowTextW(TEXT("Stop"));
		m_bShowPointsTrace = TRUE;
		m_bHaltShowPoints = FALSE;
		m_btnErrorNext.EnableWindow(FALSE);
		_beginthreadex(NULL, 0, ThreadDrawPoints, this, 0, NULL);
	}
	else if (!text.CompareNoCase(TEXT("Stop")))
	{
		m_btnErrorNext.EnableWindow(TRUE);
		m_btnPlay.SetWindowTextW(TEXT("Play"));
		m_bShowPointsTrace = FALSE;
	}

	SetPauseText(m_bHaltShowPoints);
}

LRESULT CInputEventAnalyzerDlg::OnUpdatePauseText(WPARAM wParam, LPARAM lParam)
{
	BOOL bPause = !!(int)wParam;
	
	if (bPause)
	{
		m_listEvents.SetFocus();
		SetDlgItemText(IDC_BTN_PAUSE, TEXT(">"));
	}
	else
	{
		SetDlgItemText(IDC_BTN_PAUSE, TEXT("||"));
	}
	return 0;
}

void CInputEventAnalyzerDlg::SelectEventItem(int nItem)
{
	if (nItem < 0) {
		TRACE("nitem -1\n");
		return;
	}

	if (m_listEvents.GetItemCount() > 0)
	{
		int lastItem = m_listEvents.GetNextItem(-1, LVNI_SELECTED);
		if (lastItem >= 0) {
			m_listEvents.SetItemState(lastItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}

		m_listEvents.SetItemState(nItem, 
			LVIS_SELECTED | LVIS_FOCUSED | LVIS_ACTIVATING | LVIS_OVERLAYMASK, 
			LVIS_SELECTED | LVIS_FOCUSED | LVIS_ACTIVATING | LVIS_OVERLAYMASK);

		m_listEvents.EnsureVisible(nItem, TRUE);
		m_listEvents.SetFocus();
	}
}

void CInputEventAnalyzerDlg::OnBnClickedBtnNext()
{
	if (m_listEvents.GetItemCount() > 0)
	{
		EnterDrawPointsHalt();
		int nItem = m_listEvents.GetNextItem(-1, LVNI_SELECTED);
		if (nItem == -1)
		{
			nItem = 0;
		}
		else if (nItem >= (m_listEvents.GetItemCount()-1))
		{
			m_listEvents.SetItemState(nItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
			nItem = 0;
		}
		else
		{
			m_listEvents.SetItemState(nItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
			nItem++;
		}
		m_listEvents.SetItemState(nItem, 
			LVIS_SELECTED | LVIS_FOCUSED | LVIS_ACTIVATING | LVIS_OVERLAYMASK, 
			LVIS_SELECTED | LVIS_FOCUSED | LVIS_ACTIVATING | LVIS_OVERLAYMASK);

		m_listEvents.EnsureVisible(nItem, TRUE);
		m_listEvents.SetFocus();

		UpdateDetailList(CurSelToDevNo(), nItem);
		ExitDrawPointsHalt();
	}

}

void CInputEventAnalyzerDlg::OnClose()
{
	if (m_bShowPointsTrace)
	{
		m_bHaltShowPoints = TRUE;
		m_bShowPointsTrace = FALSE;
		m_btnPlay.SendMessage(BN_CLICKED);
		Sleep(m_ctrler.sleepms+100);
	}
	CDialogEx::OnClose();

}


void CInputEventAnalyzerDlg::OnBnClickedCheckPointerLocation()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bShowPointerLocation = m_chkPointerLocation.GetCheck() == BST_CHECKED;
}


void CInputEventAnalyzerDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_bShowPointsTrace)
	{
		RECT rc;
		m_staticShowPoint.GetWindowRect(&rc);
		ScreenToClient(&rc);
		if (PtInRect(&rc, point))
		{
			CMenu   menu;   //定义下面要用到的cmenu对象
			menu.LoadMenu(IDR_MENU_TRACE_PLAY); //装载自定义的右键菜单 
			CMenu   *pContextMenu=menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单 
			if (m_bHaltShowPoints)
			{
				pContextMenu->ModifyMenu(0,MF_BYPOSITION ,ID_PLAYOPS_PAUSE,TEXT("Play"));
			}
			CPoint pointCursor;//定义一个用于确定光标位置的位置  
			GetCursorPos(&pointCursor);//获取当前光标的位置，以便使得菜单可以跟随光标  
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
				pointCursor.x,pointCursor.y,AfxGetMainWnd()); //在指定位置显示弹出菜单
		}
	}
	CDialogEx::OnRButtonDown(nFlags, point);
}


void CInputEventAnalyzerDlg::OnPlayopsPause()
{

	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_PAUSE, BN_CLICKED), NULL); 
}

void CInputEventAnalyzerDlg::SetPauseText(BOOL bPause)
{
	PostMessage(UWM_UPDATE_PAUSE_TEXT, bPause ? 1 : 0, 0);
}

void CInputEventAnalyzerDlg::OnPlayopsPlaycheckeditems()
{
	if (!m_bHaltShowPoints) 
	{
		return;
	}
	ExitDrawPointsHalt();

	SetPauseText(m_bHaltShowPoints);
	PostMessage(UWM_UPDATE_EVENT_DETAIL, 1, 1);
}

void CInputEventAnalyzerDlg::EnterDrawPointsHalt()
{
	m_bHaltShowPoints = TRUE;
}

void CInputEventAnalyzerDlg::ExitDrawPointsHalt()
{
	m_bHaltShowPoints = FALSE;
}

void CInputEventAnalyzerDlg::OnBnClickedBtnPause()
{
	if (!m_bHaltShowPoints)
	{
		m_bHaltShowPoints = TRUE;
	}
	else
	{
		m_bHaltShowPoints = FALSE;
	}
	SetPauseText(m_bHaltShowPoints);
	if (!m_bHaltShowPoints && m_ctrler.framebyframe)
	{
		PostMessage(UWM_UPDATE_EVENT_DETAIL, 1, 1);
	}
}


void CInputEventAnalyzerDlg::OnBnClickedBtnErrors()
{
	int nItem = m_listEvents.GetNextItem(-1, LVIS_FOCUSED);

	nItem = Analyzer.GetNextErrorItem(nItem);

	if (nItem == -1) {
		return;
	}

	m_chkEventLabel.SetCheck(BST_UNCHECKED);
	CheckListAll(FALSE);
	m_listEvents.SetCheck(nItem, TRUE);
	m_listEvents.SetItemState(nItem, 
		LVIS_SELECTED | LVIS_FOCUSED | LVIS_ACTIVATING | LVIS_OVERLAYMASK, 
		LVIS_SELECTED | LVIS_FOCUSED | LVIS_ACTIVATING | LVIS_OVERLAYMASK);

	m_listEvents.EnsureVisible(nItem, TRUE);

	if (nItem >= 1) {
		m_listEvents.SetCheck(nItem-1, TRUE);
		m_listEvents.SetItemState(nItem-1,
			LVIS_SELECTED | LVIS_ACTIVATING | LVIS_OVERLAYMASK, 
			LVIS_SELECTED | LVIS_ACTIVATING | LVIS_OVERLAYMASK);
	}
	m_listEvents.SetFocus();


	PostMessage(UWM_UPDATE_EVENT_DETAIL, 0, 0);
}


void CInputEventAnalyzerDlg::OnBnClickedBtnExtraFilters()
{
	CDialogFilters fdlg(&m_ctrler.filter);

	fdlg.DoModal();
}


void CInputEventAnalyzerDlg::OnPlayopsNextframe()
{
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_NEXT, BN_CLICKED), NULL); 

}


void CInputEventAnalyzerDlg::OnPlayopsPrevframe()
{
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_PREVIOUS, BN_CLICKED), NULL);
}


void CInputEventAnalyzerDlg::OnPlayopsSpeedMinus()
{
	// TODO: 在此添加命令处理程序代码
	m_ctrler.SpeedMinus();
}


void CInputEventAnalyzerDlg::OnPlayopsSpeedPlus()
{
	m_ctrler.SpeedPlus();
}


void CInputEventAnalyzerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UINT count;
	wchar_t filePath[200];

	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if(count)
	{        
		// 只获取第一个文件
		for(UINT i=0; i<1; i++)
		{
			int pathLen = DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));
			if (pathLen > 0) {
				
				m_editEventLogPath.SetWindowTextW(filePath);
				m_editEventLogPath.SetSel(-1);
				_beginthreadex(NULL, 0, ThreadParseEventLog, new CString(filePath), 0, NULL);

			}
		}
	}

	DragFinish(hDropInfo);


	CDialogEx::OnDropFiles(hDropInfo);
}
