
//==============================================================================
//Copyright (c) 2013-2014 LS MotionControl. All rights reserved.
//==============================================================================
// MODULE		: MXP_PLCopenBlock.h
// Project		: MXP2.0
// ABSTRACT		: 
// Environment	: Win32
// NOTE			: PLCopen structure 
//==============================================================================
#pragma once


//=============================================================================
// Command Code Definition
//=============================================================================
#define MXP_CMD_CODE_NULL				0x0000
#define MXP_CMD_CODE_INITKERNEL			0x0001
#define MXP_CMD_CODE_SETONLINEMODE		0x0002
#define MXP_CMD_CODE_MCPOWER			0x0003
#define MXP_CMD_CODE_SETMOTORSCALE		0x0004
#define MXP_CMD_CODE_TRQCONTROL			0x0005
#define MXP_CMD_CODE_TRQLIMIT			0x0006
#define MXP_CMD_CODE_MOVEVELOCITY		0x0007
#define MXP_CMD_CODE_DS402HOME			0x0008
#define MXP_CMD_CODE_READACTUALPOS		0x0009
#define MXP_CMD_CODE_VENDORPDOREAD		0x000A
#define MXP_CMD_CODE_SDOSYNCUPLOAD		0x000B
#define MXP_CMD_CODE_SDOSYNCDNLOAD		0x000C
#define MXP_CMD_CODE_DESTROY			0x000D
#define MXP_CMD_CODE_GETKERNELSTATUS	0x000E
#define MXP_CMD_CODE_GETONLINEMODE		0x000F
#define MXP_CMD_CODE_READACTUALVELOCITY	0x0010
#define MXP_CMD_CODE_STOP				0x0011


#define MXP_CMD_CODE_NOTDEFINED			0xFFFF

//=============================================================================
// Error Code Definition (MXP_STATUS)
//=============================================================================

#define MXP_ERROR_NOERR					0x0000

#define MXP_ERROR_KERNEL_ALREADY_RUN	0x0001
#define MXP_ERROR_ENI_XML_INVALID		0x0002
#define MXP_ERROR_NO_SLAVE				0x0003
#define MXP_ERROR_KERNEL_NOT_INIT		0x0004
#define MXP_ERROR_SLAVE_TYPE_INVALID	0x0005
#define MXP_ERROR_NO_CIA402_DEVICE		0x0006
#define MXP_ERROR_CHANNEL_INVALID		0x0007
#define MXP_ERROR_ALREADY_INITIALIZED	0x0008
#define MXP_ERROR_INVALID_PARAM			0x0009
#define MXP_ERROR_PARAM_RANGE_OVER		0x000A
#define MXP_ERROR_DEVICE_NOT_READY		0x000B
#define MXP_ERROR_MOTOR_NUM_MAXLIMIT	0x000C
#define MXP_ERROR_NOT_SUPPORTED_PDO		0x000D
#define MXP_ERROR_ENI_DEVICE_CREATION	0x000E
#define MXP_ERROR_ENI_DEVICE_OPEN		0x000F
#define MXP_ERROR_POSITION_READ_OK		0x0010

// reserved area 0x000B - 0x0FFF

#define MXP_ERROR_NOT_IMPLEMENTED		0x0FFF
#define MXP_ERROR_UNKNOWN				0x1000

#define ETHERCAT_SDO_SIZE 1024

/************************************************************************/
//	Axis State Machine Definition
//
//	(See : "Technical Specification
//		PLCopen - Technical Committee 2 Task Force
//		Function blocks for motion control")
/************************************************************************/
typedef enum EMXP_AXIS_STATE 
{
	PLCOPEN_AXIS_DISABLED_A					=0,
	PLCOPEN_AXIS_STANDSTILL_A				=1,
	PLCOPEN_AXIS_HOMING_A					=2,
	PLCOPEN_AXIS_ERRORSTOP_A				=3,
	PLCOPEN_AXIS_STOPPING_A					=4,
	PLCOPEN_AXIS_DISCRETE_MOTION_A			=5,
	PLCOPEN_AXIS_SYNCHRO_MOTION_A			=6,
	PLCOPEN_AXIS_CONTINUOUS_MOTION_A		=7,

} MXP_AXIS_STATE;

typedef enum
{
	NET_NOTCONNECTED_A			= 0x00,		// Kernel not connected. --> Status unknown.
	NET_STATE_INIT_A			= 0x01,
	NET_STATE_PREOP_A			= 0x02,
	NET_STATE_SAFEOP_A			= 0x04,
	NET_STATE_OP_A				= 0x08

} MXP_NETWORK_SATUS;


/************************************************************************/
/// \enum MXP_DIRECTION_ENUM
/// This type define kinds of supported directions
/************************************************************************/
typedef enum 
{
	MXP_NONE_DIRECTION			= 0,
	MXP_POSITIVE_DIRECTION		= 1,
	MXP_SHORTEST_WAY			= 2,
	MXP_NEGATIVE_DIRECTION		= 3,
	MXP_CURRENT_DIRECTION		= 4

} MXP_DIRECTION_ENUM;

/************************************************************************/
/// \enum MXP_BUFFERED_MODE_ENUM
/// This type define all supported buffered mode options
/************************************************************************/
typedef enum
{
	MXP_ABORTING				= 0,
	MXP_BUFFERED				= 1,
	MXP_BLENDING_LOW			= 2,
	MXP_BLENDING_PREVIOUS		= 3,
	MXP_BLENDING_NEXT			= 4,
	MXP_BLENDING_HIGH			= 5,
	MXP_SINGLE_BLOCK			= 6										// Vendor specific.

} MXP_BUFFERMODE_ENUM;

/************************************************************************/
/// \enum MXP_EXECUTIONMODE_ENUM
/// This type define all supported buffered mode options
/************************************************************************/
typedef enum
{
	MXP_IMMEDIATELY				= 1,
	MXP_QUEUED					= 2

} MXP_EXECUTIONMODE_ENUM;

/************************************************************************/
/// \enum MXP_SOURCE_ENUM
/// This type define all supported buffered mode options
/************************************************************************/
typedef enum
{
	MXP_COMMANDEDVALUE			= 1,
	MXP_SETVALUE				= 2,
	MXP_ACTUALVALUE				= 3

} MXP_SOURCE_ENUM;


typedef enum
{
	MXP_ABSOLUTE				= 1,
	MXP_RELATIVE				= 2,
	MXP_RAMPIN					= 3

} MXP_STARTMODE_ENUM;


/************************************************************************/
/// \enum MXP_COORDSYSTEM_ENUM
/// \brief
/************************************************************************/
typedef enum
{
	MXP_ACS						= 1,
	MXP_MCS						= 2,
	MXP_PCS						= 3
	
} MXP_COORDSYSTEM_ENUM;

/************************************************************************/
/// \enum MXP_TRANSITIONMODE_ENUM
/// \brief
/************************************************************************/
typedef enum
{
	MXP_TM_NONE					= 0,									// Insert no transition curve (default mode)
	MXP_TM_STARTVELOCITY		= 1,									// Transition with given start velocity
	MXP_TM_CONSTANTVELOCITY		= 2,									// Transition with given constant velocity
	MXP_TM_CORNERDISTANCE		= 3,									// Transition with given corner distance
	MXP_TM_MAXCORNERDEVIATION	= 4										// Transition with given maximum corner deviation

} MXP_TRANSITIONMODE_ENUM;

/************************************************************************/
/// \enum MXP_CIRCLEMODE_ENUM
/// \brief
/************************************************************************/
typedef enum
{
	MXP_BORDER					= 1,
	MXP_CENTER					= 2,
	MXP_RADIUS					= 3

} MXP_CIRCLEMODE_ENUM;

/************************************************************************/
/// \enum MXP_PATHCHOICE_ENUM
/// \brief
/************************************************************************/
typedef enum
{
	MXP_CLOCKWISE				= 0,
	MXP_COUNTERCLOCKWISE		= 1

} MXP_PATHCHOICE_ENUM;

/************************************************************************/
/// \enum MXP_SYNCMODE_ENUM
/// \brief
/************************************************************************/
typedef enum
{
	MXP_SHORTEST				= 1,
	MXP_CATCHUP					= 2,
	MXP_SLOWDOWN				= 3

} MXP_SYNCMODE_ENUM;

/************************************************************************/
/// \enum MC_SWITCH_MODE_ENUM
/// \brief
/************************************************************************/
typedef enum
{
	MXP_ON						= 0,
	MXP_OFF						= 1,
	MXP_EDGE_ON					= 2,
	MXP_EDGE_OFF				= 3,
	MXP_EDGE_SWITCH_POSITIVE	= 4,
	MXP_EDGE_SWITCH_NEGATIVE	= 5

} MXP_SWITCHMODE_ENUM;

/************************************************************************/
/// \enum MC_HOME_DIRECTION_ENUM
/// \brief
/************************************************************************/
typedef enum
{
	MXP_POSITIVE				= 1,
	MXP_NEGATIVE				= 2,
	MXP_SWITCH_POSITIVE			= 3,
	MXP_SWITCH_NEGATIVE			= 4

} MXP_HOMEDIRECTION_ENUM;

/************************************************************************/
/// \enum MXP_HOME_MODE_ENUM
/// \brief
/************************************************************************/
typedef enum
{
	MXP_ABS_SWITCH				= 1,
	MXP_LIMIT_SWITCH			= 2,
	MXP_REF_PULSE				= 3,
	MXP_DIRECT					= 4,
	MXP_BLOCK					= 5

} MXP_HOMEMODE_ENUM;

/************************************************************************/
/// \enum MXP_TOUCHPROBE_EDGE_ENUM
/// \brief
/************************************************************************/
typedef enum
{
	MXP_EDGE_POSITIVE	= 0,
	MXP_EDGE_NEGATIVE	= 1,
	MXP_EDGE_BOTH		=2,

} MXP_TOUCHPROBE_EDGE_ENUM;

/************************************************************************/
/// \enum MXP_TOUCHPROBE_EDGE_ENUM
/// \brief
/************************************************************************/
typedef enum
{
	MXP_TOUCH_CH1	= 0,
	MXP_TOUCH_CH2	= 1

} MXP_TOUCHPROBE_CHNL_ENUM;

/************************************************************************/
/// \enum MXP_CONTROLMODE_ENUM
/// \brief
/************************************************************************/
typedef enum
{
	MXP_PP	= 0,
	MXP_CSP	= 1,
	MXP_CSV = 2,
	MXP_CST = 3

} MXP_CONTROLMODE_ENUM;

typedef enum
{
	MXP_PDO_Tx	= 0,	//Slave -> MXP : ex)ActualPosition
	MXP_PDO_Rx	= 1		//MXP -> Slave : ex)TargetPosition
} MXP_PDODIRECTION_ENUM;



typedef	enum 
{
	_nullST						= 0,		// Bus terminator
	_dummyST					= 1,		// Bus coupler (Has address, No process data, No mailbox)
	_simpleST					= 2,
	_portST						= 3,
	_routerST					= 4,
	_ax2000ST					= 5,
	_el67xxST					= 6,
	_MBoxST						= 7,
	_CiA402ST					= 8

} MXP_SLAVE_TYPE;

typedef enum 
{
	MCI							= 0,
	Linear						= 1,
	Polynom3					= 2,
	Polynom5					= 3,
	ModifiedSine				= 4,
	Cycloid						= 5
}MXP_INTERPOLATIONTYPE;

typedef struct MXP_AXIS_REF
{
	UINT32					AxisNo;												// Axis Number

} _MXP_AXIS_REF;


typedef struct MXP_AXESGROUP_REF
{
	UINT32					AxesGroup;

} _MXP_AXESGROUP_REF;

typedef struct MXP_INPUT_REF
{
	UINT32					SourceNo;

} _MXP_INPUT_REF;

typedef struct MXP_OUTPUT_REF
{
	UINT32					SourceNo;
	
} _MXP_OUTPUT_REF;

typedef struct MXP_CAM_REF
{
	UINT32					CamTable;											// CAM Table

} _MXP_CAM_REF;

typedef struct MXP_CAMTABLE_REF 
{
	FLOAT			MasterPos;
	FLOAT			SlavePos;
	FLOAT 			SlaveVel;//지원안함
	FLOAT 			SlaveAcc; //지원안함
	FLOAT 			SlaveJerk; //지원안함
	UINT32			PointType; //지원안함	
	UINT32			InterpolationType;

}_MXP_CAMTABLE_REF;

typedef struct MXP_TP_REF
{
	time_t					Time;
	FLOAT					Position;

} _MXP_TP_REF;

typedef struct MXP_TV_REF
{
	time_t					Time;
	FLOAT					Velocity;

} _MXP_TV_REF;

typedef struct MXP_TA_REF
{
	time_t					Time;
	FLOAT					Acceleration;

} _MXP_TA_REF;


/******************************************************************************
* \struct MXP_POWER_IN
* \brief Power On Command input data structure.
*******************************************************************************/
typedef struct MXP_POWER_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis.

	UINT8					Enable;												// <UINT8> As long as 'Enable' is true, power is being enabled.
	UINT8					EnablePositive;										// <UINT8> As long as 'Enable' is true, this permits motion in positive direction.
	UINT8					EnableNegative;										// <UINT8> As long as 'Enable' is true, this permits motion in negative direction.

} _MXP_POWER_IN;

/******************************************************************************
* \struct MXP_POWER_OUT
* \brief Power On Command output data structure. Reserved for future use.
*******************************************************************************/
typedef struct MXP_POWER_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis.

	UINT8					Status;												// <UINT8> Effective state of the power stage.
	UINT8					Valid;												// <UINT8> If true, a valid set of outputs is available at the FB.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_POWER_OUT;


/******************************************************************************
* \struct MXP_HOME_IN
* \brief Home Command Input data structure.
*******************************************************************************/
typedef struct MXP_HOME_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Start the motion at rising edge												
	FLOAT					Position;											// <float> Absolute position when the reference signal is detected [u]
		
	MXP_BUFFERMODE_ENUM		BufferMode;											// MXP_BUFFERED_MODE_ENUM Defines the behavior of the axis: modes are Aborting, Buffered, Blending

} _MXP_HOME_IN;

/******************************************************************************
* \struct MXP_HOME_OUT
* \brief Home Command Output data structure.
*******************************************************************************/
typedef struct MXP_HOME_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis.

	UINT8					Done;												// <UINT8> Reference known and set successfully.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to e expected.
	UINT8					Active;												// <UINT8> indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred withing the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_HOME_OUT;


/******************************************************************************
* \struct MXP_STOP_IN
* \brief Stop Command Input data structure.
*******************************************************************************/
typedef struct MXP_STOP_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Start the action at rising edge
	FLOAT					Deceleration;										// <float> Value of the 'Deceleration' [u/s²]
	FLOAT					Jerk;												// <float> Value of the 'Jerk' [u/s³]
	
} _MXP_STOP_IN;

/******************************************************************************
* \struct MXP_STOP_OUT
* \brief Stop Command Output data structure.
*******************************************************************************/
typedef struct MXP_STOP_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> Zero velocity reached.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by switching off power (only possibility to abort)
	UINT8					Error;												// <UINT8> Signals that an error has occurred withing the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_STOP_OUT;


/******************************************************************************
* \struct MXP_HALT_IN
* \brief Halt Command Input data structure.
*******************************************************************************/
typedef struct MXP_HALT_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Start the action at rising edge.
	FLOAT					Deceleration;										// <float> Value of the 'Deceleration' [u/s²]
	FLOAT					Jerk;												// <float> Value of the 'Jerk' [u/s³]
	MXP_BUFFERMODE_ENUM		BufferMode;											// MXP_BUFFERMODE_ENUM Defines the behavior of the axis: modes are Aborting, Buffered, Blending

} _MXP_HALT_IN;

/******************************************************************************
* \struct MXP_HALT_IN
* \brief Halt Command Output data structure.
*******************************************************************************/
typedef struct MXP_HALT_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> Zero velocity reached.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error Identification.

} _MXP_HALT_OUT;


/******************************************************************************
* \struct MXP_MOVEABSOLUTE_IN
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVEABSOLUTE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.6 The input 'Continuous update'
	FLOAT					Position;											// <float> Target position for the motion (in technical unit [u]) (negative or positive) 
	FLOAT					Velocity;											// <float> Value of the maximum velocity (always positive) (not necessarily reached) [u/s].
	FLOAT					Acceleration;										// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (always positive) (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]. (always positive)
	MXP_DIRECTION_ENUM		Direction;											// Defines 'MXP_DIRECTION_ENUM'
	MXP_BUFFERMODE_ENUM		BufferMode;											// MXP_BUFFERMODE_ENUM Defines the behavior of the axis: modes are Aborting, Buffered, Blending

} _MXP_MOVEABSOLUTE_IN;


/******************************************************************************
* \struct MXP_MOVEABSOLUTE_EX_IN
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVEABSOLUTE_EX_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	UINT16					CommandBlockNo;									// SequenceMove Cmd Count
	UINT8					Execute;											// <UINT8> Start the motion at rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.6 The input 'Continuous update'
	FLOAT					Position;											// <float> Target position for the motion (in technical unit [u]) (negative or positive) 
	FLOAT					Velocity;											// <float> Value of the maximum velocity (always positive) (not necessarily reached) [u/s].
	FLOAT					Acceleration;										// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (always positive) (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]. (always positive)
	MXP_DIRECTION_ENUM		Direction;											// Defines 'MXP_DIRECTION_ENUM'
	MXP_BUFFERMODE_ENUM		BufferMode;											// MXP_BUFFERMODE_ENUM Defines the behavior of the axis: modes are Aborting, Buffered, Blending

} _MXP_MOVEABSOLUTE_EX_IN;

/******************************************************************************
* \struct MXP_MOVEABSOLUTE_OUT
* \brief Move Absolute Command output data structure.
*******************************************************************************/
typedef struct MXP_MOVEABSOLUTE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> Commanded position finally reached.										
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_MOVEABSOLUTE_OUT;


/******************************************************************************
* \struct MXP_MOVERELATIVE_IN
* \brief Move Relative Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVERELATIVE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.6 The input 'ContinuousUpdate'
	FLOAT					Distance;											// <float> Relative distance for the motion (in technical unit [u])
	FLOAT					Velocity;											// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Acceleration;										// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (always positive) (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]. (always positive)
	MXP_BUFFERMODE_ENUM		BufferMode;											// Defines the chronological sequence of the 'MXP_BUFFERMODE_ENUM' Aborting versus Buffered modes. 
		
} _MXP_MOVERELATIVE_IN;

/******************************************************************************
* \struct MXP_MOVERELATIVE_EX_IN
* \brief Move Relative Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVERELATIVE_EX_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	UINT16					CommandBlockNo;									// SequenceMove Cmd Count	
	UINT8					Execute;											// <UINT8> Start the motion at rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.6 The input 'ContinuousUpdate'
	FLOAT					Distance;											// <float> Relative distance for the motion (in technical unit [u])
	FLOAT					Velocity;											// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Acceleration;										// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (always positive) (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]. (always positive)
	MXP_BUFFERMODE_ENUM		BufferMode;											// Defines the chronological sequence of the 'MXP_BUFFERMODE_ENUM' Aborting versus Buffered modes. 

} _MXP_MOVERELATIVE_EX_IN;

/******************************************************************************
* \struct MXP_MOVERELATIVE_OUT
* \brief Move Relative Command output data structure.
*******************************************************************************/
typedef struct MXP_MOVERELATIVE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> Commanded distance reached.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_MOVERELATIVE_OUT;


/******************************************************************************
* \struct MXP_MOVEADDITIVE_IN
* \brief Move Additive Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVEADDITIVE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Start hte motion at rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.6 The input 'ContinuousUpdate'
	FLOAT					Distance;											// <float> Relative distance for the motion (in technical unit [u])
	FLOAT					Velocity;											// <float> Value of the velocity (always positive) (increasing energy of the motor) [u/s ]
	FLOAT					Acceleration;										// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (always positive) (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]. (always positive)
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_MOVEADDITIVE_IN;

/******************************************************************************
* \struct MXP_MOVEADDITIVE_OUT
* \brief Move Additive Command output data structure.
*******************************************************************************/
typedef struct MXP_MOVEADDITIVE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> Commanded distance reached.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	
} _MXP_MOVEADDITIVE_OUT;


/******************************************************************************
* \struct MXP_MOVESUPERIMPOSED_IN
* \brief Move Superimposed Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVESUPERIMPOSED_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	
	UINT8					Excute;												// <UINT8> Start the motion at rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.6 The input 'ContinuousUpdate'
	FLOAT					Distance;											// <float> Additional distance that is to be superimposed (in technical unit [u])
	FLOAT					VelocityDeff;										// <float> Value of the velocity difference of the additional motion (not necessarily reached) [u/s]
	FLOAT					Acceleration;										// <float> Value of the acceleration (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]
	
} _MXP_MOVESUPERIMPOSED_IN;

/******************************************************************************
* \struct MXP_MOVESUPERIMPOSED_IN
* \brief Move Superimposed Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVESUPERIMPOSED_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// Additional distance superimposed to the ongoing motion.
	UINT8					Busy;												// The FB is not finished and new output values are to be expected.
	UINT8					CommandAborted;										// 'Command' is aborted by another command.
	UINT8					Error;												// Signal that an error has occurred within the Function Block.
	UINT16					ErrorID;											// Error identification.
	FLOAT					CoveredDistance;									// Displays continuously the covered distance contributed by this FB since it was started.
	
} _MXP_MOVESUPERIMPOSED_OUT;


/******************************************************************************
* \struct MXP_HALTSUPERIMPOSED_IN
* \brief Halt Superimposed Command input data structure.
*******************************************************************************/
typedef struct MXP_HALTSUPERIMPOSED_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Start the action at rising edge.
	FLOAT					Deceleration;										// <float> Value of the deceleration [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]

} _MXP_HALTSUPERIMPOSED_IN;

/******************************************************************************
* \struct MXP_HALTSUPERIMPOSED_OUT
* \brief Halt Superimposed Command output data structure.
*******************************************************************************/
typedef struct MXP_HALTSUPERIMPOSED_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> Superimposed motion halted
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_HALTSUPERIMPOSED_OUT;

/******************************************************************************
* \struct MXP_ET_READPARAMETER_IN
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_READPARAMETER_IN
{
	UINT32					SlaveNo;											// Slave No.
	UINT32					Index;												// Index in parameter
	UINT32					SubIndex;											// Sub index in parameter
	UINT32					BufLen;												// Read buffer size	(*)

} _MXP_ET_READPARAMETER_IN;

/******************************************************************************
* \struct MXP_ET_READPARAMETER_OUT
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_READPARAMETER_OUT
{
	UINT32					SlaveNo;											// Slave No.

	UINT8					Valid;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;

	UINT8					Data[ETHERCAT_SDO_SIZE];

} _MXP_ET_READPARAMETER_OUT;

/******************************************************************************
* \struct MXP_ET_WRITEPARAMETER_IN
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_WRITEPARAMETER_IN
{
	UINT32					SlaveNo;											// Slave No.
	UINT32					Index;												// Index in parameter
	UINT32					SubIndex;											// Sub index in parameter
	UINT32					BufLen;												// Read buffer size	(*)

	UINT32					Data;

} _MXP_ET_WRITEPARAMETER_IN;

/******************************************************************************
* \struct MXP_ET_WRITEPARAMETER_OUT
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_WRITEPARAMETER_OUT
{
	UINT32					SlaveNo;											// Slave No.

	UINT8					Done;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;

} _MXP_ET_WRITEPARAMETER_OUT;

/******************************************************************************
* \struct MXP_MOVEVELOCITY_IN
* \brief Move Velocity Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVEVELOCITY_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.6 The input 'ContinuousUpdate'
	FLOAT					Velocity;											// <float> Value of the maximum velocity [u/s]. Can be a signed value.
	FLOAT					Acceleration;										// <float> Value of the acceleration (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]
	MXP_DIRECTION_ENUM		Direction;											// < MC_Direction Enumerator type (1-of-4 values: positive_direction, shortest_way, negative_direction, current_direction)
	MXP_BUFFERMODE_ENUM		BufferMode;											// < MC_BufferMode Defines the behavior of the axis: modes are Aborting, Buffered, Blending
	
} _MXP_MOVEVELOCITY_IN;

/******************************************************************************
* \struct MXP_MOVEVELOCITY_OUT
* \brief Move Velocity Command output data structure.
*******************************************************************************/
typedef struct MXP_MOVEVELOCITY_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					InVelocity;											// Commanded velocity reached.
	UINT8					Busy;												// The FB is not finished and new output values are to be expected.
	UINT8					Active;												// Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// 'Command' is aborted by another command.
	UINT8					Error;												// Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// Error identification.

} _MXP_MOVEVELOCITY_OUT;


/******************************************************************************
* \struct MXP_MOVEVELOCITY_IN
* \brief Move Velocity Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVECONTINUOUSABSOLUTE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.6 The input 'ContinuousUpdate'
	FLOAT					Position;											// <float> Commanded position for the motion (in technical unit [u]) (negative or positive)
	FLOAT					EndVelocity;										// <float> Value of the end velocity [u/s]. Signed value.
	FLOAT					Velocity;											// <float> Value of the maximum velocity [u/s²]
	FLOAT					Acceleration;										// <float> Value of the acceleration (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_MOVECONTINUOUSABSOLUTE_IN;

/******************************************************************************
* \struct MXP_MOVECONTINUOUSABSOLUTE_OUT
* \brief Move Velocity Command output data structure.
*******************************************************************************/
typedef struct MXP_MOVECONTINUOUSABSOLUTE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					InEndVelocity;										// <UINT8> Commanded distance reached and running at requested end velocity.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_MOVECONTINUOUSABSOLUTE_OUT;


/******************************************************************************
* \struct MXP_MOVECONTINUOUSRELATIVE_IN
* \brief Move Continuous Relative Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVECONTINUOUSRELATIVE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.6 The input 'ContinuousUpdate'
	FLOAT					Distance;											// <float> Relative distance for the motion [u]
	FLOAT					EndVelocity;										// <float> Value of the end velocity [u/s]. Signed value.
	FLOAT					Velocity;											// <float> Value of the maximum velocity [u/s²]
	FLOAT					Acceleration;										// <float> Value of the acceleration (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_MOVECONTINUOUSRELATIVE_IN;

/******************************************************************************
* \struct MXP_MOVECONTINUOUSRELATIVE_OUT
* \brief Move Continuous Relative Command output data structure.
*******************************************************************************/
typedef struct MXP_MOVECONTINUOUSRELATIVE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					InEndVelocity;										// <UINT8> Commanded distance reached and running at requested end velocity.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_MOVECONTINUOUSRELATIVE_OUT;

/******************************************************************************
* \struct MXP_MOVEDIRECTPOSITION_IN
* \brief Position control Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVEDIRECTPOSITION_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Starts the motion on a rising edge.
	FLOAT					Position;											// <float> Value of the Position (in technical unit [u])

} _MXP_MOVEDIRECTPOSITION_IN;

/******************************************************************************
* \struct MXP_MOVEDIRECTPOSITION_OUT
* \brief Position control Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVEDIRECTPOSITION_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;											// <UINT8> Setpoint value of torque or force equals the commanded value.			// Not supported (Reserved)
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_MOVEDIRECTPOSITION_OUT;

/******************************************************************************
* \struct MXP_DIRECTTORQUECONTROL_IN
* \brief Torque control Command input data structure.
*******************************************************************************/
typedef struct MXP_DIRECTTORQUECONTROL_IN
{
	MXP_AXIS_REF			Axis;											// Reference to the axis

	UINT8					Enable;											// <UINT8> Starts the motion on a rising edge.
	FLOAT					Torque;											// <float> Value of the torque (in technical unit [u])

} _MXP_DIRECTTORQUECONTROL_IN;

/******************************************************************************
* \struct MXP_DIRECTTORQUECONTROL_OUT
* \brief Torque control Command input data structure.
*******************************************************************************/
typedef struct MXP_DIRECTTORQUECONTROL_OUT
{
	MXP_AXIS_REF			Axis;											// Reference to the axis

	UINT8					Done;											// <UINT8> Setpoint value of torque or force equals the commanded value.			// Not supported (Reserved)
	UINT8					Busy;											// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;											// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;									// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;											// <UINT8> Signals that an error has occurred within the Function block.
	UINT16					ErrorID;										// <UINT16> Error identification.

} _MXP_DIRECTTORQUECONTROL_OUT;

/******************************************************************************
* \struct MXP_MOVEDIRECTVELOCITY_IN
* \brief Velocity control Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVEDIRECTVELOCITY_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Starts the motion on a rising edge.
	FLOAT					Velocity;											// <float> Value of the velocity (in technical unit [u])

} _MXP_MOVEDIRECTVELOCITY_IN;

/******************************************************************************
* \struct MXP_MOVEDIRECTVELOCITY_OUT
* \brief Velocity control Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVEDIRECTVELOCITY_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;											// <UINT8> Setpoint value of torque or force equals the commanded value.			// Not supported (Reserved)
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_MOVEDIRECTVELOCITY_OUT;

/******************************************************************************
* \struct MXP_TORQUECONTROL_IN
* \brief Torque control Command input data structure.
*******************************************************************************/
typedef struct MXP_TORQUECONTROL_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Starts the motion on a rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.5 The input 'ContinuousUpdate'																			// Not supported (Reserved)
	FLOAT					Torque;												// <float> Value of the torque (Torque or force in technical unit [u])
	FLOAT					TorqueRamp;											// <float> The maximum time derivative of the set value of the torque or force (in technical unit per sec. [u/s]			// Not supported (Reserved)
	FLOAT					Velocity;											// <float> Absolute value of the maximum velocity.																			// Not supported (Reserved)
	FLOAT					Acceleration;										// <float> Value of the maximum acceleration (acceleration is applicable with same sign of torque and velocity)				// Not supported (Reserved)
	FLOAT					Deceleration;										// <float> Value of the maximum deceleration (deceleration is applicable with opposite signs of torque and velocity)		// Not supported (Reserved)
	FLOAT					Jerk;												// <float> Value of the maximum jerk.																						// Not supported (Reserved)
	MXP_DIRECTION_ENUM		Direction;											// < MC_Direction Enumerator type (1-of-4 values: positive_direction, shortest_way, negative_direction, current_direction)	// Not supported (Reserved)
	MXP_BUFFERMODE_ENUM		BufferMode;											// < MC_BufferMode Defines the behavior of the axis: modes are Aborting, Buffered, Blending									// Not supported (Reserved)
		
} _MXP_TORQUECONTROL_IN;


/******************************************************************************
* \struct MXP_FEEDBACK_IN
* \brief Torque control Feedback Command input data structure.
*******************************************************************************/
typedef struct MXP_FEEDBACK_IN
{
	MXP_INPUT_REF			Input;												// Reference to the Input

	UINT16					Offset;												// <UINT16> 
	UINT16					Size;												// <UINT16> 
	UINT16					DataRange;											// <UINT16> 
	
	FLOAT					PGain;												// <float>
	FLOAT					IGain;												// <float>
	FLOAT					DGain;												// <float>
	
} _MXP_FEEDBACK_IN;

/******************************************************************************
* \struct MXP_TORQUECONTROL_OUT
* \brief Torque control Command input data structure.
*******************************************************************************/
typedef struct MXP_TORQUECONTROL_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					InTorque;											// <UINT8> Setpoint value of torque or force equals the commanded value.			// Not supported (Reserved)
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_TORQUECONTROL_OUT;


/******************************************************************************
* \struct MXP_POSITIONPROFILE_IN
* \brief Position profile Command input data structure.
*******************************************************************************/
typedef struct MXP_POSITIONPROFILE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	MXP_TP_REF				TimePosition;										// Reference to Time / Position. Description - see note below

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.6 The input 'ContinuousUpdate'
	FLOAT					TimeScale;											// <float> Overall time scaling factor of the profile.
	FLOAT					PositionScale;										// <float> Overall Position scaling factor.
	FLOAT					Offset;												// <float> Overall offset for profile [u]
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_POSITIONPROFILE_IN;

/******************************************************************************
* \struct MXP_POSITIONPROFILE_OUT
* \brief Position profile Command output data structure.
*******************************************************************************/
typedef struct MXP_POSITIONPROFILE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	MXP_TP_REF				TimePosition;										// Reference to Time / Position. Description - see note below

	UINT8					Done;												// <UINT8> Profile completed
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_POSITIONPROFILE_OUT;


/******************************************************************************
* \struct MXP_VELOCITYPROFILE_IN
* \brief Velocity profile Command input data structure.
*******************************************************************************/
typedef struct MXP_VELOCITYPROFILE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	MXP_TV_REF				TimeVelocity;										// Reference to Time / Velocity. Description - See note below

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.6 The input 'ContinuousUpdate'
	FLOAT					TimeScale;											// <float> Overall time scaling factor of the profile.
	FLOAT					VelocityScale;										// <float> Overall velocity scaling factor of the profile.
	FLOAT					Offset;												// <float> Overall offset for profile [u]
	MXP_BUFFERMODE_ENUM		BufferMode;											// < MC_BufferMode Defines the behavior of the axis: modes are Aborting, Buffered, Blending

} _MXP_VELOCITYPROFILE_IN;

/******************************************************************************
* \struct MXP_VELOCITYPROFILE_OUT
* \brief Velocity profile Command output data structure.
*******************************************************************************/
typedef struct MXP_VELOCITYPROFILE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	MXP_TV_REF				TimeVelocity;										// Reference to Time / Velocity. Description

	UINT8					ProfileCompleted;									// End of profile reached.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_VELOCITYPROFILE_OUT;


/******************************************************************************
* \struct MXP_ACCELERATIONPROFILE_IN
* \brief Acceleration profile Command input data structure.
*******************************************************************************/
typedef struct MXP_ACCELERATIONPROFILE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	MXP_TA_REF				TimeAcceleration;									// Reference to Time / Velocity. Description

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.
	UINT8					ContinuousUpdate;									// <UINT8> See 2.4.6 The input 'ContinuousUpdate'
	FLOAT					TimeScale;											// <float> Overall time scaling factor of the profile.
	FLOAT					AccelerationScale;									// <float> Scale factor for Acceleration ammplitude.
	FLOAT					Offset;												// <float> Overall offset for profile [u]
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_ACCELERATIONPROFILE_IN;

/******************************************************************************
* \struct MXP_ACCELERATIONPROFILE_OUT
* \brief Acceleration profile Command output data structure.
*******************************************************************************/
typedef struct MXP_ACCELERATIONPROFILE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	MXP_TA_REF				TimeAcceleration;									// Reference to Time / Acceleration. Description

	UINT8					ProfileCompleted;									// End of profile reached
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_ACCELERATIONPROFILE_OUT;


/******************************************************************************
* \struct MXP_SETPOSITION_IN
* \brief Set position Command input data structure.
*******************************************************************************/
typedef struct MXP_SETPOSITION_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Start setting position in axis.
	FLOAT					Position;											// <float> Position unit [u] (Means 'Distance' if 'Relative' = TRUE)
	UINT8					Relative;											// <float> 'Relative' distance if True, 'Absolute' position if False (= Default)
	MXP_EXECUTIONMODE_ENUM	ExecutionMode;
		
} _MXP_SETPOSITION_IN;

/******************************************************************************
* \struct MXP_SETPOSITION_OUT
* \brief Set position Command output data structure.
*******************************************************************************/
typedef struct MXP_SETPOSITION_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> 'Position' has new value.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification
	
} _MXP_SETPOSITION_OUT;


/******************************************************************************
* \struct MXP_SETPOSITION_IN
* \brief Set position Command input data structure.
*******************************************************************************/
typedef struct MXP_SETDIRECTPOSITION_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;											// <UINT8> Start setting position in axis.
	FLOAT					Position;											// <float> Position unit [u] (Means 'Distance' if 'Relative' = TRUE)
	UINT8					Relative;											// <float> 'Relative' distance if True, 'Absolute' position if False (= Default)
	MXP_EXECUTIONMODE_ENUM	ExecutionMode;

} _MXP_SETDIRECTPOSITION_IN;

/******************************************************************************
* \struct MXP_SETPOSITION_OUT
* \brief Set position Command output data structure.
*******************************************************************************/
typedef struct MXP_SETDIRECTPOSITION_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enabled;												// <UINT8> 'Position' has new value.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification

} _MXP_SETDIRECTPOSITION_OUT;

/******************************************************************************
* \struct MXP_SETOVERRIDE_IN
* \brief Set Override Command input data structure.
*******************************************************************************/
typedef struct MXP_SETOVERRIDE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> If SET, if writes the value of the override factor continuously. If RESET it should keep the last value.
	FLOAT					VelFactor;											// <float> New override factor for the velocity.
	FLOAT					AccFactor;											// <float> New override factor for the acceleration / deceleration.
	FLOAT					JerkFactor;											// <float> New override factor for the jerk.

} _MXP_SETOVERRIDE_IN;

/******************************************************************************
* \struct MXP_SETOVERRIDE_OUT
* \brief Set Override Command output data structure.
*******************************************************************************/
typedef struct MXP_SETOVERRIDE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enabled;											// <UINT8> Signals that the override factor(s) is (are0 set successfully.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_SETOVERRIDE_OUT;


/******************************************************************************
* \struct MXP_READPARAMETER_IN
* \brief Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_READPARAMETER_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the parameter continuously while enable.
	INT32					ParameterNumber;									// <INT> Number of the parameter. One can also use symbolic parameter names which are declared as VAR CONST.
	
} _MXP_READPARAMETER_IN;

/******************************************************************************
* \struct MXP_READPARAMETER_OUT
* \brief Read Parameter Command output data structure.
*******************************************************************************/
typedef struct MXP_READPARAMETER_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					Value;												// <float> Value of the specified parameter in the data type, as specified by the vendor.

} _MXP_READPARAMETER_OUT;


/******************************************************************************
* \struct MXP_READUINT8PARAMETER_IN
* \brief Read UINT8 Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_READBOOLPARAMETER_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the parameter continuously while enable.
	INT32					ParameterNumber;									// <INT> Number of the parameter. One can also use symbolic parameter names which are declared as VAR CONST.

} _MXP_READUINT8PARAMETER_IN;

/******************************************************************************
* \struct MXP_READPARAMETER_OUT
* \brief Read Parameter Command output data structure.
*******************************************************************************/
typedef struct MXP_READBOOLPARAMETER_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	UINT8					Value;												// <float> Value of the specified parameter in the datatype, as specified by the vendor.

} _MXP_READUINT8PARAMETER_OUT;


/******************************************************************************
* \struct MXP_WRITEPARAMETER_IN
* \brief Read UINT8 Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_WRITEPARAMETER_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Get the value of the parameter continuously while enable.
	INT32					ParameterNumber;									// <INT> Number of the parameter. One can also use symbolic parameter names which are declared as VAR CONST.
	FLOAT					Value;												// <float> New value of the specified parameter.
	MXP_EXECUTIONMODE_ENUM	ExecutionMode;

} _MXP_WRITEPARAMETER_IN;

/******************************************************************************
* \struct MXP_WRITEPARAMETER_OUT
* \brief Write Parameter Command output data structure.
*******************************************************************************/
typedef struct MXP_WRITEPARAMETER_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;											// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	
} MXP_WRITEPARAMETER_OUT;

/******************************************************************************
* \struct MXP_WRITEPARAMETEREX_IN
* \brief Read UINT8 Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_WRITEPARAMETEREX_IN
{
	UINT8					Execute;											// <UINT8> Get the value of the parameter continuously while enable.
	MXP_EXECUTIONMODE_ENUM	ExecutionMode;

} _MXP_WRITEPARAMETEREX_IN;

/******************************************************************************
* \struct MXP_WRITEPARAMETEREX_OUT
* \brief Write Parameter Command output data structure.
*******************************************************************************/
typedef struct MXP_WRITEPARAMETEREX_OUT
{
	UINT8					Done;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} MXP_WRITEPARAMETEREX_OUT;

/******************************************************************************
* \struct MXP_WRITEUINT8PARAMETER_IN
* \brief Write UINT8 Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_WRITEBOOLPARAMETER_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;												// <UINT8> Get the value of the parameter continuously while enable.
	INT32					ParameterNumber;									// <INT> Number of the parameter. One can also use symbolic parameter names which are declared as VAR CONST.
	UINT8					Value;												// <float> New value of the specified parameter.
	MXP_EXECUTIONMODE_ENUM	ExecutionMode;

} _MXP_WRITEUINT8PARAMETER_IN;

/******************************************************************************
* \struct MXP_WRITEUINT8PARAMETER_OUT
* \brief Write UINT8 Parameter Command output data structure.
*******************************************************************************/
typedef struct MXP_WRITEBOOLPARAMETER_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_WRITEUINT8PARAMETER_OUT;


/******************************************************************************
* \struct MXP_READDIGITALINPUT_IN
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_READDIGITALINPUT_IN
{
	MXP_INPUT_REF			Input;												// Reference to the Input
	
	UINT8					Enable;												// Get the value of the selected input signal continuously while enabled.
	INT32					InputNumber;

} _MXP_READDIGITALINPUT_IN;

/******************************************************************************
* \struct MXP_READDIGITALINPUT_OUT
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_READDIGITALINPUT_OUT
{
	MXP_INPUT_REF			Input;												// Reference to the input

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	UINT8					Value;												// <UINT8> The value of the selected input signal.

} _MXP_READDIGITALINPUT_OUT;


/******************************************************************************
* \struct MXP_READDIGITALOUTPUT_IN
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_READDIGITALOUTPUT_IN
{
	MXP_OUTPUT_REF			Output;											// Reference to the output
	
	UINT8					Enable;											// Get the value of the selected input signal continuously while enabled.
	INT32					OutputNumber;

} _MXP_READDIGITALOUTPUT_IN;

/******************************************************************************
* \struct MXP_READDIGITALINPUT_OUT
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_READDIGITALOUTPUT_OUT
{
	MXP_OUTPUT_REF			Output;												// Reference to the output

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	UINT8					Value;												// <UINT8> The value of the selected output signal.

} _MXP_READDIGITALOUTPUT_OUT;


/******************************************************************************
* Not supported yet.
* \struct MXP_WRITEDIGITALOUTPUT_IN
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_WRITEDIGITALOUTPUT_IN
{
	MXP_OUTPUT_REF			Output;											// Reference to the output.

	UINT8					Execute;											// <UINT8> Get the value of the selected input signal continuously while enabled.
	INT32					OutputNumber;										// <INT> Selects the output. Can be part of MC_OUTPUT_REF, if only one single output is referenced.
	UINT8					Value;												// <float> New value of the specified parameter.
	MXP_EXECUTIONMODE_ENUM	ExecutionMode;

} _MXP_WRITEDIGITALOUTPUT_IN;

/******************************************************************************
* Not supported yet.
* \struct MXP_READDIGITALINPUT_OUT
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_WRITEDIGITALOUTPUT_OUT
{
	MXP_OUTPUT_REF			Output;											// Reference to the output.

	UINT8					Done;												// <UINT8> Writing of the ouput signal value is done.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_WRITEDIGITALOUTPUT_OUT;

/******************************************************************************
* \struct MXP_WRITEFORCEDIGITALINPUT_IN
* \brief Write Force Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_WRITEFORCEDIGITALINPUT_IN
{
	MXP_INPUT_REF			Input;												// Reference to the Input.

	UINT8					Execute;											// <UINT8> Get the value of the selected input signal continuously while enabled.
	INT32					InputNumber;										// <INT> Selects the input.
	UINT8					Value;												// <float> New value of the specified parameter.
	MXP_EXECUTIONMODE_ENUM	ExecutionMode;

} _MXP_WRITEFORCEDIGITALINPUT_IN;

/******************************************************************************
* \struct MXP_WRITEFORCEDIGITALINPUT_OUT
* \brief Write Force Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_WRITEFORCEDIGITALINPUT_OUT
{
	MXP_INPUT_REF			Input;												// Reference to the Input.

	UINT8					Done;												// <UINT8> Writing of the ouput signal value is done.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_WRITEFORCEDIGITALINPUT_OUT;


/******************************************************************************
* \struct MXP_READINPUTS_IN
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_READINPUTS_IN
{
	MXP_INPUT_REF			Input;												// Reference to the Input

	UINT8					Enable;												// Get the value of the selected input signal continuously while enabled.

} _MXP_READINPUTS_IN;

/******************************************************************************
* \struct MXP_READDIGITALINPUT_OUT
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_READINPUTS_OUT
{
	MXP_INPUT_REF			Input;												// Reference to the input

	UINT8					Valid;												// <BOOL> A valid output is available at the FB.
	UINT8					Busy;												// <BOOL> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <BOOL> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <WORD> Error identification.
	UINT16					Size;												// <WORD> Reading data size.

} _MXP_READINPUTS_OUT;


/******************************************************************************
* \struct MXP_READDIGITALOUTPUT_IN
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_READOUTPUTS_IN
{
	MXP_OUTPUT_REF			Output;											// Reference to the output

	UINT8					Enable;											// Get the value of the selected input signal continuously while enabled.

} _MXP_READOUTPUTS_IN;

/******************************************************************************
* \struct MXP_READDIGITALINPUT_OUT
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_READOUTPUTS_OUT
{
	MXP_OUTPUT_REF			Output;												// Reference to the output

	UINT8					Valid;												// <BOOL> A valid output is available at the FB.
	UINT8					Busy;												// <BOOL> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <BOOL> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <WORD> Error identification.
	UINT16					Size;												// <WORD> Reading data size.

} _MXP_READOUTPUTS_OUT;


/******************************************************************************
* \struct MXP_WRITEOUTPUTS_IN
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_WRITEOUTPUTS_IN
{
	MXP_OUTPUT_REF			Output;											// Reference to the output.

	UINT8					Execute;										// <BOOL> Get the value of the selected input signal continuously while enabled.
	UINT16					Size;											// <WORD> Reading data size.

} _MXP_WRITEOUTPUTS_IN;

/******************************************************************************
* \struct MXP_WRITEOUTPUTS_OUT
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_WRITEOUTPUTS_OUT
{
	MXP_OUTPUT_REF			Output;											// Reference to the output.

	UINT8					Done;												// <BOOL> Writing of the ouput signal value is done.
	UINT8					Busy;												// <BOOL> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <BOOL> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <WORD> Error identification.

} _MXP_WRITEOUTPUTS_OUT;


/******************************************************************************
* \struct MXP_WRITEFORCEINPUTS_IN
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_WRITEFORCEINPUTS_IN
{
	MXP_INPUT_REF			Input;											// Reference to the Input.

	UINT8					Execute;										// <BOOL> Get the value of the selected input signal continuously while enabled.
	UINT16					Size;											// <WORD> Reading data size.
	UINT16					Offset;										//[2018.01.30] modified by jinsil - pInParam->Offset 추가
} _MXP_WRITEFORCEINPUTS_IN;

/******************************************************************************
* \struct MXP_WRITEFORCEINPUTS_OUT
* \brief Read Digital Input Command input data structure.
*******************************************************************************/
typedef struct MXP_WRITEFORCEINPUTS_OUT
{
	MXP_INPUT_REF			Input;											// Reference to the Input.

	UINT8					Done;											// <BOOL> Writing of the ouput signal value is done.
	UINT8					Busy;											// <BOOL> The FB is not finished and new output values are to be expected.
	UINT8					Error;											// <BOOL> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;										// <WORD> Error identification.

} _MXP_WRITEFORCEINPUTS_OUT;

/******************************************************************************
* \struct MXP_READACTUALPOSITION_IN
* \brief Read Actual Position Command input data structure.
*******************************************************************************/
typedef struct MXP_READACTUALPOSITION_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the selected input signal continuously while enabled.
	
} _MXP_READACTUALPOSITION_IN;

/******************************************************************************
* \struct MXP_READACTUALPOSITION_OUT
* \brief Read Actual Position Command output data structure.
*******************************************************************************/
typedef struct MXP_READACTUALPOSITION_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					Position;											// <float> New absolute position. (in axis' unit [u])
	
} _MXP_READACTUALPOSITION_OUT;


/******************************************************************************
* \struct MXP_READACTUALVELOCITY_IN
* \brief Read Actual Velocity Command input data structure.
*******************************************************************************/
typedef struct MXP_READACTUALVELOCITY_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the parameter continuously while enabled.
	
} _MXP_READACTUALVELOCITY_IN;

/******************************************************************************
* \struct MXP_READACTUALPOSITION_OUT
* \brief Read Actual Position Command output data structure.
*******************************************************************************/
typedef struct MXP_READACTUALVELOCITY_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					Velocity;											// <float> The value of the actual velocity (in axis' unit [u/s])
	
} _MXP_READACTUALVELOCITY_OUT;

/******************************************************************************
* \struct MXP_READFOLLOWINGERRORVALUE_IN
* \brief Read Following Error Value Command input data structure.
*******************************************************************************/
typedef struct MXP_READFOLLOWINGERRORVALUE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the parameter continuously while enabled.

} _MXP_READFOLLOWINGERRORVALUE_IN;

/******************************************************************************
* \struct MXP_READFOLLOWINGERRORVALUE_OUT
* \brief Read Following Error Value Command Command output data structure.
*******************************************************************************/
typedef struct MXP_READFOLLOWINGERRORVALUE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					FollowingErrorValue;											

} _MXP_READFOLLOWINGERRORVALUE_OUT;
/******************************************************************************
* \struct MXP_READACTUALTORQUE_IN
* \brief Read Actual Velocity Command input data structure.
*******************************************************************************/
typedef struct MXP_READACTUALTORQUE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the parameter continuously while enabled.

} _MXP_READACTUALTORQUE_IN;

/******************************************************************************
* \struct MXP_READACTUALTORQUE_OUT
* \brief Read Actual Position Command output data structure.
*******************************************************************************/
typedef struct MXP_READACTUALTORQUE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					Torque;												// <float> The value of the actual velocity (in axis' unit [u/s])

} _MXP_READACTUALTORQUE_OUT;


/******************************************************************************
* Not supported yet.
* \struct MXP_READACTUALFORCE_IN
* \brief Read Actual Velocity Command input data structure.
*******************************************************************************/
typedef struct MXP_READACTUALFORCE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the parameter continuously while enabled.

} _MXP_READACTUALFORCE_IN;

/******************************************************************************
* Not supported yet.
* \struct MXP_READACTUALFORCE_OUT
* \brief Read Actual Position Command output data structure.
*******************************************************************************/
typedef struct MXP_READACTUALFORCE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					Force;												// <float> The value of the actual force (in axis' unit [u/s])

} _MXP_READACTUALFORCE_OUT;



/******************************************************************************
* \struct MXP_READSTATUS_IN
* \brief Read Status Command input data structure.
*******************************************************************************/
typedef struct MXP_READSTATUS_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the parameter continuously while enabled.

} _MXP_READSTATUS_IN;

/******************************************************************************
* \struct MXP_READSTATUS_OUT
* \brief Read Status Command output data structure.
*******************************************************************************/
typedef struct MXP_READSTATUS_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	UINT8					ErrorStop;											// <UINT8> See state diagram.
	UINT8					Disabled;											// <UINT8> See state diagram.
	UINT8					Stopping;											// <UINT8> See state diagram.
	UINT8					Homing;												// <UINT8> See state diagram.
	UINT8					Standstill;											// <UINT8> See state diagram.
	UINT8					DiscreteMotion;										// <UINT8> See state diagram.
	UINT8					ContinuousMotion;									// <UINT8> See state diagram.
	UINT8					SynchronizedMotion;									// <UINT8> See state diagram.
	
} _MXP_READSTATUS_OUT;


/******************************************************************************
* \struct MXP_READMOTIONSTATE_IN
* \brief Read Motion state Command input data structure.
*******************************************************************************/
typedef struct MXP_READMOTIONSTATE_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the parameter continuously while enabled.
	MXP_SOURCE_ENUM			Source;

} _MXP_READMOTIONSTATE_IN;

/******************************************************************************
* \struct MXP_READMOTIONSTATE_OUT
* \brief Read Status Command output data structure.
*******************************************************************************/
typedef struct MXP_READMOTIONSTATE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	UINT8					ConstantVelocity;									// <UINT8> Velocity is constant. Velocity may be 0. For the actual value a windows is applicable (window is vendor specific)
	UINT8					Accelerating;										// <UINT8> Increasing the absolute value of the velocity.
	UINT8					Decelerating;										// <UINT8> Decreasing the absolute value of the velocity.
	UINT8					DirectionPositive;									// <UINT8> Signals that the position is increasing.
	UINT8					DirectionNegative;									// <UINT8> Signals that the position is decreasing.

} _MXP_READMOTIONSTATE_OUT;


/******************************************************************************
* \struct MXP_READAXISINFO_IN
* \brief Read Axis Info Command input data structure.
*******************************************************************************/
typedef struct MXP_READAXISINFO_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get The axis information constantly while enabled.
	
} _MXP_READAXISINFO_IN;

/******************************************************************************
* \struct MXP_READAXISINFO_OUT
* \brief Read Axis Info Command output data structure.
*******************************************************************************/
typedef struct MXP_READAXISINFO_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;
	UINT8					HomeAbsSwitch;
	UINT8					LimitSwitchPos;
	UINT8					LimitSwitchNeg;
	UINT8					Simulation;
	UINT8					CommunicationReady;
	UINT8					ReadyForPowerOn;
	UINT8					PowerOn;
	UINT8					IsHomed;
	UINT8					AxisWarning;

} _MXP_READAXISINFO_OUT;


/******************************************************************************
* \struct MXP_READAXISERROR_IN
* \brief Read Axis Error Command input data structure.
*******************************************************************************/
typedef struct MXP_READAXISERROR_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the parameter continuously while enabled.

} _MXP_READAXISERROR_IN;

/******************************************************************************
* \struct MXP_READAXISERROR_OUT
* \brief Read Axis Error Command output data structure.
*******************************************************************************/
typedef struct MXP_READAXISERROR_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> True if a valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error 
	UINT16					ErrorID;
	UINT16					AxisErrorID;
	UINT16					AuxErrorID;											// Vendor specific.
	
} _MXP_READAXISERROR_OUT;


/******************************************************************************
* \struct MXP_RESET_IN
* \brief Reset Command input data structure.
*******************************************************************************/
typedef struct MXP_RESET_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Resets all internal axis-related errors.
	 
} _MXP_RESET_IN;

/******************************************************************************
* \struct MXP_RESET_OUT
* \brief Reset Command output data structure.
*******************************************************************************/
typedef struct MXP_RESET_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> 'Standstill' or 'Disabled' state is reached.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_RESET_OUT;

/******************************************************************************
* \struct MXP_GROUPSTOP_IN
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_GROUPSTOP_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axis.

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.
	FLOAT					Deceleration;										
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]. (always positive)
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_GROUPSTOP_IN;

/******************************************************************************
* \struct MXP_GROUPSTOP_OUT
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_GROUPSTOP_OUT
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axis.

	UINT8					Done;											
	UINT8					Busy;
	UINT8					Active;
	UINT8					CommandAborted;											
	UINT8					Error;											
	UINT16					ErrorID;							

} _MXP_GROUPSTOP_OUT;

/******************************************************************************
* \struct MXP_MOVE_LINEARABSOLUTE_IN
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVELINEARABSOLUTE_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axis.

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.

	FLOAT					PositionX;											
	FLOAT					PositionY;
	FLOAT					PositionZ;
	FLOAT					PositionU;

	FLOAT					PositionV;											
	FLOAT					PositionW;
	FLOAT					PositionA;
	FLOAT					PositionB;
	FLOAT					PositionC;

	FLOAT					Velocity;											// <float> Value of the maximum velocity (always positive) (not necessarily reached) [u/s].
	FLOAT					Acceleration;										// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (always positive) (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]. (always positive)

	MXP_COORDSYSTEM_ENUM	CoordSystem;
	MXP_BUFFERMODE_ENUM		BufferMode;
	MXP_TRANSITIONMODE_ENUM TransitionMode;
	FLOAT					TransitionParameter;

} _MXP_MOVEINEARABSOLUTE_IN;



/******************************************************************************
* \struct MXP_MOVELINEARABSOLUTE_EX_IN
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVELINEARABSOLUTE_EX_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axis.
	
	UINT16					CommandBlockNo;										

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.

	FLOAT					PositionX;											
	FLOAT					PositionY;
	FLOAT					PositionZ;
	FLOAT					PositionU;

	FLOAT					PositionV;											
	FLOAT					PositionW;
	FLOAT					PositionA;
	FLOAT					PositionB;
	FLOAT					PositionC;

	FLOAT					Velocity;											// <float> Value of the maximum velocity (always positive) (not necessarily reached) [u/s].
	FLOAT					Acceleration;										// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (always positive) (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]. (always positive)

	MXP_COORDSYSTEM_ENUM	CoordSystem;
	MXP_BUFFERMODE_ENUM		BufferMode;
	MXP_TRANSITIONMODE_ENUM TransitionMode;
	FLOAT					TransitionParameter;

} _MXP_MOVELINEARABSOLUTE_EX_IN;

/******************************************************************************
* \struct MXP_MOVE_LINEARABSOLUTE_OUT
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVELINEARABSOLUTE_OUT
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axis.

	UINT8					Done;											
	UINT8					Busy;											
	UINT8					Active;											
	UINT8					CommandAborted;											
	UINT8					Error;											
	UINT16					ErrorID;											

} _MXP_MOVELINEARABSOLUTE_OUT;

/******************************************************************************
* \struct MXP_MOVE_CIRCULARABSOLUTE_IN
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVECIRCULARABSOLUTE_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axis.

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.
	
	MXP_CIRCLEMODE_ENUM		CircMode;
	
	FLOAT					AuxPoint1;											
	FLOAT					AuxPoint2;											
	FLOAT					EndPoint1;											
	FLOAT					EndPoint2;
	
	MXP_PATHCHOICE_ENUM		PathChoice;
	INT32					Plane1;												// Vendor specific.
	INT32					Plane2;												// Vendor specific.
	FLOAT					Velocity;											// <float> Value of the maximum velocity (always positive) (not necessarily reached) [u/s].
	FLOAT					Acceleration;										// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (always positive) (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]. (always positive)

	MXP_COORDSYSTEM_ENUM	CoordSystem;
	MXP_BUFFERMODE_ENUM		BufferMode;
	MXP_TRANSITIONMODE_ENUM TransitionMode;
	FLOAT					TransitionParameter;

} _MXP_MOVECIRCULAR_ABSOLUTE_IN;


/******************************************************************************
* \struct MXP_MOVE_CIRCULARABSOLUTE_OUT
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVECIRCULARABSOLUTE_OUT
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axis.

	UINT8					Done;											
	UINT8					Busy;											
	UINT8					Active;											
	UINT8					CommandAborted;											
	UINT8					Error;											
	UINT16					ErrorID;											


} _MXP_MOVECIRCULARABSOLUTE_OUT;

/******************************************************************************
* \struct MXP_MOVE_LINEARRELATIVE_IN
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVELINEARRELATIVE_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axis.

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.

	FLOAT					DistanceX;											
	FLOAT					DistanceY;											
	FLOAT					DistanceZ;
	FLOAT					DistanceU;

	FLOAT					DistanceV;											
	FLOAT					DistanceW;
	FLOAT					DistanceA;
	FLOAT					DistanceB;
	FLOAT					DistanceC;

	FLOAT					Velocity;											// <float> Value of the maximum velocity (always positive) (not necessarily reached) [u/s].
	FLOAT					Acceleration;										// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (always positive) (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]. (always positive)

	MXP_COORDSYSTEM_ENUM	CoordSystem;
	MXP_BUFFERMODE_ENUM		BufferMode;
	MXP_TRANSITIONMODE_ENUM TransitionMode;
	FLOAT					TransitionParameter;

} _MXP_MOVELINEARRELATIVE_IN;



/******************************************************************************
* \struct MXP_MOVE_LINEARRELATIVE_EX_IN
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVELINEARRELATIVE_EX_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axis.

	UINT16					CommandBlockNo;	
	UINT8					Execute;											// <UINT8> Start the motion at rising edge.

	FLOAT					DistanceX;											
	FLOAT					DistanceY;											
	FLOAT					DistanceZ;
	FLOAT					DistanceU;

	FLOAT					DistanceV;											
	FLOAT					DistanceW;
	FLOAT					DistanceA;
	FLOAT					DistanceB;
	FLOAT					DistanceC;

	FLOAT					Velocity;											// <float> Value of the maximum velocity (always positive) (not necessarily reached) [u/s].
	FLOAT					Acceleration;										// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (always positive) (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]. (always positive)

	MXP_COORDSYSTEM_ENUM	CoordSystem;
	MXP_BUFFERMODE_ENUM		BufferMode;
	MXP_TRANSITIONMODE_ENUM TransitionMode;
	FLOAT					TransitionParameter;

} _MXP_MOVELINEARRELATIVE_EX_IN;


/******************************************************************************
* \struct MXP_MOVE_LINEARRELATIVE_OUT
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVELINEARRELATIVE_OUT
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axis.

	UINT8					Done;											
	UINT8					Busy;											
	UINT8					Active;											
	UINT8					CommandAborted;											
	UINT8					Error;											
	UINT16					ErrorID;											

} _MXP_MOVELINEARRELATIVE_OUT;


/******************************************************************************
* \struct MXP_MOVE_CIRCULARRELATIVE_IN
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVECIRCULARRELATIVE_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axis.

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.

	MXP_CIRCLEMODE_ENUM		CircMode;
	FLOAT					AuxPoint1;											
	FLOAT					AuxPoint2;											
	FLOAT					EndPoint1;											
	FLOAT					EndPoint2;
	MXP_PATHCHOICE_ENUM		PathChoice;
	INT32					Plane1;												// Vendor specific.
	INT32					Plane2;												// Vendor specific.
	FLOAT					Velocity;											// <float> Value of the maximum velocity (always positive) (not necessarily reached) [u/s].
	FLOAT					Acceleration;										// <float> Value of the acceleration (always positive) (increasing energy of the motor) [u/s²]
	FLOAT					Deceleration;										// <float> Value of the deceleration (always positive) (decreasing energy of the motor) [u/s²]
	FLOAT					Jerk;												// <float> Value of the Jerk [u/s³]. (always positive)

	MXP_COORDSYSTEM_ENUM	CoordSystem;
	MXP_BUFFERMODE_ENUM		BufferMode;
	MXP_TRANSITIONMODE_ENUM TransitionMode;
	FLOAT					TransitionParameter;

} _MXP_MOVECIRCULARRELATIVE_IN;

/******************************************************************************
* \struct MXP_MOVE_CIRCULARRELATIVE_OUT
* \brief Move Absolute Command input data structure.
*******************************************************************************/
typedef struct MXP_MOVECIRCULARRELATIVE_OUT
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axis.

	UINT8					Done;											
	UINT8					Busy;											
	UINT8					Active;											
	UINT8					CommandAborted;											
	UINT8					Error;											
	UINT16					ErrorID;											


} _MXP_MOVECIRCULARRELATIVE_OUT;


/******************************************************************************
* Vendor Specific supported.
* \struct MXP_ReadVendorSpecificL7N2608Cmd_In
*******************************************************************************/
typedef struct MXP_READVENDORSPECIFICL7N2608_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	
	UINT8					Enable;												// <UINT8> Get the value of the parameter continuously while enabled.

} _MXP_READVENDORSPECIFICL7N2608_IN;

/******************************************************************************
* Vendor Specific supported.
* \struct MXP_ReadVendorSpecificL7N2608Cmd_Out
*******************************************************************************/
typedef struct MXP_READVENDORSPECIFICL7N2608_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					Data;												// <float> The value of the accumulated overload.

} _MXP_READVENDORSPECIFICL7N2608_OUT;


/******************************************************************************
* \struct MXP_CAMTABLESELECT_IN
* \brief CamTableSelect Command input data structure.
*******************************************************************************/
typedef struct MXP_CAMTABLESELECT_IN
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;
	MXP_CAM_REF				CamTable;											// Reference to the Cam Table

	UINT8					Execute;											
	UINT8					Periodic;
	UINT8					MasterAbsolute;
	UINT8					SlaveAbsolute;
	MXP_EXECUTIONMODE_ENUM	ExecutionMode;


} _MXP_CAMTABLESELECT_IN;


/******************************************************************************
* \struct MXP_CAMTABLESELECT_OUT
* \brief CamTableSelect Command output data structure.
*******************************************************************************/
typedef struct MXP_CAMTABLESELECT_OUT
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;
	MXP_CAM_REF				CamTable;											// Reference to the Cam Table

	UINT8					Done;											
	UINT8					Busy;									
	UINT8					Error;											
	UINT16					ErrorID;											
	UINT16					CamTableID;											// ??					


} _MXP_CAMTABLESELECT_OUT;

/******************************************************************************
* \struct MXP_WRITECAMTABLE_IN
* \brief WriteCamTable Command input data structure.
*******************************************************************************/
typedef struct MXP_WRITECAMTABLE_IN
{
	MXP_CAM_REF				CamTable;											// Reference to the Cam Table

	UINT8					Execute;											
	UINT16					DataSize;
	UINT16					ExecutionMode;
	MXP_CAMTABLE_REF		CamDataArray[400];

} _MXP_WRITECAMTABLE_IN;


/******************************************************************************
* \struct MXP_WRITECAMTABLE_OUT
* \brief WriteCamTable Command output data structure.
*******************************************************************************/
typedef struct MXP_WRITECAMTABLE_OUT
{
	MXP_CAM_REF				CamTable;											// Reference to the Cam Table

	UINT8					Done;											
	UINT8					Busy;									
	UINT8					Error;											
	UINT16					ErrorID;											

} _MXP_WRITECAMTABLE_OUT;


/******************************************************************************
* \struct MXP_READCAMTABLE_IN
* \brief ReadCamTable Command input data structure.
*******************************************************************************/
typedef struct MXP_READCAMTABLE_IN
{
	MXP_CAM_REF				CamTable;											// Reference to the Cam Table

	UINT8					Execute;											


} _MXP_READCAMTABLE_IN;


/******************************************************************************
* \struct MXP_READCAMTABLE_OUT
* \brief ReadCamTable Command output data structure.
*******************************************************************************/
typedef struct MXP_READCAMTABLE_OUT
{
	MXP_CAM_REF				CamTable;											// Reference to the Cam Table

	UINT8					Done;											
	UINT8					Busy;									
	UINT8					Error;											
	UINT16					ErrorID;			

	UINT16					DataSize;
	MXP_CAMTABLE_REF		CamDataArray[400];

} _MXP_READCAMTABLE_OUT;

/******************************************************************************
* \struct MXP_RDCAMTABLESLAVEPOS_IN
* \brief ReadCamTableSlavePos Command input data structure.
*******************************************************************************/
typedef struct MXP_RDCAMTABLESLAVEPOS_IN
{
	MXP_CAM_REF				CamTable;											// Reference to the Cam Table

	UINT8					Execute;	
	FLOAT					MasterPosition;


} _MXP_RDCAMTABLESLAVEPOS_IN;


/******************************************************************************
* \struct MXP_RDCAMTABLESLAVEPOS_OUT
* \brief ReadCamTableSlavePos Command output data structure.
*******************************************************************************/
typedef struct MXP_RDCAMTABLESLAVEPOS_OUT
{
	MXP_CAM_REF				CamTable;											// Reference to the Cam Table

	UINT8					Done;											
	UINT8					Busy;									
	UINT8					Error;											
	UINT16					ErrorID;			

	FLOAT					SlavePosition;
	
} MXP_RDCAMTABLESLAVEPOS_OUT;

/******************************************************************************
* \struct MXP_CAMSCALING_IN
* \brief CamScaling Command input data structure.
*******************************************************************************/
typedef struct MXP_CAMSCALING_IN
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					Execute;											
	UINT16					ActivationMode;
	UINT16					MasterScalingMode;
	UINT16					SlaveScalingMode;
	FLOAT					ActivationPosition;
	FLOAT					MasterOffset;
	FLOAT					SlaveOffset;
	FLOAT					MasterScaling;
	FLOAT					SlaveScaling;

} _MXP_CAMSCALING_IN;


/******************************************************************************
* \struct MXP_CAMSCALING_OUT
* \brief CamScaling Command output data structure.
*******************************************************************************/
typedef struct MXP_CAMSCALING_OUT
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					Done;												
	UINT8					Busy;												
	UINT8					Active;												
	UINT8					CommandAborted;										
	UINT8					Error;												
	UINT16					ErrorID;											

} _MXP_CAMSCALING_OUT;

/******************************************************************************
* \struct MXP_PHASING_IN
* \brief Phasing Command input data structure.
*******************************************************************************/
typedef struct MXP_PHASING_IN
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					Execute;

	FLOAT					PhaseShift;
	FLOAT					Velocity;
	FLOAT					Acceleration;
	FLOAT					Deceleration;
	FLOAT					Jerk;

	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_PHASING_IN;

/******************************************************************************
* \struct MXP_PHASING_OUT
* \brief GearIn Command output data structure.
*******************************************************************************/
typedef struct MXP_PHASING_OUT
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					Done;
	UINT8					Busy;
	UINT8					Active;
	UINT8					CommandAborted;
	UINT8					Error;
	UINT16					ErrorID;

} _MXP_PHASING_OUT;

/******************************************************************************
* \struct MXP_CAMIN_IN
* \brief CamIn Command input data structure.
*******************************************************************************/
typedef struct MXP_CAMIN_IN
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					Execute;											
	UINT8					ContinuousUpdate;
	FLOAT					MasterOffset;
	FLOAT					SlaveOffset;
	FLOAT					MasterScaling;
	FLOAT					SlaveScaling;
	FLOAT					MasterStartDistance;
	FLOAT					MasterSyncPosition;

	MXP_STARTMODE_ENUM		StartMode;
	MXP_SOURCE_ENUM			MasterValueSource;
	UINT16					CamTableID;
	MXP_BUFFERMODE_ENUM		BufferMode;


} _MXP_CAMIN_IN;




/******************************************************************************
* \struct MXP_CAMIN_OUT
* \brief CamIn Command output data structure.
*******************************************************************************/
typedef struct MXP_CAMIN_OUT
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					InSync;
	UINT8					Busy;											
	UINT8					Active;
	UINT8					CommandAborted;
	UINT8					Error;											
	UINT16					ErrorID;											
	UINT8					EndOfProfile;											

} _MXP_CAMIN_OUT;


/******************************************************************************
* \struct MXP_CAMIN_EX_IN
* \brief CamIn Command input data structure.
*******************************************************************************/

typedef struct EXTERNAL_ENC_INFO_IN
{

	UINT32					SlaveNo;											
	UINT32					SlavePos;
	BYTE					SlaveSize;
	UINT32					ExternalEncResolution;
	FLOAT					PulseToCAMMasterUnit;

} _EXTERNAL_ENC_INFO_IN;


typedef struct MXP_CAMIN_EX_IN
{
	INT32					MasterType;  //0 Axis ,1 External ENC
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;
	EXTERNAL_ENC_INFO_IN	ExternalENCInfo; // External ENC 인 경우 정보 
	UINT8					Execute;											
	UINT8					ContinuousUpdate;
	FLOAT					MasterOffset;
	FLOAT					SlaveOffset;
	FLOAT					MasterScaling;
	FLOAT					SlaveScaling;
	FLOAT					MasterStartDistance;
	FLOAT					MasterSyncPosition;

	MXP_STARTMODE_ENUM		StartMode;
	MXP_SOURCE_ENUM			MasterValueSource;
	UINT16					CamTableID;
	MXP_BUFFERMODE_ENUM		BufferMode;


} MXP_CAMIN_EX_IN;




/******************************************************************************
* \struct MXP_CAMIN_EX_OUT
* \brief CamIn Command output data structure.
*******************************************************************************/
typedef struct MXP_CAMIN_EX_OUT
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;
	EXTERNAL_ENC_INFO_IN	ExternalENCInfo; // External ENC 인 경우 정보 

	UINT8					InSync;
	UINT8					Busy;											
	UINT8					Active;
	UINT8					CommandAborted;
	UINT8					Error;											
	UINT16					ErrorID;											
	UINT8					EndOfProfile;											

} _MXP_CAMIN_EX_OUT;



/******************************************************************************
* \struct MXP_CAMOUT_IN
* \brief CamOut Command input data structure.
*******************************************************************************/
typedef struct MXP_CAMOUT_IN
{
	MXP_AXIS_REF			Slave;												// Reference to the Slave Axis

	UINT8					Execute;											

} _MXP_CAMOUT_IN;


/******************************************************************************
* \struct MXP_CAMOUT_OUT
* \brief CamOut Command output data structure.
*******************************************************************************/
typedef struct MXP_CAMOUT_OUT
{
	MXP_AXIS_REF			Slave;												// Reference to the Slave Axis

	UINT8					Done;											
	UINT8					Busy;											
	UINT8					Error;											
	UINT16					ErrorID;											

} _MXP_CAMOUT_OUT;


/******************************************************************************
* \struct MXP_GEARIN_IN
* \brief GearIn Command Input data structure.
*******************************************************************************/
typedef struct MXP_GEARIN_IN
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					Execute;
	UINT8					ContinuousUpdate;
	
	INT32					RatioNumerator;
	UINT32					RatioDenominator;
	MXP_SOURCE_ENUM			MasterValueSource;

	FLOAT					Acceleration;
	FLOAT					Deceleration;
	FLOAT					Jerk;

	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_GEARIN_IN;


/******************************************************************************
* \struct MXP_GEARIN_OUT
* \brief GearIn Command output data structure.
*******************************************************************************/
typedef struct MXP_GEARIN_OUT
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					InGear;
	UINT8					Busy;
	UINT8					Active;
	UINT8					CommandAborted;
	UINT8					Error;
	UINT16					ErrorID;

} _MXP_GEARIN_OUT;


/******************************************************************************
* \struct MXP_GEAROUT_IN
* \brief GearOut Command Input data structure.
*******************************************************************************/
typedef struct MXP_GEAROUT_IN
{
	MXP_AXIS_REF			Slave;

	UINT8					Execute;

} _MXP_GEAROUT_IN;


/******************************************************************************
* \struct MXP_GEAROUT_OUT
* \brief GearOut Command output data structure.
*******************************************************************************/
typedef struct MXP_GEAROUT_OUT
{
	MXP_AXIS_REF			Slave;

	UINT8					Done;
	UINT8					Busy; 
	UINT8					Error;
	UINT16					ErrorID; 

} _MXP_GEAROUT_OUT;


/******************************************************************************
* \struct MXP_GEARINPOS_IN
* \brief GearInPos Command Input data structure.
*******************************************************************************/
typedef struct MXP_GEARINPOS_IN
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					Execute;

	INT32					RatioNumerator;
	UINT32					RatioDenominator;
	MXP_SOURCE_ENUM			MasterValueSource;
	FLOAT					MasterSyncPosition;
	FLOAT					SlaveSyncPosition;
	MXP_SYNCMODE_ENUM		SyncMode;
	FLOAT					MasterStartDistance;
	FLOAT					Velocity;
	FLOAT					Acceleration;
	FLOAT					Deceleration;
	FLOAT					Jerk;
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_GEARINPOS_IN;


/******************************************************************************
* \struct MXP_GEARINPOS_OUT
* \brief GearInPos Command output data structure.
*******************************************************************************/
typedef struct MXP_GEARINPOS_OUT
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					StartSync;
	UINT8					InSync;
	UINT8					Busy; 
	UINT8					Active;
	UINT8					CommandAborted;
	UINT8					Error;
	UINT16					ErrorID; 
	
} _MXP_GEARINPOS_OUT;


/******************************************************************************
* \struct MXP_GEARINPOSEX_IN
* \brief GearInPosEx Command Input data structure.
*******************************************************************************/
typedef struct MXP_GEARINPOSEX_IN
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					Execute;

	INT32					RatioNumerator;
	INT32					RatioDenominator;
	MXP_SOURCE_ENUM			MasterValueSource;
	FLOAT					MasterSyncPosition;
	FLOAT					SlaveSyncPosition;
	FLOAT					SlaveSyncPosDistance;
	MXP_SYNCMODE_ENUM		SyncMode;
	FLOAT					MasterStartDistance;
	FLOAT					Velocity;
	FLOAT					Acceleration;
	FLOAT					Deceleration;
	FLOAT					Jerk;
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_GEARINPOSEX_IN;


/******************************************************************************
* \struct MXP_GEARINPOSEX_OUT
* \brief GearInPosEx Command output data structure.
*******************************************************************************/
typedef struct MXP_GEARINPOSEX_OUT
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					StartSync;
	UINT8					InSync;
	UINT8					Busy; 
	UINT8					Active;
	UINT8					CommandAborted;
	UINT8					Error;
	UINT16					ErrorID; 

} _MXP_GEARINPOSEX_OUT;


/******************************************************************************
* \struct MXP_ShiftSyncPos_IN
* \brief ShiftSyncPos Command Input data structure.
*******************************************************************************/
	typedef struct MXP_SHIFTSYNCPOS_IN
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					Execute;

	FLOAT					ShiftDistance;

} _MXP_SHIFTSYNCPOS_IN;


/******************************************************************************
* \struct MXP_ShiftSyncPos_OUT
* \brief ShiftSyncPos Command output data structure.
*******************************************************************************/
typedef struct MXP_SHIFTSYNCPOS_OUT
{
	MXP_AXIS_REF			Master;
	MXP_AXIS_REF			Slave;

	UINT8					Done;
	UINT8					Busy; 
	UINT8					Active;
	UINT8					CommandAborted;
	UINT8					Error;
	UINT16					ErrorID; 

} _MXP_SHIFTSYNCPOS_OUT;

/******************************************************************************
* \struct MXP_READCOMMANDPOSITION_IN
* \brief Read Command Position Command Input data structure.
*******************************************************************************/

typedef struct MXP_READCOMMANDPOSITION_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the selected input signal continuously while enabled.

} _MXP_READCOMMANDPOSITION_IN;


/******************************************************************************
* \struct MXP_READCOMMANDPOSITION_OUT
* \brief Read Command Position Command output data structure.
*******************************************************************************/
typedef struct MXP_READCOMMANDPOSITION_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					CommandPosition;									// <float> New absolute position. (in axis' unit [u])

} _MXP_READCOMMANDPOSITION_OUT;


/******************************************************************************
* \struct MXP_READCOMMANDVELOCITY_IN
* \brief Read Command Position Command Input data structure.
*******************************************************************************/

typedef struct MXP_READCOMMANDVELOCITY_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the selected input signal continuously while enabled.

} _MXP_READCOMMANDVELOCITY_IN;


/******************************************************************************
* \struct MXP_READCOMMANDVELOCITY_OUT
* \brief Read Command Velocity Command output data structure.
*******************************************************************************/
typedef struct MXP_READCOMMANDVELOCITY_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					CommandVelocity;									// <float> New absolute Velocity. (in axis' unit [u])

} _MXP_READCOMMANDVELOCITY_OUT;


/******************************************************************************
* \struct MXP_GROUPREADACTUALVELOCITY_IN
* \brief Read Group Actual Velocity Command input data structure.
*******************************************************************************/
typedef struct MXP_GROUPREADACTUALVELOCITY_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axes.

	UINT8					Enable;												// <UINT8> Start the motion at rising edge.
	MXP_COORDSYSTEM_ENUM	CoordSystem;	

} _MXP_GROUPREADACTUALVELOCITY_IN;

/******************************************************************************
* \struct MXP_GROUPREADACTUALVELOCITY_OUT
* \brief Read Group Actual Velocity Command Output data structure.
*******************************************************************************/
typedef struct MXP_GROUPREADACTUALVELOCITY_OUT
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axes.

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					Velocity;
	FLOAT					PathVelocity;										// <float> Group Actual Velocity Value.

} _MXP_GROUPREADACTUALVELOCITY_OUT;


/******************************************************************************
* \struct MXP_GROUPREADCOMMANDVELOCITY_IN
* \brief Read Group Command Velocity Command input data structure.
*******************************************************************************/
typedef struct MXP_GROUPREADCOMMANDVELOCITY_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axes.

	UINT8					Enable;												// <UINT8> Start the motion at rising edge.

} _MXP_GROUPREADCOMMANDVELOCITY_IN;

/******************************************************************************
* \struct MXP_GROUPREADCOMMANDVELOCITY_OUT
* \brief Read Group Command Velocity Command Output data structure.
*******************************************************************************/
typedef struct MXP_GROUPREADCOMMANDVELOCITY_OUT
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axes.

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					CommandVelocity;									
	FLOAT					PathCommandVelocity;								// <float> Group Command Velocity Value.

} _MXP_GROUPREADCOMMANDVELOCITY_OUT;


/******************************************************************************
* \struct MXP_GROUPREADACTUALPOSITION_IN
* \brief Read Group Actual Position Command input data structure.
*******************************************************************************/
typedef struct MXP_GROUPREADACTUALPOSITION_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axes.

	UINT8					Enable;												// <UINT8> Start the motion at rising edge.

	MXP_COORDSYSTEM_ENUM	CoordSystem;

} _MXP_GROUPREADACTUALPOSITION_IN;

/******************************************************************************
* \struct MXP_GROUPREADACTUALPOSITION_OUT
* \brief Read Group Actual Position Command Output data structure.
*******************************************************************************/
typedef struct MXP_GROUPREADACTUALPOSITION_OUT
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axes.

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	
	FLOAT					PositionX;											// <float> Group Actual Velocity Value.
	FLOAT					PositionY;											// <float> Group Actual Velocity Value.
	FLOAT					PositionZ;											// <float> Group Actual Velocity Value.
	FLOAT					PositionU;											// <float> Group Actual Velocity Value.
	FLOAT					PositionV;											// <float> Group Actual Velocity Value.
	FLOAT					PositionW;											// <float> Group Actual Velocity Value.
	FLOAT					PositionA;											// <float> Group Actual Velocity Value.
	FLOAT					PositionB;											// <float> Group Actual Velocity Value.
	FLOAT					PositionC;											// <float> Group Actual Velocity Value.


} _MXP_GROUPREADACTUALPOSITION_OUT;

/******************************************************************************
* \struct MXP_GROUPREADCOMMANDPOSITION_IN
* \brief Read Group Actual Position Command input data structure.
*******************************************************************************/
typedef struct MXP_GROUPREADCOMMANDPOSITION_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axes.

	UINT8					Enable;												// <UINT8> Start the motion at rising edge.

	MXP_COORDSYSTEM_ENUM	CoordSystem;

} _MXP_GROUPREADCOMMANDPOSITION_IN;


/******************************************************************************
* \struct MXP_GROUPREADCOMMANDPOSITION_OUT
* \brief Read Group Actual Position Command Output data structure.
*******************************************************************************/
typedef struct MXP_GROUPREADCOMMANDPOSITION_OUT
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axes.

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

	FLOAT					PositionX;											// <float> Group Actual Velocity Value.
	FLOAT					PositionY;											// <float> Group Actual Velocity Value.
	FLOAT					PositionZ;											// <float> Group Actual Velocity Value.
	FLOAT					PositionU;											// <float> Group Actual Velocity Value.
	FLOAT					PositionV;											// <float> Group Actual Velocity Value.
	FLOAT					PositionW;											// <float> Group Actual Velocity Value.
	FLOAT					PositionA;											// <float> Group Actual Velocity Value.
	FLOAT					PositionB;											// <float> Group Actual Velocity Value.
	FLOAT					PositionC;											// <float> Group Actual Velocity Value.


} _MXP_GROUPREADCOMMANDPOSITION_OUT;


/******************************************************************************
* \struct MXP_GROUPSETPOSITION_IN
* \brief Group SetPosition Command input data structure.
*******************************************************************************/
typedef struct MXP_GROUPSETPOSITION_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axes.

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.

	FLOAT					PositionX;											
	FLOAT					PositionY;
	FLOAT					PositionZ;
	FLOAT					PositionU;

	FLOAT					PositionV;											
	FLOAT					PositionW;
	FLOAT					PositionA;
	FLOAT					PositionB;
	FLOAT					PositionC;

	UINT8					Relative;

	MXP_COORDSYSTEM_ENUM	CoordSystem;
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_GROUPSETPOSITION_IN;

/******************************************************************************
* \struct MXP_GROUPSETPOSITION_OUT
* \brief Group SetPosition Command Output data structure.
*******************************************************************************/
typedef struct MXP_GROUPSETPOSITION_OUT
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axes.

	UINT8					Done;												// <UINT8> 'Position' has new value.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axes group
	UINT8					CommandAborted;										// <UINT8> Command is aborted by another command		
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification

} _MXP_GROUPSETPOSITION_OUT;


/******************************************************************************
* \struct MXP_GROUPHOME_IN
* \brief Group Home Command input data structure.
*******************************************************************************/
typedef struct MXP_GROUPHOME_IN
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axes.

	UINT8					Execute;											// <UINT8> Start the motion at rising edge.

	FLOAT					PositionX;											
	FLOAT					PositionY;
	FLOAT					PositionZ;
	FLOAT					PositionU;

	FLOAT					PositionV;											
	FLOAT					PositionW;
	FLOAT					PositionA;
	FLOAT					PositionB;
	FLOAT					PositionC;

	MXP_COORDSYSTEM_ENUM	CoordSystem;
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_GROUPHOME_IN;

/******************************************************************************
* \struct MXP_GROUPHOME_OUT
* \brief Group Home Command Output data structure.
*******************************************************************************/
typedef struct MXP_GROUPHOME_OUT
{
	MXP_AXESGROUP_REF		AxesGroup;											// Reference to a group of axes.

	UINT8					Done;												// <UINT8> 'Position' has new value.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axes group
	UINT8					CommandAborted;										// <UINT8> Command is aborted by another command		
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification

} _MXP_GROUPHOME_OUT;


/******************************************************************************
* \struct MXP_SETOVERRIDE_IN
* \brief Set Override Command input data structure.
*******************************************************************************/
typedef struct MXP_SETTOUCHPROBEFUNC_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> If SET, if writes the value of the override factor continuously. If RESET it should keep the last value.
	UINT16					FuncData;											// <float> New touch probe func value.

}_MXP_SETTOUCHPROBE_FUNC_IN;

/******************************************************************************
* \struct MXP_SETTOUCHPROBE_FUNC_OUT
* \brief Set Override Command output data structure.
*******************************************************************************/
typedef struct MXP_SETTOUCHPROBEFUNC_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enabled;												// <UINT8> Signals that the override factor(s) is (are0 set successfully.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

}_MXP_SETTOUCHPROBE_FUNC_OUT;

/******************************************************************************
* \struct MXP_READACTUALTOUCHPROBE_STATUS_IN
* \brief Read Actual Touch Probe Status Command input data structure.
*******************************************************************************/
typedef struct MXP_READACTUALTOUCHPROBESTATUS_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the selected input signal continuously while enabled.

}_MXP_READACTUALTOUCHPROBE_STATUS_IN;

/******************************************************************************
* \struct MXP_READACTUALPOSITION_OUT
* \brief Read Actual Touch Probe Status Command output data structure.
*******************************************************************************/
typedef struct MXP_READACTUALTOUCHPROBESTATUS_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	UINT16					Status;												// <UINT16> New touch probe status.

}_MXP_READACTUALTOUCHPROBE_STATUS_OUT;

/******************************************************************************
* \struct MXP_READACTUALTOUCHPROBE_POSITION_IN
* \brief Read Actual Touch Probe Status Command input data structure.
*******************************************************************************/
typedef struct MXP_READACTUALTOUCHPROBEPOSITION_IN
{
	MXP_AXIS_REF				Axis;											// Reference to the axis

	UINT8					Enable;												// <UINT8> Get the value of the selected input signal continuously while enabled.
	MXP_TOUCHPROBE_CHNL_ENUM	Channel;										// <float> Channel number.
	MXP_TOUCHPROBE_EDGE_ENUM	Edge;											// <float> New touch probe func value.

}_MXP_READACTUALTOUCHPROBE_POSITION_IN;

/******************************************************************************
* \struct MXP_READACTUALPOSITION_OUT
* \brief Read Actual Touch Probe Status Command output data structure.
*******************************************************************************/
typedef struct MXP_READACTUALTOUCHPROBEPOSITION_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					Position;											// <float> New touch probe position.

}_MXP_READACTUALTOUCHPROBE_POSITION_OUT;



/******************************************************************************
* \struct MXP_READACTUALPOSITION_OUT
* \brief Read Actual Touch Probe Status Command output data structure.
*******************************************************************************/
typedef struct MXP_READACTUALTOUCHPROBEPOSITION_EX_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	FLOAT					EdgePositivePosition;											// <float> New touch probe position.
	FLOAT					EdgeNegativePosition;											// <float> New touch probe position.

}MXP_READACTUALTOUCHPROBEPOSITION_EX_OUT;


/******************************************************************************
* \struct MXP_READACTUALTOUCHPROBE_POSITION_EX_IN
* \brief ReadActualTouchProbePos1PosEdge Command Input data structure.
*******************************************************************************/
typedef struct MXP_READACTUALTOUCHPROBE_STATUS_EX_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

}_MXP_READACTUALTOUCHPROBE_STATUS_EX_IN; 

/******************************************************************************
* \struct MXP_READACTUALTOUCHPROBE_POSITION_EX_OUT
* \brief ReadActualPotion Command Output data structure.
*******************************************************************************/
typedef struct MXP_READACTUALTOUCHPROBE_STATUS_EX_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	UINT16					Status;												// <UINT16> New touch probe status.

}_MXP_READACTUALTOUCHPROBE_STATUS_EX_OUT;

/******************************************************************************
* \struct MXP_READACTUALTOUCHPROBE_POSITION_EX_IN
* \brief ReadActualTouchProbePos1PosEdge Command Input data structure.
*******************************************************************************/
typedef struct MXP_READACTUALTOUCHPROBE_POSITION_EX_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

}_MXP_READACTUALTOUCHPROBE_POSITION_EX_IN; 

/******************************************************************************
* \struct MXP_READACTUALTOUCHPROBE_POSITION_EX_OUT
* \brief ReadActualPotion Command Output data structure.
*******************************************************************************/
typedef struct MXP_READACTUALTOUCHPROBE_POSITION_EX_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	FLOAT					Position;											// <float>  New touch probe Position.

}_MXP_READACTUALTOUCHPROBE_POSITION_EX_OUT;

//******************************************************************************
//* \struct MXP_SETCONTROLMODE_IN
//* \서보의 컨트롤 모드를 변경하게 하는 함수 입력 구조체
//*******************************************************************************/
typedef struct MXP_WRITECONTROLMODE_IN
{
	MXP_AXIS_REF				Axis;											// Reference to the axis

	UINT8						Execute;										
	MXP_CONTROLMODE_ENUM		ControlMode;										
	

}_MXP_WRITECONTROLMODE_IN;

//******************************************************************************
//* \struct MXP_SETCONTROLMODE_OUT
//* \서보의 컨트롤 모드를 변경하게 하는 함수 출력 구조체
//*******************************************************************************/
typedef struct MXP_WRITECONTROLMODE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

}_MXP_WRITECONTROLMODE_OUT;

//******************************************************************************
//* \struct MXP_READCONTROLMODE_IN
//* \서보의 컨트롤 모드를 읽는 함수 입력 구조체
//*******************************************************************************/
typedef struct MXP_READCONTROLMODE_IN
{
	MXP_AXIS_REF				Axis;											// Reference to the axis

	UINT8						Enable;												

}_MXP_READCONTROLMODE_IN;

//******************************************************************************
//* \struct MXP_READCONTROLMODE_OUT
//* \서보의 컨트롤 모드를 읽는 함수 입력 구조체
//*******************************************************************************/
typedef struct MXP_READCONTROLMODE_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
	UINT32					IsControlMode;
}_MXP_READCONTROLMODE_OUT;

//******************************************************************************
//* \struct MXP_SETCONTROLMODE_IN
//* \서보의 컨트롤 모드를 읽는 함수 입력 구조체
//*******************************************************************************/
typedef struct MXP_READPDODATA_IN
{
	MXP_INPUT_REF				Input;											// Reference to the axis

	UINT8						Enable;
	UINT8						Direction;
	UINT16						Offset;
	UINT16						Size;
}_MXP_READPDODATA_IN;

//******************************************************************************
//* \struct MXP_SETCONTROLMODE_OUT
//* \서보의 컨트롤 모드를 읽는 함수 입력 구조체
//*******************************************************************************/
typedef struct MXP_READPDODATA_OUT
{
	MXP_INPUT_REF				Input;

	UINT8					Valid;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.
}_MXP_READPDODATA_OUT;

/******************************************************************************
* \struct MXP_WRITEPDODATA_IN
*******************************************************************************/
typedef struct MXP_WRITEPDODATA_IN
{
	MXP_OUTPUT_REF			Output;											// Reference to the output.
	
	UINT8					Execute;										// <BOOL> Get the value of the selected input signal continuously while enabled.	
	UINT8					Direction;
	UINT16					Offset;
	UINT16					Size;
}_MXP_WRITEPDODATA_IN;

/******************************************************************************
* \struct MXP_WRITEPDODATA_OUT
*******************************************************************************/
typedef struct MXP_WRITEPDODATA_OUT
{
	MXP_OUTPUT_REF			Output;											// Reference to the output.

	UINT8					Done;												// <BOOL> Writing of the ouput signal value is done.
	UINT8					Busy;												// <BOOL> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <BOOL> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <WORD> Error identification.

} _MXP_WRITEPDODATA_OUT;

/******************************************************************************
* \struct MXP_BUFFEREDDIGITALIO_IN
* \brief Buffered Digital IO Command input data structure.
*******************************************************************************/
typedef struct MXP_BUFFEREDDIGITALIO_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Get the value of the selected input signal continuously while enabled.
	UINT16					SlaveNo;											// Slave No.
	UINT16					BitPosition;										
	UINT8					BitValue;											
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_BUFFEREDDIGITALIO_IN;



/******************************************************************************
* \struct MXP_BUFFEREDDIGITALIO_EX_IN
* \brief Buffered Digital IO Command input data structure.
* \CommandBlockNum 
*******************************************************************************/
typedef struct MXP_BUFFEREDDIGITALIO_EX_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	UINT16					CommandBlockNo;									// SequenceMove Cmd Count					
	UINT8					Execute;											// <UINT8> Get the value of the selected input signal continuously while enabled.
	UINT16					SlaveNo;											// Slave No.
	UINT16					BitPosition;										
	UINT8					BitValue;											
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_BUFFEREDDIGITALIO_EX_IN;

/******************************************************************************
* \struct MXP_BUFFEREDDIGITALIO_OUT
* \brief Buffered Digital IO Command Output data structure.
*******************************************************************************/
typedef struct MXP_BUFFEREDDIGITALIO_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> Writing of the ouput signal value is done.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_BUFFEREDDIGITALIO_OUT;



/******************************************************************************
* \struct MXP_GROUPBUFFEREDDIGITALIO_IN
* \brief GROUP Buffered Digital IO Command input data structure.
*******************************************************************************/
typedef struct MXP_GROUPBUFFEREDDIGITALIO_IN
{
	MXP_AXESGROUP_REF			AxesGroup;												// Reference to the Group

	UINT8					Execute;											// <UINT8> Get the value of the selected input signal continuously while enabled.
	UINT16					SlaveNo;											// Slave No.
	UINT16					BitPosition;										
	UINT8					BitValue;											
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_GROUPBUFFEREDDIGITALIO_IN;


/******************************************************************************
* \struct MXP_GROUPBUFFEREDDIGITALIO_EX_IN
* \brief GROUP Buffered Digital IO Command input data structure.
*******************************************************************************/
typedef struct MXP_GROUPBUFFEREDDIGITALIO_EX_IN
{
	MXP_AXESGROUP_REF		AxesGroup;												// Reference to the Group
	UINT16					CommandBlockNo;									// SequenceMove Cmd Count				
	UINT8					Execute;											// <UINT8> Get the value of the selected input signal continuously while enabled.
	UINT16					SlaveNo;											// Slave No.
	UINT16					BitPosition;										
	UINT8					BitValue;											
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_GROUPBUFFEREDDIGITALIO_EX_IN;

/******************************************************************************
* \struct MXP_GROUPBUFFEREDDIGITALIO_OUT
* \brief GROUPBuffered Digital IO Command Output data structure.
*******************************************************************************/
typedef struct MXP_GROUPBUFFEREDDIGITALIO_OUT
{
	MXP_AXESGROUP_REF			AxesGroup;												// Reference to the Group

	UINT8					Done;												// <UINT8> Writing of the ouput signal value is done.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_GROUPBUFFEREDDIGITALIO_OUT;


/******************************************************************************
* \struct MXP_DWELL_IN
* \brief Dwell Command input data structure.
*******************************************************************************/
typedef struct MXP_DWELL_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Execute;											// <UINT8> Get the value of the selected input signal continuously while enabled.
	FLOAT					TimeValue;											
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_DWELL_IN;


/******************************************************************************
* \struct MXP_DWELL_EX_IN
* \brief Dwell Command input data structure.
*******************************************************************************/
typedef struct MXP_DWELL_EX_IN
{
	MXP_AXIS_REF			Axis;												// Reference to the axis
	UINT16					CommandBlockNo;									// SequenceMove Cmd Count	
	UINT8					Execute;											// <UINT8> Get the value of the selected input signal continuously while enabled.
	FLOAT					TimeValue;											
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_DWELL_EX_IN;
/******************************************************************************
* \struct MXP_DWELL_OUT
* \brief Dwell Command Output data structure.
*******************************************************************************/
typedef struct MXP_DWELL_OUT
{
	MXP_AXIS_REF			Axis;												// Reference to the axis

	UINT8					Done;												// <UINT8> Writing of the ouput signal value is done.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_DWELL_OUT;


/******************************************************************************
* \struct MXP_GROUPDWELL_IN
* \brief Group Dwell Command input data structure.
*******************************************************************************/
typedef struct MXP_GROUPDWELL_IN
{
	MXP_AXESGROUP_REF			AxesGroup;												// Reference to the Group

	UINT8					Execute;											// <UINT8> Get the value of the selected input signal continuously while enabled.
	FLOAT					TimeValue;											
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_GROUPDWELL_IN;

/******************************************************************************
* \struct MXP_GROUPDWELL_EX_IN
* \brief Group Dwell Command input data structure.
*******************************************************************************/
typedef struct MXP_GROUPDWELL_EX_IN
{
	MXP_AXESGROUP_REF		AxesGroup;										// Reference to the Group
	UINT16					CommandBlockNo;								// SequenceMove Cmd Count
	UINT8					Execute;										// <UINT8> Get the value of the selected input signal continuously while enabled.
	FLOAT					TimeValue;											
	MXP_BUFFERMODE_ENUM		BufferMode;

} _MXP_GROUPDWELL_EX_IN;

/******************************************************************************
* \struct MXP_DWELL_OUT
* \brief Dwell Command Output data structure.
*******************************************************************************/
typedef struct MXP_GROUPDWELL_OUT
{
	MXP_AXESGROUP_REF			AxesGroup;												// Reference to the axis

	UINT8					Done;												// <UINT8> Writing of the ouput signal value is done.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_GROUPDWELL_OUT;


typedef struct MXP_RDCAMTABLESALVEPOS_OUT
{

	UINT8					Done;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;
	FLOAT					SlavePos;

} _MXP_RDCAMTABLESALVEPOS_OUT;
/******************************************************************************
* \struct MXP_ET_READPARAMETER_IN
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_READREGISTER_IN
{
	UINT32					SlaveNo;											// Slave No.
	UINT32					Index;												// Index in parameter
	UINT32					Cmd;												// Register Command
	UINT32					Adp;												// Address Position
	UINT32					Ado;												// Address Offset
	UINT32					BufLen;												// Read buffer size	(*)

} _MXP_ET_READREGISTER_IN;

/******************************************************************************
* \struct MXP_ET_READPARAMETER_OUT
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_READREGISTER_OUT
{
	UINT32					SlaveNo;											// Slave No.

	UINT8					Valid;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;
	UINT32					WorkCount;

	UINT8					Data[100];

} _MXP_ET_READREGISTER_OUT;

/******************************************************************************
* \struct MXP_ET_WRITEPARAMETER_IN
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_WRITEREGISTER_IN
{
	UINT32					SlaveNo;										// Slave No.
	UINT32					Index;											// Index in parameter
	UINT32					Cmd;											// Register Command
	UINT32					Adp;											// Address Position
	UINT32					Ado;											// Address Offset
	UINT32					BufLen;											// Read buffer size	(*)

	UINT8					Data[100];

} _MXP_ET_WRITEREGISTER_IN;

/******************************************************************************
* \struct MXP_ET_WRITEPARAMETER_OUT
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_WRITEREGISTER_OUT
{
	UINT32					SlaveNo;											// Slave No.

	UINT8					Done;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;

} _MXP_ET_WRITEREGISTER_OUT;

/******************************************************************************
* \struct MXP_ET_READFOEFILE_IN
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_READFOEFILE_IN
{
	UINT32					SlaveNo;											// Slave No.
	UINT32					Cmd;												// Register Command
	UINT32					BufLen;												// Read buffer size	(*)

	UINT8					Data[256];

} _MXP_ET_READFOEFILE_IN;

/******************************************************************************
* \struct MXP_ET_READFOEFILE_OUT
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_READFOEFILE_OUT
{
	UINT32					SlaveNo;											// Slave No.

	UINT8					Valid;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;
	UINT32					WorkCount;

} _MXP_ET_READFOEFILE_OUT;


/******************************************************************************
* \struct MXP_ET_WRITEFOEFILE_OUT
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_WRITEFOEFILE_OUT
{
	UINT8					Done;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;
	UINT32					CurFilePos;
	UINT32					TotalFileSize;

} _MXP_ET_WRITEFOEFILE_OUT;
//*********************************************************************************

/******************************************************************************
* \struct MXP_ET_INITFOEFILE_IN
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_INITFOEFILE_IN
{
	UINT32					BufLen;												// Read buffer size	(*)
	INT8					Path[256];
	UINT32					Password;

} _MXP_ET_INITFOEFILE_IN;

/******************************************************************************
* \struct MXP_ET_INITFOEFILE_OUT
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_INITFOEFILE_OUT
{
	UINT8					Done;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;

	UINT32					TotalFileSize;

} _MXP_ET_INITFOEFILE_OUT;
//*********************************************************************************

/******************************************************************************
* \struct MXP_ET_REGISTERFOESLAVE_IN
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_REGISTERFOESLAVE_IN
{
	UINT32					SlaveNo;										// Slave No.

} _MXP_ET_REGISTERFOESLAVE_IN;

/******************************************************************************
* \struct MXP_ET_REGISTERFOESLAVE_OUT
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_REGISTERFOESLAVE_OUT
{
	UINT8					Done;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;
	UINT32					nSlaves;

} _MXP_ET_REGISTERFOESLAVE_OUT;
//*********************************************************************************
//*********************************************************************************

#define IN
#define OUT

typedef enum 
{
	MXP_UNKNOWN_PARAM 				= 0,
	MXP_AXIS_MODE_PARAM				= 1,
	MXP_AXIS_OP_MOTION_MODE_PARAM	= 2,
	MXP_AXIS_STATE_PARAM 			= 3,
	MXP_AXIS_GROUP_ID_PARAM			= 4,
	MXP_DRIVE_ID_PARAM				= 5,
	MXP_MOVEMENT_TYPE_PARAM			= 6,
	MXP_POSITION_PERIOD_PARAM		= 7,
	MXP_CONTROLLER_MODE				= 8,
	MXP_float_CONTROLLER_MODE_PARAM	= 9,
	MXP_SET_POSITION_PARAM			= 10,
	MXP_ACT_POSITION_PARAM			= 11,
	MXP_AIM_POSITION_PARAM			= 12,
	MXP_SET_VELOCITY_PARAM			= 13,
	MXP_ACT_VELOCITY_PARAM			= 14,
	MXP_MAX_VELOCITY_PARAM			= 15,
	MXP_SW_MAX_VELOCITY_PARAM		= 16,
	MXP_CONSTANT_VELOCITY_PARAM		= 17,
	MXP_SET_ACCELERATION_PARAM		= 18,
	MXP_MAX_ACCELERATION_PARAM		= 19,
	MXP_SW_MAX_ACCELERATION_PARAM	= 20,
	MXP_ACCELERATING_PARAM			= 21,
	MXP_SET_DECELERATION_PARAM		= 22,
	MXP_MAX_DECELERATION_PARAM		= 23,
	MXP_SW_MAX_DECELERATION_PARAM	= 24,
	MXP_DECELERATING_PARAM			= 25,
	MXP_MAX_JERK_PARAM				= 26,
	MXP_SW_MAX_JERK_PARAM			= 27,
	MXP_SPATIAL_OPTION_PARAM		= 28,
	MXP_CYCLE_TIME_PARAM			= 29,
	MXP_RES_ID_PARAM				= 30,
	MXP_SW_LIMIT_POSITIVE_PARAM		= 31,
	MXP_SW_LIMIT_NEGATIVE_PARAM		= 32,
	MXP_I_COMM_EV_USR_1_PARAM       = 33,	/// PDO TX From Drive
	MXP_I_COMM_EV_USR_1_AUX_PARAM	= 34,	/// PDO TX From Drive
	MXP_F_COMM_EV_USR_1_PARAM       = 35,	/// PDO TX From Drive
	MXP_F_COMM_EV_USR_1_AUX_PARAM	= 36,	/// PDO TX From Drive
	MXP_I_COMM_EV_USR_2_PARAM       = 37,	/// PDO TX From Drive
	MXP_I_COMM_EV_USR_2_AUX_PARAM	= 38,	/// PDO TX From Drive
	MXP_F_COMM_EV_USR_2_PARAM       = 39,	/// PDO TX From Drive
	MXP_F_COMM_EV_USR_2_AUX_PARAM	= 40,	/// PDO TX From Drive
	MXP_I_COMM_EV_USR_3_PARAM       = 41,	/// PDO TX TO Drive
	MXP_I_COMM_EV_USR_3_AUX_PARAM	= 42,	/// PDO TX TO Drive
	MXP_F_COMM_EV_USR_3_PARAM       = 43,	/// PDO TX TO Drive
	MXP_F_COMM_EV_USR_3_AUX_PARAM	= 44,	/// PDO TX TO Drive
	MXP_I_COMM_EV_USR_4_PARAM       = 45,	/// PDO TX TO Drive
	MXP_I_COMM_EV_USR_4_AUX_PARAM	= 46,	/// PDO TX TO Drive
	MXP_F_COMM_EV_USR_4_PARAM       = 47,	/// PDO TX TO Drive
	MXP_F_COMM_EV_USR_4_AUX_PARAM	= 48,	/// PDO TX TO Drive
	MXP_MAX_PARAMETERS_NUM

} MXP_PARAMETER_LIST_ENUM;



/******************************************************************************
* \struct MXP_PROFILE_MOVE_IN
* \brief ProfileMove Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_PROFILE_MOVE_IN
{
	UINT32					AxisNo;												// Slave No.
	UINT8					Execute;											// <UINT8> Not support
	UINT32					Tablesize;											// <UINT8> profile Motion Data Table Count
	UINT32					IOTablesize;										// <UINT8> profile IO Data Table Count

	UINT8					RepeatCount;										// <UINT8> profile Move Repeat Count
	FLOAT					StartDwell;											// <float> Repeat Mode Start Wait Time (Not support).
	FLOAT					EndDwell;											// <float> Repeat Mode End Wait Time (Not support).
	UINT8					ReverseMode;										// <UINT8> Profile Reverse Move Mode 


} MXP_PROFILE_MOVE_IN;

/******************************************************************************
* \struct MXP_PROFILE_MOVE_IN
* \brief ProfileMove Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_PROFILE_MOVE_OUT
{
	UINT8					Done;												// <UINT8> Writing of the ouput signal value is done.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;												// <UINT8> Indicates that the FB has control on the axis.
	UINT8					CommandAborted;										// <UINT8> 'Command' is aborted by another command.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;

} MXP_PROFILE_MOVE_OUT;

/******************************************************************************
* \struct MXP_PROFILE_TABLE_IN
* \brief ProfileMove Read Move profile & IO Table Data structure
*******************************************************************************/
typedef struct MXP_PROFILE_TABLE_IN
{
	UINT8					TableindexNo;										// <UINT8> TABLE INDEX .
	UINT8					nMotionMode;										// <UINT8> ABS/REL (not support)
	FLOAT					Position;											// <float> Table Move Dist
	FLOAT					Velocity;											// <float> Table Step Position.
	FLOAT					Acc;												// <float> Table Step Velocity.
	FLOAT					Dec;												// <float> Table Step Acc.
	FLOAT					Jerk;												// <float> <float> Table Step Dec.

	MXP_DIRECTION_ENUM		Direction;											// Not Support
	MXP_BUFFERMODE_ENUM		Buffermode;											// <INT32>  


} MXP_PROFILE_TABLE_IN;

/******************************************************************************
* \struct MXP_ConvertTableCmd
* \brief Convert Profile Table 
*******************************************************************************/
typedef struct MXP_File_TABLE_IN
{
	UINT8					TableindexNo;										// <UINT8> TABLE INDEX .
	FLOAT					Position;										    // <float> Table Position User insert
	FLOAT					Velocity;											// <float> Table Velocity User insert
	FLOAT					Time;												// <float> Table Step Time Return
	FLOAT					DiffPosition;										// <float> Table Step Move Distance Return
	FLOAT					DiffVelocity;										// <float> Table Step Move Velocity  Return
	FLOAT					Acc		;											// <float> Table Step Move Acc Return
} MXP_File_TABLE_IN;


typedef struct MXP_IO_TABLE_IN
{
	UINT8					TableindexNo;										// <UINT8> TABLE INDEX .
	UINT16					SlaveNo;											// <UINT8> Slave No
	UINT16					BitPos;												// <UINT8> Bit Position
	UINT8					BitValue;											// <UINT8> On/Off Value



} MXP_IO_TABLE_IN;



/******************************************************************************
* \struct MXP_PROFILE_TABLE_ARRAY_IN
* \brief ProfileMove Read Move profile & IO Table Data  Array structure
*******************************************************************************/
typedef struct MXP_PROFILE_TABLE_ARRAY_IN
{
	UINT32						DataSize;
	MXP_PROFILE_TABLE_IN		ProfileTable[50];

} MXP_PROFILE_TABLE_ARRAY_IN;


typedef struct MXP_FILE_TABLE_ARRAY_IN
{
	UINT32						DataSize;
	MXP_File_TABLE_IN			FileTable[50];

} MXP_FILE_TABLE_ARRAY_IN;


typedef struct MXP_IO_TABLE_ARRAY_IN
{
	UINT32					DataSize;
	MXP_IO_TABLE_IN				IOTable[50];

} MXP_IO_TABLE_ARRAY_IN;

/******************************************************************************
* \struct MXP_ET_READOBJECTLIST_IN
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_READOBJECTLIST_IN
{
	UINT32					SlaveNo;											// Slave No.
	UINT32					ListType;											// ListType

	UINT32					BufLen;												// Read buffer size	(*)

} _MXP_ET_READOBJECTLIST_IN;

// List type 
// 0x00 : Object number requested on 5 different lists
// 0x01 : Transfers entire object in Object Dictionary in Response
// 0x02 : Transfers the object's mappable RxPDO only
// 0x03 : Transfers the object's mappable TxPDO only
// 0x04 : Transfers only the objects necessary for backup in Response
// 0x05 : Transfers the object to be used for Startup Parameters only in Response
#define LIST_NUMS						0x00
#define LIST_ALL_OBJECTS				0x01
#define LIST_MAPPABLE_RXPDO_OBJECTS		0x02
#define LIST_MAPPABLE_TXPDO_OBJECTS		0x03
#define LIST_BACKUP_OBJECTS				0x04
#define LIST_SETTIING_OBJECTS			0x05

/******************************************************************************
* \struct MXP_ET_READPARAMETER_OUT
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_READOBJECTLIST_OUT
{
	UINT32					SlaveNo;											// Slave No.

	UINT8					Valid;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;

	UINT8					Data[ETHERCAT_SDO_SIZE];

} _MXP_ET_READOBJECTLIST_OUT;

/******************************************************************************
* \struct MXP_ET_READOBJECTDICTIONARY_IN
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_READOBJECTDICTIONARY_IN
{
	UINT32					SlaveNo;											// Slave No.
	UINT32					Index;												// Index

	UINT32					BufLen;												// Read buffer size	(*)

} _MXP_ET_READOBJECTDICTIONARY_IN;

/******************************************************************************
* \struct MXP_ET_READOBJECTDICTIONARY_OUT
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_READOBJECTDICTIONARY_OUT
{
	UINT32					SlaveNo;											// Slave No.

	UINT8					Valid;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;

	UINT8					Data[ETHERCAT_SDO_SIZE];

} _MXP_ET_READOBJECTDICTIONARY_OUT;


/******************************************************************************
* \struct MXP_ET_READOBJECTENTRY_IN
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_READOBJECTENTRY_IN
{
	UINT32					SlaveNo;											// Slave No.
	UINT32					Index;												// Index in parameter
	UINT32					SubIndex;											// Sub index in parameter
	UINT32					ValueInfo;											// Sub index in parameter
	UINT32					BufLen;												// Read buffer size	(*)

} _MXP_ET_READOBJECTENTRY_IN;

/******************************************************************************
* \struct MXP_ET_READOBJECTENTRY_OUT
* \brief EtherCAT Read Parameter Command input data structure.
*******************************************************************************/
typedef struct MXP_ET_READOBJECTENTRY_OUT
{
	UINT32					SlaveNo;											// Slave No.

	UINT8					Valid;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;

	UINT8					Data[ETHERCAT_SDO_SIZE];

} _MXP_ET_READOBJECTENTRY_OUT;



/******************************************************************************
* \struct MXP_GroupSetOverride_IN
* \brief Group Set Override Command input data structure.
*******************************************************************************/
typedef struct MXP_GroupSetOverride_IN
{
	MXP_AXESGROUP_REF		AxesGroupxis;												// Reference to the axis

	UINT8					Enable;												// <UINT8> If SET, if writes the value of the override factor continuously. If RESET it should keep the last value.
	FLOAT					VelFactor;											// <float> New override factor for the velocity.
	FLOAT					AccFactor;											// <float> New override factor for the acceleration / deceleration.
	FLOAT					JerkFactor;											// <float> New override factor for the jerk.

} MXP_GroupSetOverride_IN;

/******************************************************************************
* \struct MXP_GroupSetOverride_Out
* \brief Group Set Override Command output data structure.
*******************************************************************************/
typedef struct MXP_GroupSetOverride_Out
{
	MXP_AXESGROUP_REF		AxesGroupxis;												// Reference to the axis

	UINT8					Enabled;											// <UINT8> Signals that the override factor(s) is (are0 set successfully.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Error;												// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} MXP_GroupSetOverride_Out;

/******************************************************************************
* \struct MXP_ACCDECTOACCTIME_IN
* \brief AccDec to AccTime Command input data structure.
*******************************************************************************/
typedef struct MXP_ACCDECTOACCTIME_IN
{
	MXP_AXIS_REF			Axis;												

	UINT8					Enable;												
	FLOAT					TargetVel;
	FLOAT					AccDec;
	FLOAT					Jerk;

} _MXP_ACCDECTOACCTIME_IN;


/******************************************************************************
* \struct MXP_ACCDECTOACCTIME_OUT
* \brief AccDec to AccTime Command output data structure.
*******************************************************************************/
typedef struct MXP_ACCDECTOACCTIME_OUT
{
	MXP_AXIS_REF			Axis;												

	UINT8					Valid;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;											
	FLOAT					AccDecBuildUp;												
	FLOAT					LimitAccDec;												
	FLOAT					AccDecRampDown;												
	FLOAT					RuildUppc;												
	FLOAT					Limitpc;												
	FLOAT					RampDownpc;

} _MXP_ACCDECTOACCTIME_OUT;

/******************************************************************************
* \struct MXP_ACCTIMETOACCDEC_IN
* \brief AccTime to AccDec Command input data structure.
*******************************************************************************/
typedef struct MXP_ACCTIMETOACCDEC_IN
{
	MXP_AXIS_REF			Axis;												

	UINT8					Enable;
	FLOAT					TargetVel;
	FLOAT					AccDecBuildUp;												
	FLOAT					LimitAccDec;												
	FLOAT					AccDecRampDown;												

} _MXP_ACCTIMETOACCDEC_IN;


/******************************************************************************
* \struct MXP_ACCTIMETOACCDEC_OUT
* \brief AccTime to AccDec Command output data structure.
*******************************************************************************/
typedef struct MXP_ACCTIMETOACCDEC_OUT
{
	MXP_AXIS_REF			Axis;												

	UINT8					Valid;
	UINT8					Busy;
	UINT8					Error;
	UINT16					ErrorID;											
	FLOAT					AccDec;
	FLOAT					Jerk;

} _MXP_ACCTIMETOACCDEC_OUT;

/******************************************************************************
* \struct MXP_MULTIAXISCOUPLESET_IN
* \brief Multi cross couple set input data structure.
*******************************************************************************/
#define PLC_MULTICCC_AXIS_SIZE	10
typedef struct MXP_MULTIAXISCOUPLESET_IN
{
	UINT8					Execute;		//								
	UINT32					ArrayAxisNo[PLC_MULTICCC_AXIS_SIZE]; // Mapping 된 Axis No 정보 
	UINT16					AxisCount;		// Multi Axis Count		
	UINT16					Mode;			// CCCRunMode	
} _MXP_MULTIAXISCOUPLESET_IN;

/******************************************************************************
* \struct MXP_MULTIAXISCOUPLESET_OUT
* \brief Multi cross couple set output data structure.
*******************************************************************************/
typedef struct MXP_MULTIAXISCOUPLESET_OUT
{
	UINT32					ArrayAxisNo[PLC_MULTICCC_AXIS_SIZE]; // Mapping 된 Axis No 정보 

	UINT8					Done;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;											
	UINT8					CommandAborted;		
	UINT8					Error;											// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_MULTIAXISCOUPLESET_OUT;


/******************************************************************************
* \struct MXP_MULTIAXISCOUPLERESET_IN
* \brief Multi cross couple reset input data structure.
*******************************************************************************/
typedef struct MXP_MULTIAXISCOUPLERESET_IN
{
	UINT8					Execute;		//								
	UINT32					ArrayAxisNo[PLC_MULTICCC_AXIS_SIZE]; // Mapping 된 Axis No 정보 
	UINT16					AxisCount;		// Multi Axis Count							
} _MXP_MULTIAXISCOUPLERESET_IN;


/******************************************************************************
* \struct MXP_MULTIAXISCOUPLERESET_OUT
* \brief Multi cross couple reset output data structure.
*******************************************************************************/
typedef struct MXP_MULTIAXISCOUPLERESET_OUT
{
	UINT32					ArrayAxisNo[PLC_MULTICCC_AXIS_SIZE]; // Mapping 된 Axis No 정보 

	UINT8					Done;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;											
	UINT8					CommandAborted;		
	UINT8					Error;											// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_MULTIAXISCOUPLERESET_OUT;





/******************************************************************************
* \struct MXP_MULTIAXISCOUPLEGAINSET_IN
* \brief Multi cross couple set input data structure.
*******************************************************************************/

typedef struct MXP_MULTIAXISCOUPLE_SINGLEAXISGAIN
{
	UINT16					FeedForward_V_Gain;	
	UINT16					FeedForward_A_Gain;	
	UINT16					P_Gain;	
	UINT16					I_Gain;	
	UINT16					D_Gain;	

} _MXP_MULTIAXISCOUPLE_SINGLEAXISGAIN;

typedef struct MXP_MULTIAXISCOUPLE_CCCGAIN
{
	UINT16					CCC_Wp;	
	UINT16					CCC_Wi;	

} _MXP_MULTIAXISCOUPLE_CCCGAIN;


typedef struct MXP_MULTIAXISCOUPLEGAINSET_IN
{
	UINT8					Execute;		//								
	UINT32					ArrayAxisNo[PLC_MULTICCC_AXIS_SIZE]; // Mapping 된 Axis No 정보 
	UINT16					AxisCount;		// Multi Axis Count		
	MXP_MULTIAXISCOUPLE_SINGLEAXISGAIN			ArraySingleAxisGain[PLC_MULTICCC_AXIS_SIZE];	
	MXP_MULTIAXISCOUPLE_CCCGAIN					ArrayCCCGain[PLC_MULTICCC_AXIS_SIZE];	
	UINT16					MultiControlKffGain;

} _MXP_MULTIAXISCOUPLEGAINSET_IN;

/******************************************************************************
* \struct MXP_MULTIAXISCOUPLERESET_OUT
* \brief Multi cross couple set output data structure.
*******************************************************************************/
typedef struct MXP_MULTIAXISCOUPLEGAINSET_OUT
{
	UINT32					ArrayAxisNo[PLC_MULTICCC_AXIS_SIZE]; // Mapping 된 Axis No 정보 

	UINT8					Done;												// <UINT8> A valid output is available at the FB.
	UINT8					Busy;												// <UINT8> The FB is not finished and new output values are to be expected.
	UINT8					Active;											
	UINT8					CommandAborted;		
	UINT8					Error;											// <UINT8> Signals that an error has occurred within the Function Block.
	UINT16					ErrorID;											// <UINT16> Error identification.

} _MXP_MULTIAXISCOUPLEGAINSET_OUT;