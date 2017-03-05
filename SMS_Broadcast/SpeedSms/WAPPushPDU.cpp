#include "StdAfx.h"
#include "WAPPushPDU.h"
#include "Utils.h"
#include <time.h>
#include <ctime>

WAPPushPDU::WAPPushPDU(void)
{
}
int WAPPushPDU::getPDU(std::string dstNumber, std::string url, std::string text, int tranId, std::string& result)
{
	//std::string pdu;
	CString pdu;
	
	//int out_len = 0;
	pdu.Append("006100");//header
	int out_len = 0;
	
	int number_len = 0;
	std::string bcd_number = Utils::string2BCD(dstNumber, number_len);
	//Utils::Dec2Hex(number_len, hex_data);*/
	//pdu.Append(hex_data);
	pdu.AppendFormat("%02X", number_len);
	pdu.Append("91"); //Destination is an international telephone number
	pdu.Append(bcd_number.c_str()); //msisdn number in BCD format
	pdu.Append("4104");

	//std::string user_data;
	CString user_data;

	user_data.Append("0B");//User data header length is 11 octets
	user_data.Append("05");//IEI 0×05 contains source and destination port.
	user_data.Append("04"); //IEDL. Content is 4 octets.

	user_data.Append("0B84");//Destination port is 2948. This is the port for connectionless WSP.
	user_data.Append("C002");
	user_data.Append("00");//IEI 0×00 contains concatenation information
	user_data.Append("03");//EIDL. Content is 3 octets.
	user_data.Append("F0");//Reference number
	user_data.Append("01");//Total 1 parts
	user_data.Append("01");//This is the 1st (and only) part
	user_data.AppendFormat("%02X", tranId);
	//user_data.append("0A"); //WSP transaction ID
	user_data.Append("06");//This is a push PDU
	user_data.Append("04");//Headers consist of 4 octets
	user_data.Append("03");//Content type consists of 3 octets
	user_data.Append("AE");//ontent type 0x2E: This stands for application/vnd.wap.sic (a WBXML encoded SI document)
	user_data.Append("81");//Parameter name: 0×01 means ‘Charset‘
	user_data.Append("EA");//Parameter value: 0x6A means ‘UTF-8‘
	user_data.Append("02");//WBXML Version 1.2 	WBXML
	user_data.Append("05"); //Document type -//WAPFORUM//DTD SI 1.0//EN
	user_data.Append("6A");//Encoding UTF-8
	user_data.Append("00");//String table length
	user_data.Append("45");//<si> element with content
	user_data.Append("C6");//<indication> element with attributes and content
	user_data.Append("0C");//token for attribute ‘href’ and its value starting with ’http://’
	user_data.Append("03");//Inline string follows
	//char* pUrl = Utils::char2HexString(url);
	user_data.Append(Utils::char2HexString(url).c_str()); //link

	user_data.Append("00");// end of inline string
	/*
	user_data.Append("0A");//token for attribute ‘created’
	user_data.Append("C3");//	token for attribute ‘created’
	user_data.Append("06");//	length of opaque data

	//time_t t = time(0);   // get time now
    //struct tm * now = localtime( & t );
	user_data.Append("200902261625");//a GMT date-time value using hex 
	
	//char timebuf[128] = {0};
	//strftime(timebuf, sizeof(timebuf), "%Y%m%d%H%M", now);
	//user_data.AppendFormat("%s", timebuf);
	user_data.Append("10");//	token for attribute ‘si-expires’
	user_data.Append("C3"); //	opaque data follows
	user_data.Append("06");//	length of opaque data

	user_data.Append("200903041625"); //expire date
	//now->tm_year = now->tm_year + 1;
	//strftime(timebuf, sizeof(timebuf), "%Y%m%d%H%M", now);
	//user_data.AppendFormat("%s", timebuf);
	*/
	user_data.Append("01"); //end of <indication> attributes
	user_data.Append("03");//	Inline string follows

	//char* pText = Utils::char2HexString(text);
	user_data.Append(Utils::char2HexString(text).c_str());
	
	user_data.Append("00");
	user_data.Append("01");
	user_data.Append("01");

	int user_len = user_data.GetLength()/2;
	pdu.AppendFormat("%02X", user_len);

	std::string strUserData = CT2A(user_data);
	pdu.Append(strUserData.c_str());

	out_len = pdu.GetLength()/2 - 1;
	
	result = CT2A(pdu);
	return  out_len;
}

WAPPushPDU::~WAPPushPDU(void)
{
}
