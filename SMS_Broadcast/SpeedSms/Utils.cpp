#include "StdAfx.h"
#include "Utils.h"
#include <string>
#include <iostream>
#include <sstream>
Utils::Utils(void)
{
}
char hexSym[] = {
     '0', '1', '2', '3', '4', '5', '6', '7',
     '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
 };
std::string Utils::char2HexString(std::string& buffer)
{
	std::string HexString;

	for(unsigned int i=0; i< buffer.length(); i++)
	{
		char temp[5];
		int hi = (buffer.at(i)>>4) & 0x0f;
		int low = (buffer.at(i)) & 0x0f;
		sprintf_s(temp, "%c%c", toupper(hexSym[hi]), toupper(hexSym[low]));

		HexString.append(temp);
	}

	return HexString;
}
void Utils::Dec2Hex(int val, char* result)
{
	sprintf(result, "%02X", val);
}

std::string Utils::string2BCD(std::string& mobile, int& out_len)
{
	if(mobile.at(0) == '0')
		mobile.replace(0, 1, "84");	
	out_len = mobile.length();

	if(mobile.length() % 2)
		mobile.append("F");

	std::string BCDmobile(mobile);

	for(unsigned int i=0; i< mobile.length(); i+=2)
	{
		BCDmobile.at(i) = mobile.at(i+1);
		BCDmobile.at(i+1) = mobile.at(i);
	}

	return BCDmobile;
}
Utils::~Utils(void)
{
}
