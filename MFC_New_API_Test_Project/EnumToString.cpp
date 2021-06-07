#include "pch.h"
#include "EnumToString.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h> 
#include <vector>
#include <math.h> 
#include<string>

EnumToString *ENUMTOSTRING;
EnumToString::EnumToString(void)
{
	ENUMTOSTRING = this;
}

EnumToString::~EnumToString(void)
{
	delete ENUMTOSTRING;
}
#pragma region 

CString EnumToString::EnumToFunctionStatus(MXP_EasyClass::MXP_FUNCTION_STATUS nFunStatus)
{
	CString strFunStatus;

	switch(nFunStatus)
	{
	case MXP_EasyClass::RET_NO_ERROR :
		strFunStatus = _T("RET_NO_ERROR");
		break;
	case MXP_EasyClass::RET_ERROR_FUNCTION :
		strFunStatus = _T("RET_ERROR_FUNCTION");
		break;
	case MXP_EasyClass::RET_ERROR_FULL : 
		strFunStatus = _T("RET_ERROR_FULL");
		break;
	case MXP_EasyClass::RET_ERROR_WRONG_INDEX : 
		strFunStatus = _T("RET_ERROR_WRONG_INDEX");
		break;
	case MXP_EasyClass::RET_ERROR_WRONG_AXISNO : 
		strFunStatus = _T("RET_ERROR_WRONG_AXISNO");
		break;
	case MXP_EasyClass::RET_ERROR_MOTIONBUSY : 
		strFunStatus = _T("RET_ERROR_MOTIONBUSY");
		break;
	case MXP_EasyClass::RET_ERROR_WRONG_SLAVENO : 
		strFunStatus = _T("RET_ERROR_WRONG_SLAVENO");
		break;
	case MXP_EasyClass::RET_ERROR_WRONG_CAMTABLENO : 
		strFunStatus = _T("RET_ERROR_WRONG_CAMTABLENO");
		break;
	case MXP_EasyClass::RET_ERROR_WRONG_ECMASTERNO : 
		strFunStatus = _T("RET_ERROR_WRONG_ECMASTERNO");
		break;
	case MXP_EasyClass::RET_ERROR_WRONG_ECSLAVENO : 
		strFunStatus = _T("RET_ERROR_WRONG_ECSLAVENO");
		break;
	case MXP_EasyClass::RET_ERROR_NOT_OPMODE :
		strFunStatus = _T("RET_ERROR_NOT_OPMODE");
		break;
	case MXP_EasyClass::RET_ERROR_NOTRUNNING :
		strFunStatus = _T("RET_ERROR_NOTRUNNING");
		break;
	case MXP_EasyClass::RET_ERROR_WRONG_PARAMETER_NO :
		strFunStatus = _T("RET_ERROR_WRONG_PARAMETER_NO");
		break;
	case MXP_EasyClass::RET_ERROR_WRONG_MXP_TYPE : 
		strFunStatus = _T("RET_ERROR_WRONG_MXP_TYPE");
		break;
	case MXP_EasyClass::RET_ERROR_ALREADYOPEN : 
		strFunStatus = _T("RET_ERROR_ALREADYOPEN");
		break;
	case MXP_EasyClass::RET_ERROR_NOT_SCANMODE : 
		strFunStatus = _T("RET_ERROR_NOT_SCANMODE");
		break;
	case MXP_EasyClass::RET_ERROR_WRONG_ONLINESTATE : 
		strFunStatus = _T("RET_ERROR_WRONG_ONLINESTATE");
		break;
	case MXP_EasyClass::RET_ERROR_NOT_SIMULATIONMODE : 
		strFunStatus = _T("RET_ERROR_NOT_SIMULATIONMODE");
		break;
	case MXP_EasyClass::RET_ERROR_NOT_FOEMODE : 
		strFunStatus = _T("RET_ERROR_NOT_FOEMODE");
		break;
	case MXP_EasyClass::RET_ERROR_NOT_INVALID_LIC_FEATURE : 
		strFunStatus = _T("RET_ERROR_NOT_INVALID_LIC_FEATURE");
		break;
	case MXP_EasyClass::RET_ERROR_INVAILD_LASTSTEPVEL : 
		strFunStatus = _T("RET_ERROR_INVAILD_LASTSTEPVEL");
		break;
	case MXP_EasyClass::RET_ERROR_INVAILD_LASTTEPPOS :
		strFunStatus = _T("RET_ERROR_INVAILD_LASTTEPPOS");
		break;
	case MXP_EasyClass::RET_ERROR_INVAILD_FIRSTSTEPVEL :
		strFunStatus = _T("RET_ERROR_INVAILD_FIRSTSTEPVEL");
		break;
	case MXP_EasyClass::RET_ERROR_INVAILD_IOSize :
		strFunStatus = _T("RET_ERROR_INVAILD_IOSize");
		break;
	case MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK : 
		strFunStatus = _T("RET_ERROR_FAIL_INDEX_CHECK");
		break;
	case MXP_EasyClass::RET_ERROR_EXCEPTIONERROR : 
		strFunStatus = _T("RET_ERROR_EXCEPTIONERROR");
		break;
	case MXP_EasyClass::RET_ERROR_STATECHECK_FAIL : 
		strFunStatus = _T("RET_ERROR_STATECHECK_FAIL");
		break;
	case MXP_EasyClass::RET_INVAILD_PLCADDRESS :
		strFunStatus = _T("RET_INVAILD_PLCADDRESS");
		break;
	case MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL :
		strFunStatus = _T("RET_SEQUENCEMOVE_READ_FAIL");
		break;
	case MXP_EasyClass::RET_SEQUENCEMOVE_INVAILDSTATE :
		strFunStatus = _T("RET_SEQUENCEMOVE_INVAILDSTATE");
		break;
	case MXP_EasyClass::RET_SEQUENCEMOVE_PROCESS_FAIL :
		strFunStatus = _T("RET_SEQUENCEMOVE_PROCESS_FAIL");
		break;
	}
	return strFunStatus;
}
CString EnumToString::EnumToMoveState(MXP_EasyClass::MXP_MOVESTATE nMoveState)
{
	CString strMoveState;

	switch(nMoveState)
	{
	case MXP_EasyClass::MOVESTATE_Null : 
		strMoveState = _T("MOVESTATE_Null");
		break;
	case MXP_EasyClass::MOVESTATE_Moving : 
		strMoveState = _T("MOVESTATE_Moving");
		break;
	case MXP_EasyClass::MOVESTATE_Complete : 
		strMoveState = _T("MOVESTATE_Complete");
		break;
	case MXP_EasyClass::MOVESTATE_Fail :
		strMoveState = _T("MOVESTATE_Fail");
		break;
	}

	return strMoveState;
}
CString EnumToString::EnumToOnlineMode(MXP_EasyClass::MXP_ONLINE_STATE nOnlineMode)
{
	CString strOnlineMode;

	switch(nOnlineMode)
	{
	case MXP_EasyClass::ONLINE_STATE_INIT : 
		strOnlineMode = _T("MXP_ONLINE_STATE_INIT");
		break;
	case MXP_EasyClass::ONLINE_STATE_PREOP : 
		strOnlineMode = _T("MXP_ONLINE_STATE_PREOP");
		break;
	case MXP_EasyClass::ONLINE_STATE_BOOT : 
		strOnlineMode = _T("MXP_ONLINE_STATE_BOOT");
		break;
	case MXP_EasyClass::ONLINE_STATE_SAFEOP :
		strOnlineMode = _T("MXP_ONLINE_STATE_SAFEOP");
		break;
	case MXP_EasyClass::ONLINE_STATE_OP : 
		strOnlineMode = _T("MXP_ONLINE_STATE_OP");
		break;
	case MXP_EasyClass::ONLINE_STATE_ERROR_NONE : 
		strOnlineMode = _T("MXP_ONLINE_STATE_ERROR_NONE");
		break;
	case MXP_EasyClass::ONLINE_STATE_ERROR_INIT : 
		strOnlineMode = _T("MXP_ONLINE_STATE_ERROR_INIT");
		break;
	case MXP_EasyClass::ONLINE_STATE_ERROR_PREOP :
		strOnlineMode = _T("MXP_ONLINE_STATE_ERROR_PREOP");
		break;
	case MXP_EasyClass::ONLINE_STATE_ERROR_BOOT : 
		strOnlineMode = _T("MXP_ONLINE_STATE_ERROR_BOOT");
		break;
	case MXP_EasyClass::ONLINE_STATE_ERROR_SAFEOP : 
		strOnlineMode = _T("MXP_ONLINE_STATE_ERROR_SAFEOP");
		break;
	case MXP_EasyClass::ONLINE_STATE_ERROR_OP : 
		strOnlineMode = _T("MXP_ONLINE_STATE_ERROR_OP");
		break;
	}

	return strOnlineMode;
}
CString EnumToString::EnumToPortState(MXP_EasyClass::MXP_PORT_STATE nPortState)
{
	CString strPortState;

	switch(nPortState)
	{
	case MXP_EasyClass::DL_STATUS_LOOP_OPEN_NO_LINK : 
		strPortState = _T("DL_STATUS_LOOP_OPEN_NO_LINK");
		break;
	case MXP_EasyClass::DL_STATUS_LOOP_CLOSE_NO_LINK : 
		strPortState = _T("DL_STATUS_LOOP_CLOSE_NO_LINK");
		break;
	case MXP_EasyClass::DL_STATUS_LOOP_OPEN_WITH_LINK : 
		strPortState = _T("DL_STATUS_LOOP_OPEN_WITH_LINK");
		break;
	case MXP_EasyClass::DL_STATUS_LOOP_CLOSE_WITH_LINK :
		strPortState = _T("DL_STATUS_LOOP_CLOSE_WITH_LINK");
		break;
	}

	return strPortState;
}
CString EnumToString::EnumToNodeType(MXP_EasyClass::MXP_NODE_TYPE nNodeType)
{
	CString strNodeType;

	switch(nNodeType)
	{
	case MXP_EasyClass::TYPE_NUll : 
		strNodeType = _T("TYPE_NUll");
		break;
	case MXP_EasyClass::TYPE_DRIVE : 
		strNodeType = _T("TYPE_DRIVE");
		break;
	case MXP_EasyClass::TYPE_IO : 
		strNodeType = _T("TYPE_IO");
		break;
	}

	return strNodeType;
}
CString EnumToString::EnumToCamStatus(MXP_EasyClass::MXP_CAM_STATUS nCamStatus)
{
	CString strCamStatus;

	switch(nCamStatus)
	{
	case MXP_EasyClass::RETCAM_NO_ERROR : 
		strCamStatus = _T("RETCAM_NO_ERROR");
		break;
	case MXP_EasyClass::RETCAM_ERROR_TABLEID : 
		strCamStatus = _T("RETCAM_ERROR_TABLEID");
		break;
	case MXP_EasyClass::RETCAM_ERROR_INPUTPARAM : 
		strCamStatus = _T("RETCAM_ERROR_INPUTPARAM");
		break;
	case MXP_EasyClass::RETCAM_ERROR_DATASIZE : 
		strCamStatus = _T("RETCAM_ERROR_DATASIZE");
		break;
	case MXP_EasyClass::RETCAM_ERROR_INTERPOLATION : 
		strCamStatus = _T("RETCAM_ERROR_INTERPOLATION");
		break;
	case MXP_EasyClass::RETCAM_ERROR_MASTERPOS : 
		strCamStatus = _T("RETCAM_ERROR_MASTERPOS");
		break;
	case MXP_EasyClass::RETCAM_ERROR_EXECUTIONMODE : 
		strCamStatus = _T("RETCAM_ERROR_EXECUTIONMODE");
		break;
	}

	return strCamStatus;
}

#pragma endregion EnumToString