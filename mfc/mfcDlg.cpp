
// mfcDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "config.h"
#include "helper.h"
#include "mfc.h"
#include "mfcDlg.h"
#include "afxdialogex.h"
#include "dm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>
using namespace std;

// CmfcDlg 对话框

game::Config game::config;
game::ProcessConfig game::processConfig;

CEdit* game::logEdit;
dmsoft* g_dm;

CmfcDlg::CmfcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CmfcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CmfcDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK3, &CmfcDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK9, &CmfcDlg::OnBnClickedCheck9)
	ON_EN_CHANGE(IDC_EDIT1, &CmfcDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CHECK1, &CmfcDlg::OnBnClickedCheck1)
	ON_LBN_SELCHANGE(IDC_LIST3, &CmfcDlg::OnLbnSelchangeList3)
	ON_BN_CLICKED(IDC_CHECK2, &CmfcDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK4, &CmfcDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CmfcDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CmfcDlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &CmfcDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK8, &CmfcDlg::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK10, &CmfcDlg::OnBnClickedCheck10)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST4, &CmfcDlg::OnLvnItemchangedList4)
END_MESSAGE_MAP()


// CmfcDlg 消息处理程序

BOOL CmfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CListBox* modelList = (CListBox*)GetDlgItem(IDC_LIST3);
	// modelList->InsertColumn(0, _T("自动模式"), 0, 100);

	modelList->AddString(L"搬砖模式");
	modelList->AddString(L"剧情模式");
	modelList->AddString(L"未央模式");
	// 可选择默认值
	modelList->SelectString(0, L"搬砖模式");

	CListCtrl* roleList = (CListCtrl*)GetDlgItem(IDC_LIST4);
	roleList->InsertColumn(0, _T("序号"), LVCFMT_CENTER, 50);
	roleList->InsertColumn(1, _T("角色"), LVCFMT_LEFT, 120);
	roleList->InsertColumn(2, _T("名望"), LVCFMT_LEFT, 100);
	roleList->InsertColumn(3, _T("金币"), LVCFMT_LEFT, 100);


	game::logEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	/**/
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmfcDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 初始化按钮
void CmfcDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	game::processConfig.currentPid = GetCurrentProcessId();
	// 获取当前进程路径

	// 大漠读写初始化 保护当前进程
	if (!game::processConfig.dmInit) {
		// TODO
		g_dm = new dmsoft();

		game::CmfcLog(_T("大漠读写初始化成功~"));
		game::processConfig.dmInit = true;
	}
	// 游戏进程初始化
	DWORD gamePid;
	if (!game::FindProcessPid(L"dnf.exe", gamePid)) {
		// MessageBox(_T("游戏未启动"));
		game::CmfcLog(_T("游戏未启动"));
		return;
	}
	game::processConfig.gamePid = gamePid;




	// 热键添加


	
}


void CmfcDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* cbt = (CButton*)GetDlgItem(IDC_CHECK3);
	game::config.pureInvincible = cbt->GetCheck();
}


void CmfcDlg::OnBnClickedCheck9()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* cbt = (CButton*)GetDlgItem(IDC_CHECK9);
	game::config.floatCooldown = cbt->GetCheck();
}


void CmfcDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CmfcDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* cbt = (CButton*)GetDlgItem(IDC_CHECK1);
	game::config.floatHarm = cbt->GetCheck();
	// MessageBox(to_wstring(state).c_str());
}

// 自动模式更改
void CmfcDlg::OnLbnSelchangeList3()
{
	// TODO: 在此添加控件通知处理程序代码
	// CString selectText;
	CListBox* modelList = (CListBox*)GetDlgItem(IDC_LIST3);
	int cusr = modelList->GetCurSel();
	game::config.autoModel = cusr;


}


void CmfcDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CButton* cbt = (CButton*)GetDlgItem(IDC_CHECK2);
	game::config.hookHarm = cbt->GetCheck();
}


void CmfcDlg::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* cbt = (CButton*)GetDlgItem(IDC_CHECK4);
	game::config.btInvincible = cbt->GetCheck();
}


void CmfcDlg::OnBnClickedCheck5()
{
	CButton* cbt = (CButton*)GetDlgItem(IDC_CHECK5);
	game::config.speed = cbt->GetCheck();
}


void CmfcDlg::OnBnClickedCheck6()
{
	CButton* cbt = (CButton*)GetDlgItem(IDC_CHECK6);
	game::config.firePerson = cbt->GetCheck();

}


void CmfcDlg::OnBnClickedCheck7()
{
	CButton* cbt = (CButton*)GetDlgItem(IDC_CHECK7);
	game::config.autoPick = cbt->GetCheck();
}


void CmfcDlg::OnBnClickedCheck8()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* cbt = (CButton*)GetDlgItem(IDC_CHECK8);
	game::config.fullScreenHook = cbt->GetCheck();
}


void CmfcDlg::OnBnClickedCheck10()
{
	CButton* cbt = (CButton*)GetDlgItem(IDC_CHECK10);
	game::config.skillNoCd = cbt->GetCheck();
}


void CmfcDlg::OnLvnItemchangedList4(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

