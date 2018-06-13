#include "StdAfx.h"

#include "EventAnalyzer.h"


map<CString, __u16> CEventPair::TYPES;
map<CString, __u16> CEventPair::ABS_CODES;
map<CString, __u16> CEventPair::KEY_CODES;
map<CString, __u16> CEventPair::BTN_CODES;
map<CString, __u16> CEventPair::SYN_CODES;
map<CString, __s32> CEventPair::VALUES;
map<CString, __u16> CEventPair::DEV_PROPERTIES;

void CEventPair::InitEventPairs()
{
	VALUES[CString(TEXT("DOWN"))] = 0x01;
	VALUES[CString(TEXT("UP"))] = 0x00;
	/*
	* MT_TOOL types
	*/
	VALUES[CString(TEXT("MT_TOOL_FINGER"))] = 0;
	VALUES[CString(TEXT("MT_TOOL_PEN"))] = 1;

	DEV_PROPERTIES[CString(TEXT("INPUT_PROP_POINTER"))] = (0x00/*needsapointer*/);
	DEV_PROPERTIES[CString(TEXT("INPUT_PROP_DIRECT"))] = (0x01/*directinputdevices*/);
	DEV_PROPERTIES[CString(TEXT("INPUT_PROP_BUTTONPAD"))] = (0x02/*hasbutton(s)underpad*/);
	DEV_PROPERTIES[CString(TEXT("INPUT_PROP_SEMI_MT"))] = (0x03/*touchrectangleonly*/);
	DEV_PROPERTIES[CString(TEXT("INPUT_PROP_TOPBUTTONPAD"))] = (0x04/*softbuttonsattopofpad*/);
	DEV_PROPERTIES[CString(TEXT("INPUT_PROP_POINTING_STICK"))] = (0x05/*isapointingstick*/);
	DEV_PROPERTIES[CString(TEXT("INPUT_PROP_MAX"))] = (0x1f);
	DEV_PROPERTIES[CString(TEXT("INPUT_PROP_CNT"))] = ((INPUT_PROP_MAX+1));

	TYPES[CString(TEXT("EV_SYN"))] = 0x00;
	TYPES[CString(TEXT("EV_KEY"))] = 0x01;
	TYPES[CString(TEXT("EV_REL"))] = 0x02;
	TYPES[CString(TEXT("EV_ABS"))] = 0x03;
	TYPES[CString(TEXT("EV_MSC"))] = 0x04;
	TYPES[CString(TEXT("EV_SW"))] = 0x05;
	TYPES[CString(TEXT("EV_LED"))] = 0x11;
	TYPES[CString(TEXT("EV_SND"))] = 0x12;
	TYPES[CString(TEXT("EV_REP"))] = 0x14;
	TYPES[CString(TEXT("EV_FF"))] = 0x15;
	TYPES[CString(TEXT("EV_PWR"))] = 0x16;
	TYPES[CString(TEXT("EV_FF_STATUS"))] = 0x17;
	TYPES[CString(TEXT("EV_MAX"))] = 0x1f;
	TYPES[CString(TEXT("EV_CNT"))] = (EV_MAX+1);

	SYN_CODES[CString(TEXT("SYN_REPORT"))] = 0;
	SYN_CODES[CString(TEXT("SYN_CONFIG"))] = 1;
	SYN_CODES[CString(TEXT("SYN_MT_REPORT"))] = 2;
	SYN_CODES[CString(TEXT("SYN_DROPPED"))] = 3;
	SYN_CODES[CString(TEXT("SYN_MAX"))] = 0xf;
	SYN_CODES[CString(TEXT("SYN_CNT"))] = (SYN_MAX+1);

	ABS_CODES[CString(TEXT("ABS_MT_SLOT"))] = 0x2f/* MT slot being modified */;
	ABS_CODES[CString(TEXT("ABS_MT_TOUCH_MAJOR"))] = 0x30/* Major axis of touching ellipse */;
	ABS_CODES[CString(TEXT("ABS_MT_TOUCH_MINOR"))] = 0x31/* Minor axis (omit if circular) */;
	ABS_CODES[CString(TEXT("ABS_MT_WIDTH_MAJOR"))] = 0x32/* Major axis of approaching ellipse */;
	ABS_CODES[CString(TEXT("ABS_MT_WIDTH_MINOR"))] = 0x33/* Minor axis (omit if circular) */;
	ABS_CODES[CString(TEXT("ABS_MT_ORIENTATION"))] = 0x34/* Ellipse orientation */;
	ABS_CODES[CString(TEXT("ABS_MT_POSITION_X"))] = 0x35/* Center X touch position */;
	ABS_CODES[CString(TEXT("ABS_MT_POSITION_Y"))] = 0x36/* Center Y touch position */;
	ABS_CODES[CString(TEXT("ABS_MT_TOOL_TYPE"))] = 0x37/* Type of touching device */;
	ABS_CODES[CString(TEXT("ABS_MT_BLOB_ID"))] = 0x38/* Group a set of packets as a blob */;
	ABS_CODES[CString(TEXT("ABS_MT_TRACKING_ID"))] = 0x39/* Unique ID of initiated contact */;
	ABS_CODES[CString(TEXT("ABS_MT_PRESSURE"))] = 0x3a/* Pressure on contact area */;
	ABS_CODES[CString(TEXT("ABS_MT_DISTANCE"))] = 0x3b/* Contact hover distance */;
	ABS_CODES[CString(TEXT("ABS_MT_TOOL_X"))] = 0x3c/* Center X tool position */;
	ABS_CODES[CString(TEXT("ABS_MT_TOOL_Y"))] = 0x3d/* Center Y tool position */;
	ABS_CODES[CString(TEXT("ABS_X"))] = 0x00;
	ABS_CODES[CString(TEXT("ABS_Y"))] = 0x01;
	ABS_CODES[CString(TEXT("ABS_Z"))] = 0x02;
	ABS_CODES[CString(TEXT("ABS_RX"))] = 0x03;
	ABS_CODES[CString(TEXT("ABS_RY"))] = 0x04;
	ABS_CODES[CString(TEXT("ABS_RZ"))] = 0x05;
	ABS_CODES[CString(TEXT("ABS_THROTTLE"))] = 0x06;
	ABS_CODES[CString(TEXT("ABS_RUDDER"))] = 0x07;
	ABS_CODES[CString(TEXT("ABS_WHEEL"))] = 0x08;
	ABS_CODES[CString(TEXT("ABS_GAS"))] = 0x09;
	ABS_CODES[CString(TEXT("ABS_BRAKE"))] = 0x0a;
	ABS_CODES[CString(TEXT("ABS_HAT"))] = 0x10;
	ABS_CODES[CString(TEXT("ABS_HAT0Y"))] = 0x11;
	ABS_CODES[CString(TEXT("ABS_HAT1X"))] = 0x12;
	ABS_CODES[CString(TEXT("ABS_HAT1Y"))] = 0x13;
	ABS_CODES[CString(TEXT("ABS_HAT2X"))] = 0x14;
	ABS_CODES[CString(TEXT("ABS_HAT2Y"))] = 0x15;
	ABS_CODES[CString(TEXT("ABS_HAT3X"))] = 0x16;
	ABS_CODES[CString(TEXT("ABS_HAT3Y"))] = 0x17;
	ABS_CODES[CString(TEXT("ABS_PRESSURE"))] = 0x18;
	ABS_CODES[CString(TEXT("ABS_DISTANCE"))] = 0x19;
	ABS_CODES[CString(TEXT("ABS_TILT_X"))] = 0x1a;
	ABS_CODES[CString(TEXT("ABS_TILT_Y"))] = 0x1b;
	ABS_CODES[CString(TEXT("ABS_TOOL_WIDTH"))] = 0x1c;
	ABS_CODES[CString(TEXT("ABS_VOLUME"))] = 0x20;
	ABS_CODES[CString(TEXT("ABS_MISC"))] = 0x28;

	KEY_CODES[CString(TEXT("KEY_RESERVED"))] = 0;
	KEY_CODES[CString(TEXT("KEY_ESC"))] = 1;
	KEY_CODES[CString(TEXT("KEY_1"))] = 2;
	KEY_CODES[CString(TEXT("KEY_2"))] = 3;
	KEY_CODES[CString(TEXT("KEY_3"))] = 4;
	KEY_CODES[CString(TEXT("KEY_4"))] = 5;
	KEY_CODES[CString(TEXT("KEY_5"))] = 6;
	KEY_CODES[CString(TEXT("KEY_6"))] = 7;
	KEY_CODES[CString(TEXT("KEY_7"))] = 8;
	KEY_CODES[CString(TEXT("KEY_8"))] = 9;
	KEY_CODES[CString(TEXT("KEY_9"))] = 10;
	KEY_CODES[CString(TEXT("KEY_0"))] = 11;
	KEY_CODES[CString(TEXT("KEY_MINUS"))] = 12;
	KEY_CODES[CString(TEXT("KEY_EQUAL"))] = 13;
	KEY_CODES[CString(TEXT("KEY_BACKSPACE"))] = 14;
	KEY_CODES[CString(TEXT("KEY_TAB"))] = 15;
	KEY_CODES[CString(TEXT("KEY_Q"))] = 16;
	KEY_CODES[CString(TEXT("KEY_W"))] = 17;
	KEY_CODES[CString(TEXT("KEY_E"))] = 18;
	KEY_CODES[CString(TEXT("KEY_R"))] = 19;
	KEY_CODES[CString(TEXT("KEY_T"))] = 20;
	KEY_CODES[CString(TEXT("KEY_Y"))] = 21;
	KEY_CODES[CString(TEXT("KEY_U"))] = 22;
	KEY_CODES[CString(TEXT("KEY_I"))] = 23;
	KEY_CODES[CString(TEXT("KEY_O"))] = 24;
	KEY_CODES[CString(TEXT("KEY_P"))] = 25;
	KEY_CODES[CString(TEXT("KEY_LEFTBRACE"))] = 26;
	KEY_CODES[CString(TEXT("KEY_RIGHTBRACE"))] = 27;
	KEY_CODES[CString(TEXT("KEY_ENTER"))] = 28;
	KEY_CODES[CString(TEXT("KEY_LEFTCTRL"))] = 29;
	KEY_CODES[CString(TEXT("KEY_A"))] = 30;
	KEY_CODES[CString(TEXT("KEY_S"))] = 31;
	KEY_CODES[CString(TEXT("KEY_D"))] = 32;
	KEY_CODES[CString(TEXT("KEY_F"))] = 33;
	KEY_CODES[CString(TEXT("KEY_G"))] = 34;
	KEY_CODES[CString(TEXT("KEY_H"))] = 35;
	KEY_CODES[CString(TEXT("KEY_J"))] = 36;
	KEY_CODES[CString(TEXT("KEY_K"))] = 37;
	KEY_CODES[CString(TEXT("KEY_L"))] = 38;
	KEY_CODES[CString(TEXT("KEY_SEMICOLON"))] = 39;
	KEY_CODES[CString(TEXT("KEY_APOSTROPHE"))] = 40;
	KEY_CODES[CString(TEXT("KEY_GRAVE"))] = 41;
	KEY_CODES[CString(TEXT("KEY_LEFTSHIFT"))] = 42;
	KEY_CODES[CString(TEXT("KEY_BACKSLASH"))] = 43;
	KEY_CODES[CString(TEXT("KEY_Z"))] = 44;
	KEY_CODES[CString(TEXT("KEY_X"))] = 45;
	KEY_CODES[CString(TEXT("KEY_C"))] = 46;
	KEY_CODES[CString(TEXT("KEY_V"))] = 47;
	KEY_CODES[CString(TEXT("KEY_B"))] = 48;
	KEY_CODES[CString(TEXT("KEY_N"))] = 49;
	KEY_CODES[CString(TEXT("KEY_M"))] = 50;
	KEY_CODES[CString(TEXT("KEY_COMMA"))] = 51;
	KEY_CODES[CString(TEXT("KEY_DOT"))] = 52;
	KEY_CODES[CString(TEXT("KEY_SLASH"))] = 53;
	KEY_CODES[CString(TEXT("KEY_RIGHTSHIFT"))] = 54;
	KEY_CODES[CString(TEXT("KEY_KPASTERISK"))] = 55;
	KEY_CODES[CString(TEXT("KEY_LEFTALT"))] = 56;
	KEY_CODES[CString(TEXT("KEY_SPACE"))] = 57;
	KEY_CODES[CString(TEXT("KEY_CAPSLOCK"))] = 58;
	KEY_CODES[CString(TEXT("KEY_F1"))] = 59;
	KEY_CODES[CString(TEXT("KEY_F2"))] = 60;
	KEY_CODES[CString(TEXT("KEY_F3"))] = 61;
	KEY_CODES[CString(TEXT("KEY_F4"))] = 62;
	KEY_CODES[CString(TEXT("KEY_F5"))] = 63;
	KEY_CODES[CString(TEXT("KEY_F6"))] = 64;
	KEY_CODES[CString(TEXT("KEY_F7"))] = 65;
	KEY_CODES[CString(TEXT("KEY_F8"))] = 66;
	KEY_CODES[CString(TEXT("KEY_F9"))] = 67;
	KEY_CODES[CString(TEXT("KEY_F10"))] = 68;
	KEY_CODES[CString(TEXT("KEY_NUMLOCK"))] = 69;
	KEY_CODES[CString(TEXT("KEY_SCROLLLOCK"))] = 70;
	KEY_CODES[CString(TEXT("KEY_KP7"))] = 71;
	KEY_CODES[CString(TEXT("KEY_KP8"))] = 72;
	KEY_CODES[CString(TEXT("KEY_KP9"))] = 73;
	KEY_CODES[CString(TEXT("KEY_KPMINUS"))] = 74;
	KEY_CODES[CString(TEXT("KEY_KP4"))] = 75;
	KEY_CODES[CString(TEXT("KEY_KP5"))] = 76;
	KEY_CODES[CString(TEXT("KEY_KP6"))] = 77;
	KEY_CODES[CString(TEXT("KEY_KPPLUS"))] = 78;
	KEY_CODES[CString(TEXT("KEY_KP1"))] = 79;
	KEY_CODES[CString(TEXT("KEY_KP2"))] = 80;
	KEY_CODES[CString(TEXT("KEY_KP3"))] = 81;
	KEY_CODES[CString(TEXT("KEY_KP0"))] = 82;
	KEY_CODES[CString(TEXT("KEY_KPDOT"))] = 83;
	KEY_CODES[CString(TEXT("KEY_ZENKAKUHANKAKU"))] = 85;
	KEY_CODES[CString(TEXT("KEY_102ND"))] = 86;
	KEY_CODES[CString(TEXT("KEY_F11"))] = 87;
	KEY_CODES[CString(TEXT("KEY_F12"))] = 88;
	KEY_CODES[CString(TEXT("KEY_RO"))] = 89;
	KEY_CODES[CString(TEXT("KEY_KATAKANA"))] = 90;
	KEY_CODES[CString(TEXT("KEY_HIRAGANA"))] = 91;
	KEY_CODES[CString(TEXT("KEY_HENKAN"))] = 92;
	KEY_CODES[CString(TEXT("KEY_KATAKANAHIRAGANA"))] = 93;
	KEY_CODES[CString(TEXT("KEY_MUHENKAN"))] = 94;
	KEY_CODES[CString(TEXT("KEY_KPJPCOMMA"))] = 95;
	KEY_CODES[CString(TEXT("KEY_KPENTER"))] = 96;
	KEY_CODES[CString(TEXT("KEY_RIGHTCTRL"))] = 97;
	KEY_CODES[CString(TEXT("KEY_KPSLASH"))] = 98;
	KEY_CODES[CString(TEXT("KEY_SYSRQ"))] = 99;
	KEY_CODES[CString(TEXT("KEY_RIGHTALT"))] = 100;
	KEY_CODES[CString(TEXT("KEY_LINEFEED"))] = 101;
	KEY_CODES[CString(TEXT("KEY_HOME"))] = 102;
	KEY_CODES[CString(TEXT("KEY_UP"))] = 103;
	KEY_CODES[CString(TEXT("KEY_PAGEUP"))] = 104;
	KEY_CODES[CString(TEXT("KEY_LEFT"))] = 105;
	KEY_CODES[CString(TEXT("KEY_RIGHT"))] = 106;
	KEY_CODES[CString(TEXT("KEY_END"))] = 107;
	KEY_CODES[CString(TEXT("KEY_DOWN"))] = 108;
	KEY_CODES[CString(TEXT("KEY_PAGEDOWN"))] = 109;
	KEY_CODES[CString(TEXT("KEY_INSERT"))] = 110;
	KEY_CODES[CString(TEXT("KEY_DELETE"))] = 111;
	KEY_CODES[CString(TEXT("KEY_MACRO"))] = 112;
	KEY_CODES[CString(TEXT("KEY_MUTE"))] = 113;
	KEY_CODES[CString(TEXT("KEY_VOLUMEDOWN"))] = 114;
	KEY_CODES[CString(TEXT("KEY_VOLUMEUP"))] = 115;
	KEY_CODES[CString(TEXT("KEY_POWER"))] = 116/* SC System Power Down */;
	KEY_CODES[CString(TEXT("KEY_KPEQUAL"))] = 117;
	KEY_CODES[CString(TEXT("KEY_KPPLUSMINUS"))] = 118;
	KEY_CODES[CString(TEXT("KEY_PAUSE"))] = 119;
	KEY_CODES[CString(TEXT("KEY_SCALE"))] = 120/* AL Compiz Scale (Expose) */;
	KEY_CODES[CString(TEXT("KEY_KPCOMMA"))] = 121;
	KEY_CODES[CString(TEXT("KEY_HANGEUL"))] = 122;
	KEY_CODES[CString(TEXT("KEY_HANGUEL"))] = KEY_HANGEUL;
	KEY_CODES[CString(TEXT("KEY_HANJA"))] = 123;
	KEY_CODES[CString(TEXT("KEY_YEN"))] = 124;
	KEY_CODES[CString(TEXT("KEY_LEFTMETA"))] = 125;
	KEY_CODES[CString(TEXT("KEY_RIGHTMETA"))] = 126;
	KEY_CODES[CString(TEXT("KEY_COMPOSE"))] = 127;
	KEY_CODES[CString(TEXT("KEY_STOP"))] = 128/* AC Stop */;
	KEY_CODES[CString(TEXT("KEY_AGAIN"))] = 129;
	KEY_CODES[CString(TEXT("KEY_PROPS"))] = 130/* AC Properties */;
	KEY_CODES[CString(TEXT("KEY_UNDO"))] = 131/* AC Undo */;
	KEY_CODES[CString(TEXT("KEY_FRONT"))] = 132;
	KEY_CODES[CString(TEXT("KEY_COPY"))] = 133/* AC Copy */;
	KEY_CODES[CString(TEXT("KEY_OPEN"))] = 134/* AC Open */;
	KEY_CODES[CString(TEXT("KEY_PASTE"))] = 135/* AC Paste */;
	KEY_CODES[CString(TEXT("KEY_FIND"))] = 136/* AC Search */;
	KEY_CODES[CString(TEXT("KEY_CUT"))] = 137/* AC Cut */;
	KEY_CODES[CString(TEXT("KEY_HELP"))] = 138/* AL Integrated Help Center */;
	KEY_CODES[CString(TEXT("KEY_MENU"))] = 139/* Menu (show menu) */;
	KEY_CODES[CString(TEXT("KEY_CALC"))] = 140/* AL Calculator */;
	KEY_CODES[CString(TEXT("KEY_SETUP"))] = 141;
	KEY_CODES[CString(TEXT("KEY_SLEEP"))] = 142/* SC System Sleep */;
	KEY_CODES[CString(TEXT("KEY_WAKEUP"))] = 143/* System Wake Up */;
	KEY_CODES[CString(TEXT("KEY_FILE"))] = 144/* AL Local Machine Browser */;
	KEY_CODES[CString(TEXT("KEY_SENDFILE"))] = 145;
	KEY_CODES[CString(TEXT("KEY_DELETEFILE"))] = 146;
	KEY_CODES[CString(TEXT("KEY_XFER"))] = 147;
	KEY_CODES[CString(TEXT("KEY_PROG1"))] = 148;
	KEY_CODES[CString(TEXT("KEY_PROG2"))] = 149;
	KEY_CODES[CString(TEXT("KEY_WWW"))] = 150/* AL Internet Browser */;
	KEY_CODES[CString(TEXT("KEY_MSDOS"))] = 151;
	KEY_CODES[CString(TEXT("KEY_COFFEE"))] = 152/* AL Terminal Lock/Screensaver */;
	KEY_CODES[CString(TEXT("KEY_SCREENLOCK"))] = KEY_COFFEE;
	KEY_CODES[CString(TEXT("KEY_DIRECTION"))] = 153;
	KEY_CODES[CString(TEXT("KEY_CYCLEWINDOWS"))] = 154;
	KEY_CODES[CString(TEXT("KEY_MAIL"))] = 155;
	KEY_CODES[CString(TEXT("KEY_BOOKMARKS"))] = 156/* AC Bookmarks */;
	KEY_CODES[CString(TEXT("KEY_COMPUTER"))] = 157;
	KEY_CODES[CString(TEXT("KEY_BACK"))] = 158/* AC Back */;
	KEY_CODES[CString(TEXT("KEY_FORWARD"))] = 159/* AC Forward */;
	KEY_CODES[CString(TEXT("KEY_CLOSECD"))] = 160;
	KEY_CODES[CString(TEXT("KEY_EJECTCD"))] = 161;
	KEY_CODES[CString(TEXT("KEY_EJECTCLOSECD"))] = 162;
	KEY_CODES[CString(TEXT("KEY_NEXTSONG"))] = 163;
	KEY_CODES[CString(TEXT("KEY_PLAYPAUSE"))] = 164;
	KEY_CODES[CString(TEXT("KEY_PREVIOUSSONG"))] = 165;
	KEY_CODES[CString(TEXT("KEY_STOPCD"))] = 166;
	KEY_CODES[CString(TEXT("KEY_RECORD"))] = 167;
	KEY_CODES[CString(TEXT("KEY_REWIND"))] = 168;
	KEY_CODES[CString(TEXT("KEY_PHONE"))] = 169/* Media Select Telephone */;
	KEY_CODES[CString(TEXT("KEY_ISO"))] = 170;
	KEY_CODES[CString(TEXT("KEY_CONFIG"))] = 171/* AL Consumer Control Configuration */;
	KEY_CODES[CString(TEXT("KEY_HOMEPAGE"))] = 172/* AC Home */;
	KEY_CODES[CString(TEXT("KEY_REFRESH"))] = 173/* AC Refresh */;
	KEY_CODES[CString(TEXT("KEY_EXIT"))] = 174/* AC Exit */;
	KEY_CODES[CString(TEXT("KEY_MOVE"))] = 175;
	KEY_CODES[CString(TEXT("KEY_EDIT"))] = 176;
	KEY_CODES[CString(TEXT("KEY_SCROLLUP"))] = 177;
	KEY_CODES[CString(TEXT("KEY_SCROLLDOWN"))] = 178;
	KEY_CODES[CString(TEXT("KEY_KPLEFTPAREN"))] = 179;
	KEY_CODES[CString(TEXT("KEY_KPRIGHTPAREN"))] = 180;
	KEY_CODES[CString(TEXT("KEY_NEW"))] = 181/* AC New */;
	KEY_CODES[CString(TEXT("KEY_REDO"))] = 182/* AC Redo/Repeat */;
	KEY_CODES[CString(TEXT("KEY_F13"))] = 183;
	KEY_CODES[CString(TEXT("KEY_F14"))] = 184;
	KEY_CODES[CString(TEXT("KEY_F15"))] = 185;
	KEY_CODES[CString(TEXT("KEY_F16"))] = 186;
	KEY_CODES[CString(TEXT("KEY_F17"))] = 187;
	KEY_CODES[CString(TEXT("KEY_F18"))] = 188;
	KEY_CODES[CString(TEXT("KEY_F19"))] = 189;
	KEY_CODES[CString(TEXT("KEY_F20"))] = 190;
	KEY_CODES[CString(TEXT("KEY_F21"))] = 191;
	KEY_CODES[CString(TEXT("KEY_F22"))] = 192;
	KEY_CODES[CString(TEXT("KEY_F23"))] = 193;
	KEY_CODES[CString(TEXT("KEY_F24"))] = 194;
	KEY_CODES[CString(TEXT("KEY_PLAYCD"))] = 200;
	KEY_CODES[CString(TEXT("KEY_PAUSECD"))] = 201;
	KEY_CODES[CString(TEXT("KEY_PROG3"))] = 202;
	KEY_CODES[CString(TEXT("KEY_PROG4"))] = 203;
	KEY_CODES[CString(TEXT("KEY_DASHBOARD"))] = 204/* AL Dashboard */;
	KEY_CODES[CString(TEXT("KEY_SUSPEND"))] = 205;
	KEY_CODES[CString(TEXT("KEY_CLOSE"))] = 206/* AC Close */;
	KEY_CODES[CString(TEXT("KEY_PLAY"))] = 207;
	KEY_CODES[CString(TEXT("KEY_FASTFORWARD"))] = 208;
	KEY_CODES[CString(TEXT("KEY_BASSBOOST"))] = 209;
	KEY_CODES[CString(TEXT("KEY_PRINT"))] = 210/* AC Print */;
	KEY_CODES[CString(TEXT("KEY_HP"))] = 211;
	KEY_CODES[CString(TEXT("KEY_CAMERA"))] = 212;
	KEY_CODES[CString(TEXT("KEY_SOUND"))] = 213;
	KEY_CODES[CString(TEXT("KEY_QUESTION"))] = 214;
	KEY_CODES[CString(TEXT("KEY_EMAIL"))] = 215;
	KEY_CODES[CString(TEXT("KEY_CHAT"))] = 216;
	KEY_CODES[CString(TEXT("KEY_SEARCH"))] = 217;
	KEY_CODES[CString(TEXT("KEY_CONNECT"))] = 218;
	KEY_CODES[CString(TEXT("KEY_FINANCE"))] = 219/* AL Checkbook/Finance */;
	KEY_CODES[CString(TEXT("KEY_SPORT"))] = 220;
	KEY_CODES[CString(TEXT("KEY_SHOP"))] = 221;
	KEY_CODES[CString(TEXT("KEY_ALTERASE"))] = 222;
	KEY_CODES[CString(TEXT("KEY_CANCEL"))] = 223/* AC Cancel */;
	KEY_CODES[CString(TEXT("KEY_BRIGHTNESSDOWN"))] = 224;
	KEY_CODES[CString(TEXT("KEY_BRIGHTNESSUP"))] = 225;
	KEY_CODES[CString(TEXT("KEY_MEDIA"))] = 226;
	KEY_CODES[CString(TEXT("KEY_SWITCHVIDEOMODE"))] = 227/* Cycle between available video outputs (Monitor/LCD/TV-out/etc) */;
	KEY_CODES[CString(TEXT("KEY_KBDILLUMTOGGLE"))] = 228;
	KEY_CODES[CString(TEXT("KEY_KBDILLUMDOWN"))] = 229;
	KEY_CODES[CString(TEXT("KEY_KBDILLUMUP"))] = 230;
	KEY_CODES[CString(TEXT("KEY_SEND"))] = 231/* AC Send */;
	KEY_CODES[CString(TEXT("KEY_REPLY"))] = 232/* AC Reply */;
	KEY_CODES[CString(TEXT("KEY_FORWARDMAIL"))] = 233/* AC Forward Msg */;
	KEY_CODES[CString(TEXT("KEY_SAVE"))] = 234/* AC Save */;
	KEY_CODES[CString(TEXT("KEY_DOCUMENTS"))] = 235;
	KEY_CODES[CString(TEXT("KEY_BATTERY"))] = 236;
	KEY_CODES[CString(TEXT("KEY_BLUETOOTH"))] = 237;
	KEY_CODES[CString(TEXT("KEY_WLAN"))] = 238;
	KEY_CODES[CString(TEXT("KEY_UWB"))] = 239;
	KEY_CODES[CString(TEXT("KEY_UNKNOWN"))] = 240;
	KEY_CODES[CString(TEXT("KEY_VIDEO_NEXT"))] = 241/* drive next video source */;
	KEY_CODES[CString(TEXT("KEY_VIDEO_PREV"))] = 242/* drive previous video source */;
	KEY_CODES[CString(TEXT("KEY_BRIGHTNESS_CYCLE"))] = 243/* brightness up, after max is min */;
	KEY_CODES[CString(TEXT("KEY_BRIGHTNESS_AUTO"))] = 244/* Set Auto Brightness: manual brightness control is off, rely on ambient */;
	KEY_CODES[CString(TEXT("KEY_BRIGHTNESS_ZERO"))] = KEY_BRIGHTNESS_AUTO;
	KEY_CODES[CString(TEXT("KEY_DISPLAY_OFF"))] = 245/* display device to off state */;
	KEY_CODES[CString(TEXT("KEY_WWAN"))] = 246/* Wireless WAN (LTE, UMTS, GSM, etc.) */;
	KEY_CODES[CString(TEXT("KEY_WIMAX"))] = KEY_WWAN;
	KEY_CODES[CString(TEXT("KEY_RFKILL"))] = 247/* Key that controls all radios */;
	KEY_CODES[CString(TEXT("KEY_MICMUTE"))] = 248/* Mute / unmute the microphone */;
	KEY_CODES[CString(TEXT("KEY_OK"))] = 0x160;
	KEY_CODES[CString(TEXT("KEY_SELECT"))] = 0x161;
	KEY_CODES[CString(TEXT("KEY_GOTO"))] = 0x162;
	KEY_CODES[CString(TEXT("KEY_CLEAR"))] = 0x163;
	KEY_CODES[CString(TEXT("KEY_POWER2"))] = 0x164;
	KEY_CODES[CString(TEXT("KEY_OPTION"))] = 0x165;
	KEY_CODES[CString(TEXT("KEY_INFO"))] = 0x166/* AL OEM Features/Tips/Tutorial */;
	KEY_CODES[CString(TEXT("KEY_TIME"))] = 0x167;
	KEY_CODES[CString(TEXT("KEY_VENDOR"))] = 0x168;
	KEY_CODES[CString(TEXT("KEY_ARCHIVE"))] = 0x169;
	KEY_CODES[CString(TEXT("KEY_PROGRAM"))] = 0x16a/* Media Select Program Guide */;
	KEY_CODES[CString(TEXT("KEY_CHANNEL"))] = 0x16b;
	KEY_CODES[CString(TEXT("KEY_FAVORITES"))] = 0x16c;
	KEY_CODES[CString(TEXT("KEY_EPG"))] = 0x16d;
	KEY_CODES[CString(TEXT("KEY_PVR"))] = 0x16e/* Media Select Home */;
	KEY_CODES[CString(TEXT("KEY_MHP"))] = 0x16f;
	KEY_CODES[CString(TEXT("KEY_LANGUAGE"))] = 0x170;
	KEY_CODES[CString(TEXT("KEY_TITLE"))] = 0x171;
	KEY_CODES[CString(TEXT("KEY_SUBTITLE"))] = 0x172;
	KEY_CODES[CString(TEXT("KEY_ANGLE"))] = 0x173;
	KEY_CODES[CString(TEXT("KEY_ZOOM"))] = 0x174;
	KEY_CODES[CString(TEXT("KEY_MODE"))] = 0x175;
	KEY_CODES[CString(TEXT("KEY_KEYBOARD"))] = 0x176;
	KEY_CODES[CString(TEXT("KEY_SCREEN"))] = 0x177;
	KEY_CODES[CString(TEXT("KEY_PC"))] = 0x178/* Media Select Computer */;
	KEY_CODES[CString(TEXT("KEY_TV"))] = 0x179/* Media Select TV */;
	KEY_CODES[CString(TEXT("KEY_TV2"))] = 0x17a/* Media Select Cable */;
	KEY_CODES[CString(TEXT("KEY_VCR"))] = 0x17b/* Media Select VCR */;
	KEY_CODES[CString(TEXT("KEY_VCR2"))] = 0x17c/* VCR Plus */;
	KEY_CODES[CString(TEXT("KEY_SAT"))] = 0x17d/* Media Select Satellite */;
	KEY_CODES[CString(TEXT("KEY_SAT2"))] = 0x17e;
	KEY_CODES[CString(TEXT("KEY_CD"))] = 0x17f/* Media Select CD */;
	KEY_CODES[CString(TEXT("KEY_TAPE"))] = 0x180/* Media Select Tape */;
	KEY_CODES[CString(TEXT("KEY_RADIO"))] = 0x181;
	KEY_CODES[CString(TEXT("KEY_TUNER"))] = 0x182/* Media Select Tuner */;
	KEY_CODES[CString(TEXT("KEY_PLAYER"))] = 0x183;
	KEY_CODES[CString(TEXT("KEY_TEXT"))] = 0x184;
	KEY_CODES[CString(TEXT("KEY_DVD"))] = 0x185/* Media Select DVD */;
	KEY_CODES[CString(TEXT("KEY_AUX"))] = 0x186;
	KEY_CODES[CString(TEXT("KEY_MP3"))] = 0x187;
	KEY_CODES[CString(TEXT("KEY_AUDIO"))] = 0x188/* AL Audio Browser */;
	KEY_CODES[CString(TEXT("KEY_VIDEO"))] = 0x189/* AL Movie Browser */;
	KEY_CODES[CString(TEXT("KEY_DIRECTORY"))] = 0x18a;
	KEY_CODES[CString(TEXT("KEY_LIST"))] = 0x18b;
	KEY_CODES[CString(TEXT("KEY_MEMO"))] = 0x18c/* Media Select Messages */;
	KEY_CODES[CString(TEXT("KEY_CALENDAR"))] = 0x18d;
	KEY_CODES[CString(TEXT("KEY_RED"))] = 0x18e;
	KEY_CODES[CString(TEXT("KEY_GREEN"))] = 0x18f;
	KEY_CODES[CString(TEXT("KEY_YELLOW"))] = 0x190;
	KEY_CODES[CString(TEXT("KEY_BLUE"))] = 0x191;
	KEY_CODES[CString(TEXT("KEY_CHANNELUP"))] = 0x192/* Channel Increment */;
	KEY_CODES[CString(TEXT("KEY_CHANNELDOWN"))] = 0x193/* Channel Decrement */;
	KEY_CODES[CString(TEXT("KEY_FIRST"))] = 0x194;
	KEY_CODES[CString(TEXT("KEY_LAST"))] = 0x195/* Recall Last */;
	KEY_CODES[CString(TEXT("KEY_AB"))] = 0x196;
	KEY_CODES[CString(TEXT("KEY_NEXT"))] = 0x197;
	KEY_CODES[CString(TEXT("KEY_RESTART"))] = 0x198;
	KEY_CODES[CString(TEXT("KEY_SLOW"))] = 0x199;
	KEY_CODES[CString(TEXT("KEY_SHUFFLE"))] = 0x19a;
	KEY_CODES[CString(TEXT("KEY_BREAK"))] = 0x19b;
	KEY_CODES[CString(TEXT("KEY_PREVIOUS"))] = 0x19c;
	KEY_CODES[CString(TEXT("KEY_DIGITS"))] = 0x19d;
	KEY_CODES[CString(TEXT("KEY_TEEN"))] = 0x19e;
	KEY_CODES[CString(TEXT("KEY_TWEN"))] = 0x19f;
	KEY_CODES[CString(TEXT("KEY_VIDEOPHONE"))] = 0x1a0/* Media Select Video Phone */;
	KEY_CODES[CString(TEXT("KEY_GAMES"))] = 0x1a1/* Media Select Games */;
	KEY_CODES[CString(TEXT("KEY_ZOOMIN"))] = 0x1a2/* AC Zoom In */;
	KEY_CODES[CString(TEXT("KEY_ZOOMOUT"))] = 0x1a3/* AC Zoom Out */;
	KEY_CODES[CString(TEXT("KEY_ZOOMRESET"))] = 0x1a4/* AC Zoom */;
	KEY_CODES[CString(TEXT("KEY_WORDPROCESSOR"))] = 0x1a5/* AL Word Processor */;
	KEY_CODES[CString(TEXT("KEY_EDITOR"))] = 0x1a6/* AL Text Editor */;
	KEY_CODES[CString(TEXT("KEY_SPREADSHEET"))] = 0x1a7/* AL Spreadsheet */;
	KEY_CODES[CString(TEXT("KEY_GRAPHICSEDITOR"))] = 0x1a8/* AL Graphics Editor */;
	KEY_CODES[CString(TEXT("KEY_PRESENTATION"))] = 0x1a9/* AL Presentation App */;
	KEY_CODES[CString(TEXT("KEY_DATABASE"))] = 0x1aa/* AL Database App */;
	KEY_CODES[CString(TEXT("KEY_NEWS"))] = 0x1ab/* AL Newsreader */;
	KEY_CODES[CString(TEXT("KEY_VOICEMAIL"))] = 0x1ac/* AL Voicemail */;
	KEY_CODES[CString(TEXT("KEY_ADDRESSBOOK"))] = 0x1ad/* AL Contacts/Address Book */;
	KEY_CODES[CString(TEXT("KEY_MESSENGER"))] = 0x1ae/* AL Instant Messaging */;
	KEY_CODES[CString(TEXT("KEY_DISPLAYTOGGLE"))] = 0x1af/* Turn display (LCD) on and off */;
	KEY_CODES[CString(TEXT("KEY_BRIGHTNESS_TOGGLE"))] = KEY_DISPLAYTOGGLE;
	KEY_CODES[CString(TEXT("KEY_SPELLCHECK"))] = 0x1b0   /* AL Spell Check */;
	KEY_CODES[CString(TEXT("KEY_LOGOFF"))] = 0x1b1   /* AL Logoff */;
	KEY_CODES[CString(TEXT("KEY_DOLLAR"))] = 0x1b2;
	KEY_CODES[CString(TEXT("KEY_EURO"))] = 0x1b3;
	KEY_CODES[CString(TEXT("KEY_FRAMEBACK"))] = 0x1b4/* Consumer - transport controls */;
	KEY_CODES[CString(TEXT("KEY_FRAMEFORWARD"))] = 0x1b5;
	KEY_CODES[CString(TEXT("KEY_CONTEXT_MENU"))] = 0x1b6/* GenDesc - system context menu */;
	KEY_CODES[CString(TEXT("KEY_MEDIA_REPEAT"))] = 0x1b7/* Consumer - transport control */;
	KEY_CODES[CString(TEXT("KEY_10CHANNELSUP"))] = 0x1b8/* 10 channels up (10) */;
	KEY_CODES[CString(TEXT("KEY_10CHANNELSDOWN"))] = 0x1b9/* 10 channels down (10-) */;
	KEY_CODES[CString(TEXT("KEY_IMAGES"))] = 0x1ba/* AL Image Browser */;
	KEY_CODES[CString(TEXT("KEY_DEL_EOL"))] = 0x1c0;
	KEY_CODES[CString(TEXT("KEY_DEL_EOS"))] = 0x1c1;
	KEY_CODES[CString(TEXT("KEY_INS_LINE"))] = 0x1c2;
	KEY_CODES[CString(TEXT("KEY_DEL_LINE"))] = 0x1c3;
	KEY_CODES[CString(TEXT("KEY_FN"))] = 0x1d0;
	KEY_CODES[CString(TEXT("KEY_FN_ESC"))] = 0x1d1;
	KEY_CODES[CString(TEXT("KEY_FN_F1"))] = 0x1d2;
	KEY_CODES[CString(TEXT("KEY_FN_F2"))] = 0x1d3;
	KEY_CODES[CString(TEXT("KEY_FN_F3"))] = 0x1d4;
	KEY_CODES[CString(TEXT("KEY_FN_F4"))] = 0x1d5;
	KEY_CODES[CString(TEXT("KEY_FN_F5"))] = 0x1d6;
	KEY_CODES[CString(TEXT("KEY_FN_F6"))] = 0x1d7;
	KEY_CODES[CString(TEXT("KEY_FN_F7"))] = 0x1d8;
	KEY_CODES[CString(TEXT("KEY_FN_F8"))] = 0x1d9;
	KEY_CODES[CString(TEXT("KEY_FN_F9"))] = 0x1da;
	KEY_CODES[CString(TEXT("KEY_FN_F10"))] = 0x1db;
	KEY_CODES[CString(TEXT("KEY_FN_F11"))] = 0x1dc;
	KEY_CODES[CString(TEXT("KEY_FN_F12"))] = 0x1dd;
	KEY_CODES[CString(TEXT("KEY_FN_1"))] = 0x1de;
	KEY_CODES[CString(TEXT("KEY_FN_2"))] = 0x1df;
	KEY_CODES[CString(TEXT("KEY_FN_D"))] = 0x1e0;
	KEY_CODES[CString(TEXT("KEY_FN_E"))] = 0x1e1;
	KEY_CODES[CString(TEXT("KEY_FN_F"))] = 0x1e2;
	KEY_CODES[CString(TEXT("KEY_FN_S"))] = 0x1e3;
	KEY_CODES[CString(TEXT("KEY_FN_B"))] = 0x1e4;
	KEY_CODES[CString(TEXT("KEY_BRL_DOT1"))] = 0x1f1;
	KEY_CODES[CString(TEXT("KEY_BRL_DOT2"))] = 0x1f2;
	KEY_CODES[CString(TEXT("KEY_BRL_DOT3"))] = 0x1f3;
	KEY_CODES[CString(TEXT("KEY_BRL_DOT4"))] = 0x1f4;
	KEY_CODES[CString(TEXT("KEY_BRL_DOT5"))] = 0x1f5;
	KEY_CODES[CString(TEXT("KEY_BRL_DOT6"))] = 0x1f6;
	KEY_CODES[CString(TEXT("KEY_BRL_DOT7"))] = 0x1f7;
	KEY_CODES[CString(TEXT("KEY_BRL_DOT8"))] = 0x1f8;
	KEY_CODES[CString(TEXT("KEY_BRL_DOT9"))] = 0x1f9;
	KEY_CODES[CString(TEXT("KEY_BRL_DOT10"))] = 0x1fa;
	KEY_CODES[CString(TEXT("KEY_NUMERIC_0"))] = 0x200/* used by phones, remote controls, */;
	KEY_CODES[CString(TEXT("KEY_NUMERIC_1"))] = 0x201/* and other keypads */;
	KEY_CODES[CString(TEXT("KEY_NUMERIC_2"))] = 0x202;
	KEY_CODES[CString(TEXT("KEY_NUMERIC_3"))] = 0x203;
	KEY_CODES[CString(TEXT("KEY_NUMERIC_4"))] = 0x204;
	KEY_CODES[CString(TEXT("KEY_NUMERIC_5"))] = 0x205;
	KEY_CODES[CString(TEXT("KEY_NUMERIC_6"))] = 0x206;
	KEY_CODES[CString(TEXT("KEY_NUMERIC_7"))] = 0x207;
	KEY_CODES[CString(TEXT("KEY_NUMERIC_8"))] = 0x208;
	KEY_CODES[CString(TEXT("KEY_NUMERIC_9"))] = 0x209;
	KEY_CODES[CString(TEXT("KEY_NUMERIC_STAR"))] = 0x20a;
	KEY_CODES[CString(TEXT("KEY_NUMERIC_POUND"))] = 0x20b;
	KEY_CODES[CString(TEXT("KEY_CAMERA_FOCUS"))] = 0x210;
	KEY_CODES[CString(TEXT("KEY_WPS_BUTTON"))] = 0x211/* WiFi Protected Setup key */;
	KEY_CODES[CString(TEXT("KEY_TOUCHPAD_TOGGLE"))] = 0x212/* Request switch touchpad on or off */;
	KEY_CODES[CString(TEXT("KEY_TOUCHPAD_ON"))] = 0x213;
	KEY_CODES[CString(TEXT("KEY_TOUCHPAD_OFF"))] = 0x214;
	KEY_CODES[CString(TEXT("KEY_CAMERA_ZOOMIN"))] = 0x215;
	KEY_CODES[CString(TEXT("KEY_CAMERA_ZOOMOUT"))] = 0x216;
	KEY_CODES[CString(TEXT("KEY_CAMERA_UP"))] = 0x217;
	KEY_CODES[CString(TEXT("KEY_CAMERA_DOWN"))] = 0x218;
	KEY_CODES[CString(TEXT("KEY_CAMERA_LEFT"))] = 0x219;
	KEY_CODES[CString(TEXT("KEY_CAMERA_RIGHT"))] = 0x21a;
	KEY_CODES[CString(TEXT("KEY_ATTENDANT_ON"))] = 0x21b;
	KEY_CODES[CString(TEXT("KEY_ATTENDANT_OFF"))] = 0x21c;
	KEY_CODES[CString(TEXT("KEY_ATTENDANT_TOGGLE"))] = 0x21d/* Attendant call on or off */;
	KEY_CODES[CString(TEXT("KEY_LIGHTS_TOGGLE"))] = 0x21e/* Reading light on or off */;
	KEY_CODES[CString(TEXT("BTN_DPAD_UP"))] = 0x220;
	KEY_CODES[CString(TEXT("BTN_DPAD_DOWN"))] = 0x221;
	KEY_CODES[CString(TEXT("BTN_DPAD_LEFT"))] = 0x222;
	KEY_CODES[CString(TEXT("BTN_DPAD_RIGHT"))] = 0x223;
	KEY_CODES[CString(TEXT("KEY_ALS_TOGGLE"))] = 0x230/* Ambient light sensor */;
	KEY_CODES[CString(TEXT("KEY_BUTTONCONFIG"))] = 0x240/* AL Button Configuration */;
	KEY_CODES[CString(TEXT("KEY_TASKMANAGER"))] = 0x241/* AL Task/Project Manager */;
	KEY_CODES[CString(TEXT("KEY_JOURNAL"))] = 0x242/* AL Log/Journal/Timecard */;
	KEY_CODES[CString(TEXT("KEY_CONTROLPANEL"))] = 0x243/* AL Control Panel */;
	KEY_CODES[CString(TEXT("KEY_APPSELECT"))] = 0x244/* AL Select Task/Application */;
	KEY_CODES[CString(TEXT("KEY_SCREENSAVER"))] = 0x245/* AL Screen Saver */;
	KEY_CODES[CString(TEXT("KEY_VOICECOMMAND"))] = 0x246/* Listening Voice Command */;
	KEY_CODES[CString(TEXT("KEY_BRIGHTNESS_MIN"))] = 0x250/* Set Brightness to Minimum */;
	KEY_CODES[CString(TEXT("KEY_BRIGHTNESS_MAX"))] = 0x251/* Set Brightness to Maximum */;
	KEY_CODES[CString(TEXT("KEY_KBDINPUTASSIST_PREV"))] = 0x260;
	KEY_CODES[CString(TEXT("KEY_KBDINPUTASSIST_NEXT"))] = 0x261;
	KEY_CODES[CString(TEXT("KEY_KBDINPUTASSIST_PREVGROUP"))] = 0x262;
	KEY_CODES[CString(TEXT("KEY_KBDINPUTASSIST_NEXTGROUP"))] = 0x263;
	KEY_CODES[CString(TEXT("KEY_KBDINPUTASSIST_ACCEPT"))] = 0x264;
	KEY_CODES[CString(TEXT("KEY_KBDINPUTASSIST_CANCEL"))] = 0x265;

	BTN_CODES[CString(TEXT("BTN_MISC"))] = 0x100;
	BTN_CODES[CString(TEXT("BTN_0"))] = 0x100;
	BTN_CODES[CString(TEXT("BTN_1"))] = 0x101;
	BTN_CODES[CString(TEXT("BTN_2"))] = 0x102;
	BTN_CODES[CString(TEXT("BTN_3"))] = 0x103;
	BTN_CODES[CString(TEXT("BTN_4"))] = 0x104;
	BTN_CODES[CString(TEXT("BTN_5"))] = 0x105;
	BTN_CODES[CString(TEXT("BTN_6"))] = 0x106;
	BTN_CODES[CString(TEXT("BTN_7"))] = 0x107;
	BTN_CODES[CString(TEXT("BTN_8"))] = 0x108;
	BTN_CODES[CString(TEXT("BTN_9"))] = 0x109;
	BTN_CODES[CString(TEXT("BTN_MOUSE"))] = 0x110;
	BTN_CODES[CString(TEXT("BTN_LEFT"))] = 0x110;
	BTN_CODES[CString(TEXT("BTN_RIGHT"))] = 0x111;
	BTN_CODES[CString(TEXT("BTN_MIDDLE"))] = 0x112;
	BTN_CODES[CString(TEXT("BTN_SIDE"))] = 0x113;
	BTN_CODES[CString(TEXT("BTN_EXTRA"))] = 0x114;
	BTN_CODES[CString(TEXT("BTN_FORWARD"))] = 0x115;
	BTN_CODES[CString(TEXT("BTN_BACK"))] = 0x116;
	BTN_CODES[CString(TEXT("BTN_TASK"))] = 0x117;
	BTN_CODES[CString(TEXT("BTN_JOYSTICK"))] = 0x120;
	BTN_CODES[CString(TEXT("BTN_TRIGGER"))] = 0x120;
	BTN_CODES[CString(TEXT("BTN_THUMB"))] = 0x121;
	BTN_CODES[CString(TEXT("BTN_THUMB2"))] = 0x122;
	BTN_CODES[CString(TEXT("BTN_TOP"))] = 0x123;
	BTN_CODES[CString(TEXT("BTN_TOP2"))] = 0x124;
	BTN_CODES[CString(TEXT("BTN_PINKIE"))] = 0x125;
	BTN_CODES[CString(TEXT("BTN_BASE"))] = 0x126;
	BTN_CODES[CString(TEXT("BTN_BASE2"))] = 0x127;
	BTN_CODES[CString(TEXT("BTN_BASE3"))] = 0x128;
	BTN_CODES[CString(TEXT("BTN_BASE4"))] = 0x129;
	BTN_CODES[CString(TEXT("BTN_BASE5"))] = 0x12a;
	BTN_CODES[CString(TEXT("BTN_BASE6"))] = 0x12b;
	BTN_CODES[CString(TEXT("BTN_DEAD"))] = 0x12f;
	BTN_CODES[CString(TEXT("BTN_GAMEPAD"))] = 0x130;
	BTN_CODES[CString(TEXT("BTN_SOUTH"))] = 0x130;
	BTN_CODES[CString(TEXT("BTN_A"))] = BTN_SOUTH;
	BTN_CODES[CString(TEXT("BTN_EAST"))] = 0x131;
	BTN_CODES[CString(TEXT("BTN_B"))] = BTN_EAST;
	BTN_CODES[CString(TEXT("BTN_C"))] = 0x132;
	BTN_CODES[CString(TEXT("BTN_NORTH"))] = 0x133;
	BTN_CODES[CString(TEXT("BTN_X"))] = BTN_NORTH;
	BTN_CODES[CString(TEXT("BTN_WEST"))] = 0x134;
	BTN_CODES[CString(TEXT("BTN_Y"))] = BTN_WEST;
	BTN_CODES[CString(TEXT("BTN_Z"))] = 0x135;
	BTN_CODES[CString(TEXT("BTN_TL"))] = 0x136;
	BTN_CODES[CString(TEXT("BTN_TR"))] = 0x137;
	BTN_CODES[CString(TEXT("BTN_TL2"))] = 0x138;
	BTN_CODES[CString(TEXT("BTN_TR2"))] = 0x139;
	BTN_CODES[CString(TEXT("BTN_SELECT"))] = 0x13a;
	BTN_CODES[CString(TEXT("BTN_START"))] = 0x13b;
	BTN_CODES[CString(TEXT("BTN_MODE"))] = 0x13c;
	BTN_CODES[CString(TEXT("BTN_THUMBL"))] = 0x13d;
	BTN_CODES[CString(TEXT("BTN_THUMBR"))] = 0x13e;
	BTN_CODES[CString(TEXT("BTN_DIGI"))] = 0x140;
	BTN_CODES[CString(TEXT("BTN_TOOL_PEN"))] = 0x140;
	BTN_CODES[CString(TEXT("BTN_TOOL_RUBBER"))] = 0x141;
	BTN_CODES[CString(TEXT("BTN_TOOL_BRUSH"))] = 0x142;
	BTN_CODES[CString(TEXT("BTN_TOOL_PENCIL"))] = 0x143;
	BTN_CODES[CString(TEXT("BTN_TOOL_AIRBRUSH"))] = 0x144;
	BTN_CODES[CString(TEXT("BTN_TOOL_FINGER"))] = 0x145;
	BTN_CODES[CString(TEXT("BTN_TOOL_MOUSE"))] = 0x146;
	BTN_CODES[CString(TEXT("BTN_TOOL_LENS"))] = 0x147;
	BTN_CODES[CString(TEXT("BTN_TOOL_QUINTTAP"))] = 0x148/* Five fingers on trackpad */;
	BTN_CODES[CString(TEXT("BTN_TOUCH"))] = 0x14a;
	BTN_CODES[CString(TEXT("BTN_STYLUS"))] = 0x14b;
	BTN_CODES[CString(TEXT("BTN_STYLUS2"))] = 0x14c;
	BTN_CODES[CString(TEXT("BTN_TOOL_DOUBLETAP"))] = 0x14d;
	BTN_CODES[CString(TEXT("BTN_TOOL_TRIPLETAP"))] = 0x14e;
	BTN_CODES[CString(TEXT("BTN_TOOL_QUADTAP"))] = 0x14f/* Four fingers on trackpad */;
	BTN_CODES[CString(TEXT("BTN_WHEEL"))] = 0x150;
	BTN_CODES[CString(TEXT("BTN_GEAR_DOWN"))] = 0x150;
	BTN_CODES[CString(TEXT("BTN_GEAR_UP"))] = 0x151;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY"))] = 0x2c0;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY1"))] = 0x2c0;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY2"))] = 0x2c1;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY3"))] = 0x2c2;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY4"))] = 0x2c3;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY5"))] = 0x2c4;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY6"))] = 0x2c5;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY7"))] = 0x2c6;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY8"))] = 0x2c7;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY9"))] = 0x2c8;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY10"))] = 0x2c9;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY11"))] = 0x2ca;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY12"))] = 0x2cb;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY13"))] = 0x2cc;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY14"))] = 0x2cd;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY15"))] = 0x2ce;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY16"))] = 0x2cf;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY17"))] = 0x2d0;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY18"))] = 0x2d1;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY19"))] = 0x2d2;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY20"))] = 0x2d3;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY21"))] = 0x2d4;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY22"))] = 0x2d5;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY23"))] = 0x2d6;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY24"))] = 0x2d7;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY25"))] = 0x2d8;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY26"))] = 0x2d9;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY27"))] = 0x2da;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY28"))] = 0x2db;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY29"))] = 0x2dc;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY30"))] = 0x2dd;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY31"))] = 0x2de;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY32"))] = 0x2df;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY33"))] = 0x2e0;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY34"))] = 0x2e1;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY35"))] = 0x2e2;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY36"))] = 0x2e3;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY37"))] = 0x2e4;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY38"))] = 0x2e5;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY39"))] = 0x2e6;
	BTN_CODES[CString(TEXT("BTN_TRIGGER_HAPPY40"))] = 0x2e7;
}

__u16 CEventPair::PropToU16(CString strProp)
{
	map<CString, __u16>::const_iterator cit = DEV_PROPERTIES.find(strProp);

	if (cit != DEV_PROPERTIES.end()) 
	{
		return cit->second;
	}
	else 
	{
		return 0xFFFF;
	}
}

CString CEventPair::U16ToProp(__u16 prop)
{
	map<CString, __u16>::const_iterator cit = DEV_PROPERTIES.begin();

	for (; cit != DEV_PROPERTIES.end(); cit++)
	{
		if (cit->second == prop) {
			return cit->first;
		}
	}

	CString retError;
	retError.Format(TEXT("UNKNOWN_PROP(%04X)"), prop);
	return retError;
}


CString CEventPair::S32ToValue(__s32 val, __u16 code, __u16 type)
{
	if (type == EV_KEY)
	{
		return (val == 1) ? TEXT("DOWN") : TEXT("UP");
	}
	else if ((type == EV_ABS) && (code == ABS_MT_TOOL_TYPE))
	{
		return (code == MT_TOOL_PEN) ? TEXT("MT_TOOL_PEN") : TEXT("MT_TOOL_FINGER");
	}
	CString strValue;
	strValue.Format(TEXT("%08x"), val);
	return strValue;
}

__s32 CEventPair::ValueToS32(CString value)
{
	map<CString, __s32>::const_iterator cit = VALUES.find(value);

	if (cit != VALUES.end()) 
	{
		return cit->second;
	}
	else 
	{
		return 0xFFFFFFFF;
	}
}

CString CEventPair::U16ToType(__u16 type)
{

	map<CString, __u16>::const_iterator cit = TYPES.begin();

	for (; cit != TYPES.end(); cit++)
	{
		if (cit->second == type) {
			return cit->first;
		}
	}

	CString retError;
	retError.Format(TEXT("UNKNOWN_TYPE(%04X)"), type);
	return retError;
}

__u16 CEventPair::TypeToU16(CString strType)
{
	map<CString, __u16>::const_iterator cit = TYPES.find(strType);

	if (cit == TYPES.end()) {
		return 0xFFFF;
	}
	else {
		return cit->second;
	}
}

CString CEventPair::U16ToCode(__u16 code, __u16 type)
{
	map<CString, __u16>::const_iterator cit;
	if (type == EV_KEY) 
	{
		cit = KEY_CODES.begin();
		for (; cit != KEY_CODES.end(); cit++) 
		{
			if (cit->second == code) 
			{
				return cit->first;
			}
		}

		cit = BTN_CODES.begin();
		for (; cit != BTN_CODES.end(); cit++) 
		{
			if (cit->second == code) 
			{
				return cit->first;
			}
		}
	}
	else if (type == EV_ABS) 
	{
		cit = ABS_CODES.begin();
		for (; cit != ABS_CODES.end(); cit++) 
		{
			if (cit->second == code)
			{
				return cit->first;
			}
		}
	}
	else if (type == EV_SYN)
	{
		cit = SYN_CODES.begin();
		for (; cit != SYN_CODES.end(); cit++) 
		{
			if (cit->second == code)
			{
				return cit->first;
			}
		}
	}

	CString retError;
	retError.Format(TEXT("UNKNOWN_CODE(%04X)"), code);
	return retError;
}

CString CEventPair::U16ToKey(__u16 keycode)
{
	map<CString, __u16>::const_iterator cit = KEY_CODES.begin();

	for (; cit != KEY_CODES.end(); cit++)
	{
		if (cit->second == keycode) {
			return cit->first;
		}
	}

	cit = BTN_CODES.begin();
	for (; cit != BTN_CODES.end(); cit++)
	{
		if (cit->second == keycode) {
			return cit->first;
		}
	}

	CString retError;
	retError.Format(TEXT("UNKNOWN_KEY(%04X)"), keycode);
	return retError;
}

__u16 CEventPair::CodeToU16(CString strCode)
{
	map<CString, __u16>::const_iterator cit = ABS_CODES.find(strCode);

	if (cit != ABS_CODES.end()) {
		return cit->second;
	}

	cit = KEY_CODES.find(strCode);

	if (cit != KEY_CODES.end()) {
		return cit->second;
	}

	cit = BTN_CODES.find(strCode);

	if (cit != BTN_CODES.end()) {
		return cit->second;
	}


	cit = SYN_CODES.find(strCode);

	if (cit != SYN_CODES.end()) {
		return cit->second;
	}

	return 0xFFFF;
}


