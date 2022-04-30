
// AIAssistDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "AIAssist.h"
#include "AIAssistDlg.h"
#include "afxdialogex.h"

#include "AssistConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAIAssistDlg 对话框



CAIAssistDlg::CAIAssistDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AIASSIST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_AssistConfig = AssistConfig::GetInstance();
	m_AssistWorker = new AssistWorker();
}

void CAIAssistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PROCESSNAME, m_processName);
	DDX_Control(pDX, IDC_CHECK_IMGDETECT, m_imgDetect);
	DDX_Control(pDX, IDC_CHECK_AUTOTRACE, m_autoTrace);
	DDX_Control(pDX, IDC_CHECK_AUTOFIRE, m_autoFire);
	DDX_Control(pDX, IDC_CHECK_AUTOPRESS, m_autoPress);
	DDX_Control(pDX, IDC_STATIC_IMG, m_img);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_status);
	DDX_Control(pDX, IDC_EDIT_DETECTWIDTH, m_detectWidth);
	DDX_Control(pDX, IDC_EDIT_DETECTHEIGHT, m_detectHeight);
	DDX_Control(pDX, IDC_RADIO_TRACETYPE, m_traceType);
	DDX_Control(pDX, IDC_RADIO_FIRETYPE, m_fireType);
	DDX_Control(pDX, IDC_EDIT_FIRETIME, m_fireTime);

	//数据动态绑定
	DDX_Text(pDX, IDC_EDIT_DETECTWIDTH, m_AssistConfig->detectWidth);
	DDX_Text(pDX, IDC_EDIT_DETECTHEIGHT, m_AssistConfig->detectHeight);
	//DDX_Check(pDX, IDC_CHECK_IMGDETECT, m_AssistConfig->detectImg);
}

BEGIN_MESSAGE_MAP(CAIAssistDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CAIAssistDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_FINDPROCESS, &CAIAssistDlg::OnBnClickedButtonFindprocess)
	ON_BN_CLICKED(IDC_CHECK_IMGDETECT, &CAIAssistDlg::OnBnClickedCheckImgdetect)
	ON_BN_CLICKED(IDC_CHECK_AUTOTRACE, &CAIAssistDlg::OnBnClickedCheckAutotrace)
	ON_BN_CLICKED(IDC_CHECK_AUTOFIRE, &CAIAssistDlg::OnBnClickedCheckAutofire)
	ON_BN_CLICKED(IDC_CHECK_AUTOPRESS, &CAIAssistDlg::OnBnClickedCheckAutopress)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAIAssistDlg 消息处理程序

BOOL CAIAssistDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//初始化控件值
	CString gameName = CommUtil::StringToCString(m_AssistConfig->gameName);
	m_processName.SetWindowTextW(gameName);

	//m_processName.ResetContent();
	for (int i = 0; i < m_AssistConfig->gameLength;i++ ) {
		string itemName = m_AssistConfig->gameNames[i];
		CString itemCName = CommUtil::StringToCString(itemName);
		m_processName.AddString(itemCName);
	}

	m_imgDetect.SetCheck(m_AssistConfig->detectImg);
	m_autoTrace.SetCheck(m_AssistConfig->autoTrace);
	m_autoFire.SetCheck(m_AssistConfig->autoFire);
	m_autoPress.SetCheck(m_AssistConfig->autoPush);

	CString detectWidth, detectHeight;
	detectWidth.Format(_T("%d"), m_AssistConfig->detectWidth);
	detectHeight.Format(_T("%d"), m_AssistConfig->detectHeight);
	m_detectWidth.SetWindowTextW(detectWidth);
	m_detectHeight.SetWindowTextW(detectHeight);

	//启动绘图线程
	DRAWTHREADPARAM * pParam;
	pParam = new DRAWTHREADPARAM{ IDC_STATIC_IMG,this->GetSafeHwnd(),this->m_AssistWorker, this->m_AssistConfig};
	m_DrawThread = AfxBeginThread(DrawThreadProc, pParam);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAIAssistDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAIAssistDlg::OnPaint()
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
HCURSOR CAIAssistDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAIAssistDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CAIAssistDlg::OnBnClickedButtonFindprocess()
{
	// TODO: 在此添加控件通知处理程序代码
	//查找游戏进程，获取游戏屏幕
	CString name;
	m_processName.GetWindowText(name);

	if(!name.IsEmpty()){
		string nameStr = CommUtil::CStringToString(name);
		m_AssistConfig->changGameName(nameStr);

		//刷新动态数据绑定
		UpdateData();

		//查找进程窗口，重新初始化AssistWorker
		SystemUtil::findProcessWindowRect();
		m_AssistWorker->ReInit();

		//更新状态信息
		string stat = AssistState::getStatInf();
		m_status.SetWindowText(CommUtil::StringToCString(stat));
	}
}


void CAIAssistDlg::OnBnClickedCheckImgdetect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_imgDetect.GetCheck() == 1) {
		m_AssistConfig->detectImg = true;
	}
	else {
		m_AssistConfig->detectImg = false;
	}
	//更新状态信息
	string stat = AssistState::getStatInf();
	m_status.SetWindowText(CommUtil::StringToCString(stat));
}


void CAIAssistDlg::OnBnClickedCheckAutotrace()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_autoTrace.GetCheck() == 1) {
		m_AssistConfig->autoTrace = true;
	}
	else {
		m_AssistConfig->autoTrace = false;
	}
	//更新状态信息
	string stat = AssistState::getStatInf();
	m_status.SetWindowText(CommUtil::StringToCString(stat));
}


void CAIAssistDlg::OnBnClickedCheckAutofire()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_autoFire.GetCheck() == 1) {
		m_AssistConfig->autoFire = true;
	}
	else {
		m_AssistConfig->autoFire = false;
	}
	//更新状态信息
	string stat = AssistState::getStatInf();
	m_status.SetWindowText(CommUtil::StringToCString(stat));
}


void CAIAssistDlg::OnBnClickedCheckAutopress()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_autoPress.GetCheck() == 1) {
		m_AssistConfig->autoPush = true;
	}
	else {
		m_AssistConfig->autoPush = false;
	}
	//更新状态信息
	string stat = AssistState::getStatInf();
	m_status.SetWindowText(CommUtil::StringToCString(stat));
}



BOOL CAIAssistDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//return CDialogEx::PreTranslateMessage(pMsg);

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	else if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}
	else if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_TAB))
	{
		return TRUE;
	}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAIAssistDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	switch (nType)
	{
	case SIZE_RESTORED:
		CommUtil::winSizeType = SIZE_RESTORED;
		break;
	case SIZE_MINIMIZED:
		CommUtil::winSizeType = SIZE_MINIMIZED;
		break;
	case SIZE_MAXIMIZED:
		CommUtil::winSizeType = SIZE_MAXIMIZED;
		break;
	}
}
