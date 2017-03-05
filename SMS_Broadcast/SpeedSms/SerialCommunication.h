#ifndef __SERIALCOMMUNICATION_H
#define __SERIALCOMMUNICATION_H
//#include <stdint.h>

#define MAX_BUFFER_SIZE 1500
#include <string>

using namespace std;

typedef struct SERIAL_PARAM
{
	CString szName;
	HANDLE hCom;
	COMMTIMEOUTS m_CommTimeouts;

}SERIAL_PARAM;

class SerialCommunication
{
public:
	SerialCommunication();
	int Open(CString szName);
	int Close();
	int Write(const char* data, int len);
	int WriteWaitReply(const char* data);
	int Read(char* data_out, int data_out_len);

	int SendSms(const char* szText, const char* szNumber);
	int SendWap(const char* szText, const char* szUrl, const char* szNumber);

	int ChangeSmsMode();
	int ChangeWapMode();

	SERIAL_PARAM* GetParam()
	{
		return &m_serialParam;
	}

	string GetNumber();

	~SerialCommunication(void);
private:
	SERIAL_PARAM m_serialParam;
};
#endif

