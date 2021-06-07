#pragma once

#include "pch.h"
#include "MXP_EasyClass.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include "json.h"

using namespace std;


MXP_EasyClass::AxisSequenceData mAxisSequenceData[128] = {0,};
MXP_EasyClass::GroupSequenceData mGroupSequenceData[32] = {0,};
MXP_EasyClass::strExAPIAxisParam mExAPIAxisParam[128] = {0,};
MXP_EasyClass::stPLCFBRunInfo mPLCFBInfo[128] = {0,};
int nAxis_TaskPos[128] = {0,};
int nGroup_TaskPos[32] = {0,};
MXP_EasyClass::MXP_FUNCTION_STATUS nAxisSeq_Result[128];
const int PLCStartAddress = 5000;
const int PLCEndAddress = 6000;
const int FeedBackSensorFBSize = 40;
const int SpinFBSize = 10;

string intToString(int n)
{
	string str;
	stringstream s;
	s << n;
	str = s.str();
	return str;
}
string floatToString(float n)
{
	string str;
	stringstream s;
	s << n;
	str = s.str();
	return str;
}
void intToBytes(UINT8* arrayByte, int paramInt)
{
	for (int i = 0; i < 4; i++)
	{
		arrayByte[3 - i] = (paramInt >> (i * 8));
	}
}
void floatToByte(UINT8* bytes, float f)
{

	int length = sizeof(float);

	for(int i = 0; i < length; i++)
	{
		bytes[i] = ((UINT8*)&f)[i];
	}
}
void Uint16ToByte(UINT8* bytes, UINT16 nParam){

	int length = sizeof(UINT16);

	for(int i = 0; i < length; i++){
		bytes[i] = ((UINT8*)&nParam)[i];
	}
}
UINT32 byteToUINT32(UINT8* arrByte)
{
	UINT32 nParam;
	nParam = arrByte[0] | (arrByte[1] << 8) | (arrByte[2] << 16) | (arrByte[3] << 24);
	return nParam;
}
UINT64 byteToUINT64(UINT8* arrByte)
{
	UINT64 nParam;
	nParam = (arrByte[0]) | (arrByte[1]) << 8 |	(arrByte[2]) << 16 | (arrByte[3]) << 24 | (arrByte[4]) << 32 |
		(arrByte[5]) << 40 | (arrByte[6]) << 48 | (arrByte[7]) << 56;
	return nParam;
}
vector<string> split(string str, char delimiter) {
	vector<string> internal;
	std::stringstream strem(str);
	string temp;

	while (getline(strem, temp, delimiter)) {
		internal.push_back(temp);
	}

	return internal;
}
std::string replace(std::string source, const std::string &find, const std::string &replace)
{
	std::size_t pos = 0;
	while ((pos = source.find(find, pos)) != std::string::npos)
	{
		source.replace(pos, find.length(), replace);
		pos += replace.length();
	}
	return source;
}

void MXP_EasyClass::Tick()
{
	for(int i =0; i< 128; i++)
	{
		MXP_EasyClass::SEQ_AxisSequenceMoveManager(i);
	}
	for(int i =0; i< 32; i++)
	{
		MXP_EasyClass::SEQ_GroupSequenceMoveManager(i);
	}
}
void CALLBACK MXP_EasyClass::OnTick(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	MXP_EasyClass *MXPEasyClass = (MXP_EasyClass *)dwUser;
	MXPEasyClass->Tick();
}

MXP_EasyClass::MXP_EasyClass()
{

	m_idEvent = timeSetEvent(5, 0, (LPTIMECALLBACK)OnTick, (DWORD)this, TIME_PERIODIC);
	AX_Init_ExAPI_Param();
	FBInfoReset(true);
}

MXP_EasyClass::~MXP_EasyClass()
{
 	if (m_idEvent)
 	{
 		timeKillEvent(m_idEvent);
 	}
}

#pragma region
class DataVaildCheck
{
public:
	friend class TXTLoader;

	bool FCheckDataVaild(MXP_EasyClass::StSequenceMove stReadData, std::vector<bool> &arrCheckData)
	{
		if(arrCheckData[MXP_EasyClass::Axis_AxisGroupNo] == FALSE)
			return FALSE;
		if(arrCheckData[MXP_EasyClass::Axis_MoveType] == FALSE)
			return FALSE;
		if(arrCheckData[MXP_EasyClass::Axis_nCmdType] == FALSE)
			return FALSE;
		switch (stReadData.nCmdType)
		{
		case MXP_EasyClass::e_AbsMove:
		case MXP_EasyClass::e_RelativeMove:
			{
				if (arrCheckData[MXP_EasyClass::Axis_fJerk] == FALSE || stReadData.fJerk == 0)return FALSE;
				if (arrCheckData[MXP_EasyClass::Axis_fAcc] == FALSE || stReadData.fAcc == 0) return FALSE;
				if (arrCheckData[MXP_EasyClass::Axis_fDec] == FALSE || stReadData.fDec == 0) return FALSE;
				if (arrCheckData[MXP_EasyClass::Axis_fPos] == FALSE) return FALSE;
				if (arrCheckData[MXP_EasyClass::Axis_fVel] == FALSE || stReadData.fVel == 0) return FALSE;
				if (arrCheckData[MXP_EasyClass::Axis_nBufferMode] == FALSE || stReadData.nBufferMode == 0) return FALSE;
				break;
			}
		case MXP_EasyClass::e_Dwell:
			{
				if (arrCheckData[MXP_EasyClass::Axis_fDwellTime] == FALSE || stReadData.fDwellTime == 0) return FALSE;
				break;
			}
		case MXP_EasyClass::e_IO:
			{
				if (arrCheckData[MXP_EasyClass::Axis_nIOBitNo] == FALSE) return FALSE;
				if (arrCheckData[MXP_EasyClass::Axis_nIOSlaveNo] == FALSE) return FALSE;
				if (arrCheckData[MXP_EasyClass::Axis_bIOBitSet] == FALSE) return FALSE;
				break;
			}
		default:
			break;
		}
		return true;
	}
	bool FCheckGroupDataVaild(MXP_EasyClass::StGroupSequenceMove stReadData, std::vector<bool> &arrCheckData)
	{
		if(arrCheckData[MXP_EasyClass::Group_AxisGroupNo] == FALSE)
			return FALSE;
		if(arrCheckData[MXP_EasyClass::Group_MoveType] == FALSE)
			return FALSE;
		if(arrCheckData[MXP_EasyClass::Group_nCmdType] == FALSE)
			return FALSE;
		switch (stReadData.nCmdType)
		{
		case MXP_EasyClass::e_AbsMove:
		case MXP_EasyClass::e_RelativeMove:
			{
				if (arrCheckData[MXP_EasyClass::Group_fJerk] == FALSE || stReadData.fJerk == 0)return FALSE;
				if (arrCheckData[MXP_EasyClass::Group_fAcc] == FALSE || stReadData.fAcc == 0) return FALSE;
				if (arrCheckData[MXP_EasyClass::Group_fDec] == FALSE || stReadData.fDec == 0) return FALSE;
				if (arrCheckData[MXP_EasyClass::Group_fPos] == FALSE) return FALSE;
				if (arrCheckData[MXP_EasyClass::Group_fVel] == FALSE || stReadData.fVel == 0) return FALSE;
				if (arrCheckData[MXP_EasyClass::Group_nBufferMode] == FALSE || stReadData.nBufferMode == 0) return FALSE;
				break;
			}
		case MXP_EasyClass::e_Dwell:
			{
				if (arrCheckData[MXP_EasyClass::Group_fDwellTime] == FALSE || stReadData.fDwellTime == 0) return FALSE;
				break;
			}
		case MXP_EasyClass::e_IO:
			{
				if (arrCheckData[MXP_EasyClass::Group_nIOBitNo] == FALSE) return FALSE;
				if (arrCheckData[MXP_EasyClass::Group_nIOSlaveNo] == FALSE) return FALSE;
				if (arrCheckData[MXP_EasyClass::Group_bIOBitSet] == FALSE) return FALSE;
				break;
			}
		default:
			break;
		}
		
		return true;
	}
};
#pragma endregion datavaildcheck

#pragma region
class TXTLoader
{
public:

	DataVaildCheck *DataCheck;
	bool TXTLoader::AxisSequenceFileLoad(const std::string strFilePath, unsigned int &nAxisNo, std::vector<MXP_EasyClass::StSequenceMove> &arrData, std::string &strError)
	{
		nAxisNo = 0;
		arrData = std::vector<MXP_EasyClass::StSequenceMove>(1);
		std::ifstream readFile;
		try
		{
			std::string nReadStr = "";
			std::vector<std::string> strSplit(0);
			std::vector<std::string> strParam(0);
			
			readFile.open(strFilePath);

			if (!readFile)
			{
				return false;
			}

			int nLineCount = 0;
			std::vector<bool> bDataCheck(static_cast<int>(MXP_EasyClass::Axis_End));

			int paramCount = 0;
			
			while (! readFile.eof())
			{
				nLineCount = nLineCount + 1;
				std::getline(readFile, nReadStr);

				if (nReadStr.length() != 0)
				{
					nReadStr = replace(nReadStr, "\"", "");

					if (nReadStr.substr(0,1) != "#")
					{
						strSplit = split(nReadStr, L'\t');

						if (strSplit.size() < 2)
						{
							strError = "Line " + intToString(nLineCount) + " data is invalid";
							break;
						}
						if (strSplit[0] == "MoveType")
						{
							if (std::stoi(strSplit[1]) != 0)
							{
								strError = "MoveType data is invalid";
								return false;
							}
							bDataCheck[static_cast<int>(MXP_EasyClass::Axis_MoveType)] = true;
						}
						else if (strSplit[0] == "Axis_GroupNo")
						{
							unsigned int nReadAxisNo = std::stoul(strSplit[1]);

							if (nReadAxisNo < 0 || nReadAxisNo > 127)
							{
								strError = "Axis_GroupNo data is invalid";
								return false;
							}
							nAxisNo = nReadAxisNo;

							bDataCheck[static_cast<int>(MXP_EasyClass::Axis_AxisGroupNo)] = true;
						}
						else
						{
							arrData.resize(paramCount+1);

							arrData[paramCount].bIOBitSet = false;
							arrData[paramCount].fAcc = 0.0;
							arrData[paramCount].fVel = 0.0;
							arrData[paramCount].fPos = 0.0;
							arrData[paramCount].fDec = 0.0;
							arrData[paramCount].fJerk = 0.0;
							arrData[paramCount].fDwellTime = 0.0;
							arrData[paramCount].strStepName = "";
							arrData[paramCount].nBufferMode = (MXP_EasyClass::MXP_BUFFERMODE_ENUM)0;
							arrData[paramCount].nCmdType = (MXP_EasyClass::ESequenceCmdType)0;
							arrData[paramCount].nDirection = (MXP_EasyClass::MXP_DIRECTION_ENUM)0;

							strParam = split(strSplit[1], L',');

							if (strParam.size() < 2)
							{
								strError =  "Line" + intToString(static_cast<int>(nLineCount)) + " data is invalid";
								return false;
							}

							

							arrData[paramCount].strStepName = strSplit.front();
														
							for (int nDataCheckCount = static_cast<int>(MXP_EasyClass::Axis_AxisGroupNo) + 1; nDataCheckCount < static_cast<int>(MXP_EasyClass::Axis_End); nDataCheckCount++)
							{
								bDataCheck[nDataCheckCount] = false;
							}

							for (int nParamCount = 0; nParamCount < (int)strParam.size(); nParamCount++)
							{
								std::vector<std::string> strSplitParam = split(strParam[nParamCount], L':');

								if (strSplitParam.size() < 2)
								{
									strError =  "Line" + intToString(static_cast<int>(nLineCount)) + " data is invalid";
									return false;
								}
								if (strSplitParam[0] == "nCmdType")
								{
									arrData[paramCount].nCmdType = (MXP_EasyClass::ESequenceCmdType)(std::stoi(strSplitParam[1]));
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_nCmdType)] = true;
								}
								else if (strSplitParam[0] == "nDirection")
								{
									arrData[paramCount].nDirection = (MXP_EasyClass::MXP_DIRECTION_ENUM)(std::stoi(strSplitParam[1]));
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_nDirection)] = true;
								}
								else if (strSplitParam[0] == "fPos")
								{
									arrData[paramCount].fPos = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_fPos)] = true;
								}
								else if (strSplitParam[0] == "fVel")
								{
									arrData[paramCount].fVel = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_fVel)] = true;
								}
								else if (strSplitParam[0] == "fAcc")
								{
									arrData[paramCount].fAcc = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_fAcc)] = true;
								}
								else if (strSplitParam[0] == "fDec")
								{
									arrData[paramCount].fDec = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_fDec)] = true;
								}
								else if (strSplitParam[0] == "fJerk")
								{
									arrData[paramCount].fJerk = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_fJerk)] = true;
								}
								else if (strSplitParam[0] == "fDwellTime")
								{
									arrData[paramCount].fDwellTime = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_fDwellTime)] = true;
								}
								else if (strSplitParam[0] == "nIOSlaveNo")
								{
									arrData[paramCount].nIOSlaveNo = std::stoi(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_nIOSlaveNo)] = true;
								}
								else if (strSplitParam[0] == "nIOBitNo")
								{
									arrData[paramCount].nIOBitNo = (UINT16)std::stoul(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_nIOBitNo)] = true;
								}
								else if (strSplitParam[0] == "bIOBitSet")
								{
									if (std::stoi(strSplitParam[1]) == 1)
									{
										arrData[paramCount].bIOBitSet = true;
									}
									else
									{
										arrData[paramCount].bIOBitSet = false;
									}

									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_bIOBitSet)] = true;
								}
								else if (strSplitParam[0] == "nBufferMode")
								{
									arrData[paramCount].nBufferMode = (MXP_EasyClass::MXP_BUFFERMODE_ENUM)(std::stoi(strSplitParam[1]));
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_nBufferMode)] = true;
								}
							}

 							if (DataCheck->FCheckDataVaild(arrData[paramCount], bDataCheck) == false)
 							{
 								strError = "Step " + intToString(paramCount + 1) + " data is invalid";
 								return false;
 							}

							paramCount++;

						}
					}
				}
			}

			readFile.close();

			return true;
		}
		catch (invalid_argument e)
		{
			strError = e.what();
			readFile.close();
			return FALSE;
		}
	}

	bool TXTLoader::GroupSequenceFileLoad(const std::string strFilePath, unsigned int &nAxesGroup, std::vector<MXP_EasyClass::StGroupSequenceMove> &arrData, std::string &strError)
	{
		nAxesGroup = 0;
		arrData = std::vector<MXP_EasyClass::StGroupSequenceMove>(1);
		std::ifstream readFile;
		try
		{
			std::string nReadStr = "";
			std::vector<std::string> strSplit(0);
			std::vector<std::string> strParam(0);


			readFile.open(strFilePath);

			if (!readFile)
			{
				return false;
			}

			int nLineCount = 0;
			std::vector<bool> bDataCheck(static_cast<int>(MXP_EasyClass::Group_End));

			int paramCount = 0;

			while (! readFile.eof())
			{
				nLineCount = nLineCount + 1;
				std::getline(readFile, nReadStr);

				if (nReadStr.length() != 0)
				{
					nReadStr = replace(nReadStr, "\"", "");

					if (nReadStr.substr(0,1) != "#")
					{
						strSplit = split(nReadStr, L'\t');

						if (strSplit.size() < 2)
						{
							strError = "Line " + intToString(nLineCount) + " data is invalid";
							break;
						}
						if (strSplit[0] == "MoveType")
						{
							if (std::stoi(strSplit[1]) != 1)
							{
								strError = "MoveType data is invalid";
								return false;
							}
							bDataCheck[static_cast<int>(MXP_EasyClass::Group_MoveType)] = true;
						}
						else if (strSplit[0] == "Axis_GroupNo")
						{
							unsigned int nReadGroupNo = std::stoul(strSplit[1]);

							if (nReadGroupNo < 0 || nReadGroupNo > 31)
							{
								strError = "Axis_GroupNo data is invalid";
								return false;
							}
							nAxesGroup = nReadGroupNo;

							bDataCheck[static_cast<int>(MXP_EasyClass::Group_AxisGroupNo)] = true;
						}
						else
						{
							arrData.resize(paramCount+1);
							///초기화
							arrData[paramCount].bIOBitSet = false;
							arrData[paramCount].fAcc = 0.0;
							arrData[paramCount].fVel = 0.0;
							arrData[paramCount].fXPos = 0.0;
							arrData[paramCount].fYPos = 0.0;
							arrData[paramCount].fZPos = 0.0;
							arrData[paramCount].fUPos = 0.0;
							arrData[paramCount].fVPos = 0.0;
							arrData[paramCount].fWPos = 0.0;
							arrData[paramCount].fAPos = 0.0;
							arrData[paramCount].fBPos = 0.0;
							arrData[paramCount].fCPos = 0.0;
							arrData[paramCount].fDec = 0.0;
							arrData[paramCount].fJerk = 0.0;
							arrData[paramCount].fDwellTime = 0.0;
							arrData[paramCount].strStepName = "";
							arrData[paramCount].nBufferMode = (MXP_EasyClass::MXP_BUFFERMODE_ENUM)0;
							arrData[paramCount].nCmdType = (MXP_EasyClass::ESequenceCmdType)0;

							strParam = split(strSplit[1], L',');

							if (strParam.size() < 2)
							{
								strError =  "Line" + intToString(static_cast<int>(nLineCount)) + " data is invalid";
								return false;
							}

							arrData[paramCount].strStepName = strSplit.front();

							for (int nDataCheckCount = static_cast<int>(MXP_EasyClass::Group_AxisGroupNo) + 1; nDataCheckCount < static_cast<int>(MXP_EasyClass::Group_End); nDataCheckCount++)
							{
								bDataCheck[nDataCheckCount] = false;
							}

							for (int nParamCount = 0; nParamCount < (int)strParam.size(); nParamCount++)
							{
								std::vector<std::string> strSplitParam = split(strParam[nParamCount], L':');

								if (strSplitParam.size() < 2)
								{
									strError =  "Line" + intToString(static_cast<int>(nLineCount)) + " data is invalid";
									return false;
								}
								if (strSplitParam[0] == "nCmdType")
								{
									arrData[paramCount].nCmdType = (MXP_EasyClass::ESequenceCmdType)(std::stoi(strSplitParam[1]));
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_nCmdType)] = true;
								}
								else if (strSplitParam[0] == "fXPos")
								{
									arrData[paramCount].fXPos = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strSplitParam[0] == "fYPos")
								{
									arrData[paramCount].fYPos = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strSplitParam[0] == "fZPos")
								{
									arrData[paramCount].fZPos = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strSplitParam[0] == "fUPos")
								{
									arrData[paramCount].fUPos = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strSplitParam[0] == "fVPos")
								{
									arrData[paramCount].fVPos = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strSplitParam[0] == "fWPos")
								{
									arrData[paramCount].fWPos = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strSplitParam[0] == "fAPos")
								{
									arrData[paramCount].fAPos = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strSplitParam[0] == "fBPos")
								{
									arrData[paramCount].fBPos = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strSplitParam[0] == "fCPos")
								{
									arrData[paramCount].fCPos = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strSplitParam[0] == "fVel")
								{
									arrData[paramCount].fVel = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fVel)] = true;
								}
								else if (strSplitParam[0] == "fAcc")
								{
									arrData[paramCount].fAcc = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fAcc)] = true;
								}
								else if (strSplitParam[0] == "fDec")
								{
									arrData[paramCount].fDec = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fDec)] = true;
								}
								else if (strSplitParam[0] == "fJerk")
								{
									arrData[paramCount].fJerk = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fJerk)] = true;
								}
								else if (strSplitParam[0] == "fDwellTime")
								{
									arrData[paramCount].fDwellTime = std::stof(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fDwellTime)] = true;
								}
								else if (strSplitParam[0] == "nIOSlaveNo")
								{
									arrData[paramCount].nIOSlaveNo = std::stoi(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_nIOSlaveNo)] = true;
								}
								else if (strSplitParam[0] == "nIOBitNo")
								{
									arrData[paramCount].nIOBitNo = (UINT16)std::stoul(strSplitParam[1]);
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_nIOBitNo)] = true;
								}
								else if (strSplitParam[0] == "bIOBitSet")
								{
									if (std::stoi(strSplitParam[1]) == 1)
									{
										arrData[paramCount].bIOBitSet = true;
									}
									else
									{
										arrData[paramCount].bIOBitSet = false;
									}

									bDataCheck[static_cast<int>(MXP_EasyClass::Group_bIOBitSet)] = true;
								}
								else if (strSplitParam[0] == "nBufferMode")
								{
									arrData[paramCount].nBufferMode = (MXP_EasyClass::MXP_BUFFERMODE_ENUM)(std::stoi(strSplitParam[1]));
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_nBufferMode)] = true;
								}
							}

							if (DataCheck->FCheckGroupDataVaild(arrData[paramCount], bDataCheck) == false)
							{
								strError = "Step " + intToString(paramCount + 1) + " data is invalid";
								return false;
							}

							paramCount++;

						}
					}
				}
			}

			readFile.close();

			return true;
		}
		catch (invalid_argument e)
		{
			strError = e.what();
			readFile.close();
			return FALSE;
		}
	}
};

class JSONLoader
{
public:
	DataVaildCheck *DataCheck;
	ifstream read;
	bool AxisSequenceFileLoad(const std::string strFilePath, unsigned int &nAxisNo, std::vector<MXP_EasyClass::StSequenceMove> &arrData, std::string &strError)
	{
		arrData = std::vector<MXP_EasyClass::StSequenceMove>(1);
		try
		{
			string nReadStr = "";
			Json::Reader reader;
			Json::Value root;
			Json::Value sequenceMoveData;
			std::vector<bool> bDataCheck(static_cast<int>(MXP_EasyClass::Axis_End));
			ifstream readFile(strFilePath, ifstream::binary);
			string strName, strName1;
			int i = 0;

			if (!readFile)
			{
				return false;
			}
			
			reader.parse(readFile,root);//json >> root; 두 구문 모두 json을 파싱하는데 사용할 수 있다.
			//전체 key()를 읽어보기
			sequenceMoveData = root["SequenceMove"];
			for (auto it = sequenceMoveData.begin(); it != sequenceMoveData.end(); it++)
			{
				auto dataName = sequenceMoveData.getMemberNames();
				for each (auto strDataName in dataName)
				{
					if (strDataName == "MoveType(0:Axis,1:Group)")
					{
						if(sequenceMoveData["MoveType(0:Axis,1:Group)"].asInt() != 0)
						{
							strError = "MoveType(0:Axis,1:Group) data is invalid";
							return false;
						}
						bDataCheck[static_cast<int>(MXP_EasyClass::Axis_MoveType)] = true;
					}
					else if (strDataName == "Axis_GroupNo")
					{
						nAxisNo = sequenceMoveData["Axis_GroupNo"].asInt();
						if(nAxisNo <0 || nAxisNo > 127)
						{
							strError = "Axis_GroupNo data is invalid";
							return false;
						}
						bDataCheck[static_cast<int>(MXP_EasyClass::Axis_AxisGroupNo)] = true;
					}
					else if (strDataName == "StepData")
					{
						for (int nDataCheckCount = static_cast<int>(MXP_EasyClass::Axis_AxisGroupNo) + 1; nDataCheckCount < static_cast<int>(MXP_EasyClass::Axis_End); nDataCheckCount++)
						{
							bDataCheck[nDataCheckCount] = false;
						}
						auto  stepData = sequenceMoveData["StepData"];
						
						arrData.resize(stepData.size());
						for (i=0; i< (int)stepData.size(); i++)
						{
							//auto dataArray = stepData[i];
							auto dataName1 = stepData[i].getMemberNames();
							for each (auto strDataName1 in dataName1)
						    {
								Json::Value arrStepData = stepData[i];
								if (strDataName1 == "strStepName")
									arrData[i].strStepName = stepData[i]["strStepName"].asString();
								else if (strDataName1 == "nCmdType")
								{
									arrData[i].nCmdType = (MXP_EasyClass::ESequenceCmdType)(stepData[i]["nCmdType"].asInt());
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_nCmdType)] = true;
								}
								else if (strDataName1 =="nDirection")
								{
									arrData[i].nDirection = (MXP_EasyClass::MXP_DIRECTION_ENUM)(stepData[i]["nDirection"].asInt());
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_nDirection)] = true;
								}
								else if (strDataName1 =="fPos")
								{
									arrData[i].fPos = stepData[i]["fPos"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_fPos)] = true;
								}
								else if (strDataName1 =="fVel")
								{
									arrData[i].fVel = stepData[i]["fVel"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_fVel)] = true;
								}
								else if (strDataName1 == "fAcc")
								{
									arrData[i].fAcc = stepData[i]["fAcc"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_fAcc)] = true;
								}
								else if (strDataName1 =="fDec")
								{
									arrData[i].fDec = stepData[i]["fDec"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_fDec)] = true;
								}
								else if (strDataName1 =="fJerk")
								{
									arrData[i].fJerk = stepData[i]["fJerk"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_fJerk)] = true;
								}
								else if (strDataName1 =="fDwellTime")
								{
									arrData[i].fDwellTime = stepData[i]["fDwellTime"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_fDwellTime)] = true;
								}
								else if (strDataName1 =="nIOSlaveNo")
								{
									arrData[i].nIOSlaveNo = stepData[i]["nIOSlaveNo"].asUInt();
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_nIOSlaveNo)] = true;
								}
								else if (strDataName1 == "nIOBitNo")
								{
									arrData[i].nIOBitNo = stepData[i]["nIOBitNo"].asBool();
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_nIOBitNo)] = true;
								}
								else if (strDataName1 == "bIOBitSet")
								{
									if (stepData[i]["bIOBitSet"].asUInt() == 1)
									{
										arrData[i].bIOBitSet = true;
									}
									else
									{
										arrData[i].bIOBitSet = false;
									}

									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_bIOBitSet)] = true;
								}
								else if (strDataName1 == "nBufferMode")
								{
									arrData[i].nBufferMode = (MXP_EasyClass::MXP_BUFFERMODE_ENUM)(stepData[i]["nBufferMode"].asUInt());
									bDataCheck[static_cast<int>(MXP_EasyClass::Axis_nBufferMode)] = true;
								}
								
							}
						}
						if (DataCheck->FCheckDataVaild(arrData[i-1], bDataCheck) == false)
						{
							strError = "Step " + intToString(i + 1) + " data is invalid";
							return false;
						}
						//strDataName = dataName.erase(strDataName);
					}
				}
				//strDataName = dataName.erase(strDataName);
			}
			return true;

		}
		catch (invalid_argument e)
		{
			strError = e.what();
			read.close();
			return FALSE;
		}
	}
	bool GroupSequenceFileLoad(const std::string strFilePath, unsigned int &nAxesGroup, std::vector<MXP_EasyClass::StGroupSequenceMove> &arrData, std::string &strError)
	{
		arrData = std::vector<MXP_EasyClass::StGroupSequenceMove>(1);
		try
		{
			string nReadStr = "";
			Json::Reader reader;
			Json::Value root;
			Json::Value sequenceMoveData;
			std::vector<bool> bDataCheck(static_cast<int>(MXP_EasyClass::Group_End));
			ifstream readFile(strFilePath, ifstream::binary);
			string strName, strName1;
			int i = 0;

			if (!readFile)
			{
				return false;
			}

			reader.parse(readFile,root);//json >> root; 두 구문 모두 json을 파싱하는데 사용할 수 있다.
			//전체 key()를 읽어보기
			sequenceMoveData = root["SequenceMove"];
			for (auto it = sequenceMoveData.begin(); it != sequenceMoveData.end(); it++)
			{
				auto dataName = sequenceMoveData.getMemberNames();
				for each (auto strDataName in dataName)
				{
					if (strDataName == "MoveType(0:Axis,1:Group)")
					{
						if(sequenceMoveData["MoveType(0:Axis,1:Group)"].asInt() != 1)
						{
							strError = "MoveType(0:Axis,1:Group) data is invalid";
							return false;
						}
						bDataCheck[static_cast<int>(MXP_EasyClass::Group_MoveType)] = true;
					}
					else if (strDataName == "Axis_GroupNo")
					{
						nAxesGroup = sequenceMoveData["Axis_GroupNo"].asInt();
						if(nAxesGroup <0 || nAxesGroup > 32)
						{
							strError = "Axis_GroupNo data is invalid";
							return false;
						}
						bDataCheck[static_cast<int>(MXP_EasyClass::Group_AxisGroupNo)] = true;
					}
					else if (strDataName == "StepData")
					{
						for (int nDataCheckCount = static_cast<int>(MXP_EasyClass::Group_AxisGroupNo) + 1; nDataCheckCount < static_cast<int>(MXP_EasyClass::Group_End); nDataCheckCount++)
						{
							bDataCheck[nDataCheckCount] = false;
						}
						auto  stepData = sequenceMoveData["StepData"];

						arrData.resize(stepData.size());
						for (i=0; i<(int)stepData.size(); i++)
						{
							//auto dataArray = stepData[i];
							auto dataName1 = stepData[i].getMemberNames();
							for each (auto strDataName1 in dataName1)
							{
								if (strDataName1 == "strStepName")
									arrData[i].strStepName = stepData[i]["strStepName"].asString();
								else if (strDataName1 == "nCmdType")
								{
									arrData[i].nCmdType = (MXP_EasyClass::ESequenceCmdType)(stepData[i]["nCmdType"].asInt());
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_nCmdType)] = true;
								}
								else if (strDataName1 == "fXPos")
								{
									arrData[i].fXPos = stepData[i]["fXPos"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strDataName1 == "fYPos")
								{
									arrData[i].fYPos = stepData[i]["fYPos"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strDataName1 == "fZPos")
								{
									arrData[i].fZPos = stepData[i]["fZPos"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strDataName1 == "fUPos")
								{
									arrData[i].fUPos = stepData[i]["fUPos"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strDataName1 == "fVPos")
								{
									arrData[i].fVPos = stepData[i]["fVPos"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strDataName1 == "fWPos")
								{
									arrData[i].fWPos = stepData[i]["fWPos"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strDataName1 == "fAPos")
								{
									arrData[i].fAPos = stepData[i]["fAPos"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strDataName1 == "fBPos")
								{
									arrData[i].fBPos = stepData[i]["fBPos"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strDataName1 == "fCPos")
								{
									arrData[i].fCPos = stepData[i]["fCPos"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fPos)] = true;
								}
								else if (strDataName1 == "fVel")
								{
									arrData[i].fVel = stepData[i]["fVel"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fVel)] = true;
								}
								else if (strDataName1 == "fAcc")
								{
									arrData[i].fAcc = stepData[i]["fAcc"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fAcc)] = true;
								}
								else if (strDataName1 == "fDec")
								{
									arrData[i].fDec = stepData[i]["fDec"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fDec)] = true;
								}
								else if (strDataName1 == "fJerk")
								{
									arrData[i].fJerk = stepData[i]["fJerk"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fJerk)] = true;
								}
								else if (strDataName1 == "fDwellTime")
								{
									arrData[i].fDwellTime = stepData[i]["fDwellTime"].asFloat();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_fDwellTime)] = true;
								}
								else if (strDataName1 == "nIOSlaveNo")
								{
									arrData[i].nIOSlaveNo = stepData[i]["nIOSlaveNo"].asUInt();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_nIOSlaveNo)] = true;
								}
								else if (strDataName1 == "nIOBitNo")
								{
									arrData[i].nIOBitNo = stepData[i]["nIOBitNo"].asUInt();
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_nIOBitNo)] = true;
								}
								else if (strDataName1 == "bIOBitSet")
								{
									if (stepData[i]["bIOBitSet"].asBool() == 1)
									{
										arrData[i].bIOBitSet = true;
									}
									else
									{
										arrData[i].bIOBitSet = false;
									}

									bDataCheck[static_cast<int>(MXP_EasyClass::Group_bIOBitSet)] = true;
								}
								else if (strDataName1 == "nBufferMode")
								{
									arrData[i].nBufferMode = (MXP_EasyClass::MXP_BUFFERMODE_ENUM)(stepData[i]["nBufferMode"].asInt());
									bDataCheck[static_cast<int>(MXP_EasyClass::Group_nBufferMode)] = true;
								}

							}
							//i++;
						}
						if (DataCheck->FCheckGroupDataVaild(arrData[i-1], bDataCheck) == false)
						{
							strError = "Step " + intToString(i + 1) + " data is invalid";
							return false;
						}
					}
				}

			}
			return true;

		}
		catch (invalid_argument e)
		{
			strError = e.what();
			read.close();
			return FALSE;
		}
	}
};
#pragma endregion file 처리

#pragma region
MXP_EasyClass::strIndexInfo arrMotionIndexInfo[MXP_EasyClass::MC_USEREND_INDEX - MXP_EasyClass::MC_USERSTART_INDEX];
MXP_EasyClass::strIndexInfo arrIOIndexInfo[MXP_EasyClass::MC_IO_INDEX_END - MXP_EasyClass::MC_IO_INDEX_START];

void MXP_EasyClass::ClearAllIndex()
{
	INT32 nIndexCount;

	for (nIndexCount = 0; nIndexCount < sizeof(arrMotionIndexInfo)/sizeof(*arrMotionIndexInfo); nIndexCount++)
	{
		arrMotionIndexInfo[nIndexCount].bUsing = FALSE;
		arrMotionIndexInfo[nIndexCount].nSlaveNo = 0;
		arrMotionIndexInfo[nIndexCount].nCmdType = (MXP_EasyClass::ECmdType)0;
		arrMotionIndexInfo[nIndexCount].nSubInfo = 0;
	}

	for (nIndexCount = 0; nIndexCount <sizeof(arrIOIndexInfo)/sizeof(*arrIOIndexInfo) ; nIndexCount++)
	{
		arrIOIndexInfo[nIndexCount].bUsing = FALSE;
		arrIOIndexInfo[nIndexCount].nSlaveNo = 0;
		arrIOIndexInfo[nIndexCount].nCmdType = (MXP_EasyClass::ECmdType)0;
	}

	FBInfoReset(false); // FB 정보 Reset
}

void MXP_EasyClass::ClearIndex(UINT32 nIndexCount)
{
	INT32 nClearCount = nIndexCount - MC_USERSTART_INDEX;
	arrMotionIndexInfo[nClearCount].bUsing = FALSE;
	arrMotionIndexInfo[nClearCount].nSlaveNo = 0;
	arrMotionIndexInfo[nClearCount].nCmdType = (ECmdType)0;
	arrMotionIndexInfo[nClearCount].nSubInfo = 0;
}

UINT32 MXP_EasyClass::GetMotionIndex(UINT32 nAxisNo, ECmdType nCmdType)
{
	INT32 nIndexCount;

	// Index UsingCheck
	for (nIndexCount =0; nIndexCount < sizeof(arrMotionIndexInfo)/sizeof(*arrMotionIndexInfo); nIndexCount++)
	{
		if (arrMotionIndexInfo[nIndexCount].nSlaveNo == nAxisNo && arrMotionIndexInfo[nIndexCount].nCmdType == nCmdType && arrMotionIndexInfo[nIndexCount].bUsing == TRUE)
		{
			return nIndexCount + MC_USERSTART_INDEX;
		}
	}

	for (nIndexCount =0; nIndexCount < sizeof(arrMotionIndexInfo)/sizeof(*arrMotionIndexInfo); nIndexCount++)
	{
		if (arrMotionIndexInfo[nIndexCount].bUsing == FALSE)
		{
			arrMotionIndexInfo[nIndexCount].bUsing = TRUE;
			arrMotionIndexInfo[nIndexCount].nSlaveNo = nAxisNo;
			arrMotionIndexInfo[nIndexCount].nCmdType = nCmdType;
			return nIndexCount + MC_USERSTART_INDEX;
		}
	}
	return 0;
}

UINT32 MXP_EasyClass::GetMotionIndex(UINT32 nAxisNo, ECmdType nCmdType, INT32 nSubInfo)
{
	INT32 nIndexCount;

	// Index UsingCheck
	for (nIndexCount =0; nIndexCount < sizeof(arrMotionIndexInfo)/sizeof(*arrMotionIndexInfo); nIndexCount++)
	{
		if (arrMotionIndexInfo[nIndexCount].nSlaveNo == nAxisNo && arrMotionIndexInfo[nIndexCount].nCmdType == nCmdType && arrMotionIndexInfo[nIndexCount].nSubInfo == nSubInfo && arrMotionIndexInfo[nIndexCount].bUsing == TRUE)
		{
			return nIndexCount + MC_USERSTART_INDEX;
		}
	}

	for (nIndexCount =0; nIndexCount < sizeof(arrMotionIndexInfo)/sizeof(*arrMotionIndexInfo); nIndexCount++)
	{
		if (arrMotionIndexInfo[nIndexCount].bUsing == FALSE)
		{
			arrMotionIndexInfo[nIndexCount].bUsing = TRUE;
			arrMotionIndexInfo[nIndexCount].nSlaveNo = nAxisNo;
			arrMotionIndexInfo[nIndexCount].nCmdType = nCmdType;
			arrMotionIndexInfo[nIndexCount].nSubInfo = nSubInfo;
			return nIndexCount + MC_USERSTART_INDEX;
		}
	}
	return 0;
}

UINT32 MXP_EasyClass::GetMotionIndexCheck(UINT32 nAxisNo, ECmdType nCmdType)
{
	INT32 nIndexCount;

	for (nIndexCount =0; nIndexCount < sizeof(arrMotionIndexInfo)/sizeof(*arrMotionIndexInfo); nIndexCount++)
	{
		if (arrMotionIndexInfo[nIndexCount].nSlaveNo == nAxisNo && arrMotionIndexInfo[nIndexCount].nCmdType == nCmdType && arrMotionIndexInfo[nIndexCount].bUsing)
		{
			return nIndexCount + MC_USERSTART_INDEX;
		}
	}

	return 0;

}

UINT32 MXP_EasyClass::GetMotionIndexCheck(UINT32 nAxisNo, ECmdType nCmdType, INT32 nSubInfo)
{
	INT32 nIndexCount;

	for (nIndexCount =0; nIndexCount < sizeof(arrMotionIndexInfo[nIndexCount]); nIndexCount++)
	{
		if (arrMotionIndexInfo[nIndexCount].nSlaveNo == nAxisNo && arrMotionIndexInfo[nIndexCount].nCmdType == nCmdType && arrMotionIndexInfo[nIndexCount].bUsing && arrMotionIndexInfo[nIndexCount].nSubInfo == nSubInfo)
		{
			return nIndexCount + MC_USERSTART_INDEX;
		}
	}

	return 0;

}

UINT32 MXP_EasyClass::GetIOIndex(UINT32 nSlave)
{
	INT32 nIndexCount;

	for (nIndexCount =0; nIndexCount < sizeof(arrIOIndexInfo[nIndexCount]); nIndexCount++)
	{
		if (arrIOIndexInfo[nIndexCount].nSlaveNo == nSlave && arrIOIndexInfo[nIndexCount].bUsing == TRUE)
		{
			return nIndexCount + MC_IO_INDEX_START;
		}
	}

	for (nIndexCount =0; nIndexCount < sizeof(arrIOIndexInfo[nIndexCount]); nIndexCount++)
	{
		if (arrIOIndexInfo[nIndexCount].bUsing == FALSE)
		{
			arrIOIndexInfo[nIndexCount].bUsing = TRUE;
			arrIOIndexInfo[nIndexCount].nSlaveNo = nSlave;
			arrIOIndexInfo[nIndexCount].nCmdType = (ECmdType)0;
			return nIndexCount + MC_IO_INDEX_START;
		}
	}
	return 0;
}
#pragma endregion MotionIndex

#pragma region

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_Init()
{
	INT32 nResult = 0;
	MXP_EasyClass::KERNEL_STATUS stStatus;
	nResult = MXP_InitKernel((UINT32 &)stStatus);
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_Init_Developer()
{
	INT32 nResult = 0;
	KERNEL_STATUS nStatus;
	nResult = MXP_InitKernel_Developer((UINT32 &)nStatus);

	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_Run()
{
	INT32 nResult;
	ClearAllIndex();
	nResult = MXP_SystemRun();
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_Stop()
{
	INT32 nResult;
	nResult = MXP_SystemStop();
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_Destroy()
{
	try
	{
		INT32 nResult;
		nResult = MXP_Destroy(); 
		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}

}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_Destroy_Developer()
{
	try
	{
		INT32 nResult;
		nResult = MXP_Destroy_Developer(); 
		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_GetStatus(KERNEL_STATUS &stStatus)
{
	INT32 nResult;
	nResult = MXP_GetKernelStatus((INT32 &)stStatus);
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_CheckLicense(MXP_USERLib nFunction, bool &bStatus)
{
	try
	{
		UINT8 nReadStatus;

		UINT32 nResult;
		UINT16 nFeature = nFunction;
		nResult = MXP_CheckFeature(nFeature, nReadStatus);
		bStatus = nReadStatus == 1? TRUE : FALSE;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_GetMasterOnlineMode(MXP_ONLINE_STATE &stStatus)
{
	INT32 nResult;
	nResult = MXP_GetOnlineMode((UINT32 &)stStatus);
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_GetMasterSimulationMode(bool &bSimulation)
{
	UINT32 stStatus;
	INT32 nResult;
	nResult = MXP_SM_IsSimulationMode(stStatus);
	bSimulation = ( stStatus == 0 ? FALSE :TRUE);
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_SetMasterOnlineMode(EC_NETWORK_CMD nMode)
{
	INT32 nResult;
	nResult = MXP_ET_SetMasterOnlineMode((::EC_NETWORK_CMD)nMode);
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_SetSlaveOnlineMode(UINT32 nSlaveNo, EC_NETWORK_CMD nMode)
{
	INT32 nResult;
	nResult = MXP_ET_SetSlaveOnlineMode(nSlaveNo, (::EC_NETWORK_CMD)nMode);
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_Reset()
{
	INT32 nResult;
	ClearAllIndex();
	nResult = MXP_SystemReset();
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

#pragma endregion System

#pragma region 

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_GetSlaveNoFromNodeId(UINT32 nNodeID, UINT32 &nSlaveNo)
{
	INT32 nResult;
	nResult = MXP_GetSlaveNoFromNodeId(nNodeID, nSlaveNo);
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_GetAxisNoFromNodeId(UINT32 nNodeID, UINT32 &nAxisNo)
{
	INT32 nResult;
	nResult = MXP_GetAxisNoFromNodeId(nNodeID, nAxisNo);
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_GetSlaveCurrentState(UINT32 nSlaveNo, MXP_ONLINE_STATE &stStatus)
{
	INT32 nResult;
	nResult = MXP_IsSlaveOnline(nSlaveNo, (UINT32&)stStatus);
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_GetSlaveCount(UINT32 &nSlaveCount)
{
	INT32 nResult;
	nResult = MXP_GetSlaveCount(0,nSlaveCount);
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_GetAxisCount(UINT32 &nAxisCount)
{
	INT32 nResult;
	nResult = MXP_GetSlaveCount(1,nAxisCount);
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_GetSlaveType(UINT32 nSlaveNo, MXP_NODE_TYPE &nType)
{
	INT32 nResult;
	nResult = MXP_QueryNodeType(nSlaveNo, (UINT32&)nType);
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_GetMasterEtherCATLineStatus(bool &bConnectState)
{
	INT32 nResult;
	MXP_SYSINFO_OUT stIn;

	nResult = MXP_GetSystemInformation(stIn);

	if (stIn.EthecatLinkStatus.Master == 1)
		bConnectState =TRUE;
	else
		bConnectState = FALSE;

	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_GetSlaveEtherCATLineStatus(UINT32 nSlaveNo, PORT_STATE &stPortState)
{
	INT32 nResult;
	MXP_SYSINFO_OUT stIn;

	nResult = MXP_GetSystemInformation(stIn);

	INT32 nIndex;
	UINT8 arrBit[8];
	UINT8 nRemain = stIn.EthecatLinkStatus.Slaves[nSlaveNo];

	for (nIndex = 0; nIndex<8; nIndex++)
	{
		arrBit[nIndex] = nRemain  % 2;
		nRemain = nRemain / 2;
	}

	stPortState.Port1State = (MXP_EasyClass::MXP_PORT_STATE)(arrBit[0] + arrBit[1] * 2);
	stPortState.Port2State = (MXP_EasyClass::MXP_PORT_STATE)(arrBit[2] + arrBit[3] * 2);
	stPortState.Port3State = (MXP_EasyClass::MXP_PORT_STATE)(arrBit[4] + arrBit[5] * 2);
	stPortState.Port4State = (MXP_EasyClass::MXP_PORT_STATE)(arrBit[6] + arrBit[7] * 2);

	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_GetEtherCATHistoryAlarm(INT32 *arrAlarm)
{
	INT32 nResult;
	MXP_SYSINFO_OUT stIn;
	//nAlarmArray = new INT32[20];
	nResult = MXP_GetSystemInformation(stIn);
	arrAlarm = stIn.Alarm.History;
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_GetSlaveName(UINT32 nSlaveNo, char *strName)
{
	INT32 nResult;

	WCHAR arrName[200] = {0,};
	nResult = MXP_GetSlaveName(nSlaveNo, arrName);
	WideCharToMultiByte(CP_ACP, 0, arrName, -1, strName, sizeof(arrName), NULL, NULL);

	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_SlaveReadyCheck(UINT32 nSlaveNo, bool &bReady)
{
	INT32 nResult;
	KERNEL_STATUS nStatus;
	MXP_ONLINE_STATE nOnlineState;
	bReady = FALSE;

	nResult = SYS_GetStatus(nStatus);

	if (nStatus >= MXP_EasyClass::SYSTEM_RUN && nResult ==0)
	{
		nResult = ECAT_GetMasterOnlineMode(nOnlineState);
		if (nOnlineState == MXP_EasyClass::ONLINE_STATE_OP && nResult == 0)
		{
			nResult = ECAT_GetSlaveCurrentState(nSlaveNo, nOnlineState);
			if (nOnlineState == MXP_EasyClass::ONLINE_STATE_OP && nResult == 0)
			{
				bReady = TRUE;
			}
		}
	}

	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_ReadyCheck(bool &bReady)
{
	INT32 nResult = 0;
	KERNEL_STATUS nStatus;
	MXP_ONLINE_STATE nOnlineState;
	UINT32 nSlaveCount;
	UINT32 nCount;

	bReady = FALSE;
	nResult = SYS_GetStatus(nStatus);
	if(nStatus >= MXP_EasyClass::SYSTEM_RUN && nResult ==0)
	{
		nResult = ECAT_GetMasterOnlineMode(nOnlineState);
		if(nOnlineState == MXP_EasyClass::ONLINE_STATE_OP && nResult ==0)
		{
			nResult = ECAT_GetSlaveCount(nSlaveCount);
			for (nCount = 0; nCount<nSlaveCount; nCount++)
			{
				nResult = ECAT_GetSlaveCurrentState(nCount, nOnlineState);
				if (nOnlineState != MXP_EasyClass::ONLINE_STATE_OP || nResult !=0)
				{
					bReady = FALSE;
					return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
				}
			}
			bReady = TRUE;
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		}
	}
	bReady = FALSE;
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}
MXP_EasyClass::MXP_FUNCTION_STATUS   MXP_EasyClass::ECAT_PDOMappingCheck(UINT32 nAxisNo, MXP_PDO_DIRECTION nDirection, UINT16 nPDOIndex, bool &bCheckState)
{
	INT32 nResult;
	UINT8 nPDODirection = nDirection;
	BOOL bState;
	nResult = MXP_PDO_MappingCheck(nAxisNo, nPDODirection, nPDOIndex, bState);
	bCheckState = bState == 1 ? TRUE : FALSE;

	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}
#pragma endregion Slave

#pragma region 

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::IO_Write(UINT32 nSlaveNo, UINT16 nOffset, UINT16 nSize, UINT8 * arrData)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::GetIOIndex(nSlaveNo);
		MXP_WRITEOUTPUTS_IN stIn;

		UINT8 arrArray[100] = {0,};
		UINT16 nIOSize;
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		nResult = MC_IO_Out_Read(nSlaveNo, nIOSize, arrArray);

		if (nResult !=  0)
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		if (nIOSize < nOffset + nSize)
			return MXP_EasyClass::RET_ERROR_INVAILD_IOSize;

		memmove(&arrData[nOffset], &arrArray[0], nSize); // (복사되는 메모리의 첫번째 주소, 복사할 메모리의 첫번째 주소, 복사할 크기)
		stIn.Output.SourceNo = (UINT16)nSlaveNo;
		stIn.Size = nIOSize;

		stIn.Execute = 0;

		nResult = (MXP_EasyClass::MXP_FUNCTION_STATUS)MXP_WriteOutputsCmd(nMotionIndex, &stIn, &arrArray[0]);

		stIn.Execute = 1;
		nResult = (MXP_EasyClass::MXP_FUNCTION_STATUS)MXP_WriteOutputsCmd(nMotionIndex, &stIn, &arrArray[0]);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;

	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::IO_WriteBit(UINT32 nSlaveNo, UINT16 nOffset, UINT8 nBitOffset, bool bData)
{
	try
	{
		UINT32 nMotionIndex = GetIOIndex(nSlaveNo);
		MXP_WRITEDIGITALOUTPUT_IN stIn;

		INT32 nResult;
		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.Output.SourceNo = nSlaveNo;
		stIn.OutputNumber = nOffset * 8 + nBitOffset;
		stIn.Value = ( bData == TRUE ? 1 : 0);

		stIn.Execute = 0;
		nResult = MXP_WriteDigitalOutputCmd(nMotionIndex, &stIn);

		stIn.Execute = 1;
		nResult = MXP_WriteDigitalOutputCmd(nMotionIndex, &stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;

	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::IO_WriteByte(UINT32 nSlaveNo, UINT16 nOffset, UINT8 nData)
{
	try
	{
		UINT32 nMotionIndex = GetIOIndex(nSlaveNo);
		MXP_WRITEOUTPUTS_IN stIn;
		UINT8 arrArray[100];
		UINT16 nIOSize;
		INT32 nResult;

		if (nMotionIndex = 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		nResult = MC_IO_Out_Read(nSlaveNo, nIOSize, arrArray);

		if (nResult != 0)
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		if (nIOSize < nOffset)
			return MXP_EasyClass::RET_ERROR_INVAILD_IOSize;

		arrArray[nOffset] = nData;

		stIn.Output.SourceNo = nSlaveNo;
		stIn.Size = nIOSize;

		stIn.Execute = 0;

		nResult = MXP_WriteOutputsCmd(nMotionIndex, &stIn, &arrArray[0]);

		stIn.Execute = 1;
		nResult = MXP_WriteOutputsCmd(nMotionIndex, &stIn, &arrArray[0]);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;

	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::IO_WriteWord(UINT32 nSlaveNo, UINT16 nOffset, UINT16 nData)
{
	try
	{
		UINT32 nMotionIndex = GetIOIndex(nSlaveNo);
		MXP_WRITEOUTPUTS_IN stIn;

		UINT8 arrArray[100];
		UINT16 nIOSize;
		INT32 nResult;
		UINT8 arrData[sizeof(UINT8)];
		if (nMotionIndex = 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		memcpy(arrData, &nData, sizeof(UINT8));

		nResult = MC_IO_Out_Read(nSlaveNo, nIOSize, arrArray);

		if (nResult != 0)
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		if (nIOSize < nOffset + 2)
			return MXP_EasyClass::RET_ERROR_INVAILD_IOSize;

		memmove(&arrArray[nOffset],&arrData[0], sizeof(arrData)/sizeof(*arrData) ); // (복사되는 메모리의 첫번째 주소, 복사할 메모리의 첫번째 주소, 복사할 크기)

		stIn.Output.SourceNo = nSlaveNo;
		stIn.Size = nIOSize;

		stIn.Execute = 0;
		nResult = MXP_WriteOutputsCmd(nMotionIndex, &stIn, &arrArray[0]);

		stIn.Execute = 1;
		nResult = MXP_WriteOutputsCmd(nMotionIndex, &stIn, &arrArray[0]);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;

	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}


MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::IO_WriteDword(UINT32 nSlaveNo, UINT16 nOffset, UINT32 nData)
{
	try
	{
		UINT32 nMotionIndex = GetIOIndex(nSlaveNo);
		MXP_WRITEOUTPUTS_IN stIn;

		UINT8 arrArray[100];
		UINT16 nIOSize;
		INT32 nResult;
		UINT8 arrData[sizeof(UINT8)];

		if (nMotionIndex = 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		memcpy(arrData, &nData, sizeof(UINT8));

		nResult = MC_IO_Out_Read(nSlaveNo, nIOSize, arrArray);

		if (nResult != 0)
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		if (nIOSize < nOffset + 4)
			return MXP_EasyClass::RET_ERROR_INVAILD_IOSize;
		memmove(&arrArray[nOffset], &arrData[0], sizeof(arrData)/sizeof(*arrData) ); // (복사되는 메모리의 첫번째 주소, 복사할 메모리의 첫번째 주소, 복사할 크기)

		stIn.Output.SourceNo = nSlaveNo;
		stIn.Size = nIOSize;

		stIn.Execute = 0;
		nResult = MXP_WriteOutputsCmd(nMotionIndex, &stIn, &arrArray[0]);

		stIn.Execute = 1;
		nResult = MXP_WriteOutputsCmd(nMotionIndex, &stIn, &arrArray[0]);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;

	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}


MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::IO_Read(UINT32 nSlaveNo, MXP_IO_Type nIoType, UINT16 nOffset, UINT16 nSize, UINT8 * arrData)
{
	try
	{
		INT32 nResult;
		UINT8 arrArray[10] = {0, };
		UINT32 nIOSize = 0;

		if (nIoType == MXP_EasyClass::IO_In)
		{
			nResult = MC_IO_In_Read(nSlaveNo, (UINT16&)nIOSize, arrArray);
			if (nResult !=0)
				return (MXP_FUNCTION_STATUS)nResult;
			if (nIOSize < UINT32(nOffset + nSize))
				return MXP_EasyClass::RET_ERROR_INVAILD_IOSize;

			memcpy(&arrData[0], &arrArray[nOffset], (UINT8)nSize);
		} 
		else
		{
			nResult = MC_IO_Out_Read(nSlaveNo, (UINT16&)nIOSize, arrArray);
			if (nResult !=0)
				return (MXP_FUNCTION_STATUS)nResult;
			if (nIOSize < UINT32(nOffset + nSize))
				return MXP_EasyClass::RET_ERROR_INVAILD_IOSize;

			memcpy(&arrData[0], &arrArray[nOffset], (UINT8)nSize);
		}
		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}


MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::IO_ReadBit(UINT32 nSlaveNo, MXP_IO_Type nIoType, UINT16 nOffset, UINT8 nBitOffset, bool &bData)
{
	try
	{
		INT32 nResult;
		UINT32 nIOCount = nOffset * 8 + nBitOffset;

		if (nIoType == MXP_EasyClass::IO_In)
			nResult = MC_IO_In_Read_BIT(nSlaveNo, nIOCount, bData);
		else
			nResult = MC_IO_Out_Read_BIT(nSlaveNo, nIOCount, bData);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::IO_ReadByte(UINT32 nSlaveNo, MXP_IO_Type nIoType, UINT16 nOffset, UINT8  &nData)
{
	try
	{
		INT32 nResult;
		UINT8 arrArray[10] = {0, };
		UINT32 nIOSize;

		if (nIoType == MXP_EasyClass::IO_In)
		{
			nResult = MC_IO_In_Read(nSlaveNo, (UINT16&)nIOSize, arrArray);
			if (nResult !=0)
				return (MXP_FUNCTION_STATUS)nResult;
			if (nIOSize < nOffset)
				return MXP_EasyClass::RET_ERROR_INVAILD_IOSize;

			nData = arrArray[nOffset];
		} 
		else
		{
			nResult = MC_IO_Out_Read(nSlaveNo, (UINT16&)nIOSize, arrArray);
			if (nResult !=0)
				return (MXP_FUNCTION_STATUS)nResult;
			if (nIOSize < nOffset)
				return MXP_EasyClass::RET_ERROR_INVAILD_IOSize;

			nData = arrArray[nOffset];
		}

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}



MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::IO_ReadWord(UINT32 nSlaveNo, MXP_IO_Type nIoType, UINT16 nOffset, UINT16 &nData)
{
	try
	{
		INT32 nResult;
		UINT8 arrArray[10] = {0, };
		UINT32 nIOSize;

		if (nIoType == MXP_EasyClass::IO_In)
		{
			nResult = MC_IO_In_Read(nSlaveNo, (UINT16&)nIOSize, arrArray);
			if (nResult !=0)
				return (MXP_FUNCTION_STATUS)nResult;
			if (nIOSize < UINT32(nOffset + 2))
				return MXP_EasyClass::RET_ERROR_INVAILD_IOSize;

			memcpy(&nData, &arrArray[nOffset], sizeof(arrArray));

		} 
		else
		{
			nResult = MC_IO_Out_Read(nSlaveNo, (UINT16&)nIOSize, arrArray);
			if (nResult !=0)
				return (MXP_FUNCTION_STATUS)nResult;
			if (nIOSize < UINT32(nOffset + 2))
				return MXP_EasyClass::RET_ERROR_INVAILD_IOSize;

			memcpy(&nData, &arrArray[nOffset], sizeof(arrArray));
		}

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::IO_ReadDWord(UINT32 nSlaveNo, MXP_IO_Type nIoType, UINT16 nOffset, UINT32 &nData)
{
	try
	{
		INT32 nResult;
		UINT8 arrArray[10] = {0, };
		UINT32 nIOSize;

		if (nIoType == MXP_EasyClass::IO_In)
		{
			nResult = MC_IO_In_Read(nSlaveNo, (UINT16&)nIOSize, arrArray);
			if (nResult !=0)
				return (MXP_FUNCTION_STATUS)nResult;
			if (nIOSize < UINT32(nOffset + 4))
				return MXP_EasyClass::RET_ERROR_INVAILD_IOSize;

			memcpy(&nData, &arrArray[nOffset], sizeof(arrArray));

		} 
		else
		{
			nResult = MC_IO_Out_Read(nSlaveNo, (UINT16&)nIOSize, arrArray);
			if (nResult !=0)
				return (MXP_FUNCTION_STATUS)nResult;
			if (nIOSize < UINT32(nOffset + 4))
				return MXP_EasyClass::RET_ERROR_INVAILD_IOSize;

			memcpy(&nData, &arrArray[nOffset], sizeof(arrArray));
		}

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::MC_IO_Out_Read(UINT32 nSlaveNo, UINT16 &nSize, UINT8 * arrData)
{
	MXP_READOUTPUTS_IN stIn;
	MXP_READOUTPUTS_OUT stOut;
	INT32 nResult;

	stIn.Output.SourceNo = nSlaveNo;
	stIn.Enable = 1;

	nResult = MXP_ReadOutputs(&stIn, stOut, &arrData[0]);
	nSize = stOut.Size;
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::MC_IO_Out_Read_BIT(UINT32 nSlaveNo, UINT16 nBitNo, bool &bData)
{
	MXP_READDIGITALOUTPUT_IN stIn;
	MXP_READDIGITALOUTPUT_OUT stOut;
	INT32 nResult;

	stIn.Output.SourceNo = nSlaveNo;
	stIn.OutputNumber = nBitNo;
	stIn.Enable = 1;

	nResult = MXP_ReadDigitalOutput(&stIn, stOut);
	bData = (stOut.Value == 1? true : false );  

	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::MC_IO_In_Read(UINT32 nSlaveNo, UINT16 &nSize, UINT8 * arrData)
{
	MXP_READINPUTS_IN stIn;
	MXP_READINPUTS_OUT stOut;
	INT32 nResult;

	stIn.Input.SourceNo = nSlaveNo;
	stIn.Enable = 1;

	nResult = MXP_ReadInputs(&stIn, stOut, &arrData[0]);
	nSize = stOut.Size;
	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}


MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::MC_IO_In_Read_BIT(UINT32 nSlaveNo, UINT16 nBitNo, bool &bData)
{
	MXP_READDIGITALINPUT_IN stIn;
	MXP_READDIGITALINPUT_OUT stOut;
	INT32 nResult;

	stIn.Input.SourceNo = nSlaveNo;
	stIn.InputNumber = nBitNo;
	stIn.Enable = 1;

	nResult = MXP_ReadDigitalInput(&stIn, stOut);

	bData = (stOut.Value == 1? true : false ); 


	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
}

#pragma endregion IO

#pragma region 
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_Power(UINT32 nAxisNo, bool bEnable)
{
	try
	{
		UINT32 nMotionIndex = MC_POWER_INDEX + nAxisNo;
		MXP_POWER_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Enable = ( bEnable == true ? true : false ); 

		nResult = MXP_PowerCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_Reset(UINT32 nAxisNo)
{
	try
	{
		UINT32 nMotionIndex = MC_RESETHOME_INDEX + nAxisNo;
		MXP_RESET_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;

		stIn.Execute = 0;
		nResult = MXP_ResetCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_ResetCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_Home(UINT32 nAxisNo)
{
	try
	{
		UINT32 nMotionIndex = MC_RESETHOME_INDEX + nAxisNo;
		MXP_HOME_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)0;
		stIn.Position =0;
		stIn.Execute = 0;
		nResult = MXP_HomeCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_HomeCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_SetPosition(UINT32 nAxisNo, float fPosition)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_SETPOSITION_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Position = fPosition;

		stIn.Execute = 0;
		nResult = MXP_SetPositionCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_SetPositionCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_Halt(UINT32 nAxisNo, float fDec, float fJerk)	
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_HALT_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Deceleration = fDec;
		stIn.Jerk = fJerk;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_ABORTING;
		stIn.Execute = 0;
		nResult = MXP_HaltCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_HaltCmd(nMotionIndex, &stIn);

		mAxisSequenceData[nAxisNo].bRunFlag = false;

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_Stop(UINT32 nAxisNo, bool bExecute, float fDec, float fJerk)
{
	try
	{

		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_STOP_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Deceleration = fDec;
		stIn.Jerk = fJerk;
		if (bExecute ==TRUE)
		{
			stIn.Execute = 1;
			mAxisSequenceData[nAxisNo].bRunFlag = FALSE;
		}
		else
			stIn.Execute = 0;

		nResult = MXP_StopCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_MoveAbsolute(UINT32 nAxisNo, float fPosition, float fVel, float fAcc,
																  float fDec, float fJerk, MXP_DIRECTION_ENUM nDirection,
																  MXP_BUFFERMODE_ENUM nBufferMode)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_MOVEABSOLUTE_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.ContinuousUpdate = 0;
		stIn.Position = fPosition;
		stIn.Velocity = fVel;
		stIn.Acceleration = fAcc;
		stIn.Deceleration = fDec;
		stIn.Jerk = fJerk;
		stIn.Direction = (::MXP_DIRECTION_ENUM)nDirection;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;

		stIn.Execute = 0;
		nResult = MXP_MoveAbsoluteCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_MoveAbsoluteCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_MoveAbsolute_VelocityOverride(UINT32 nAxisNo, FLOAT fTargetPosition, FLOAT fTargetVel, 
	                                                                               INT16 nOverrideCount, FLOAT *fOverridePosition, FLOAT *fOverrideVelocity, 
																				   FLOAT fAcc, FLOAT fDec, FLOAT fJerk, MXP_BUFFERMODE_ENUM nBufferMode)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_MOVEABSOLUTE_IN stIn;
		FLOAT *fPosCal = new FLOAT[nOverrideCount];

		INT32 nResult;
		int nCount;

		stIn.Axis.AxisNo = nAxisNo;
		
		stIn.Acceleration = fAcc;
		stIn.Deceleration = fDec;
		stIn.Jerk = fJerk;
		stIn.Direction = (::MXP_DIRECTION_ENUM)MXP_EasyClass::MXP_NONE_DIRECTION;

		if(sizeof(fOverridePosition) != nOverrideCount || sizeof(fOverrideVelocity) != nOverrideCount)
			return MXP_EasyClass::RET_Move_Absolute_VelocityOverride_PARAMETER_INVAILD;

				for (nCount = 0; nCount <= nOverrideCount; nCount++)
		{
			if(nCount == 0)
			{
				stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_EasyClass::MXP_BUFFERED;
				stIn.Position = fOverridePosition[0];
				stIn.Velocity = fTargetVel;
			}
			else if (nCount == nOverrideCount)
			{
				stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;
				stIn.Position = fTargetPosition;
				stIn.Velocity = fOverrideVelocity[nCount-1];
			}
			else
			{
				stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;
				stIn.Position = fOverridePosition[nCount];
				stIn.Velocity = fOverrideVelocity[nCount-1];
			}

			stIn.Execute = 0;
			nResult = MXP_MoveAbsoluteCmd(nMotionIndex, &stIn);
			stIn.Execute = 1;
			nResult = MXP_MoveAbsoluteCmd(nMotionIndex, &stIn);
		}
		
		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_MoveRelative(UINT32 nAxisNo, float fDistance, float fVel, float fAcc, 
																  float fDec, float fJerk, MXP_BUFFERMODE_ENUM nBufferMode)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_MOVERELATIVE_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.ContinuousUpdate = 0;
		stIn.Distance = fDistance;
		stIn.Velocity = fVel;
		stIn.Acceleration = fAcc;
		stIn.Deceleration = fDec;
		stIn.Jerk = fJerk;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;

		stIn.Execute = 0;
		nResult = MXP_MoveRelativeCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_MoveRelativeCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_MoveVelocity(UINT32 nAxisNo, float fVel, float fAcc, float fDec, float fJerk,
																  MXP_DIRECTION_ENUM nDirection, MXP_BUFFERMODE_ENUM nBufferMode)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_MOVEVELOCITY_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Velocity = fVel;
		stIn.Acceleration = fAcc;
		stIn.Deceleration = fDec;
		stIn.Jerk = fJerk;
		stIn.Direction = (::MXP_DIRECTION_ENUM)nDirection;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;

		stIn.Execute = 0;
		nResult = MXP_MoveVelocityCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_MoveVelocityCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_SetOverride(UINT32 nAxisNo, float fVelFactor)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_SETOVERRIDE_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.VelFactor = fVelFactor;

		stIn.Enable = 0;
		nResult = MXP_SetOverrideCmd(nMotionIndex, &stIn);
		stIn.Enable = 1;
		nResult = MXP_SetOverrideCmd(nMotionIndex, &stIn);

		mExAPIAxisParam[nAxisNo].fOverrideSetValue = fVelFactor; // velFactor 저장 

		if (mAxisSequenceData[nAxisNo].bRunFlag && mFeedbackSensorParam[nAxisNo].bUsing) // 센서 사용 조건인 경우
		{
			if (fVelFactor == 0)
				AX_FeedBackSensorOverrideInVaildFlag(nAxisNo, 1);
			else
				AX_FeedBackSensorOverrideInVaildFlag(nAxisNo, 0);
		}
		else
			AX_FeedBackSensorOverrideInVaildFlag(nAxisNo, 0);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

float MXP_EasyClass::AX_ReadOverride(UINT32 nAxisNo)
{
	return mExAPIAxisParam[nAxisNo].fOverrideSetValue;
}


MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_BufferedIO(UINT32 nAxisNo, UINT16 nSlaveNo, UINT16 nBitPos, bool bBitValue)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_BUFFEREDDIGITALIO_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_EasyClass::MXP_BUFFERED;
		stIn.SlaveNo = nSlaveNo;
		stIn.BitPosition = nBitPos;
		stIn.BitValue = (bBitValue == TRUE)? 1 : 0;

		stIn.Execute = 0;
		nResult = MXP_BufferedDigitalioCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_BufferedDigitalioCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_Dwell(UINT32 nAxisNo, float fTime)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_DWELL_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_EasyClass::MXP_BUFFERED;
		stIn.TimeValue = fTime;

		stIn.Execute = 0;
		nResult = MXP_DwellCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_DwellCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

#pragma endregion Single Motion

#pragma region
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_GearIn(UINT32 nMasterAxis, UINT32 nSlaveAxis, INT32 nRationNumerator,UINT32 nRatioDenominator,
															MXP_SOURCE_ENUM nMasterValueSource, float	fAcc, float fDec, float fJerk)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nSlaveAxis, MXP_EasyClass::e_GearIn);
		MXP_GEARIN_IN stIn;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.Master.AxisNo = nMasterAxis;
		stIn.Slave.AxisNo = nSlaveAxis;
		stIn.RatioNumerator = nRationNumerator;
		stIn.RatioDenominator = nRatioDenominator;
		stIn.MasterValueSource = (::MXP_SOURCE_ENUM)nMasterValueSource;
		stIn.Acceleration = fAcc;
		stIn.Deceleration = fDec;
		stIn.Jerk = fJerk;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_ABORTING;
		stIn.ContinuousUpdate = 0;

		stIn.Execute = 0;
		nResult = MXP_GearInCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_GearInCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_GearInMonitor(UINT32 nSlaveAxis, PROCESS_CHECK &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndexCheck(nSlaveAxis, MXP_EasyClass::e_GearIn);
		MXP_GEARIN_OUT stOut;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		nResult = MXP_GetGearInOutParam(nMotionIndex, stOut);

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Active == 1? TRUE : FALSE;
		stStatus.Done = stOut.InGear == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;


		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_GearOut(UINT32 nSlaveAxis)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndexCheck(nSlaveAxis, MXP_EasyClass::e_GearIn);
		MXP_GEAROUT_IN stIn;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		stIn.Slave.AxisNo = nSlaveAxis;

		stIn.Execute = 0;
		nResult = MXP_GearOutCmd(nMotionIndex, &stIn);

		stIn.Execute = 1;
		nResult = MXP_GearOutCmd(nMotionIndex, &stIn);

		ClearIndex(nMotionIndex);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
#pragma endregion GearIn

#pragma region
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_GearInPos(UINT32 nMasterAxis, UINT32 nSlaveAxis, INT32 nRatioNumerator, UINT32 nRatioDenominator,
															   MXP_EasyClass::MXP_SOURCE_ENUM nMasterValueSource, float fMasterSycPos, float fSlaveSycPos,
															   float fMasterStartDistance, float fVel, float fAcc, float fDec, float fJerk)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nSlaveAxis, MXP_EasyClass::e_GearInPos);
		MXP_GEARINPOS_IN stIn;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.Master.AxisNo = nMasterAxis;
		stIn.Slave.AxisNo = nSlaveAxis;
		stIn.RatioDenominator = nRatioDenominator;
		stIn.RatioNumerator = nRatioNumerator;
		stIn.MasterValueSource = (::MXP_SOURCE_ENUM)nMasterValueSource;
		stIn.MasterSyncPosition = fMasterSycPos;
		stIn.SlaveSyncPosition = fSlaveSycPos;
		stIn.MasterStartDistance = fMasterStartDistance;
		stIn.Velocity = fVel;
		stIn.Acceleration = fAcc;
		stIn.Deceleration = fDec;
		stIn.Jerk = fJerk;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_ABORTING;

		stIn.Execute = 0;
		nResult = MXP_GearInPosCmd(nMotionIndex, &stIn);

		stIn.Execute = 1;
		nResult = MXP_GearInPosCmd(nMotionIndex, &stIn);

		ClearIndex(nMotionIndex);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_GearInPosMonitor(UINT32 nSlaveAxis, PROCESS_CHECK &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndexCheck(nSlaveAxis, MXP_EasyClass::e_GearInPos);
		MXP_GEARINPOS_OUT stOut;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		nResult = MXP_GetGearInPosOutParam(nMotionIndex, stOut);

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Active == 1? TRUE : FALSE;
		stStatus.Done = stOut.InSync == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_GearInPosOut(UINT32 nSlaveAxis)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndexCheck(nSlaveAxis, MXP_EasyClass::e_GearInPos);
		MXP_GEAROUT_IN stIn;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		stIn.Slave.AxisNo = nSlaveAxis;

		stIn.Execute = 0;
		nResult = MXP_GearOutCmd(nMotionIndex, &stIn);

		stIn.Execute = 1;
		nResult = MXP_GearOutCmd(nMotionIndex, &stIn);

		ClearIndex(nMotionIndex);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
#pragma endregion GearPos

#pragma region
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamTableSelect(UINT32 nMasterAxis, UINT32 nSlaveAxis, UINT16 nCamTableID, bool bPeriodic)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nSlaveAxis, MXP_EasyClass::e_CAMTableOption);
		MXP_CAMTABLESELECT_IN stIn;
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.Master.AxisNo = nMasterAxis;
		stIn.Slave.AxisNo = nSlaveAxis;
		stIn.CamTable.CamTable = nCamTableID;
		stIn.Periodic = (bPeriodic == TRUE) ? 1:0 ;

		stIn.Execute = 1;
		nResult = MXP_CamTableSelectCmd(nMotionIndex, &stIn);

		stIn.Execute = 0;
		nResult = MXP_CamTableSelectCmd(nMotionIndex, &stIn);

		ClearIndex(nMotionIndex);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamWriteTableRequest(UINT16 nCamTableID, bool bExecute, UINT16 nTableRowCount, MXP_CAMTABLE_REF *arrCamData)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nCamTableID, MXP_EasyClass::e_CAMTableEdit);
		MXP_WRITECAMTABLE_IN stIn = {0, };
		INT32 nResult;
		::MXP_CAMTABLE_REF arrWriteData[400] = {0, };
		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		INT16 nArrayCount;
		for (nArrayCount =0; nArrayCount < nTableRowCount; nArrayCount++)
		{
			arrWriteData[nArrayCount] = arrCamData[nArrayCount].stData;	
		}

		stIn.CamTable.CamTable = nCamTableID;

		if (nTableRowCount < 100)

			stIn.DataSize = 100;
		else if (nTableRowCount < 200)
			stIn.DataSize = 200;
		else
			stIn.DataSize = 400;

		stIn.ExecutionMode = 2;
		memcpy(stIn.CamDataArray, arrWriteData, sizeof(arrWriteData));


		stIn.Execute = 0;
		nResult = MXP_WriteCamTableCmd(nMotionIndex, &stIn);

		if (bExecute == TRUE)
		{
			stIn.Execute = 1;
			nResult = MXP_WriteCamTableCmd(nMotionIndex, &stIn);
		}
		else
			ClearIndex(nMotionIndex);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamWriteTableReply(UINT16 nCamTableID, PROCESS_CHECK &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndexCheck(nCamTableID, MXP_EasyClass::e_CAMTableEdit);
		MXP_WRITECAMTABLE_OUT stOut = {0, };
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		nResult = MXP_GetWriteCamTableOutParam(nMotionIndex, stOut);


		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Done == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamWriteTable(UINT16 nCamTableID, UINT16 nTableRowCount, MXP_CAMTABLE_REF *arrCamData, 
																   INT32 nWaitTime, PROCESS_CHECK &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nCamTableID, MXP_EasyClass::e_CAMTableEdit);
		MXP_WRITECAMTABLE_IN stIn = {0,};
		MXP_WRITECAMTABLE_OUT stOut= {0,};
		INT32 nResult;
		MXP_CAMTABLE_REF arrWriteData[400]= {0,};

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		INT16 nArrayCount;

		for (nArrayCount = 0; nArrayCount < nTableRowCount; nArrayCount++)
		{
			arrWriteData[nArrayCount] = arrCamData[nArrayCount];
		}

		stIn.CamTable.CamTable = nCamTableID;
		if (nTableRowCount < 100)
		{
			stIn.DataSize = 100;
		} 
		else if(nTableRowCount <200)
		{
			stIn.DataSize = 200;
		}
		else
		{
			stIn.DataSize = 400;
		}

		stIn.ExecutionMode = 2;
		memcpy(stIn.CamDataArray, arrWriteData,  sizeof(arrWriteData));

		stIn.Execute = 0;
		nResult = MXP_WriteCamTableCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_WriteCamTableCmd(nMotionIndex, &stIn);

		if (nMotionIndex == 0)
		{
			ClearIndex(nMotionIndex);
			return (MXP_FUNCTION_STATUS)nResult;
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(nWaitTime));
		Sleep(nWaitTime);

		nResult = MXP_GetWriteCamTableOutParam(nMotionIndex, stOut);
		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Done == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;

		ClearIndex(nMotionIndex);
		return (MXP_FUNCTION_STATUS)nResult;
	}

	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}

}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamReadTableRequest(UINT16 nCamTableID, bool bExecute)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nCamTableID, MXP_EasyClass::e_CAMTableEdit);
		MXP_READCAMTABLE_IN stIn;
		INT32 nResult;


		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.CamTable.CamTable = nCamTableID;
		stIn.Execute = 0;
		nResult = MXP_ReadCamTableCmd(nMotionIndex, &stIn);

		if (bExecute == TRUE)
		{
			stIn.Execute = 1;
			nResult = MXP_ReadCamTableCmd(nMotionIndex, &stIn);
		}
		else
			ClearIndex(nMotionIndex);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamReadTableReply(UINT16 nCamTableID, READ_CAMTABLE_REPLY &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndexCheck(nCamTableID, MXP_EasyClass::e_CAMTableEdit);
		MXP_READCAMTABLE_OUT stOut;
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		nResult = MXP_GetReadCamTableOutParam(nMotionIndex, stOut);

		if (nResult != 0)
			return (MXP_FUNCTION_STATUS)nResult;


		stStatus.TableRowCount = stOut.DataSize;

		memcpy(stStatus.CamDataArray, stOut.CamDataArray, sizeof(stOut.CamDataArray));

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Done == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamReadTable(UINT16 nCamTableID, INT32 nWaitTime, READ_CAMTABLE_REPLY &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nCamTableID, MXP_EasyClass::e_CAMTableEdit);
		MXP_READCAMTABLE_IN stIn;
		MXP_READCAMTABLE_OUT stOut;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.CamTable.CamTable = nCamTableID;
		stIn.Execute = 0;
		nResult = MXP_ReadCamTableCmd(nMotionIndex, &stIn);

		stIn.Execute = 1;
		nResult = MXP_ReadCamTableCmd(nMotionIndex, &stIn);

		if (nResult != 0)
		{
			ClearIndex(nMotionIndex);
			return (MXP_FUNCTION_STATUS)nResult;
		}
		Sleep(nWaitTime);

		nResult = MXP_GetReadCamTableOutParam(nMotionIndex, stOut);
		if (nResult != 0)
		{
			ClearIndex(nMotionIndex);
			return (MXP_FUNCTION_STATUS)nResult;
		}

		stStatus.TableRowCount = stOut.DataSize;

		memcpy(stStatus.CamDataArray, stOut.CamDataArray,sizeof(stOut.CamDataArray));

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Done == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;

		ClearIndex(nMotionIndex);
		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamIn(UINT32 nMasterAxis, UINT32 nSlaveAxis, float fMasterOffset, float fSlaveOffset,
															float fMasterScaling, float fSlaveScaling, float fMasterSyncPosition, float fMasterStartDistance,
															MXP_STARTMODE_ENUM nStartMode, MXP_SOURCE_ENUM nMasterValSource, UINT16 nCamTableID)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nSlaveAxis, MXP_EasyClass::e_CAMIn);
		MXP_CAMIN_IN stIn;
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.Master.AxisNo = nMasterAxis;
		stIn.Slave.AxisNo = nSlaveAxis;
		stIn.MasterOffset = fMasterOffset;
		stIn.SlaveOffset = fSlaveOffset;
		stIn.MasterScaling = fMasterScaling;
		stIn.SlaveScaling = fSlaveScaling;
		stIn.MasterSyncPosition = fMasterSyncPosition;
		stIn.MasterStartDistance = fMasterStartDistance;
		stIn.StartMode = (::MXP_STARTMODE_ENUM)nStartMode;
		stIn.MasterValueSource = (::MXP_SOURCE_ENUM)nMasterValSource;
		stIn.CamTableID = nCamTableID;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_ABORTING; 

		stIn.Execute = 0;
		nResult = MXP_CamInCmd(nMotionIndex, &stIn);

		stIn.Execute = 1;
		nResult = MXP_CamInCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamIn_MasterIO(UINT32 nExternalENC_SlaveNo, UINT32 nExternalENC_SlavePos, UINT8 nExternalENC_SlaveSize,
																	UINT32 nExternalENC_Resolution, float fExternalENC_PulseToCamMasterUnit, UINT32 nSlaveAxis,
																	FLOAT fMasterOffset, FLOAT fSlaveOffset, FLOAT fMasterScaling, FLOAT fSlaveScaling, 
																	FLOAT fMasterSyncPosition, FLOAT fMasterStartDistance, MXP_STARTMODE_ENUM nStartMode, 
																	MXP_SOURCE_ENUM nMasterValSource, UINT16 nCamTableID)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nSlaveAxis, MXP_EasyClass::e_CAMIn);
		MXP_CAMIN_EX_IN stIn;
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.MasterType = MXP_EasyClass::MXP_CAM_MASTER_TYPE_EXTENAL_ENC;
		stIn.ExternalENCInfo.ExternalEncResolution = nExternalENC_Resolution;
		stIn.ExternalENCInfo.PulseToCAMMasterUnit = fExternalENC_PulseToCamMasterUnit;
		stIn.ExternalENCInfo.SlaveNo = nExternalENC_SlaveNo;
		stIn.ExternalENCInfo.SlavePos = nExternalENC_SlavePos;
		stIn.ExternalENCInfo.SlaveSize = nExternalENC_SlaveSize;

		stIn.Master.AxisNo = 0;
		stIn.Slave.AxisNo = nSlaveAxis;
		stIn.MasterOffset = fMasterOffset;
		stIn.SlaveOffset = fSlaveOffset;
		stIn.MasterScaling = fMasterScaling;
		stIn.SlaveScaling = fSlaveScaling;
		stIn.MasterSyncPosition = fMasterSyncPosition;
		stIn.MasterStartDistance = fMasterStartDistance;
		stIn.StartMode = (::MXP_STARTMODE_ENUM)nStartMode;
		stIn.MasterValueSource = (::MXP_SOURCE_ENUM)nMasterValSource;
		stIn.CamTableID = nCamTableID;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_ABORTING; 

		stIn.Execute = 0;
		nResult = MXP_CamInCmd_Ex(nMotionIndex, &stIn);

		stIn.Execute = 1;
		nResult = MXP_CamInCmd_Ex(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamInMonitor(UINT32 nSlaveAxis, PROCESS_CHECK &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndexCheck(nSlaveAxis, MXP_EasyClass::e_CAMIn);
		MXP_CAMIN_OUT stOut = {0,};
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		nResult = MXP_GetCamInOutParam(nMotionIndex, stOut);

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Active == 1? TRUE : FALSE;
		stStatus.Done = stOut.InSync == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamOut(UINT32 nSlaveAxis)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndexCheck(nSlaveAxis, MXP_EasyClass::e_CAMIn);
		MXP_CAMOUT_IN stIn;
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		stIn.Slave.AxisNo = nSlaveAxis;
		stIn.Execute = 0;
		nResult = MXP_CamOutCmd(nMotionIndex, &stIn);

		stIn.Execute = 1;
		nResult = MXP_CamOutCmd(nMotionIndex, &stIn);

		ClearIndex(nMotionIndex);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_Camscaling(UINT32 nMasterAxis, UINT32 nSlaveAxis, MXP_ACTIVATIONMODE nActivationMode, float fActivationPos,
																float fMasterScaling, float fSlaveScaling)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nSlaveAxis, MXP_EasyClass::e_CAMTableOption);
		MXP_CAMSCALING_IN stIn;
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.Master.AxisNo = nMasterAxis;
		stIn.Slave.AxisNo = nSlaveAxis;
		stIn.ActivationMode = nActivationMode;
		stIn.ActivationPosition = fActivationPos;
		stIn.MasterScaling = fMasterScaling;
		stIn.SlaveScaling = fSlaveScaling;

		stIn.Execute = 0;
		nResult = MXP_CamScalingCmd(nMotionIndex, &stIn);

		stIn.Execute = 1;
		nResult = MXP_CamScalingCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_Phasing(UINT32 nMasterAxis, UINT32 nSlaveAxis, float fPhaseShift)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nSlaveAxis, MXP_EasyClass::e_CAMTableOption);
		MXP_PHASING_IN stIn;
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.Master.AxisNo = nMasterAxis;
		stIn.Slave.AxisNo = nSlaveAxis;
		stIn.PhaseShift = fPhaseShift;

		stIn.Execute = 0;
		nResult = MXP_PhasingCmd(nMotionIndex, &stIn);

		stIn.Execute = 1;
		nResult = MXP_PhasingCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamReadSlavePositionRequest(UINT16 nCamTableID, bool bExecute, float fMasterPos)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nCamTableID, MXP_EasyClass::e_ReadCAMSlavePos);
		MXP_RDCAMTABLESLAVEPOS_IN stIn;
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.CamTable.CamTable = nCamTableID;
		stIn.MasterPosition = fMasterPos;

		stIn.Execute = 0;
		nResult = MXP_RdCamTableSlavePosCmd(nMotionIndex, &stIn);

		if (bExecute == TRUE)
		{
			stIn.Execute = 1;
			nResult = MXP_RdCamTableSlavePosCmd(nMotionIndex, &stIn);
		}
		else
			ClearIndex(nMotionIndex);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamReadSlavePositionReply(UINT16 nCamTableID, READ_CAMSLAVEPOSITION_REPLY &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nCamTableID, MXP_EasyClass::e_ReadCAMSlavePos);
		MXP_RDCAMTABLESLAVEPOS_OUT stOut;
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		nResult = MXP_GetRdCamTableSlavePosOutParam(nMotionIndex, stOut);

		if (nResult != 0)
			return (MXP_FUNCTION_STATUS)nResult;


		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Done == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;
		stStatus.SlavePos = stOut.SlavePosition;

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_CamReadSlavePosition(UINT16 nCamTableID, float fMasterPos, INT32 nWaitTime, READ_CAMSLAVEPOSITION_REPLY &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nCamTableID, MXP_EasyClass::e_ReadCAMSlavePos);
		MXP_RDCAMTABLESLAVEPOS_IN stIn;
		MXP_RDCAMTABLESLAVEPOS_OUT stOut;
		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.CamTable.CamTable = nCamTableID;
		stIn.MasterPosition = fMasterPos;
		stIn.Execute = 0;
		nResult = MXP_RdCamTableSlavePosCmd(nMotionIndex, &stIn);

		stIn.Execute = 1;
		nResult = MXP_RdCamTableSlavePosCmd(nMotionIndex, &stIn);

		if (nResult != 0)
		{
			ClearIndex(nMotionIndex);
			return (MXP_FUNCTION_STATUS)nResult;
		}
		Sleep(nWaitTime);

		nResult = MXP_GetRdCamTableSlavePosOutParam(nMotionIndex, stOut);

		if (nResult != 0)
		{
			ClearIndex(nMotionIndex);
			return (MXP_FUNCTION_STATUS)nResult;
		}

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Done == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;
		stStatus.SlavePos = stOut.SlavePosition;

		ClearIndex(nMotionIndex);
		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}


#pragma endregion CAM

#pragma region

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_GetProfileTable(MXP_FILE_TABLE_ARRAY_IN *stUserTable, UINT16 nUserTableCount, MXP_PROFILE_TABLE_ARRAY_IN *stMotionTable)
{
	try
	{
		INT32 nResult;
		UINT16 nMotionTableCount;

		stUserTable->stData.DataSize = nUserTableCount;

		nResult = MXP_GetProfileTable_Ex(&stUserTable->stData, (UINT32)nUserTableCount, (UINT32 &)nMotionTableCount, &stMotionTable->stData);

		stMotionTable->stData.DataSize = nMotionTableCount;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;

		//delete[] stMotionTable;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_ProfileMove(UINT32 nAxisNo, UINT16 nMotionTableCount, UINT16 nIOTableCount, UINT16 nRepeatCount,
																 float fStartDwell, float fEndDwell, UINT16 nReverseMode, 
																 MXP_PROFILE_TABLE_ARRAY_IN *stMotionTable, MXP_IO_TABLE_ARRAY_IN *stIOTable)
{
	INT32 nResult;

	UINT32 nMotionIndex = MC_PROFILEMOVE_INDEX;
	stMotionTable->stData.DataSize = nMotionTableCount;
	stIOTable->stData.DataSize = nIOTableCount;

	stProfileIn.AxisNo = nAxisNo;
	stProfileIn.EndDwell = fEndDwell;
	stProfileIn.IOTablesize = nIOTableCount;
	stProfileIn.RepeatCount = (UINT8)nRepeatCount;
	stProfileIn.ReverseMode = (UINT8)nReverseMode;
	stProfileIn.StartDwell = fStartDwell;
	stProfileIn.Tablesize = nMotionTableCount;

	stProfileIn.Execute = 0;
	nResult = MXP_ProfileMoveCmd_Ex(nMotionIndex, &stProfileIn, &stMotionTable->stData, &stIOTable->stData);

	stProfileIn.Execute = 1;
	nResult = MXP_ProfileMoveCmd_Ex(nMotionIndex, &stProfileIn, &stMotionTable->stData, &stIOTable->stData);

	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;

	delete[] stIOTable;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_ProfileMoveCheck(UINT32 nAxisNo, PROCESS_CHECK &stStatus)
{
	try
	{
		INT32 nResult;

		UINT32 nMotionIndex = MC_PROFILEMOVE_INDEX;

		MXP_PROFILE_MOVE_OUT stOut;

		nResult = MXP_ProfileMoveOutParam_Ex(nMotionIndex, &stProfileIn, stOut);

		if (nResult != 0)
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Done == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
#pragma endregion ProfileMove

#pragma region
bool MXP_EasyClass::WordToBitArray(UINT16 nData,  bool* bArray)
{
	UINT16 nRemain = nData;
	INT16 nbitCount;

	for (nbitCount =0; nbitCount <=15; nbitCount++ )
	{
		bArray[nbitCount] = (nRemain % 2 ? TRUE : FALSE);
		nRemain = nRemain / 2;
	}
	return TRUE;
}
bool MXP_EasyClass::ByteToBitArray(UINT8 nData, bool* bArray)
{
	UINT16 nRemain = nData;
	INT16 nbitCount;

	for (nbitCount =0; nbitCount <=7; nbitCount++ )
	{
		bArray[nbitCount] = (nRemain % 2 ? TRUE : FALSE);
		nRemain = nRemain / 2;
	}
	return TRUE;
}
bool MXP_EasyClass::BitArrayToByte(bool* bArray, UINT8 &nByte)
{
	INT16 nbitCount;
	for (nbitCount =0; nbitCount < sizeof(bArray); nbitCount++ )
	{
		nByte = nByte + (bArray[nbitCount] ? TRUE : FALSE)*2^nbitCount;
	}
	return TRUE;
}
UINT16 buffToUInt16(char * buffer)
{
	int a = int((unsigned char)(buffer[0]) << 24 |
		(unsigned char)(buffer[1]) << 16 |
		(unsigned char)(buffer[2]) << 8 |
		(unsigned char)(buffer[3]));
	return a;
}

short toInt16(char *tail, int index)
{
#if defined(BIGENDIAN)
	return (short)( ((unsigned short)(tail[index])<<8) + (unsigned char)(tail[index+1]));
#else
	return (short)( ((unsigned short)(tail[index+1])<<8) + (unsigned char)(tail[index]));
#endif
}




MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_TouchProbe1Set(UINT32 nAxisNo, MXP_TRIGGER_MODE nMode, MXP_TRIGGER_TYPE nTriggerType, MXP_TRIGGER_EDGE nTriggerEdge)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::GetMotionIndex(nAxisNo, MXP_EasyClass::e_TouchprobeSet);
		MXP_SETTOUCHPROBEFUNC_IN stIn;

		INT32 nResult;

		if (nMotionIndex ==0) 
			return MXP_EasyClass::RET_NO_ERROR;
		UINT8 *arrFunc = new UINT8[0]();
		memcpy(&arrFunc, &arrTouchprobeFunc[nAxisNo], 2);

		arrFunc[0] = (UINT8)(1 + nMode * pow(2.0,1) + nTriggerType * pow(2.0,2));
		arrFunc[0] = arrFunc[0] + (UINT8)(((nTriggerEdge == MXP_EasyClass::e_Falling) ? 0 : 1) * pow(2.0,4));
		arrFunc[0] = arrFunc[0] + (UINT8)(((nTriggerEdge == MXP_EasyClass::e_Rising) ? 0 : 1) * pow(2.0,5));

		memcpy(arrFunc, &arrTouchprobeFunc[nAxisNo], sizeof(arrTouchprobeFunc[nAxisNo]));

		stIn.Axis.AxisNo = nAxisNo;
		stIn.FuncData = arrTouchprobeFunc[nAxisNo];

		stIn.Enable = 1;
		nResult = MXP_SetTouchProbeFunctionCmd(nMotionIndex, &stIn);

		stIn.Enable = 0;
		nResult = MXP_SetTouchProbeFunctionCmd(nMotionIndex, &stIn);

		ClearIndex(nMotionIndex);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_TouchProbe1SingleModeReSet(UINT32 nAxisNo)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::GetMotionIndex(nAxisNo, MXP_EasyClass::e_TouchprobeSet);
		MXP_SETTOUCHPROBEFUNC_IN stIn;

		INT32 nResult;

		if (nMotionIndex ==0) 
			return MXP_EasyClass::RET_NO_ERROR;
		UINT8 *arrFunc = new UINT8[0]();
		memcpy(&arrFunc, &arrTouchprobeFunc[nAxisNo], 2);

		arrFunc[0] = 0; 

		memcpy(arrFunc, &arrTouchprobeFunc[nAxisNo], sizeof(arrTouchprobeFunc[nAxisNo]));

		stIn.Axis.AxisNo = nAxisNo;
		stIn.FuncData = arrTouchprobeFunc[nAxisNo];

		stIn.Enable = 1;
		nResult = MXP_SetTouchProbeFunctionCmd(nMotionIndex, &stIn);

		stIn.Enable = 0;
		nResult = MXP_SetTouchProbeFunctionCmd(nMotionIndex, &stIn);

		ClearIndex(nMotionIndex);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_TouchProbe2Set(UINT32 nAxisNo, MXP_TRIGGER_MODE nMode, MXP_TRIGGER_TYPE nTriggerType, MXP_TRIGGER_EDGE nTriggerEdge)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::GetMotionIndex(nAxisNo, MXP_EasyClass::e_TouchprobeSet);
		MXP_SETTOUCHPROBEFUNC_IN stIn;

		INT32 nResult;

		if (nMotionIndex ==0) 
			return MXP_EasyClass::RET_NO_ERROR;

		UINT8 *arrFunc = new UINT8[0]();
		memcpy(&arrFunc, &arrTouchprobeFunc[nAxisNo], 2);

		arrFunc[1] = (UINT8)(1 + nMode * pow(2.0,1) + nTriggerType * pow(2.0,2));
		arrFunc[1] = arrFunc[1] + (UINT8)(((nTriggerEdge == MXP_EasyClass::e_Falling) ? 0 : 1) * pow(2.0,4));
		arrFunc[1] = arrFunc[1] + (UINT8)(((nTriggerEdge == MXP_EasyClass::e_Rising) ? 0 : 1) * pow(2.0,5));

		memcpy(arrFunc, &arrTouchprobeFunc[nAxisNo], sizeof(arrTouchprobeFunc[nAxisNo]));

		stIn.Axis.AxisNo = nAxisNo;
		stIn.FuncData = arrTouchprobeFunc[nAxisNo];

		stIn.Enable = 1;
		nResult = MXP_SetTouchProbeFunctionCmd(nMotionIndex, &stIn);

		stIn.Enable = 0;
		nResult = MXP_SetTouchProbeFunctionCmd(nMotionIndex, &stIn);

		ClearIndex(nMotionIndex);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}

}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_TouchProbe2SingleModeReSet(UINT32 nAxisNo)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::GetMotionIndex(nAxisNo, MXP_EasyClass::e_TouchprobeSet);
		MXP_SETTOUCHPROBEFUNC_IN stIn;

		INT32 nResult;

		if (nMotionIndex ==0) 
			return MXP_EasyClass::RET_NO_ERROR;

		UINT8 *arrFunc = new UINT8[0]();
		memcpy(&arrFunc, &arrTouchprobeFunc[nAxisNo], 2);

		arrFunc[1] = 0; 
		memcpy(arrFunc, &arrTouchprobeFunc[nAxisNo], sizeof(arrTouchprobeFunc[nAxisNo]));

		stIn.Axis.AxisNo = nAxisNo;
		stIn.FuncData = arrTouchprobeFunc[nAxisNo];

		stIn.Enable = 1;
		nResult = MXP_SetTouchProbeFunctionCmd(nMotionIndex, &stIn);

		stIn.Enable = 0;
		nResult = MXP_SetTouchProbeFunctionCmd(nMotionIndex, &stIn);

		ClearIndex(nMotionIndex);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_TouchProbe1ReadState(UINT32 nAxisNo, strTouchprobeState &stState)
{
	try
	{
		MXP_READACTUALTOUCHPROBESTATUS_IN stIn;
		MXP_READACTUALTOUCHPROBESTATUS_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Enable = 1;

		nResult = MXP_ReadActualTouchProbeStatus(&stIn, stOut);

		UINT8 *arrFunc = new UINT8[0]();
		memcpy(&arrFunc, &stOut.Status, 2);

		bool arrbit[8];

		ByteToBitArray(arrFunc[0], arrbit);

		stState.TouchprobeUsing = arrbit[0];
		stState.TouchprobeRisingEdgeSave = arrbit[1];
		stState.TouchprobeFallingEdgeSave = arrbit[2];
		stState.TouchprobeRisingPositionUpdate = arrbit[6];
		stState.TouchprobeFallingPositionUpdate = arrbit[7];

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}	
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_TouchProbe2ReadState(UINT32 nAxisNo, strTouchprobeState &stState)
{
	try
	{
		MXP_READACTUALTOUCHPROBESTATUS_IN stIn;
		MXP_READACTUALTOUCHPROBESTATUS_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Enable = 1;

		nResult = MXP_ReadActualTouchProbeStatus(&stIn, stOut);

		UINT8 *arrFunc = new UINT8[0]();
		memcpy(&arrFunc, &stOut.Status, 2);

		bool arrbit[8];

		ByteToBitArray(arrFunc[1], arrbit);

		stState.TouchprobeUsing = arrbit[0];
		stState.TouchprobeRisingEdgeSave = arrbit[1];
		stState.TouchprobeFallingEdgeSave = arrbit[2];
		stState.TouchprobeRisingPositionUpdate = arrbit[6];
		stState.TouchprobeFallingPositionUpdate = arrbit[7];

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}	
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_TouchProbe1ReadPosition(UINT32 nAxisNo, MXP_TRIGGER_EDGE nTriggerEdge, TouchProbeReadPos_Reply &stPosition)
{
	try
	{
		MXP_READACTUALTOUCHPROBEPOSITION_IN stIn;
		MXP_READACTUALTOUCHPROBEPOSITION_EX_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Channel = MXP_TOUCH_CH1;
		stIn.Edge = (MXP_TOUCHPROBE_EDGE_ENUM)nTriggerEdge;
		stIn.Enable = 1;

		nResult = MXP_ReadActualTouchProbePosition_Ex(&stIn, stOut);

		stPosition.EdgePositivePosition = stOut.EdgePositivePosition;
		stPosition.EdgeNegativePosition = stOut.EdgeNegativePosition;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}	
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_TouchProbe2ReadPosition(UINT32 nAxisNo, MXP_TRIGGER_EDGE nTriggerEdge, TouchProbeReadPos_Reply &stPosition)
{
	try
	{
		MXP_READACTUALTOUCHPROBEPOSITION_IN stIn;
		MXP_READACTUALTOUCHPROBEPOSITION_EX_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Channel = MXP_TOUCH_CH2;
		stIn.Edge = (MXP_TOUCHPROBE_EDGE_ENUM)nTriggerEdge;
		stIn.Enable = 1;

		nResult = MXP_ReadActualTouchProbePosition_Ex(&stIn, stOut);

		stPosition.EdgePositivePosition = stOut.EdgePositivePosition;
		stPosition.EdgeNegativePosition = stOut.EdgeNegativePosition;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}	
}
#pragma endregion TouchProbe

#pragma region
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_MoveDirectTorque(UINT32 nAxisNo, float fTorque)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_DIRECTTORQUECONTROL_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Torque = fTorque;
		stIn.Enable = 1;

		nResult = MXP_DirectTorqueControlCmd(nMotionIndex, &stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_MoveDirectVelocity(UINT32 nAxisNo, float fVelocity)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_MOVEDIRECTVELOCITY_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Velocity = fVelocity;
		stIn.Enable = 1;

		nResult = MXP_MoveDirectVelocityCmd(nMotionIndex, &stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_MoveDirectPosition(UINT32 nAxisNo, float fPosition)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_MOVEDIRECTPOSITION_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Position = fPosition;
		stIn.Enable = 1;

		nResult = MXP_MoveDirectPositionCmd(nMotionIndex, &stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
#pragma endregion DirectSet

#pragma region
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_ReadActualPosition(UINT32 nAxisNo, float &fPosition)
{
	try
	{
		MXP_READACTUALPOSITION_IN stIn;
		MXP_READACTUALPOSITION_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;

		stIn.Enable = 1;
		nResult = MXP_ReadActualPosition(&stIn, stOut);

		fPosition = stOut.Position;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_ReadActualVelocity(UINT32 nAxisNo, float &fVelocity)
{
	try
	{
		MXP_READACTUALVELOCITY_IN stIn;
		MXP_READACTUALVELOCITY_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;

		stIn.Enable = 1;
		nResult = MXP_ReadActualVelocity(&stIn, stOut);

		fVelocity = stOut.Velocity;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_ReadFollowingError(UINT32 nAxisNo, float &fFollowingErrorValue)
{
	try
	{
		MXP_READFOLLOWINGERRORVALUE_IN stIn;
		MXP_READFOLLOWINGERRORVALUE_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;

		stIn.Enable = 1;
		nResult = MXP_ReadFollowingErrorValue(&stIn, stOut);

		fFollowingErrorValue = stOut.FollowingErrorValue;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_ReadActualTorque(UINT32 nAxisNo, float &fTorque)
{
	try
	{
		MXP_READACTUALTORQUE_IN stIn;
		MXP_READACTUALTORQUE_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;

		stIn.Enable = 1;
		nResult = MXP_ReadActualTorque(&stIn, stOut);

		fTorque = stOut.Torque;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_ReadCommandPosition(UINT32 nAxisNo, float &fPosition)
{
	try
	{
		MXP_READCOMMANDPOSITION_IN stIn;
		MXP_READCOMMANDPOSITION_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;

		stIn.Enable = 1;
		nResult = MXP_ReadCommandPosition(&stIn, stOut);

		fPosition = stOut.CommandPosition;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_ReadCommandVelocity(UINT32 nAxisNo, float &fVelocity)
{
	try
	{
		MXP_READCOMMANDVELOCITY_IN stIn;
		MXP_READCOMMANDVELOCITY_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;

		stIn.Enable = 1;
		nResult = MXP_ReadCommandVelocity(&stIn, stOut);

		fVelocity = stOut.CommandVelocity;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_ReadError(UINT32 nAxisNo, AXIS_ERROR &stAxisError)
{
	try
	{
		MXP_READAXISERROR_IN stIn;
		MXP_READAXISERROR_OUT stOut = {0,};

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;

		stIn.Enable = 1;
		nResult = MXP_ReadAxisError(&stIn, stOut);

		stAxisError.MXPError = stOut.AxisErrorID;
		stAxisError.DriveError = stOut.AuxErrorID;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}


MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_ReadStatus(UINT32 nAxisNo, MXP_AxisStateBit &stAxisStatus)
{
	try
	{
		MXP_READSTATUS_IN stIn;
		MXP_READSTATUS_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Enable = 1;

		nResult = MXP_ReadStatus(&stIn, stOut);
		stAxisStatus.ErrorStop = (stOut.ErrorStop == 1 ? TRUE : FALSE);
		stAxisStatus.Disable = (stOut.Disabled == 1 ? TRUE : FALSE);
		stAxisStatus.Stopping = (stOut.Stopping == 1 ? TRUE : FALSE);
		stAxisStatus.Standstill = (stOut.Standstill == 1 ? TRUE : FALSE);
		stAxisStatus.DiscreteMotion = (stOut.DiscreteMotion == 1 ? TRUE : FALSE);
		stAxisStatus.ContinuousMotion = (stOut.ContinuousMotion == 1 ? TRUE : FALSE);
		stAxisStatus.SynchronizedMotion = (stOut.SynchronizedMotion == 1 ? TRUE : FALSE);
		stAxisStatus.Homming = (stOut.Homing == 1 ? TRUE : FALSE);

		MXP_READMOTIONSTATE_IN nIn1;
		MXP_READMOTIONSTATE_OUT nOut1;

		nIn1.Axis.AxisNo = nAxisNo;
		nIn1.Enable = 1;
		nIn1.Source = (::MXP_SOURCE_ENUM)0;

		nResult = MXP_ReadMotionState(&nIn1, nOut1);
		stAxisStatus.ConstantVelocity = (nOut1.ConstantVelocity == 1 ? TRUE : FALSE);
		stAxisStatus.Accelerating = (nOut1.Accelerating == 1 ? TRUE : FALSE);
		stAxisStatus.Decelerating = (nOut1.Decelerating == 1 ? TRUE : FALSE);
		stAxisStatus.DirectionPositive = (nOut1.DirectionPositive == 1 ? TRUE : FALSE);
		stAxisStatus.DirectionNegative = (nOut1.DirectionNegative == 1 ? TRUE : FALSE);

		MXP_READAXISINFO_IN nIn2;
		MXP_READAXISINFO_OUT nOut2;

		nIn2.Axis.AxisNo = nAxisNo;
		nIn2.Enable = 1;

		nResult = MXP_ReadAxisInfo(&nIn2, nOut2);
		stAxisStatus.HomeAbsSwitch = (nOut2.HomeAbsSwitch == 1 ? TRUE : FALSE);
		stAxisStatus.HWLimitSwitchPosEvent = (nOut2.LimitSwitchPos == 1 ? TRUE : FALSE);
		stAxisStatus.HWLimitSwitchNegEvent = (nOut2.LimitSwitchNeg == 1 ? TRUE : FALSE);
		stAxisStatus.ReadyForPowerOn = (nOut2.ReadyForPowerOn == 1 ? TRUE : FALSE);
		stAxisStatus.PowerOn = (nOut2.PowerOn == 1 ? TRUE : FALSE);
		stAxisStatus.IsHomed = (nOut2.IsHomed == 1 ? TRUE : FALSE);
		stAxisStatus.AxisWarning = (nOut2.AxisWarning == 1 ? TRUE : FALSE);


		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_ReadParameter(UINT32 nAxisNo, UINT16 nParameterNum, float &fValue)
{
	try
	{
		MXP_READPARAMETER_IN stIn;
		MXP_READPARAMETER_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.ParameterNumber = nParameterNum;

		stIn.Enable = 1;

		nResult = MXP_ReadParameter(&stIn,stOut);

		fValue = stOut.Value;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_WriteParameter(UINT32 nAxisNo, UINT16 nParameterNum, float fValue)
{
	try
	{
		MXP_WRITEPARAMETER_IN stIn;
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.ParameterNumber = nParameterNum;
		stIn.Value = fValue;

		stIn.Execute = 0;
		nResult = MXP_WriteParameterCmd(nMotionIndex ,&stIn);

		stIn.Execute = 1;
		nResult = MXP_WriteParameterCmd(nMotionIndex ,&stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_StoreParameter()
{
	try
	{
		MXP_WRITEPARAMETEREX_IN stIn;

		stIn.Execute = TRUE;
		stIn.ExecutionMode = MXP_IMMEDIATELY;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)MXP_WriteParameterExCmd(&stIn);
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_ReadPdoData(UINT32 nSlaveNo, MXP_PDO_DIRECTION nDirection, UINT16 nOffset, UINT16 nSize, UINT8 *arrValue)
{
	try
	{
		MXP_READPDODATA_IN stIn;
		MXP_READPDODATA_OUT stOut;
		INT32 nResult;

		stIn.Input.SourceNo = nSlaveNo;
		stIn.Direction = nDirection;
		stIn.Offset = nOffset;
		stIn.Size = nSize;
		stIn.Enable = 1;

		nResult = MXP_ReadPDOData(&stIn, stOut, &arrValue[0]);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_WritePdoData(UINT32 nSlaveNo, UINT16 nOffset, UINT16 nSize, UINT8 *arrValue)
{
	try
	{
		MXP_WRITEPDODATA_IN stIn;

		INT32 nResult;
		INT32 nMotionIndex = MC_PDOWRITE;
		UINT8 arrValue[4] = {0, };

		stIn.Output.SourceNo = nSlaveNo;
		stIn.Offset = nOffset;
		stIn.Size = nSize;
		stIn.Direction = MXP_EasyClass::e_MXPWrite;

		stIn.Execute = 0;
		nResult = MXP_WritePDODataCmd(nMotionIndex, &stIn, &arrValue[0]);
		stIn.Execute = 1;
		nResult = MXP_WritePDODataCmd(nMotionIndex, &stIn, &arrValue[0]);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_ReadAxisIO(UINT32 nAxisNo, UINT8 *arrValue)
{
	try
	{
		MXP_READOUTPUTS_IN stIn;
		MXP_READOUTPUTS_OUT stOut;
		INT32 nResult;

		stIn.Output.SourceNo = nAxisNo;
		stIn.Enable = 1;

		nResult = MXP_ReadOutputs(&stIn, stOut, &arrValue[0]);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_WriteAxisIO(UINT32 nAxisNo, UINT8 *arrValue)
{
	try
	{
	MXP_WRITEOUTPUTS_IN stIn;

	INT32 nResult;
	INT32 nMotionIndex = MC_PDOWRITE;

	stIn.Output.SourceNo = nAxisNo;
	stIn.Size = 4;
	stIn.Execute = 0;
	nResult = MXP_WriteOutputsCmd(nMotionIndex, &stIn, &arrValue[0]);
	stIn.Execute = 1;
	nResult = MXP_WriteOutputsCmd(nMotionIndex, &stIn, &arrValue[0]);

	return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_CoeReadRequest(UINT32 nSlaveNo, UINT16 nIndex, UINT16 nSubIndex, UINT16 nSize)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nSlaveNo, MXP_EasyClass::e_ETParameterReadReq);
		MXP_ET_READPARAMETER_IN stIn;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.SlaveNo = nSlaveNo;
		stIn.Index = nIndex;
		stIn.SubIndex = nSubIndex;
		stIn.BufLen = nSize;

		nResult = MXP_ET_ReadParameterCmd(nMotionIndex, &stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_CoeReadReply(UINT32 nSlaveNo, READ_ETParameterReply &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndexCheck(nSlaveNo, MXP_EasyClass::e_ETParameterReadReq);
		MXP_ET_READPARAMETER_OUT stOut;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		nResult = MXP_ET_GetReadParameterOutParam(nMotionIndex, stOut);

		if (nResult != 0)
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;

		memmove(stStatus.ReadData, stOut.Data, sizeof(stStatus.ReadData));

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Valid == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_CoeWriteRequest(UINT32 nSlaveNo, UINT16 nIndex, UINT16 nSubIndex, UINT16 nSize, UINT32 nWriteData)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nSlaveNo, MXP_EasyClass::e_ETParameterWriteReq);
		MXP_ET_WRITEPARAMETER_IN stIn;

		INT32 nResult;


		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.SlaveNo = nSlaveNo;
		stIn.Index = nIndex;
		stIn.SubIndex = nSubIndex;
		stIn.BufLen = nSize;
		stIn.Data = nWriteData;

		nResult = MXP_ET_WriteParameterCmd(nMotionIndex, &stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_CoeWriteReply(UINT16 nSlaveNo, PROCESS_CHECK &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndexCheck(nSlaveNo, MXP_EasyClass::e_ETParameterWriteReq);
		MXP_ET_WRITEPARAMETER_OUT stOut;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		nResult = MXP_ET_GetWriteParameterOutParam(nMotionIndex, stOut);

		if (nResult != 0)
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Done == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_CoeWrite(UINT32 nSlaveNo, UINT16 nIndex, UINT16 nSubIndex, UINT16 nSize, 
																UINT32 nWriteData, INT32 nWaitTime, PROCESS_CHECK &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nSlaveNo, MXP_EasyClass::e_ETParameterWriteReq);
		MXP_ET_WRITEPARAMETER_IN stIn;
		MXP_ET_WRITEPARAMETER_OUT stOut;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.SlaveNo = nSlaveNo;
		stIn.Index = nIndex;
		stIn.SubIndex = nSubIndex;
		stIn.BufLen = nSize;
		stIn.Data = nWriteData;

		nResult = MXP_ET_WriteParameterCmd(nMotionIndex, &stIn);

		if (nResult != 0)
		{	
			ClearIndex(nMotionIndex);
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		}

		Sleep(nWaitTime);
		nResult = MXP_ET_GetWriteParameterOutParam(nMotionIndex, stOut);
		if (nResult != 0)
		{	
			ClearIndex(nMotionIndex);
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		}

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Done == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;

		ClearIndex(nMotionIndex);
		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::ECAT_CoeRead(UINT32 nSlaveNo, UINT16 nIndex, UINT16 nSubIndex, UINT16 nSize, 
																INT32 nWaitTime, READ_ETParameterReply &stStatus)
{
	try
	{
		UINT32 nMotionIndex = GetMotionIndex(nSlaveNo, MXP_EasyClass::e_ETParameterWriteReq);
		MXP_ET_READPARAMETER_IN stIn;
		MXP_ET_READPARAMETER_OUT stOut;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		stIn.SlaveNo = nSlaveNo;
		stIn.Index = nIndex;
		stIn.SubIndex = nSubIndex;
		stIn.BufLen = nSize;

		nResult = MXP_ET_ReadParameterCmd(nMotionIndex, &stIn);

		if (nResult != 0)
		{	
			ClearIndex(nMotionIndex);
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		}
		Sleep(nWaitTime);

		nResult = MXP_ET_GetReadParameterOutParam(nMotionIndex, stOut);
		if (nResult != 0)
		{	
			ClearIndex(nMotionIndex);
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		}

		memcpy(stStatus.ReadData, stOut.Data, sizeof(stStatus.ReadData));

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Valid == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;

		ClearIndex(nMotionIndex);
		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_AccDecToAccTime(UINT32 nAxisNo, float fTargetVel, float fAccDec, float fJerk, AccDecToAccTime_Reply &stData)
{
	try
	{
		MXP_ACCDECTOACCTIME_IN stIn;
		MXP_ACCDECTOACCTIME_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.TargetVel = fTargetVel;
		stIn.AccDec = fAccDec;
		stIn.Jerk = fJerk;
		stIn.Enable = 1;
		nResult = MXP_AccDecToAccTime(&stIn,stOut);

		stData.AccDecBuildUp = stOut.AccDecBuildUp;
		stData.LimitAccDec = stOut.LimitAccDec;
		stData.AccDecRampDown = stOut.AccDecRampDown;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_AccTimeToAccDec(UINT32 nAxisNo, float fTargetVel, float fAccDecBuildUp, float fLimitAccDec,
																	 float fAccDecRampDown, AccTimeToAccDec_Reply &stData)
{
	try
	{
		MXP_ACCTIMETOACCDEC_IN stIn;
		MXP_ACCTIMETOACCDEC_OUT stOut;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.TargetVel = fTargetVel;
		stIn.AccDecBuildUp = fAccDecBuildUp;
		stIn.LimitAccDec = fLimitAccDec;
		stIn.AccDecRampDown = fAccDecRampDown;

		stIn.Enable = 1;
		nResult = MXP_AccTimeToAccDec(&stIn,stOut);

		stData.Accdec = stOut.AccDec;
		stData.Jerk = stOut.Jerk;


		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_ReadyCheck(UINT32 nAxisNo, bool &bReady)
{
	try
	{
		MXP_AxisStateBit stState;
		INT32 nResult;

		nResult = AX_ReadStatus(nAxisNo, stState);
		bReady = (stState.PowerOn == TRUE && stState.Standstill == TRUE ? TRUE :FALSE);
		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_MoveStateCheck(UINT32 nAxisNo, float fTargetPos, float fInPositionCheckRange, MXP_MOVESTATE &nMoveState)
{
	try
	{
		float nCurPos;
		MXP_AxisStateBit stState;
		INT32 nResult;
		bool bInpos;

		nResult = AX_ReadActualPosition(nAxisNo, nCurPos);
		nResult = AX_ReadStatus(nAxisNo, stState);

		bInpos = (abs(fTargetPos - nCurPos) < fInPositionCheckRange) ? TRUE: FALSE;
		if (stState.Standstill && bInpos)
			nMoveState = MXP_EasyClass::MOVESTATE_Complete;
		if (stState.ErrorStop)
			nMoveState = MXP_EasyClass::MOVESTATE_Fail;
		if (stState.ConstantVelocity || stState.Accelerating || stState.Decelerating)
			nMoveState = MXP_EasyClass::MOVESTATE_Moving;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

#pragma endregion Read Data

#pragma region

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_Stop(UINT32 nAxesGroup, bool bExecute, float fDec, float fJerk)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::MC_GROUPAXIS_INDEX + nAxesGroup;
		MXP_GROUPSTOP_IN stIn;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;
		stIn.Deceleration = fDec;
		stIn.Jerk = fJerk;

		stIn.Execute = (bExecute == TRUE ? 1 : 0);

		nResult = MXP_GroupStopCmd(nMotionIndex,&stIn);

		if(bExecute)
			mGroupSequenceData[nAxesGroup].bRunFlag = false;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}

}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_MoveLinearAbsolute(UINT32 nAxesGroup, GROUP_POS stPosition, float fVelocity, float fAcceleration,
																		 float fDeceleration, float fJerk, MXP_BUFFERMODE_ENUM nBufferMode)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::MC_GROUPAXIS_INDEX + nAxesGroup;
		MXP_MOVELINEARABSOLUTE_IN stIn;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;
		stIn.PositionX = stPosition.nX;
		stIn.PositionY = stPosition.nY;
		stIn.PositionZ = stPosition.nZ;
		stIn.PositionU = stPosition.nU;
		stIn.PositionV = stPosition.nV;
		stIn.PositionW = stPosition.nW;
		stIn.PositionA = stPosition.nA;
		stIn.PositionB = stPosition.nB;
		stIn.PositionC = stPosition.nC;
		stIn.Velocity = fVelocity;
		stIn.Acceleration = fAcceleration;
		stIn.Deceleration = fDeceleration;
		stIn.Jerk = fJerk;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;


		stIn.Execute = 0;
		nResult = MXP_MoveLinearAbsoluteCmd(nMotionIndex,&stIn);

		stIn.Execute = 1;
		nResult = MXP_MoveLinearAbsoluteCmd(nMotionIndex,&stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}

}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_MoveLinearRelative(UINT32 nAxesGroup, GROUP_POS stDistance, float fVelocity, float fAcceleration,
																		 float fDeceleration, float fJerk, MXP_BUFFERMODE_ENUM nBufferMode)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::MC_GROUPAXIS_INDEX + nAxesGroup;
		MXP_MOVELINEARRELATIVE_IN stIn;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;
		stIn.DistanceX = stDistance.nX;
		stIn.DistanceY = stDistance.nY;
		stIn.DistanceZ = stDistance.nZ;
		stIn.DistanceU = stDistance.nU;
		stIn.DistanceV = stDistance.nV;
		stIn.DistanceW = stDistance.nW;
		stIn.DistanceA = stDistance.nA;
		stIn.DistanceB = stDistance.nB;
		stIn.DistanceC = stDistance.nC;

		stIn.Velocity = fVelocity;
		stIn.Acceleration = fAcceleration;
		stIn.Deceleration = fDeceleration;
		stIn.Jerk = fJerk;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;


		stIn.Execute = 0;
		nResult = MXP_MoveLinearRelativeCmd(nMotionIndex,&stIn);

		stIn.Execute = 1;
		nResult = MXP_MoveLinearRelativeCmd(nMotionIndex,&stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_MoveCircularAbsolute(UINT32 nAxesGroup, MXP_PATHCHOICE_ENUM nPathChoice, float fAuxPoint1, float fAuxPoint2,
	                                                                       float fEndPoint1, float fEndPoint2, MXP_PLANE nPlane1, MXP_PLANE nPlane2, 
																		   float fVelocity, float fAcceleration, float fDeceleration, 
																		   float fJerk, MXP_BUFFERMODE_ENUM nBufferMode)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::MC_GROUPAXIS_INDEX + nAxesGroup;
		MXP_MOVECIRCULARABSOLUTE_IN stIn;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;
		stIn.CircMode = (MXP_CIRCLEMODE_ENUM)0;
		stIn.AuxPoint1 = fAuxPoint1;
		stIn.AuxPoint2 = fAuxPoint2;
		stIn.EndPoint1 = fEndPoint1;
		stIn.EndPoint2 = fEndPoint2;
		stIn.PathChoice = (::MXP_PATHCHOICE_ENUM)nPathChoice;
		stIn.Plane1 = nPlane1;
		stIn.Plane2 = nPlane2;
		stIn.Velocity = fVelocity;
		stIn.Acceleration = fAcceleration;
		stIn.Deceleration = fDeceleration;
		stIn.Jerk = fJerk;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;


		stIn.Execute = 0;
		nResult = MXP_MoveCircularAbsoluteCmd(nMotionIndex,&stIn);

		stIn.Execute = 1;
		nResult = MXP_MoveCircularAbsoluteCmd(nMotionIndex,&stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_MoveCircularRelative(UINT32 nAxesGroup, MXP_PATHCHOICE_ENUM nPathChoice, float fAuxPoint1, float fAuxPoint2,
																		   float fEndPoint1, float fEndPoint2, MXP_PLANE nPlane1, MXP_PLANE nPlane2, 
																		   float fVelocity, float fAcceleration, float fDeceleration, 
																		   float fJerk, MXP_BUFFERMODE_ENUM nBufferMode)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::MC_GROUPAXIS_INDEX + nAxesGroup;
		MXP_MOVECIRCULARRELATIVE_IN stIn;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;
		stIn.CircMode = (MXP_CIRCLEMODE_ENUM)0;
		stIn.AuxPoint1 = fAuxPoint1;
		stIn.AuxPoint2 = fAuxPoint2;
		stIn.EndPoint1 = fEndPoint1;
		stIn.EndPoint2 = fEndPoint2;
		stIn.PathChoice = (::MXP_PATHCHOICE_ENUM)nPathChoice;
		stIn.Plane1 = nPlane1;
		stIn.Plane2 = nPlane2;
		stIn.Velocity = fVelocity;
		stIn.Acceleration = fAcceleration;
		stIn.Deceleration = fDeceleration;
		stIn.Jerk = fJerk;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;


		stIn.Execute = 0;
		nResult = MXP_MoveCircularRelativeCmd(nMotionIndex,&stIn);

		stIn.Execute = 1;
		nResult = MXP_MoveCircularRelativeCmd(nMotionIndex,&stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_SetOverride(UINT32 nAxesGroup, float fVelFactor)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::MC_GROUPAXIS_INDEX + nAxesGroup;
		MXP_GroupSetOverride_IN stIn;

		INT32 nResult;

		stIn.AxesGroupxis.AxesGroup = nAxesGroup;
		stIn.VelFactor = fVelFactor;

		stIn.Enable = 0;
		nResult = MXP_GroupSetOverrideCmd(nMotionIndex,&stIn);

		stIn.Enable = 1;
		nResult = MXP_GroupSetOverrideCmd(nMotionIndex,&stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_ReadActualVelocity(UINT32 nAxesGroup, float &fPathVelocity)
{
	try
	{
		MXP_GROUPREADACTUALVELOCITY_IN stIn;
		MXP_GROUPREADACTUALVELOCITY_OUT stOut;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;

		stIn.Enable = 1;
		nResult = MXP_GroupReadActualVelocity(&stIn, stOut);
		fPathVelocity = stOut.PathVelocity;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_ReadActualPosition(UINT32 nAxesGroup, GROUP_POS &stPosition)
{
	try
	{
		MXP_GROUPREADACTUALPOSITION_IN stIn;
		MXP_GROUPREADACTUALPOSITION_OUT stOut;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;

		stIn.Enable = 1;
		nResult = MXP_GroupReadActualPosition(&stIn, stOut);
		stPosition.nX = stOut.PositionX;
		stPosition.nY = stOut.PositionY;
		stPosition.nZ = stOut.PositionZ;
		stPosition.nU = stOut.PositionU;
		stPosition.nV = stOut.PositionV;
		stPosition.nW = stOut.PositionV;
		stPosition.nA = stOut.PositionA;
		stPosition.nB = stOut.PositionB;
		stPosition.nC = stOut.PositionC;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_ReadCommandVelocity(UINT32 nAxesGroup, float &fPathVelocity)
{
	try
	{
		MXP_GROUPREADCOMMANDVELOCITY_IN stIn;
		MXP_GROUPREADCOMMANDVELOCITY_OUT stOut;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;

		stIn.Enable = 1;
		nResult = MXP_GroupReadCommandVelocity(&stIn, stOut);
		fPathVelocity = stOut.PathCommandVelocity;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_ReadCommandPosition(UINT32 nAxesGroup, GROUP_POS &stPosition)
{
	try
	{
		MXP_GROUPREADCOMMANDPOSITION_IN stIn;
		MXP_GROUPREADCOMMANDPOSITION_OUT stOut;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;

		stIn.Enable = 1;
		nResult = MXP_GroupReadCommandPosition(&stIn, stOut);
		stPosition.nX = stOut.PositionX; 
		stPosition.nY = stOut.PositionY;
		stPosition.nZ = stOut.PositionZ;
		stPosition.nU = stOut.PositionU;
		stPosition.nV = stOut.PositionV;
		stPosition.nW = stOut.PositionV;
		stPosition.nA = stOut.PositionA;
		stPosition.nB = stOut.PositionB;
		stPosition.nC = stOut.PositionC;


		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_GetGroupAxis(UINT32 nAxesGroup, INT32 *arrAxisNo)
{
	try

	{
		MXP_EasyClass::MXP_FUNCTION_STATUS nResult;
		UINT32 nAxisCount;

		nResult = ECAT_GetAxisCount(nAxisCount);

		if (nResult != MXP_EasyClass::RET_NO_ERROR)
		{
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		}
		else
		{
			UINT32 nCount;
			for (nCount = 0; nCount < nAxisCount; nCount++)
			{
				float nGroupUsing;
				float nGroupNo;
				float nAxis;

				nResult = AX_ReadParameter(nCount, 400, nGroupUsing);
				nResult = AX_ReadParameter(nCount, 401, nGroupNo);
				nResult = AX_ReadParameter(nCount, 402, nAxis);

				if (nGroupUsing==1 && nGroupNo == nAxesGroup)
					arrAxisNo[(INT32)nAxis] = nCount;
			}
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		}

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_ReadyCheck(UINT32 nAxesGroup, bool &bReady)
{
	try
	{
		MXP_AxisStateBit stState;
		INT32 nResult;
		INT32 nGroupAxisInfo[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
		INT32 nCount;
		bReady = FALSE;

		nResult = GRP_GetGroupAxis(nAxesGroup, nGroupAxisInfo);
		if(nResult != MXP_EasyClass::RET_NO_ERROR)
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;

		for (nCount = 0; nCount < sizeof(nGroupAxisInfo)/sizeof(*nGroupAxisInfo); nCount++)
		{
			if(nGroupAxisInfo[nCount] != -1)
			{
				nResult = AX_ReadStatus(nGroupAxisInfo[nCount], stState);
				bReady = stState.PowerOn == TRUE ? TRUE : FALSE;
				if(bReady ==FALSE)
					return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
			}
		}
		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS   MXP_EasyClass::GRP_ReadStatus(UINT32 nAxesGroup, MXP_AxisStateBit &stAxisStatus)
{
	try
	{
		MXP_READSTATUS_IN stIn;
		MXP_READSTATUS_OUT stOut;

		INT32 nResult;

		INT32 arrGroupAxisInfo[] = {-1,-1,-1,-1,-1,-1,-1,-1};
		INT32 nCount;

		nResult = GRP_GetGroupAxis(nAxesGroup, arrGroupAxisInfo);
		if(nResult != RET_NO_ERROR)
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		for (nCount = 0; nCount < sizeof(arrGroupAxisInfo)/sizeof(*arrGroupAxisInfo); nCount++)
		{
			if(arrGroupAxisInfo[nCount] != -1)
			{
				stIn.Axis.AxisNo = arrGroupAxisInfo[nCount];
				stIn.Enable = 1;

				nResult = MXP_ReadStatus(&stIn, stOut);
				if (stOut.ErrorStop == 1)
					stAxisStatus.ErrorStop;
				if (stOut.Disabled == 1)
					stAxisStatus.Disable;
				if (stOut.Stopping == 1)
					stAxisStatus.Stopping;
				if (stOut.Standstill == 1)
					stAxisStatus.Standstill;
				if (stOut.DiscreteMotion == 1)
					stAxisStatus.DiscreteMotion;
				if (stOut.ContinuousMotion == 1)
					stAxisStatus.ContinuousMotion;
				if (stOut.SynchronizedMotion == 1)
					stAxisStatus.SynchronizedMotion;
				if (stOut.Homing == 1)
					stAxisStatus.Homming;
			}
		}
		if (stAxisStatus.Disable == FALSE && stAxisStatus.ErrorStop == FALSE && stAxisStatus.Stopping == FALSE && 
			stAxisStatus.DiscreteMotion == FALSE && stAxisStatus.ContinuousMotion == FALSE && 
			stAxisStatus.SynchronizedMotion == FALSE && stAxisStatus.Homming == FALSE)
			stAxisStatus.Standstill = TRUE;


		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_BufferedIO(UINT32 nAxesGroup, UINT16 nSlaveNo, UINT16 nBitPos, bool bBitValue)
{
	try
	{
		UINT32 nMotionIndex = MC_GROUPAXIS_INDEX + nAxesGroup;
		MXP_GROUPBUFFEREDDIGITALIO_IN stIn;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_EasyClass::MXP_BUFFERED;
		stIn.SlaveNo = nSlaveNo;
		stIn.BitPosition = nBitPos;
		stIn.BitValue = (bBitValue == TRUE)? 1 : 0;

		stIn.Execute = 0;
		nResult = MXP_GroupBufferedDigitalioCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_GroupBufferedDigitalioCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_Dwell(UINT32 nAxesGroup, float fTime)
{
	try
	{
		UINT32 nMotionIndex = MC_GROUPAXIS_INDEX + nAxesGroup;
		MXP_GROUPDWELL_IN stIn;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_EasyClass::MXP_BUFFERED;
		stIn.TimeValue = fTime;

		stIn.Execute = 0;
		nResult = MXP_GroupDwellCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_GroupDwellCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}


#pragma endregion Group

#pragma region

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_ReadBit(UINT32 nAddress, UINT8 nBitNo, bool &bData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		DWORD nReadData;
		nResult = MXP_PLC_ReadSystemRegister(nAddress, nBitNo, MXP_EasyClass::DATA_TYPE_Bit, nReadData);

		bData = (nReadData == 1) ? true : false;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_ReadByte(UINT32 nAddress, UINT8 &nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		UINT8 nReadData;
		nResult = MXP_PLC_ReadSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_Byte,  (DWORD &)nReadData);

		nData = nReadData;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_ReadSByte(UINT32 nAddress, INT8 &nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		UINT32 nReadData;

		nResult = MXP_PLC_ReadSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_Byte,  (DWORD &)nReadData);

		UINT8 nByteData = nReadData;
		nData = nByteData <128 ? nByteData : nByteData - 256;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_ReadUInt16(UINT32 nAddress, UINT16 &nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		UINT32 nReadData;

		nResult = MXP_PLC_ReadSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_Word,  (DWORD &)nReadData);

		UINT8 arrByte[4];
		memcpy(&arrByte, &nReadData, 4);

		memcpy(&nData, arrByte, sizeof(nData));

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_ReadInt16(UINT32 nAddress, INT16 &nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		UINT32 nReadData;

		nResult = MXP_PLC_ReadSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_Word,   (DWORD&)nReadData);

		UINT8 arrByte[4];
		memcpy(&arrByte, &nReadData, 4);

		memcpy(&nData, arrByte, sizeof(nData));

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_ReadUInt32(UINT32 nAddress, UINT32 &nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		UINT32 nReadData;

		nResult = MXP_PLC_ReadSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_L,  (DWORD&)nReadData);

		UINT8 arrByte[4];
		memcpy(&arrByte, &nReadData, 4);

		memcpy(&nData, arrByte, sizeof(nData));

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_ReadInt32(UINT32 nAddress, INT32 &nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		UINT32 nReadData;


		nResult = MXP_PLC_ReadSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_L,  (DWORD&)nReadData);

		UINT8 arrByte[4];
		memcpy(&arrByte, &nReadData, 4);

		memcpy(&nData, arrByte, sizeof(nData));

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_ReadFloat(UINT32 nAddress, float &fData)
{
	try
	{
		if (nAddress < 5000)
		{
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;
		}

		INT32 nResult = 0;
		UINT32 nReadData = 0;


		nResult = MXP_PLC_ReadSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_F, (DWORD&)nReadData);
		
		UINT8 arrByte[4];
		memcpy(arrByte, &nReadData, 4);

		memcpy(&fData, arrByte, sizeof(fData));

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_ReadUInt64(UINT32 nAddress, UINT64 &nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		UINT64 nReadData;

		nResult = MXP_PLC_ReadSystemRegisterEx(nAddress, 0, MXP_EasyClass::DATA_TYPE_D, nReadData);

		UINT8 arrByte[8];
		memcpy(&arrByte, &nReadData, 8);

		memcpy(&nData, arrByte, sizeof(nData));

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_ReadInt64(UINT32 nAddress, INT64 &nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;


		INT32 nResult;
		UINT64 nReadData;

		nResult = MXP_PLC_ReadSystemRegisterEx(nAddress, 0, MXP_EasyClass::DATA_TYPE_D, nReadData);

		UINT8 arrByte[8];
		memcpy(&arrByte, &nReadData, 8);

		memcpy(&nData, arrByte, sizeof(nData));

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_ReadDouble(UINT32 nAddress, double &dData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		UINT64 nReadData;

		nResult = MXP_PLC_ReadSystemRegisterEx(nAddress, 0, MXP_EasyClass::DATA_TYPE_D, nReadData);

		UINT8 arrByte[8];
		memcpy(&arrByte, &nReadData, 8);

		memcpy(&dData, arrByte, sizeof(dData));

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}	

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_WriteBit(UINT32 nAddress, UINT8 nBitNo, bool &bData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		DWORD nWriteData = (bData ==TRUE) ? 1 : 0;


		nResult = MXP_PLC_WriteSystemRegister(nAddress, nBitNo, MXP_EasyClass::DATA_TYPE_Bit, nWriteData);


		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_WriteByte(UINT32 nAddress, UINT8 nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		DWORD nWriteData = (DWORD)nData;
		nResult = MXP_PLC_WriteSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_Byte, nWriteData);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_WriteSByte(UINT32 nAddress, INT8 nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		DWORD nWriteData = (DWORD)nData;

		UINT8 arrByte[1];
		memcpy(&arrByte, &nData, 1);

		memcpy(arrByte, &nWriteData, 4);

		nResult = MXP_PLC_WriteSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_Byte, nWriteData);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_WriteUInt16(UINT32 nAddress, UINT16 nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		DWORD nWriteData;

		UINT8 arrByte[2];
		memcpy(arrByte, &nData, 2);

		UINT8 arrByte_re[4] = {0,};
		memcpy(arrByte_re, &arrByte, 4);
		nWriteData = byteToUINT32(arrByte_re);

		nResult = MXP_PLC_WriteSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_Word, nWriteData);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_WriteInt16(UINT32 nAddress, INT16 nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		DWORD nWriteData = 0;
		
		UINT8 arrByte[2];
		memcpy(arrByte, &nData, 2);

		nWriteData = byteToUINT32(arrByte);

		nResult = MXP_PLC_WriteSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_Word, nWriteData);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_WriteUInt32(UINT32 nAddress, UINT32 nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		DWORD nWriteData;
		nWriteData = nData;

		nResult = MXP_PLC_WriteSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_L, nWriteData);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_WriteInt32(UINT32 nAddress, INT32 nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		DWORD nWriteData = 0;

		UINT8 arrByte[4];
		memcpy(arrByte, &nData, 4);

		nWriteData = byteToUINT32(arrByte);

		nResult = MXP_PLC_WriteSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_L, nWriteData);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_WriteFloat(UINT32 nAddress, float nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		DWORD nWriteData = 0;

		UINT8 arrByte[4];
		memcpy(arrByte, &nData, 4);

		nWriteData = byteToUINT32(arrByte);

		nResult = MXP_PLC_WriteSystemRegister(nAddress, 0, MXP_EasyClass::DATA_TYPE_F, nWriteData);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_WriteUInt64(UINT32 nAddress, UINT64 nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		UINT64 nWriteData = 0;

		UINT8 arrByte[8];
		memcpy(arrByte, &nData, 8);

		nWriteData = byteToUINT64(arrByte);

		nResult = MXP_PLC_WriteSystemRegisterEx(nAddress, 0, MXP_EasyClass::DATA_TYPE_D, (ULONGLONG)nWriteData);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_WriteInt64(UINT32 nAddress, INT64 nData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		UINT64 nWriteData = 0;

		UINT8 arrByte[8];
		memcpy(arrByte, &nData, 8);

		nWriteData = byteToUINT64(arrByte);

		nResult = MXP_PLC_WriteSystemRegisterEx(nAddress, 0, MXP_EasyClass::DATA_TYPE_D, (ULONGLONG)nWriteData);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::PLC_WriteDouble(UINT32 nAddress, double dData)
{
	try
	{
		if (nAddress < 5000)
			return MXP_EasyClass::RET_INVAILD_PLCADDRESS;

		INT32 nResult;
		UINT64 nWriteData = 0;

		UINT8 arrByte[8];
		memcpy(arrByte, &dData, 8);

		memcpy(&nWriteData, arrByte, sizeof(nWriteData));

		nResult = MXP_PLC_WriteSystemRegisterEx(nAddress, 0, MXP_EasyClass::DATA_TYPE_D, (ULONGLONG)nWriteData);
		
		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

#pragma endregion PLC Iterface

#pragma region
MXP_EasyClass::MXP_FUNCTION_STATUS   MXP_EasyClass::AX_MultiAxisCCCSet(UINT16 nAxisCount, UINT32 *arrAxis, UINT16 nMode)
{
	try
	{

		UINT32 nAxisNo = arrAxis[nAxisCount-1];
		UINT32 nMotionIndex = GetMotionIndex(nAxisNo, MXP_EasyClass::e_CCCSet);
		MXP_MULTIAXISCOUPLESET_IN stIn;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;
		for (INT j =0; j < 10; j++)
		{
			stIn.ArrayAxisNo[j] = arrAxis[j];
		}
		stIn.AxisCount = nAxisCount;
		stIn.Mode = nMode;		

		stIn.Execute = 0;
		nResult = MXP_MultiAxisCCCSetCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_MultiAxisCCCSetCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS   MXP_EasyClass::AX_MultiAxisCCCReset(UINT16 nAxisCount, UINT32 *arrAxis)
{
	try
	{
		UINT32 nAxisNo = arrAxis[nAxisCount-1];
		UINT32 nMotionIndex = GetMotionIndex(nAxisNo, MXP_EasyClass::e_CCCReset); 
		MXP_MULTIAXISCOUPLERESET_IN stIn;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;

		for (INT j =0; j < 10; j++)
		{
			stIn.ArrayAxisNo[j] = arrAxis[j];
		}
		stIn.AxisCount = nAxisCount;

		stIn.Execute = 0;
		nResult = MXP_MultiAxisCCCReSetCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_MultiAxisCCCReSetCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS   MXP_EasyClass::AX_MultiAxisCCCGainSet(UINT16 nAxisCount, UINT32 *arrAxis, 
																			MXP_MULTIAXISCOUPLE_SINGLEAXISGAIN *arrSingleAxisGain, 
																			MXP_MULTIAXISCOUPLE_CCCGAIN *arrCCCGain, UINT16 nMultiContolKffGain)
{
	try
	{
		UINT32 nAxisNo = arrAxis[nAxisCount-1];
		UINT32 nMotionIndex = GetMotionIndex(nAxisNo, MXP_EasyClass::e_CCCGainSet);
		MXP_MULTIAXISCOUPLEGAINSET_IN stIn;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_ERROR_FAIL_INDEX_CHECK;
		for (INT j =0; j < 10; j++)
		{
			stIn.ArrayAxisNo[j] = arrAxis[j];
		}

		stIn.ArraySingleAxisGain[10]; 
		stIn.ArrayCCCGain[10];
		for(int i =0; i<nAxisCount; i++)
		{
			stIn.ArraySingleAxisGain[i] = arrSingleAxisGain[i].stData; 
			stIn.ArrayCCCGain[i] = arrCCCGain[i].stData;
		}
		stIn.AxisCount = nAxisCount;
		stIn.MultiControlKffGain = nMultiContolKffGain;	

		stIn.Execute = 0;
		nResult = MXP_MultiAxisCCCGainSetCmd(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_MultiAxisCCCGainSetCmd(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS   MXP_EasyClass::AX_MultiAxisCCCSetCheck(UINT16 nAxisCount, UINT32 *arrAxis, PROCESS_CHECK &stStatus)
{
	try
	{
		UINT32 nAxisNo = arrAxis[nAxisCount-1];
		UINT32 nMotionIndex = GetMotionIndexCheck(nAxisNo, MXP_EasyClass::e_CCCSet);
		MXP_MULTIAXISCOUPLESET_OUT stOut;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		nResult = MXP_GetMultiAxisCCCSetOutParam(nMotionIndex, stOut);

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Done == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;


		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS   MXP_EasyClass::AX_MultiAxisCCCResetCheck(UINT16 nAxisCount, UINT32 *arrAxis, PROCESS_CHECK &stStatus)
{
	try
	{
		UINT32 nAxisNo = arrAxis[nAxisCount-1];
		UINT32 nMotionIndex = GetMotionIndexCheck(nAxisNo, MXP_EasyClass::e_CCCReset);
		MXP_MULTIAXISCOUPLERESET_OUT stOut;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		nResult = MXP_GetMultiAxisCCCReSetOutParam(nMotionIndex, stOut);

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Done == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;


		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS   MXP_EasyClass::AX_MultiAxisCCCGainSetCheck(UINT16 nAxisCount, UINT32 *arrAxis, PROCESS_CHECK &stStatus)
{
	try
	{
		UINT32 nAxisNo = arrAxis[nAxisCount-1];
		UINT32 nMotionIndex = GetMotionIndexCheck(nAxisNo, MXP_EasyClass::e_CCCGainSet);
		MXP_MULTIAXISCOUPLEGAINSET_OUT stOut;

		INT32 nResult;

		if (nMotionIndex == 0)
			return MXP_EasyClass::RET_NO_ERROR;

		nResult = MXP_GetMultiAxisCCCGainSetOutParam(nMotionIndex, stOut);

		stStatus.ErrorID = stOut.ErrorID;
		stStatus.Busy = stOut.Busy == 1? TRUE : FALSE;
		stStatus.Done = stOut.Done == 1? TRUE : FALSE;
		stStatus.ErrorOn = stOut.Error == 1? TRUE : FALSE;


		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
#pragma endregion MutiAxis CCC

#pragma region
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_SequenceMove_File(string strFilePath, UINT16 nStartStep, UINT16 nEndStep, string &strFileReadError)
{
	try
	{
		UINT32 nAxisNo;
		std::vector<StSequenceMove> stData(0);
		TXTLoader TxtReader;
		JSONLoader JsonReader;
		nStartStep = 0;

		//File 확장자 구분
		string strSplitPath(strFilePath.substr(strFilePath.find_last_of(".") +1));

		if(strSplitPath == "txt")
		{
			if(TxtReader.AxisSequenceFileLoad(strFilePath, nAxisNo, stData, strFileReadError))
			{
				if(stData.size() < nStartStep)
				{					
					return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
				}
				if(stData.size() < nEndStep)
				{				
					return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
				}
				if(nEndStep !=0 && nStartStep > nEndStep)
				{					
					return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
				}

				AX_SequenceMove(nAxisNo, stData.size(), stData, nStartStep,nEndStep); ///
				return MXP_EasyClass::RET_NO_ERROR;
			}
			else
			{				
				return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
			}
		}
		else if (strSplitPath == "json")
		{
			if(JsonReader.AxisSequenceFileLoad(strFilePath, nAxisNo, stData, strFileReadError))
			{
				if(stData.size() < nStartStep)
				{					
					return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
				}
				if(stData.size() < nEndStep)
				{					
					return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
				}
				if(nEndStep !=0 && nStartStep > nEndStep)
				{					
					return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
				}
				AX_SequenceMove(nAxisNo, stData.size(), stData, nStartStep, nEndStep); ///
				return MXP_EasyClass::RET_NO_ERROR;
			}
			else
			{				
				return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
			}
		}
		else
		{			
			return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
		}
		return MXP_EasyClass::RET_NO_ERROR;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_SequenceMoveCheck(UINT32 nAxisNo, UINT16 &nCurStep, string &strCurStepName, 
																		UINT16 &nRemainCount,SEQUENCEMOVEPROCESS_CHECK &stStatus)
{
	nRemainCount = (mAxisSequenceData[nAxisNo].nData).size() - mAxisSequenceData[nAxisNo].nCurBlock;
	nCurStep = mAxisSequenceData[nAxisNo].nCurBlock;
	stStatus = mAxisSequenceData[nAxisNo].nState;
	INT32 nIndex;
	if(nCurStep == 0)
		nIndex = 0;
	else
		nIndex = nCurStep -1;
	strCurStepName = mAxisSequenceData[nAxisNo].nData[nIndex].strStepName;

	return MXP_EasyClass::RET_NO_ERROR;

}
MXP_EasyClass::MXP_FUNCTION_STATUS  MXP_EasyClass::AX_SequenceMoveRequest(UINT32 nAxisNo, MXP_SequenceMoveMode nMoveMode, MXP_BUFFERMODE_ENUM nBufferedMode, std::vector<StSequenceMoveStep> &arrData, float fAcc, float fDec, float fJerk)
{
	std::vector<StSequenceMove> nStepData(arrData.size());
	ESequenceCmdType nCmdType;

	if(nBufferedMode == MXP_ABORTING || nBufferedMode == MXP_SINGLE_BLOCK)
		return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;

	if(nMoveMode == e_Absolute)
		nCmdType = e_AbsMove;
	else
		nCmdType = e_RelativeMove;

	for (int nIndex = 0; nIndex < (int)arrData.size(); nIndex++)
	{
		nStepData[nIndex].nCmdType = nCmdType;
		nStepData[nIndex].fPos = arrData[nIndex].fPos;
		nStepData[nIndex].fVel = arrData[nIndex].fVel;
		nStepData[nIndex].fAcc = fAcc;
		nStepData[nIndex].fDec = fDec;
		nStepData[nIndex].fJerk = fJerk;
		nStepData[nIndex].nBufferMode = nBufferedMode;
		nStepData[nIndex].strStepName = "Step" + intToString(nIndex + 1);
	}
	
	AX_SetUsingPositionSensor(nAxisNo, false);

	return AX_SequenceMove(nAxisNo, arrData.size(), nStepData, 0, 0);
}

MXP_EasyClass::MXP_FUNCTION_STATUS  MXP_EasyClass::AX_SequenceMoveRequest(UINT32 nAxisNo, MXP_SequenceMoveMode nMoveMode, MXP_BUFFERMODE_ENUM nBufferedMode, 
																		std::vector<StSequenceMoveStep> &arrData, float fAcc, float fDec, float fJerk, bool bPositionSensorUising)
{
	std::vector<StSequenceMove> nStepData(arrData.size());
	ESequenceCmdType nCmdType;
	MXP_FUNCTION_STATUS nResult;

	if(mAxisSequenceData[nAxisNo].bRunFlag)
		return RET_SEQUENCEMOVE_INVAILDSTATE; //동작 지령을 주는 경우

	if(nBufferedMode == MXP_ABORTING || nBufferedMode == MXP_SINGLE_BLOCK)
		return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;

	if(nMoveMode == e_Absolute)
		nCmdType = e_AbsMove;
	else
		nCmdType = e_RelativeMove;

	std::vector<StSequenceMoveStep> arrOutData;
	
	if (bPositionSensorUising)
	{
		//nStepData.resize(arrData.size()+1);
		//arrOutData.resize(arrData.size()+1);
		//nResult = AX_SequenceMoveLastStepVelocityChange(nAxisNo, nMoveMode, arrData, arrOutData);
		//if(nResult != 0)
		//	return (MXP_FUNCTION_STATUS)nResult;
		
		nResult = AX_FeedBackSensorRun(nAxisNo, ePLCFBRunMode_Reset); // Feedback 센서 모니터링 FB Reset
		if(nResult != 0)
			return (MXP_FUNCTION_STATUS)nResult;
	}
	//else
	//{
		//nStepData.resize(arrData.size());
		//arrOutData.resize(arrData.size());
		//arrOutData = arrData;
	//}
	nStepData.resize(arrData.size());
	arrOutData.resize(arrData.size());

	arrOutData = arrData;

	AX_SetUsingPositionSensor(nAxisNo, bPositionSensorUising);

	for (int nIndex = 0; nIndex < (int)arrData.size(); nIndex++)
	{
		nStepData[nIndex].nCmdType = nCmdType;
		nStepData[nIndex].fPos = arrData[nIndex].fPos;
		nStepData[nIndex].fVel = arrData[nIndex].fVel;
		nStepData[nIndex].fAcc = fAcc;
		nStepData[nIndex].fDec = fDec;
		nStepData[nIndex].fJerk = fJerk;
		nStepData[nIndex].nBufferMode = nBufferedMode;
		nStepData[nIndex].strStepName = "Step" + intToString(nIndex + 1);
	}

	return AX_SequenceMove(nAxisNo, arrData.size(), nStepData, 0, 0);
}
//MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_SequenceMoveLastStepVelocityChange(UINT32 nAxisNo, MXP_SequenceMoveMode nMoveMode, 
//																						std::vector<StSequenceMoveStep> &arrInData, std::vector<StSequenceMoveStep> &arrOutData)
//{
//	MXP_FUNCTION_STATUS nResult;
//	int nCmdSequenceMoveCount = arrInData.size() -1;
//	int nSequenceMoveCount = nCmdSequenceMoveCount; // Feedback 사용인 경우 1Step 이 내부적으로 증가함
//
//	if (mFeedbackSensorParam[nAxisNo].bUsing == false)
//	{
//		arrOutData = arrInData;
//		return RET_FEEDBACKSENSOR_PARAM_INVAILD;
//	}
//
//	if(mFeedbackSensorParam[nAxisNo].bUsing)
//		nSequenceMoveCount = nCmdSequenceMoveCount +1; //Feedback 사용시
//
//	vector<StSequenceMoveStep> arrSequenceData(nSequenceMoveCount);
//	for(int nStepCount = 0; nStepCount < nCmdSequenceMoveCount; nStepCount++)
//	{
//		arrSequenceData[nStepCount] = arrInData[nStepCount];
//	}
//
//	vector<StSequenceMove> nStepData(nSequenceMoveCount);
//	float fCalCmdLastStepMoveDist; // 마지막 구간 이송 거리
//	float fCalCmdLastStepMoveRemain;
//
//	float fActualPosition;
//	nResult = AX_ReadActualPosition(nAxisNo, fActualPosition);
//
//	if (nResult != RET_NO_ERROR)
//		return (MXP_FUNCTION_STATUS)nResult;
//	if(nMoveMode == e_Absolute)
//	{
//		if(nCmdSequenceMoveCount == 0)
//			fCalCmdLastStepMoveDist = arrSequenceData[nCmdSequenceMoveCount].fPos - fActualPosition;
//		else
//			fCalCmdLastStepMoveDist = arrSequenceData[nCmdSequenceMoveCount].fPos - arrSequenceData[nCmdSequenceMoveCount-1].fPos;
//	}
//	else
//		fCalCmdLastStepMoveDist = arrSequenceData[nCmdSequenceMoveCount].fPos;
//
//	fCalCmdLastStepMoveRemain = abs(fCalCmdLastStepMoveDist) - mFeedbackSensorParam[nAxisNo].fTargetPosCalDist;
//
//	//마지막 이송 스텝 거리가 감속 거리보다 작은경우
//	if(fCalCmdLastStepMoveRemain <= 0)
//		return RET_FEEDBACKSENSOR_MOVE_LASTSTEP_DIST_INVALID;
//	if(AX_ValidCheckUsingFeedBackSensorCtrl(nAxisNo, fCalCmdLastStepMoveDist) == false)
//		return RET_SEQUENCEMOVE_READ_FAIL;
//
//	if(mFeedbackSensorParam[nAxisNo].bUsing) // Feedback 사용시
//	{
//		if (fCalCmdLastStepMoveDist < 0) //- 방향 이송시
//		{
//			if (nMoveMode = e_Absolute)
//			{
//				if (nCmdSequenceMoveCount == 0) //1Step 구동시
//				{
//					arrSequenceData[nSequenceMoveCount].fPos = arrSequenceData[nCmdSequenceMoveCount].fPos; // 최종 감속 Step Target
//					arrSequenceData[nCmdSequenceMoveCount].fPos = fActualPosition - fCalCmdLastStepMoveRemain; // 감속전 Step Target
//				}
//				else
//				{
//					arrSequenceData[nSequenceMoveCount].fPos = arrSequenceData[nCmdSequenceMoveCount].fPos; // 최종 감속 Step Target
//					arrSequenceData[nCmdSequenceMoveCount].fPos = arrSequenceData[nCmdSequenceMoveCount-1].fPos - fCalCmdLastStepMoveRemain; // 감속전 Step Target
//				}
//			}
//			else
//			{
//				arrSequenceData[nCmdSequenceMoveCount].fPos = fCalCmdLastStepMoveRemain * -1; //감속전 Step Target
//				arrSequenceData[nSequenceMoveCount].fPos = mFeedbackSensorParam[nAxisNo].fTargetPosCalDist * -1; // 최종 감속 Step Target
//			}
//
//			mFeedbackSensorParam[nAxisNo].nDirection = MXP_NEGATIVE_DIRECTION;
//		}
//		else // + 방향 이송 시
//		{
//			if (nMoveMode = e_Absolute)
//			{
//				if (nCmdSequenceMoveCount == 0) //1Step 구동시
//				{
//					arrSequenceData[nSequenceMoveCount].fPos = arrSequenceData[nCmdSequenceMoveCount].fPos; // 최종 감속 Step Target
//					arrSequenceData[nCmdSequenceMoveCount].fPos = fActualPosition + fCalCmdLastStepMoveRemain; // 감속전 Step Target
//				}
//				else
//				{
//					arrSequenceData[nSequenceMoveCount].fPos = arrSequenceData[nCmdSequenceMoveCount].fPos; // 최종 감속 Step Target
//					arrSequenceData[nCmdSequenceMoveCount].fPos = arrSequenceData[nCmdSequenceMoveCount-1].fPos + fCalCmdLastStepMoveRemain; // 감속전 Step Target
//				}
//			}
//			else
//			{
//				arrSequenceData[nCmdSequenceMoveCount].fPos = fCalCmdLastStepMoveRemain; //감속전 Step Target
//				arrSequenceData[nSequenceMoveCount].fPos = mFeedbackSensorParam[nAxisNo].fTargetPosCalDist; // 최종 감속 Step Target
//			}
//
//			mFeedbackSensorParam[nAxisNo].nDirection = MXP_POSITIVE_DIRECTION;
//		}
//	}
//
//	//감속 위치 연산
//	/*if (nMoveMode == e_Absolute)
//	{
//		if (fCalCmdLastStepMoveDist < 0)
//			mFeedbackSensorParam[nAxisNo].fOverridePos = arrSequenceData[nSequenceMoveCount].fPos + mFeedbackSensorParam[nAxisNo].fOverrideDist;
//		else
//			mFeedbackSensorParam[nAxisNo].fOverridePos = arrSequenceData[nSequenceMoveCount].fPos - mFeedbackSensorParam[nAxisNo].fOverrideDist;
//	}
//	else
//	{
//		mFeedbackSensorParam[nAxisNo].fOverridePos = fActualPosition;
//
//		for (int i = 0; i <= nSequenceMoveCount; i++)
//		{
//			mFeedbackSensorParam[nAxisNo].fOverridePos = mFeedbackSensorParam[nAxisNo].fOverridePos + arrSequenceData[i].fPos;
//		}
//
//		if (fCalCmdLastStepMoveDist < 0)
//			mFeedbackSensorParam[nAxisNo].fOverridePos = mFeedbackSensorParam[nAxisNo].fOverridePos + mFeedbackSensorParam[nAxisNo].fOverrideDist;
//		else
//			mFeedbackSensorParam[nAxisNo].fOverridePos = mFeedbackSensorParam[nAxisNo].fOverridePos - mFeedbackSensorParam[nAxisNo].fOverrideDist;
//	}*/
//
//	arrOutData = arrSequenceData;
//	return RET_NO_ERROR;
//}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_SequenceMove_File(string strFilePath, UINT16 nStartStep, UINT16 nEndStep, string &strErrorMessage)
{
	try
	{
		UINT32 nGroupNo;
		std::vector<StGroupSequenceMove> stData(0);
		TXTLoader TxtReader;
		JSONLoader JsonReader;
		nStartStep = 0;
		//File 확장자 구분
		string strSplitPath(strFilePath.substr(strFilePath.find_last_of(".") +1));

		if(strSplitPath == "txt")
		{
			if(TxtReader.GroupSequenceFileLoad(strFilePath, nGroupNo, stData, strErrorMessage))
			{

				if(stData.size() < nStartStep)
				{
					
					return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
				}
				if(stData.size() < nEndStep)
				{
					
					return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
				}
				if(nEndStep !=0 && nStartStep > nEndStep)
				{
					
					return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
				}
				
				GRP_SequenceMove(nGroupNo, stData.size(), stData, nStartStep, nEndStep); ///
				return MXP_EasyClass::RET_NO_ERROR;
			}
			else
			{
				
				return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
			}
		}
		else if (strSplitPath == "json")
		{
			if(JsonReader.GroupSequenceFileLoad(strFilePath, nGroupNo, stData, strErrorMessage))
			{

				if(stData.size() < nStartStep)
				{
					
					return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
				}
				if(stData.size() < nEndStep)
				{
					
					return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
				}
				if(nEndStep !=0 && nStartStep > nEndStep)
				{
					
					return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
				}

				GRP_SequenceMove(nGroupNo, stData.size(), stData, nStartStep, nEndStep); ///
				return MXP_EasyClass::RET_NO_ERROR;
			}
			else
			{
				
				return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
			}
		}
		else
		{
			
			return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
		}

		return MXP_EasyClass::RET_NO_ERROR;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_SEQUENCEMOVE_READ_FAIL;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_SequenceMoveCheck(UINT32 nAxesGroup, UINT16 &nCurStep, string &strCurStepName, 
																		UINT16 &nRemainCount, SEQUENCEMOVEPROCESS_CHECK &stStatus)
{
		nRemainCount = (mGroupSequenceData[nAxesGroup].nData).size()- mGroupSequenceData[nAxesGroup].nCurBlock;
		nCurStep = mGroupSequenceData[nAxesGroup].nCurBlock;
		stStatus = mGroupSequenceData[nAxesGroup].nState;
		INT32 nIndex;
		if(nCurStep == 0)
			nIndex = 0;
		else
			nIndex = nCurStep -1;
		strCurStepName = mGroupSequenceData[nAxesGroup].nData[nIndex].strStepName;

		return MXP_EasyClass::RET_NO_ERROR;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_BufferedIO_Ex(UINT32 nAxisNo, UINT16 nCommandBlockNo, UINT16 nSlaveNo, UINT16 nBitPos, bool bBitValue)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_BUFFEREDDIGITALIO_EX_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_EasyClass::MXP_BUFFERED;
		stIn.SlaveNo = nSlaveNo;
		stIn.BitPosition = nBitPos;
		stIn.BitValue = (bBitValue == TRUE)? 1 : 0;
		stIn.CommandBlockNo = nCommandBlockNo;

		stIn.Execute = 0;
		nResult = MXP_BufferedDigitalioCmd_Ex(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_BufferedDigitalioCmd_Ex(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_Dwell_Ex(UINT32 nAxisNo, UINT16 nCommandBlockNo, float fTime)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_DWELL_EX_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_EasyClass::MXP_BUFFERED;
		stIn.TimeValue = fTime;
		stIn.CommandBlockNo = nCommandBlockNo;

		stIn.Execute = 0;
		nResult = MXP_DwellCmd_Ex(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_DwellCmd_Ex(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS  MXP_EasyClass::GRP_BufferedIO_Ex(UINT32 nAxesGroup, UINT16 nCommandBlockNo, UINT16 nSlaveNo, UINT16 nBitPos, bool bBitValue)
{
	try
	{
		UINT32 nMotionIndex = MC_GROUPAXIS_INDEX + nAxesGroup;
		MXP_GROUPBUFFEREDDIGITALIO_EX_IN stIn;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_EasyClass::MXP_BUFFERED;
		stIn.SlaveNo = nSlaveNo;
		stIn.BitPosition = nBitPos;
		stIn.BitValue = (bBitValue == TRUE)? 1 : 0;
		stIn.CommandBlockNo = nCommandBlockNo;

		stIn.Execute = 0;
		nResult = MXP_GroupBufferedDigitalioCmd_Ex(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_GroupBufferedDigitalioCmd_Ex(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS  MXP_EasyClass::GRP_Dwell_Ex(UINT32 nAxesGroup, UINT16 nCommandBlockNo, float fTime)
{
	try
	{
		UINT32 nMotionIndex = MC_GROUPAXIS_INDEX + nAxesGroup;
		MXP_GROUPDWELL_EX_IN stIn;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)MXP_EasyClass::MXP_BUFFERED;
		stIn.TimeValue = fTime;
		stIn.CommandBlockNo = nCommandBlockNo;

		stIn.Execute = 0;
		nResult = MXP_GroupDwellCmd_Ex(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_GroupDwellCmd_Ex(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_MoveAbsolute_Ex(UINT32 nAxisNo, UINT16 nCommandBlockNo, float fPosition, float fVel, 
																	 float fAcc, float fDec, float fJerk,
																	 MXP_DIRECTION_ENUM nDirection, MXP_BUFFERMODE_ENUM nBufferMode)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_MOVEABSOLUTE_EX_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Position = fPosition;
		stIn.Velocity = fVel;
		stIn.Acceleration = fAcc;
		stIn.Deceleration = fDec;
		stIn.Jerk = fJerk;
		stIn.Direction = (::MXP_DIRECTION_ENUM)nDirection;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;
		stIn.CommandBlockNo = nCommandBlockNo;

		stIn.Execute = 0;
		nResult = MXP_MoveAbsoluteCmd_Ex(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_MoveAbsoluteCmd_Ex(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_MoveRelative_Ex(UINT32 nAxisNo, UINT16 nCommandBlockNo, float fDistance, float fVel,
																	 float fAcc, float fDec, float fJerk, MXP_BUFFERMODE_ENUM nBufferMode)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		MXP_MOVERELATIVE_EX_IN stIn;

		INT32 nResult;

		stIn.Axis.AxisNo = nAxisNo;
		stIn.Distance = fDistance;
		stIn.Velocity = fVel;
		stIn.Acceleration = fAcc;
		stIn.Deceleration = fDec;
		stIn.Jerk = fJerk;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;
		stIn.CommandBlockNo = nCommandBlockNo;

		stIn.Execute = 0;
		nResult = MXP_MoveRelativeCmd_Ex(nMotionIndex, &stIn);
		stIn.Execute = 1;
		nResult = MXP_MoveRelativeCmd_Ex(nMotionIndex, &stIn);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_MoveLinearAbsolute_Ex(UINT32 nAxesGroup, UINT16 nCommandBlockNo, MXP_EasyClass::GROUP_POS stPosition, float fVelocity,
																			float fAcceleration, float fDeceleration, float fJerk, MXP_BUFFERMODE_ENUM nBufferMode)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::MC_GROUPAXIS_INDEX + nAxesGroup;
		MXP_MOVELINEARABSOLUTE_EX_IN stIn;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;
		stIn.PositionX = stPosition.nX;
		stIn.PositionY = stPosition.nY;
		stIn.PositionZ = stPosition.nZ;
		stIn.PositionU = stPosition.nU;
		stIn.PositionV = stPosition.nV;
		stIn.PositionW = stPosition.nW;
		stIn.PositionA = stPosition.nA;
		stIn.PositionB = stPosition.nB;
		stIn.PositionC = stPosition.nC;
		stIn.Velocity = fVelocity;
		stIn.Acceleration = fAcceleration;
		stIn.Deceleration = fDeceleration;
		stIn.Jerk = fJerk;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;
		stIn.CommandBlockNo = nCommandBlockNo;


		stIn.Execute = 0;
		nResult = MXP_MoveLinearAbsoluteCmd_Ex(nMotionIndex,&stIn);

		stIn.Execute = 1;
		nResult = MXP_MoveLinearAbsoluteCmd_Ex(nMotionIndex,&stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_MoveLinearRelative_Ex(UINT32 nAxesGroup, UINT16 nCommandBlockNo, MXP_EasyClass::GROUP_POS stDistance, float fVelocity,
																			float fAcceleration, float fDeceleration, float fJerk, MXP_BUFFERMODE_ENUM nBufferMode)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::MC_GROUPAXIS_INDEX + nAxesGroup;
		MXP_MOVELINEARRELATIVE_EX_IN stIn;

		INT32 nResult;

		stIn.AxesGroup.AxesGroup = nAxesGroup;
		stIn.DistanceX = stDistance.nX;
		stIn.DistanceY = stDistance.nY;
		stIn.DistanceZ = stDistance.nZ;
		stIn.DistanceU = stDistance.nU;
		stIn.DistanceV = stDistance.nV;
		stIn.DistanceW = stDistance.nW;
		stIn.DistanceA = stDistance.nA;
		stIn.DistanceB = stDistance.nB;
		stIn.DistanceC = stDistance.nC;
		stIn.Velocity = fVelocity;
		stIn.Acceleration = fAcceleration;
		stIn.Deceleration = fDeceleration;
		stIn.Jerk = fJerk;
		stIn.BufferMode = (::MXP_BUFFERMODE_ENUM)nBufferMode;
		stIn.CommandBlockNo = nCommandBlockNo;


		stIn.Execute = 0;
		nResult = MXP_MoveLinearRelativeCmd_Ex(nMotionIndex,&stIn);

		stIn.Execute = 1;
		nResult = MXP_MoveLinearRelativeCmd_Ex(nMotionIndex,&stIn);

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_GetAxisBufferInfo(UINT32 nAxisNo, UINT16 &nSaveNum, UINT16 &nReadNum, UINT16 &nCurBlock)
{
	try
	{
		INT32 nResult = MXP_ReadAxisBufferInfo(nAxisNo, nSaveNum, nReadNum, nCurBlock);
		if(mAxisSequenceData[nAxisNo].nSendCount == 0)
			nCurBlock = 0;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::SYS_GetGroupBufferInfo(UINT32 nAxesGroup, UINT16 &nSaveNum, UINT16 &nReadNum, UINT16 &nCurBlock)
{
	try
	{
		INT32 nResult = MXP_ReadGroupBufferInfo(nAxesGroup, nSaveNum, nReadNum, nCurBlock);
		if(mGroupSequenceData[nAxesGroup].nSendCount == 0)
			nCurBlock = 0;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_SequenceBufferState MXP_EasyClass::AX_SequenceBufferStateCheck(UINT32 nAxisNo)
{
	bool bAxisReadCheck;
	MXP_AxisStateBit nAxisState;
	AX_ReadyCheck(nAxisNo, bAxisReadCheck);
	AX_ReadStatus(nAxisNo, nAxisState);

	//Fail Case
	if(nAxisState.Disable || nAxisState.ErrorStop || nAxisState.PowerOn == FALSE || nAxisState.Stopping || mAxisSequenceData[nAxisNo].nState.ErrorOn )
		return SequenceBuffer_Fail;
	//comp case
	if (mAxisSequenceData[nAxisNo].nCurBlock == mAxisSequenceData[nAxisNo].nSendCount && bAxisReadCheck)
		return SequenceBuffer_Complete;

	if(mAxisSequenceData[nAxisNo].nSendCount == 0)
		return SequenceBuffer_Null;

	return SequenceBuffer_Runing;
}
MXP_EasyClass::MXP_SequenceBufferState MXP_EasyClass::GRP_SequenceBufferStateCheck(UINT32 nAxesGroup)
{
	bool bAxisReadCheck;
	MXP_AxisStateBit nGroupState;
	GRP_ReadyCheck(nAxesGroup, bAxisReadCheck);
	GRP_ReadStatus(nAxesGroup, nGroupState);

	//Fail Case
	if(nGroupState.Disable || nGroupState.ErrorStop || nGroupState.PowerOn == FALSE || nGroupState.Stopping || mGroupSequenceData[nAxesGroup].nState.ErrorOn )
		return SequenceBuffer_Fail;
	//comp case
	if (mGroupSequenceData[nAxesGroup].nCurBlock == mGroupSequenceData[nAxesGroup].nSendCount && bAxisReadCheck)
		return SequenceBuffer_Complete;

	if(mGroupSequenceData[nAxesGroup].nSendCount == 0)
		return SequenceBuffer_Null;

	return SequenceBuffer_Runing;
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_SequenceMove(UINT32 nAxisNo, UINT16 nSequenceCount, std::vector<StSequenceMove> &arrData, UINT16 nStartStep, UINT16 nEndStep)
{
	try
	{
		UINT32 nMotionIndex = MC_SINGLEAXIS_INDEX + nAxisNo;
		INT32 nResult = 0;

		if (mAxisSequenceData[nAxisNo].bRunFlag)
			return MXP_EasyClass::RET_SEQUENCEMOVE_INVAILDSTATE;

		mAxisSequenceData[nAxisNo].bRunFlag = TRUE;
		mAxisSequenceData[nAxisNo].nSendCount = 0;
		mAxisSequenceData[nAxisNo].nState.Busy = TRUE;
		mAxisSequenceData[nAxisNo].nState.Done = FALSE;
		mAxisSequenceData[nAxisNo].nState.ErrorOn = FALSE;
		mAxisSequenceData[nAxisNo].nState.ErrorID = 0;
		mAxisSequenceData[nAxisNo].nCurBlock = 0;
		mAxisSequenceData[nAxisNo].nData = arrData;
		mAxisSequenceData[nAxisNo].nStartStep = nStartStep;
		mAxisSequenceData[nAxisNo].nEndStep = nEndStep;


		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_SequenceMoveCmdSet(UINT32 nAxisNo, UINT16 nStartIndex, UINT16 nEndIndex)
{
	INT32 i;
	MXP_FUNCTION_STATUS nResult;

	for (i = nStartIndex; i < nEndIndex; i++)
	{
		UINT16 nBlockIndex = i+1;

		StSequenceMove nCurCmd = mAxisSequenceData[nAxisNo].nData[i];

		if (mAxisSequenceData[nAxisNo].nData[i].nCmdType == MXP_EasyClass::e_AbsMove)
			nResult = AX_MoveAbsolute_Ex(nAxisNo, nBlockIndex, nCurCmd.fPos, nCurCmd.fVel, nCurCmd.fAcc, nCurCmd.fDec, nCurCmd.fJerk, nCurCmd.nDirection, nCurCmd.nBufferMode);
		else if (mAxisSequenceData[nAxisNo].nData[i].nCmdType == MXP_EasyClass::e_RelativeMove)
			nResult = AX_MoveRelative_Ex(nAxisNo, nBlockIndex, nCurCmd.fPos, nCurCmd.fVel, nCurCmd.fAcc, nCurCmd.fDec, nCurCmd.fJerk, nCurCmd.nBufferMode);
		else if (mAxisSequenceData[nAxisNo].nData[i].nCmdType == MXP_EasyClass::e_IO)
			nResult = AX_BufferedIO_Ex(nAxisNo, nBlockIndex, nCurCmd.nIOSlaveNo, nCurCmd.nIOBitNo, nCurCmd.bIOBitSet);
		else if (mAxisSequenceData[nAxisNo].nData[i].nCmdType == MXP_EasyClass::e_Dwell)
			nResult = AX_Dwell_Ex(nAxisNo, nBlockIndex, nCurCmd.fDwellTime);

		if(nResult != 0)
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;

		mAxisSequenceData[nAxisNo].nSendCount = nBlockIndex;
	}

	return MXP_EasyClass::RET_NO_ERROR;

}
bool MXP_EasyClass::AX_SequenceMoveSetVaildCheck(UINT32 nAxisNo, UINT16 nSequenceCount, bool bFirstStep)
{
	INT32 nMaxCount = 100;
	INT32 nVaildCount = 0;
	// Cmd 를 전송하면 nSaveNum 갱신시까지 시간 지연이 필요 한다.
	// 따라서 nSaveNum 대신 전송완료시 BufferCount 를 예측 한다.
	UINT16 nCountOffset = 0;

	if (mAxisSequenceData[nAxisNo].nStartStep != 0)
		nCountOffset = mAxisSequenceData[nAxisNo].nStartStep -1;

	UINT16 nCmdBufferUsingCount = (mAxisSequenceData[nAxisNo].nSendCount - nCountOffset) % nMaxCount;

	if(bFirstStep)
		nCmdBufferUsingCount = mAxisSequenceData[nAxisNo].nSaveNum;

	if(nCmdBufferUsingCount != mAxisSequenceData[nAxisNo].nSaveNum)
		return FALSE; // 이전 명령어가 미 전송 상태임.

	if (mAxisSequenceData[nAxisNo].nSaveNum == mAxisSequenceData[nAxisNo].nReadNum)
		nVaildCount = 100;
	else if (mAxisSequenceData[nAxisNo].nSaveNum > mAxisSequenceData[nAxisNo].nReadNum)
		nVaildCount = nMaxCount - mAxisSequenceData[nAxisNo].nSaveNum + mAxisSequenceData[nAxisNo].nReadNum;
	else if (mAxisSequenceData[nAxisNo].nSaveNum < mAxisSequenceData[nAxisNo].nReadNum)
		nVaildCount = mAxisSequenceData[nAxisNo].nReadNum - mAxisSequenceData[nAxisNo].nSaveNum;

	nVaildCount = nVaildCount -5; //혹시 몰라서 5개 spare 처리 함.
	if(nSequenceCount < nVaildCount)
		return TRUE;
	else
		return FALSE;

}
bool MXP_EasyClass::GRP_SequenceMoveSetVaildCheck(UINT32 nAxesGroup, UINT16 nSequenceCount, bool bFirstStep)
{
	INT32 nMaxCount = 100;
	INT32 nVaildCount;
	// Cmd 를 전송하면 nSaveNum 갱신시까지 시간 지연이 필요 한다.
	// 따라서 nSaveNum 대신 전송완료시 BufferCount 를 예측 한다.
	UINT16 nCountOffset = 0;

	if (mGroupSequenceData[nAxesGroup].nStartStep != 0)
		nCountOffset = mGroupSequenceData[nAxesGroup].nStartStep -1;

	UINT16 nCmdBufferUsingCount = (mGroupSequenceData[nAxesGroup].nSendCount - nCountOffset) % nMaxCount;

	if(bFirstStep)
		nCmdBufferUsingCount = mGroupSequenceData[nAxesGroup].nSaveNum;

	if(nCmdBufferUsingCount != mGroupSequenceData[nAxesGroup].nSaveNum)
		return FALSE; // 이전 명령어가 미 전송 상태임.

	if (mGroupSequenceData[nAxesGroup].nSaveNum == mGroupSequenceData[nAxesGroup].nReadNum)
		nVaildCount = 100;
	else if (mGroupSequenceData[nAxesGroup].nSaveNum > mGroupSequenceData[nAxesGroup].nReadNum)
		nVaildCount = nMaxCount - mGroupSequenceData[nAxesGroup].nSaveNum + mGroupSequenceData[nAxesGroup].nReadNum;
	else if (mGroupSequenceData[nAxesGroup].nSaveNum < mGroupSequenceData[nAxesGroup].nReadNum)
		nVaildCount = mGroupSequenceData[nAxesGroup].nReadNum - mGroupSequenceData[nAxesGroup].nSaveNum;

	nVaildCount = nVaildCount -5; //혹시 몰라서 5개 spare 처리 함.
	if(nSequenceCount < nVaildCount)
		return TRUE;
	else
		return FALSE;

}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_SequenceMove(UINT32 nAxesGroup, UINT16 nSequenceCount, std::vector<StGroupSequenceMove> &arrData, UINT16 nStartStep, UINT16 nEndStep)
{
	try
	{
		UINT32 nMotionIndex = MXP_EasyClass::MC_GROUPAXIS_INDEX + nAxesGroup;
		INT32 nResult = 0;

		if (mGroupSequenceData[nAxesGroup].bRunFlag)
			return MXP_EasyClass::RET_SEQUENCEMOVE_INVAILDSTATE;  // 동작중 지령을 주는 경우

		mGroupSequenceData[nAxesGroup].bRunFlag = TRUE;
		mGroupSequenceData[nAxesGroup].nSendCount = 0;
		mGroupSequenceData[nAxesGroup].nState.Busy = TRUE;
		mGroupSequenceData[nAxesGroup].nState.Done = FALSE;
		mGroupSequenceData[nAxesGroup].nState.ErrorOn = FALSE;
		mGroupSequenceData[nAxesGroup].nState.ErrorID = 0;
		mGroupSequenceData[nAxesGroup].nCurBlock = 0;
		mGroupSequenceData[nAxesGroup].nData = arrData;
		mGroupSequenceData[nAxesGroup].nStartStep = nStartStep;
		mGroupSequenceData[nAxesGroup].nEndStep = nEndStep;

		return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}

MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::GRP_SequenceMoveCmdSet(UINT32 nAxesGroup, UINT16 nStartIndex, UINT16 nEndIndex)
{
	INT32 i;
	MXP_FUNCTION_STATUS nResult;

	for (i = nStartIndex; i < nEndIndex; i++)
	{
		UINT16 nBlockIndex = i+1;
		StGroupSequenceMove nCurCmd = mGroupSequenceData[nAxesGroup].nData[i];
		if (mGroupSequenceData[nAxesGroup].nData[i].nCmdType == MXP_EasyClass::e_AbsMove)
		{
			GROUP_POS stPosition;
			stPosition.nX = nCurCmd.fXPos;
			stPosition.nY = nCurCmd.fYPos;
			stPosition.nZ = nCurCmd.fZPos;
			stPosition.nU = nCurCmd.fUPos;
			stPosition.nV = nCurCmd.fVPos;
			stPosition.nW = nCurCmd.fWPos;
			stPosition.nA = nCurCmd.fAPos;
			stPosition.nB = nCurCmd.fBPos;
			stPosition.nC = nCurCmd.fCPos;

			nResult = GRP_MoveLinearAbsolute_Ex(nAxesGroup, nBlockIndex, stPosition, nCurCmd.fVel, nCurCmd.fAcc, nCurCmd.fDec, nCurCmd.fJerk, nCurCmd.nBufferMode);
		}
		else if (mGroupSequenceData[nAxesGroup].nData[i].nCmdType == MXP_EasyClass::e_RelativeMove)
		{
			GROUP_POS stPosition;
			stPosition.nX = nCurCmd.fXPos;
			stPosition.nY = nCurCmd.fYPos;
			stPosition.nZ = nCurCmd.fZPos;
			stPosition.nU = nCurCmd.fUPos;
			stPosition.nV = nCurCmd.fVPos;
			stPosition.nW = nCurCmd.fWPos;
			stPosition.nA = nCurCmd.fAPos;
			stPosition.nB = nCurCmd.fBPos;
			stPosition.nC = nCurCmd.fCPos;

			nResult = GRP_MoveLinearRelative_Ex(nAxesGroup, nBlockIndex, stPosition, nCurCmd.fVel, nCurCmd.fAcc, nCurCmd.fDec, nCurCmd.fJerk, nCurCmd.nBufferMode);
		}
		else if (mGroupSequenceData[nAxesGroup].nData[i].nCmdType == MXP_EasyClass::e_IO)
		{	nResult = GRP_BufferedIO_Ex(nAxesGroup, nBlockIndex, nCurCmd.nIOSlaveNo, nCurCmd.nIOBitNo, nCurCmd.bIOBitSet);
		}
		else if (mGroupSequenceData[nAxesGroup].nData[i].nCmdType == MXP_EasyClass::e_Dwell)
		{	nResult = GRP_Dwell_Ex(nAxesGroup, nBlockIndex, nCurCmd.fDwellTime);
		}
		if(nResult != 0)
			return (MXP_EasyClass::MXP_FUNCTION_STATUS)nResult;
		
		mGroupSequenceData[nAxesGroup].nSendCount = nBlockIndex;
	}

	return MXP_EasyClass::RET_NO_ERROR;
}

void MXP_EasyClass::SEQ_AxisSequenceMoveManager(INT nAxisNo)
{
	int nSendMaxCount = 30;
	INT nSendCmdCount = 0;
	if (mAxisSequenceData[nAxisNo].bRunFlag)
		MXP_EasyClass::SYS_GetAxisBufferInfo(nAxisNo, mAxisSequenceData[nAxisNo].nSaveNum, mAxisSequenceData[nAxisNo].nReadNum, mAxisSequenceData[nAxisNo].nCurBlock);

	if(nAxis_TaskPos[nAxisNo] > 0 && nAxis_TaskPos[nAxisNo] <800 && mAxisSequenceData[nAxisNo].bRunFlag == FALSE)
		nAxis_TaskPos[nAxisNo] = 800;

	if(nAxis_TaskPos[nAxisNo] == 0)
	{
		bool bReadyCheck;
		if(mAxisSequenceData[nAxisNo].bRunFlag) 
		{
			// 명령 전 초기화
			mExAPIAxisParam[nAxisNo].bSequenceOverrideFlag = false;
			if (mFeedbackSensorParam[nAxisNo].bUsing)
			{
				nAxisSeq_Result[nAxisNo] = AX_FeedBackSensorDataSet(nAxisNo);
				if(nAxisSeq_Result[nAxisNo] != 0)
				{
					nAxis_TaskPos[nAxisNo] = 800;
					return;
				}
			}

			mAxisSequenceData[nAxisNo].nCurBlock = 0;
			if(mAxisSequenceData[nAxisNo].nStartStep != 0)
				mAxisSequenceData[nAxisNo].nSendCount = mAxisSequenceData[nAxisNo].nStartStep -1;
			else
				mAxisSequenceData[nAxisNo].nSendCount = 0;
			nAxisSeq_Result[nAxisNo] = AX_ReadyCheck(nAxisNo, bReadyCheck);
			if(bReadyCheck == FALSE)
			{
				nAxis_TaskPos[nAxisNo] = 800;
				return;
			}
			nSendCmdCount = GetAxisSequenceMoveEndCount(nAxisNo) - mAxisSequenceData[nAxisNo].nSendCount;
			if(nSendCmdCount >= nSendMaxCount)
				nSendCmdCount = nSendMaxCount;
			if(AX_SequenceMoveSetVaildCheck(nAxisNo, nSendCmdCount, TRUE)) //명령어 전송 가능 상태
			{
				nAxisSeq_Result[nAxisNo] = AX_SequenceMoveCmdSet(nAxisNo, mAxisSequenceData[nAxisNo].nSendCount, (mAxisSequenceData[nAxisNo].nSendCount + nSendCmdCount));
				if(nAxisSeq_Result[nAxisNo] != 0)
				{
					nAxis_TaskPos[nAxisNo] = 800;
					return;
				}
				nAxis_TaskPos[nAxisNo] = 100;
			}
		}
	}
	else if(nAxis_TaskPos[nAxisNo] == 100) // 다음 버퍼 지령 가능 여부 확인
	{
		nSendCmdCount = GetAxisSequenceMoveEndCount(nAxisNo) - mAxisSequenceData[nAxisNo].nSendCount;
		if(nSendCmdCount >= nSendMaxCount) //30개씩 send
			nSendCmdCount = nSendMaxCount;
		else if(nSendCmdCount == 0)
		{
			nAxis_TaskPos[nAxisNo] = 200;
		}
		if(AX_SequenceMoveSetVaildCheck(nAxisNo, nSendCmdCount, FALSE)) // 명령어 전송 가능 상태
		{
			nAxisSeq_Result[nAxisNo] = AX_SequenceMoveCmdSet(nAxisNo, mAxisSequenceData[nAxisNo].nSendCount, (mAxisSequenceData[nAxisNo].nSendCount + nSendCmdCount));
			if (nAxisSeq_Result[nAxisNo] != 0)
			{
				nAxis_TaskPos[nAxisNo] = 800;
				return;
			}
			nAxis_TaskPos[nAxisNo] = 100;
		}
	}
	else if(nAxis_TaskPos[nAxisNo] == 200) // 다음 버퍼 지령 수행 상태 Check 
	{
		//Complete Case Check
		if (AX_SequenceBufferStateCheck(nAxisNo) == MXP_EasyClass::SequenceBuffer_Complete)
		{
			nAxis_TaskPos[nAxisNo] = 1000;
		} 
		else if (AX_SequenceBufferStateCheck(nAxisNo) == MXP_EasyClass::SequenceBuffer_Fail)
		{
			nAxisSeq_Result[nAxisNo] = MXP_EasyClass::RET_SEQUENCEMOVE_PROCESS_FAIL;
			nAxis_TaskPos[nAxisNo] = 800;
		}
		else //  Run 중이면
		{
			if(mFeedbackSensorParam[nAxisNo].bUsing)
			{
				SEQUENCEMOVEPROCESS_CHECK stResult;
				UINT16 nRemainCount;
				UINT16 nCurStep;
				string strCurStepName = "";

				AX_SequenceMoveCheck(nAxisNo, nCurStep, strCurStepName, nRemainCount, stResult);
				if (nRemainCount < 1)
				{
					nAxisSeq_Result[nAxisNo] = AX_FeedBackSensorRun(nAxisNo, ePLCFBRunMode_Run);
					if (nAxisSeq_Result[nAxisNo] != 0)
					{
						nAxis_TaskPos[nAxisNo] = 800;
						return;
					}
					nAxis_TaskPos[nAxisNo] = 300;
				}
			}
		}
	}
	else if(nAxis_TaskPos[nAxisNo] == 300)
	{
		FBPROCESS_CHECK nSensorCheck;
		nAxisSeq_Result[nAxisNo] = AX_FeedBackSensorUsingCheck(nAxisNo, nSensorCheck);
		if (nAxisSeq_Result[nAxisNo] != 0)
		{
			nAxis_TaskPos[nAxisNo] = 800;
			return;
		}

		if (nSensorCheck.nRunState = e_Complete)
		{
			nAxisSeq_Result[nAxisNo] = AX_FeedBackSensorLog(nAxisNo);
			nAxis_TaskPos[nAxisNo] = 310;
		}
		else if (nSensorCheck.nRunState == e_Fail)
		{
			//RET_FEEDBACKSENSOR_MOVE_FAIL
			nAxisSeq_Result[nAxisNo] = RET_FEEDBACKSENSOR_MOVE_FAIL;
			nAxis_TaskPos[nAxisNo] = 800;
			return;
		}
		
	}
	else if(nAxis_TaskPos[nAxisNo] == 310)
	{
		bool bReadyCheck;
		nAxisSeq_Result[nAxisNo] = AX_ReadyCheck(nAxisNo, bReadyCheck);

		if (nAxisSeq_Result[nAxisNo] != 0)
		{
			nAxis_TaskPos[nAxisNo] = 800;
			return;
		}
		if (bReadyCheck)
			nAxis_TaskPos[nAxisNo] = 1000;
	}
	else if(nAxis_TaskPos[nAxisNo] == 800)
	{
		mAxisSequenceData[nAxisNo].bRunFlag = FALSE;
		mAxisSequenceData[nAxisNo].nState.Busy = FALSE;
		mAxisSequenceData[nAxisNo].nState.Done = FALSE;
		mAxisSequenceData[nAxisNo].nState.ErrorOn = TRUE;
		mAxisSequenceData[nAxisNo].nState.ErrorID = (nAxisSeq_Result[nAxisNo] != 0) ? nAxisSeq_Result[nAxisNo] : MXP_EasyClass::RET_SEQUENCEMOVE_PROCESS_FAIL;
		nAxis_TaskPos[nAxisNo] = 0;

		nAxisSeq_Result[nAxisNo] = AX_FeedBackSensorRun(nAxisNo, ePLCFBRunMode_Reset);
		mExAPIAxisParam[nAxisNo].bSequenceOverrideFlag = false;
	}
	else if(nAxis_TaskPos[nAxisNo] == 1000)
	{
		mAxisSequenceData[nAxisNo].bRunFlag = FALSE;
		mAxisSequenceData[nAxisNo].nState.Busy = FALSE;
		mAxisSequenceData[nAxisNo].nState.Done = TRUE;
		mAxisSequenceData[nAxisNo].nState.ErrorOn = FALSE;
		mAxisSequenceData[nAxisNo].nState.ErrorID = 0;
		nAxisSeq_Result[nAxisNo] = AX_FeedBackSensorRun(nAxisNo, ePLCFBRunMode_Reset);
		mExAPIAxisParam[nAxisNo].bSequenceOverrideFlag = false;
		
		nAxis_TaskPos[nAxisNo] = 0;
	}

}

void MXP_EasyClass::SEQ_GroupSequenceMoveManager(INT nAxesGroup)
{
	int nSendMaxCount = 30;
	MXP_FUNCTION_STATUS nResult[32];
	INT nSendCmdCount;

	try
	{
		if (mGroupSequenceData[nAxesGroup].bRunFlag)
			MXP_EasyClass::SYS_GetGroupBufferInfo(nAxesGroup, mGroupSequenceData[nAxesGroup].nSaveNum, mGroupSequenceData[nAxesGroup].nReadNum, mGroupSequenceData[nAxesGroup].nCurBlock);

		if(nGroup_TaskPos[nAxesGroup] > 0 && nGroup_TaskPos[nAxesGroup] < 800 && mGroupSequenceData[nAxesGroup].bRunFlag == FALSE)
			nGroup_TaskPos[nAxesGroup] = 800;

		if(nGroup_TaskPos[nAxesGroup] == 0)
		{
			bool bReadyCheck;
			if(mGroupSequenceData[nAxesGroup].bRunFlag) 
			{
				// 명령 전송전 초기화
				mGroupSequenceData[nAxesGroup].nCurBlock = 0;

				if(mGroupSequenceData[nAxesGroup].nStartStep != 0)
					mGroupSequenceData[nAxesGroup].nSendCount = mGroupSequenceData[nAxesGroup].nStartStep -1;
				else
					mGroupSequenceData[nAxesGroup].nSendCount = 0;

				nResult[nAxesGroup] = GRP_ReadyCheck(nAxesGroup, bReadyCheck);
				if(bReadyCheck == FALSE)
				{
					nGroup_TaskPos[nAxesGroup] = 800;
					return;
				}

				nSendCmdCount = GetGroupSequenceMoveEndCount(nAxesGroup) - mGroupSequenceData[nAxesGroup].nSendCount;

				if(nSendCmdCount >= nSendMaxCount) //30개 씩 send
					nSendCmdCount = nSendMaxCount;

				if(GRP_SequenceMoveSetVaildCheck(nAxesGroup, nSendCmdCount, TRUE)) //명령어 전송 가능 상태
				{
					nResult[nAxesGroup] = GRP_SequenceMoveCmdSet(nAxesGroup, mGroupSequenceData[nAxesGroup].nSendCount, (mGroupSequenceData[nAxesGroup].nSendCount + nSendCmdCount));
					if(nResult[nAxesGroup] != 0)
					{
						nGroup_TaskPos[nAxesGroup] = 800;
						return;
					}
					nGroup_TaskPos[nAxesGroup] = 100;
				}
			}
		}
		else if(nGroup_TaskPos[nAxesGroup] == 100) // 다음 버퍼 지령 가능 여부 확인
		{
			nSendCmdCount = GetGroupSequenceMoveEndCount(nAxesGroup) - mGroupSequenceData[nAxesGroup].nSendCount;
			if(nSendCmdCount >= nSendMaxCount) //30개씩 send
				nSendCmdCount = nSendMaxCount;
			else if(nSendCmdCount == 0)
			{
				nGroup_TaskPos[nAxesGroup] = 200;
				return;
			}

			if(GRP_SequenceMoveSetVaildCheck(nAxesGroup, nSendCmdCount, FALSE)) // 명령어 전송 가능 상태
			{
				nResult[nAxesGroup] = GRP_SequenceMoveCmdSet(nAxesGroup, mGroupSequenceData[nAxesGroup].nSendCount, (mGroupSequenceData[nAxesGroup].nSendCount + nSendCmdCount));
				if (nResult[nAxesGroup] != 0)
				{
					nGroup_TaskPos[nAxesGroup] = 800;
					return;
				}
				nGroup_TaskPos[nAxesGroup] = 100;
			}
		}
		else if(nGroup_TaskPos[nAxesGroup] == 200) // 다음 버퍼 지령 수행 상태 Check 
		{
			//Complete Case Check
			if (AX_SequenceBufferStateCheck(nAxesGroup) == MXP_EasyClass::SequenceBuffer_Complete)
			{
				nGroup_TaskPos[nAxesGroup] = 1000;
			} 
			else if (AX_SequenceBufferStateCheck(nAxesGroup) == MXP_EasyClass::SequenceBuffer_Fail)
			{
				nResult[nAxesGroup] = MXP_EasyClass::RET_SEQUENCEMOVE_PROCESS_FAIL;
				nGroup_TaskPos[nAxesGroup] = 800;
			}
		}
		else if(nGroup_TaskPos[nAxesGroup] == 800)
		{
			mGroupSequenceData[nAxesGroup].bRunFlag = FALSE;
			mGroupSequenceData[nAxesGroup].nState.Busy = FALSE;
			mGroupSequenceData[nAxesGroup].nState.Done = FALSE;
			mGroupSequenceData[nAxesGroup].nState.ErrorOn = TRUE;
			mGroupSequenceData[nAxesGroup].nState.ErrorID = (nResult[nAxesGroup] != 0) ? nResult[nAxesGroup] : MXP_EasyClass::RET_SEQUENCEMOVE_PROCESS_FAIL;

			nGroup_TaskPos[nAxesGroup] = 0;
		}
		else if(nGroup_TaskPos[nAxesGroup] == 1000)
		{
			mGroupSequenceData[nAxesGroup].bRunFlag = FALSE;
			mGroupSequenceData[nAxesGroup].nState.Busy = FALSE;
			mGroupSequenceData[nAxesGroup].nState.Done = TRUE;
			mGroupSequenceData[nAxesGroup].nState.ErrorOn = FALSE;
			mGroupSequenceData[nAxesGroup].nState.ErrorID = 0;

			nGroup_TaskPos[nAxesGroup] = 0;
		}
	}
	catch (invalid_argument)
	{
		mGroupSequenceData[nAxesGroup].bRunFlag = FALSE;
		mGroupSequenceData[nAxesGroup].nState.Busy = FALSE;
		mGroupSequenceData[nAxesGroup].nState.Done = FALSE;
		mGroupSequenceData[nAxesGroup].nState.ErrorOn = TRUE;
		mGroupSequenceData[nAxesGroup].nState.ErrorID = (nResult[nAxesGroup] != 0) ? nResult[nAxesGroup] : MXP_EasyClass::RET_SEQUENCEMOVE_PROCESS_FAIL;

		nGroup_TaskPos[nAxesGroup] = 0;
	}	
}

UINT16 MXP_EasyClass::GetAxisSequenceMoveEndCount(INT nAxisNo)
{
	if(mAxisSequenceData[nAxisNo].nEndStep != 0)
		return mAxisSequenceData[nAxisNo].nEndStep;
	else
		return mAxisSequenceData[nAxisNo].nData.size();
}
UINT16 MXP_EasyClass::GetGroupSequenceMoveEndCount(INT nAxisNo)
{
	if(mGroupSequenceData[nAxisNo].nEndStep != 0)
		return mGroupSequenceData[nAxisNo].nEndStep;
	else
		return mGroupSequenceData[nAxisNo].nData.size();
}
#pragma endregion SequeceMove

#pragma region
void MXP_EasyClass::AX_Init_ExAPI_Param()
{
	int nAxisCount;
	for (nAxisCount = 0; nAxisCount < sizeof(mExAPIAxisParam)/sizeof(*mExAPIAxisParam); nAxisCount++)
	{
		mExAPIAxisParam[nAxisCount].fOverrideSetValue = 1;
	}
}
#pragma endregion ExAPIAxisParam

#pragma region

bool MXP_EasyClass::AX_ValidCheckUsingFeedBackSensorCtrl(UINT32 nAxisNo, float fLastStepDist)
{
	if (mFeedbackSensorParam[nAxisNo].bUsing)
	{
		if (mFeedbackSensorParam[nAxisNo].fTargetPosCalDist > abs(fLastStepDist))
			return false;
	}
	return true;
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_SetUsingPositionSensor(UINT32 nAxisNo, bool bUsing, UINT32 nSlaveNo, UINT16 nOffset, UINT16 nSize, 
																			float fSensorTargetValue, float fSensorPositionSetRange,float fSensorPulseToUnit, 
																			float fTargetPosCalDist, float fTargetPosCalVel, float fOverrideDist)
{
	MXP_FUNCTION_STATUS nResult;
	UINT32 nSlaveCount;
	int AxisNo = (int)nAxisNo;

	if (bUsing)
	{
		nResult = ECAT_GetSlaveCount(nSlaveCount);

		if (nSlaveNo > nSlaveCount || nSize == 0)
			return RET_FEEDBACKSENSOR_PARAM_INVAILD;
		if (fTargetPosCalDist < fOverrideDist)
			return RET_FEEDBACKSENSOR_PARAM_INVAILD;
		if (fSensorPulseToUnit < 0)
			return RET_FEEDBACKSENSOR_PARAM_INVAILD;
		if (fTargetPosCalVel <= 0)
			return RET_FEEDBACKSENSOR_PARAM_INVAILD;
		
		mFeedbackSensorParam[AxisNo].bUsing = bUsing;
		mFeedbackSensorParam[AxisNo].nSensorSlaveNo = (UINT8)nSlaveNo;
		mFeedbackSensorParam[AxisNo].nSensorPos = (UINT8)nOffset;
		mFeedbackSensorParam[AxisNo].nSensorSize = (UINT8)nSize;
		mFeedbackSensorParam[AxisNo].fSensorTargetValue = fSensorTargetValue;
		mFeedbackSensorParam[AxisNo].fSensorPositionSetRange = fSensorPositionSetRange;
		mFeedbackSensorParam[AxisNo].fSensorPulseToUnit = fSensorPulseToUnit;
		mFeedbackSensorParam[AxisNo].fTargetPosCalDist = fTargetPosCalDist;
	}
	else
		mFeedbackSensorParam[nAxisNo].bUsing = false;

	return RET_NO_ERROR;

}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_SetUsingPositionSensor(UINT32 nAxisNo, bool bUsing, UINT32 nSlaveNo, UINT16 nOffset, UINT16 nSize,
	float fSensorTargetValue, float fSensorPositionSetRange, float fSensorPulseToUnit,
	float fTargetPosCalDist)
{
	MXP_FUNCTION_STATUS nResult;
	UINT32 nSlaveCount;
	int AxisNo = (int)nAxisNo;

	if (bUsing)
	{
		nResult = ECAT_GetSlaveCount(nSlaveCount);

		if (nSlaveNo > nSlaveCount || nSize == 0)
			return RET_FEEDBACKSENSOR_PARAM_INVAILD;
		if (fTargetPosCalDist <= 150)
			return RET_FEEDBACKSENSOR_PARAM_INVAILD;
		if (fSensorPulseToUnit < 0)
			return RET_FEEDBACKSENSOR_PARAM_INVAILD;

		mFeedbackSensorParam[AxisNo].bUsing = bUsing;
		mFeedbackSensorParam[AxisNo].nSensorSlaveNo = (UINT8)nSlaveNo;
		mFeedbackSensorParam[AxisNo].nSensorPos = (UINT8)nOffset;
		mFeedbackSensorParam[AxisNo].nSensorSize = (UINT8)nSize;
		mFeedbackSensorParam[AxisNo].fSensorTargetValue = fSensorTargetValue;
		mFeedbackSensorParam[AxisNo].fSensorPositionSetRange = fSensorPositionSetRange;
		mFeedbackSensorParam[AxisNo].fSensorPulseToUnit = fSensorPulseToUnit;
		mFeedbackSensorParam[AxisNo].fTargetPosCalDist = fTargetPosCalDist;
	}
	else
		mFeedbackSensorParam[nAxisNo].bUsing = false;

	return RET_NO_ERROR;

}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_SetUsingPositionSensor(UINT32 nAxisNo, bool bUsing)
{
	int AxisNo = (int)nAxisNo;
	mFeedbackSensorParam[AxisNo].bUsing = bUsing;
	return RET_NO_ERROR;
}

#pragma endregion SequenceMove_UsingSensor

#pragma region
bool MXP_EasyClass::FBInfoReset(bool bLocalMemoryOnly)
{
	int nAxisCount;
	MXP_FUNCTION_STATUS nResult;
	for(nAxisCount = 0; nAxisCount < sizeof(mPLCFBInfo)/sizeof(*mPLCFBInfo); nAxisCount++)
	{
		mPLCFBInfo[nAxisCount].nFBIndex = -1;
		mPLCFBInfo[nAxisCount].nRunState = ePLCFBRunMode_Null;
		mPLCFBInfo[nAxisCount].bUsing = false;
	}

	int nAddress;
	if (bLocalMemoryOnly == false)
	{
		for (nAddress = PLCStartAddress; nAddress < PLCEndAddress; nAddress++)
		{
			nResult = PLC_WriteByte((UINT32)nAddress, 0);
		}
	}

	return true;
}
#pragma endregion PLC Move

#pragma region
int MXP_EasyClass::GetValidFeedBackSensorFBNumber(UINT32 nAxisNo)
{
	MXP_FUNCTION_STATUS nResult;
	UINT8 nFBCount, nFBMaxCount;

	nResult = PLC_ReadByte(e_PLCBlockMax, nFBMaxCount);

	if (nFBMaxCount == 0)
		return -1;

	for (nFBCount = 0; nFBCount < nFBMaxCount; nFBCount++)
	{
		UINT8 nFBState;
		UINT nAddress = e_FBRunState + (FeedBackSensorFBSize*nFBCount);
		nResult = PLC_ReadByte(nAddress, nFBState);
		if (nFBState == e_Ready)
		{
			//해당 block이 사용중인 경우 Pass
			int nAxisCount;
			bool bCurBlockUsing = false;
			for (nAxisCount = 0; nAxisCount < sizeof(mPLCFBInfo); nAxisCount++)
			{
				if(mPLCFBInfo[nAxisCount].nFBIndex == nFBCount && nAxisCount != nAxisNo)
					bCurBlockUsing = true;
			}

			if (bCurBlockUsing == false)
				return nFBCount;
		}
	}

	return -1;
}
bool MXP_EasyClass::AX_FeedBackSensorMove_ValidCheck(UINT32 nAxisNo)
{
	// Axis 가 진행 중인경우 Run Command 처리 
	if (mPLCFBInfo[nAxisNo].bUsing)
		return false;

	return true;
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_FeedBackSensorUsingCheck(UINT32 nAxisNo, FBPROCESS_CHECK &stStatus)
{
	try
	{
		int nAxis = (int)nAxisNo;
		MXP_FUNCTION_STATUS nResult; 
		stStatus.nRunState = e_Ready;
		stStatus.ErrorID = 0;
		stStatus.nRunStep = 0;

		if(mPLCFBInfo[nAxis].nFBIndex != -1)
		{
			UINT8 nReadData;
			nResult = PLC_ReadByte(e_FBRunState + mPLCFBInfo[nAxis].nFBIndex*FeedBackSensorFBSize, nReadData);
			stStatus.nRunState = (eFBRunState)nReadData;
			UINT16 nReadWData;
			nResult = PLC_ReadUInt16((e_FBErrorCode + mPLCFBInfo[nAxis].nFBIndex * FeedBackSensorFBSize), nReadWData);
		}
		else
		{
			stStatus.ErrorID = 0;
			stStatus.nRunState = e_Ready;
			stStatus.nRunStep = 0;
		}
		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_FeedBackSensorOverrideInVaildFlag(UINT32 nAxisNo, UINT8 bInVaildFlag)
{
	try
	{
		MXP_FUNCTION_STATUS nResult;
		
		nResult = PLC_WriteByte(e_FBSensorVaildFlag, bInVaildFlag);
		
		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_FeedBackSensorLog(UINT32 nAxisNo)
{
	try
	{
		MXP_FUNCTION_STATUS nResult;
		int nFeedBackSensorFBNumber = 0;
		float fDist1, fDist2, fVel1, fVel2;

		nResult = PLC_ReadFloat(e_FBSensorDist1 + nFeedBackSensorFBNumber * FeedBackSensorFBSize, fDist1);
		nResult = PLC_ReadFloat(e_FBSensorDist2 + nFeedBackSensorFBNumber * FeedBackSensorFBSize, fDist2);
		nResult = PLC_ReadFloat(e_FBSensorVel1 + nFeedBackSensorFBNumber * FeedBackSensorFBSize, fVel1);
		nResult = PLC_ReadFloat(e_FBSensorVel2 + nFeedBackSensorFBNumber * FeedBackSensorFBSize, fVel2);

		string stTempLog = floatToString(fDist1) + ", " + floatToString(fDist2) + floatToString(fVel1) + ", " + floatToString(fVel2);
		std::cout << ("SequecneMove Temp Log:" + stTempLog); //Debug 용? #using <system.dll> using namespace System::Diagnostics; using namespace System;
															//Trace::WriteLine or Debug::WriteLine
		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_FeedBackSensorDataSet(UINT32 nAxisNo)
{
	try
	{
		MXP_FUNCTION_STATUS nResult;
		int nFeedBackSensorFBNumber;

		if(AX_FeedBackSensorMove_ValidCheck(nAxisNo) == false)
			return RET_PLCFB_MOVE_FAIL;

		nFeedBackSensorFBNumber = GetValidFeedBackSensorFBNumber(nAxisNo);

		if (nFeedBackSensorFBNumber < 0)
			return RET_SPINMOVE_COMMAND_INVALIDSTATE;

		mPLCFBInfo[nAxisNo].nFBIndex = nFeedBackSensorFBNumber; // FB 번호 Set
		mPLCFBInfo[nAxisNo].bUsing = true;
		mPLCFBInfo[nAxisNo].nRunState = ePLCFBRunMode_Run;

		nResult = PLC_WriteByte(e_FBAxisNo + nFeedBackSensorFBNumber * FeedBackSensorFBSize , nAxisNo);
		nResult = PLC_WriteByte(e_FBSensorSlaveNo + nFeedBackSensorFBNumber * FeedBackSensorFBSize , mFeedbackSensorParam[nAxisNo].nSensorSlaveNo);
		nResult = PLC_WriteByte(e_FBSensorPos + nFeedBackSensorFBNumber * FeedBackSensorFBSize , mFeedbackSensorParam[nAxisNo].nSensorPos);
		nResult = PLC_WriteByte(e_FBSensorSize + nFeedBackSensorFBNumber * FeedBackSensorFBSize , mFeedbackSensorParam[nAxisNo].nSensorSize);

		nResult = PLC_WriteFloat(e_FBSensorTarget + nFeedBackSensorFBNumber * FeedBackSensorFBSize , mFeedbackSensorParam[nAxisNo].fSensorTargetValue);
		nResult = PLC_WriteFloat(e_FBSensorSetRange + nFeedBackSensorFBNumber * FeedBackSensorFBSize , mFeedbackSensorParam[nAxisNo].fSensorPositionSetRange);
		nResult = PLC_WriteFloat(e_FBSensorOverrideDist + nFeedBackSensorFBNumber * FeedBackSensorFBSize , mFeedbackSensorParam[nAxisNo].fOverrideDist);
		nResult = PLC_WriteFloat(e_FBSensorUnit + nFeedBackSensorFBNumber * FeedBackSensorFBSize , mFeedbackSensorParam[nAxisNo].fSensorPulseToUnit);
		nResult = PLC_WriteFloat(e_FBSensorTargetVel + nFeedBackSensorFBNumber * FeedBackSensorFBSize , mFeedbackSensorParam[nAxisNo].fTargetPosCalVel);
		nResult = PLC_WriteFloat(e_FBSensorTargetCalDist + nFeedBackSensorFBNumber * FeedBackSensorFBSize , mFeedbackSensorParam[nAxisNo].fTargetPosCalDist);

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
MXP_EasyClass::MXP_FUNCTION_STATUS MXP_EasyClass::AX_FeedBackSensorRun(UINT32 nAxisNo, ePLCFBRunMode nRunMode)
{
	try
	{
		MXP_FUNCTION_STATUS nResult;
		int nFeedBackSensorFBNumber;

		if (nRunMode == ePLCFBRunMode_Stop)
		{
			if (mPLCFBInfo[nAxisNo].bUsing == true)
			{
				nFeedBackSensorFBNumber = mPLCFBInfo[nAxisNo].nFBIndex;
				nResult = PLC_WriteByte(e_FBCommand + nFeedBackSensorFBNumber * FeedBackSensorFBSize, (UINT8)nRunMode);
			}
		}

		if (nRunMode == ePLCFBRunMode_Stop)
		{
			if (mPLCFBInfo[nAxisNo].bUsing == true)
			{
				nFeedBackSensorFBNumber = mPLCFBInfo[nAxisNo].nFBIndex;
				nResult = PLC_WriteByte(e_FBCommand + nFeedBackSensorFBNumber * FeedBackSensorFBSize, (UINT8)nRunMode);
			}
		}

		if (nRunMode == ePLCFBRunMode_Reset || nRunMode == ePLCFBRunMode_Null)
		{
			if (mPLCFBInfo[nAxisNo].bUsing == true)
			{
				FBPROCESS_CHECK nFBProcessCheck;

				nResult = AX_FeedBackSensorUsingCheck(nAxisNo, nFBProcessCheck);
				if (nFBProcessCheck.nRunState != e_Ready)
				{
					nFeedBackSensorFBNumber = mPLCFBInfo[nAxisNo].nFBIndex;
					nResult = PLC_WriteByte(e_FBCommand + nFeedBackSensorFBNumber * FeedBackSensorFBSize, (UINT8)nRunMode);
				}
				
			}
			mPLCFBInfo[nAxisNo].nFBIndex = -1;
			mPLCFBInfo[nAxisNo].bUsing = false;
		}

		return (MXP_FUNCTION_STATUS)nResult;
	}
	catch (invalid_argument)
	{
		return MXP_EasyClass::RET_ERROR_EXCEPTIONERROR;
	}
}
#pragma endregion FeedbackSensorFBInterface
