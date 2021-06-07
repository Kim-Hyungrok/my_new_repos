#pragma once
#include "MXP_EasyClass.h"
#include "pch.h"

class EnumToString
{
public:
#pragma region
public:
	CString EnumToFunctionStatus(MXP_EasyClass::MXP_FUNCTION_STATUS nFunStatus);
	CString EnumToMoveState(MXP_EasyClass::MXP_MOVESTATE nMoveState);
	CString EnumToOnlineMode(MXP_EasyClass::MXP_ONLINE_STATE nOnlineMode);
	CString EnumToPortState(MXP_EasyClass::MXP_PORT_STATE nPortState);
	CString EnumToNodeType(MXP_EasyClass::MXP_NODE_TYPE nNodeType);
	CString EnumToCamStatus(MXP_EasyClass::MXP_CAM_STATUS nCamStatus);
#pragma endregion EnumToString
	EnumToString(void);
	~EnumToString(void);
};
extern EnumToString *ENUMTOSTRING;
