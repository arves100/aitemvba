// LinkMenu.cpp : implementation file
//

#include "stdafx.h"
#include "vba.h"
#include "LinkMenu.h"
//#include "LinkWait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LinkMenu dialog


LinkMenu::LinkMenu(CWnd* pParent /*=NULL*/)
	: CDialog(LinkMenu::IDD, pParent)
{
	//{{AFX_DATA_INIT(LinkMenu)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void LinkMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LinkMenu)
	DDX_Control(pDX, IDC_TIMEOUT, m_timeout);
	DDX_Check(pDX, IDC_LINKLOG, m_linklog);
	DDX_Radio(pDX, IDC_LINK_LAN, m_lan);
	DDX_Radio(pDX, IDC_LINK2P, m_numplayers);
	DDX_Control(pDX, IDC_SERVERIP, m_serverip);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LinkMenu, CDialog)
	//{{AFX_MSG_MAP(LinkMenu)
	ON_BN_CLICKED(ID_OK, OnOk)
	ON_BN_CLICKED(IDC_LINKLOG, OnLinklog)
	ON_BN_CLICKED(IDC_LINK_LAN, OnLinkLan)
	ON_BN_CLICKED(IDC_LINKCONNECT, OnLinkConnect)
	ON_BN_CLICKED(IDC_LINK2P, OnLink2p)
	ON_BN_CLICKED(IDC_LINK3P, OnLink3p)
	ON_BN_CLICKED(ID_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_LINK_ONECOMP, OnLinkLan)
	ON_BN_CLICKED(IDC_LINK4P, OnLink4p)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
 

/////////////////////////////////////////////////////////////////////////////
// LinkMenu message handlers

BOOL LinkMenu::OnInitDialog(){
	CDialog::OnInitDialog();

	m_numplayers = lanlink.numgbas-1;
	m_timeout.LimitText(30);
	m_linklog = linklog != 0;

	CString buffer;
	buffer.Format("%d", linktimeout);
	m_timeout.SetWindowText(buffer);

	if(linkid&&!lanlink.active) GetDlgItem(IDC_LINKLOG)->EnableWindow(FALSE);
	if(!lanlink.active){
		m_lan = 1;
		GetDlgItem(IDC_LINKCONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LINK2P)->EnableWindow(FALSE);
		GetDlgItem(IDC_LINK3P)->EnableWindow(FALSE);
		GetDlgItem(IDC_LINK4P)->EnableWindow(FALSE);
		GetDlgItem(IDC_SERVERIP)->EnableWindow(FALSE);
	} else {
		m_lan = 0;
		GetDlgItem(IDC_LINKCONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_LINK2P)->EnableWindow(TRUE);
		GetDlgItem(IDC_LINK3P)->EnableWindow(TRUE);
		GetDlgItem(IDC_LINK4P)->EnableWindow(TRUE);
		GetDlgItem(IDC_SERVERIP)->EnableWindow(TRUE);
	}

  UpdateData(FALSE);
  CenterWindow();
  
  return TRUE;
}
	

void LinkMenu::OnOk() 
{
	CString buffer;

	if(linklog&&jjj==NULL){
			if((jjj=fopen("vbalog.txt", "wt"))==NULL){
				PostQuitMessage(0);
			}
			fprintf(jjj, "GBA0 GBA1 GBA2 GBA3 clocks between transfers\n");
	}

	if(!linklog&&jjj!=NULL){
		fclose(jjj);
		jjj = NULL;
	}
	
	m_timeout.GetWindowText(buffer);

	linktimeout = atoi(buffer);
	EndDialog(linktimeout);
	
}

void LinkMenu::OnCancel()
{
	EndDialog(-1);
}

void LinkMenu::OnLinklog()
{
	linklog ^= 1;
}

void LinkMenu::OnLinkLan()
{
	lanlink.wasactive ^= 1;

	systemMessage(0, "Setting will be effective the next time you start the emulator");      
	GetDlgItem(IDC_LINKCONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_LINK2P)->EnableWindow(FALSE);
	GetDlgItem(IDC_LINK3P)->EnableWindow(FALSE);
	GetDlgItem(IDC_LINK4P)->EnableWindow(FALSE);
	GetDlgItem(IDC_SERVERIP)->EnableWindow(FALSE);

}


void LinkMenu::OnLinkConnect() 
{
	CString buffer;
	int nret;

	m_serverip.GetWindowText(buffer);
	lanlink.serveraddress.S_un.S_addr = inet_addr(buffer);
	
	if((nret=LanConnect())!=0){
		char errror[100];
		sprintf(errror, "An error occured, please try again.\nError code: %d", nret);
		MessageBox(errror, "Error!", MB_OK);
	}
	return;
}



void LinkMenu::OnLink2p() 
{
	lanlink.numgbas = 1;	
}

void LinkMenu::OnLink3p() 
{
	lanlink.numgbas = 2;	
}

void LinkMenu::OnLink4p() 
{
	lanlink.numgbas = 3;	
}
