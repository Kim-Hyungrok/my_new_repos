//==============================================================================
//Copyright (c) 2013-2015 LS Mecapion Motion R&D Lab. All rights reserved.
//==============================================================================
//MODULE		: MXP_SoftMotion.h
//Project		: MXP2.0 
//ABSTRACT		: Main include header file
//Environment	: Win32
//NOTE			: 
//==============================================================================
#pragma warning(disable : 4996)
#pragma warning(disable : 4995)
#pragma once

//#ifdef _FOR_DEBUG
//#pragma comment(lib, "MXP_SoftMotionD")		// MXP_SoftMotion lib for Debug mode.
//#else
//#pragma comment(lib, "MXP_SoftMotion")		// MXP_SoftMotion lib for Release mode.

#ifdef _X64
#define ExternC		
#else 
#define ExternC		extern "C"
#endif
#ifdef MXP_SOFTMOTIONDLLDLL
	#define MXPSOFTMOTION_API __declspec( dllexport )
#else
	#define MXPSOFTMOTION_API __declspec( dllimport )
#endif

#ifndef __SOFTMOTION__
#include "MXP_PLCopenBlock.h"				
#endif

#define IN
#define OUT

#define MXP_STATUS MXP_FUNCTION_STATUS

#define	ALARM_A_APPEAR		2

typedef enum MXP_FUNCTION_STATUS
{
	RET_NO_ERROR						= 0,
	RET_ERROR_FUNCTION					= -1,	// Error (Functional buffer over flow)
	RET_ERROR_FULL						= -2,	// Buffer for command is full.
	RET_ERROR_WRONG_INDEX				= -3,	// Commanded Motion block index number is out of range.
	RET_ERROR_WRONG_AXISNO				= -4,	// Axis number does not exist.
	RET_ERROR_MOTIONBUSY				= -5,	// Commanded Motion block is already working.
	RET_ERROR_WRONG_SLAVENO				= -6,	// Slave number does not exist.
	RET_ERROR_WRONG_CAMTABLENO			= -7,	// CamTable number is out of range.
	RET_ERROR_WRONG_ECMASTERNO			= -8,	// ECamMaster number does not exist.
	RET_ERROR_WRONG_ECSLAVENO			= -9,	// ECamSlaver number does not exist.
	RET_ERROR_NOT_OPMODE				= -10,	// Slave is not op-mode.
	RET_ERROR_NOTRUNNING				= -11,	// Motion kernel is not running
	RET_ERROR_WRONG_PARAMETER_NO		= -12,	// Parameter number does not exist.
	RET_ERROR_WRONG_MXP_TYPE			= -13,	// License is not MXP-A type.
	RET_ERROR_ALREADYOPEN				= -14,	// Anaother API Opne to DLL
	RET_ERROR_NOT_SCANMODE				= -15,	// Anaother API Opne to DLL
	RET_ERROR_WRONG_ONLINESTATE			= -16,	// Anaother API Opne to DLL
	RET_ERROR_NOT_SIMULATIONMODE		= -17,	// Anaother API Opne to DLL
	RET_ERROR_NOT_FOEMODE				= -18,	// Anaother API Opne to DLL
	RET_ERROR_NOT_INVALID_LIC_FEATURE	= -19,	// Anaother API Opne to DLL

	///// ProfileMove Error Define 
	RET_ERROR_INVAILD_LASTSTEPVEL = -20 ,
	RET_ERROR_INVAILD_LASTTEPPOS = -21,
	RET_ERROR_INVAILD_FIRSTSTEPVEL = -22
};

typedef enum
{
	STATUS_KRNL_BUSY			= 0,
	STATUS_KRNL_READY			= 1,
	STATUS_KRNL_RUN				= 2,
	STATUS_KRNL_STOP			= 3,
};


typedef enum
{
	SYSTEM_UNLICENSED	= -2,			// System is NOT licensed.
	SYSTEM_IDLE			= 1,			// System is no working.
	SYSTEM_KILLING		= 2,			// System is killing.
	SYSTEM_KILLED		= 3,			// System is killed. 
	SYSTEM_CREATING		= 4,			// System is creating.
	SYSTEM_CREATED		= 5,			// System is created.
	SYSTEM_INITING		= 6,			// System is initializing.
	SYSTEM_INITED		= 7,			// System is initialized. 
	SYSTEM_READY		= 8,			// System is Initialized. System is ready to run.
	SYSTEM_RUN			= 9,			// System is running.
};


typedef enum
{
	NET_STATE_INIT_CMD		= 0x01,
	NET_STATE_PREOP_CMD		= 0x02,
	NET_STATE_BOOT_CMD		= 0x03,
	NET_STATE_SAFEOP_CMD	= 0x04,
	NET_STATE_OP_CMD		= 0x08,

} EC_NETWORK_CMD;

typedef enum
{
	NET_STATE_INIT			= 0x01,
	NET_STATE_PREOP			= 0x02,
	NET_STATE_BOOT			= 0x03,
	NET_STATE_SAFEOP		= 0x04,
	NET_STATE_OP			= 0x08,
	NET_STATE_ERR_NONE		= 0x10,
	NET_STATE_ERR_INIT		= 0x11,
	NET_STATE_ERR_PREOP		= 0x12,
	NET_STATE_ERR_BOOT		= 0x13,
	NET_STATE_ERR_SAFEOP	= 0x14,
	NET_STATE_ERR_OP		= 0x18

} EC_NETWORK_SATUS;

typedef enum 
{
	EC_CMD_TYPE_NOP		= 0,
	EC_CMD_TYPE_APRD	= 1,
	EC_CMD_TYPE_APWR	= 2,
	EC_CMD_TYPE_APRW	= 3,
	EC_CMD_TYPE_FPRD	= 4,
	EC_CMD_TYPE_FPWR	= 5,
	EC_CMD_TYPE_FPRW	= 6,
	EC_CMD_TYPE_BRD		= 7,
	EC_CMD_TYPE_BWR		= 8,
	EC_CMD_TYPE_BRW		= 9,
	EC_CMD_TYPE_LRD		= 10,
	EC_CMD_TYPE_LWR		= 11,
	EC_CMD_TYPE_LRW		= 12,
	EC_CMD_TYPE_ARMW	= 13

} EC_CMD_TYPE;

#define	EC_HEAD_IDX_EXTERN_VALUE	0xFF

/************************************************************************/
/* EtherCAT Online status		                                        */
/************************************************************************/
#define		STATE_NONE			((UINT8) 0x00)
#define 	STATE_INIT			((UINT8) 0x01)
#define 	STATE_PREOP 		((UINT8) 0x02)
#define 	STATE_BOOT			((UINT8) 0x03)
#define 	STATE_SAFEOP		((UINT8) 0x04)
#define 	STATE_OP			((UINT8) 0x08)

#define		STATE_ERROR_NONE	((UINT8) 0x10)
#define		STATE_ERROR_INIT	((UINT8) 0x11)
#define		STATE_ERROR_PREOP	((UINT8) 0x12)
#define		STATE_ERROR_BOOT	((UINT8) 0x13)
#define		STATE_ERROR_SAFEOP	((UINT8) 0x14)
#define		STATE_ERROR_OP		((UINT8) 0x18)


#define	ESC_REG_ESC_TYPE				0x0000	// --> ET1000 local address
#define	ESC_REG_ESC_REVISION			0x0001
#define	ESC_REG_ESC_BUILD				0x0002

#define	ESC_REG_FEATURES				0x0008
#define	ESC_FEATURE_DC_AVAILABLE	0x0004
#define	ESC_FEATURE_DC64_AVAILABLE	0x0008

//////////////////////////////////////////////

#define ESC_REG_CONFIG_STATION_ADDR		0x0010
#define ESC_REG_CONFIG_STATION_ALIAS	0x0012

#define ESC_REG_WR_REG_ENABLE			0x0020
#define ESC_REG_WR_REG_PROTECT			0x0021
#define ESC_REG_WR_ENABLE				0x0030
#define ESC_REG_WR_PROTECT				0x0031

#define ESC_REG_RST_ECAT				0x0040
#define ESC_REG_RST_PDI					0x0041
#define ESC_REG_ESC_DL_CNTL0			0x0100		//:0x0103 4
#define ESC_REG_PHY_RW_OFFSET			0x0108		//:0x0109 2
#define ESC_REG_ESC_DL_STATUS			0x0110		//:0x0111 2
//Application Layer
#define ESC_REG_AL_CNTL					0x0120		//:0x0121 2
#define ESC_REG_AL_STATUS				0x0130		//:0x0131 2
#define ESC_REG_AL_STATUS_CODE			0x0134		//:0x0135 2

//ESI EEPROM Interface
#define ESC_REG_EEPROM_CONFIG			0x0500
#define ESC_REG_EEPROM_PDI_ACCESS_STAT	0x0501
#define ESC_REG_EEPROM_CNTR_STAT		0x0502		//:0x0503 2 EEPROM Control/Status
#define ESC_REG_EEPROM_ADDR				0x0504		//:0x0507 4 EEPROM Address
#define ESC_REG_EEPROM_DATA				0x0508		//:0x050F 4/8 EEPROM Data

//////////////////////////////////////////////

#define	ESC_REG_ESC_STATUS				0x0111
#define	EC_REG_PDI_CTRL					0x0140

#define	ESC_REG_SYNCMAN_MBX_OUT			0x0800	//
// +0x0:0x1 : Physical Start Address
// +0x2:0x3 : Length
// +0x4		: Control Register
// +0x5		: Status Register
// +0x6		: Activate
// +0x7		: PDI Control
#define	ESC_REG_SYNCMAN_MBX_IN			0x0808

#define	ESC_REG_SYNCMAN0				0x0800
#define	ESC_REG_SYNCMAN1				0x0808
#define	ESC_REG_SYNCMAN2				0x0810
#define	ESC_REG_SYNCMAN3				0x0818
#define	ESC_REG_SYNCMAN4				0x0820
#define	ESC_REG_SYNCMAN5				0x0828
#define	ESC_REG_SYNCMAN6				0x0830
#define	ESC_REG_SYNCMAN7				0x0838
#define	ESC_REG_SYNCMAN8				0x0840
#define	ESC_REG_SYNCMAN9				0x0848
#define	ESC_REG_SYNCMAN10				0x0850
#define	ESC_REG_SYNCMAN11				0x0858
#define	ESC_REG_SYNCMAN12				0x0860
#define	ESC_REG_SYNCMAN13				0x0868
#define	ESC_REG_SYNCMAN14				0x0870
#define	ESC_REG_SYNCMAN15				0x0878

#define	ESC_STATUS_LOOP_A_MASK			0x0300
#define	ESC_STATUS_LOOP_A_CLOSE			0x0100
#define	ESC_STATUS_LOOP_A_LINK			0x0200
#define	ESC_STATUS_LOOP_B_MASK			0x0C00
#define	ESC_STATUS_LOOP_B_CLOSE			0x0400
#define	ESC_STATUS_LOOP_B_LINK			0x0800
#define	ESC_STATUS_LOOP_C_MASK			0x3000
#define	ESC_STATUS_LOOP_C_CLOSE			0x1000
#define	ESC_STATUS_LOOP_C_LINK			0x2000
#define	ESC_STATUS_LOOP_D_MASK			0xC000
#define	ESC_STATUS_LOOP_D_CLOSE			0x4000
#define	ESC_STATUS_LOOP_D_LINK			0x8000

#define	ESC_REG_DC_SOF_LATCH_A			0x0900
#define	ESC_REG_DC_SOF_LATCH_B			0x0904
#define	ESC_REG_DC_SOF_LATCH_C			0x0908
#define	ESC_REG_DC_SOF_LATCH_D			0x090c

#define	ESC_REG_DC_SYSTIME				0x0910
#define	ESC_REG_DC_SOF_LATCH_A_64		0x0918
#define	ESC_REG_DC_SYSTIME_OFFS			0x0920
#define	ESC_REG_DC_SYSTIME_DELAY		0x0928
#define	ESC_REG_DC_CONTROL_ERROR		0x092c
#define	ESC_REG_DC_SPEED_START			0x0930
#define	ESC_REG_DC_SPEED_DIFF			0x0932
#define	ESC_REG_DC_FILTER_EXP			0x0934
#define	ESC_REG_DC_SOF_LATCH_FRAME		0x0936

#define	ESC_REG_DC_CYC_START_TIME		0x0990

#define	ESC_REG_DIGITAL_OUTPUTS			0x0f00
// 2018.02.19 KEJ
#define SET	1
#define CLEAR 0



/************************************************************************/
/* System Information Structure	                                        */
/************************************************************************/
typedef struct MXP_SYSINFO_OUT
{
	struct HEARTBEAT
	{
		INT AX;
		INT Main;
		INT Motion;
		INT Scheduler;
		INT Modbus;
		INT EtherCAT;

	} Heartbeat;

	struct CREATION
	{
//#if UNUSED_RTOS_SW
//		BOOL Main;
//		BOOL Motion;
//		BOOL Scheduler;
//		BOOL Modbus;
//		BOOL EtherCAT;
//#elif UNDER_RTX || UNDER_INTIME || UNDER_INTIME_NTX
		INT Main;
		INT Motion;
		INT Scheduler;
		INT Modbus;
		INT EtherCAT;
		//#endif

	} Creation;

	struct MODE
	{
//#if UNUSED_RTOS_SW
//		BOOL Start;
//		BOOL Init;
//		BOOL Ready;
//		BOOL Run;
//		BOOL Stop;
//		BOOL Reserved1;
//		BOOL Download;
//		BOOL Clear;
//
//		BOOL Reserved;
//#elif UNDER_RTX || UNDER_INTIME || UNDER_INTIME_NTX
		INT Start;
		INT Init;
		INT Ready;
		INT Run;
		INT Stop;
		INT Reserved1;
		INT Download;
		INT Clear;

		INT Reserved;
//#endif

	} Mode;

	struct SCANTIME
	{
		struct SETTINGTIME
		{
			FLOAT Sheduler;
			FLOAT Motion;
			FLOAT EtherCATIO;
			FLOAT EtherCATMaster;

		} Settingtime;

		struct CURRENTTIME
		{
			FLOAT Sheduler;
			FLOAT Motion;
			FLOAT EtherCATIO;
			FLOAT EtherCATMaster;

		} CurrentTime;

		struct MINTIME
		{
			FLOAT Sheduler;
			FLOAT Motion;
			FLOAT EtherCATIO;
			FLOAT EtherCATMaster;

		} MinTime;

		struct MAXTIME
		{
			FLOAT Sheduler;
			FLOAT Motion;
			FLOAT EtherCATIO;
			FLOAT EtherCATMaster;

		} MaxTime;

		struct CURRENTOPTIME
		{
			FLOAT Sheduler;
			FLOAT Motion;
			FLOAT EtherCATIO;
			FLOAT EtherCATMaster;

		} CurrentOPTime;

		struct MAXOPTIME
		{
			FLOAT Sheduler;
			FLOAT Motion;
			FLOAT EtherCATIO;
			FLOAT EtherCATMaster;

		} MaxOPTime;
						
	} Scantime;

	struct ETHERCAT_STATE
	{
		INT	  DCfaultCnt;
		FLOAT DCSlotPos;
		FLOAT DCPrevInterval;
		INT   DcPlus;
		INT   DcMinus;

		ULONG QueuedSendFrames;
		ULONG QueuedLostFrames;
		FLOAT QueuedSendFramesPerSec;

		ULONG CyclicSendFrames;
		ULONG CyclicLostFrames;
		FLOAT CyclicSendFramesPerSec;

		ULONG RxErrorCnt;
		ULONG TxErrorCnt;

		INT Master;
		INT Slaves[132];

	} EthercatState;

	struct ECAT_ERR
	{
		BYTE LinkLost[4];
		BYTE InvalidFrameCnt[4];
		BYTE RxErrCnt[4];
		BYTE ForwardRxErrCnt[4];
		BYTE EcatPUErrorCnt;
		BYTE PDIErrorCnt;
		BYTE AlstatusCode;

	} EcatErr[132];

	struct ETHERCATLINKSTATUS
	{
		INT Master;
		INT Slaves[132];

	} EthecatLinkStatus;

	struct ALARM
	{
		INT History[20];

	} Alarm;

	struct PID
	{
		DWORD UserApp;
		DWORD Mpm;
		DWORD Main;
		DWORD Motion;
		DWORD Scheduler;
		DWORD Modbus;
		DWORD EtherCAT;
	} Pid;

//#if UNUSED_RTOS_SW
//	BOOL VerboseMode;
//#elif UNDER_RTX || UNDER_INTIME || UNDER_INTIME_NTX
	INT VerboseMode;
//#endif
	
} _MXP_SYSINFO_OUT;


/**************************************************************************************************************************************************/
/* Administrative System Functions																											      */
/**************************************************************************************************************************************************/
/*****************************************************************

	Function	: MXPSOFTMOTION_API MXP_InitKernel
	Description	: Initialize Motion Kernel process

				  ※ If motion kernel is not closed normally, Doing Init Kernel process takes for long time.
				  
	Output		:	1 : System Idle
					4 : System Creating
					5 : System Created
					6 : System_Initializing
					7 : System_Initialized
					8 : System ready.
					9 : System running.

	return		:   -3 : Motion kernel already running.
					-2 : Use Invalid Lock key.
					-1 : Functional error (Can not create kernel)
					 0 : No error
	
	Revision	: 2.0
	Note        :

*******************************************************************/

ExternC	MXPSOFTMOTION_API MXP_STATUS	__stdcall MXP_IsInited(OUT BYTE &Status);
ExternC MXPSOFTMOTION_API MXP_STATUS	__stdcall MXP_InitKernel(OUT UINT32 &Status);	// Initialize MXP kernel engine
ExternC MXPSOFTMOTION_API MXP_STATUS	__stdcall MXP_Destroy();						// Terminator

ExternC MXPSOFTMOTION_API MXP_STATUS	__stdcall MXP_InitKernel_Developer(OUT UINT32 &Status);
ExternC MXPSOFTMOTION_API MXP_STATUS	__stdcall MXP_Destroy_Developer();

/*****************************************************************

	Function	: MXPSOFTMOTION_API MXP_GetKernelStatus
	Description	: Get Motion kernel status

	return		: Motion kernel status
					1 : System idle.
					2 : System killing.
					3 : System killed.
					4 : System creating.
					5 : System created.
					6 : System initializing.
					7 : System initialized.
					8 : System ready.
					9 : System running.

*******************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS	__stdcall MXP_GetKernelStatus(INT32 &Status);														

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_SystemRun();		// Run MXP system.
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_SystemStop();		// Stop MXP system.
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_SystemReset();	// Reset MXP system.


/*****************************************************************

	Function	: MXPSOFTMOTION_API MXP_GetOnlineMode
	Description	: Get current mode state

	Output		: Network status
				  STATE_INIT		((UINT328) 0x01)
				  STATE_PREOP 		((UINT328) 0x02)
				  STATE_BOOT		((UINT328) 0x03)
				  STATE_SAFEOP		((UINT328) 0x04)
				  STATE_OP			((UINT328) 0x08)

	return		:    0 : No error
					-1 : Error

*******************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetOnlineMode(OUT UINT32 &Status);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_IsSlaveOnline(IN UINT32 SlaveNo, OUT UINT32 &Status);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetSlaveCount(IN UINT32 IsAxis, OUT UINT32 &Count);
//ExternC MXPSOFTMOTION_API LPTSTR	 __stdcall MXP_GetSlaveName(IN UINT32 SlaveNo);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetSlaveName(IN UINT32 SlaveNo, OUT LPWSTR pName);


ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_QueryNodeType(IN UINT32 SlaveNo, OUT UINT32 &NodeType );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetSystemInformation(OUT MXP_SYSINFO_OUT &SysInfo);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetAxisNoFromNodeId(IN UINT32 NodeId, OUT UINT32 &AxisNo);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetSlaveNoFromNodeId(IN UINT32 NodeId, OUT UINT32 &SlaveNo);

/**************************************************************************************************************************************************/
/* Single Axis																													  */
/**************************************************************************************************************************************************/

/*************************************************************************
 param  InParam		: [IN]  Motion block number *Must be same as number which inputted when function command uses
					  [IN] Structure to MC_***** input parameters

 return				: 0 if success
 					: error_id in case of error
					  -1  : Error (Functional buffer over flow)
					  -2  : Buffer for command is full.
					  -3  : Commanded Motion block index number is out of range.
					  -4  : Axis number does not exist.
					  -10 : Axis is not operation mode (communication not ready)
					  -11 : Motion kernel is not running.
					  
*************************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_PowerCmd( IN UINT32 MotionBlockIndexNo, IN  MXP_POWER_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_HaltCmd( IN UINT32 MotionBlockIndexNo, IN MXP_HALT_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_StopCmd( IN UINT32 MotionBlockIndexNo, IN MXP_STOP_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ResetCmd( IN UINT32 MotionBlockIndexNo, IN MXP_RESET_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_HomeCmd( IN UINT32 MotionBlockIndexNo, IN MXP_HOME_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveAbsoluteCmd( IN UINT32 MotionBlockIndexNo, IN MXP_MOVEABSOLUTE_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveRelativeCmd( IN UINT32 MotionBlockIndexNo, IN MXP_MOVERELATIVE_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveAbsoluteCmd_Ex( IN UINT32 MotionBlockIndexNo, IN MXP_MOVEABSOLUTE_EX_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveRelativeCmd_Ex( IN UINT32 MotionBlockIndexNo, IN MXP_MOVERELATIVE_EX_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveVelocityCmd( IN UINT32 MotionBlockIndexNo, IN MXP_MOVEVELOCITY_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_TorqueControlCmd( IN UINT32 MotionBlockIndexNo, IN _MXP_TORQUECONTROL_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_TorqueFeedbackControlCmd( IN UINT32 MotionBlockIndexNo, IN _MXP_TORQUECONTROL_IN *pInParam, IN MXP_FEEDBACK_IN *pInParam2 );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_DirectTorqueControlCmd( IN UINT32 MotionBlockIndexNo, IN MXP_DIRECTTORQUECONTROL_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetDirectTorqueControlOutParam(IN UINT32 nMotionBlockIndexNo, OUT MXP_DIRECTTORQUECONTROL_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveDirectVelocityCmd( IN UINT32 MotionBlockIndexNo, IN MXP_MOVEDIRECTVELOCITY_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMoveDirectVelocityOutParam(IN UINT32 nMotionBlockIndexNo, OUT MXP_MOVEDIRECTVELOCITY_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveDirectPositionCmd( IN UINT32 MotionBlockIndexNo, IN MXP_MOVEDIRECTPOSITION_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMoveDirectPositionOutParam(IN UINT32 nMotionBlockIndexNo, OUT MXP_MOVEDIRECTPOSITION_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_SetDirectPositionCmd( IN UINT32 MotionBlockIndexNo, IN MXP_SETDIRECTPOSITION_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetSetDirectPositionOutParam( IN UINT32 nMotionBlockIndexNo, OUT MXP_SETDIRECTPOSITION_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_SetPositionCmd( IN UINT32 MotionBlockIndexNo, IN MXP_SETPOSITION_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_SetOverrideCmd( IN UINT32 MotionBlockIndexNo, IN MXP_SETOVERRIDE_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_WriteParameterCmd( IN UINT32 MotionBlockIndexNo, IN MXP_WRITEPARAMETER_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_WriteBoolParameterCmd( IN UINT32 MotionBlockIndexNo, IN MXP_WRITEBOOLPARAMETER_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_WriteDigitalOutputCmd( IN UINT32 MotionBlockIndexNo, IN MXP_WRITEDIGITALOUTPUT_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadParameter( IN MXP_READPARAMETER_IN *pInParam, OUT MXP_READPARAMETER_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadBoolParameter( IN MXP_READBOOLPARAMETER_IN *pInParam, OUT MXP_READBOOLPARAMETER_OUT &OutParam );	// Not supports yet/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadActualPosition( IN MXP_READACTUALPOSITION_IN *pInParam, OUT MXP_READACTUALPOSITION_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadActualVelocity( IN MXP_READACTUALVELOCITY_IN *pInParam, OUT MXP_READACTUALVELOCITY_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadFollowingErrorValue( IN MXP_READFOLLOWINGERRORVALUE_IN *pInParam, OUT MXP_READFOLLOWINGERRORVALUE_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadActualTorque( IN MXP_READACTUALTORQUE_IN *pInParam, OUT MXP_READACTUALTORQUE_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadActualForce( IN MXP_READACTUALFORCE_IN *pInParam, OUT MXP_READACTUALFORCE_OUT &OutParam ); 	// Not supports yet.
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadStatus( IN MXP_READSTATUS_IN *pInParam, OUT MXP_READSTATUS_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadMotionState( IN MXP_READMOTIONSTATE_IN *pInParam, OUT MXP_READMOTIONSTATE_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadAxisInfo( IN MXP_READAXISINFO_IN *pInParam, OUT MXP_READAXISINFO_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadAxisError( IN MXP_READAXISERROR_IN *pInParam, OUT MXP_READAXISERROR_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadDigitalInput( IN MXP_READDIGITALINPUT_IN *pInParam, OUT MXP_READDIGITALINPUT_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadDigitalOutput( IN MXP_READDIGITALOUTPUT_IN *pInParam, OUT MXP_READDIGITALOUTPUT_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_WriteForceDigitalInputCmd( IN UINT32 MotionBlockIndexNo, IN MXP_WRITEFORCEDIGITALINPUT_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_WriteForceInputsCmd(IN UINT32 MotionBlockIndexNo, IN MXP_WRITEFORCEINPUTS_IN *pInParam, IN BYTE *Data);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GroupSetOverrideCmd( IN UINT32 MotionBlockIndexNo, IN MXP_GroupSetOverride_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_WriteParameterExCmd( IN MXP_WRITEPARAMETEREX_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetWriteParameterExOutParam( OUT MXP_WRITEPARAMETEREX_OUT &OutParam );

/**************************************************************************************************************************************************/
/* Multiple Axis																																  */
/**************************************************************************************************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_CamTableSelectCmd( IN UINT32 MotionBlockIndexNo, IN MXP_CAMTABLESELECT_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_WriteCamTableCmd( IN UINT32 MotionBlockIndexNo, IN MXP_WRITECAMTABLE_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadCamTableCmd( IN UINT32 MotionBlockIndexNo, IN MXP_READCAMTABLE_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_CamInCmd( IN UINT32 MotionBlockIndexNo, IN MXP_CAMIN_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_CamOutCmd( IN UINT32 MotionBlockIndexNo, IN MXP_CAMOUT_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_CamInCmd_Ex( IN UINT32 MotionBlockIndexNo, IN MXP_CAMIN_EX_IN *pInParam ); 



ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_CamScalingCmd( IN UINT32 MotionBlockIndexNo, IN MXP_CAMSCALING_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_PhasingCmd( IN UINT32 MotionBlockIndexNo, IN MXP_PHASING_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_RdCamTableSlavePosCmd( IN UINT32 MotionBlockIndexNo, IN MXP_RDCAMTABLESLAVEPOS_IN *pInParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GearInCmd( IN UINT32 MotionBlockIndexNo, IN MXP_GEARIN_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GearOutCmd( IN UINT32 MotionBlockIndexNo, IN MXP_GEAROUT_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GearInPosCmd( IN UINT32 MotionBlockIndexNo, IN MXP_GEARINPOS_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GroupReadActualVelocity(IN MXP_GROUPREADACTUALVELOCITY_IN *pInParam, OUT MXP_GROUPREADACTUALVELOCITY_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GroupReadActualPosition(IN MXP_GROUPREADACTUALPOSITION_IN *pInParam, OUT MXP_GROUPREADACTUALPOSITION_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GroupReadCommandVelocity(IN MXP_GROUPREADCOMMANDVELOCITY_IN *pInParam, OUT MXP_GROUPREADCOMMANDVELOCITY_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GroupReadCommandPosition(IN MXP_GROUPREADCOMMANDPOSITION_IN *pInParam, OUT MXP_GROUPREADCOMMANDPOSITION_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveLinearAbsoluteCmd( IN UINT32 MotionBlockIndexNo, IN MXP_MOVELINEARABSOLUTE_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveLinearRelativeCmd( IN UINT32 MotionBlockIndexNo, IN MXP_MOVELINEARRELATIVE_IN *pInParam );


ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveLinearAbsoluteCmd_Ex( IN UINT32 MotionBlockIndexNo, IN MXP_MOVELINEARABSOLUTE_EX_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveLinearRelativeCmd_Ex( IN UINT32 MotionBlockIndexNo, IN MXP_MOVELINEARRELATIVE_EX_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveCircularAbsoluteCmd( IN UINT32 MotionBlockIndexNo, IN MXP_MOVECIRCULARABSOLUTE_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MoveCircularRelativeCmd( IN UINT32 MotionBlockIndexNo, IN MXP_MOVECIRCULARRELATIVE_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GroupStopCmd( IN UINT32 MotionBlockIndexNo, IN MXP_GROUPSTOP_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GroupHomeCmd( IN UINT32 MotionBlockIndexNo, IN MXP_GROUPHOME_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GroupSetPositionCmd( IN UINT32 MotionBlockIndexNo, IN MXP_GROUPSETPOSITION_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MultiAxisCCCSetCmd( IN UINT32 MotionBlockIndexNo, IN MXP_MULTIAXISCOUPLESET_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MultiAxisCCCReSetCmd( IN UINT32 MotionBlockIndexNo, IN MXP_MULTIAXISCOUPLERESET_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_MultiAxisCCCGainSetCmd( IN UINT32 MotionBlockIndexNo, IN MXP_MULTIAXISCOUPLEGAINSET_IN *pInParam );


/*************************************************************************
 param  InParam		: [IN]  Motion block number *Must be same as number which inputted when function command uses
 param  OutParam	: [OUT] Reference to MC_Power output parameters
 return	return		: 0 if success
 					  error_id in case of error
					  -1 : Error (Functional buffer over flow)
					  -3 : Commanded Motion block index number is out of range.
					  					  
*************************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetPowerOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_POWER_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetHaltOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_HALT_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetStopOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_STOP_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetResetOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_RESET_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetHomeOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_HOME_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMoveAbsoluteOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_MOVEABSOLUTE_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMoveRelativeOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_MOVERELATIVE_OUT &OutParam);

//ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMoveAbsoluteOutParam_Ex(IN UINT32 MotionBlockIndexNo, OUT MXP_MOVEABSOLUTE_OUT &OutParam);
//ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMoveRelativeOutParam_Ex(IN UINT32 MotionBlockIndexNo, OUT MXP_MOVERELATIVE_OUT &OutParam);


ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMoveVelocityOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_MOVEVELOCITY_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetTorqueControlOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_TORQUECONTROL_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetTorqueFeedbackControlOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_TORQUECONTROL_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetSetPositionOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_SETPOSITION_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetSetOverrideOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_SETOVERRIDE_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetWriteParameterOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_WRITEPARAMETER_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetWriteBoolParameterOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_WRITEBOOLPARAMETER_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetWriteDigitalOutputOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_WRITEDIGITALOUTPUT_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetWriteForceDigitalInputOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_WRITEFORCEDIGITALINPUT_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetWriteForceInputsOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_WRITEFORCEINPUTS_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetGroupSetOverrideOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_GroupSetOverride_Out &OutParam);

/**************************************************************************************************************************************************/
/* Multiple Axis																																  */
/**************************************************************************************************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetCamTableSelectOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_CAMTABLESELECT_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetWriteCamTableOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_WRITECAMTABLE_OUT & OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetReadCamTableOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_READCAMTABLE_OUT & OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetCamInOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_CAMIN_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetCamInOutParam_EX( IN UINT32 MotionBlockIndexNo, OUT MXP_CAMIN_EX_OUT &OutParam );



ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetCamOutOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_CAMOUT_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetCamScalingOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_CAMSCALING_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetPhasingOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_PHASING_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetRdCamTableSlavePosOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_RDCAMTABLESLAVEPOS_OUT & OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetGearInOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_GEARIN_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetGearOutOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_GEAROUT_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetGearInPosOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_GEARINPOS_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMoveLinearAbsoluteOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_MOVELINEARABSOLUTE_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMoveLinearRelativeOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_MOVELINEARRELATIVE_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMoveCircularAbsoluteOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_MOVECIRCULARABSOLUTE_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMoveCircularRelativeOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_MOVECIRCULARRELATIVE_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetGroupSetPositionOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_GROUPSETPOSITION_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetGroupStopOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_GROUPSTOP_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetGroupHomeOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_GROUPHOME_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMultiAxisCCCSetOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_MULTIAXISCOUPLESET_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMultiAxisCCCReSetOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_MULTIAXISCOUPLERESET_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetMultiAxisCCCGainSetOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_MULTIAXISCOUPLEGAINSET_OUT &OutParam );



/**************************************************************************************************************************************************/
/* Vendor specific api																															  */
/**************************************************************************************************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_GetReadParameterOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_ET_READPARAMETER_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_GetWriteParameterOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_ET_WRITEPARAMETER_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetReadVendorSpecificL7N2608OutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_READVENDORSPECIFICL7N2608_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadInputs( IN MXP_READINPUTS_IN *pInParam, OUT MXP_READINPUTS_OUT &OutParam , OUT BYTE *Data);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadOutputs(  IN MXP_READOUTPUTS_IN *pInParam, OUT MXP_READOUTPUTS_OUT &OutParam , OUT BYTE *Data);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_WriteOutputsCmd(IN UINT32 MotionBlockIndexNo, IN MXP_WRITEOUTPUTS_IN *pInParam, IN BYTE *Data);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetWriteOutputsOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_WRITEOUTPUTS_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadCommandPosition(IN MXP_READCOMMANDPOSITION_IN *pInParam, OUT MXP_READCOMMANDPOSITION_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadCommandVelocity(IN MXP_READCOMMANDVELOCITY_IN *pInParam, OUT MXP_READCOMMANDVELOCITY_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_ReadParameterCmd( IN UINT32 MotionBlockIndexNo, MXP_ET_READPARAMETER_IN *pInParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_WriteParameterCmd( IN UINT32 MotionBlockIndexNo, MXP_ET_WRITEPARAMETER_IN *pInParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadVendorSpecificL7N2608Cmd( IN UINT32 MotionBlockIndexNo, IN MXP_READVENDORSPECIFICL7N2608_IN *pInParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_SetTouchProbeFunctionCmd( IN UINT32 MotionBlockIndexNo, IN MXP_SETTOUCHPROBEFUNC_IN *pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetSetTouchProbeFunctionOutParam( IN UINT32 MotionBlockIndexNo, OUT MXP_SETTOUCHPROBEFUNC_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadActualTouchProbePosition(IN MXP_READACTUALTOUCHPROBEPOSITION_IN *pInParam, OUT MXP_READACTUALTOUCHPROBEPOSITION_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadActualTouchProbePosition_Ex(IN MXP_READACTUALTOUCHPROBEPOSITION_IN *pInParam, OUT MXP_READACTUALTOUCHPROBEPOSITION_EX_OUT &OutParam );



ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadActualTouchProbeStatus(IN MXP_READACTUALTOUCHPROBESTATUS_IN *pInParam, OUT MXP_READACTUALTOUCHPROBESTATUS_OUT &OutParam );


ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_WriteControlModeCmd(IN UINT32 MotionBlockIndexNo, IN MXP_WRITECONTROLMODE_IN *pInParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetWriteControlModeOutParam(IN UINT32 MotionBlockIndexNo, IN MXP_WRITECONTROLMODE_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadControlMode(IN MXP_READCONTROLMODE_IN *pInParam, OUT MXP_READCONTROLMODE_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadPDOData(IN MXP_READPDODATA_IN *pInParam, OUT MXP_READPDODATA_OUT &OutParam, OUT BYTE *Data);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_WritePDODataCmd(IN UINT nMotionBlockIndexNo, IN MXP_WRITEPDODATA_IN *pInParam, IN BYTE *Data);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetWritePDODataOutParam( IN UINT32 nMotionBlockIndexNo, OUT MXP_WRITEPDODATA_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_BufferedDigitalioCmd( IN UINT32 MotionBlockIndexNo, IN MXP_BUFFEREDDIGITALIO_IN	*pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetBufferedDigitalioOutParam( IN UINT32 nMotionBlockIndexNo, OUT MXP_BUFFEREDDIGITALIO_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GroupBufferedDigitalioCmd( IN UINT32 MotionBlockIndexNo, IN MXP_GROUPBUFFEREDDIGITALIO_IN	*pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetGroupBufferedDigitalioOutParam( IN UINT32 nMotionBlockIndexNo, OUT MXP_GROUPBUFFEREDDIGITALIO_OUT &OutParam );



ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_BufferedDigitalioCmd_Ex( IN UINT32 MotionBlockIndexNo, IN MXP_BUFFEREDDIGITALIO_EX_IN	*pInParam );
//ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetBufferedDigitalioOutParam_Ex( IN UINT32 nMotionBlockIndexNo, OUT MXP_BUFFEREDDIGITALIO_OUT &OutParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GroupBufferedDigitalioCmd_Ex( IN UINT32 MotionBlockIndexNo, IN MXP_GROUPBUFFEREDDIGITALIO_EX_IN	*pInParam );
//ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetGroupBufferedDigitalioOutParam_Ex( IN UINT32 nMotionBlockIndexNo, OUT MXP_GROUPBUFFEREDDIGITALIO_OUT &OutParam );




ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_DwellCmd( IN UINT32 MotionBlockIndexNo, IN MXP_DWELL_IN	*pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetDwellOutParam( IN UINT32 nMotionBlockIndexNo, OUT MXP_DWELL_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GroupDwellCmd( IN UINT32 MotionBlockIndexNo, IN MXP_GROUPDWELL_IN	*pInParam );
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetGroupDwellOutParam( IN UINT32 nMotionBlockIndexNo, OUT MXP_GROUPDWELL_OUT &OutParam );


ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_DwellCmd_Ex( IN UINT32 MotionBlockIndexNo, IN MXP_DWELL_EX_IN	*pInParam );
//ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetDwellOutParam_Ex( IN UINT32 nMotionBlockIndexNo, OUT MXP_DWELL_OUT &OutParam );

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GroupDwellCmd_Ex( IN UINT32 MotionBlockIndexNo, IN MXP_GROUPDWELL_EX_IN	*pInParam );
//ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetGroupDwellOutParam_Ex( IN UINT32 nMotionBlockIndexNo, OUT MXP_GROUPDWELL_OUT &OutParam );


ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_SM_ReadSystemRegister(int nRegType, int nRegNo, int nBitPos, int nDataType, DWORD &dwVal);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_SM_WriteSystemRegister(int nRegType, int nRegNo, int nBitPos, int nDataType, DWORD dwVal);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_OpenExtraSharedMemory(OUT UINT32 &Status);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_CloseExtraSharedMemory();
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_SM_ExtraReadSystemRegister(int nRegType, int nRegNo, int nBitPos, int nDataType, DWORD &dwVal);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_SM_ExtraWriteSystemRegister(int nRegType, int nRegNo, int nBitPos, int nDataType, DWORD dwVal);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_PLC_ReadSystemRegister(int nRegNo, int nBitPos, int nDataType, DWORD &dwVal);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_PLC_WriteSystemRegister(int nRegNo, int nBitPos, int nDataType, DWORD dwVal);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_PLC_ReadSystemRegisterEx(int nRegNo, int nBitPos, int nDataType, ULONGLONG &ullVal);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_PLC_WriteSystemRegisterEx(int nRegNo, int nBitPos, int nDataType, ULONGLONG ulVal);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadAxisBufferInfo(UINT32 nAxisNo , UINT16 &nSaveNum, UINT16 &nReadNum, UINT16 &nCurBlock);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ReadGroupBufferInfo(UINT32 nGroupNo , UINT16 &nSaveNum, UINT16 &nReadNum, UINT16 &nCurBlock);


ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_SM_IsSimulationMode(OUT UINT32 &Status);


/**************************************************************************************************************************************************/
/* Vendor specific api : Scanner API																															  */
/**************************************************************************************************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_ReadRegisterCmd(IN UINT32 MotionBlockIndexNo, MXP_ET_READREGISTER_IN *pInParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_GetReadRegisterOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_ET_READREGISTER_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_WriteRegisterCmd(IN UINT32 MotionBlockIndexNo, MXP_ET_WRITEREGISTER_IN *pInParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_GetWriteRegisterOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_ET_WRITEREGISTER_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_SetMasterOnlineMode( IN EC_NETWORK_CMD iMode);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_SetSlaveOnlineMode( IN int iSlave,IN EC_NETWORK_CMD iMode);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_SetScanMode(BYTE *pMacAddr);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_ReleaseScanMode();
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_IsScanMode(OUT UINT32 &Status);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_ReadObjectListCmd(IN UINT32 MotionBlockIndexNo, MXP_ET_READOBJECTLIST_IN *pInParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_GetReadObjectListOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_ET_READOBJECTLIST_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_ReadObjectDictionaryCmd(IN UINT32 MotionBlockIndexNo, MXP_ET_READOBJECTDICTIONARY_IN *pInParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_GetReadObjectDictionaryOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_ET_READOBJECTDICTIONARY_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_ReadObjectEntryCmd(IN UINT32 MotionBlockIndexNo, MXP_ET_READOBJECTENTRY_IN *pInParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_GetReadObjectEntryOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_ET_READOBJECTENTRY_OUT &OutParam);

/**************************************************************************************************************************************************/
/* Vendor specific api : 무진 PROFILE MOVE API																															  */
/**************************************************************************************************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ProfileMoveCmd( IN UINT32 MotionBlockIndexNo, MXP_PROFILE_MOVE_IN *pInParam , 
																  MXP_PROFILE_TABLE_IN *pProfileTable ,MXP_IO_TABLE_IN *pIOTable);


ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ProfileMoveOutParam(IN UINT32 MotionBlockIndexNo,MXP_PROFILE_MOVE_IN *pInParam,
																	   OUT MXP_PROFILE_MOVE_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetProfileTable(IN MXP_File_TABLE_IN *pInUserTable ,IN UINT32 nUserTableSize, 
																OUT UINT32 &nTableSize , MXP_PROFILE_TABLE_IN **ppProfileTableCov );



/**************************************************************************************************************************************************/
/* Vendor specific api : 무진 PROFILE MOVE API	 ArrayType Ver2																														  */
/**************************************************************************************************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ProfileMoveCmd_Ex( IN UINT32 MotionBlockIndexNo, MXP_PROFILE_MOVE_IN *pInParam , 
																  MXP_PROFILE_TABLE_ARRAY_IN *pProfileTable ,MXP_IO_TABLE_ARRAY_IN *pIOTable);


ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ProfileMoveOutParam_Ex(IN UINT32 MotionBlockIndexNo,MXP_PROFILE_MOVE_IN *pInParam,
																	   OUT MXP_PROFILE_MOVE_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_GetProfileTable_Ex(IN MXP_FILE_TABLE_ARRAY_IN *pInUserTable ,IN UINT32 nUserTableSize, 
																   OUT UINT32 &nTableSize , MXP_PROFILE_TABLE_ARRAY_IN *pProfileTableCov );


/**************************************************************************************************************************************************/
/* Vendor specific api : FOE API																															  */
/**************************************************************************************************************************************************/

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_SetFoeMode(void);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_ReleaseFoeMode(void);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_IsFoeMode(OUT UINT32 &Status);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_InitFoeFileCmd(IN UINT32 MotionBlockIndexNo, MXP_ET_INITFOEFILE_IN *pInParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_GetInitFoeFileOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_ET_INITFOEFILE_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_RegisterFoeSlaveCmd(IN UINT32 MotionBlockIndexNo, MXP_ET_REGISTERFOESLAVE_IN *pInParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_GetRegisterFoeSlaveOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_ET_REGISTERFOESLAVE_OUT &OutParam);

ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_WriteFoeFileCmd(IN UINT32 MotionBlockIndexNo);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_ET_GetWriteFoeFileOutParam(IN UINT32 MotionBlockIndexNo, OUT MXP_ET_WRITEFOEFILE_OUT &OutParam);

/**************************************************************************************************************************************************/
/* License Get api : 																															  */
/**************************************************************************************************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_CheckFeature( IN UINT16 FeatureBit, OUT BYTE &CheckState );


/**************************************************************************************************************************************************/
/* Acc,Dec,Jerk Get api : 																															  */
/**************************************************************************************************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_AccDecToAccTime(IN MXP_ACCDECTOACCTIME_IN *pInParam, OUT MXP_ACCDECTOACCTIME_OUT &OutParam);
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_AccTimeToAccDec(IN MXP_ACCTIMETOACCDEC_IN *pInParam, OUT MXP_ACCTIMETOACCDEC_OUT &OutParam);
	

/**************************************************************************************************************************************************/
/* PDO Mapping Check API : 																															  */
/**************************************************************************************************************************************************/
ExternC MXPSOFTMOTION_API MXP_STATUS __stdcall MXP_PDO_MappingCheck( IN UINT32 AxisID, IN UINT8 Direction, IN UINT32 PDOIndex ,OUT BOOL &MappingState );