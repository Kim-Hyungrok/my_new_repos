#pragma once

#include "MXP_EasyClass.h"		// Include new version of MXP API

#include <chrono>
#include <thread>
#include <vector>

typedef MXP_EasyClass::MXP_FUNCTION_STATUS	FUNC_STATUS;
typedef MXP_EasyClass::KERNEL_STATUS		KERNEL_STATUS;
typedef MXP_EasyClass::MXP_ONLINE_STATE		MXP_ONLINE_STATE;
typedef MXP_EasyClass::PORT_STATE			PORT_STATE;
typedef MXP_EasyClass::MXP_AxisStateBit		AXIS_STATE;
typedef MXP_EasyClass::AXIS_ERROR			AXIS_ERROR;
typedef MXP_EasyClass::MXP_PDO_DIRECTION	MXP_PDO_DIRECTION;

using std::vector;

const FLOAT byte_size = 256;

typedef FLOAT millimeter_t;								// Unit definition
typedef FLOAT kilogram_t;
typedef double second_t;

typedef enum
{
	STOP_DEC = 1000,
	STOP_JERK = 5000,
	SCALE_GEN_ACC = 5,
	SCALE_GEN_DEC = 5,
	SCALE_GEN_JERK = 25

}CUSTOM_CONST;						// CUSTOM_CONSTANT enum (or => constant ? + #define ?) 
typedef enum
{
	ON = true,
	OFF = false

}ONOFF;								// On Off enum
typedef enum {
	RED,
	GREEN,
	BLUE
}LED_COLOR;
typedef enum 
{
	MASTER_LINE_CONNECTION_ERROR	= -1,
	MASTER_KERNEL_ERROR				= -2,
	MASTER_NETWORK_ERROR			= -3,
	SLAVE_COUNT_ERROR				= -4,
	AXIS_COUNT_ERROR				= -5,
	SLAVE_LINE_CONNECTION_ERROR		= -6,
	SLAVE_COMMUNICATION_ERROR		= -7,
	LIMIT_SENSOR_ERROR				= -8,
	EMERGENCY_BUTTON_IS_ON			= -9,
	HOMING_ERROR					=-10,
	STANBY_ERROR					=-11
	
}SELF_DIAGNOSIS_ERROR_LIST;
typedef enum
{
	A = 0,					// main x
	B,						// main y
												// C : main z rotation, PH54(Rs485)
												// D : main y rotation, PH54(Rs485)
	E,						// probe z				
	F,						// needle z
												// G : needle rotation, PH42(Rs485)
	H,						// needle z'
	I,						// upper arm
	J,						// wrist
	K,						// alcohol eject
	L						// hemostasis
	
}MOTOR_SLAVE_NO;					// Match motor name with slave_no
enum class IO_SLAVE_NO : UINT32 
{
	// EL 1808												

	DI_EMER_BUTTON = 10,
	DI_RUN_BUTTON = 10,
	DI_CHECK_WINDOW_OPEN = 10,										

	// EL 1889 for limit sensor (11)
												// Slave no. for DI(EL1808, EL1889) function
	// EL 2008

	
	DO_ALCOHOL_EJECTION_MOTOR = 12,
	DO_NEEDLE_MOTOR = 12,
	DO_HEMOSTASIS_MOTOR = 12,
	DO_SPARE = 12,
	DO_IR_SOURCE = 12,
	DO_PROBE = 12,
	DO_BRAKE = 12,
	DO_NEEDLE_LOAD_VALVE = 12,

	// EL 2624

	DO_LED_RED = 13,
	DO_LED_GREEN = 13,
	DO_LED_BLUE = 13,
	DO_IR_LASER = 13,							// Slave no. for DO(EL2008, EL2624) function
													
	// EL 3162

	AI_IR_LASER_DISTANCE = 14,


	// EL 3356

	AI_UPPER_ARM_LOAD = 16,
	AI_WRIST_LOAD = 17,
	AI_ARM_REST_LOAD = 18,
	AI_PROBE_LOAD = 19,
	AI_ALCOHOL_WEIGHT = 20						// Slave no. for AI(El3162, EL3356) function
};												
enum class OFFSET : UINT16
{
	// EL 1808

	DI_EMER_BUTTON = 0,
	DI_RUN_BUTTON = 0,
	DI_CHECK_WINDOW_OPEN = 0,

	// EL 1889 for limit sensor (0)
												// Offset. for DI(EL1808, EL1889) function
	// EL 2008

	DO_ALCOHOL_EJECTION_MOTOR = 0,
	DO_NEEDLE_MOTOR = 0,
	DO_HEMOSTASIS_MOTOR = 0,
	DO_SPARE = 0,
	DO_IR_SOURCE = 0,
	DO_PROBE = 0,
	DO_BRAKE = 0,
	DO_NEEDLE_LOAD_VALVE = 0,

	// EL 2624

	DO_LED_RED = 0,
	DO_LED_GREEN = 0,
	DO_LED_BLUE = 0,
	DO_IR_LASER = 0,							// Offset. for DO(EL2008, EL2624) function

	// EL 3162

	AI_IR_LASER_DISTANCE = 1,
	AI_IR_LASER_DISTANCE_NEXT,

	// EL 3356

	AI_UPPER_ARM_LOAD = 2,
	AI_UPPER_ARM_LOAD_NEXT = 3,
	AI_WRIST_LOAD = 2,
	AI_WRIST_LOAD_NEXT = 3,
	AI_ARM_REST_LOAD = 2,
	AI_ARM_REST_LOAD_NEXT = 3,
	AI_PROBE_LOAD = 2,
	AI_PROBE_LOAD_NEXT = 3,
	AI_ALCOHOL_WEIGHT = 2,						// Offset. for AI(El3162, EL3356) function
	AI_ALCOHOL_WEIGHT_NEXT = 3
};												
enum class BIT_OFFSET : UINT8
{
	// EL 1808

	DI_EMER_BUTTON = 0,
	DI_RUN_BUTTON,
	DI_CHECK_WINDOW_OPEN,

	// EL 1889 for limit sensor
												// Bit offset. for DI(EL1808, EL1889) function
	// EL 2008

	DO_ALCOHOL_EJECTION_MOTOR = 0,
	DO_NEEDLE_MOTOR,
	DO_HEMOSTASIS_MOTOR,
	DO_SPARE,
	DO_IR_SOURCE,
	DO_PROBE,
	DO_BRAKE,
	DO_NEEDLE_LOAD_VALVE,

	// EL 2624

	DO_LED_RED = 0,
	DO_LED_GREEN,
	DO_LED_BLUE,
	DO_IR_LASER,								// Bit offset. for DO(EL2008, EL2624) function

	// EL 3162

	AI_IR_LASER_DISTANCE = 0,


	// EL 3356

	AI_UPPER_ARM_LOAD = 0,
	AI_WRIST_LOAD,
	AI_ARM_REST_LOAD,
	AI_PROBE_LOAD,
	AI_ALCOHOL_WEIGHT,							// Bit offset. for AI(El3162, EL3356) function
};												
																			

class ECATManager {
public:

	ECATManager();
	~ECATManager();

	// Basic ECAT //
	// Basic Command //

	bool InitSys();			// Initialize MXP
	bool DestroySys();		// Terminate MXP
	bool RunMXP();			// Run MXP
	bool StopMXP();			// Stop MXP

	// PDO, SDO // need to be updated

	UINT8* ReadPDOData(UINT32 slave_no, MXP_PDO_DIRECTION direction, UINT16 offset, UINT16 size);					// Read PDO data
	bool WritePDOData(UINT32 slave_no, UINT16 offset, UINT16 size, UINT32 value);					// Write PDO data
	UINT32 ReadSDOData(UINT32 slave_no, UINT16 index, UINT16 sub_index, UINT16 size);				// Read SDO data
	bool WriteSDOData(UINT32 slave_no, UINT16 index, UINT16 sub_index, UINT16 size, UINT32 value);	// Write SDO data

	// Read Master(MXP) Status //

	bool ReadMasterLineStatus();			// Read&write master's line connection status
	bool ReadMasterKernelStatus();			// Read&write master's kernel status
	bool ReadMasterNetworkStatus();			// Read&write master's network status 

	// Read Slave Status //

	bool ReadSlaveLineStatus();				// Read&write slave's line connection status
	bool ReadSlaveCommStatus();				// Read&write slave's status (both of KERNEL_STATUS and MXP_ONLINE_STATE)
	bool CountSlave();						// Read&write num of slaves
	bool CountAxis();						// Read&write num of axes
	

	// Return Slave Status (same as 'Read Slave Status' but return private member variable) //
	FUNC_STATUS			GetECATFuncStatus();		// Return EtherCAT function status (ex. RET_NO_ERROR)
	bool				GetMasterLineStatus();		// Return master's line connection status
	KERNEL_STATUS		GetMasterKernelStatus();	// Return master's kernel status (ex. SYSTEM_RUN)
	MXP_ONLINE_STATE	GetMasterNetworkStatus();	// Return master's network status (ex. ONLINE_STATE_OP)
	vector<PORT_STATE>	GetSlaveLineStatus();		// Return slave line status
	vector<bool>		GetSlaveCommStatus();		// Return slave communication status

	UINT32 num_of_slave_;							// Total number of slave
	UINT32 num_of_axis_;							// Total number of axis

	// Combination ECAT //

	bool InitSystem();								// Initialize system
	INT8 SelfDiagnosis();							// Self-diagnosis process

private:

	FUNC_STATUS ECAT_curr_func_status_;				// Current function status if error occurs(ex. RET_NO_ERROR != 0)
	vector<FUNC_STATUS> ECAT_error_func_status_;	// Current error status (if error occurs) 

	bool master_line_status_;						// Master's hardware(line) connection status
	KERNEL_STATUS master_kernel_status_;			// Master's kernel status (ex. SYSTEM_READY = 8)
	MXP_ONLINE_STATE master_network_status_;		// Master's network status (ex. ONLINE_STATE_SAFEOP = 0x08)

	vector<PORT_STATE> slave_line_status_;			// Slave's hardware(line) connection status
	vector<bool> slave_comm_status_;				// Slave's communication status = kernel and network status (if SYSTEM_RUN && ONLINE_STATE_OP => TRUE)

	friend class DeviceManager;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////
class DeviceManager {
public:

	DeviceManager();
	~DeviceManager();
	
	/////////////////////////////
	//		    Motion         //
	/////////////////////////////
	
	// Basic Motion //
	// Read Axis' Data //

	vector<bool> ReadAxisStatus();							// Read&write total axis State-Machine status (ex. Standstill)
	vector<bool> ReadAxisError();							// Read&write total axis error (both of MXP and Drive error)
	

	// Return Axis' Data (same as 'Read Axis' Data' but return value) //

	AXIS_STATE GetAxisStatus(UINT32 axis_no);				// Return axis State-Machine status
	AXIS_ERROR GetAxisError(UINT32 axis_no);				// Return axis error
	FLOAT GetAxisPos(UINT32 axis_no);						// Return actual position
	FLOAT GetAxisVel(UINT32 axis_no);						// Return actual velocity
	FLOAT GetAxisTor(UINT32 axis_no);						// Return actual torque

	// Single Axis Motion //

	bool TurnServoOnOff(UINT32 axis_no, bool enable);									// Servo on:TRUE, Servo off:FALSE
	bool ResetAxis(UINT32 axis_no);														// Reset servo (when axis error)
	bool HomeAxis(UINT32 axis_no);														// Home servo
	bool MoveAxis(UINT32 axis_no, FLOAT target_position, FLOAT target_velocity);		// Move absolute position to target position with target velocity
	bool StopAxis(UINT32 axis_no);														// Stop servo
	
private:
	
	FUNC_STATUS Dev_curr_func_status_;				// Current device function status if error occurs(ex. RET_NO_ERROR != 0)
	
	vector<AXIS_STATE> axis_state_;					// Current axis state
	vector<AXIS_ERROR> axis_error_state_;			// Current axis error state
	
	std::thread* ptr_thread;
	/////////////////////////////
	//		     IO            //
	/////////////////////////////

public:

	// Basic IO (API summary) //

	bool GetIOBitData(IO_SLAVE_NO slave_no, OFFSET offset, BIT_OFFSET bit_offset);						// Return In/Output data of selected IO slave by bit unit
	UINT8 GetIOByteData(IO_SLAVE_NO slave_no, OFFSET offset);											// Return In/Output data of selected IO slave by byte unit
	FUNC_STATUS WriteIOBitData(IO_SLAVE_NO slave_no, OFFSET offset, BIT_OFFSET bit_offset, bool data);	// Write Output data to selected IO slave by bit unit
	FUNC_STATUS WriteIOByteData(IO_SLAVE_NO slave_no, OFFSET offset, UINT8 data);						// Write Output data to selected IO slave by byte unit

	// Combination IO //
	// Basic Digital In //
	// EL1808
	bool IsEmerButtonOn();			// Check if emergency button is on or not (ON = TRUE, OFF = FALSE)
	bool IsMotionButtonOn();		// Check if motion button is on or not
	bool IsWindowClosed();			// Check if window is closed or not (CLOSED = ,  OPEN= )

	// Basic Digital Out //
	// EL2008
	bool TurnAlcoholEjectionMotorOnOff(ONOFF on_off);		// Turn on and off alcohol ejection motor
	bool LoadNeedleMotor(ONOFF on_off);						// Turn on and off needle loading air motor
	bool TurnHemostasisMotorOnOff(ONOFF on_off);			// Turn on and off hemostasis air motor
	bool SpareDO();											// Spare DO
	bool TurnIRSourceOnOff(ONOFF on_off);					// Turn on and off IR source
	bool TurnProbeOnOff(ONOFF on_off);						// Turn on and off probe
	bool TurnOriginBrakeOnOff(ONOFF on_off);				// Turn on and off brake
	bool TurnNeedleValveOnOff(ONOFF on_off);				// Turn on and off needle valve

	// EL2624
	bool TurnLEDRedOnOff(ONOFF on_off);						// Turn on and off RED LED
	bool TurnLEDGreenOnOff(ONOFF on_off);					// Turn on and off GREEN LED
	bool TurnLEDBlueOnOff(ONOFF on_off);					// Turn on and off BLUE LED	
	bool TurnIRLaserOnOff(ONOFF on_off);					// Turn on and off IR laser
		
	// Customized Digital Out //
	bool TurnLEDOnOff(LED_COLOR color, ONOFF on_off);		// Turn on and off selected color LED
	bool BlinkLED(LED_COLOR color, second_t interval, second_t duration);		// Blink selected color LED at regular interval for duration (unit:secs ex. 3.5s)
	bool CreateThreadForBlinkLED(LED_COLOR color, second_t interval, second_t duration);
	bool TerminateThread();

	// Basic Analog In //
	// EL3162
	millimeter_t GetLaserDistance();		// Return laser distance in mm

	// EL3356
	kilogram_t GetUpperArmLoad();			// Return upper arm load in kg
	kilogram_t GetWristLoad();				// Return wrist load in kg
	kilogram_t GetArmRestLoad();			// Return arm rest load in kg	
	kilogram_t GetProbeLoad();				// Return probe load in kg
	kilogram_t GetAlcoholWeight();			// Return alcohol weight in kg

	// Get Error message from I/O module (EL3162, EL3356)
	

	vector< pair<const FLOAT, const FLOAT>> software_limit_posisiton =
	{
		// NOT, POT for each axis //

	};												// Software limit position value
	std::thread th1;
	void operator()(LED_COLOR color, second_t interval, second_t duration);
	
	friend class ECATManager;
};

