#include "StdAfx.h"
#include "EventAnalyzer.h"


#define EVENT_PREFIX	"/dev/input/event"
#define EVENT_PREFIXW	_T(EVENT_PREFIX)
const int EVENT_PREFIX_LEN = strlen(EVENT_PREFIX);
CEventAnalyzer Analyzer;
CEventDrawer Drawer;
TPTOUCH_T TPIRQ_T::MultiTouch_SLOTS[MAX_POINT_NUM];

void TPIRQ_T::InitMtSlots()
{
	for (int i = 0; i < MAX_POINT_NUM; i++)
	{
		TPTOUCH_T &tpt = MultiTouch_SLOTS[i];

		tpt.slot = i;
		tpt.tracking_id = i; /* 本应是-1, 但getevent时未上报TRACKING_ID会是UP*/
		tpt.touch_major = 1;
		tpt.touch_minor = 1;
		tpt.x = 1;
		tpt.y = 1;
		tpt.pressure = 1;
	}
}

CEventAnalyzer::CEventAnalyzer(void)
{
	//CEventPair::InitEventPairs();
	TPIRQ_T::InitMtSlots();
}

CEventAnalyzer::~CEventAnalyzer(void)
{
}

void CEventAnalyzer::ResetContents()
{
	m_vecAllEvents.clear();
	m_vecEventLines.clear();
	m_vecLogLines.clear();
	m_vecEventDevLines.clear();

	m_mapEvents.clear();
	m_mapEventDevices.clear();

	m_mapDevTpirqs.clear();
	m_vecAllTouches.clear();
	m_mapTouches.clear();

	TPIRQ_T::InitMtSlots();
}

BOOL CEventAnalyzer::IsWhitespaceLine(CString &str)
{
	WCHAR single;
	int length = str.GetLength();

	for (int i = 0; i < length; i++)
	{
		single = str.GetAt(i);

		if (!iswspace(single))
		{
			return FALSE;
		}
	}
	return TRUE;
}

/* 0010  : value 0, min 0, max -1, fuzz 0, flat 0, resolution 0 */
void CEventAnalyzer::AbsLineToAbsT(CString strLine, EVENT_ABS_T &evat)
{
	int order = PEVENT_ABS_HEX;
	int len = strLine.GetLength();
	WCHAR single;
	CString strValue;

	for (int i = 0; i < len; i++)
	{
		single = strLine.GetAt(i);
		if ((single == TEXT(' ')) || 
			(single == TEXT(':')) ||
			(single == TEXT(',')) ||
			(i == (len-1)))
		{
			if (i == (len-1))
			{
				if ((single != TEXT(' ')) &&
					(single != TEXT(',')) &&
					(single != TEXT(':')))
				{
					strValue.AppendChar(single);
				}
			}

			if ((strValue.Find(TEXT("l")) != -1) ||
				(strValue.Find(TEXT("m")) != -1) ||
				(strValue.Find(TEXT("u")) != -1))
			{
				strValue.Empty();
				continue;
			}

			if (strValue.IsEmpty())
			{
				continue;
			}

			switch (order)
			{
			case PEVENT_ABS_HEX:
				evat.abs = HexsToU16(strValue);
				break;

			case PEVENT_ABS_VALUE:
				evat.value = _wtoi(strValue.GetString());
				break;

			case PEVENT_ABS_MIN:
				evat.min = _wtoi(strValue.GetString());
				break;

			case PEVENT_ABS_MAX:
				evat.max = _wtoi(strValue.GetString());
				break;

			case PEVENT_ABS_RESOLUTION:
				evat.resolution = _wtoi(strValue.GetString());
				break;

			case PEVENT_ABS_FUZZ:
				evat.fuzz = _wtoi(strValue.GetString());
				break;

			case PEVENT_ABS_FLAT:
				evat.flat = _wtoi(strValue.GetString());
				break;
			default:
				break;
			}
			strValue.Empty();
			order++;
		}
		else
		{
			strValue.AppendChar(single);
		}
	}

	
}

/*  [KEY (0001):] 0072  0073  00e2  0104  0105  0106  0107  0246  */
void CEventAnalyzer::LineToInts(CString strLine, set<__u16> &vecInts)
{
	CString strValue;
	int len = strLine.GetLength();
	WCHAR single;

	for (int i = 0; i < len; i++)
	{
		single = strLine.GetAt(i);

		if (single == TEXT(' ') ||
			single == TEXT('\t') ||
			(i == (len-1)))
		{
			if (!strValue.IsEmpty())
			{
				__u16 value = HexsToU16(strValue);
				vecInts.insert(value);
				strValue.Empty();
			}
		}
		else
		{
			strValue += single;
		}
	}
}

/* 0004 INPUT_PROP_DIRECT */
void CEventAnalyzer::InputpToPropair(CString strLine, map<__u16, CString> &properties)
{
	CString strValue;
	int len = strLine.GetLength();
	WCHAR single;

	for (int i = 0; i < len; i++)
	{
		single = strLine.GetAt(i);

		if (single == TEXT(' ') ||
			single == TEXT('\t') ||
			(i == (len-1)))
		{
			if (i == (len-1))
			{
				if ((single != TEXT(' ')) &&
					(single != TEXT('\t')) &&
					(single != TEXT('\r')))
				{
					strValue.AppendChar(single);
				}
			}
			if (!strValue.IsEmpty())
			{
				if (strValue.Find(TEXT("none")) != -1)
				{
					return;
				}

				BOOL bRet = IsHexStr(strValue);
				if (bRet)
				{
					__u16 value = HexsToU16(strValue);
					properties[value] = CEventPair::U16ToProp(value);
				}
				else
				{
					__u16 value = CEventPair::PropToU16(strValue);
					properties[value] = strValue;
				}

				strValue.Empty();
			}
		}
		else
		{
			strValue += single;
		}
	}
}

void CEventAnalyzer::ParseEventpLines(vector<CString> &vecLines, map<int, EVENT_DEV_T> &mapDevsEvent)
{
	
	int nPos;
	CString strSingle;
	BOOL bPowerKey = FALSE;

	vector<CString>::const_iterator allcit = vecLines.begin();
	
	vector<CString> vecSingleDevice;
	for (; allcit != vecLines.end();)
	{
		if ( (allcit->Find(TEXT("add device")) != -1) && (vecSingleDevice.size() > 1))
		{
			//
		}
		else
		{
			vecSingleDevice.push_back(*allcit);
			allcit++;
			if (allcit != vecLines.end())
			{
				continue;
			}
		}
		
		EVENT_DEV_T edt;
		edt.protocol = MT_PROTOCOL_B;
		edt.x_res = 0;
		edt.y_res = 0;
		edt.power_key = FALSE;
		edt.vecAttrs.assign(vecSingleDevice.begin(), vecSingleDevice.end());

		vector<CString>::const_iterator cit = vecSingleDevice.begin();
		int order = PEVENT_NULL;
		for ( ; cit != vecSingleDevice.end(); cit++)
		{
			if (cit->Find(TEXT("could not")) != -1)
			{
				continue;
			}

			if (order < PEVENT_EVENTS)
			{
				if (cit->Find(TEXT("add device")) != -1)
				{
					nPos = cit->Find(TEXT("/dev/input/event"));
					TRACE(cit->Mid(nPos+16));
					TRACE(TEXT("\n"));
					edt.no = _wtoi(cit->Mid(nPos+16));
					order = PEVENT_NO;
				}
				else if (cit->Find(TEXT("name:")) != -1)
				{
					int nRightQuote;
					nPos = cit->Find(TEXT('\"'));
					nRightQuote = cit->Find(TEXT('\"'), nPos+1);
					edt.name = cit->Mid(nPos+1, nRightQuote-nPos-1);
					order = PEVENT_NAME;
				}
				else if (cit->Find(TEXT("events:")) != -1)
				{
					order = PEVENT_EVENTS;

				}
			}
			else
			{
				CString strValues = *cit;
				if (cit->Find(TEXT("KEY (0001):")) != -1)
				{
					nPos = cit->Find(TEXT(":"));
					strValues = cit->Mid(nPos+1);
					order = PEVENT_EV_KEY;
				}
				else if (cit->Find(TEXT("ABS (0003):")) != -1)
				{
					order = PEVENT_EV_ABS;
					nPos = cit->Find(TEXT(":"));
					strValues = cit->Mid(nPos+1);
				}
				else if (cit->Find(TEXT("SW  (0005):")) != -1)
				{
					order = PEVENT_EV_SW;
					nPos = cit->Find(TEXT(":"));
					strValues = cit->Mid(nPos+1);
				}
				else if (cit->Find(TEXT("LED (0011):")) != -1)
				{
					order = PEVENT_EV_LED;
					nPos = cit->Find(TEXT(":"));
					strValues = cit->Mid(nPos+1);
				}
				else if (cit->Find(TEXT("REL (0002):")) != -1)
				{
					order = PEVENT_EV_RL;
					nPos = cit->Find(TEXT(":"));
					strValues = cit->Mid(nPos+1);
				}
				else if (cit->Find(TEXT("input props:")) != -1)
				{
					order = PEVENT_INPUT_PROPS;
					continue;
				}

				set<__u16> &vecEvents = edt.events[EV_KEY];
				switch (order)
				{
				case PEVENT_EV_SW:
					vecEvents = edt.events[EV_SW];
					LineToInts(strValues, vecEvents);
					break;

				case PEVENT_EV_KEY:
					vecEvents = edt.events[EV_KEY];
					LineToInts(strValues, vecEvents);

					if (cit->Find(TEXT("0074")) != -1) 
					{
						if (!bPowerKey)
						{
							bPowerKey = TRUE;
							edt.power_key = TRUE;
						}
					}
					break;

				case PEVENT_EV_LED:
					vecEvents = edt.events[EV_LED];
					LineToInts(strValues, vecEvents);
					break;

				case PEVENT_EV_RL:
					vecEvents = edt.events[EV_REL];
					LineToInts(strValues, vecEvents);
					break;

				case PEVENT_EV_ABS:
					{
						vecEvents = edt.events[EV_ABS];
						EVENT_ABS_T evat;
						AbsLineToAbsT(strValues, evat);
						edt.abses[evat.abs] = evat;
						vecEvents.insert(evat.abs);

						if (evat.abs == ABS_MT_SLOT)
						{
							edt.protocol = MT_PROTOCOL_B;
						}
						else if (evat.abs == ABS_MT_POSITION_X)
						{
							edt.x_res = evat.max;
						}
						else if (evat.abs == ABS_MT_POSITION_Y)
						{
							edt.y_res = evat.max;
						}
					}
					break;
				case PEVENT_INPUT_PROPS:
					{
						InputpToPropair(strValues, edt.properties);
					}
				default:
					break;
				}
			}
		}

		mapDevsEvent[edt.no] = (edt);
		vecSingleDevice.clear();
	}
}

CString CEventAnalyzer::GetAnalysisResult() const
{
	CString strResult;

	strResult += GetEventDevs();
	strResult += TEXT("\r\n");
	
	strResult += GetEvents();
	strResult += TEXT("\r\n");

	return strResult;
}

BOOL CEventAnalyzer::OpenGeteventLog(CString strLogPath)
{
	ResetContents();
	ParseEventDevices(strLogPath, m_mapEventDevices);
	
	ParseAndroidEvents(strLogPath);
	DetectError();

	return TRUE;
}

BOOL CEventAnalyzer::ParseEventDevices(CString strEventpPath,  map<int, EVENT_DEV_T> &mapEventDevices)
{
	ifstream ifs;
	ifs.open(strEventpPath);

	if (!ifs.is_open())
	{
		return FALSE;
	}

	char buf[200] = {0};
	mapEventDevices.clear();
	vector<CString> vecLines;
	while (!ifs.eof())
	{
		memset(buf, 0, 200);
		ifs.getline(buf, 200);


		CString line;
		line = buf;
		
		if (IsWhitespaceLine(line))
		{
			continue;
		}
		if (line.Find(TEXT("/dev/input/event")) != -1)
		{
			if ((line.Find(TEXT("EV_ABS")) != -1) || (line.Find(TEXT("0003")) != -1))
			{
				break;
			}
			else if ((line.Find(TEXT("0000")) != -1) &&
				(line.Find(TEXT("00000000")) != -1))
			{
				break;
			}
			else if ((line.Find(TEXT("EV_SYN")) != -1) &&
				(line.Find(TEXT("SYN_REPORT")) != -1))
			{
				break;
			}
		}
		
		vecLines.push_back(line);
	}
	ifs.close();

	ParseEventpLines(vecLines, mapEventDevices);

	return TRUE;
}

BOOL CEventAnalyzer::ParseAndroidEvents(CString strLogPath)
{
	ifstream ifs;
	ifs.open(strLogPath);
	
	if (!ifs.is_open())
	{
		return FALSE;
	}

	char buf[200] = {0};
	BOOL bStartEvent = FALSE;
	while (!ifs.eof())
	{
		memset(buf, 0, 200);
		ifs.getline(buf, 200);

		CString line;
		line = buf;

		if (IsWhitespaceLine(line))
		{
			continue;
		}

		if (!bStartEvent && 
			(!strncmp(buf, EVENT_PREFIX, EVENT_PREFIX_LEN) ||
			 (line.Find(TEXT("] ")EVENT_PREFIXW) != -1) ||
			 (line.Find(TEXT("EV_")) != -1) ||
			 (line.Find(TEXT("000")) != -1)
			))
		{
			bStartEvent = TRUE;
		}

		m_vecLogLines.push_back(line);
		if (bStartEvent) {
			m_vecEventLines.push_back(line);
		}
		else {
			m_vecEventDevLines.push_back(line);
		}
	}
	ifs.close();

	AnalyzeEvents();

	return TRUE;
}
BOOL CEventAnalyzer::IsHexStr(CString str)
{
	WCHAR single;
	int length = str.GetLength();

	str.MakeLower();
	if (length == 0) {
		return FALSE;
	}

	for (int i = 0; i < length; i++)
	{
		single = str.GetAt(i);
		if ((single >= TEXT('0')) && (single <= TEXT('9')))
		{
			//
		}
		else if ( (single >= TEXT('a')) && (single <= TEXT('f')))
		{
			//
		}
		else 
		{
			return FALSE;
		}
		
	}
	return TRUE;
}

LONGLONG CEventAnalyzer::HexsToLL(CString hex)
{
	WCHAR single;
	int length = hex.GetLength();

	hex.MakeLower();
	if (length == 0) {
		return 0xFFFFFFFF;
	}

	LONGLONG retval = 0;
	__u16 nibble = 0;
	int base = 1;
	for (int i = (length-1); i >= 0; i--)
	{
		single = hex.GetAt(i);

		if ((single >= TEXT('0')) && (single <= TEXT('9')))
		{
			nibble = (single - TEXT('0'));
		}
		else if ((single >= TEXT('a')) && (single <= TEXT('f')))
		{
			nibble = single - TEXT('a') + 10;
		}
		else {
			return 0xFFFFFFFF;
		}
		retval += nibble * base;

		base *= 16;
	}

	return retval;
}

__u16 CEventAnalyzer::HexsToU16(CString hex)
{
	return (__u16)(HexsToLL(hex) & 0xFFFF);
}

__s32 CEventAnalyzer::HexsToS32(CString hex)
{
	return (__s32)(HexsToLL(hex) & 0xFFFFFFFF);
}


BOOL CEventAnalyzer::MappingEvent(EVENT_T &ev)
{
	BOOL bHex = IsHexStr(ev.stype);
	if (bHex) {
		ev.type = HexsToU16(ev.stype);
		ev.stype = CEventPair::U16ToType(ev.type);
	}
	else {
		ev.type = CEventPair::TypeToU16(ev.stype);
	}

	bHex = IsHexStr(ev.scode);
	if (bHex) {
		ev.code = HexsToU16(ev.scode);
		ev.scode = CEventPair::U16ToCode(ev.code, ev.type);
	}
	else {
		ev.code = CEventPair::CodeToU16(ev.scode);
	}

	bHex = IsHexStr(ev.svalue);

	if (bHex) {
		ev.value = HexsToS32(ev.svalue);
		ev.svalue = CEventPair::S32ToValue(ev.value, ev.code, ev.type);
	}
	else {
		ev.value = CEventPair::ValueToS32(ev.svalue);
	}

	
	return TRUE;
}

void CEventAnalyzer::PrintEvent(EVENT_T &ev)
{
	TRACE(TEXT("event%d: %s\t%s\t%s(%d)\n"), ev.dev_no,
		ev.stype, ev.scode, ev.svalue, ev.value);
}

BOOL CEventAnalyzer::AnalyzeEventLine(const CString &line, EVENT_T &ev)
{
	CString prefix = line.Mid(0, EVENT_PREFIX_LEN);
	int begPos = EVENT_PREFIX_LEN;

	if (prefix.CompareNoCase(EVENT_PREFIXW) && 
		(line.Find(TEXT("] ")EVENT_PREFIXW) == -1) &&
		(line.Find(TEXT("EV_")) == -1) &&
		(line.Find(TEXT("00")) == -1))
	{
		return FALSE;
	}
	
	CString value;
	int pos = EVENT_NO;

	if (line.Find(TEXT("] ")EVENT_PREFIXW) != -1) 
	{
		begPos = 0;
		pos = EVENT_TIME;
	}
	else if (prefix.CompareNoCase(EVENT_PREFIXW))
	{
		if (line.Find(TEXT("EV_")) != -1) 
		{
			begPos = line.Find(TEXT("EV_"));
		}
		else 
		{
			begPos = 0;
		}
		pos = EVENT_TYPE;
	}

	ev.time = 0.0F;
	WCHAR sgl;
	int length = line.GetLength();
	for (int i = begPos; i < length; i++)
	{
		sgl = line.GetAt(i);

		if (sgl == TEXT(' ') || sgl == TEXT(':') ||
			sgl == TEXT('\t') ||
			(i == (length-1)))
		{
			if (i == (length-1))
			{
				if ((sgl >= TEXT('0')) && (sgl <= TEXT('9')))
				{
					value.AppendChar(sgl);
				}
				else if ((sgl >= TEXT('a')) && (sgl <= TEXT('z')))
				{
					value.AppendChar(sgl);
				}
				else if ((sgl >= TEXT('A')) && (sgl <= TEXT('Z')))
				{
					value.AppendChar(sgl);
				}
			}

			if (!value.IsEmpty()) 
			{
				switch (pos) 
				{
				case EVENT_TIME:
					ev.stime = value;
					//ev.time = _wtof(value);
					break; 

				case EVENT_NO:
					ev.dev_no = _wtoi(value.GetString());
					break;

				case EVENT_TYPE:
					ev.stype = value;
					break;

				case EVENT_CODE:
					ev.scode = value;
					break;
				case EVENT_VALUE:
					ev.svalue = value;
					break;
				case EVENT_RATE:
					ev.srate = value;
					break;
				default:
					break;
				}
				pos++;
				value.Empty();
			}
		}
		else {
			if (sgl != TEXT('[') &&
				sgl != TEXT(']'))
			{
				value.AppendChar(sgl);
			}
		}
	}
	TRACE(TEXT("> %s \n"), line);
	BOOL bRet = MappingEvent(ev);

	PrintEvent(ev);
	return bRet;
}


BOOL CEventAnalyzer::AnalyzeEvents()
{
	vector<CString>::const_iterator evcit = m_vecEventLines.begin();
	
	int maxX = 0;
	int maxY = 0;
	for (; evcit != m_vecEventLines.end(); evcit++)
	{
		EVENT_T evt;
		evt.dev_no = 55;		/* default for getevent -l /dev/input/event? */

		if (AnalyzeEventLine(*evcit, evt))
		{
			m_vecAllEvents.push_back(evt);
			m_mapEvents[evt.dev_no].push_back(evt);

			if (m_mapEventDevices.find(evt.dev_no) == m_mapEventDevices.end()) 
			{
				EVENT_DEV_T evdev;
				evdev.no = evt.dev_no;
				evdev.protocol = MT_PROTOCOL_B;
				evdev.x_res = 1080;
				evdev.y_res = 1920;
				m_mapEventDevices[evt.dev_no] = evdev;
			}
			else 
			{
				EVENT_DEV_T& evdev = m_mapEventDevices[evt.dev_no];

				switch (evt.type)
				{
				case EV_ABS:
				case EV_KEY:
				case EV_SYN:
					evdev.events[evt.type].insert(evt.code);
					break;
				default:
					break;
				}



				if ((evt.type == EV_ABS) || (evt.type == EV_SYN))
				{

					if (evt.code == ABS_MT_POSITION_Y)
					{
						if (evt.value > maxY)
						{
							maxY = evt.value;
						}
					}
					else if (evt.code == ABS_MT_POSITION_X)
					{
						if (evt.value > maxX)
						{
							maxX = evt.value;
						}
					}
					if ( (evt.type == EV_SYN) &&
						 (evt.code == SYN_MT_REPORT))
					{
						evdev.protocol = MT_PROTOCOL_A;
					}
					else if (evt.code == ABS_MT_SLOT)
					{
						evdev.protocol = MT_PROTOCOL_B;
					}
					evdev.abses[evt.code];
				}
			}
		}
	}


	map<int, EVENT_DEV_T>::iterator cit = m_mapEventDevices.begin();

	set<int> setTpDevs;
	for (; cit != m_mapEventDevices.end(); cit++)
	{
		EVENT_DEV_T &dev = cit->second;
		if ( (dev.events.find(EV_ABS) == dev.events.end()) )
		{ 
			if (dev.events.find(EV_KEY) != dev.events.end())
			{
				dev.protocol = MT_KEY;
			}
			else
			{
				dev.protocol = MT_NULL;
			}
		}
		else 
		{
			int res_x = 720;
			int res_y = 1080;

			if (maxY > 1920)
			{
				res_x = 1440; // 2k
				res_y = 2560;
			}
			else if (maxY > 1280)
			{
				res_x = 1080; // 1080p
				res_y = 1920;
			}
			else
			{
				res_x = 720; // 1080
				res_y = 1280;
			}

			if (res_x < maxX)
			{
				res_x = maxX - (maxX%10) + 20;
			}
			if (res_y < maxY)
			{
				res_y = maxY - (maxY%10) + 20;
			}

			
			dev.x_res = res_x;
			dev.y_res = res_y;
		}

		if ((dev.no == 55) && (dev.name.IsEmpty()))
		{
			dev.name = TEXT("target_event");
		}

		if (dev.protocol != MT_NULL) 
		{
			setTpDevs.insert(dev.no);
		}
	}


	m_mapDevTpirqs.clear();
	set<int>::const_iterator devit = setTpDevs.begin();
	for (; devit != setTpDevs.end(); devit++)
	{
		EventsToPoints(*devit);
	}
	return TRUE;
}

int CEventAnalyzer::GetProtocolType(int dev_no) const
{
	map<int, EVENT_DEV_T>::const_iterator cit = m_mapEventDevices.find(dev_no);

	if (cit != m_mapEventDevices.end()) 
	{
		return (cit->second.protocol);
	}
	else 
	{
		return MT_NULL;
	}
}

void CEventAnalyzer::PrintTouchStack(vector<EVENT_T> &stack)
{
	vector<EVENT_T>::const_iterator cit = stack.begin();
	

	TRACE("---------------------\n");
	for (; cit != stack.end(); cit++)
	{
		TRACE(TEXT("event%d: %s\t%s\t%s\n"), cit->dev_no, cit->stype, cit->scode,
			cit->svalue);
	}
}

void CEventAnalyzer::PrintIRQ(TPIRQ_T &irq)
{
	TRACE("\ntouch_point: %d, touch_num: %d\n", irq.touch_point, irq.point_num);
	TRACE("protocol: %s\n", (irq.protocol == MT_PROTOCOL_B) ? "B" : "A");

	vector<TPTOUCH_T>::const_iterator cit = irq.touches.begin();

	for (; cit != irq.touches.end(); cit++)
	{
		if (irq.protocol == MT_PROTOCOL_B)
		{
			if ((cit->tracking_id == -1) ||
				(cit->tracking_id == 0xFFFFFFFF))
			{
				TRACE(TEXT(" [B]P%d UP!\r\n"),
					cit->slot);
			}
			else 
			{
				TRACE(TEXT(" [B]P%d(%d, %d)[p:%d,tm:%d] Down!\r\n"),
					cit->slot, cit->x, cit->y, cit->pressure, cit->touch_major);
			}
		}
		else 
		{
			TRACE(TEXT(" [A]P%d(%d, %d)[p:%d,tm:%d] Down!\r\n"),
				cit->tracking_id, cit->x, cit->y, cit->pressure, cit->touch_major);
		}
	}
	if (irq.protocol == MT_PROTOCOL_B)
	{
		TRACE(TEXT(" syn: %d\r\n"), irq.syns.syn_num);
	}
	else 
	{
		TRACE(TEXT(" mt_syn: %d, syn: %d\r\n"),
			irq.syns.mt_syn_num, irq.syns.syn_num);
	}
	TRACE("---------------------\n");
}

void CEventAnalyzer::AddTouch(TPIRQ_T &tpirq, TPTOUCH_T &tpt)
{
	if (tpt.type == TPTYPE_TOUCH)
	{
		if (tpirq.protocol == MT_PROTOCOL_B)
		{
			BOOL bNewTouch = TRUE;
			vector<TPTOUCH_T>::iterator it = tpirq.touches.begin();
			for (; it != tpirq.touches.end(); it++)
			{
				if (it->slot == tpt.slot)
				{
					TPIRQ_T::CopySlotData(tpt, *it);
					bNewTouch = FALSE;
					break;
				}
			}
			if (bNewTouch)
			{
				tpirq.touches.push_back(tpt);
			}
		}
		else
		{
			tpirq.touches.push_back(tpt);
		}

		tpirq.touch_point++;
		if ((tpt.tracking_id != -1) && 
			(tpt.tracking_id != 0xFFFFFFFF))
		{
			tpirq.point_num++;
		}
	}
	else if (tpt.type == TPTYPE_KEY)
	{
		tpirq.touches.push_back(tpt);
		if (tpt.key != BTN_TOUCH)
		{
			tpirq.touch_point++;
			tpirq.point_num++;
		}
	}

	//memset(&tpt, 0, sizeof(tpt));
	tpt.type = TPTYPE_UNDEF;
}


BOOL CEventAnalyzer::AnalyzeTouchEvents(vector<EVENT_T> &vec, TPIRQ_T& tpirq, int dev_no)
{
	vector<EVENT_T>::const_iterator cit = vec.begin();

	TPTOUCH_T tpt;
	TPSYN_T tpsyn;

	memset(&tpt, 0, sizeof(tpt));
	memset(&tpsyn, 0, sizeof(tpsyn));

	tpsyn.type = TPTYPE_UNDEF;
	tpt.type = TPTYPE_UNDEF;

	for (; cit != vec.end(); cit++)
	{
		if (cit->dev_no != dev_no)
		{
			continue;
		}
		if (cit->type == EV_ABS) 
		{
			if (cit->code == ABS_MT_SLOT)		// SLOT as separator of points for MT-PROTOCOL B
			{
				if ((tpt.type != TPTYPE_UNDEF) && cit != vec.begin())
				{
					AddTouch(tpirq, tpt);
				}
			}

			tpt.type = TPTYPE_TOUCH;
			switch (cit->code) 
			{
			case ABS_MT_SLOT:
				tpt.slot = cit->value;
				TPIRQ_T::InitSlotData(tpt, tpt.slot);
				break;

			case ABS_MT_TOUCH_MINOR:
				tpt.touch_minor = cit->value;
				break;

			case ABS_MT_TOUCH_MAJOR:
				tpt.touch_major = cit->value;
				break;

			case ABS_MT_TRACKING_ID:
				tpt.tracking_id = cit->value;
				break;

			case ABS_MT_PRESSURE:
			case ABS_PRESSURE:
				tpt.pressure = cit->value;
				break;

			case ABS_X:
			case ABS_MT_POSITION_X:
				tpt.x = cit->value;
				break;

			case ABS_Y:
			case ABS_MT_POSITION_Y:
				tpt.y = cit->value;
				break;

			default:	
				//tpt.extra_datas[cit->code] = cit->value;
				break;
			}
		}
		else
		{

			if (cit->type == EV_KEY)
			{	
				TPTOUCH_T tptkey;
				tptkey.type = TPTYPE_KEY;
				tptkey.key = cit->code;
				tptkey.tracking_id = cit->value;

				AddTouch(tpirq, tptkey);
			}
			else {
				if ((tpt.type != TPTYPE_UNDEF) && 
					(cit != vec.begin()))
				{
					AddTouch(tpirq, tpt);
				}
				if (cit->type == EV_SYN)
				{
					tpsyn.type = TPTYPE_SYN;
					switch (cit->code) 
					{
					case SYN_MT_REPORT:			// MT_REPORT as separator of points for MT-PROTOCOL A
						tpsyn.mt_syn_num++;
						break;
					case SYN_REPORT:
						tpsyn.syn_num++;
						break;
					default:
						//tpsyn.datas[cit->code] = cit->value;
						break;
					}
				}
			}
		}
	}

	if (tpt.type != TPTYPE_UNDEF)
	{
		AddTouch(tpirq, tpt);
	}

	if (tpsyn.type == TPTYPE_SYN)
	{
		tpirq.syns = tpsyn;
	}

	//PrintTouchStack(vec);
	//PrintIRQ(tpirq);

	return TRUE;
}

void TPIRQ_T::CopySlotData(const TPTOUCH_T &src, TPTOUCH_T &dst)
{
	dst.tracking_id = src.tracking_id;
	dst.key = src.key;
	dst.x = src.x;
	dst.y = src.y;
	dst.pressure = src.pressure;
	dst.touch_major = src.touch_major;
	dst.touch_minor = src.touch_minor;
}

BOOL TPIRQ_T::IsSlotActive(__u16 slot, __s32 tracking_id)
{
	if ((tracking_id == -1) || (tracking_id == 0xFFFFFFFF))
	{
		return FALSE;
	}
	return TRUE;
}

__u16 TPIRQ_T::s_lastActiveSlot = 0;

void TPIRQ_T::SetLastActiveSlot(vector<EVENT_T> &lastEvents)
{
	vector<EVENT_T>::const_reverse_iterator crit = lastEvents.crbegin();

	for (; crit != lastEvents.crend(); crit++)
	{
		if (crit->code == ABS_MT_SLOT) {
			//if (IsSlotActive(crit->code, crit->value))
			{
				s_lastActiveSlot = crit->value;
				break;
			}
		}
	}
}

void TPIRQ_T::UpdateMTSlots(vector<TPTOUCH_T> &touches)
{
	vector<TPTOUCH_T>::const_iterator cit = touches.begin();

	if (touches.empty()) {
		return;
	}

	for (int i = 1; cit != touches.end(); cit++, i++)
	{
		if (cit->slot < MAX_POINT_NUM)
		{
			if (IsSlotActive(cit->slot, cit->tracking_id))
			{
				TPTOUCH_T &tpt = MultiTouch_SLOTS[cit->slot];
				CopySlotData(*cit, tpt);
			}
		}
	}

}


void TPIRQ_T::InitSlotData(TPTOUCH_T &tpt, __u16 slot)
{
	if (slot < MAX_POINT_NUM)
	{
		TPTOUCH_T &last = MultiTouch_SLOTS[slot];

		CopySlotData(last, tpt);
	}
}


__u16 TPIRQ_T::GetLastActiveSlot()
{
	return s_lastActiveSlot;
}

BOOL CEventAnalyzer::EventsNeedAddSlot(const vector<EVENT_T>& vec)
{
	BOOL hasSlot = FALSE;

	int size = vec.size();
	for (int i = 0; i < size; i++)
	{
		if (vec[i].type == EV_ABS)
		{
			if (vec[i].code == ABS_MT_SLOT) 
			{
				hasSlot = TRUE;
			}
			else {
				return !hasSlot;
			}
		}
	}
	return FALSE;
}

BOOL CEventAnalyzer::EventsToPoints(int dev_no)
{
	map<int, int> mapProtocols;
	
	map<int, EVENT_DEV_T>::const_iterator cit = m_mapEventDevices.begin();
	for (; cit != m_mapEventDevices.end(); cit++)
	{
		mapProtocols[cit->first] = cit->second.protocol;
	}

	vector<EVENT_T> &vecAllEvents = m_mapEvents[dev_no];
	vector<EVENT_T> vecTouchStack;

	int i = 0;
	int len = vecAllEvents.size();
	BOOL bJump = TRUE;
	for (i = 0; i < len; i++)
	{
		const EVENT_T &cur = vecAllEvents.at(i);

		if (bJump)
		{
			if (cur.type == EV_SYN)	// filter the EV_SYNs at the beginning
			{
				continue;
			}
			bJump = FALSE;
		}
		vecTouchStack.push_back(cur);
		if  ((cur.type != EV_SYN) || (cur.code != SYN_REPORT))
		{
			continue;
		}

		if (vecTouchStack.size() < 2)
		{
			vecTouchStack.clear();
			continue;
		}

		TPIRQ_T tpit;
		tpit.point_num = 0;
		tpit.touch_point = 0;
		tpit.protocol = mapProtocols[dev_no];
		if (tpit.protocol == MT_PROTOCOL_B)
		{
			//if (vecTouchStack.at(0).code != ABS_MT_SLOT)
			if (EventsNeedAddSlot(vecTouchStack))
			{
				EVENT_T ev;
				ev.dev_no = dev_no;
				ev.type = EV_ABS;
				ev.code = ABS_MT_SLOT;
				ev.scode = TEXT("ABS_MT_SLOT");
				ev.stype = TEXT("EV_ABS");
				ev.value = TPIRQ_T::GetLastActiveSlot();
				ev.svalue.Format(TEXT("%08x"), ev.value);
				ev.extra = EXTRA_EVENT_ADD;
				vecTouchStack.insert(vecTouchStack.begin(), ev);
			}
			map<int, vector<TPIRQ_T> >::const_iterator cit = m_mapDevTpirqs.find(dev_no);
			if ((cit != m_mapDevTpirqs.end()) && (cit->second.size() > 0))
			{
				const TPIRQ_T &lastIrq = *((cit->second).crbegin());
				vector<TPTOUCH_T>::const_iterator touchit = lastIrq.touches.begin();

				for (; touchit != lastIrq.touches.end(); touchit++)
				{
					if (touchit->type == TPTYPE_TOUCH)
					{
						if (TPIRQ_T::IsSlotActive(touchit->slot, touchit->tracking_id))
						{
							tpit.touches.push_back(*touchit);
						}
					}
				}
			}
		}

		AnalyzeTouchEvents(vecTouchStack, tpit, dev_no);
		tpit.event_stack.assign(vecTouchStack.begin(), vecTouchStack.end());
		m_mapDevTpirqs[dev_no].push_back(tpit);
		if (tpit.protocol == MT_PROTOCOL_B)
		{
			TPIRQ_T::SetLastActiveSlot(vecTouchStack);
			TPIRQ_T::UpdateMTSlots(tpit.touches);
		}

		vecTouchStack.clear();
	}

	return TRUE;
}


CString CEventAnalyzer::GetEventDevs() const
{
	CString strDevsInfo;

	map<int, EVENT_DEV_T>::const_iterator cit = m_mapEventDevices.begin();

	strDevsInfo.AppendFormat(TEXT("Total %d input event devices: \r\n"), m_mapEventDevices.size());
	for (; cit != m_mapEventDevices.end(); cit++)
	{
		strDevsInfo.AppendFormat(TEXT("Event%d: \r\n name: \"%s\""),
			cit->first, cit->second.name);

		if (cit->second.abses.size() > 0)
		{
			strDevsInfo.AppendFormat(TEXT("\r\n Protocol: %s"),
				(cit->second.protocol == MT_PROTOCOL_B) ? TEXT("B") : TEXT("A"));
			strDevsInfo.Append(TEXT("\r\n ABS: "));
			map<__u16, EVENT_ABS_T>::const_iterator it = cit->second.abses.begin();
			for (; it != cit->second.abses.end(); it++)
			{
				strDevsInfo.AppendFormat(TEXT("\r\n\t%s(%04x) "), 
					CEventPair::U16ToCode(it->first, EV_ABS), it->first);
			}
		}
		strDevsInfo.Append(TEXT("\r\n"));
	}

	return strDevsInfo;
}

CString CEventAnalyzer::GetEvents() const
{
	CString strEventInfo;

	strEventInfo.Append(TEXT("\r\nEvents: \r\n"));
	map<int, vector<EVENT_T> >::const_iterator cit = m_mapEvents.begin();

	map<__s32, int> mapEventStatistic;

	for (; cit != m_mapEvents.end(); cit++)
	{
		strEventInfo.AppendFormat(TEXT("> Event%d: %d events\r\n"), 
					cit->first, cit->second.size());

		__s32 typecode;
		mapEventStatistic.clear();
		vector<EVENT_T>::const_iterator it = cit->second.begin();
		for (; it != cit->second.end(); it++)
		{
			typecode = (it->type << 16) & 0xFFFF0000;
			typecode += it->code;

			mapEventStatistic[typecode]++;
		}

		map<__s32, int>::const_iterator sit = mapEventStatistic.begin();
		for (; sit != mapEventStatistic.end(); sit++)
		{
			__u16 type, code;
			CString stype, scode;

			type = (sit->first >> 16) & 0xFFFF;
			stype = CEventPair::U16ToType(type);
			code = (__u16)((sit->first) & 0xFFFF);
			scode = CEventPair::U16ToCode(code, type);

			strEventInfo.AppendFormat(TEXT("  %s %s: %d\r\n"), stype, scode, sit->second);
		}
	}

	return strEventInfo;
}

CString CEventAnalyzer::GetTouches() const
{
	CString strTouchesInfo;

	int protocol;
	strTouchesInfo.Append(TEXT("\r\nTouches: \r\n"));
	map<int, vector<TPIRQ_T> >::const_iterator cit = m_mapDevTpirqs.begin();

	for (; cit != m_mapDevTpirqs.end(); cit++)
	{
		protocol = m_mapEventDevices.find(cit->first)->second.protocol;
		
		strTouchesInfo.AppendFormat(TEXT(" event%d[%s]: %d irqs\r\n"), cit->first, 
			((protocol == MT_PROTOCOL_B) ? TEXT("B") : TEXT("A")), cit->second.size());
		vector<TPIRQ_T>::const_iterator irqit = cit->second.begin();
		for (int i = 1; irqit != cit->second.end(); irqit++, i++)
		{
			strTouchesInfo.AppendFormat(TEXT(" ----------[%d]----------\r\n"), i);
			strTouchesInfo.AppendFormat(TEXT(" touch_point: %d, point_num: %d\r\n"), 
				irqit->touch_point, irqit->point_num);
			
			vector<TPTOUCH_T>::const_iterator tchit = irqit->touches.begin();
			for (; tchit != irqit->touches.end(); tchit++)
			{
				if (protocol == MT_PROTOCOL_B)
				{
					if ((tchit->tracking_id == -1) ||
						(tchit->tracking_id == 0xFFFFFFFF))
					{
						strTouchesInfo.AppendFormat(TEXT(" [B]P%d UP!\r\n"),
							tchit->slot);
					}
					else 
					{
						strTouchesInfo.AppendFormat(TEXT(" [B]P%d(%d, %d)[p:%d,tm:%d] Down!\r\n"),
							tchit->slot, tchit->x, tchit->y, tchit->pressure, tchit->touch_major);
					}
				}
				else 
				{
					strTouchesInfo.AppendFormat(TEXT(" [A]P%d(%d, %d)[p:%d,tm:%d] Down!\r\n"),
						tchit->tracking_id, tchit->x, tchit->y, tchit->pressure, tchit->touch_major);
				}
			}
			if (protocol == MT_PROTOCOL_B)
			{
				strTouchesInfo.AppendFormat(TEXT(" syn: %d\r\n"), irqit->syns.syn_num);
			}
			else 
			{
				strTouchesInfo.AppendFormat(TEXT(" mt_syn: %d, syn: %d\r\n"),
					irqit->syns.mt_syn_num, irqit->syns.syn_num);
			}
		}
	}
	return strTouchesInfo;
}



//////////////////////////////////////////////////////////////////////////
// DIagnoser
int CEventDiagnoser::PushEvent(EVENT_T est, int index)
{
	int ret = ME_ERROR_NULL;

	EVENT_DIAGNOSE_T edt;
	edt.code = est.code;
	edt.index = index;
	edt.type = est.type;
	edt.value = est.value;

	m_stkEvents.push_back(edt);

	/* 先判断event是否合法 */
	if (est.type == EV_ABS)
	{
		switch (est.code)
		{
		case ABS_MT_POSITION_X:
			if (est.value > 2000) 
			{
				return ME_VALUE_INVALID_SUSPECTED;
			}
			else if (est.value < 0) 
			{
				return ME_VALUE_INVALID;
			}
			break;
		case ABS_MT_POSITION_Y:
			if (est.value > 2500) 
			{
				return ME_VALUE_INVALID_SUSPECTED;
			}
			else if (est.value < 0)
			{
				return ME_VALUE_INVALID;
			}
			break;
		case ABS_MT_PRESSURE:
		case ABS_MT_TOUCH_MAJOR:
		case ABS_MT_TOUCH_MINOR:
			if (est.value <= 0)
			{
				return ME_VALUE_INVALID;
			}
		default:
			break;
		}
	}

	if (est.code == SYN_REPORT)
	{
		if (!m_bProtocolB)
		{
			// A协议需要上报SYN_MT_REPORT事件
			vector<EVENT_DIAGNOSE_T>::reverse_iterator it = m_stkEvents.rbegin();
			for (; it != m_stkEvents.rend(); it++)
			{
				if (it->code == SYN_MT_REPORT)
				{
					break;
				}
			}
			if (it == m_stkEvents.rend())
			{
				ret = ME_NO_MT_SYNC;
			}
		}
		m_stkEvents.clear();
	}

	return ret;
}

int CEventDiagnoser::PushTouchA(TPTOUCH_T &tpt, int index)
{
	vector<TOUCH_DIAGNOSE_T>::iterator it = m_stkTouches.begin();
	for (; it != m_stkTouches.end(); it++)
	{
		if (tpt.type == it->tpt.type)
		{
			if (tpt.type == TPTYPE_KEY) /* key */
			{
				if (tpt.key == it->tpt.key)
				{
					if (IsKeyDown(it->tpt) && !IsKeyDown(tpt))
					{
						m_stkTouches.erase(it); /* KEY DOWN/UP paired */
					}
					else
					{
						/* update key */
						it->index = index;
						it->tpt = tpt;
					}
					return ME_ERROR_NULL;
				}
			}
			else if (tpt.tracking_id == it->tpt.tracking_id) /* touch */
			{
				int ret = ME_ERROR_NULL;
				if (IsXJump(tpt.x, it->tpt.x))
				{
					ret |= ME_POINT_X_JUMP;
				}
				if (IsYJump(tpt.y, it->tpt.y))
				{
					ret |= ME_POINT_Y_JUMP;
				}

				/* update point */
				it->index = index;
				it->tpt = tpt;
				return ret;
			}
		}
	}
	TOUCH_DIAGNOSE_T tdt;
	tdt.index = index;
	tdt.tpt = tpt;
	m_stkTouches.push_back(tdt);
	return ME_ERROR_NULL;
}

void CEventDiagnoser::SetResolution(int x, int y)
{
	JUMP_MAX_X = (int)(x * 0.1);
	JUMP_MAX_Y = (int)(y * 0.1);

	JUMP_MAX_X = (JUMP_MAX_X < 30) ? 100 : JUMP_MAX_X;
	JUMP_MAX_Y = (JUMP_MAX_Y < 50) ? 150 : JUMP_MAX_Y;
}

int CEventDiagnoser::PushTouchB(TPTOUCH_T &cur, int index)
{
	vector<TOUCH_DIAGNOSE_T>::iterator it = m_stkTouches.begin();
	for (; it != m_stkTouches.end(); it++)
	{
		if (cur.type == it->tpt.type)
		{
			if (cur.type == TPTYPE_KEY) /* key */
			{
				if (cur.key == it->tpt.key)
				{
					if (IsKeyDown(it->tpt) && !IsKeyDown(cur))
					{
						m_stkTouches.erase(it); /* KEY DOWN/UP paired */
					}
					else
					{
						/* update key */
						it->index = index;
						it->tpt = cur;
					}
					return ME_ERROR_NULL;
				}
			}
			else if (cur.slot == it->tpt.slot) /* touch */
			{
				//TRACE("[%d]P%d: %d -> %d\n", index, 
					//cur.slot, IsTouchUpB(it->tpt), IsTouchUpB(cur));

				/* Point DOWN/UP paired */
				if (!IsTouchUpB(it->tpt) && IsTouchUpB(cur))
				{
					m_stkTouches.erase(it);
					return ME_ERROR_NULL;
				}

				int ret = ME_ERROR_NULL;
				/* 由于B协议未能获得最早的XY值, 所以X/Y为1时不计算值 */
				if ((it->tpt.x != 1) && (it->tpt.y != 1))
				{
					if (IsXJump(cur.x, it->tpt.x))
					{
						ret |= ME_POINT_X_JUMP;
					}
					if (IsYJump(cur.y, it->tpt.y))
					{
						ret |= ME_POINT_Y_JUMP;
					}
				}
				/* update point */
				it->index = index;
				it->tpt = cur;
				return ret;
			}
		}
	}

	TOUCH_DIAGNOSE_T tdt;
	tdt.index = index;
	tdt.tpt = cur;
	m_stkTouches.push_back(tdt);
	return ME_ERROR_NULL;
}


int CEventDiagnoser::PushTouch(TPTOUCH_T tpt, int index)
{
	if (m_bProtocolB)
	{
		return PushTouchB(tpt, index);
	}
	else
	{
		return PushTouchA(tpt, index);
	}
}

int CEventDiagnoser::GetPointId(TPTOUCH_T &tpt)
{
	if (m_bProtocolB)
	{
		return tpt.slot;
	}
	else 
	{
		return tpt.tracking_id;
	}
}



int CEventDiagnoser::Diagnose(TPIRQ_T &irq, int index)
{
	int ret = ME_ERROR_NULL;
	int error = ME_ERROR_NULL;

	vector<EVENT_T>::iterator eit = irq.event_stack.begin();
	for (; eit != irq.event_stack.end(); eit++)
	{
		error = PushEvent(*eit, index);
		if (error > ME_ERROR_NULL)
		{
			DIAGNOSE_ERROR_T det;
			det.dev_no = m_DeviceNo;
			det.index = index;
			det.type = error;
			det.err_param_1 = eit->code;
			det.err_param_2 = eit->value;
			AddError(index, det);
			ret |= error;
		}
	}

	vector<TPTOUCH_T>::iterator tit = irq.touches.begin();
	for (; tit != irq.touches.end(); tit++)
	{
		error = PushTouch(*tit, index);
		if (error > ME_ERROR_NULL)
		{
			DIAGNOSE_ERROR_T det;
			det.dev_no = m_DeviceNo;
			det.index = index;
			det.type = error;
			det.err_param_1 = GetPointId(*tit);

			AddError(index, det);
			ret |= error;
		}
	}
	

	return ret;
}

void CEventDiagnoser::AddError(int index, DIAGNOSE_ERROR_T &det)
{
	map<int, set<DIAGNOSE_ERROR_T> > &error = m_mapDevErrors[m_DeviceNo];

	error[index].insert(det);
}


void CEventDiagnoser::Reset(BOOL bPrtlB, int devNo)
{
	m_DeviceNo = devNo;
	m_bProtocolB = bPrtlB;
	m_stkTouches.clear();
	m_stkEvents.clear();
	m_mapDevErrors[devNo].clear();
}

void CEventDiagnoser::Flush()
{
	if (!m_stkEvents.empty())
	{
		DIAGNOSE_ERROR_T ert;
		ert.dev_no = m_DeviceNo;
		ert.index = m_stkEvents.at(0).index;
		ert.type = ME_NO_SYNC;
		AddError(ert.index, ert);
	}
	
	vector<TOUCH_DIAGNOSE_T>::iterator it = m_stkTouches.begin();
	for (; it != m_stkTouches.end(); it++)
	{
		DIAGNOSE_ERROR_T ert;
		ert.dev_no = m_DeviceNo;
		ert.index = it->index;
		if (it->tpt.type == TPTYPE_KEY)
		{
			if (IsKeyDown(it->tpt))
			{
				ert.type = ME_KEY_NO_UP;
				ert.err_param_1 = it->tpt.key;
			}
		}
		else 
		{
			ert.err_param_1 = GetPointId(it->tpt);
			ert.type = ME_POINT_NO_UP;
		}
		AddError(it->index, ert);
	}
}

int CEventDiagnoser::GetDiagnoseResult(int index)
{
	map<int, set<DIAGNOSE_ERROR_T> > &errors = m_mapDevErrors[m_DeviceNo];

	if (errors.empty()) {
		return ME_ERROR_NULL;
	}
	
	map<int, set<DIAGNOSE_ERROR_T> >::const_iterator cit = errors.begin();

	for (; cit != errors.end(); cit++)
	{
		if (cit->first > index) 
		{
			break;
		}
		if (cit->first == index)
		{
			int ret = ME_ERROR_NULL;
			set<DIAGNOSE_ERROR_T>::const_iterator dcit = cit->second.begin();
			for (; dcit != cit->second.end(); dcit++)
			{
				ret |= dcit->type;
			}
			return ret;
		}
		
	}
	return ME_ERROR_NULL;
}

map<int, CString> CEventDiagnoser::SYMBOLIC_DIAG_ERRORS;
void CEventDiagnoser::InitSymbolicErrors()
{
	SYMBOLIC_DIAG_ERRORS[ME_ERROR_NULL] = TEXT("ME_ERROR_NULL");
	SYMBOLIC_DIAG_ERRORS[ME_VALUE_INVALID_SUSPECTED] = TEXT("ME_VALUE_INVALID_SUSPECTED");
	SYMBOLIC_DIAG_ERRORS[ME_VALUE_INVALID] = TEXT("ME_VALUE_INVALID");
	SYMBOLIC_DIAG_ERRORS[ME_POINT_NO_DOWN] = TEXT("ME_POINT_NO_DOWN");
	SYMBOLIC_DIAG_ERRORS[ME_NO_MT_SYNC] = TEXT("ME_NO_MT_SYNC");
	SYMBOLIC_DIAG_ERRORS[ME_POINT_NO_UP] = TEXT("ME_POINT_NO_UP");
	SYMBOLIC_DIAG_ERRORS[ME_POINT_X_JUMP] = TEXT("ME_POINT_X_JUMP");
	SYMBOLIC_DIAG_ERRORS[ME_POINT_Y_JUMP] = TEXT("ME_POINT_Y_JUMP");
	SYMBOLIC_DIAG_ERRORS[ME_POINT_XY_JUMP] = TEXT("ME_POINT_XY_JUMP");
	SYMBOLIC_DIAG_ERRORS[ME_KEY_NO_DOWN] = TEXT("ME_KEY_NO_DOWN");
	SYMBOLIC_DIAG_ERRORS[ME_KEY_NO_UP] = TEXT("ME_KEY_NO_UP");
	SYMBOLIC_DIAG_ERRORS[ME_NO_SYNC] = TEXT("ME_NO_SYNC");
	SYMBOLIC_DIAG_ERRORS[ME_NO_BTN_TOUCH] = TEXT("ME_NO_BTN_TOUCH");
	SYMBOLIC_DIAG_ERRORS[ME_EVENT_NOT_SUPPORTED] = TEXT("ME_EVENT_NOT_SUPPORTED");
}
CString CEventDiagnoser::ToErrorString(int dev_no, int index, int errorcode)
{
	map<int, set<DIAGNOSE_ERROR_T> > &errors = m_mapDevErrors[dev_no];

	if (errors.empty())
	{
		return TEXT("Error:DeviceNotFound");
	}
	set<DIAGNOSE_ERROR_T> &derr = errors[index];

	if (derr.empty())
	{
		return TEXT("Error:ErrorNotFound");
	}

	CString strRet;
	int type;
	set<DIAGNOSE_ERROR_T>::const_iterator cit = derr.begin();
	for (; cit != derr.end(); cit++)
	{
		type = errorcode & cit->type;
		if (type)
		{
			strRet.AppendFormat(TEXT("%s"), SYMBOLIC_DIAG_ERRORS[type]);
			switch (type) 
			{
			case ME_KEY_NO_UP:
			case ME_KEY_NO_DOWN:
				strRet.AppendFormat(TEXT("(%s)"), CEventPair::U16ToCode((__u16)cit->err_param_1, EV_KEY));
				break;
			case ME_VALUE_INVALID:
			case ME_VALUE_INVALID_SUSPECTED:
				strRet.AppendFormat(TEXT("(%s: %d)"), 
					CEventPair::U16ToCode((__u16)cit->err_param_1, EV_ABS), cit->err_param_2);
				break;
			case ME_POINT_NO_DOWN:
			case ME_POINT_NO_UP:
			case ME_POINT_X_JUMP:
			case ME_POINT_Y_JUMP:
			case ME_POINT_XY_JUMP:
				strRet.AppendFormat(TEXT("(P%d)"), cit->err_param_1);
				break;
			default:
				strRet.AppendFormat(TEXT("(%d,%d)"), cit->err_param_1,
					cit->err_param_2);
				break;
			}
		}
		strRet.AppendFormat(TEXT("\r\n"));
	}
	return strRet;
}

int CEventDiagnoser::GetNextErrorItem(int offset) 
{
	map<int, set<DIAGNOSE_ERROR_T> > &errors = m_mapDevErrors[m_DeviceNo];

	if (errors.empty())
	{
		return -1;
	}

	map<int, set<DIAGNOSE_ERROR_T> >::const_iterator cit = errors.begin();

	for (; cit != errors.end(); cit++)
	{
		if (!cit->second.empty() && (cit->first > offset)) {
			return cit->first;
		}
	}

	return errors.begin()->first;
}

//////////////////////////////////////////////////////////////////////////

int CEventAnalyzer::GetNextErrorItem(int offset) 
{
	return m_diagnoser.GetNextErrorItem(offset);
}

BOOL CEventAnalyzer::DiagnoseEvents(vector<TPIRQ_T> &vec, BOOL bProtocolB, int devNo)
{
	int x = m_mapEventDevices[devNo].x_res;
	int y = m_mapEventDevices[devNo].y_res;
	m_diagnoser.SetResolution(x, y);

	m_diagnoser.Reset(bProtocolB, devNo);

	vector<TPIRQ_T>::iterator iit = vec.begin();
	for (int i = 0; iit != vec.end(); iit++, i++)
	{
		m_diagnoser.Diagnose(*iit, i);
	}
	m_diagnoser.Flush();

	iit = vec.begin();
	for (int i = 0; iit != vec.end(); iit++, i++)
	{
		iit->dgnflag = m_diagnoser.GetDiagnoseResult(i);
	}
	return TRUE;
}

BOOL CEventAnalyzer::DetectError()
{
	map<int, vector<TPIRQ_T> >::iterator it = m_mapDevTpirqs.begin();

	for (; it != m_mapDevTpirqs.end(); it++)
	{
		int protocol = GetProtocolType(it->first);
		DiagnoseEvents(it->second,  protocol == MT_PROTOCOL_B, it->first);
	}

	return FALSE;
}

