// ColorControls.cpp : implementation file

#include "stdafx.h"
#include "ColorControls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CColorEdit

CColorEdit::CColorEdit()
{
	m_crBkColor = ::GetSysColor(COLOR_3DFACE);
	m_crTextColor = BLACK;
	m_brBkgnd.CreateSolidBrush(m_crBkColor);
}

CColorEdit::~CColorEdit()
{
}

BEGIN_MESSAGE_MAP(CColorEdit, CEdit)
	//{{AFX_MSG_MAP(CColorEdit)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorEdit message handlers

void CColorEdit::SetTextColor(COLORREF crColor)
{
	m_crTextColor = crColor;
	RedrawWindow();
}

void CColorEdit::SetBkColor(COLORREF crColor)
{
	m_crBkColor = crColor;
	m_brBkgnd.DeleteObject();
	m_brBkgnd.CreateSolidBrush(crColor);
	RedrawWindow();
}

HBRUSH CColorEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH hbr;
	hbr = (HBRUSH)m_brBkgnd;
	pDC->SetBkColor(m_crBkColor);
	pDC->SetTextColor(m_crTextColor);
	if (nCtlColor) nCtlColor += 0;
	return hbr;
}

BOOL CColorEdit::SetReadOnly(BOOL flag)
{
   if (flag == TRUE)
      SetBkColor(m_crBkColor);
   else
      SetBkColor(WHITE);
   return CEdit::SetReadOnly(flag);
}




/////////////////////////////////////////////////////////////////////////////
// CColorStatic

CColorStatic::CColorStatic()
{
	m_crBkColor = ::GetSysColor(COLOR_3DFACE);
	m_crTextColor = BLACK;
	m_brBkgnd.CreateSolidBrush(m_crBkColor);
}

CColorStatic::~CColorStatic()
{
}

BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic message handlers

void CColorStatic::SetTextColor(COLORREF crColor)
{
	m_crTextColor = crColor;
	RedrawWindow();
}

void CColorStatic::SetBkColor(COLORREF crColor)
{
	m_crBkColor = crColor;
	m_brBkgnd.DeleteObject();
	m_brBkgnd.CreateSolidBrush(crColor);
	RedrawWindow();
}

HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH hbr;
	hbr = (HBRUSH)m_brBkgnd;
	pDC->SetBkColor(m_crBkColor);
	pDC->SetTextColor(m_crTextColor);
	if (nCtlColor) nCtlColor += 0;
	return hbr;
}
