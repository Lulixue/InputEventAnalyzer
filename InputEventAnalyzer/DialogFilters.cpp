// DialogFilters.cpp : 实现文件
//

#include "stdafx.h"
#include "InputEventAnalyzer.h"
#include "DialogFilters.h"
#include "afxdialogex.h"


UINT CHECK_IDS[] = {
		IDC_CHECK_P0, 	IDC_CHECK_P1, 	IDC_CHECK_P2, 	
		IDC_CHECK_P3, 	IDC_CHECK_P4, 	
		IDC_CHECK_P5, 	IDC_CHECK_P6, 	IDC_CHECK_P7, 	
		IDC_CHECK_P8, 	IDC_CHECK_P9, 
};
// CDialogFilters 对话框

IMPLEMENT_DYNAMIC(CDialogFilters, CDialogEx)

CDialogFilters::CDialogFilters(INPUT_FILTER_T *pift, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogFilters::IDD, pParent),
	m_pIft(pift)
{

}

CDialogFilters::~CDialogFilters()
{
}

void CDialogFilters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogFilters, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_P0, &CDialogFilters::OnBnClickedCheckP0)
	ON_BN_CLICKED(IDC_CHECK_P1, &CDialogFilters::OnBnClickedCheckP1)
	ON_BN_CLICKED(IDC_CHECK_P2, &CDialogFilters::OnBnClickedCheckP2)
	ON_BN_CLICKED(IDC_CHECK_P3, &CDialogFilters::OnBnClickedCheckP3)
	ON_BN_CLICKED(IDC_CHECK_P4, &CDialogFilters::OnBnClickedCheckP4)
	ON_BN_CLICKED(IDC_CHECK_P5, &CDialogFilters::OnBnClickedCheckP5)
	ON_BN_CLICKED(IDC_CHECK_P6, &CDialogFilters::OnBnClickedCheckP6)
	ON_BN_CLICKED(IDC_CHECK_P7, &CDialogFilters::OnBnClickedCheckP7)
	ON_BN_CLICKED(IDC_CHECK_P8, &CDialogFilters::OnBnClickedCheckP8)
	ON_BN_CLICKED(IDC_CHECK_P9, &CDialogFilters::OnBnClickedCheckP9)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_RESET, &CDialogFilters::OnBnClickedBtnReset)
END_MESSAGE_MAP()


// CDialogFilters 消息处理程序

BOOL CDialogFilters::GetCheckStatus(UINT id) 
{
	CButton *pBtn = (CButton *) (GetDlgItem(id));
	return pBtn->GetCheck() == BST_CHECKED;
}

void CDialogFilters::SetCheckStatus(UINT id, BOOL bCheck)
{
	CButton *pBtn = (CButton *) (GetDlgItem(id));
	pBtn->SetCheck(bCheck);
}

void CDialogFilters::OnBnClickedCheckP0()
{
	m_pIft->SetPoint(0, GetCheckStatus(CHECK_IDS[0]));
}


void CDialogFilters::OnBnClickedCheckP1()
{
	m_pIft->SetPoint(1, GetCheckStatus(CHECK_IDS[1]));
}


void CDialogFilters::OnBnClickedCheckP2()
{
	m_pIft->SetPoint(2, GetCheckStatus(CHECK_IDS[2]));
}


void CDialogFilters::OnBnClickedCheckP3()
{
	m_pIft->SetPoint(3, GetCheckStatus(CHECK_IDS[3]));
}


void CDialogFilters::OnBnClickedCheckP4()
{
	m_pIft->SetPoint(4, GetCheckStatus(CHECK_IDS[4]));
}


void CDialogFilters::OnBnClickedCheckP5()
{
	m_pIft->SetPoint(5, GetCheckStatus(CHECK_IDS[5]));
}


void CDialogFilters::OnBnClickedCheckP6()
{
	m_pIft->SetPoint(6, GetCheckStatus(CHECK_IDS[6]));
}


void CDialogFilters::OnBnClickedCheckP7()
{
	m_pIft->SetPoint(7, GetCheckStatus(CHECK_IDS[7]));
}


void CDialogFilters::OnBnClickedCheckP8()
{
	m_pIft->SetPoint(8, GetCheckStatus(CHECK_IDS[8]));
}


void CDialogFilters::OnBnClickedCheckP9()
{
	m_pIft->SetPoint(9, GetCheckStatus(CHECK_IDS[9]));
}


void CDialogFilters::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	for (int i = 0; i < 10; i++) {
		SetCheckStatus(CHECK_IDS[i], !m_pIft->IsFilterPnt(i));
	}
}


void CDialogFilters::OnBnClickedBtnReset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pIft->Init();
	for (int i = 0; i < 10; i++) {
		SetCheckStatus(CHECK_IDS[i], !m_pIft->IsFilterPnt(i));
	}
}
