#if !defined(AFX_CLivePropertySheet_H__621F0E09_3C98_4A3E_A023_1426A45FD56B__INCLUDED_)
#define AFX_CLivePropertySheet_H__621F0E09_3C98_4A3E_A023_1426A45FD56B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CLivePropertySheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLivePropertySheet

#include "LiveImagePage.h"
#include "LivenessResultPage.h"

class CLivePropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CLivePropertySheet)

// Construction
public:
	CLivePropertySheet( CWnd* pParentWnd = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLivePropertySheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:

	CLivenessResultPage m_livenessPage;
	CLiveImagePage  m_imagePage;

public:
	virtual ~CLivePropertySheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLivePropertySheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLivePropertySheet_H__621F0E09_3C98_4A3E_A023_1426A45FD56B__INCLUDED_)
