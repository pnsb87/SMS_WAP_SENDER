#ifndef __SERIALCOMMUNICATION_H
#define __SERIALCOMMUNICATION_H
#include <stdint.h>

#define MAX_BUFFER_SIZE 1500

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
	int Read(char* data_out, int data_out_len);

	SERIAL_PARAM* GetParam()
	{
		return &m_serialParam;
	}

	~SerialCommunication(void);
private:
	SERIAL_PARAM m_serialParam;
};
#endif

