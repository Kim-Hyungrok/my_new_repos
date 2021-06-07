#pragma once
#include <windows.h>
#include "Mmsystem.h"
#pragma comment( lib, "winmm.lib" )
#include "MXP_SoftMotion.h"
#include <string>
#include <vector>

using namespace std;

class MXP_EasyClass 
{
public:
	MXP_EasyClass();
	~MXP_EasyClass();

	friend class DataVaildCheck;
	friend class TXTLoader;
	friend class JSONLoader;

	MMRESULT m_idEvent;  
	static void CALLBACK OnTick(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

#pragma region
public:


	static const INT32 MC_SINGLEAXIS_INDEX = 0;
	static const INT32 MC_POWER_INDEX = 128;
	static const INT32 MC_RESETHOME_INDEX = 256;
	static const INT32 MC_GROUPAXIS_INDEX = 384;
	static const INT32 MC_IO_INDEX_START = 416;
	static const INT32 MC_IO_INDEX_END = 447;
	static const INT32 MC_PLCINTERFACE_INDEX = 448;
	static const INT32 MC_PDOWRITE = 450;
	static const INT32 MC_USERSTART_INDEX = 500;
	static const INT32 MC_USEREND_INDEX = 799; 
	static const INT32 MC_PROFILEMOVE_INDEX = 800;


	enum ECmdType
	{
		e_Null = 0,
		e_GearIn = 1,
		e_GearInPos,
		e_CAMIn,
		e_CAMTableOption,
		e_CAMTableEdit,
		e_CAMTable,
		e_ReadCAMSlavePos,
		e_ETParameterReadReq,
		e_ETParameterWriteReq,
		e_ProfileMove,
		e_TouchprobeSet,
		e_Direct_torque,
		e_Direct_Velocity,
		e_Direct_Position,
		e_CCCSet,
		e_CCCReset,
		e_CCCGainSet
	};

	struct strIndexInfo
	{
		bool bUsing;
		INT32 nSlaveNo;
		ECmdType nCmdType;
		INT32 nSubInfo;
	};


private:

	void ClearAllIndex();
	void ClearIndex(UINT32 nIndexCount);
	UINT32 GetMotionIndex(UINT32 nAxisNo, ECmdType nCmdType);
	UINT32 GetMotionIndex(UINT32 nAxisNo, ECmdType nCmdType, INT32 nSubInfo);
	UINT32 GetMotionIndexCheck(UINT32 nAxisNo, ECmdType nCmdType);
	UINT32 GetMotionIndexCheck(UINT32 nAxisNo, ECmdType nCmdType, INT32 nSubInfo);
	UINT32 GetIOIndex(UINT32 nSlave);

#pragma endregion MotionIndex

#pragma region
public:
	enum MXP_CAM_MASTER_TYPE
	{
		MXP_CAM_MASTER_TYPE_MOTOR = 0,
		MXP_CAM_MASTER_TYPE_EXTENAL_ENC = 1,
	};

	enum MXP_INTERPOLATION_ENUM
	{
		CAM_Profile_Monotone_Cubic = 0,
		CAM_Profile_Linear,
		CAM_Profile_Polynom3,
		CAM_Profile_Polynom5,
		CAM_Profile_ModifiedSine,
		CAM_Profile_Cycloid,
		CAM_Profile_Sinusline,
		CAM_Profile_Polynom7
	};

	enum MXP_FUNCTION_STATUS
	{
		RET_NO_ERROR = 0,
		RET_ERROR_FUNCTION = -1, //Error (Functional buffer over flow)
		RET_ERROR_FULL = -2,    // Buffer for command is full.
		RET_ERROR_WRONG_INDEX = -3,    // Commanded Motion block index number is out of range.
		RET_ERROR_WRONG_AXISNO = -4,    // Axis number does not exist.
		RET_ERROR_MOTIONBUSY = -5,    // Commanded Motion block is already working.
		RET_ERROR_WRONG_SLAVENO = -6,    // Slave number does not exist.
		RET_ERROR_WRONG_CAMTABLENO = -7,    // CamTable number is out of range.
		RET_ERROR_WRONG_ECMASTERNO = -8,    // ECamMaster number does not exist.
		RET_ERROR_WRONG_ECSLAVENO = -9,    // ECamSlaver number does not exist.
		RET_ERROR_NOT_OPMODE = -10,   //Slave is not op-mode.
		RET_ERROR_NOTRUNNING = -11,   // Motion kernel is not running
		RET_ERROR_WRONG_PARAMETER_NO = -12,   // Parameter number does not exist.
		RET_ERROR_WRONG_MXP_TYPE = -13,   // License is not MXP-A type.
		RET_ERROR_ALREADYOPEN = -14,   // Anaother API Opne to DLL
		RET_ERROR_NOT_SCANMODE = -15,   // Anaother API Opne to DLL
		RET_ERROR_WRONG_ONLINESTATE = -16,   // Anaother API Opne to DLL
		RET_ERROR_NOT_SIMULATIONMODE = -17,   // Anaother API Opne to DLL
		RET_ERROR_NOT_FOEMODE = -18,  //Anaother API Opne to DLL
		RET_ERROR_NOT_INVALID_LIC_FEATURE = -19,   // Anaother API Opne to DLL

		RET_ERROR_INVAILD_LASTSTEPVEL = -20,
		RET_ERROR_INVAILD_LASTTEPPOS = -21,
		RET_ERROR_INVAILD_FIRSTSTEPVEL = -22,


		//API Wrapping Class Define  = -200,
		RET_ERROR_INVAILD_IOSize = -200,
		RET_ERROR_FAIL_INDEX_CHECK,
		RET_ERROR_EXCEPTIONERROR,
		RET_ERROR_STATECHECK_FAIL,
		RET_INVAILD_PLCADDRESS,

		RET_SEQUENCEMOVE_READ_FAIL,
		RET_SEQUENCEMOVE_INVAILDSTATE,
		RET_SEQUENCEMOVE_PROCESS_FAIL,

		RET_Move_Absolute_VelocityOverride_PARAMETER_INVAILD, //입력 파라메터 유효성 Check Fail

		RET_SPINTABLE_INVALID,  // 입력Table 이 유효하지 않음 
		RET_SPINMOVE_COMMAND_INVALIDSTATE,
		RET_PLCFB_MOVE_FAIL,

		RET_FEEDBACKSENSOR_PARAM_INVAILD,
		RET_FEEDBACKSENSOR_INVALID,  // 입력Table 이 유효하지 않음 
		RET_FEEDBACKSENSOR_COMMAND_INVALIDSTATE,
		RET_FEEDBACKSENSOR_MOVE_FAIL,
		RET_FEEDBACKSENSOR_MOVE_LASTSTEP_DIST_INVALID,
	};

	enum KERNEL_STATUS
	{
		SYSTEM_UNLICENSED = -2,    //System is NOT licensed.
		SYSTEM_IDLE = 1,            //System is no working.
		SYSTEM_KILLING = 2,       //System is killing.
		SYSTEM_KILLED = 3,         //System is killed. 
		SYSTEM_CREATING = 4,       //System is creating.
		SYSTEM_CREATED = 5,        //System is created.
		SYSTEM_INITING = 6,        //System is initializing.
		SYSTEM_INITED = 7,        //System is initialized. 
		SYSTEM_READY = 8,         //System is Initialized. System is ready to run.
		SYSTEM_RUN = 9            //System is running.
	};

	enum MXP_ONLINE_STATE
	{
		ONLINE_STATE_NONE = 0x00,
		ONLINE_STATE_INIT= 0x01,
		ONLINE_STATE_PREOP=0x02,
		ONLINE_STATE_BOOT=0x03,
		ONLINE_STATE_SAFEOP=0x04,
		ONLINE_STATE_OP=0x08,
		ONLINE_STATE_ERROR_NONE=0x10,
		ONLINE_STATE_ERROR_INIT=0x11,
		ONLINE_STATE_ERROR_PREOP=0x12,
		ONLINE_STATE_ERROR_BOOT= 0x13,
		ONLINE_STATE_ERROR_SAFEOP=0x14,
		ONLINE_STATE_ERROR_OP=0x18
	};

	enum MXP_NODE_TYPE
	{
		TYPE_NUll = 0,
		TYPE_DRIVE,
		TYPE_IO,
	};

	enum MXP_PORT_STATE
	{
		DL_STATUS_LOOP_OPEN_NO_LINK = 0,
		DL_STATUS_LOOP_CLOSE_NO_LINK = 1,
		DL_STATUS_LOOP_OPEN_WITH_LINK = 2,
		DL_STATUS_LOOP_CLOSE_WITH_LINK = 3
	};

	enum MXP_IO_Type
	{
		IO_Out = 0,
		IO_In
	};

	enum MXP_ACTIVATIONMODE
	{
		IMMEDIATELY = 0,
		ACTIVATIONPOSITION,
		NEXTPERIOD
	};

	enum MXP_TRIGGER_MODE
	{
		e_Single = 0,
		e_Continuous
	};

	enum MXP_TRIGGER_TYPE
	{
		e_Touchprobe = 0,
		e_Index
	};

	enum MXP_TRIGGER_EDGE
	{
		e_Rising = 0, ///< Untransformed detector coordinates 열겨형일땐 이렇게 주석
		e_Falling, ///< Untransformed detector coordinates
		e_Both  ///< Untransformed detector coordinates
	};

	enum MXP_PDO_DIRECTION
	{
		e_ServoWrite = 0,
		e_MXPWrite
	};

	enum MXP_MOVESTATE
	{
		MOVESTATE_Null = 0,
		MOVESTATE_Moving = 1,
		MOVESTATE_Complete,
		MOVESTATE_Fail
	};

	enum MXP_PLANE
	{
		XPLANE = 0,
		YPLANE,
		ZPLANE,
		UPLANE,
		VPLANE,
		WPLANE,
		APLANE,
		BPLANE,
		CPLANE
	};

	enum MXP_USERLib
	{
		MXP_R2R = 0,
		MXP_Robotics,
		MXP_EDG,
	};

	enum MXP_CAM_STATUS
	{
		RETCAM_NO_ERROR = 0,
		RETCAM_ERROR_TABLEID,
		RETCAM_ERROR_INPUTPARAM,
		RETCAM_ERROR_DATASIZE,
		RETCAM_ERROR_INTERPOLATION,
		RETCAM_ERROR_MASTERPOS,
		RETCAM_ERROR_EXECUTIONMODE
	};

	enum EC_NETWORK_CMD
	{
		NET_STATE_INIT_CMD = 0x01,
		NET_STATE_PREOP_CMD = 0x02,
		NET_STATE_BOOT_CMD = 0x03,
		NET_STATE_SAFEOP_CMD = 0x04,
		NET_STATE_OP_CMD = 0x08
	};

	enum MXP_BUFFERMODE_ENUM
	{
		MXP_ABORTING = 0,
		MXP_BUFFERED = 1,
		MXP_BLENDING_LOW = 2,
		MXP_BLENDING_PREVIOUS = 3,
		MXP_BLENDING_NEXT = 4,
		MXP_BLENDING_HIGH = 5,
		MXP_SINGLE_BLOCK = 6
	};

	enum MXP_DIRECTION_ENUM
	{
		MXP_NONE_DIRECTION = 0,
		MXP_POSITIVE_DIRECTION = 1,
		MXP_SHORTEST_WAY = 2,
		MXP_NEGATIVE_DIRECTION = 3,
		MXP_CURRENT_DIRECTION = 4
	};

	enum MXP_SOURCE_ENUM
	{
		MXP_COMMANDEDVALUE = 1,
		MXP_SETVALUE = 2,
		MXP_ACTUALVALUE = 3
	};

	enum MXP_PATHCHOICE_ENUM
	{
		MXP_CLOCKWISE = 0,
		MXP_COUNTERCLOCKWISE = 1
	};

	enum MXP_STARTMODE_ENUM
	{
		MXP_ABSOLUTE = 0,
		MXP_RELATIVE = 1,
		MXP_RAMPIN = 2
	};

	enum ESequenceCmdType
	{
		e_AbsMove = 0,
		e_RelativeMove,
		e_Dwell,
		e_IO,
	};

	enum MXP_SequenceBufferState
	{
		SequenceBuffer_Null = 0,
		SequenceBuffer_Runing,
		SequenceBuffer_Complete,
		SequenceBuffer_Fail,
	};

	enum MXP_SequenceMoveMode
	{
		e_Absolute = 0,
		e_Relative,
	};

	enum EAxisSequenceData
	{
		Axis_MoveType = 0,
		Axis_AxisGroupNo,
		Axis_nCmdType,
		Axis_fPos,
		Axis_fVel,
		Axis_fAcc,
		Axis_fDec,
		Axis_fJerk,
		Axis_fDwellTime,
		Axis_nIOSlaveNo,
		Axis_nIOBitNo,
		Axis_bIOBitSet,
		Axis_nDirection,
		Axis_nBufferMode,
		Axis_StepName,
		Axis_End,
	};

	enum EGroupSequenceData
	{
		Group_MoveType = 0,
		Group_AxisGroupNo,
		Group_nCmdType,
		Group_fPos,
		Group_fVel,
		Group_fAcc,
		Group_fDec,
		Group_fJerk,
		Group_fDwellTime,
		Group_nIOSlaveNo,
		Group_nIOBitNo,
		Group_bIOBitSet,
		Group_nBufferMode,
		Group_StepName,
		Group_End,
	};

	enum eFeedBackSensorAddress
	{
		e_FBStart = 5000,
		e_FBAxisNo = 5000,
		e_FBCommand,
		e_FBSensorSlaveNo,
		e_FBSensorPos,
		e_FBSensorSize,
		e_FBSensorTarget = 5008,
		e_FBSensorSetRange = 5012,
		e_FBSensorUnit = 5016,
		e_FBSensorOverrideDist = 5020,

		e_FBSensorTargetVel = 5024,
		e_FBSensorVaildFlag = 5028,
		e_FBSensorTargetCalDist = 5032,

		e_FBRunState = 5040,
		e_FBErrorCode = 5042,
		e_FBSensorDist1 = 5080,
		e_FBSensorDist2 = 5084,
		e_FBSensorVel1 = 5088,
		e_FBSensorVel2 = 5092,

		e_FBEnd = 5399,
		e_PLCBlockMax = 5500,
	};
		
	enum eFBRunState
	{
		e_Ready = 0,
		e_Run,
		e_Complete,
		e_Fail,
	};

	enum ePLCFBRunMode
	{
		ePLCFBRunMode_Null = 0,
		ePLCFBRunMode_Run,
		ePLCFBRunMode_Stop,
		ePLCFBRunMode_Reset,
	};

	enum eSpinAddress
	{
		eSpinAddress_PLCBlockMax = 5000,
		eSpinAddress_SpinRecipeTable = 5008,
		OriginMoveWaitTime = 5328,
		OriginMoveVel = 5332,
		eSpinAddress_FBStart = 5340,
		eSpinAddress_FBAxisNo = 5340,
		eSpinAddress_FBCommand,
		eSpinAddress_FBRecipeCount,
		eSpinAddress_FBRunState = 5346,
		eSpinAddress_FBRunStep = 5347,
		eSpinAddress_FBErrorCode = 5348,
		eSpinAddress_FBEnd = 5509,
	};

#pragma endregion MXP_ENUM_DEFINE

#pragma region

public:
	struct strExAPIAxisParam 
	{
		float fOverrideSetValue;
		float fDefaultAcc; //Acc== Dec
		float fDefaultJerk;
		bool bSequenceOverrideFlag;
	};

	struct MXP_MULTIAXISCOUPLE_SINGLEAXISGAIN
	{
		::MXP_MULTIAXISCOUPLE_SINGLEAXISGAIN stData;
	};

	struct MXP_MULTIAXISCOUPLE_CCCGAIN
	{
		::MXP_MULTIAXISCOUPLE_CCCGAIN stData;
	};

	struct MXP_FILE_TABLE_ARRAY_IN
	{
		::MXP_FILE_TABLE_ARRAY_IN stData;
	};

	struct MXP_PROFILE_TABLE_ARRAY_IN
	{
		::MXP_PROFILE_TABLE_ARRAY_IN stData;
	};

	struct MXP_IO_TABLE_ARRAY_IN
	{
		::MXP_IO_TABLE_ARRAY_IN stData;
	};

	struct MXP_CAMTABLE_REF
	{
		::MXP_CAMTABLE_REF stData;
	};

	struct PORT_STATE
	{
		MXP_PORT_STATE Port1State;
		MXP_PORT_STATE Port2State;
		MXP_PORT_STATE Port3State;
		MXP_PORT_STATE Port4State;
	};

	struct FBPROCESS_CHECK
	{
		eFBRunState nRunState;
		UINT16	ErrorID;  
		UINT8 nRunStep;
	};

	struct PROCESS_CHECK
	{
		bool Busy;
		bool Done;
		bool ErrorOn;
		UINT16 ErrorID;
	};

	struct READ_CAMTABLE_REPLY
	{
		bool Busy;
		bool Done;
		bool ErrorOn;
		UINT16 ErrorID;
		UINT16 TableRowCount;
		MXP_CAMTABLE_REF CamDataArray[400];
	};

	struct READ_CAMSLAVEPOSITION_REPLY
	{
		bool Busy;
		bool Done;
		bool ErrorOn;
		UINT16 ErrorID;
		FLOAT SlavePos;
	};

	struct AXIS_ERROR
	{
		UINT16 MXPError;
		UINT16 DriveError;
	};

	struct READ_ETParameterReply
	{
		bool Busy;
		bool Done;
		bool ErrorOn;
		UINT16 ErrorID;
		UINT8 *ReadData;
	};

	struct AccDecToAccTime_Reply
	{
		FLOAT AccDecBuildUp;
		FLOAT LimitAccDec;
		FLOAT AccDecRampDown;
	};

	struct AccTimeToAccDec_Reply
	{
		FLOAT Accdec;
		FLOAT Jerk;
	};

	struct TouchProbeReadPos_Reply
	{
		FLOAT EdgePositivePosition;
		FLOAT EdgeNegativePosition;
	};

	struct strTouchprobeState
	{
		bool TouchprobeUsing;
		bool TouchprobeRisingEdgeSave;
		bool TouchprobeFallingEdgeSave;
		bool TouchprobeRisingPositionUpdate;
		bool TouchprobeFallingPositionUpdate;
	};

	struct MXP_AxisStateBit
	{
		bool ErrorStop;
		bool Disable;
		bool Stopping;
		bool Standstill;
		bool DiscreteMotion;
		bool ContinuousMotion;
		bool SynchronizedMotion;
		bool Homming;
		bool ConstantVelocity;
		bool Accelerating;
		bool Decelerating;
		bool DirectionPositive;
		bool DirectionNegative;
		bool HomeAbsSwitch;
		bool HWLimitSwitchPosEvent;
		bool HWLimitSwitchNegEvent;
		bool ReadyForPowerOn;
		bool PowerOn;
		bool IsHomed;
		bool AxisWarning;
	};

	struct GROUP_POS
	{
		FLOAT nX;
		FLOAT nY;
		FLOAT nZ;
		FLOAT nU;
		FLOAT nV;
		FLOAT nW;
		FLOAT nA;
		FLOAT nB;
		FLOAT nC;
	};

	struct SEQUENCEMOVEPROCESS_CHECK
	{
		bool Busy;
		bool Done;
		bool ErrorOn;
		UINT16 ErrorID;
	};

	struct StSequenceMoveStep
	{
		float fPos;
		float fVel;
	};

	struct StSequenceMove
	{
		ESequenceCmdType nCmdType;
		float fPos;
		float fVel;
		float fAcc;
		float fDec;
		float fJerk;
		float fDwellTime;
		UINT16 nIOSlaveNo;
		UINT16 nIOBitNo;
		bool bIOBitSet;
		MXP_DIRECTION_ENUM nDirection;
		MXP_BUFFERMODE_ENUM nBufferMode;
		string strStepName;
	};

	struct StGroupSequenceMove
	{
		ESequenceCmdType nCmdType;
		float fXPos;
		float fYPos;
		float fZPos;
		float fUPos;
		float fVPos;
		float fWPos;
		float fAPos;
		float fBPos;
		float fCPos;
		float fVel;
		float fAcc;
		float fDec;
		float fJerk;
		float fDwellTime;
		UINT16 nIOSlaveNo;
		UINT16 nIOBitNo;
		bool bIOBitSet;
		MXP_BUFFERMODE_ENUM nBufferMode;
		string strStepName;
	};

	struct AxisSequenceData
	{
		bool bRunFlag;
		std::vector<StSequenceMove> nData;
		UINT16 nSaveNum;
		UINT16 nReadNum;
		UINT16 nCurBlock;
		UINT16 nSendCount;
		UINT16 nStartStep;
		UINT16 nEndStep;
		SEQUENCEMOVEPROCESS_CHECK nState;
	};

	struct GroupSequenceData
	{
		bool bRunFlag;
		std::vector<StGroupSequenceMove> nData;
		UINT16 nSaveNum;
		UINT16 nReadNum;
		UINT16 nCurBlock;
		UINT16 nSendCount;
		UINT16 nStartStep;
		UINT16 nEndStep;
		SEQUENCEMOVEPROCESS_CHECK nState;
	};

#pragma endregion MXP Structure Define

#pragma region

public:

	/// <summary>
	/// <para>Int32ernal initialization to use the MXP.</para>
	/// <br>The result of Calling the function is returned. </br>
	/// </summary>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   SYS_Init();

	/// <summary>
	/// <para>Int32ernal initialization to use the MXP.</para>
	/// <br>The result of Calling the function is returned. </br>
	/// </summary>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   SYS_Init_Developer();

	/// <summary>
	/// <para>This function runs the MXP.</para>
	/// <br>The result of calling the function is returned.</br>
	/// </summary>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   SYS_Run();

	/// <summary>
	/// <para>This function stops the MXP.</para>
	/// <br>The result of calling the function is returned.</br>
	/// </summary>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   SYS_Stop();;

	/// <summary>
	/// This function is called to terminate the connection with the MXP.<para />
	/// The result of calling the function is returned.<para />
	/// </summary>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   SYS_Destroy();

	/// <summary>
	/// This function is called to terminate the connection with the MXP.<para />
	/// The result of calling the function is returned.<para />
	/// </summary>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   SYS_Destroy_Developer();

	/// <summary>
	/// This function returns the current status of the MXP State.<para />
	/// Before Calling this function. initialize the MXP by calling SYS_Init.<para />
	/// The result of calling the function is returned.<para />
	/// </summary>
	/// <param name="nStatus">return the current status of the MXP.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   SYS_GetStatus(KERNEL_STATUS &nStatus);

	/// <summary>
	/// This function check the MXP license.<para/>
	/// The result of calling the function is returned.<para />
	/// </summary>
	/// <param name="nFunction">Set MXP_USERLib value</param>
	/// <param name="bStatus">Return the licence Status(valid : true, invalid : false)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   SYS_CheckLicense(MXP_USERLib nFunction, bool &bStatus);

	/// <summary>
	/// <para>This function returns the network state of the current EtherCAT.</para>
	/// <br>the result of calling the function is returned.</br>
	/// </summary>
	/// <param name="nStatus">return the current EtherCAT master.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_GetMasterOnlineMode(MXP_ONLINE_STATE &nStatus);

	/// <summary>
	/// <para>This function returnes the MXP Run Mode</para>
	/// <br>The result of calling the function is returned.</br>
	/// </summary>
	/// <param name="bSimulation">return MXP simulation Mode.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   SYS_GetMasterSimulationMode(bool &bSimulation);

	/// <summary>
	/// When MXP state is running, Set the mode of master
	/// The result of calling the function is returned.
	/// </summary>
	/// <param name="nMode">>Set the mode(0:Init, 1:ProOP, 2:Boot, 4:SafeOP, 8:OP)</param>
	/// <returns></returns>
	/// <remarks></remarks>
	MXP_FUNCTION_STATUS   ECAT_SetMasterOnlineMode(EC_NETWORK_CMD nMode);

	/// <summary>
	///	When MXP state is running, Set the mode of slave
	///	The result of calling the function is returned.
	///	</summary>
	///	<param name="nSlaveNo">Set the slave number</param>
	///	<param name="nMode">Set the mode(0:Init, 1:ProOP, 2:Boot, 4:SafeOP, 8:OP)</param>
	///	<returns></returns>
	///	<remarks></remarks>
	MXP_FUNCTION_STATUS   ECAT_SetSlaveOnlineMode(UINT32 nSlaveNo, EC_NETWORK_CMD nMode);

	///''' <summary>
	///	This function Restart the MXP
	///	The result of calling the function is returned
	///	</summary>
	///	<returns></returns>
	///	<remarks></remarks> 
	MXP_FUNCTION_STATUS   SYS_Reset();
#pragma endregion System

#pragma region

public:

	/// <summary>
	/// This function obtains slave number using the NodeID<para />
	/// The result of Calling the function is returned.
	/// </summary>
	/// <param name="nNodeID">NodeID of the EtherCAT module(0~255)</param>
	/// <param name="nSlaveNo">Return slave number of the EtherCAT module(0~127)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_GetSlaveNoFromNodeId(UINT32 nNodeID, UINT32 &nSlaveNo); 

	/// <summary>
	/// This function obtains axis number using the NodeID. <para />
	/// The result of calling the function is returned.
	/// </summary>
	/// <param name="nNodeID">NodeID of the EtherCAT module(0~255)</param>
	/// <param name="nAxisNo">Return axis number of the EtherCAT module(0~127)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_GetAxisNoFromNodeId(UINT32 nNodeID, UINT32 &nAxisNo);

	/// <summary>
	/// Return the network state of an individual slave. <para />
	/// The result of calling the function is returned.
	/// </summary>
	/// <param name="nSlaveNO">Slave number of the EtherCAT module(0~127)</param>
	/// <param name="nStatus">Return the EtherCAT network state(0~0x18). refer MXP_ONLINE_STATE</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_GetSlaveCurrentState(UINT32 nSlaveNo, MXP_ONLINE_STATE &nStatus);

	/// <summary>
	/// Return the number of slave devices currently connected.<para/>
	/// The result of calling the function is returned.
	/// </summary>
	/// <param name="nSlaveCount">Return the number of slave devices currently connected</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_GetSlaveCount(UINT32 &nSlaveCount);

	/// <summary>
	/// The result of calling the function is returned.
	/// </summary>
	/// <param name="nAxisCount">Return the number of axes currently connected</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_GetAxisCount(UINT32 &nAxisCount);

	/// <summary>
	/// Return the type of a slave node.<para/>
	/// The result of calling the function is returned.
	/// </summary>
	/// <param name="nSlaveNo">Number of the slave module</param>
	/// <param name="nType">Return the node type(1 : Servo Drive, 2 : IO)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_GetSlaveType(UINT32 nSlaveNo, MXP_NODE_TYPE &nType);

	/// <summary>
	/// This function returns the hardware network connect state of the EtherCAT master <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="bConnectState">Return the PortState(True:Connect)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_GetMasterEtherCATLineStatus(bool &bConnectState);

	///<summary>        
	/// This function returns the hardware network connect state of the EtherCAT slave <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Set the Slave number</param>
	/// <param name="stPortState">Return the PortState(port1 ~ port4)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_GetSlaveEtherCATLineStatus(UINT32 nSlaveNo, PORT_STATE &stPortState);

	/// <summary>
	/// This function returns the EtherCAT network alarm history<para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="arrAlarm">Return alarm history</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   SYS_GetEtherCATHistoryAlarm(INT32 *arrAlarm);

	/// <summary>
	/// Return the Slave name of entered slave number<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Set the Slave number</param>
	/// <param name="strName">Return the slave name</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_GetSlaveName(UINT32 nSlaveNo, char *strName);

	/// <summary>
	/// Check the slave communication ready status<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Set the Slave number</param>
	/// <param name="bReady">Return the value which check the slave communication status</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_SlaveReadyCheck(UINT32 nSlaveNo, bool &bReady);

	/// <summary>
	/// Check the All slaves communication ready status<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="bReady">Return the value which check the All slaves communication status</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_ReadyCheck(bool &bReady);

	MXP_FUNCTION_STATUS   ECAT_PDOMappingCheck(UINT32 nAxisNo, MXP_PDO_DIRECTION nDirection, UINT16 nPDOIndex, bool &bCheckState);
#pragma endregion Slave

#pragma region

public:

	/// <summary>
	///  This function writes UINT8 array value to the selected slave <para />
	///  The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Number of the slave</param>
	/// <param name="nOffset">Start address of array to write</param>
	/// <param name="nSize">Size of the write UINT8 array</param>
	/// <param name="arrData">UINT8 array value to write</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   IO_Write(UINT32 nSlaveNo, UINT16 nOffset, UINT16 nSize, UINT8 *arrData);

	/// <summary>
	/// This function writes bit value to the selected slave <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Number of the slave</param>
	/// <param name="nOffset">Start address of bit to write</param>
	/// <param name="nBitOffset">Bit number to write(0 ~ 7)</param>
	/// <param name="bData">Bit value to write</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   IO_WriteBit(UINT32 nSlaveNo, UINT16 nOffset, UINT8 nBitOffset, bool bData);

	/// <summary>
	/// This function writes byte value to the selected slave <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Number of the slave</param>
	/// <param name="nOffset">Start address of UINT8 to write</param>
	/// <param name="nData">UINT8 value to write</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   IO_WriteByte(UINT32 nSlaveNo, UINT16 nOffset, UINT8 nData);

	/// <summary>
	/// This function writes word value to selected slave <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Number of slave</param>
	/// <param name="nOffset">Start address of word to write</param>
	/// <param name="nData">word value to write</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   IO_WriteWord(UINT32 nSlaveNo, UINT16 nOffset, UINT16 nData);

	/// <summary>
	/// This function writes Dword value to selected slave <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Number of slave</param>
	/// <param name="nOffset">Start address of Dword to write</param>
	/// <param name="nData">Dword value to write</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   IO_WriteDword(UINT32 nSlaveNo, UINT16 nOffset, UINT32 nData);

	/// <summary>
	/// This function reads UINT8 array value to selected slave <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Number of slave</param>
	/// <param name="nIoType">MXP_IO_Type(IO_IN or IO_OUT)</param>
	/// <param name="nOffset">Start address of UINT8 array to read</param>
	/// <param name="nSize">Size of the UINT8 array to read</param>
	/// <param name="arrData">UINT8 array value to read</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   IO_Read(UINT32 nSlaveNo, MXP_IO_Type nIoType, UINT16 nOffset, UINT16 nSize, UINT8 *arrData);

	/// <summary>
	/// This function reads bit value to selected slave <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Number of slave</param>
	/// <param name="nIoType">MXP_IO_Type(IO_IN or IO_OUT)</param>
	/// <param name="nOffset">Start address of  bit to read</param>
	/// <param name="nBitOffset">Bit number of address to read</param>
	/// <param name="bData">Return bit value</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   IO_ReadBit(UINT32 nSlaveNo, MXP_IO_Type nIoType, UINT16 nOffset, UINT8 nBitOffset, bool &bData);

	/// <summary>
	/// This function read UINT8 value to selected slave <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Number of slave</param>
	/// <param name="nIoType">MXP_IO_Type(IO_IN or IO_OUT)</param>
	/// <param name="nOffset">Start address of UINT8 to read</param>
	/// <param name="nData">Return UINT8 value</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   IO_ReadByte(UINT32 nSlaveNo, MXP_IO_Type nIoType, UINT16 nOffset, UINT8 &nData);

	/// <summary>
	/// This function read word value to selected slave <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Number of slave</param>
	/// <param name="nIoType">MXP_IO_Type(IO_IN or IO_OUT)</param>
	/// <param name="nOffset">Start address of word to read</param>
	/// <param name="nData">Return word value</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   IO_ReadWord(UINT32 nSlaveNo, MXP_IO_Type nIoType, UINT16 nOffset, UINT16 &nData);

	/// <summary>
	/// This function read Dword value to selected slave <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">number of slave</param>
	/// <param name="nIoType">MXP_IO_Type(IO_IN or IO_OUT)</param>
	/// <param name="nOffset">Start address of Dword to read</param>
	/// <param name="nData">Return Dword value</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   IO_ReadDWord(UINT32 nSlaveNo, MXP_IO_Type nIoType, UINT16 nOffset, UINT32 &nData);

private:	
	MXP_FUNCTION_STATUS   MC_IO_Out_Read(UINT32 nSlaveNo, UINT16 &nSize, UINT8 * arrData);
	MXP_FUNCTION_STATUS   MC_IO_Out_Read_BIT(UINT32 nSlaveNo, UINT16 nBitNo, bool &bData);
	MXP_FUNCTION_STATUS   MC_IO_In_Read(UINT32 nSlaveNo, UINT16 &nSize, UINT8 * arrData);
	MXP_FUNCTION_STATUS   MC_IO_In_Read_BIT(UINT32 nSlaveNo, UINT16 nBitNo, bool &bData);

#pragma endregion IO

#pragma region
public:

	/// <summary>
	/// Control the servo on/off state of the specified axis <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <param name="bEnable">True : Servo On, False : Servo Off</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_Power(UINT32 nAxisNo, bool bEnable);

	/// <summary>
	///  Reset all axis-related errors and Change the state of the axis from ErrorStop to Standstill <para />
	///  The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_Reset(UINT32 nAxisNo);

	/// <summary>
	/// Command the servo axis to perform the homing motion <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_Home(UINT32 nAxisNo);

	/// <summary>
	/// When selected axis is in the Standstill state, Reset the actual position to set Position. <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <param name="fPosition">Set position value</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_SetPosition(UINT32 nAxisNo, FLOAT fPosition);

	/// <summary>
	/// Stop the ongoing motion and Transfer the axis to the Standstill state <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <param name="fDec">Set Decelation value</param>
	/// <param name="fJerk">Set Jerk value</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_Halt(UINT32 nAxisNo, FLOAT fDec, FLOAT fJerk);

	/// <summary>
	///  Stop the ongoing motion and Transfer the axis to the Stopping state <para />
	///  To perform another motion command, the execute parameter must be changed from true to false<para/>
	///  The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <param name="bExecute">Execute the command with rising edge</param>
	/// <param name="fDec">Set Decelation value</param>
	/// <param name="fJerk">Set Jerk value</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_Stop(UINT32 nAxisNo, bool bExecute, FLOAT fDec, FLOAT fJerk);

	/// <summary>
	/// Move the axis from the actual position to a specified absolute position <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <param name="fPosition">Set Position value</param>
	/// <param name="fVel">Set Velocity value</param>
	/// <param name="fAcc">Set Acceleration value</param>
	/// <param name="fDec">Set Deceleration value</param>
	/// <param name="fJerk">Set Jerk value</param>
	/// <param name="nDirection">Set Direction of movement</param>
	/// <param name="nBufferMode">Set Buffer mode </param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MoveAbsolute(UINT32 nAxisNo, FLOAT fPosition, FLOAT fVel, FLOAT fAcc, FLOAT fDec, FLOAT fJerk, MXP_DIRECTION_ENUM nDirection, MXP_BUFFERMODE_ENUM nBufferMode);

	/// <summary>
	/// Setting the override position for the entire interval performs <para/>
	/// the corresponding velocity override function.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <param name="fTargetPosition">Set Target Position value</param>
	/// <param name="fTargetVel">Set Target Velocity value</param>
	/// <param name="nOverrideCount">Set Override Count</param>
	/// <param name="fOverridePosition">Set Override Position</param>
	/// <param name="fOverrideVelocity">Set Override Velocity</param>
	/// <param name="fAcc">Set Acceleration value</param>
	/// <param name="fDec">Set Deceleration value</param>
	/// <param name="fJerk">Set Jerk value</param>
	/// <param name="nBufferMode">Set Buffer mode</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MoveAbsolute_VelocityOverride(UINT32 nAxisNo, FLOAT fTargetPosition, FLOAT fTargetVel, INT16 nOverrideCount, FLOAT *fOverridePosition, FLOAT *fOverrideVelocity, FLOAT fAcc, FLOAT fDec, FLOAT fJerk, MXP_BUFFERMODE_ENUM nBufferMode);

	/// <summary>
	/// Move the axis from the actual position by the distance set in the distance input <para />
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <param name="fDistance">Set Distance</param>
	/// <param name="fVel">Set Velocity value</param>
	/// <param name="fAcc">Set Acceleration value</param>
	/// <param name="fDec">Set Deceleration value</param>
	/// <param name="fJerk">Set Jerk value</param>
	/// <param name="nBufferMode">Set Buffer mode</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MoveRelative(UINT32 nAxisNo, FLOAT fDistance, FLOAT fVel, FLOAT fAcc, FLOAT fDec, FLOAT fJerk, MXP_BUFFERMODE_ENUM nBufferMode);

	/// <summary>
	/// Move the axis at the velocity specified in the velocity parameter <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <param name="fVel">Set Velocity value</param>
	/// <param name="fAcc">Set Acceleration value </param>
	/// <param name="fDec">Set Deceleration value</param>
	/// <param name="fJerk">Set Jerk value</param>
	/// <param name="nDirection">Set Direction of movement</param>
	/// <param name="nBufferMode">Set Buffer mode</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MoveVelocity(UINT32 nAxisNo, FLOAT fVel, FLOAT fAcc, FLOAT fDec, FLOAT fJerk, MXP_DIRECTION_ENUM nDirection, MXP_BUFFERMODE_ENUM nBufferMode);

	/// <summary>
	/// Set the velocity rate for all function<para/>
	/// The factor can be set as a real number from 0 to 1(0 ~ 100%) <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <param name="fVelFactor">Set the velocity scale factor as a multiplier of a real number</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_SetOverride(UINT32 nAxisNo, FLOAT fVelFactor);

private :
	MXP_FUNCTION_STATUS   AX_BufferedIO(UINT32 nAxisNo, UINT16 nSlaveNo, UINT16 nBitPos, bool bBitValue);
	MXP_FUNCTION_STATUS   AX_Dwell(UINT32 nAxisNo, float fTime);

#pragma endregion Single Motion

#pragma region
public:

	/// <summary>
	/// Command a controlled motion on an electronic gear <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nMasterAxis">Master Axis number(0 ~ 127)</param>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <param name="nRatioNumerator">Set Gear ratio of the slave axis</param>
	/// <param name="nRatioDenominator">Set Gear ratio of the master axis</param>
	/// <param name="nMasterValueSource">Select master data for synchronization <para/>
	/// <param name="fAcc">Set Acceleration value</param>
	/// <param name="fDec">Set Deceleration value</param>
	/// <param name="fJerk">Set Jerk value</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_GearIn(UINT32 nMasterAxis, UINT32 nSlaveAxis, INT32 nRationNumerator,UINT32 nRatioDenominator, MXP_SOURCE_ENUM nMasterValueSource, FLOAT	fAcc, FLOAT fDec, FLOAT fJerk);

	/// <summary>
	/// Return a Status and ErrorID of the slave which is executed GearIn command <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <param name="nStatus">Return the GearIn status.(type : MXPEasyClass.PROCESS_CHECK)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_GearInMonitor(UINT32 nSlaveAxis, PROCESS_CHECK &stStatus);

	/// <summary>
	/// Disengage a slave axis from the master axis <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_GearOut(UINT32 nSlaveAxis);
#pragma endregion GearIn

#pragma region
public:

	/// <summary>
	/// Command a controlled motion on an electronic gear <para/>
	/// Unlike AX_GearIn, Set the position in which the slave and master axes are synchronized <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nMasterAxis">Master Axis number(0 ~ 127)</param>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <param name="nRatioNumerator">Set Gear ratio of the slave axis</param>
	/// <param name="nRatioDenominator">Set Gear ratio of the master axis</param>
	/// <param name="nMasterValueSource">Select master data for synchronizition</param>
	/// <param name="fMasterSyncPos">Set the position where synchronization of the master axis is completed</param>
	/// <param name="fSlaveSyncPos">Set the position where synchronization of the slave axis is completed</param>
	/// <param name="fMasterStartDistance">Set the position of the master axis where the master axis starts synchronizing with the slave axis</param>
	/// <param name="fVel">Set Maximum Velocity of the slave axis until the synchronization is complete</param>
	/// <param name="fAcc">Set Maximum Acceleration of the slave axis until the synchronization is complete</param>
	/// <param name="fDec">Set Maximum Deceleration of the slave axis until the synchronization is complete</param>
	/// <param name="fJerk">Set Maximum Jerk of the slave axis until the synchronization is complete</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_GearInPos(UINT32 nMasterAxis, UINT32 nSlaveAxis, INT32 nRatioNumerator, UINT32 nRatioDenominator, MXP_SOURCE_ENUM nMasterValueSource, FLOAT fMasterSycPos, FLOAT fSlaveSycPos, FLOAT fMasterStartDistance, FLOAT fVel, FLOAT fAcc, FLOAT fDec, FLOAT fJerk);

	/// <summary>
	/// Return a Status and ErrorID of the slave which is executed GearInPos command <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <param name="stStatus">Return the GearInPos status.(type : MXPEasyClass.PROCESS_CHECK)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_GearInPosMonitor(UINT32 nSlaveAxis, PROCESS_CHECK &stStatus);

	/// <summary>
	/// Disengage a slave axis from the master axis <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_GearInPosOut(UINT32 nSlaveAxis);
#pragma endregion GearInPos

#pragma region
public:

	/// <summary>
	/// Read the CAM table to use the electronic CAM function <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nMasterAxis">Master Axis number(0 ~ 127)</param>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <param name="nCamTableID">Set the number of the table to be read in</param>
	/// <param name="bPeriodic">Specify whether to execute the table periodically. (0 : NonPeriodic, 1 : Periodic)<para/>
	/// false : NonPeriodic</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamTableSelect(UINT32 nMasterAxis, UINT32 nSlaveAxis, UINT16 nCamTableID, bool bPeriodic);

	/// <summary>
	/// Write the CAM table data <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nCamTableID">Set the table number to write</param>
	/// <param name="bExecute">Execute the command with rising edge</param>
	/// <param name="nTableRowCount">Set the number of table rows to write</param>
	/// <param name="arrCamData">CAM table value to write</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamWriteTableRequest(UINT16 nCamTableID, bool bExecute, UINT16 nTableRowCount, MXP_CAMTABLE_REF *arrCamData);

	/// <summary>
	/// Return a Status and ErrorID of the CAM table which is executed AX_CamWriteTableRequest command <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nCamTableID">CamTableID(0~63)</param>
	/// <param name="stStatus">Return the CAMWriteTable status.(type : MXPEasyClass.PROCESS_CHECK)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamWriteTableReply(UINT16 nCamTableID, PROCESS_CHECK &stStatus);

	/// <summary>
	/// Execute the AX_CamWriteTableRequest and AX_CamWriteTableReply Command<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nCamTableID">CamTableID(0~63)</param>
	/// <param name="nTableRowCount">Set the number of table rows to write</param>
	/// <param name="arrCamData">CAM table value to write</param>
	/// <param name="nWaitTime">Set the wait time(ms) before AX_CamWriteTableReply Command</param>
	/// <param name="stStatus">Return the CAMWriteTable status.(type : MXPEasyClass.PROCESS_CHECK)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamWriteTable(UINT16 nCamTableID, UINT16 nTableRowCount, MXP_CAMTABLE_REF *arrCamData, INT32 nWaitTime, PROCESS_CHECK &stStatus);

	/// <summary>
	/// Read the existing CAM table file <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nCamTableID">Set the table number to read</param>
	/// <param name="bExecute">Execute the command with rising edge</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamReadTableRequest(UINT16 nCamTableID, bool bExecute);

	/// <summary>
	/// Return a Status and ErrorID of the CAM table which is executed AX_CamReadTableRequest command <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nCamTableID">Set the table number to read</param>
	/// <param name="stStatus">Return the CAMReadTable status(Type:READ_CAMTABLE_REPLY)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamReadTableReply(UINT16 nCamTableID, READ_CAMTABLE_REPLY &stStatus);

	/// <summary>
	/// Execute the AX_CamReadTableRequest and AX_CamReadTableReply Command<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nCamTableID">Set the table number to read</param>
	/// <param name="nWaitTime">Set the wait time(ms) before AX_CamReadTableReply Command</param>
	/// <param name="stStatus">Return the CAMReadTable status(Type:READ_CAMTABLE_REPLY)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamReadTable(UINT16 nCamTableID, INT32 nWaitTime, READ_CAMTABLE_REPLY &stStatus);

	/// <summary>
	/// Engage the electronic CAM(CAM table must have been set by AX_CamTableSelect) <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nMasterAxis">Master Axis number(0 ~ 127)</param>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <param name="fMasterOffset">Set the offset for the table master position</param>
	/// <param name="fSlaveOffset">Set the offset for the table slave position</param>
	/// <param name="fMasterScaling">Set the scaling factor for the table master position</param>
	/// <param name="fSlaveScaling">Set the scaling factor for the table slave position </param>
	/// <param name="fMasterSyncPosition">Set the position where synchronization of the slave axis is completed</param>
	/// <param name="fMasterStartDistance">Set the master distance for the slave to start synchronizing with the master</param>
	/// <param name="nStartMode">Set the talbe position date type to start synchronizition</param>
	/// <param name="nMasterValSource">Select the source data of the master axis for synchronization</param>
	/// <param name="nCamTableID">Select the CAM table ID for synchronization</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamIn(UINT32 nMasterAxis, UINT32 nSlaveAxis, FLOAT fMasterOffset, FLOAT fSlaveOffset, FLOAT fMasterScaling, FLOAT fSlaveScaling, FLOAT fMasterSyncPosition, FLOAT fMasterStartDistance, MXP_STARTMODE_ENUM nStartMode, MXP_SOURCE_ENUM nMasterValSource, UINT16 nCamTableID); 

	/// <summary>
	/// Sets the electronic CAM <para/>
	/// behavior to the master of an external Encoder or IO signal.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nExternalENC_SlaveNo">Sets the external Encoder input Slave number.</param>
	/// <param name="nExternalENC_SlavePos">Sets the external Encoder input address.</param>
	/// <param name="nExternalENC_SlaveSize">Sets the external Encoder input size.</param>
	/// <param name="nExternalENC_Resolution">Sets the external Encoder resolution.</param>
	/// <param name="fExternalENC_PulseToCamMasterUnit">Enter the external Encoder pulse required for user input unit 1.</param>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <param name="fMasterOffset">Sets the offset of the master axis.</param>
	/// <param name="fSlaveOffset">Sets offset for slave axis</param>
	/// <param name="fMasterScaling">Set the scaling of the master axis.</param>
	/// <param name="fSlaveScaling">Set the scaling of the slave axis.</param>
	/// <param name="fMasterSyncPosition">Sets the position of the master axis <para/>
	/// where the slave axis will complete synchronization with the master axis.</param>
	/// <param name="fMasterStartDistance">Sets the Distance of the master axis <para/>
	/// to start synchronizing with the master axis.</param>
	/// <param name="nStartMode">Sets the Start Mode</param>
	/// <param name="nMasterValSource">Select the data of the master axis to be synchronized.</param>
	/// <param name="nCamTableID">Select the table to use for Cam operation. (0~63)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamIn_MasterIO(UINT32 nExternalENC_SlaveNo, UINT32 nExternalENC_SlavePos, UINT8 nExternalENC_SlaveSize, UINT32 nExternalENC_Resolution, float fExternalENC_PulseToCamMasterUnit, UINT32 nSlaveAxis, FLOAT fMasterOffset, FLOAT fSlaveOffset, FLOAT fMasterScaling, FLOAT fSlaveScaling, FLOAT fMasterSyncPosition, FLOAT fMasterStartDistance, MXP_STARTMODE_ENUM nStartMode, MXP_SOURCE_ENUM nMasterValSource, UINT16 nCamTableID); 

	/// <summary>
	/// Return a Status and ErrorID of the CAM table which is executed AX_CamIn command <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <param name="stStatus">Return the Status to check Cam In(Type:PROCESS_CHECK)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamInMonitor(UINT32 nSlaveAxis, PROCESS_CHECK &stStatus);

	/// <summary>
	/// Disengage a slave axis from the master axis <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamOut(UINT32 nSlaveAxis);

	/// <summary>
	/// A CAM plate coupling can be scaled with AX_CamScaling <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nMasterAxis">Master Axis number(0 ~ 127)</param>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <param name="nActivationMode">Specify the scaling time and position</param>
	/// <param name="fActivationPos">Master position at which a cam plate is scaled, dapending on the ActivationMode</param>
	/// <param name="fMasterScaling">Scaling of the master position of the cam plate</param>
	/// <param name="fSlaveScaling">Scaling of the slave position of the cam plate</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_Camscaling(UINT32 nMasterAxis, UINT32 nSlaveAxis, MXP_ACTIVATIONMODE nActivationMode, FLOAT fActivationPos, FLOAT fMasterscaling, FLOAT fSlavescaling);

	/// <summary>
	/// Achieve an offset of the slave axis with respect to the master axis <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nMasterAxis">Master Axis number(0 ~ 127)</param>
	/// <param name="nSlaveAxis">Slave Axis number(0 ~ 127)</param>
	/// <param name="fPhaseShift">The calculated phase shift is transferred to the slave axis as the master axis position.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_Phasing(UINT32 nMasterAxis, UINT32 nSlaveAxis, FLOAT fPhaseShift);

	/// <summary>
	/// Determine the slave position at a certain poInt32 of a cam plate table <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nCamTableID">CAM table ID to request(0~63)</param>
	/// <param name="bExecute">Execute the command with rising edge</param>
	/// <param name="fMasterPos">Master position within the table for which the slave position is to be determined</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamReadSlavePositionRequest(UINT16 nCamTableID, bool bExecute, FLOAT fMasterPos);

	/// <summary>
	///  Return the slave position at a certain poInt32 of a cam plate table <para/>
	///  The result of calling the function is returned
	/// </summary>
	/// <param name="nCamTableID">CAM table ID to reply(0~63)</param>
	/// <param name="stStatus">Return the Cam slave position(Type:READ_CAMSLAVEPOSITION_REPLY)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamReadSlavePositionReply(UINT16 nCamTableID, READ_CAMSLAVEPOSITION_REPLY &stStatus);
	
	/// <summary>
	/// Execute the AX_CamReadSlavePositionRequest and AX_CamReadSlavePositionReply Command<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nCamTableID">CAM table ID(0~63)</param>
	/// <param name="fMasterPos">Master position within the table for which the slave position is to be determined</param>
	/// <param name="nWaitTime">Set the wait time(ms) before AX_CamReadSlavePositionReply Command</param>
	/// <param name="stStatus">Return the Cam slave position(Type:READ_CAMSLAVEPOSITION_REPLY)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_CamReadSlavePosition(UINT16 nCamTableID, FLOAT fMasterPos, INT32 nWaitTime, READ_CAMSLAVEPOSITION_REPLY &stStatus);
#pragma endregion CAM

#pragma region
private:
	MXP_PROFILE_MOVE_IN stProfileIn;
public:

	/// <summary>
	/// When driving the Recipe, convert the transfer table(Step,Position,Velocity) according to the Step to the table for the MXP and return it <para/>
	///  The result of calling the function is returned
	/// </summary>
	/// <param name="stUserTable">Sets the UserTable. It starts from 0</param>
	/// <param name="nUserTableCount">Set the total driving step of the user table</param>
	/// <param name="stMotionTable">Return the profile table value</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_GetProfileTable(MXP_FILE_TABLE_ARRAY_IN *stUserTable, UINT16 nUserTableCount, MXP_PROFILE_TABLE_ARRAY_IN *stMotionTable);

	/// <summary>
	///  It is a function that makes profile movement<para/>
	///  The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is issued(0~127)</param>
	/// <param name="nMotionTableCount">Set the profile table count</param>
	/// <param name="nIOTableCount">Set the IO table count</param>
	/// <param name="nRepeatCount">Set the repeat count for how many iterations motion</param>
	/// <param name="fStartDwell">Set the Waiting time before driving (msec)</param>
	/// <param name="fEndDwell">When setting the RepeatCount, Set the waiting time before return operation (msec).</param>
	/// <param name="nReverseMode">Set the reverse mode(1 : reverse mode)</param>
	/// <param name="stMotionTable">return the profile table value</param>
	/// <param name="stIOTable">return the IO table value</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_ProfileMove(UINT32 nAxisNo, UINT16 nMotionTableCount, UINT16 nIOTableCount, UINT16 nRepeatCount, FLOAT fStartDwell, FLOAT fEndDwell, UINT16 nReverseMode, MXP_PROFILE_TABLE_ARRAY_IN *stMotionTable, MXP_IO_TABLE_ARRAY_IN *stIOTable);

	/// <summary>
	///  It checks the operating status of the profile movement.<para/>
	///  The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is issued(0~127)</param>
	/// <param name="stStatus">Return the Profile move out value(Type:PROCESS_CHECK)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_ProfileMoveCheck(UINT32 nAxisNo, PROCESS_CHECK &stStatus); 

#pragma endregion ProfileMoveEx

#pragma region
private:
	UINT16 arrTouchprobeFunc[127];

	bool WordToBitArray(UINT16 nData, bool * bArray);
	bool ByteToBitArray(UINT8 nData,  bool * bArray);
	bool BitArrayToByte(bool *bArray, UINT8 &nByte);
public:
	/// <summary>
	/// Configure the touch probe function of the servo drive. 
	/// The touch probe function reads the position value of the servo drive when the specified sensor sends an input signal <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the axis on which to use the touch probe(0~127)</param>
	/// <param name="nMode">Select a trigger mode(Use MXP_TRIGEER_MODE)</param>
	/// <param name="nTriggerType">Select a trigger type(Use MXP_TRIGGER_TYPE)</param>
	/// <param name="nTriggerEdge">Select a trigger edge(Use MXP_TRIGGER_EDGE)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_TouchProbe1Set(UINT32 nAxisNo, MXP_TRIGGER_MODE nMode, MXP_TRIGGER_TYPE nTriggerType, MXP_TRIGGER_EDGE nTriggerEdge);

	/// <summary>
	/// Reset the touchprobe1.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the axis on which to use the touch probe(0~127)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_TouchProbe1SingleModeReSet(UINT32 nAxisNo);

	/// <summary>
	/// Configure the touch probe function of the servo drive. 
	/// The touch probe function reads the position value of the servo drive when the specified sensor sends an input signal <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the axis on which to use the touch probe(0~127)</param>
	/// <param name="nMode">Select a trigger mode(Use MXP_TRIGEER_MODE)</param>
	/// <param name="nTriggerType">Selcet a trigger type(Use MXP_TRIGGER_TYPE)</param>
	/// <param name="nTriggerEdge">Select a trigger edge(Use MXP_TRIGGER_EDGE)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_TouchProbe2Set(UINT32 nAxisNo, MXP_TRIGGER_MODE nMode, MXP_TRIGGER_TYPE nTriggerType, MXP_TRIGGER_EDGE nTriggerEdge);

	/// <summary>
	/// Reset the touchprobe2.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the axis on which to use the touch probe(0~127)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_TouchProbe2SingleModeReSet(UINT32 nAxisNo);

	/// <summary>
	/// Read the current status of the touch probe<para/>
	/// The result of calling of the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the axis on which to use the touch probe(0~127)</param>
	/// <param name="stState">Returns the state of the touch probe(Type : strTouchprobeState)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_TouchProbe1ReadState(UINT32 nAxisNo, strTouchprobeState &stState);

	/// <summary>
	/// Read the current status of the touch probe<para/>
	/// The result of calling of the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the axis on which to use the touch probe(0~127)</param>
	/// <param name="stState">Returns the state of the touch probe(Type : strTouchprobeState)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_TouchProbe2ReadState(UINT32 nAxisNo, strTouchprobeState &stState);

	/// <summary>
	/// Read the actual position obtained by the touch probe of the servo drive<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the axis on which to use the touch probe(0~127)</param>
	/// <param name="nTriggerEdge">Select an edge (Use MXP_TOUCHPROBE_EDGE_ENUM)</param>
	/// <param name="stPosition">Return the actual position of the touch probe(Use TouchProbeReadPos_Reply)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_TouchProbe1ReadPosition(UINT32 nAxisNo, MXP_TRIGGER_EDGE nTriggerEdge, TouchProbeReadPos_Reply &stPosition);

	/// <summary>
	/// Read the actual position obtained by the touch probe of the servo drive<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the axis on which to use the touch probe(0~127)</param>
	/// <param name="nTriggerEdge">Select an edge (Use MXP_TOUCHPROBE_EDGE_ENUM)</param>
	/// <param name="stPosition">Return the actual position of the touch probe(Use TouchProbeReadPos_Reply)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_TouchProbe2ReadPosition(UINT32 nAxisNo, MXP_TRIGGER_EDGE nTriggerEdge, TouchProbeReadPos_Reply &stPosition);

#pragma endregion TouchProbe

#pragma region
public:

	/// <summary>
	/// Move the axis at the torque specified in the torque parameter<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is issued(0~127)</param>
	/// <param name="fTorque">Set the torque to be commanded</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MoveDirectTorque(UINT32 nAxisNo, FLOAT fTorque);

	/// <summary>
	/// Move the axis at the velocity specified in the Velocity parameter<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is issued(0~127)</param>
	/// <param name="fVelocity">Set the movement velocity</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MoveDirectVelocity(UINT32 nAxisNo, FLOAT fVelocity);

	/// <summary>
	/// Move the axis at the position specified in the position parameter<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is issued(0~127)</param>
	/// <param name="fPosition">Set the movement velocity</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MoveDirectPosition(UINT32 nAxisNo, FLOAT fPosition);
#pragma endregion DirectSet

#pragma region
public:

	/// <summary>
	/// Return the actual position of the specified axis<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is issued(0~127)</param>
	/// <param name="fPosition">Return the position value</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_ReadActualPosition(UINT32 nAxisNo, FLOAT &fPosition);

	/// <summary>
	/// Return the actual position of the specified axis<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is issued(0~127)</param>
	/// <param name="fVelocity">Returns the velocity (position/time) value<para/>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_ReadActualVelocity(UINT32 nAxisNo, FLOAT &fVelocity);

	/// <summary>
	/// Returns the Following Error values of the axes commanded<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to read the Following Error value(0~127)</param>
	/// <param name="fFollowingErrorValue">Return the Following Error value.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_ReadFollowingError(UINT32 nAxisNo, FLOAT &fFollowingErrorValue);

	/// <summary>
	/// Return the actual torque of the specified axis<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is issued(0~127)</param>
	/// <param name="fTorque">Return the rated torque value of the specified axis as a percentage (%).</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_ReadActualTorque(UINT32 nAxisNo, FLOAT &fTorque);

	/// <summary>
	/// This function block returns the position commanded to the servo drive in real time.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is instructed(0~127).</param>
	/// <param name="fPosition">Return the command position.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_ReadCommandPosition(UINT32 nAxisNo, FLOAT &fPosition);

	/// <summary>
	/// This function block returns the velocity commanded to the servo drive in real time.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is instructed(0~127).</param>
	/// <param name="fVelocity">Return the command velocity.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_ReadCommandVelocity(UINT32 nAxisNo, FLOAT &fVelocity);

	/// <summary>
	/// Return an error code when the axis is in the ErrorStop state.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is issued(0~127)</param>
	/// <param name="stAxisError">Return the error state(Type:AXIS_ERROR)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_ReadError(UINT32 nAxisNo, AXIS_ERROR &stAxisError);

	/// <summary>
	/// Return the selected axis status.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is issued(0~127)</param>
	/// <param name="stAxisStatus">Return the Axis State value(Type:MXP_AxisStateBit)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_ReadStatus(UINT32 nAxisNo, MXP_AxisStateBit &stAxisStatus);

	/// <summary>
	/// Return the setting value of an axis parameter.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is issued(0~127)</param>
	/// <param name="nParameterNum">Set the number of the parameter</param>
	/// <param name="fValue">Return the value of the specified parameter.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_ReadParameter(UINT32 nAxisNo, UINT16 nParameterNum, FLOAT &fValue);

	/// <summary>
	/// Set the value of the motion parameter for the specified axis.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis to which the motion command is issued(0~127)</param>
	/// <param name="nParameterNum">Set the number of the parameter.</param>
	/// <param name="fValue">Set the new value of the specified parameter.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_WriteParameter(UINT32 nAxisNo, UINT16 nParameterNum, FLOAT fValue);

	/// <summary>
	/// Store current all parameters.<para/>
	/// If you want to use saved parameter, Use the Upload option in mxConfigurator.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_StoreParameter();

	/// <summary>
	/// In real time, Return the PDO data currently mapped to the specified slave device. <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Set the number of the slave(0~127)</param>
	/// <param name="nDirection">Set PDO direction(0 : Servo write, 1 : MXP write)</param>
	/// <param name="nOffset">Set the offset for PDO mapping.</param>
	/// <param name="nSize">Set the size of the PDO data.</param>
	/// <param name="arrValue">Return the value.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_ReadPdoData(UINT32 nSlaveNo, MXP_PDO_DIRECTION nDirection, UINT16 nOffset, UINT16 nSize, UINT8 *arrValue);

	/// <summary>
	/// Output the data to the PDO currently mapped to the specified slave device.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Set the number of the slave(0~127)</param>
	/// <param name="nOffset">Set the offset for PDO mapping.</param>
	/// <param name="nSize">Set the size of the PDO data.</param>
	/// <param name="arrValue">Set the PDO data value</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_WritePdoData(UINT32 nSlaveNo, UINT16 nOffset, UINT16 nSize, UINT8 *arrValue);

	/// <summary>
	/// Returns the physical output data of the drive.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis(0~127)</param>
	/// <param name="arrValue">Returns the set data as an array of bytes.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_ReadAxisIO(UINT32 nAxisNo, UINT8 *arrValue);

	/// <summary>
	/// Sets the physical output data for the drive.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis(0~127)</param>
	/// <param name="arrValue">Sets the byte array data.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_WriteAxisIO(UINT32 nAxisNo, UINT8 *arrValue);

	/// <summary>
	/// Read parameters from a slave device through EtherCAT communication.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Set the number of the slave(0~127)</param>
	/// <param name="nIndex">Set the index of the parameter.</param>
	/// <param name="nSubIndex">Set the subIndex value.</param>
	/// <param name="nSize">Set the buffer size.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_CoeReadRequest(UINT32 nSlaveNo, UINT16 nIndex, UINT16 nSubIndex, UINT16 nsize);

	/// <summary>
	/// Return the result about calling the ECAT_CoeReadRequest function.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Set the number of the slave(0~127)</param>
	/// <param name="stStatus">Output the data contained in the specified index of the parameter(Type:READ_ETParameterReply)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_CoeReadReply(UINT32 nSlaveNo, READ_ETParameterReply &stStatus);

	/// <summary>
	/// Write parameters to a slave device through the EtherCAT communication network.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Set the number of the slave</param>
	/// <param name="nIndex">Set the index of the parameter.</param>
	/// <param name="nSubIndex">Set the subIndex value.</param>
	/// <param name="nSize">Set the buffer size.</param>
	/// <param name="nWriteData">Set the data in the parameter.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_CoeWriteRequest(UINT32 nSlaveNo, UINT16 nIndex, UINT16 nSubIndex, UINT16 nSize, UINT32 nWriteData);

	/// <summary>
	/// Return the result about calling the ECAT_CoeWriteRequest function.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Set the number of the slave(0~127)</param>
	/// <param name="stStatus">Return the status value(Type:PROCESS_CHECK)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_CoeWriteReply(UINT16 nSlaveNo, PROCESS_CHECK &stStatus);

	/// <summary>
	/// Execute the ECAT_CoeWriteRequest and ECAT_CoeWriteReply Command<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Set the number of the slave(0~127)</param>
	/// <param name="nIndex">Set the index of the parameter.</param>
	/// <param name="nSubIndex">Set the subIndex value.</param>
	/// <param name="nSize">Set the buffer size.</param>
	/// <param name="nWriteData">Set the data in the parameter.</param>
	/// <param name="nWaitTime">Set the wait time(ms) before ECAT_CoeWriteReply Command</param>
	/// <param name="stStatus">Return the status value(Type:PROCESS_CHECK)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_CoeWrite(UINT32 nSlaveNo, UINT16 nIndex, UINT16 nSubIndex, UINT16 nSize, UINT32 nWriteData, INT32 nWaitTime, PROCESS_CHECK &stStatus);

	/// <summary>
	/// Execute the ECAT_CoeReadRequest and ECAT_CoeReadReply Command<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nSlaveNo">Set the number of the slave(0~127)</param>
	/// <param name="nIndex">Set the index of the parameter.</param>
	/// <param name="nSubIndex">Set the subIndex value.</param>
	/// <param name="nSize">Set the buffer size.</param>
	/// <param name="nWaitTime">Set the wait time(ms) before ECAT_CoeReadReply Command</param>
	/// <param name="stStatus">Output the data contained in the specified index of the parameter(Type:READ_ETParameterReply)</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   ECAT_CoeRead(UINT32 nSlaveNo, UINT16 nIndex, UINT16 nSubIndex, UINT16 nSize, INT32 nWaitTime, READ_ETParameterReply &nStatus);


	/// <summary>
	/// Return to Change MXP Unit to Time value<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis(0~127)</param>
	/// <param name="fTargetVel">Set the Velocity value</param>
	/// <param name="fAccDec">Set the Acc/Dec value</param>
	/// <param name="fJerk">Set the Jerk value</param>
	/// <param name="stData">Return the Time Value after calcurating</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_AccDecToAccTime(UINT32 nAxisNo, FLOAT fTargetVel, FLOAT fAccDec, FLOAT fJerk, AccDecToAccTime_Reply &stData);

	/// <summary>
	/// Return to Change Time value to MXP Unit<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis(0~127)</param>
	/// <param name="fTargetVel">Set the Velocity value</param>
	/// <param name="fAccDecBuildUp">Set the AccDecBuildUp value</param>
	/// <param name="fLimitAccDec">Set the LimitAccDec value</param>
	/// <param name="fAccDecRampDown">Set the AccDecRampDown value</param>
	/// <param name="stData">Return the MXP Unit Value after calcurating</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_AccTimeToAccDec(UINT32 nAxisNo, FLOAT fTargetVel, FLOAT fAccDecBuildUp, FLOAT fLimitAccDec, FLOAT fAccDecRampDown, AccTimeToAccDec_Reply &stData);

	/// <summary>
	/// Check the Axis ready status<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis(0~127)</param>
	/// <param name="bReady">Return the value which check the axis status</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_ReadyCheck(UINT32 nAxisNo, bool &bReady);

	/// <summary>
	/// Check the Axis move status<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Set the number of the axis(0~127)</param>
	/// <param name="nTargetPos">Set the target position which wrote when you call the Axis move motion function </param>
	/// <param name="fInPositionCheckRange">Set the inposition range</param>
	/// <param name="nMoveState">Return the move status</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MoveStateCheck(UINT32 nAxisNo, FLOAT nTargetPos, FLOAT fInPositionCheckRange, MXP_MOVESTATE &nMoveState);

#pragma endregion Read Data

#pragma region
public:

	/// <summary>
	/// Command a controlled motion stop on the motor axes of the specified axis group.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Set the number of the target group(0~31).</param>
	/// <param name="bExecute">Execute the command with rising edge.</param>
	/// <param name="fDec">Set the deceleration.</param>
	/// <param name="fJerk">Set the jerk value.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   GRP_Stop(UINT32 nAxesGroup, bool bExecute, FLOAT fDec, FLOAT fJerk);

	/// <summary>
	/// Command a linear Int32erpolation motion from the actual position to a specified absolute position. <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Set the number of the target group(0~31).</param>
	/// <param name="nPosition">Set the position value of the path</param>
	/// <param name="fVelocity">Set the velocity of the path.</param>
	/// <param name="fAcceleration">Set the acceleration of the path.</param>
	/// <param name="fDeceleration">Set the deceleration of the path.</param>
	/// <param name="fJerk">Set the jerk of the path.</param>
	/// <param name="nBufferMode">Set the buffer mode.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   GRP_MoveLinearAbsolute(UINT32 nAxesGroup, GROUP_POS stPosition, FLOAT fVelocity, FLOAT fAcceleration, FLOAT fDeceleration, FLOAT fJerk, MXP_BUFFERMODE_ENUM nBufferMode);

	/// <summary>
	/// Command a linear Int32erpolation from the actual position by the specified distance.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Set the number of the target group(0~31).</param>
	/// <param name="nDistance">Set the distance value of the path</param>
	/// <param name="fVelocity">Set the velocity of the path.</param>
	/// <param name="fAcceleration">Set the acceleration of the path.</param>
	/// <param name="fDeceleration">Set the deceleration of the path.</param>
	/// <param name="fJerk">Set the jerk of the path.</param>
	/// <param name="nBufferMode">Set the buffer mode.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   GRP_MoveLinearRelative(UINT32 nAxesGroup, GROUP_POS stDistance, FLOAT fVelocity, FLOAT fAcceleration, FLOAT fDeceleration, FLOAT fJerk, MXP_BUFFERMODE_ENUM nBufferMode);

	/// <summary>
	/// Command a circular Int32erpolation based on the actual position and the absolute coordinate system.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Set the number of the target group(0~31).</param>
	/// <param name="nPathChoice">Set the direction of the circular path (0: clockwise, 1: counterclockwise).</param>
	/// <param name="fAuxPoInt321">Set the center poInt32 of the circle on Plane1.</param>
	/// <param name="fAuxPoInt322">Set the center poInt32 of the circle on Plane2.</param>
	/// <param name="fEndPoInt321">Set the end poInt32 of the circle on Plane1.</param>
	/// <param name="fEndPoInt322">Set the end poInt32 of the circle on Plane2.</param>
	/// <param name="nPlane1">Specify a plane to be Plane1 (X-C: 0~8)</param>
	/// <param name="nPlane2">Specify a plane to be Plane2 (X-C: 0~8)</param>
	/// <param name="fVelocity">Set the velocity of the path.</param>
	/// <param name="fAcceleration">Set the acceleration of the path.</param>
	/// <param name="fDeceleration">Set the deceleration of the path.</param>
	/// <param name="fJerk">Set the jerk of the path.</param>
	/// <param name="nBufferMode">Set the buffer mode.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   GRP_MoveCircularAbsolute(UINT32 nAxesGroup, MXP_PATHCHOICE_ENUM nPathChoice, FLOAT fAuxPoint1, FLOAT fAuxPoint2, FLOAT fEndPoint1, FLOAT fEndPoint2, MXP_PLANE nPlane1, MXP_PLANE nPlane2, FLOAT fVelocity, FLOAT fAcceleration, FLOAT fDeceleration, FLOAT fJerk, MXP_BUFFERMODE_ENUM nBufferMode);

	/// <summary>
	/// Command a circular Int32erpolation based on the actual position and the relative coordinate system.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Set the number of the target group(0~31).</param>
	/// <param name="nPathChoice">Set the direction of the circular path (0: clockwise, 1: counterclockwise).</param>
	/// <param name="fAuxPoInt321">Set the center poInt32 of the circle on Plane1 relative to the starting poInt32.</param>
	/// <param name="fAuxPoInt322">Set the center poInt32 of the circle on Plane2 relative to the starting poInt32.</param>
	/// <param name="fEndPoInt321">Set the end poInt32 of the circle on Plane1 relative to the starting poInt32.</param>
	/// <param name="fEndPoInt322">Set the end poInt32 of the circle on Plane2 relative to the starting poInt32.</param>
	/// <param name="nPlane1">Specify a plane to be Plane1 (X-C: 0~8)</param>
	/// <param name="nPlane2">Specify a plane to be Plane2 (X-C: 0~8)</param>
	/// <param name="fVelocity">Set the velocity of the path.</param>
	/// <param name="fAcceleration">Set the acceleration of the path.</param>
	/// <param name="fDeceleration">Set the deceleration of the path.</param>
	/// <param name="fJerk">Set the jerk of the path.</param>
	/// <param name="nBufferMode">Set the buffer mode.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   GRP_MoveCircularRelative(UINT32 nAxesGroup, MXP_PATHCHOICE_ENUM nPathChoice, FLOAT fAuxPoint1, FLOAT fAuxPoint2, FLOAT fEndPoint1, FLOAT fEndPoint2, MXP_PLANE nPlane1, MXP_PLANE nPlane2, FLOAT fVelocity, FLOAT fAcceleration, FLOAT fDeceleration, FLOAT fJerk, MXP_BUFFERMODE_ENUM nBufferMode);

	/// <summary>
	/// Set the velocity rate for all function in group axis<para/>
	/// The factor can be set as a real number from 0 to 1(0 ~ 100%) <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Set the number of the target group(0~31).</param>
	/// <param name="fVelFactor">Set the velocity scale factor as a multiplier of a real number</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   GRP_SetOverride(UINT32 nAxesGroup, FLOAT fVelFactor);

	/// <summary>
	/// Return the actual velocity of the path in real time.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Set the number of the target group(0~31).</param>
	/// <param name="fPathVelocity">Return the actual velocity of the path.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   GRP_ReadActualVelocity(UINT32 nAxesGroup, FLOAT &fPathVelocity);

	/// <summary>
	/// Return the actual position of each coordinate in real time. <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Set the number of the target group(0~31).</param>
	/// <param name="stPosition">Return the position</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   GRP_ReadActualPosition(UINT32 nAxesGroup, GROUP_POS &stPosition);

	/// <summary>
	///  Return the command velocity of the path in real time.<para/>
	///  The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Set the number of the target group(0~31).</param>
	/// <param name="fPathVelocity">Return the command velocity of the path. </param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   GRP_ReadCommandVelocity(UINT32 nAxesGroup, FLOAT &fPathVelocity);

	/// <summary>
	/// Return the actual position of each coordinate in real time. <para/>
	///  The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Set the number of the target group(0~31).</param>
	/// <param name="stPosition">Return the position</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   GRP_ReadCommandPosition(UINT32 nAxesGroup, GROUP_POS &stPosition);

	/// <summary>
	/// Return the axis number from the entered group number.<para/>
	///  The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Set the number of the target group(0~31).</param>
	/// <param name="arrAxisNo">Return the axis numbers in group. </param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   GRP_GetGroupAxis(UINT32 nAxesGroup, INT32 *arrAxisNo);

	/// <summary>
	/// Check the Axis ready status<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Set the number of the Group(0~31)</param>
	/// <param name="bReady">Return the value which check the axis status</param>
	/// <returns></returns>
	/// <remarks></remarks>
	MXP_FUNCTION_STATUS   GRP_ReadyCheck(UINT32 nAxesGroup, bool &bReady);

	/// <summary>
	/// Returns the status of the selected group.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Sets the number of the Group(0~31)</param>
	/// <param name="stAxisStatus">Returns the status of the group.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   GRP_ReadStatus(UINT32 nAxesGroup, MXP_AxisStateBit &stAxisStatus);

private:
	MXP_FUNCTION_STATUS   GRP_BufferedIO(UINT32 nAxesGroup, UINT16 nSlaveNo, UINT16 nBitPos, bool bBitValue);
	MXP_FUNCTION_STATUS   GRP_Dwell(UINT32 nAxesGroup, float fTime);


#pragma endregion Group

#pragma region
	enum MXP_DATA_TYPE
	{
		DATA_TYPE_Bit = 0x10,
		DATA_TYPE_Word = 0x20,
		DATA_TYPE_L = 0x30,
		DATA_TYPE_F = 0x40,
		DATA_TYPE_A = 0x50,
		DATA_TYPE_Byte = 0x60,
		DATA_TYPE_D = 0x70,
	};
public: 

	/// <summary>
	/// Read Bit data of PLC from entered address and bit number. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nBitNo">Set the bit number</param>
	/// <param name="bData">Return the bit data of bool format</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_ReadBit(UINT32 nAddress, UINT8 nBitNo, bool &bData);

	/// <summary>
	/// Read UINT8 data of PLC from entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Return the UINT8 data</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_ReadByte(UINT32 nAddress, UINT8 &nData);

	/// <summary>
	/// Read INT8 data of PLC from entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Return the INT8 data</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_ReadSByte(UINT32 nAddress, INT8 &nData);


	/// <summary>
	/// Read UINT16 data of PLC from entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Return the UINT16 data</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_ReadUInt16(UINT32 nAddress, UINT16 &nData);

	/// <summary>
	/// Read INT16 data of PLC from entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Return the INT16 data</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_ReadInt16(UINT32 nAddress, INT16 &nData);

	/// <summary>
	/// Read UINT32 data of PLC from entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Return the UINT32 data</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_ReadUInt32(UINT32 nAddress, UINT32 &nData);

	/// <summary>
	/// Read INT32 data of PLC from entered address. <para/>
	/// A must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Return the INT32 data</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_ReadInt32(UINT32 nAddress, INT32 &nData);

	/// <summary>
	/// Read Single data of PLC from entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="fData">Return the Single data</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_ReadFloat(UINT32 nAddress, FLOAT &fData);

	/// <summary>
	/// Read UINT64 data of PLC from entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Return the UINT64 data</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_ReadUInt64(UINT32 nAddress, UINT64 &nData);

	/// <summary>
	/// Read INT64 data of PLC from entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Return the INT64 data</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_ReadInt64(UINT32 nAddress, INT64 &nData);

	/// <summary>
	/// Read double data of PLC from entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="dData">Return the double data</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_ReadDouble(UINT32 nAddress, double &dData);

	/// <summary>
	/// Write Bit data to entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nBitNo">Set the bit number</param>
	/// <param name="bData">Set the write data to PLC</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_WriteBit(UINT32 nAddress, UINT8 nBitNo, bool &bData);

	/// <summary>
	/// Write UINT8 data to entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Set the write data to PLC</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_WriteByte(UINT32 nAddress, UINT8 nData);

	/// <summary>
	/// Write INT8 data to entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Set the write data to PLC</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_WriteSByte(UINT32 nAddress, INT8 nData);

	/// <summary>
	/// Write UINT16 data to entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Set the write data to PLC</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_WriteUInt16(UINT32 nAddress, UINT16 nData);

	/// <summary>
	/// Write INT16 data to entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Set the write data to PLC</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_WriteInt16(UINT32 nAddress, INT16 nData);

	/// <summary>
	/// Write UINT32 data to entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Set the write data to PLC</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_WriteUInt32(UINT32 nAddress, UINT32 nData);

	/// <summary>
	/// Write INT32 data to entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Set the write data to PLC</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_WriteInt32(UINT32 nAddress, INT32 nData);

	/// <summary>
	/// Write Single data to entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Set the write data to PLC</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_WriteFloat(UINT32 nAddress, FLOAT nData);

	/// <summary>
	/// Write UINT64 data to entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Set the write data to PLC</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_WriteUInt64(UINT32 nAddress, UINT64 nData);

	/// <summary>
	/// Write INT64 data to entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="nData">Set the write data to PLC</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_WriteInt64(UINT32 nAddress, INT64 nData);

	/// <summary>
	/// Write double data to entered address. <para/>
	/// Address must be over 5000.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAddress">Set the address number(over 5000)</param>
	/// <param name="dData">Set the write data to PLC</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   PLC_WriteDouble(UINT32 nAddress, double dData);
#pragma endregion PLC Interface

#pragma region
public:
	/// <summary>
	/// Sets the entered axis to the CCC function.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisCount">Sets the entire axis count that is Cross Coupled Control (CCC). (2 ~ 10)</param>
	/// <param name="arrAxis">Sets the axis number to be CCC. (0 ~ 127) <para/>
	/// Supports up to 10 axes.</param>
	/// <param name="nMode">0 : CCC always applied <para/>
	/// 1: CCC operation only if the axis command speed is not zero.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MultiAxisCCCSet(UINT16 nAxisCount, UINT32 *arrAxis,  UINT16 nMode);

	/// <summary>
	/// Turns off the CCC function on the entered axis.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisCount">Sets the entire axis count that is Cross Coupled Control (CCC). (2 ~ 10)</param>
	/// <param name="arrAxis">Sets the axis number to be CCC. (0 ~ 127) <para/>
	/// Supports up to 10 axes.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MultiAxisCCCReset(UINT16 nAxisCount, UINT32 *arrAxis);

	/// <summary>
	/// Sets the Gain on the CCC axis.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisCount">Sets the entire axis count that is Cross Coupled Control (CCC). (2 ~ 10)</param>
	/// <param name="arrAxis">Sets the axis number to be CCC. (0 ~ 127) <para/>
	/// Supports up to 10 axes.</param>
	/// <param name="arrSingleAxisGain">Set the PID Control Loop Gain for each axis.</param>
	/// <param name="arrCCCGain">Set CCC Gain.</param>
	/// <param name="nMultiControlKffGain">Sets the reward gain between the last axis and the first and second axes.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MultiAxisCCCGainSet(UINT16 nAxisCount, UINT32 *arrAxis, MXP_MULTIAXISCOUPLE_SINGLEAXISGAIN *arrSingleAxisGain, MXP_MULTIAXISCOUPLE_CCCGAIN *arrCCCGain, UINT16 nMultiContolKffGain);

	/// <summary>
	/// Returns the operational state of the AX_MultiAxisCCCSet function.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisCount">Sets the entire axis count that is Cross Coupled Control (CCC). (2 ~ 10)</param>
	/// <param name="arrAxis">Sets the axis number to be CCC. (0 ~ 127) <para/>
	/// Supports up to 10 axes.</param>
	/// <param name="stStatus">Returns the AX_MultiAxisCCCSet function behavior state.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MultiAxisCCCSetCheck(UINT16 nAxisCount, UINT32 *arrAxis, PROCESS_CHECK &stStatus);

	/// <summary>
	/// Returns the operational state of the AX_MultiAxisCCCReset function.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisCount">Sets the entire axis count that is Cross Coupled Control (CCC). (2 ~ 10)</param>
	/// <param name="arrAxis">Sets the axis number to be CCC. (0 ~ 127) <para/>
	/// Supports up to 10 axes.</param>
	/// <param name="stStatus">Returns the AX_MultiAxisCCCReset function action state.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MultiAxisCCCResetCheck(UINT16 nAxisCount, UINT32 *arrAxis, PROCESS_CHECK &stStatus);

	/// <summary>
	/// Returns the operational state of the AX_MultiAxisCCCGainSet function.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisCount">Sets the entire axis count that is Cross Coupled Control (CCC). (2 ~ 10)</param>
	/// <param name="arrAxis">Sets the axis number to be CCC. (0 ~ 127) <para/>
	/// Supports up to 10 axes.</param>
	/// <param name="stStatus">Returns the AX_MultiAxisCCCGainSet function behavior state.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS   AX_MultiAxisCCCGainSetCheck(UINT16 nAxisCount, UINT32 *arrAxis, PROCESS_CHECK &stStatus);
#pragma endregion MutiAxis CCC

#pragma region
public:
	/// <summary>
	/// Load and run the defined file (.txt,.json) with commands <para/>
	/// in IO, Dwell, and Group Actions. <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="strFilePath">Sets the path of the file which is defined the single axis sequence.</param>
	/// <param name="nStartStep">Sets the Start step</param>
	/// <param name="nEndStep">Sets the End step</param>
	/// <param name="strErrorMessage">Returns the relevant information if the file type is invalid.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS  GRP_SequenceMove_File(string strFilePath, UINT16 nStartStep, UINT16 nEndStep, string &strErrorMessage);

	/// <summary>
	/// Returns the GRP_SequenceMove_File action state.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxesGroup">Sets the number of the Group(0~31)</param>
	/// <param name="nCurStep">Returns the Step number currently in progress.</param>
	/// <param name="strCurStepName">Returns the name of the Step currently in progress</param>
	/// <param name="nRemainCount">Returns the remaining unused Step count.</param>
	/// <param name="stStatus">Returns the status of the Sequence Move</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS  GRP_SequenceMoveCheck(UINT32 nAxesGroup, UINT16 &nCurStep, string &strCurStepName, UINT16 &nRemainCount, SEQUENCEMOVEPROCESS_CHECK &stStatus);

	/// <summary>
	/// Load and run files (.txt, .json) that define the sequence of actions, <para/>
	/// such as IO, Dwell, interpolation, etc.<para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="strFilePath">Sets the path of the file which is defined the single axis sequence.</param>
	/// <param name="nStartStep">Sets the Start step</param>
	/// <param name="nEndStep">Sets the End step</param>
	/// <param name="strFileReadError">Returns the relevant information if the file type is invalid.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS  AX_SequenceMove_File(string strFilePath, UINT16 nStartStep, UINT16 nEndStep, string &strFileReadError);

	/// <summary>
	/// Returns the AX_SequenceMove_File action state. <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <param name="nCurStep">Returns the Step number currently in progress.</param>
	/// <param name="strCurStepName">Returns the name of the Step currently in progress</param>
	/// <param name="nRemainCount">Returns the remaining unexecuted Step count.</param>
	/// <param name="stStatus">Returns the status of the Sequence Move</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS  AX_SequenceMoveCheck(UINT32 nAxisNo, UINT16 &nCurStep, string &strCurStepName, UINT16 &nRemainCount,SEQUENCEMOVEPROCESS_CHECK &stStatus);

	/// <summary>
	/// Supports multipoint transfer command function. <para/>
	/// The result of calling the function is returned
	/// </summary>
	/// <param name="nAxisNo">Axis number(0 ~ 127)</param>
	/// <param name="nMoveMode">Sets Sequence Move Mode.</param>
	/// <param name="nBufferedMode">Sets the Buffer Mode.</param>
	/// <param name="arrData">Set Sequence Move Data.</param>
	/// <param name="fAcc">Sets the acceleration value.</param>
	/// <param name="fDec">Sets the deceleration value.</param>
	/// <param name="fJerk">Sets Jerk value.</param>
	/// <returns></returns>
	MXP_FUNCTION_STATUS  AX_SequenceMoveRequest(UINT32 nAxisNo, MXP_SequenceMoveMode nMoveMode, MXP_BUFFERMODE_ENUM nBufferedMode, std::vector<StSequenceMoveStep> &arrData, float fAcc, float fDec, float fJerk);
	MXP_FUNCTION_STATUS  AX_SequenceMoveRequest(UINT32 nAxisNo, MXP_SequenceMoveMode nMoveMode, MXP_BUFFERMODE_ENUM nBufferedMode, std::vector<StSequenceMoveStep> &arrData, float fAcc, float fDec, float fJerk, bool bPositionSensorUsing);
	void SEQ_AxisSequenceMoveManager(INT nAxisNo);
	void SEQ_GroupSequenceMoveManager(INT nAxesGroup);
	void Tick();
private:
	MXP_FUNCTION_STATUS SYS_GetAxisBufferInfo(UINT32 nAxisNo, UINT16 &nSaveNum, UINT16 &nReadNum, UINT16 &nCurBlock);
	MXP_FUNCTION_STATUS SYS_GetGroupBufferInfo(UINT32 nAxesGroup, UINT16 &nSaveNum, UINT16 &nReadNum, UINT16 &nCurBlock);
	MXP_FUNCTION_STATUS AX_MoveAbsolute_Ex(UINT32 nAxisNo, UINT16 nCommandBlockNo, float fPosition, float fVel,
		float fAcc, float fDec, float fJerk,MXP_DIRECTION_ENUM nDirection, MXP_BUFFERMODE_ENUM nBufferMode);
	MXP_FUNCTION_STATUS AX_MoveRelative_Ex(UINT32 nAxisNo, UINT16 nCommandBlockNo, float fDistance, float fVel,
		float fAcc, float fDec, float fJerk, MXP_BUFFERMODE_ENUM nBufferMode);
	MXP_FUNCTION_STATUS GRP_MoveLinearAbsolute_Ex(UINT32 nAxesGroup, UINT16 nCommandBlockNo, GROUP_POS stPosition, float fVelocity,
		float fAcceleration, float fDeceleration, float fJerk, MXP_BUFFERMODE_ENUM nBufferMode);
	MXP_FUNCTION_STATUS GRP_MoveLinearRelative_Ex(UINT32 nAxesGroup, UINT16 nCommandBlockNo, GROUP_POS stDistance, float fVelocity,
		float fAcceleration, float fDeceleration, float fJerk, MXP_BUFFERMODE_ENUM nBufferMode);
	MXP_FUNCTION_STATUS  AX_BufferedIO_Ex(UINT32 nAxisNo, UINT16 nCommandBlockNo, UINT16 nSlaveNo, UINT16 nBitPos, bool bBitValue);
	MXP_FUNCTION_STATUS  AX_Dwell_Ex(UINT32 nAxisNo, UINT16 nCommandBlockNo, float fTime);
	MXP_FUNCTION_STATUS  GRP_BufferedIO_Ex(UINT32 nAxesGroup, UINT16 nCommandBlockNo, UINT16 nSlaveNo, UINT16 nBitPos, bool bBitValue);
	MXP_FUNCTION_STATUS  GRP_Dwell_Ex(UINT32 nAxesGroup, UINT16 nCommandBlockNo, float fTime);
	MXP_FUNCTION_STATUS AX_SequenceMoveLastStepVelocityChange(UINT32 nAxisNo, MXP_SequenceMoveMode nMoveMode, std::vector<StSequenceMoveStep> &arrInData, std::vector<StSequenceMoveStep> &arrOutData);
	MXP_FUNCTION_STATUS AX_SequenceMove(UINT32 nAxisNo, UINT16 nSequenceCount, std::vector<StSequenceMove> &arrData, UINT16 nStartStep, UINT16 nEndStep);
	MXP_FUNCTION_STATUS GRP_SequenceMove(UINT32 nAxesGroup, UINT16 nSequenceCount, std::vector<StGroupSequenceMove> &arrData, UINT16 nStartStep, UINT16 nEndStep);
	bool AX_SequenceMoveSetVaildCheck(UINT32 nAxisNo, UINT16 nSequenceCount, bool bFirstStep);
	bool GRP_SequenceMoveSetVaildCheck(UINT32 nAxesGroup, UINT16 nSequenceCount, bool bFirstStep);
	MXP_FUNCTION_STATUS AX_SequenceMoveCmdSet(UINT32 nAxisNo, UINT16 nStartIndex, UINT16 nEndIndex);
	MXP_FUNCTION_STATUS GRP_SequenceMoveCmdSet(UINT32 nAxesGroup, UINT16 nStartIndex, UINT16 nEndIndex);
	MXP_SequenceBufferState AX_SequenceBufferStateCheck(UINT32 nAxisNo);
	MXP_SequenceBufferState GRP_SequenceBufferStateCheck(UINT32 nAxesGroup);
	UINT16 GetAxisSequenceMoveEndCount(INT32 nAxisNo);
	UINT16 GetGroupSequenceMoveEndCount(INT32 nAxisNo);
#pragma endregion SequenceMove

#pragma region 

	private: 
		void AX_Init_ExAPI_Param();
#pragma endregion ExAPIAxisParam

#pragma region

	struct strFeedbackSensorControlInfo
	{
		bool bUsing;
		UINT8 nSensorSlaveNo;
		UINT8 nSensorPos;
		UINT8 nSensorSize;
		float fSensorTargetValue;
		float fSensorPositionSetRange;
		float fSensorPulseToUnit;
		float fTargetPosCalDist;
		float fTargetPosCalVel;
		float fOverrideDist;
		float fOverrideValue;
		float fFinalTargetPos;

		MXP_DIRECTION_ENUM nDirection;
	};

	private: 
		strFeedbackSensorControlInfo mFeedbackSensorParam[127];
		bool AX_ValidCheckUsingFeedBackSensorCtrl(UINT32 nAxisNo, float fLastStepDist);

	public:
		MXP_FUNCTION_STATUS AX_SetUsingPositionSensor(UINT32 nAxisNo, bool bUsing, UINT32 nSlaveNo, UINT16 nOffset,
													  UINT16 nSize, float fSensorTargetValue, float fSensorPositionSetRange,
													  float fSensorPulseToUnit, float fTargetPosCalDist, float fTargetPosCalVel,
													  float	fOverrideDist);
		MXP_FUNCTION_STATUS AX_SetUsingPositionSensor(UINT32 nAxisNo, bool bUsing, UINT32 nSlaveNo, UINT16 nOffset, 
			                                          UINT16 nSize, float fSensorTargetValue, float fSensorPositionSetRange,
													  float fSensorPulseToUnit, float fTargetPosCalDist);
		MXP_FUNCTION_STATUS AX_SetUsingPositionSensor(UINT32 nAxisNo, bool bUsing);

#pragma endregion SequenceMove_UsingSensor

#pragma region
	struct  SpinTable
	{
		float fStepVel;
		float fStepTime;
	};
	 // Move 지령만 수행 
	struct stSpinRecipeWriteTable
	{
		float fVel;
		float fAcc;
		float fJerk;
		int nTime;
	};
	
	struct stPLCFBRunInfo
	{
		bool bUsing;
		int nFBIndex;
		ePLCFBRunMode nRunState;
	};


	private:
		stSpinRecipeWriteTable mSpinRecipeWriteTable[19];
		UINT8 mSpinRecipeWriteTableCount;
		// 사용 가능한  Block 을 Get
		bool FBInfoReset(bool bLocalMemoryOnly);
#pragma endregion PLC Move

#pragma region
	private:	
		int GetValidFeedBackSensorFBNumber(UINT32 nAxisNo);
		bool AX_FeedBackSensorMove_ValidCheck(UINT32 nAxisNo);
	public:
		MXP_FUNCTION_STATUS AX_FeedBackSensorUsingCheck(UINT32 nAxisNo, FBPROCESS_CHECK &stStatus);
		MXP_FUNCTION_STATUS AX_FeedBackSensorOverrideInVaildFlag(UINT32 nAxisNo, UINT8 bInVaildFlag);
		MXP_FUNCTION_STATUS AX_FeedBackSensorLog(UINT32 nAxisNo);
		MXP_FUNCTION_STATUS AX_FeedBackSensorDataSet(UINT32 nAxisNo);
		MXP_FUNCTION_STATUS AX_FeedBackSensorRun(UINT32 nAxisNo, ePLCFBRunMode nRunMode);

		float AX_ReadOverride(UINT32 nAxisNo);
#pragma endregion FeedbackSensorFBInterface
};

