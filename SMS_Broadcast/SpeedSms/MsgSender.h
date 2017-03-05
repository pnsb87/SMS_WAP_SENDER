#pragma once

#include "SerialCommunication.h"
#include "WAPPushPDU.h"
#include <string>
#include <map>

using namespace std;

class CMsgSender
{
public:
	CMsgSender(void);
	~CMsgSender(void);

	virtual int ConfigCom(SerialCommunication* comm);
	virtual int ConfigMode(int nMode);

	virtual int Send(string dstNumber);

	virtual int SendSms(string dstNumber);
	virtual int SendSmsPdu(string dstNumber);
	virtual int SendWap(string dstNumber);

	CStringA Encode7bit(CString message, int *len);
	CStringA ConvertSpecial(CStringA msg);

	void setMsgText(string text__);
	string getMsgText(){return m_sMsgText;};

	void setUrlText(string url__);
	string getUrlText(){return m_sUrlText;};
	void exit();
	int getTranId();

	void SetProtocolId(const char* szProtocolID);

public:
	char	m_szProtocolId[10];

	string m_sMsgText;
	string m_sUrlText;

	SerialCommunication* m_serialComm;
	bool bReadySms;
	bool bReadyWap;

	int tranID;
	int	m_nMode;

	map<byte, byte> convertTableSingle;
	map<byte, byte> convertTable;
};
