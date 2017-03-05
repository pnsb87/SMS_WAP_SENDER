// ByteBuffer.h: interface for the CByteBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BYTEBUFFER_H__295DD8B8_40E1_4562_B518_3AA1A076FC92__INCLUDED_)
#define AFX_BYTEBUFFER_H__295DD8B8_40E1_4562_B518_3AA1A076FC92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NotEnoughDataInByteBufferException.h"
#include "TerminatingZeroNotFoundException.h"
#include "EndlineNotFoundException.h"

class CByteBuffer  
{
public:
	BYTE ValueAt(int i);
	CString DumpWithNoFormat();
	void ResetBuffer();
	void AppendEndline();
	char * RemoveLine();
	void AppendCString(CString csString);
	CByteBuffer* ReadBuffer(int nSize);
	CString DumpHex(int NumsPerRow);
	void AppendBuffer(CByteBuffer* pBuffer);
	CByteBuffer* RemoveBuffer(int nSize);
	char* GetHexString(BYTE byte);
	void ShowHex(BYTE numsPerRow);
	CByteBuffer* Clone();
	char * RemoveString(int count);
	BOOL IsEmpty();
	BYTE ReadByte() ;
	BYTE RemoveByte();
	short RemoveShort();
	int RemoveInt();
	char * RemoveCString();
	
	BYTE* RemoveBytes(int count);
	int ReadInt();
	BYTE* ReadBytes(int count);
	
	char * GetHexDump();
	void AppendString(char * string);
	void AppendString(char * string, const char* Encoding);
	void AppendShort(short data);
	void AppendInt(int intNumber);
	void AppendCString(char *  string);
	void AppendByte(BYTE byte);
	void AppendBytes(BYTE* byteArray,int count);
	BYTE * GetBuffer();
	void AppendBytes(CByteBuffer buffer, int count);
	int GetLength();
	CByteBuffer();
	CByteBuffer(BYTE* data,int len);
	virtual ~CByteBuffer();
private : 
	BYTE* zero;
	int length;
	BYTE* buffer;
	void AppendString0(char* string, BOOL isCString, CString encoding);
	void RemoveBytes0(int count);
	void SetBuffer(BYTE* buffer,int len);
	void AppendBytes0(BYTE* data,int count);
};

#endif // !defined(AFX_BYTEBUFFER_H__295DD8B8_40E1_4562_B518_3AA1A076FC92__INCLUDED_)
