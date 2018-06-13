#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "input.h"
#include <stack>
using std::stack;

using std::map;
using std::string;
using std::ifstream;
using std::vector;
using std::set;
using std::pair;

#define MAX_POINT_NUM	15
#define MAX_EVENT_DEV	20

#define EXTRA_EVENT_ADD		(0xADD)
#define EXTRA_EVENT_NO		(0xCCCC)
#define EXTRA_EVENT_POINTS	(0xBBBB)

class DIAGNOSE_ERROR_T
{
public:
	DIAGNOSE_ERROR_T() :dev_no(0), index(0), type(0), err_param_1(0), err_param_2(0){}

	bool operator == (const DIAGNOSE_ERROR_T &err) {
		return this->type == err.type;
	}
	bool operator == (const DIAGNOSE_ERROR_T &err) const {
		return this->type == err.type;
	}
	bool operator < (const DIAGNOSE_ERROR_T &err) {
		return this->type < err.type;
	}
	bool operator < (const DIAGNOSE_ERROR_T &err) const {
		return this->type < err.type;
	}
	bool operator > (const DIAGNOSE_ERROR_T &err) {
		return this->type > err.type;
	}
	bool operator > (const DIAGNOSE_ERROR_T &err) const {
		return this->type > err.type;
	}

public:
	int dev_no;
	int index;
	int type;
	int err_param_1;
	int err_param_2;
};


enum MT_PROTOCOL{
	MT_PROTOCOL_A,	/* mt_sync */
	MT_PROTOCOL_B,	/* slot */
	MT_KEY,		/* for key etc */
	MT_NULL,
};

typedef struct  
{
	__u16 abs;
	__s32 value;
	__s32 min;
	__s32 max;
	__s32 fuzz;
	__s32 flat;
	__s32 resolution;
}EVENT_ABS_T, *P_EVENT_ABS_T;

/* record the input properties */
/*
add device 1: /dev/input/event3
name:     "mtk-tpd-kpd"
events:
KEY (0001): 008b  009e  00ac 
	input props:
<none>
	add device 2: /dev/input/event2
name:     "mtk-tpd"
events:
KEY (0001): 008b  009e  00ac  014a 
	ABS (0003): 0000  : value 0, min 0, max 1080, fuzz 0, flat 0, resolution 1080
	0001  : value 0, min 0, max 1920, fuzz 0, flat 0, resolution 1920
	0018  : value 0, min 0, max 255, fuzz 0, flat 0, resolution 0
	0030  : value 0, min 0, max 100, fuzz 0, flat 0, resolution 0
	0031  : value 0, min 0, max 100, fuzz 0, flat 0, resolution 0
	0035  : value 0, min 0, max 1080, fuzz 0, flat 0, resolution 0
	0036  : value 0, min 0, max 1920, fuzz 0, flat 0, resolution 0
	0039  : value 0, min 0, max 10, fuzz 0, flat 0, resolution 0
	input props:
INPUT_PROP_DIRECT
*/
typedef struct 
{
	int no;
	CString name;
	int x_res;
	int y_res;
	int protocol;
	map<__u16, set<__u16> > events;
	map<__u16, EVENT_ABS_T> abses;
	map<__u16, CString> properties;
	vector<CString> vecAttrs;

	BOOL power_key;
}EVENT_DEV_T, *P_EVENT_DEV_T;

/* input event properties */
class EVENT_T
{
public:
	int dev_no;
	__u16 type;
	__u16 code;
	__s32 value;
	double time;
	int rate;

	CString stype;
	CString scode;
	CString svalue;
	CString stime;
	CString srate;
	int extra;
};



enum TPPOINT_ERRORS {
	TPPERR_UNDEFINED,
	TPPERR_ID_ILLEGAL,	/* tracking_id */
	TPPERR_SLOT_ILLEGAL, /* slot appears to be illegal */
	TPPERR_PRESSURE_ILLEGAL, /* illegal pressure value */
	TPPERR_X_ILLEGAL, /* illegal x value, like x < 0 */
	TPPERR_Y_ILLEGAL, /* illegal y value, like y = 60000 */
};

enum TPTOUCH_TYPE {
	TPTYPE_UNDEF = 0,
	TPTYPE_TOUCH = 1,
	TPTYPE_KEY = 2,
	TPTYPE_PEN,
	TPTYPE_SYN,
};

typedef struct  
{
	int index;
	int type; 
	int code;
	int value;
} EVENT_DIAGNOSE_T, *P_EVENT_DIAGNOSE_T;

typedef struct 
{
	int type;

	//map<int, int> extra_datas;
	int key;
	__u16 slot;
	int tracking_id;	/* 0: key up, 1: key down */ 
	int x;
	int y;
	int pressure;
	int touch_major;
	int touch_minor;

	double begtime;
	double endtime;
	int error;
} TPTOUCH_T, *P_TPTOUCH_T;

typedef struct  
{
	int index;
	TPTOUCH_T tpt;
}TOUCH_DIAGNOSE_T, *P_TOUCH_DIAGNOSE_T;

enum MT_ERROR_TYPE {
	ME_ERROR_NULL	= 0x00,
	ME_VALUE_INVALID_SUSPECTED	= 0x01,
	ME_VALUE_INVALID			= 0x02,
	ME_POINT_NO_DOWN			= 0x04,
	ME_NO_MT_SYNC				= 0x08,
	ME_POINT_NO_UP				= 0x10,
	ME_POINT_X_JUMP				= 0x20,  /* xy坐标跨度太大 */
	ME_POINT_Y_JUMP				= 0x40,
	ME_POINT_XY_JUMP			= ME_POINT_X_JUMP | ME_POINT_Y_JUMP, 
	ME_KEY_NO_UP				= 0x80,
	ME_NO_SYNC					= 0x100,
	ME_NO_BTN_TOUCH				= 0x200,
	ME_EVENT_NOT_SUPPORTED		= 0x400,
	ME_KEY_NO_DOWN				= 0x800,
};





typedef struct
{
	int type;
	int syn_num;
	int mt_syn_num;
	//map<__u16, __s32> datas;
}TPSYN_T, *P_TPSYN_T;

class TPIRQ_T
{
public:
	int dgnflag; /* 是否有出错 */
	int touch_point;
	int point_num;
	int protocol;
	TPSYN_T syns;
	vector<TPTOUCH_T> touches;
	vector<EVENT_T> event_stack;

public:
	static __u16 s_lastActiveSlot;
	static void InitMtSlots();
	static TPTOUCH_T MultiTouch_SLOTS[MAX_POINT_NUM];
	static void SetLastActiveSlot(vector<EVENT_T> &LastEvents);
	static void UpdateMTSlots(vector<TPTOUCH_T> &touches);
	static void InitSlotData(TPTOUCH_T &tpt, __u16 slot);
	static __u16 GetLastActiveSlot();
	static BOOL IsSlotActive(__u16 slot, __s32 tracking_id);
	static void CopySlotData(const TPTOUCH_T &src, TPTOUCH_T &dst);

	//TPIRQ_T() {}
	//TPIRQ_T(const TPIRQ_T &irq)
	//{
	//	this->touch_point = irq.touch_point;
	//	this->point_num = irq.point_num;
	//	this->protocol = irq.protocol;
	//	this->syns = irq.syns;
	//	this->touches.assign(irq.touches.begin(), irq.touches.end());
	//}
};

class CEventPair
{
public:
	static map<CString, __u16> TYPES;
	static map<CString, __u16> ABS_CODES;
	static map<CString, __u16> KEY_CODES;
	static map<CString, __u16> BTN_CODES;
	static map<CString, __u16> SYN_CODES;
	static map<CString, __s32> VALUES;
	static map<CString, __u16> DEV_PROPERTIES;

public:
	static void InitEventPairs();
	static __u16 TypeToU16(CString strType);
	static CString U16ToType(__u16 type);
	static __u16 CodeToU16(CString strCode);
	static CString U16ToCode(__u16 code, __u16 type);
	static __s32 ValueToS32(CString value);
	static CString S32ToValue(__s32 val, __u16 code, __u16 type);
	static CString U16ToKey(__u16 keycode);

	static __u16 PropToU16(CString strProp);
	static CString U16ToProp(__u16 prop);
};


class CEventDiagnoser
{
private:
	int JUMP_MAX_X;
	int JUMP_MAX_Y;

public:
	CEventDiagnoser() : m_bProtocolB(TRUE),m_DeviceNo(0) { SetResolution(1080, 1920);}
	CEventDiagnoser(BOOL bPrtlB, int devNo) : m_bProtocolB(bPrtlB),m_DeviceNo(devNo) {SetResolution(1080, 1920);}

	void SetResolution(int x, int y);
	void Reset(BOOL bPrtlB, int devNo);
	int Diagnose(TPIRQ_T &irq, int index);
	void Flush(); /* flush all data */
	int GetDiagnoseResult(int index);
	int GetNextErrorItem(int offset);
	CString ToErrorString(int dev_no, int index, int errorcode);

public:
	static map<int, CString> SYMBOLIC_DIAG_ERRORS;
	static void InitSymbolicErrors();

private:
	void AddError(int index, DIAGNOSE_ERROR_T &det);
	int PushEvent(EVENT_T est, int index);
	int PushTouch(TPTOUCH_T tpt, int index);
	int PushTouchA(TPTOUCH_T &tpt, int index);
	int PushTouchB(TPTOUCH_T &tpt, int index);

	int GetPointId(TPTOUCH_T &tpt);
	inline BOOL IsTouchUpB(TPTOUCH_T &tpt) { 
		return (tpt.tracking_id == -1) || (tpt.tracking_id == 0xFFFFFFFF); 
	}
	inline BOOL IsKeyDown(TPTOUCH_T &tpt) { return tpt.tracking_id == 1; }
	inline BOOL IsXJump(int x1, int x2) { return abs(x1-x2) > JUMP_MAX_X; }
	inline BOOL IsYJump(int y1, int y2) { return abs(y1-y2) > JUMP_MAX_Y; }

private:
	int m_DeviceNo;
	BOOL m_bProtocolB;
	vector<EVENT_DIAGNOSE_T> m_stkEvents;
	vector<TOUCH_DIAGNOSE_T> m_stkTouches;
	// device_no, index, error_details
	map<int, map<int, set<DIAGNOSE_ERROR_T> > > m_mapDevErrors;
};

class CEventDrawer
{
public:
	CEventDrawer() : m_xResolution(1080), m_yResolution(1920), m_nSpeed(1), 
				m_pParentDC(NULL), m_nCurrentPos(-1){}
	void SetDC(CDC *pDC);
	void InitDraw(int x, int y, vector<TPIRQ_T> &vecIrqs);
	void DrawPoint(int position);
	void DrawNext();
	void DrawPrevious();
	void SetSpeed(int speed) { m_nSpeed = speed; }

private:
	CDC *m_pParentDC;
	int m_xResolution;
	int m_yResolution;
	vector<TPIRQ_T> m_vecPoints;
	int m_nSpeed;
	int m_nCurrentPos;
};

class CEventAnalyzer
{
public:
	CEventAnalyzer(void);
	~CEventAnalyzer(void);


public:
	BOOL OpenGeteventLog(CString strLogPath);
	CString GetAnalysisResult() const;

	CString GetDgnErrorInfo(int dev_no, int index, int errorcode) {
		return m_diagnoser.ToErrorString(dev_no, index, errorcode);
	}

private:
	CString GetEventDevs() const;
	CString GetEvents() const;
	CString GetTouches() const;
	BOOL EventsNeedAddSlot(const vector<EVENT_T>& stack);
	/* getevent [-ltr] */
	BOOL ParseAndroidEvents(CString strLogPath);
	/* getevent -p */
	BOOL ParseEventDevices(CString strEventpPath, map<int, EVENT_DEV_T> &mapDevsEvent);


private:
	void InputpToPropair(CString strLine, map<__u16, CString> &properties);
	void AbsLineToAbsT(CString strLine, EVENT_ABS_T &evat);
	void LineToInts(CString strLine, set<__u16> &vecInts);
	void ParseEventpLines(vector<CString> &strLines,  map<int, EVENT_DEV_T> &mapDevsEvent);
	void ResetContents();

	BOOL AnalyzeEvents();
	BOOL EventsToPoints(int dev_no);

	void PrintEvent(EVENT_T &ev);
	void PrintTouchStack(vector<EVENT_T> &stack);
	void PrintIRQ(TPIRQ_T &irq);

private:
	enum EVENT_ORDER{
		EVENT_TIME,		/* -t para */
		EVENT_NO,
		EVENT_TYPE,
		EVENT_CODE,
		EVENT_VALUE,
		EVENT_RATE,		/* -r para */
	};
	enum EVENT_P_ORDER 
	{
		PEVENT_NULL = 0x00,
		PEVENT_NO = 0x01,
		PEVENT_NAME = 0x02,
		PEVENT_EVENTS = 0x10,
		PEVENT_EV_SW = 0x11,
		PEVENT_EV_KEY = 0x12,
		PEVENT_EV_ABS = 0x14,
		PEVENT_EV_RL = 0x18,
		PEVENT_EV_LED = 0x19,
		PEVENT_INPUT_PROPS = 0x20,
	};
	enum EVENT_ABS_ORDER {
		PEVENT_ABS_HEX,		
		PEVENT_ABS_VALUE,
		PEVENT_ABS_MIN,
		PEVENT_ABS_MAX,
		PEVENT_ABS_FUZZ,
		PEVENT_ABS_FLAT,	
		PEVENT_ABS_RESOLUTION,	
	};

	LONGLONG HexsToLL(CString hex);
	__s32 HexsToS32(CString hex);
	__u16 HexsToU16(CString hex);
	BOOL IsHexStr(CString str);
	BOOL MappingEvent(EVENT_T &ev);
	BOOL AnalyzeEventLine(const CString &line, EVENT_T &ev);
	BOOL AnalyzeTouchEvents(vector<EVENT_T> &vec, TPIRQ_T& tpirq, int dev_no);

	void AddTouch(TPIRQ_T &tpirq, TPTOUCH_T &tpt);
	int GetProtocolType(int dev_no) const;
	BOOL IsWhitespaceLine(CString &str);
	//////////////////////////////////////////////////////////////////////////
	// Multi-Touch Error Detect
	BOOL DiagnoseEvents(vector<TPIRQ_T> &vec, BOOL bProtocolB, int devNo);
	BOOL DetectError();
	//////////////////////////////////////////////////////////////////////////
public:
	int GetNextErrorItem(int offset);
public:
	map<int, vector<TPIRQ_T> > m_mapDevTpirqs;
	map<int, EVENT_DEV_T> m_mapEventDevices;
private:
	CEventDiagnoser m_diagnoser;
	vector<CString> m_vecLogLines;
	vector<CString> m_vecEventDevLines;
	vector<CString> m_vecEventLines;
	vector<EVENT_T> m_vecAllEvents;
	map<int, vector<EVENT_T> > m_mapEvents;

	vector<TPTOUCH_T> m_vecAllTouches;
	map<EVENT_DEV_T, vector<TPTOUCH_T> > m_mapTouches;

};

extern CEventAnalyzer Analyzer;
extern CEventDrawer Drawer;