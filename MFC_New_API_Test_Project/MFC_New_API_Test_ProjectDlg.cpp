
// MFC_New_API_Test_ProjectDlg.cpp: 구현 파일
//

#include "pch.h"

#include "MFC_New_API_Test_ProjectDlg.h"

#include "afxdialogex.h"
#include "EnumToString.h"
#include "EtherCAT_manager.h"
#include "framework.h"
#include "MFC_New_API_Test_Project.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern MXP_EasyClass* MXP;			//Declaration of extern MXP Object which is in EthrtCAT_manager.cpp
extern ECATManager* EManager;		// Declaration of extern ECAT Manager Object which is in EthrtCAT_manager.cpp
extern DeviceManager* DManager;		// Declaration of extern Device Manager Object which is in EthrtCAT_manager.cpp

//////////////////////////////////////////////////////////////////////////
//								MFC										//
//////////////////////////////////////////////////////////////////////////
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CMFCNewAPITestProjectDlg 대화 상자


CMFCNewAPITestProjectDlg::CMFCNewAPITestProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_NEW_API_TEST_PROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

CMFCNewAPITestProjectDlg::~CMFCNewAPITestProjectDlg()
{
	//thLED.join();
	delete MXP;
	delete EManager;
	delete DManager;
}

void CMFCNewAPITestProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCNewAPITestProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Init, &CMFCNewAPITestProjectDlg::OnBnClickedInit)
	ON_BN_CLICKED(IDC_RUN, &CMFCNewAPITestProjectDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_STOP, &CMFCNewAPITestProjectDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_DESTROY, &CMFCNewAPITestProjectDlg::OnBnClickedDestroy)
	ON_BN_CLICKED(IDC_SERVO_ON, &CMFCNewAPITestProjectDlg::OnBnClickedServoOn)
	ON_BN_CLICKED(IDC_SERVO_OFF, &CMFCNewAPITestProjectDlg::OnBnClickedServoOff)
	ON_EN_CHANGE(IDC_SLAVE_COUNT, &CMFCNewAPITestProjectDlg::OnEnChangeSlaveCount)
	ON_EN_CHANGE(IDC_AXIS_COUNT, &CMFCNewAPITestProjectDlg::OnEnChangeAxisCount)
	ON_BN_CLICKED(IDC_M0_HOME, &CMFCNewAPITestProjectDlg::OnBnClickedM0Home)
	ON_EN_CHANGE(IDC_M0_TARGET_POS, &CMFCNewAPITestProjectDlg::OnEnChangeM0TargetPos)
	ON_BN_CLICKED(IDC_M0_MOVE, &CMFCNewAPITestProjectDlg::OnBnClickedM0Move)
	ON_BN_CLICKED(IDC_M0_STOP, &CMFCNewAPITestProjectDlg::OnBnClickedM0Stop)
	ON_EN_CHANGE(IDC_DP_POS, &CMFCNewAPITestProjectDlg::OnEnChangeDpPos)
	ON_EN_CHANGE(IDC_DP_VEL, &CMFCNewAPITestProjectDlg::OnEnChangeDpVel)
	ON_EN_CHANGE(IDC_DP_TOR, &CMFCNewAPITestProjectDlg::OnEnChangeDpTor)
	ON_EN_CHANGE(IDC_M0_STATUS, &CMFCNewAPITestProjectDlg::OnEnChangeM0Status)
	ON_EN_CHANGE(IDC_M0_ERROR, &CMFCNewAPITestProjectDlg::OnEnChangeM0Error)
	ON_BN_CLICKED(IDC_M0_RESET, &CMFCNewAPITestProjectDlg::OnBnClickedM0Reset)
	ON_EN_CHANGE(IDC_ECAT_STATUS, &CMFCNewAPITestProjectDlg::OnEnChangeEcatStatus)
	ON_EN_CHANGE(IDC_KERNEL_STATUS, &CMFCNewAPITestProjectDlg::OnEnChangeKernelStatus)
	ON_EN_CHANGE(IDC_NETWORK_STATUS, &CMFCNewAPITestProjectDlg::OnEnChangeNetworkStatus)
	ON_BN_CLICKED(IDC_REFRESH, &CMFCNewAPITestProjectDlg::OnBnClickedRefresh)
	ON_BN_CLICKED(IDC_DO_ON, &CMFCNewAPITestProjectDlg::OnBnClickedDoOn)
	ON_BN_CLICKED(IDC_DO_OFF, &CMFCNewAPITestProjectDlg::OnBnClickedDoOff)
	ON_EN_CHANGE(IDC_DI, &CMFCNewAPITestProjectDlg::OnEnChangeDi)
	ON_EN_CHANGE(IDC_SDO_WRITE_VALUE, &CMFCNewAPITestProjectDlg::OnEnChangeSdoWriteValue)
	ON_BN_CLICKED(IDC_SDO_WRITE, &CMFCNewAPITestProjectDlg::OnBnClickedSdoWrite)
	ON_EN_CHANGE(IDC_SDO_READ_VALUE, &CMFCNewAPITestProjectDlg::OnEnChangeSdoReadValue)
	ON_BN_CLICKED(IDC_SDO_READ, &CMFCNewAPITestProjectDlg::OnBnClickedSdoRead)
	ON_EN_CHANGE(IDC_AI_READ, &CMFCNewAPITestProjectDlg::OnEnChangeAiRead)
	ON_BN_CLICKED(IDC_AI_LASER_READ, &CMFCNewAPITestProjectDlg::OnBnClickedAiLaserRead)
	ON_EN_CHANGE(IDC_LOAD_READ, &CMFCNewAPITestProjectDlg::OnEnChangeLoadRead)
	ON_BN_CLICKED(IDC_LOAD_UPPER_READ, &CMFCNewAPITestProjectDlg::OnBnClickedLoadUpperRead)
	ON_BN_CLICKED(IDC_LED_R, &CMFCNewAPITestProjectDlg::OnBnClickedLedR)
	ON_BN_CLICKED(IDC_LED_G, &CMFCNewAPITestProjectDlg::OnBnClickedLedG)
	ON_BN_CLICKED(IDC_LED_B, &CMFCNewAPITestProjectDlg::OnBnClickedLedB)
	ON_EN_CHANGE(IDC_FUNC_STATUS, &CMFCNewAPITestProjectDlg::OnEnChangeFuncStatus)
	ON_BN_CLICKED(IDC_TURN_LED, &CMFCNewAPITestProjectDlg::OnBnClickedTurnLed)
	ON_BN_CLICKED(IDC_INITSYSTEM, &CMFCNewAPITestProjectDlg::OnBnClickedInitsystem)
	ON_EN_CHANGE(IDC_LOAD_CELL_STATUS, &CMFCNewAPITestProjectDlg::OnEnChangeLoadCellStatus)
	ON_BN_CLICKED(IDC_PERIODIC_TEST, &CMFCNewAPITestProjectDlg::OnBnClickedPeriodicTest)
	ON_BN_CLICKED(IDC_LOOP_CONTROLLER, &CMFCNewAPITestProjectDlg::OnBnClickedLoopController)
END_MESSAGE_MAP()


// CMFCNewAPITestProjectDlg 메시지 처리기

BOOL CMFCNewAPITestProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCNewAPITestProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCNewAPITestProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCNewAPITestProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////
//								MFC										//
//////////////////////////////////////////////////////////////////////////

void CMFCNewAPITestProjectDlg::OnBnClickedInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Str;

	EManager->InitSys();
	EManager->CountSlave();	// Count num of all slave
	EManager->CountAxis();	// Count num of all axis

	Str.Format(_T("%d"), EManager->num_of_slave_);
	SetDlgItemText(IDC_SLAVE_COUNT, Str);			// Display num of all slave

	Str.Format(_T("%d"), EManager->num_of_axis_);
	SetDlgItemText(IDC_AXIS_COUNT, Str);			// Display num of all axis

	EManager->ReadMasterLineStatus();
	EManager->ReadMasterKernelStatus();
	EManager->ReadMasterNetworkStatus();

	Str.Format(_T("%d"), EManager->GetMasterLineStatus());
	SetDlgItemText(IDC_ECAT_STATUS, Str);			// Display master's line connection status

	Str.Format(_T("%d"), EManager->GetMasterKernelStatus());
	SetDlgItemText(IDC_KERNEL_STATUS, Str);			// Display master's kernel status

	Str.Format(_T("%d"), EManager->GetMasterNetworkStatus());
	SetDlgItemText(IDC_NETWORK_STATUS, Str);		// Display master's network status
}

void CMFCNewAPITestProjectDlg::OnBnClickedRun()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	EManager->RunMXP();
}

void CMFCNewAPITestProjectDlg::OnBnClickedStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	EManager->StopMXP();
}

void CMFCNewAPITestProjectDlg::OnBnClickedDestroy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	EManager->DestroySys();

	
}

void CMFCNewAPITestProjectDlg::OnBnClickedServoOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < EManager->num_of_axis_; i++)
	{
		DManager->TurnServoOnOff(i, TRUE);
	}
}

void CMFCNewAPITestProjectDlg::OnBnClickedServoOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < EManager->num_of_axis_; i++)
	{
		DManager->TurnServoOnOff(i, FALSE);
	}
}

void CMFCNewAPITestProjectDlg::OnBnClickedM0Home()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	DManager->HomeAxis(1);
}

void CMFCNewAPITestProjectDlg::OnBnClickedM0Move()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CWnd* PWnd;
	CString Str;
	FLOAT target_pos;
	FLOAT target_vel = 15;
	FLOAT cur_pos;
	FLOAT cur_vel;
	FLOAT cur_tor;
	FUNC_STATUS f_state;
	AXIS_STATE cur_state;
	AXIS_ERROR cur_error;

	PWnd = GetDlgItem(IDC_M0_TARGET_POS);
	PWnd->GetWindowTextW(Str);
	target_pos = (FLOAT)_ttoi(Str);

	DManager->MoveAxis(A, target_pos, target_vel);
	DManager->ReadAxisStatus();
	DManager->ReadAxisError();
	cur_pos = DManager->GetAxisPos(0);
	cur_vel = DManager->GetAxisVel(0);
	cur_tor = DManager->GetAxisTor(0);

	Str.Format(_T("%d"), (INT32)cur_pos);
	SetDlgItemText(IDC_DP_POS, Str);
	Str.Format(_T("%d"), (INT32)cur_vel);
	SetDlgItemText(IDC_DP_VEL, Str);
	Str.Format(_T("%d"), (INT32)cur_tor);
	SetDlgItemText(IDC_DP_TOR, Str);

	f_state = EManager->GetECATFuncStatus();
	cur_state = DManager->GetAxisStatus(0);
	cur_error = DManager->GetAxisError(0);

	Str.Format(_T("%d"), f_state);
	SetDlgItemText(IDC_FUNC_STATUS, Str);
	Str.Format(_T("%d"), cur_state.Standstill);
	SetDlgItemText(IDC_M0_STATUS, Str);
	Str.Format(_T("%d"), cur_error.MXPError);
	SetDlgItemText(IDC_M0_ERROR, Str);
	
}

void CMFCNewAPITestProjectDlg::OnBnClickedPeriodicTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CWnd* PWnd;
	CString Str;
	FLOAT cur_pos;
	FLOAT cur_vel;
	FLOAT cur_tor;
	FLOAT target_pos;
	FLOAT target_vel = 6;

	bool bcontrol = false;

	std::chrono::system_clock::time_point start_test = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	std::chrono::duration<double> period = std::chrono::duration<double, milli>(1000);

	while (bloop)
	{
		end = std::chrono::system_clock::now();
		if (std::chrono::duration<double>(end - start) < period)						// 1000ms loop
			bcontrol = false;
		else
			bcontrol = true;
		
		PWnd = GetDlgItem(IDC_M0_TARGET_POS);
		PWnd->GetWindowTextW(Str);
		target_pos = (FLOAT)_ttoi(Str);

		DManager->MoveAxis(A, target_pos, target_vel);

		if (bcontrol)
		{

			cur_pos = DManager->GetAxisPos(0);
			cur_vel = DManager->GetAxisVel(0);
			cur_tor = DManager->GetAxisTor(0);

			Str.Format(_T("%d"), (INT32)cur_pos);
			SetDlgItemText(IDC_DP_POS, Str);
			Str.Format(_T("%d"), (INT32)cur_vel);
			SetDlgItemText(IDC_DP_VEL, Str);
			Str.Format(_T("%d"), (INT32)cur_tor);
			SetDlgItemText(IDC_DP_TOR, Str);

			start = end;
		}
		if (std::chrono::duration<double>(start_test - end)  > std::chrono::duration<double, milli>(5000))
			bloop = false;
	}

}

void CMFCNewAPITestProjectDlg::OnBnClickedM0Stop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	DManager->StopAxis(1);
}

void CMFCNewAPITestProjectDlg::OnBnClickedM0Reset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	DManager->ResetAxis(1);
}

void CMFCNewAPITestProjectDlg::OnBnClickedRefresh()
{
	CString Str;

	EManager->CountSlave();	// Count num of all slave
	EManager->CountAxis();	// Count num of all axis
	EManager->ReadMasterLineStatus();				
	EManager->ReadMasterKernelStatus();
	EManager->ReadMasterNetworkStatus();

	Str.Format(_T("%d"), EManager->num_of_slave_);
	SetDlgItemText(IDC_SLAVE_COUNT, Str);			// Display num of all slave

	Str.Format(_T("%d"), EManager->num_of_axis_);
	SetDlgItemText(IDC_AXIS_COUNT, Str);			// Display num of all axis

	Str.Format(_T("%d"), EManager->GetMasterLineStatus());
	SetDlgItemText(IDC_ECAT_STATUS, Str);			// Display master's line connection status

	Str.Format(_T("%d"), EManager->GetMasterKernelStatus());
	SetDlgItemText(IDC_KERNEL_STATUS, Str);			// Display master's kernel status

	Str.Format(_T("%d"), EManager->GetMasterNetworkStatus());
	SetDlgItemText(IDC_NETWORK_STATUS, Str);		// Display master's network status

	Str.Format(_T("%d"), DManager->IsEmerButtonOn());
	SetDlgItemText(IDC_DI, Str);					// Display DI Emergency button : FALSE(0) in emergency situation 
}

void CMFCNewAPITestProjectDlg::OnBnClickedDoOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	DManager->TurnIRLaserOnOff(ON);			// Turn on IR Laser													
}

void CMFCNewAPITestProjectDlg::OnBnClickedDoOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	DManager->TurnIRLaserOnOff(OFF);			// Turn off IR Laser
}

void CMFCNewAPITestProjectDlg::OnBnClickedSdoWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CWnd* PWnd;
	CString Str;
	UINT32 value;

	PWnd = GetDlgItem(IDC_SDO_WRITE_VALUE);
	PWnd->GetWindowTextW(Str);
	value = (FLOAT)_ttoi(Str);

	EManager->WriteSDOData(0, 8448, 0, 2, value);
}

void CMFCNewAPITestProjectDlg::OnBnClickedSdoRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Str;
	
	UINT32 value = EManager->ReadSDOData(0, 8448, 0, 2);

	Str.Format(_T("%d"), value);
	SetDlgItemText(IDC_SDO_READ_VALUE, Str);					// Display COE data
}

void CMFCNewAPITestProjectDlg::OnBnClickedAiLaserRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Str;

	FLOAT value = DManager->GetLaserDistance();

	Str.Format(_T("%f"), value);
	SetDlgItemText(IDC_AI_READ, Str);					// Display Laser distance data

}

void CMFCNewAPITestProjectDlg::OnBnClickedLoadUpperRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString Str;

	FLOAT value = DManager->GetUpperArmLoad();

	Str.Format(_T("%f"), value);
	SetDlgItemText(IDC_LOAD_READ, Str);					// Display Laser distance data
}

void CMFCNewAPITestProjectDlg::OnBnClickedLedR()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DManager->TurnLEDRedOnOff(ON);
}

void CMFCNewAPITestProjectDlg::OnBnClickedLedG()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DManager->TurnLEDGreenOnOff(ON);
}

void CMFCNewAPITestProjectDlg::OnBnClickedLedB()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DManager->TurnLEDBlueOnOff(ON);
}

void CMFCNewAPITestProjectDlg::OnBnClickedTurnLed()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	DManager->CreateThreadForBlinkLED(BLUE, 0.5, 5);
}

void CMFCNewAPITestProjectDlg::OnBnClickedInitsystem()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EManager->InitSystem();
}

void CMFCNewAPITestProjectDlg::OnEnChangeM0TargetPos() {}
void CMFCNewAPITestProjectDlg::OnEnChangeSlaveCount() {}
void CMFCNewAPITestProjectDlg::OnEnChangeAxisCount() {}
void CMFCNewAPITestProjectDlg::OnEnChangeDpPos() {}
void CMFCNewAPITestProjectDlg::OnEnChangeDpVel() {}
void CMFCNewAPITestProjectDlg::OnEnChangeDpTor() {}
void CMFCNewAPITestProjectDlg::OnEnChangeM0Status() {}
void CMFCNewAPITestProjectDlg::OnEnChangeM0Error() {}
void CMFCNewAPITestProjectDlg::OnEnChangeEcatStatus() {}
void CMFCNewAPITestProjectDlg::OnEnChangeKernelStatus() {}
void CMFCNewAPITestProjectDlg::OnEnChangeNetworkStatus() {}
void CMFCNewAPITestProjectDlg::OnEnChangeDi() {}
void CMFCNewAPITestProjectDlg::OnEnChangeSdoWriteValue() {}
void CMFCNewAPITestProjectDlg::OnEnChangeSdoReadValue() {}
void CMFCNewAPITestProjectDlg::OnEnChangeSdoReadValue2() {}
void CMFCNewAPITestProjectDlg::OnEnChangeAiRead() {}
void CMFCNewAPITestProjectDlg::OnEnChangeLoadRead() {}
void CMFCNewAPITestProjectDlg::OnEnChangeFuncStatus() {}
void CMFCNewAPITestProjectDlg::OnEnChangeLoadCellStatus() {}


void CMFCNewAPITestProjectDlg::OnBnClickedLoopController()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (bloop == true)
		bloop = false;
	else
		bloop = true;
}
