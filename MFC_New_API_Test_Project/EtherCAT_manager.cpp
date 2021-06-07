#include "pch.h"								// Precompiled header : should be first header 

#include "EtherCAT_manager.h"								


MXP_EasyClass* MXP = new MXP_EasyClass;			// Declaration of MXP Object
ECATManager* EManager = new ECATManager;		// Declaration of ECAT Manager Object
DeviceManager* DManager = new DeviceManager;	// Declaration of Device Manager Object

ECATManager::ECATManager()		// Constructor : Initialization
{
	num_of_slave_ = 0;							
	num_of_axis_ = 0;

	ECAT_curr_func_status_ = FUNC_STATUS::RET_NO_ERROR;			

	master_line_status_ = false;						
	master_kernel_status_ = KERNEL_STATUS::SYSTEM_IDLE;			
	master_network_status_ = MXP_ONLINE_STATE::ONLINE_STATE_NONE;		

	//vector<PORT_STATE> slave_line_status_;			
	vector<bool> slave_comm_status_(20, false);
}		
ECATManager::~ECATManager() {}		// Destructor

// Basic ECAT //
// Basic Command //

bool ECATManager::InitSys()		// Initialize MXP
{
	FUNC_STATUS result = MXP->SYS_Init();
	
	if (result == RET_NO_ERROR)
		return true;
	else
	{
		ECAT_curr_func_status_ = result;
		return false;
	}
}
bool ECATManager::DestroySys()		// Terminate MXP
{
	FUNC_STATUS result = MXP->SYS_Destroy();

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		ECAT_curr_func_status_ = result;
		return false;
	}
}
bool ECATManager::RunMXP()		// Run MXP
{
	FUNC_STATUS result = MXP->SYS_Run();

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		ECAT_curr_func_status_ = result;
		return false;
	}
}
bool ECATManager::StopMXP()		// Stop MXP
{
	FUNC_STATUS result = MXP->SYS_Stop();

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		ECAT_curr_func_status_ = result;
		return false;
	}
}

// PDO, SDO // need to be updated

UINT8* ECATManager::ReadPDOData(UINT32 slave_no, MXP_PDO_DIRECTION direction, UINT16 offset, UINT16 size)			// Read PDO Data : e_ServoWrite when read Tx Data in Servo drive's view
{																																//                 e _MXP Write when read Rx Data 													
	FUNC_STATUS  result;

	UINT8* read_value = new BYTE[size]();

	result = MXP->ECAT_ReadPdoData(slave_no, direction, offset, size, read_value);
	if (result != RET_NO_ERROR)
		ECAT_curr_func_status_ = result;

	return read_value;
}
bool ECATManager::WritePDOData(UINT32 slave_no, UINT16 offset, UINT16 size, UINT32 value)					// Write PDO Data
{
	FUNC_STATUS  result;

	BYTE* arr_write_value = new  BYTE[size]();

	memcpy(&arr_write_value, &value, sizeof(value));

	result = MXP->ECAT_WritePdoData(slave_no, offset, size, arr_write_value);
	if (result != RET_NO_ERROR)
	{
		ECAT_curr_func_status_ = result;
		return false;
	}

	return true;
}
UINT32 ECATManager::ReadSDOData(UINT32 slave_no, UINT16 index, UINT16 sub_index, UINT16 size)					// Read SDO Data
{
	MXP_EasyClass::MXP_FUNCTION_STATUS  result;
	MXP_EasyClass::READ_ETParameterReply  state;
	state.ReadData = new BYTE[size];
	UINT32 value = 0;
	UINT32 power = 1;
	
	result = MXP->ECAT_CoeReadRequest(slave_no, index, sub_index, size);
	if (result != RET_NO_ERROR)
	{
		ECAT_curr_func_status_ = result;
		return -1;
	}

	result = MXP->ECAT_CoeReadReply(slave_no, state);
	if (result != RET_NO_ERROR)
	{
		ECAT_curr_func_status_ = result;
		return -2;
	}

	for (int i = 0; i < size; i++)
	{
		value += state.ReadData[i] * power;
		power *= byte_size;
	}

	return value;
}
bool ECATManager::WriteSDOData(UINT32 slave_no, UINT16 index, UINT16 sub_index, UINT16 size, UINT32 value)	// Write SDO Data
{
	MXP_EasyClass::MXP_FUNCTION_STATUS  result;
	MXP_EasyClass::PROCESS_CHECK  state;

	result = MXP->ECAT_CoeWriteRequest(slave_no, index, sub_index, size, value);
	if (result != RET_NO_ERROR)
	{
		ECAT_curr_func_status_ = result;
		return false;
	}

	result = MXP->ECAT_CoeWriteReply(slave_no, state);
	if (result != RET_NO_ERROR)
	{
		ECAT_curr_func_status_ = result;
		return false;
	}

	return true;
}

// Read Master(MXP) Status //

bool ECATManager::ReadMasterLineStatus()		// Read master's line connection
{
	FUNC_STATUS  result = MXP->ECAT_GetMasterEtherCATLineStatus(this->master_line_status_);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		ECAT_curr_func_status_ = result;
		return false;
	}
}
bool ECATManager::ReadMasterKernelStatus()		// Read master's kernel status
{
	FUNC_STATUS  result = MXP->SYS_GetStatus(this->master_kernel_status_);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		ECAT_curr_func_status_ = result;
		return false;
	}
}
bool ECATManager::ReadMasterNetworkStatus()		// Read master's network status 
{
	FUNC_STATUS  result = MXP->ECAT_GetMasterOnlineMode(this->master_network_status_);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		ECAT_curr_func_status_ = result;
		return false;
	}
}

// Read Slave Status //

bool ECATManager::ReadSlaveLineStatus()		// Read slave's line connection
{
	FUNC_STATUS  result;
	PORT_STATE port_state;

	for (int i = 0; i < num_of_slave_; i++)
	{
		result = MXP->ECAT_GetSlaveEtherCATLineStatus(i, port_state);
		this->slave_line_status_.push_back(port_state);
	}
	if (result == RET_NO_ERROR)
		return true;
	else
	{
		ECAT_curr_func_status_ = result;
		return false;
	}
}
bool ECATManager::ReadSlaveCommStatus()		// Read slave's status (both of KERNEL_STATUS and MXP_ONLINE_STATE)
{
	FUNC_STATUS  result;
	bool ready;

	for (int i = 0; i < num_of_slave_; i++)
	{
		result = MXP->ECAT_SlaveReadyCheck(i, ready);
		this->slave_comm_status_.push_back(ready);
	}
	if (result == RET_NO_ERROR)
		return true;
	else
	{
		ECAT_curr_func_status_ = result;
		return false;
	}
}
bool ECATManager::CountSlave()								// Count num of slaves
{
	FUNC_STATUS  result = MXP->ECAT_GetSlaveCount(this->num_of_slave_);
	if (result == RET_NO_ERROR)
		return true;
	else
	{
		ECAT_curr_func_status_ = result;
		return false;
	}
}
bool ECATManager::CountAxis()								// Count num of axes
{
	FUNC_STATUS  result = MXP->ECAT_GetAxisCount(this->num_of_axis_);
	if (result == RET_NO_ERROR)
		return true;
	else
	{
		ECAT_curr_func_status_ = result;
		return false;
	}
}

// Return Slave Status (same as 'Read Slave Status' but return private member variable) //

FUNC_STATUS	ECATManager::GetECATFuncStatus()		// Return EtherCAT function status (ex. RET_NO_ERROR)
{
	return ECAT_curr_func_status_;
}
bool ECATManager::GetMasterLineStatus()		// Read and return master's line connection status
{
	ReadMasterLineStatus();
	return master_line_status_;
}
KERNEL_STATUS ECATManager::GetMasterKernelStatus()		// Read and return master's kernel status (ex. SYSTEM_RUN)
{
	ReadMasterKernelStatus();
	return master_kernel_status_;
}
MXP_ONLINE_STATE ECATManager::GetMasterNetworkStatus()		// Read and return master's network status (ex. ONLINE_STATE_OP)
{
	ReadMasterNetworkStatus();
	return master_network_status_;
}
vector<PORT_STATE> ECATManager::GetSlaveLineStatus()	// Read and return slave line status
{
	ReadSlaveLineStatus();
	return slave_line_status_;
}
vector<bool> ECATManager::GetSlaveCommStatus()		// Read and return slave communication status
{
	ReadSlaveCommStatus();
	return slave_comm_status_;
}

// Combination ECAT //

bool ECATManager::InitSystem()								// Initialize system = Init + Run
{
	
	if (InitSys())
	{
		if (GetMasterKernelStatus() >= KERNEL_STATUS::SYSTEM_READY)
		{		
			RunMXP();																// RunMXP takes at least 100ms
			if (GetMasterNetworkStatus() == MXP_ONLINE_STATE::ONLINE_STATE_OP)
			{
				return true;
			}
			else
			{
				ECAT_error_func_status_.push_back(GetECATFuncStatus());
				return false;
			}
		}
		else
		{
			ECAT_error_func_status_.push_back(GetECATFuncStatus());
			return false;
		}
	}
	else
	{
		ECAT_error_func_status_.push_back(GetECATFuncStatus());
		return false;
	}
}

INT8 ECATManager::SelfDiagnosis()							// Self-diagnosis process = ECAT check + slave check + limit sensor check + emergency check + homing + standby
{
	if (!GetMasterLineStatus())
		return MASTER_LINE_CONNECTION_ERROR;
	else if (GetMasterKernelStatus() != KERNEL_STATUS::SYSTEM_READY)
		return MASTER_KERNEL_ERROR;
	else if (GetMasterNetworkStatus() != MXP_ONLINE_STATE::ONLINE_STATE_OP)
		return MASTER_NETWORK_ERROR;
	else if (num_of_slave_ != 21)
		return SLAVE_COUNT_ERROR;
	else if (num_of_axis_ != 9)
		return AXIS_COUNT_ERROR;
	else if (!DManager->IsEmerButtonOn())
		return EMERGENCY_BUTTON_IS_ON;
	else
	{

	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

DeviceManager::DeviceManager() {}
DeviceManager::~DeviceManager() {}

/////////////////////////////
//		    Motion         //
/////////////////////////////

// Read Axis' Data //

vector<bool> DeviceManager::ReadAxisStatus()		// Read&write total axis State-Machine status (ex. Standstill)
{
	FUNC_STATUS  result;
	vector<bool> func_error(EManager->num_of_axis_, false);
	AXIS_STATE each_axis_state;

	for (int i = 0; i < EManager->num_of_axis_; i++)
	{
		result = MXP->AX_ReadStatus(i, each_axis_state);
		axis_state_.push_back(each_axis_state);		// at vs []?

		if (result == RET_NO_ERROR)
			func_error.at(i) = false;
		else
			func_error.at(i) = true;
	}
	return func_error;
}
vector<bool> DeviceManager::ReadAxisError()		// Read&write total axis error (both of MXP and Drive error)
{
	FUNC_STATUS  result;
	vector<bool> func_error(EManager->num_of_axis_, false);
	AXIS_ERROR each_axis_error_state;

	for (int i = 0; i < EManager->num_of_axis_; i++)
	{
		result = MXP->AX_ReadError(i, each_axis_error_state);
		axis_error_state_.push_back(each_axis_error_state);

		if (result == RET_NO_ERROR)
			func_error[i] = false;
		else
			func_error[i] = true;
	}
	return func_error;
}

// Return Axis' Data (same as 'Read Axis' Data' but return value) //

AXIS_STATE DeviceManager::GetAxisStatus(UINT32 axis_no)
{
	return axis_state_.at(axis_no);
}
AXIS_ERROR DeviceManager::GetAxisError(UINT32 axis_no)
{
	return axis_error_state_.at(axis_no);
}
FLOAT DeviceManager::GetAxisPos(UINT32 axis_no)
{
	FLOAT cur_position;
	MXP->AX_ReadActualPosition(axis_no, cur_position);

	return cur_position;
}
FLOAT DeviceManager::GetAxisVel(UINT32 axis_no)
{
	FLOAT cur_velocity;
	MXP->AX_ReadActualVelocity(axis_no, cur_velocity);

	return cur_velocity;
}
FLOAT DeviceManager::GetAxisTor(UINT32 axis_no)
{
	FLOAT cur_torque;
	MXP->AX_ReadActualTorque(axis_no, cur_torque);

	return cur_torque;
}

// Single Axis Motion //

bool DeviceManager::TurnServoOnOff(UINT32 axis_no, bool enable)										// Servo on:TRUE, Servo off:FALSE
{
	FUNC_STATUS  result = MXP->AX_Power(axis_no, enable);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
}
bool DeviceManager::ResetAxis(UINT32 axis_no)													// Reset servo (when axis error)
{
	FUNC_STATUS  result = MXP->AX_Reset(axis_no);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
}
bool DeviceManager::HomeAxis(UINT32 axis_no)													// Home servo
{
	if (IsEmerButtonOn() == true)		// if Emergency button is on, do not move
	{
		return false;
	}

	if (axis_no >= EManager->num_of_axis_)		// wrong axis
	{
		return false;
	}

	FUNC_STATUS  result = MXP->AX_Home(axis_no);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
}
bool DeviceManager::MoveAxis(UINT32 axis_no, FLOAT target_position, FLOAT target_velocity)		// Move absolute position
{
	if (IsEmerButtonOn() == true)		// if Emergency button is on, do not move
	{
		return false;
	}

	if (axis_no >= EManager->num_of_axis_)		// wrong axis
	{
		return false;
	}

	//if (target_position < DManager->software_limit_posisiton[axis_no].first || target_position > DManager->software_limit_posisiton[axis_no].second)		// wrong position
	//{
	//	return FALSE;
	//}

	FUNC_STATUS  result = MXP->AX_MoveAbsolute(axis_no, target_position, target_velocity, target_velocity * static_cast<FLOAT>(SCALE_GEN_ACC), target_velocity * static_cast<FLOAT>(SCALE_GEN_DEC), target_velocity * static_cast<FLOAT>(SCALE_GEN_JERK), MXP_EasyClass::MXP_NONE_DIRECTION, MXP_EasyClass::MXP_ABORTING);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
}
bool DeviceManager::StopAxis(UINT32 axis_no)													// Stop servo
{
	FUNC_STATUS  result = MXP->AX_Halt(axis_no, STOP_DEC, STOP_JERK);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
}

/////////////////////////////
//		     IO            //
/////////////////////////////

// Basic IO (API summary) //

bool DeviceManager::GetIOBitData(IO_SLAVE_NO slave_no, OFFSET offset, BIT_OFFSET bit_offset)							// Return In/Output data of selected IO slave by bit unit
{
	FUNC_STATUS  result;
	UINT32  nBit = 0;
	memcpy(&bit_offset, &nBit, sizeof(nBit));
	bool value;

	result = MXP->IO_ReadBit(static_cast<UINT32> (slave_no), MXP_EasyClass::MXP_IO_Type::IO_In, static_cast<UINT16> (offset), static_cast<UINT8> (bit_offset), value);
	return value;
}
UINT8 DeviceManager::GetIOByteData(IO_SLAVE_NO slave_no, OFFSET offset)												// Return In/Output data of selected IO slave by byte unit
{
	FUNC_STATUS  result;
	UINT8   value;

	result = MXP->IO_ReadByte(static_cast<UINT32> (slave_no), MXP_EasyClass::MXP_IO_Type::IO_In, static_cast<UINT16> (offset), value);
	return value;
}
FUNC_STATUS DeviceManager::WriteIOBitData(IO_SLAVE_NO slave_no, OFFSET offset, BIT_OFFSET bit_offset, bool data)		// Write Output data to selected IO slave by bit unit
{
	FUNC_STATUS  result = MXP->IO_WriteBit(static_cast<UINT32> (slave_no), static_cast<UINT16> (offset), static_cast<UINT8> (bit_offset), data);
	
	return result;
}
FUNC_STATUS DeviceManager::WriteIOByteData(IO_SLAVE_NO slave_no, OFFSET offset, UINT8 data)							// Write Output data to selected IO slave by byte unit
{
	FUNC_STATUS  result = MXP->IO_WriteByte(static_cast<UINT32> (slave_no), static_cast<UINT16> (offset), data);

	return result;
}


// Basic Digital In //

bool DeviceManager::IsEmerButtonOn()	// Check emergency button	ON=True, OFF=False
{
	bool result = DManager->GetIOBitData(IO_SLAVE_NO::DI_EMER_BUTTON, OFFSET::DI_EMER_BUTTON, BIT_OFFSET::DI_EMER_BUTTON);
	return !result;
}
bool DeviceManager::IsMotionButtonOn()	// Check motion button	ON=True, OFF=False
{
	bool result = DManager->GetIOBitData(IO_SLAVE_NO::DI_RUN_BUTTON, OFFSET::DI_RUN_BUTTON, BIT_OFFSET::DI_RUN_BUTTON);
	return result;
}
bool DeviceManager::IsWindowClosed()	// Check window Closed=True, Open=False
{
	bool result = DManager->GetIOBitData(IO_SLAVE_NO::DI_CHECK_WINDOW_OPEN, OFFSET::DI_CHECK_WINDOW_OPEN, BIT_OFFSET::DI_CHECK_WINDOW_OPEN);
	return result;
}

// Basic Digital Out //
// EL2008
bool DeviceManager::TurnAlcoholEjectionMotorOnOff(ONOFF on_off)		// Turn alcohol ejection motor on&off
{
	FUNC_STATUS result = DManager->WriteIOBitData(IO_SLAVE_NO::DO_ALCOHOL_EJECTION_MOTOR, OFFSET::DO_ALCOHOL_EJECTION_MOTOR, BIT_OFFSET::DO_ALCOHOL_EJECTION_MOTOR, on_off);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
	return 0;
}
bool DeviceManager::LoadNeedleMotor(ONOFF on_off)	// Turn needle load motor on&off
{
	FUNC_STATUS result = DManager->WriteIOBitData(IO_SLAVE_NO::DO_NEEDLE_MOTOR, OFFSET::DO_NEEDLE_MOTOR, BIT_OFFSET::DO_NEEDLE_MOTOR, on_off);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
	return 0;
}
bool DeviceManager::TurnHemostasisMotorOnOff(ONOFF on_off)	// Turn hemostasis motor on&off
{
	FUNC_STATUS result = DManager->WriteIOBitData(IO_SLAVE_NO::DO_HEMOSTASIS_MOTOR, OFFSET::DO_HEMOSTASIS_MOTOR, BIT_OFFSET::DO_HEMOSTASIS_MOTOR, on_off);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
}
//bool DeviceManager::SpareDO();
bool DeviceManager::TurnIRSourceOnOff(ONOFF on_off)	// Turn IR source on&off
{
	FUNC_STATUS result = DManager->WriteIOBitData(IO_SLAVE_NO::DO_IR_SOURCE, OFFSET::DO_IR_SOURCE, BIT_OFFSET::DO_IR_SOURCE, on_off);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
}
bool DeviceManager::TurnProbeOnOff(ONOFF on_off)	// Turn probe power on&off
{
	FUNC_STATUS result = DManager->WriteIOBitData(IO_SLAVE_NO::DO_PROBE, OFFSET::DO_PROBE, BIT_OFFSET::DO_PROBE, on_off);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
	return 0;
}
bool DeviceManager::TurnOriginBrakeOnOff(ONOFF on_off)		// Turn origin brake on&off
{
	FUNC_STATUS result = DManager->WriteIOBitData(IO_SLAVE_NO::DO_BRAKE, OFFSET::DO_BRAKE, BIT_OFFSET::DO_BRAKE, on_off);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
	return 0;
}
bool DeviceManager::TurnNeedleValveOnOff(ONOFF on_off)	// Turn needle valve on&off
{
	FUNC_STATUS result = DManager->WriteIOBitData(IO_SLAVE_NO::DO_NEEDLE_LOAD_VALVE, OFFSET::DO_NEEDLE_LOAD_VALVE, BIT_OFFSET::DO_NEEDLE_LOAD_VALVE, on_off);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
	
	return 0;
}

// EL2064
bool DeviceManager::TurnLEDRedOnOff(ONOFF on_off)	// Turn Red LED on&off
{
	FUNC_STATUS result = DManager->WriteIOBitData(IO_SLAVE_NO::DO_LED_RED, OFFSET::DO_LED_RED, BIT_OFFSET::DO_LED_RED, on_off);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
	return 0;
}
bool DeviceManager::TurnLEDGreenOnOff(ONOFF on_off)		// Turn Green LED on&off
{
	FUNC_STATUS result = DManager->WriteIOBitData(IO_SLAVE_NO::DO_LED_GREEN, OFFSET::DO_LED_GREEN, BIT_OFFSET::DO_LED_GREEN, on_off);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
	return 0;
}
bool DeviceManager::TurnLEDBlueOnOff(ONOFF on_off)		// Turn Blue LED on&off
{
	FUNC_STATUS result = DManager->WriteIOBitData(IO_SLAVE_NO::DO_LED_BLUE, OFFSET::DO_LED_BLUE, BIT_OFFSET::DO_LED_BLUE, on_off);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
	return 0;
}
bool DeviceManager::TurnIRLaserOnOff(ONOFF on_off)	// Turn IR laser light on&off
{
	FUNC_STATUS result = DManager->WriteIOBitData(IO_SLAVE_NO::DO_IR_LASER, OFFSET::DO_IR_LASER, BIT_OFFSET::DO_IR_LASER, !on_off);

	if (result == RET_NO_ERROR)
		return true;
	else
	{
		Dev_curr_func_status_ = result;
		return false;
	}
}

// Customized Digital Out //
bool DeviceManager::TurnLEDOnOff(LED_COLOR color, ONOFF on_off)		// Turn on and off selected color LED
{
	bool result;
	switch (color) 
	{
	case RED:
		result = TurnLEDRedOnOff(on_off);
		break;
	case GREEN:
		result = TurnLEDGreenOnOff(on_off);
		break;
	case BLUE:
		result = TurnLEDBlueOnOff(on_off);
		break;
	}
	return result;
}

bool DeviceManager::BlinkLED(LED_COLOR color, second_t interval_p, second_t duration_p)		// Blink selected color LED at regular interval for duration (unit:secs ex. 3.5s)
{
	ONOFF LED_status = ON;
	
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now() + std::chrono::hours(5);
	std::chrono::system_clock::time_point interval_start = std::chrono::system_clock::now();

	std::chrono::duration<double> duration_check = std::chrono::duration<double>(0);
	std::chrono::duration<double> duration = std::chrono::duration<double>(duration_p);
	std::chrono::duration<double> interval_check = std::chrono::duration<double, ratio<3600>>(5);
	std::chrono::duration<double> interval = std::chrono::duration<double>(interval_p);
	
	while (duration_check < duration)										// While check point reaches duration
	{
		TurnLEDOnOff(color, LED_status);

		if (interval_check >= interval)										// For regular interval
		{	
			if (LED_status == ON)											// Switch LED on&off status
				LED_status = OFF;
			else
				LED_status = ON;

			interval_start = std::chrono::system_clock::now();
		}
		
		end = std::chrono::system_clock::now();								// Time update
		duration_check = end - start;
		interval_check = end - interval_start;
	}

	TurnLEDOnOff(color, OFF);

	return true;
}

bool DeviceManager::CreateThreadForBlinkLED(LED_COLOR color, second_t interval, second_t duration)
{
	th1 = std::thread(&DeviceManager(), color, interval, duration);
	std::thread thread_LED(DeviceManager(), color, interval, duration);		 // check out to member function

	if (thread_LED.joinable())
	{
		thread_LED.join();
	}
	return true;
}

bool DeviceManager::TerminateThread()
{
	
	return true;
}

// Basic Analog In //
// EL3162
millimeter_t DeviceManager::GetLaserDistance()		// Get laser distance in mm
{
	millimeter_t distance;
	FLOAT sensor_output_range = 16340;	// About 2^14
	FLOAT actual_distance_ragne = 159.8; // -79.9 to 79.9
	FLOAT slope = actual_distance_ragne / sensor_output_range;
	FLOAT sensor_offset = 16340;
	FLOAT actual_offset = 120.1;
	FLOAT sample_size = 16;
	FLOAT sum = 0;

	for (int i = 0; i < sample_size; i++)	// Average filter
	{
		FLOAT value = (FLOAT)DManager->GetIOByteData(IO_SLAVE_NO::AI_IR_LASER_DISTANCE, OFFSET::AI_IR_LASER_DISTANCE) + byte_size * (FLOAT)DManager->GetIOByteData(IO_SLAVE_NO::AI_IR_LASER_DISTANCE, OFFSET::AI_IR_LASER_DISTANCE_NEXT);
		
		if (value > sensor_output_range)
		{
			sum += sum / (i + 1);
		}
		else
		{
			sum += value;
		}
	}

	distance = (-slope) * (sum / sample_size - sensor_offset) + actual_offset;	// Linear mapping

	return distance;
}

// EL3356
kilogram_t DeviceManager::GetUpperArmLoad()		// Get upper arm load in kg
{
	kilogram_t load = 0;
	kilogram_t value = (FLOAT)DManager->GetIOByteData(IO_SLAVE_NO::AI_UPPER_ARM_LOAD, OFFSET::AI_UPPER_ARM_LOAD) + byte_size * (FLOAT)DManager->GetIOByteData(IO_SLAVE_NO::AI_UPPER_ARM_LOAD, OFFSET::AI_UPPER_ARM_LOAD_NEXT);
	return value;
}
kilogram_t DeviceManager::GetWristLoad()		// Get wrist load in kg
{
	kilogram_t load = 0;
	FLOAT value = (FLOAT)DManager->GetIOByteData(IO_SLAVE_NO::AI_WRIST_LOAD, OFFSET::AI_WRIST_LOAD) + byte_size * (FLOAT)DManager->GetIOByteData(IO_SLAVE_NO::AI_WRIST_LOAD, OFFSET::AI_WRIST_LOAD_NEXT);
	return load;
}
kilogram_t DeviceManager::GetArmRestLoad()		// Get arm rest load in kg
{
	kilogram_t load = 0;
	FLOAT value = (FLOAT)DManager->GetIOByteData(IO_SLAVE_NO::AI_ARM_REST_LOAD, OFFSET::AI_ARM_REST_LOAD) + byte_size * (FLOAT)DManager->GetIOByteData(IO_SLAVE_NO::AI_ARM_REST_LOAD, OFFSET::AI_ARM_REST_LOAD_NEXT);
	return load;
}
kilogram_t DeviceManager::GetProbeLoad()		// Get probe load in kg
{
	kilogram_t load = 0;
	FLOAT value = (FLOAT)DManager->GetIOByteData(IO_SLAVE_NO::AI_PROBE_LOAD, OFFSET::AI_PROBE_LOAD) + byte_size * (FLOAT)DManager->GetIOByteData(IO_SLAVE_NO::AI_PROBE_LOAD, OFFSET::AI_PROBE_LOAD_NEXT);
	return load;
}
kilogram_t DeviceManager::GetAlcoholWeight()		// Get alcohol weight in kg
{
	kilogram_t weight = 0;
	FLOAT value = (FLOAT)DManager->GetIOByteData(IO_SLAVE_NO::AI_ALCOHOL_WEIGHT, OFFSET::AI_ALCOHOL_WEIGHT) + byte_size * (FLOAT)DManager->GetIOByteData(IO_SLAVE_NO::AI_ALCOHOL_WEIGHT, OFFSET::AI_ALCOHOL_WEIGHT_NEXT);
	return weight;
}

void DeviceManager::operator()(LED_COLOR color, second_t interval, second_t duration)
{
	BlinkLED(color, interval, duration);
}


