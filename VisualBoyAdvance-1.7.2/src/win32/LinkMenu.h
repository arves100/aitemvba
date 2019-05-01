#if !defined(AFX_LINKMENU_H__31D2BC05_FD20_4CDC_9C13_EE1477423FC5__INCLUDED_)
#define AFX_LINKMENU_H__31D2BC05_FD20_4CDC_9C13_EE1477423FC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinkMenu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LinkMenu dialog

#include "../Link.h"

extern int linktimeout;
extern int linklog;
extern FILE *jjj;
extern int linkid;
extern LANLINKDATA lanlink;
extern int LanConnect(void);
extern int InitLink(void);
extern void CloseLink(void);
//extern void CloseLanLink(void);

class LinkMenu : public CDialog
{
// Construction
public:
	LinkMenu(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(LinkMenu)
	CEdit m_timeout;
	CEdit m_serverip;
	BOOL m_linklog;
	int m_lan;
	int m_numplayers;
	enum { IDD = IDD_LINK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LinkMenu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LinkMenu)
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnLinklog();
	afx_msg void OnLinkLan();
	virtual BOOL OnInitDialog();
	afx_msg void OnLinkConnect();
	afx_msg void OnLink2p();
	afx_msg void OnLink3p();
	afx_msg void OnLink4p();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKMENU_H__31D2BC05_FD20_4CDC_9C13_EE1477423FC5__INCLUDED_)
