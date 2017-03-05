#pragma once
#include "stdafx.h"
#include "Utils.h"
//#include "constants.h"
#include <bitset>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>

using namespace std;

// Macro for SMS type field
#define  SMS_INBOX_UNREAD 0
#define  SMS_INBOX_READ 1

#ifndef SMS_OUTBOX
#define  SMS_OUTBOX 2
#endif // SMS_OUTBOX

#ifndef SMS_SENT
#define  SMS_SENT 3
#endif // SMS_SENT

#define  SMS_DELIVERY_SUCCESSED 4
#define  SMS_DELIVERY_FAILED 5

static const char hexval[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

//class CPhoneEntry: public CArg
//{
//protected:
//	CString key;
//	CString val;
//	CString storage;
//	CString GroupName;
//	int m_slotID;
//public:
//	CPhoneEntry(CString key = _T(""), CString val = _T(""), CString sstorage = _T("SIM"), int slotID = 0, CString groupName = _T("")) // dungtx: change to int type
//	{
//		this->key = key;
//		this->val = val;
//		this->storage = sstorage;
//		this->m_slotID = slotID;
//		this->GroupName = groupName; // Dungtx: add GROUP name
//	}
//	inline CString& GetKey()
//	{
//		return key;
//	}
//	inline CString& GetValue()
//	{
//		return val;
//	}
//	inline CString& GetStorage()
//	{
//		return storage;
//	}
//	inline int GetID()
//	{
//		return m_slotID;
//	}
//
//	inline void SetID(int id)
//	{
//		m_slotID = id;
//	}
//	inline CString& GetGroupName()
//	{
//		return GroupName;
//	}
//
//	inline void SetKey(LPCTSTR pKey)
//	{
//		key = pKey;
//	}
//
//	inline void SetValue(LPCTSTR pvalue)
//	{
//		val = pvalue;
//	}
//
//	inline void SetGroupName(LPCTSTR pvalue)
//	{
//		GroupName = pvalue;
//	}
//
//	inline void SetStorage(LPCTSTR pvalue)
//	{
//		storage = pvalue;
//	}
//};
//class CSMSEntry: public CArg
//{
//public:
//	CString m_phoneNumber;
//	CString m_content;
//	CTime m_dateTime;
//	int m_smsType; // dungtx
//	int m_slotID;
//	CString storage;
//	CStringA m_pdu;
//	//bug #25: use enable SR and save SMS internally
//	bool isEnableSR; // is enable status report
//	bool isSaveSMS; // Is log SMS or normal SMS
//	int id_sms;
//	int offset;
//public:
//	CSMSEntry(int m_slotID = -1, int m_smsType = 0, LPCSTR m_phoneNumber= "", LPCTSTR m_message= _T(""), CTime m_dateTime = NULL, LPCTSTR p_storage = _T("SIM"), LPCSTR pdu = "")
//	{
//		this->m_content = m_message;
//		this->m_phoneNumber = m_phoneNumber;
//		this->m_dateTime = m_dateTime;
//		this->m_smsType = m_smsType;
//		this->m_slotID  = m_slotID;
//		this->storage = p_storage;
//		this->m_pdu = pdu;
//		this->isEnableSR = false;
//		this->isSaveSMS = false;
//		this->offset = 0;
//	}
//	/*CSMSEntry(int m_slotID = 0, int m_smsType = 0, CStringA m_phoneNumber= "", CString m_message= _T(""), CTime m_dateTime = _T(""), CString p_storage = _T("SIM"), CStringA pdu = "")
//	{
//		this->m_content = m_message;
//		this->m_phoneNumber = m_phoneNumber;
//		this->m_dateTime = m_dateTime;
//		this->m_smsType = m_smsType;
//		this->m_slotID  = m_slotID;
//		this->storage = p_storage;
//		this->m_pdu = pdu;
//	}*/
//	CString& GetPhoneNumber()
//	{
//		return m_phoneNumber;
//	}
//	CString& GetContent()
//	{
//		return m_content;
//	}
//	CTime GetDateTime()
//	{
//		return m_dateTime;
//	}
//	CString GetTimeYearFirst()
//	{
//		return m_dateTime.Format("%Y/%m/%d %H:%M:%S");
//	}
//	
//	CString GetTimeDayFirst()
//	{
//		return m_dateTime.Format("%d/%m/%Y %H:%M:%S");
//	}
//
//	int GetSMSType()
//	{
//		return m_smsType;
//	}
//	int GetSlotID()
//	{
//		return m_slotID;
//	}
//	CString& GetStorage()
//	{
//		return storage;
//	}
//	CStringA& GetPDU()
//	{
//		return m_pdu;
//	}
//
//	int GetID()
//	{
//		return id_sms;
//	}
//
//	int GetOffset()
//	{
//		return offset;
//	}
//
//	void SetPDU(LPCSTR pdu)
//	{
//		m_pdu = pdu;
//	}
//
//	// dungtx: add set function
//	void SetPhoneNumber(LPCTSTR p_phonenumber)
//	{
//		m_phoneNumber = p_phonenumber;
//	}
//	void SetContent(LPCTSTR p_content)
//	{
//		m_content = p_content;
//	}
//	
//	void SetSMSType(int p_smstype)
//	{
//		m_smsType = p_smstype;
//	}
//
//	void SetSlotID(int p_slotid)
//	{
//		m_slotID = p_slotid;
//	}
//	void SetEnableSR(bool isEnable)
//	{
//		isEnableSR = isEnable;
//	}
//	bool GetEnableSR()
//	{
//		return isEnableSR;
//	}
//	void SetSaveSMS(bool isSave)
//	{
//		isSaveSMS = isSave;
//	}
//	bool GetSaveSMS()
//	{
//		return isSaveSMS;
//	}
//
//	void SetID(int id)
//	{
//		this->id_sms = id;
//	}
//
//	void SetOffset(int offset)
//	{
//		this->offset = offset;
//	}
//	void SetDateTime(CTime time)
//	{
//		this->m_dateTime = time;
//	}
//	void SetStorage(CString storage)
//	{
//		this->storage = storage;
//	}
//};
//
//class CSMSEntryEx: public CSMSEntry
//{
//protected:
//	CString m_contactName;
//
//public:
//	CSMSEntryEx() : m_contactName(_T(""))
//	{
//
//	}
//	
//	CSMSEntryEx(const CSMSEntry& v): m_contactName(_T(""))
//	{
//		this->m_content = v.m_content;
//		this->m_phoneNumber = v.m_phoneNumber;
//		this->m_dateTime = v.m_dateTime;
//		this->m_smsType = v.m_smsType;
//		this->m_slotID  = v.m_slotID;
//		this->storage = v.storage;
//		this->m_pdu = v.m_pdu;
//		this->id_sms = v.id_sms;
//	}
//	
//	inline void SetContactName(CString& contactName)
//	{
//		m_contactName = contactName;
//	}
//
//	inline CString& GetContactName()
//	{
//		return m_contactName;
//	}
//};

class CPDULib
{
protected:
	bool m_bTP_RP;		// Reply path. Parameter indicating that reply path exists.
	bool m_bTP_UDHI;	// User data header indicator. This bit is set to 1 if the User Data field starts with a header
	bool m_bTP_SRI;		// Status report indication. This bit is set to 1 if a status report is going to be returned to the SME
	bool m_bTP_MMS;		// More messages to send. This bit is set to 0 if there are more messages to send
	map<byte, byte> convertTable;
	map<byte, byte> convertTableSingle;
	map<byte, byte> reserveConvertTable;
	map<byte, byte> reserveConvertTableSingle;
	CStringA ConvertSpecial(CStringA msg);
	CString ReverseConvertSpecial(CStringA msg, int msgLen =-1);
public:
	CPDULib();
	~CPDULib();
	CStringA EncodePDUPhoneNumber(CStringA phoneNumber);
	CStringA DecodePDUPhoneNumber(CStringA PDUPhoneNumber);
	CStringA Encode7bit(CString message, int *len);
	CString Decode7bit(CStringA message, int startIndex = -1, int msgLen =-1);
	CStringA Encode8bit(CString message);
	CString Decode8bit(CStringA message);
	void EncodeUCS2(const CString & message,CStringA& ret);
	void DecodeUCS2(CStringA & message,CString& ret);
	CStringA EncodeUCS2(CString message);
	CString DecodeUCS2(CStringA message);
	CTime GetTimeStamp(CStringA pduTimeStamp);
	void ParseSMSPara(byte para);
	CStringA CreatePDUMessage(CStringA phoneNumber, CString message, bool statusReport = false, bool useUnicode = true, CStringA smscNumber = "");
	CSMSEntry FetchSMS(int slotID, int messageType, CStringA message); 
	CSMSEntry GetMessageContent(int slotID, int messageType, CStringA content, CStringA org_msg);
	CSMSEntry GetStatusReport(int slotID, CStringA message);
	CStringA GetPDUTimeStamp(CTime timestamp);
	//Thang
	CStringA UTF16toUTF8(const CStringW& utf16);
	CStringW UTF8toUTF16(const CStringA& utf8);
	void UTF16toUTF8(const CStringW& utf16,CStringA&);
	void UTF8toUTF16(const CStringA& utf8,CStringW&);
	int stringToHex(CStringA& ,CStringA& );
	int hexToString(CStringA& ,CStringA & );
	int stringToHex(char* input,int sz,char* out);
	int hexToString(char* input,int len,char* out);
};

////
//// CCallEntry section
////
//class CCallEntry
//{
//protected:
//	TCHAR PhoneNumber[MAX_NUMBER_LEN + 1];		// phone number number (receive/dial number) (max length is 20)
//	CTime StartDateTime;					// data and time of the call
//	int Duration;							// seconds
//	int CallType;							// miss call (2), dial call (0), or receive call (1)
//	int CallIdx;
//	int id;
//public:
//	CCallEntry()
//	{
//		PhoneNumber[0] = 0;
//		Duration = 0;
//		CallIdx = 0;
//		CallType = 0;
//		id = 0;
//	}
//
//	CCallEntry(CString phonenumer, int calltype, CTime datetime, int duration = 0, int callidx = 0)
//	{
//		if (phonenumer.GetLength() > MAX_NUMBER_LEN)
//		{
//			phonenumer = phonenumer.Left(MAX_NUMBER_LEN);
//		}
//
//		for (int idx = 0; idx < phonenumer.GetLength(); idx++)
//		{
//			PhoneNumber[idx] = phonenumer.GetAt(idx);
//		}
//		PhoneNumber[phonenumer.GetLength()] = 0;
//		this->StartDateTime = datetime;
//		this->Duration = duration;
//		this->CallType = calltype;
//		
//		this->CallIdx = callidx;
//	}
//
//	CString GetPhoneNumber()
//	{
//		return PhoneNumber;
//	}
//	CTime GetStartTime()
//	{
//		return StartDateTime;
//	}
//	int GetDuration()
//	{
//		return Duration;
//	}
//	int GetCallType()
//	{
//		return CallType;
//	}
//
//	int GetIndex()
//	{
//		return CallIdx;
//	}
//
//	bool SetPhoneNumber(CString pvalue)
//	{
//		if (pvalue.IsEmpty()) return false;
//		if (pvalue.GetLength() > MAX_NUMBER_LEN)
//		{
//			pvalue = pvalue.Left(MAX_NUMBER_LEN);
//		}
//
//		for (int idx = 0; idx < pvalue.GetLength(); idx++)
//		{
//			PhoneNumber[idx] = pvalue.GetAt(idx);
//		}
//		PhoneNumber[pvalue.GetLength()] = 0;
//		return true;
//	}
//	bool SetStartTime(CTime pvalue)
//	{
//		this->StartDateTime = pvalue;
//		return true;
//	}
//	bool SetDuration(int pvalue)
//	{
//		if (pvalue < 0) pvalue = 0;
//		this->Duration = pvalue;
//		return true;
//	}
//	bool SetCallType(int pvalue)
//	{
//		if (pvalue < 0) pvalue = 0;
//		if (pvalue > 2 ) pvalue = 2;
//		this->CallType = pvalue;
//		return true;
//	}
//
//	bool SetCallIdx(int idx)
//	{
//		if (idx < 0) idx = 0;
//		CallIdx = idx;
//		return true;
//	}
//	void SetID(int id)
//	{
//		this->id = id;
//	}
//
//	int GetID()
//	{
//		return id;
//	}
//};