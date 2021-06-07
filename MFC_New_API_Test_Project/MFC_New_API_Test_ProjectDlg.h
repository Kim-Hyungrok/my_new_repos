
// MFC_New_API_Test_ProjectDlg.h: 헤더 파일
//

#pragma once

#include "MXP_EasyClass.h"		//Include new version of MXP API

// CMFCNewAPITestProjectDlg 대화 상자
class CMFCNewAPITestProjectDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCNewAPITestProjectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CMFCNewAPITestProjectDlg();

	bool bloop = true;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_NEW_API_TEST_PROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInit();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedDestroy();
	afx_msg void OnBnClickedServoOn();
	afx_msg void OnBnClickedServoOff();
	afx_msg void OnEnChangeSlaveCount();
	afx_msg void OnEnChangeAxisCount();
	afx_msg void OnBnClickedM0Home();
	afx_msg void OnEnChangeM0TargetPos();
	afx_msg void OnBnClickedM0Move();
	afx_msg void OnBnClickedM0Stop();
	afx_msg void OnEnChangeDpPos();
	afx_msg void OnEnChangeDpVel();
	afx_msg void OnEnChangeDpTor();
	afx_msg void OnEnChangeM0Status();
	afx_msg void OnEnChangeM0Error();
	afx_msg void OnBnClickedM0Reset();
	afx_msg void OnEnChangeEcatStatus();
	afx_msg void OnEnChangeKernelStatus();
	afx_msg void OnEnChangeNetworkStatus();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedDoOn();
	afx_msg void OnBnClickedDoOff();
	afx_msg void OnEnChangeDi();
	afx_msg void OnEnChangeSdoWriteValue();
	afx_msg void OnBnClickedSdoWrite();
	afx_msg void OnEnChangeSdoReadValue();
	afx_msg void OnBnClickedSdoRead();
	afx_msg void OnEnChangeSdoReadValue2();
	afx_msg void OnEnChangeAiRead();
	afx_msg void OnBnClickedAiLaserRead();
	afx_msg void OnEnChangeLoadRead();
	afx_msg void OnBnClickedLoadUpperRead();
	afx_msg void OnBnClickedLedR();
	afx_msg void OnBnClickedLedG();
	afx_msg void OnBnClickedLedB();
	afx_msg void OnEnChangeFuncStatus();
	afx_msg void OnBnClickedTurnLed();
	afx_msg void OnBnClickedInitsystem();
	afx_msg void OnEnChangeLoadCellStatus();
	afx_msg void OnBnClickedPeriodicTest();
	afx_msg void OnBnClickedLoopController();
};
