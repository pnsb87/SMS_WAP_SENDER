// EndlineNotFoundException.h: interface for the CEndlineNotFoundException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENDLINENOTFOUNDEXCEPTION_H__87D690DF_9A4D_4760_BC9D_AAB2CA40E4DF__INCLUDED_)
#define AFX_ENDLINENOTFOUNDEXCEPTION_H__87D690DF_9A4D_4760_BC9D_AAB2CA40E4DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afx.h"
class CEndlineNotFoundException : public CException  
{
public:
	CEndlineNotFoundException();
	virtual ~CEndlineNotFoundException();

};

#endif // !defined(AFX_ENDLINENOTFOUNDEXCEPTION_H__87D690DF_9A4D_4760_BC9D_AAB2CA40E4DF__INCLUDED_)
