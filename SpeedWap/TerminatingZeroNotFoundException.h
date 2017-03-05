// TerminatingZeroNotFoundException.h: interface for the CTerminatingZeroNotFoundException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERMINATINGZERONOTFOUNDEXCEPTION_H__24BCB5B8_4BBC_4AC6_B09B_F7909E3B4BD1__INCLUDED_)
#define AFX_TERMINATINGZERONOTFOUNDEXCEPTION_H__24BCB5B8_4BBC_4AC6_B09B_F7909E3B4BD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afx.h"
class CTerminatingZeroNotFoundException : public CException  
{
public:
	CTerminatingZeroNotFoundException();
	virtual ~CTerminatingZeroNotFoundException();

};

#endif // !defined(AFX_TERMINATINGZERONOTFOUNDEXCEPTION_H__24BCB5B8_4BBC_4AC6_B09B_F7909E3B4BD1__INCLUDED_)
