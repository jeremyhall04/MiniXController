#if !defined(AFX_ColorControls_H__INCLUDED_)
#define AFX_ColorControls_H__INCLUDED_
// ColorControls.h

#define CC_SYSCOLOR(ind) (0x80000000|((ind) & ~CLR_DEFAULT))
#define ctrlColBlue RGB(0x00,0x00,0xFF)
#define ctrlColSkyBlue RGB(0x32,0x99,0xCC)
#define ctrlColLightBlue RGB(0xC0,0xD9,0xD9)
#define ctrlColLightCyan RGB(0xE0,0xFF,0xFF)
#define ctrlColYellow RGB(0xFF,0xFF,0x00)
#define ctrlColGreen RGB(0x00,0xFF,0x00)
#define ctrlDarkGreen RGB(0x2F,0x4F,0x2F)
#define ctrlDarkGreenCopper RGB(0x4A,0x76,0x6E)
#define ctrlDarkOliveGreen RGB(0x4F,0x4F,0x2F)
#define ctrlColRed RGB(0xFF,0x00,0x00)
#define ctrlColBlack RGB(0x00,0x00,0x00)
#define ctrlColWhite RGB(0xFF,0xFF,0xFF)
#define ctrlColDefault CLR_DEFAULT
//#define ctrlColWinBk CC_SYSCOLOR(COLOR_WINDOW)
#define ctrlColWinBk GetSysColor(0x80000005L)
#define ctrlColWinText CC_SYSCOLOR(COLOR_WINDOWTEXT)
#define ctrlColBtnFace CC_SYSCOLOR(COLOR_BTNFACE)
#define ctrlColBtnFace3D CC_SYSCOLOR(COLOR_3DFACE)
#define ctrlColInfoBk CC_SYSCOLOR(COLOR_INFOBK)
#define ctrlColInfoText CC_SYSCOLOR(COLOR_INFOTEXT)
#define BLACK RGB(0x00,0x00,0x00)
#define BLUE RGB(0x00,0x00,0x7F)
#define GREEN RGB(0x00,0x7F,0x00)
#define RED RGB(0x7F,0x00,0x00)
#define LIGHTRED RGB(0xFF,0x00,0x00)
#define LIGHTGREEN RGB(0x00,0xFF,0x00)
#define LIGHTBLUE RGB(0x00,0x00,0xFF)
#define GRAY RGB(0xC0,0xC0,0xC0)
#define WHITE RGB(0xFF,0xFF,0xFF)
#define MAGENTA RGB(0xFF,0x00,0xFF)


/////////////////////////////////////////////////////////////////////////////
// CColorEdit Class

class CColorEdit : public CEdit
{
// Construction
public:
	CColorEdit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorEdit)
	//}}AFX_VIRTUAL

	void SetTextColor(COLORREF crColor);
	void SetBkColor(COLORREF crColor);
	BOOL SetReadOnly(BOOL flag = TRUE);
	virtual ~CColorEdit();

protected:
	CBrush m_brBkgnd;
	COLORREF m_crBkColor;
	COLORREF m_crTextColor;

	//{{AFX_MSG(CColorEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CColorStatic Class

class CColorStatic : public CStatic
{
// Construction
public:
	CColorStatic();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorStatic)
	//}}AFX_VIRTUAL

	void SetTextColor(COLORREF crColor);
	void SetBkColor(COLORREF crColor);
	virtual ~CColorStatic();

protected:
	CBrush m_brBkgnd;
	COLORREF m_crBkColor;
	COLORREF m_crTextColor;

	//{{AFX_MSG(CColorStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_ColorControls_H__INCLUDED_)
