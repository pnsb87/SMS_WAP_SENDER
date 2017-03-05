// NotEnoughDataInByteBufferException.h: interface for the CNotEnoughDataInByteBufferException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTENOUGHDATAINBYTEBUFFEREXCEPTION_H__FD617C15_9840_4A91_991A_881579A8BB56__INCLUDED_)
#define AFX_NOTENOUGHDATAINBYTEBUFFEREXCEPTION_H__FD617C15_9840_4A91_991A_881579A8BB56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afx.h"
class CNotEnoughDataInByteBufferException : public CException  
{
public:
	CNotEnoughDataInByteBufferException();
	virtual ~CNotEnoughDataInByteBufferException();

};

#endif // !defined(AFX_NOTENOUGHDATAINBYTEBUFFEREXCEPTION_H__FD617C15_9840_4A91_991A_881579A8BB56__INCLUDED_)
