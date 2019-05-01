// VisualBoyAdvance - Nintendo Gameboy/GameboyAdvance (TM) emulator.
// Copyright (C) 1999-2003 Forgotten
// Copyright (C) 2004 Forgotten and the VBA development team
// This file was written by denopqrihg

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or(at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

// Link.cpp : Emulation of GBA link cable
//

#include "GBA.h"
#include<stdio.h>
#include<afxwin.h>
#include "port.h"
#include "Link.h"
#include "win32/vba.h"
#include "win32/MainWnd.h"

#define UPDATE_REG(address, value) WRITE16LE(((u16 *)&ioMem[address]),value)
#define LINK_ID1FINISH 2
#define LINK_ID2FINISH 4
#define GBLINK_READY 8

int linktime = 0;
u8 tspeed=3;
u8 transfer=0;
LINKDATA *linkmem=NULL;
int linkid = 1;
HANDLE linksync[4];
extern int *extTicks;
int savedlinktime=0;
char inifile[] = "vba1.ini";
HANDLE mmf=NULL;
char linkevent[] = "VBA link event  ";
static int i, j;
int linktimeout = 500;
int linklog = 0;
FILE *jjj = NULL;
LANLINKDATA lanlink;


extern unsigned char *gbMemory;
extern int gbInterrupt;

int trspeed1[4] = {34080, 8520, 5680, 2840};
int trspeed2[4] = {65536, 16384, 10923, 5461};
int trspeed2_5[4] = {72527, 18132, 12088, 6044};
int trspeed3[4] = {99609, 24903, 16602, 8301};
int trspeed3_5[4] = {106608, 26652, 17768, 8884};
int trspeed4[4] = {133692, 33423, 22282, 11141};
int gbtime = 1024;

DWORD WINAPI LanLinkThread(void *);

void StartLink(WORD value){
	if(ioMem==NULL)
		return;
	if(!(READ16LE(&ioMem[0x134])&0x8000)){
			switch(value & 0x3000){
			case 0x2000: // Multiplayer
				if(value & 0x80){
					if(!linkid){  
						if(!transfer&&linkmem->numgbas>1){
							linkmem->linkflags &= !(LINK_ID1FINISH | LINK_ID2FINISH);
							ResetEvent(linksync[0]);
							linkmem->linkcmd[0] = ('M'<<8)+(value&3);
							linkmem->linkdata[0] = READ16LE(&ioMem[0x12a]);
							transfer = 1;
							if(linkmem->numtransfers!=0)
								savedlinktime = linktime;
							else
								savedlinktime = 0;
							linktime = 0;
							tspeed = value & 3;
							_asm{
								mov eax, ioMem;
								mov dword ptr [eax+0x120], 0xffffffff;
								mov dword ptr [eax+0x124], 0xffffffff;
							}
						}
					} else {
						value &= 0xff7f;
						value |= (transfer!=0)<<7;
					}
				}
				value &= 0xffbb;
				value |= (linkid ? 0xc : 8);

				UPDATE_REG(0x128, value);
			break;
			
			case 0:	// Normal 8-bit
				value |= 4;
				UPDATE_REG(0x128, value);
				if(linklog) fprintf(jjj, "Attempt to use 8-bit Normal mode %04x\n", value);
				break;

			case 0x1000: // Normal 32-bit
				value |= 4;
				UPDATE_REG(0x128, value);
				if(linklog) fprintf(jjj, "Attempt to use 32-bit Normal mode %04x %x%x\n", value, READ16LE(&ioMem[0x122]), READ16LE(&ioMem[0x120]));
				break;

			case 0x3000:	// UART
				if(linklog&&!linkid) fprintf(jjj, "Attempt to use UART mode %04x\n", value);
				UPDATE_REG(0x128, value);
				break;
			}
	}
}

void StartGPLink(u16 value){
	if(!value){
		UPDATE_REG(0x134, 0);
		return;
	}
	switch(value&0xC000){
	case 0:
	case 0x4000:
		switch(READ16LE(&ioMem[0x128])&0x3000){
		case 0x2000:
			value = READ16LE(&ioMem[0x128]);
			value &= 0xffbb;
			value |= (linkid ? 0xc : 8);
			UPDATE_REG(0x128, value);
			return;
			break;
		}
		break;
	case 0x8000:
		if(linklog) 
			if(value==0x8000)
				fprintf(jjj, "Circuit reset\n");
			else
				fprintf(jjj, "Attempt to use General-purpose mode %04x\n", value);
		UPDATE_REG(0x134, value);
		break;
	case 0xC000:
		UPDATE_REG(0x134, value);
		break;
	}
	return;
}

void StartJOYLink(u16 value){
	if(!value){
		UPDATE_REG(0x140, 0);
		return;
	}
	if(READ16LE(&ioMem[0x134])&0xC000==0xC000){
		if(linklog) fprintf(jjj, "Attempt to use JOY-BUS mode %04x\n", value);
	}
	return;
}

void LinkUpdate(void){
	if(linkid){	
		savedlinktime = linkmem->lastlinktime;
		if(linkid==1||(linkid==2&&(linkmem->linkflags&LINK_ID1FINISH))||(linkid==3&&(linkmem->linkflags&LINK_ID2FINISH))){
			if(!transfer&&linktime>=savedlinktime&&linkmem->numtransfers){
				if(linkid==1){
					linkmem->linkdata[1] = READ16LE(&ioMem[0x12a]);
				}
				else 
				if(linkid==2){
					linkmem->linkflags &= !LINK_ID1FINISH;
					linkmem->linkdata[2] = READ16LE(&ioMem[0x12a]);
				}
				else if(linkid==3){
					linkmem->linkflags &= !LINK_ID2FINISH;
					linkmem->linkdata[3] = READ16LE(&ioMem[0x12a]);
				}

				if(linkmem->numtransfers!=1)
					linktime -= savedlinktime;
				else
					linktime = 0;

				if(linkmem->numtransfers==1&&lanlink.active)
					linkmem->numtransfers = 2;

				switch((linkmem->linkcmd[0])>>8){
				case 'M':
					tspeed = (linkmem->linkcmd[0]) & 3;
					transfer = 1;
					_asm{
						mov eax, ioMem;
						mov dword ptr [eax+0x120], 0xffffffff;
						mov dword ptr [eax+0x124], 0xffffffff;
						or byte ptr [eax+0x128], 0x80;
					}
					break;
				}
			}
		}
	}
	
	if(!transfer) return;

	if(transfer == 1 && linktime >= trspeed1[tspeed]){
		UPDATE_REG(0x120, linkmem->linkdata[0]);
		transfer++;
	}
	
	if(transfer == 2 && linktime >= trspeed2[tspeed]){
		if(!linkid){
			linkmem->numtransfers++;
			if(linkmem->numtransfers==0)
				linkmem->numtransfers=2;
			linkmem->lastlinktime=savedlinktime;
				SetEvent(linksync[1]);
			WaitForSingleObject(linksync[0], linktimeout);
			ResetEvent(linksync[0]);
		}
		if(linklog)
			fprintf(jjj, "%04x %04x %04x %04x %10u\n", 
				linkmem->linkdata[0], linkmem->linkdata[1], linkmem->linkdata[2], linkmem->linkdata[3], linkmem->lastlinktime);
		UPDATE_REG(0x122, linkmem->linkdata[1]);
		transfer++;
	}

	if(transfer == 3 && linktime >= trspeed2_5[tspeed] && linkmem->numgbas==2){
		if(linkid){
			SetEvent(linksync[0]);
			if(WaitForSingleObject(linksync[1], linktimeout)==WAIT_TIMEOUT){
				linkmem->numtransfers=0;
			}
			ResetEvent(linksync[1]);
		}
		transfer = 0;
		linktime -= trspeed2_5[tspeed];
		if((*(u16*)&ioMem[0x128]) & 0x4000){
			IF |= 0x80;
			UPDATE_REG(0x202, IF);
		}
		UPDATE_REG(0x128, (*(u16 *)&ioMem[0x128] & 0xff0f) | (linkid << 4));
		linkmem->linkdata[linkid] = 0xffff;
	}
	
	if(transfer == 3 && linktime >= trspeed3[tspeed]){
		if(linkid==1){
			linkmem->linkflags |= LINK_ID1FINISH;
			if(linkmem->numtransfers!=1)
				SetEvent(linksync[2]);
			if(WaitForSingleObject(linksync[1], linktimeout)==WAIT_TIMEOUT){
				linkmem->numtransfers=0;
			}
			ResetEvent(linksync[1]);
		}
		UPDATE_REG(0x124, linkmem->linkdata[2]);
		transfer++;
	}

	if(transfer == 4 && linktime >= trspeed3_5[tspeed] && linkmem->numgbas==3){
		if(linkid==2){
			SetEvent(linksync[0]);
			if(WaitForSingleObject(linksync[2], linktimeout)==WAIT_TIMEOUT){
				linkmem->numtransfers=0;
			}
		ResetEvent(linksync[2]);
		}
		transfer = 0;
		linktime -= trspeed3_5[tspeed];
		if((*(u16*)&ioMem[0x128]) & 0x4000){
			IF |= 0x80;
			UPDATE_REG(0x202, IF);
		}
		UPDATE_REG(0x128, (*(u16 *)&ioMem[0x128] & 0xff0f) | (linkid << 4));
		linkmem->linkdata[linkid] = 0xffff;
	}

	if(transfer == 4 && linktime >= trspeed4[tspeed]){
		if(linkid==2){
			linkmem->linkflags |= LINK_ID2FINISH;
			if(linkmem->numtransfers!=1)
				SetEvent(linksync[3]);
			if(WaitForSingleObject(linksync[2], linktimeout)==WAIT_TIMEOUT){
				linkmem->numtransfers=0;
			}
			ResetEvent(linksync[2]);
		}
		UPDATE_REG(0x126, linkmem->linkdata[3]);
		if(linkid==3){
			SetEvent(linksync[0]);
			if(WaitForSingleObject(linksync[3], linktimeout)==WAIT_TIMEOUT){
				linkmem->numtransfers=0;
			}
		ResetEvent(linksync[3]);
		}
		transfer = 0;
		linktime -= trspeed4[tspeed];
		if((*(u16*)&ioMem[0x128]) & 0x4000){
			IF |= 0x80;
			UPDATE_REG(0x202, IF);
		}
		UPDATE_REG(0x128, (*(u16 *)&ioMem[0x128] & 0xff0f) | (linkid << 4));
		linkmem->linkdata[linkid] = 0xffff;
	}

return;
}

void gbLinkStart(u8 value){
// Not in this version :-)
}


void gbLinkUpdate(void){	
}

int InitLink(void){
	if(lanlink.active){
		WSADATA wsadata;
		WORD version;
		if((linkmem=(LINKDATA*)malloc(sizeof(LINKDATA)))==NULL){
			MessageBox(NULL, "Not enough memory.", "Error!", MB_OK);
			return 0;
		}
		linkid = 0;
		inifile[3]='1';
		lanlink.linkthread = NULL;
		
		version=MAKEWORD(1,1);
		if(WSAStartup(version, &wsadata)!=0){
			WSACleanup();
			return -1;
		}
		
		if((lanlink.serversocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==INVALID_SOCKET){
			MessageBox(NULL, "Couldn't create socket.", "Error!", MB_OK);
			WSACleanup();
			return -1;
		}

		for(i=0;i<4;i++){
			if((linksync[i]=CreateEvent(NULL, true, false, NULL))==NULL){
				closesocket(lanlink.serversocket);
				WSACleanup();
				return -1;
			}
		}

		lanlink.numgbas = 1;
	} else {

		if((mmf=CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, sizeof(LINKDATA), "VBA link memory"))==NULL){	  
			MessageBox(NULL, "Error creating file mapping", "Error", MB_OK|MB_ICONEXCLAMATION);
		  return 0;
	  }
		
	  if(GetLastError() == ERROR_ALREADY_EXISTS)
		  linkid = 1;
		else
 		  linkid = 0;
		  
	  if((linkmem=(LINKDATA *)MapViewOfFile(mmf, FILE_MAP_WRITE, 0, 0, sizeof(LINKDATA)))==NULL){
		  CloseHandle(mmf);
		  MessageBox(NULL, "Error mapping file", "Error", MB_OK|MB_ICONEXCLAMATION);
		  return 0;
	  }

	  if(linkmem->linkflags&LINK_PARENTLOST)
		  linkid = 0;

		if(linkid==0){
			inifile[3]='1';
			if(linkmem->linkflags&LINK_PARENTLOST){
				linkmem->numgbas++;
				linkmem->linkflags &= !LINK_PARENTLOST;
			}
			else
				linkmem->numgbas=1;

			for(i=0;i<4;i++){
				linkevent[15]=(char)i+'1';
				if((linksync[i]=CreateEvent(NULL, true, false, linkevent))==NULL){
					UnmapViewOfFile(linkmem);
					CloseHandle(mmf);
					for(j=0;j<i;j++){
						CloseHandle(linksync[j]);
					}
					MessageBox(NULL, "Error opening event", "Error", MB_OK|MB_ICONEXCLAMATION);
					return 0;
				}
			}
		} else {
			linkid=linkmem->numgbas;
			linkmem->numgbas++;
			linklog = 0;
			if(linkmem->numgbas>4){
				linkmem->numgbas=4;
				MessageBox(NULL, "5 or more GBAs not supported.", "Error!", MB_OK|MB_ICONEXCLAMATION);
				UnmapViewOfFile(linkmem);
				CloseHandle(mmf);
				return 0;
			}
			inifile[3]=(char)linkmem->numgbas+'0';
			for(i=0;i<4;i++){
				linkevent[15]=(char)i+'1';
				if((linksync[i]=OpenEvent(EVENT_ALL_ACCESS, false, linkevent))==NULL){
					CloseHandle(mmf);
					UnmapViewOfFile(linkmem);
					for(j=0;j<i;j++){
						CloseHandle(linksync[j]);
					}
					MessageBox(NULL, "Error opening event", "Error", MB_OK|MB_ICONEXCLAMATION);
					return 0;
				}
			}
		}
	}  
	linkmem->lastlinktime=0xffffffff;
	linkmem->numtransfers=0;
	linkmem->linkflags=0;
	for(i=0;i<4;i++)
		linkmem->linkdata[i] = 0xffff;

	theApp.vbaLinkID = linkmem->numgbas;

	

return 1;
}

void CloseLanLink(void){
	char buffer[8];
	buffer[0] = 'S';
	send(lanlink.serversocket, buffer, 8, 0);
	closesocket(lanlink.serversocket);
	free(linkmem);
	WSACleanup();
	for(i=0;i<4;i++)
		CloseHandle(linksync[i]);
	if(lanlink.linkthread!=NULL) CloseHandle(lanlink.linkthread);
	if(linklog) fclose(jjj);
}

void CloseLink(void){
	if(lanlink.active){
		CloseLanLink();
		return;
	}
	linkmem->numgbas--;
	if(!linkid&&linkmem->numgbas!=0)
		linkmem->linkflags|=LINK_PARENTLOST;
	CloseHandle(mmf);
	UnmapViewOfFile(linkmem);

	for(i=0;i<4;i++){
		if(linksync[i]!=NULL){
			PulseEvent(linksync[i]);
			CloseHandle(linksync[i]);
		}
	}
	if(linklog&&!linkid) fclose(jjj);
return;
}

int LanConnect(void){
	DWORD ehh;
	LPHOSTENT hostentry;
	SOCKADDR_IN serverinfo;
	if(lanlink.linkthread!=NULL){
		CloseHandle(lanlink.linkthread);
		lanlink.linkthread = NULL;
	}
	
	hostentry = gethostbyaddr((const char *)&lanlink.serveraddress, sizeof(IN_ADDR), AF_INET);
	if(hostentry==NULL){
		return WSAGetLastError();
	}

	ZeroMemory(&serverinfo, sizeof(SOCKADDR_IN));
	serverinfo.sin_family = AF_INET;
	serverinfo.sin_port = htons(6477 + lanlink.numgbas);
	serverinfo.sin_addr = *((LPIN_ADDR)*hostentry->h_addr_list);

	if(connect(lanlink.serversocket, (LPSOCKADDR)&serverinfo, sizeof(SOCKADDR))==SOCKET_ERROR){
		return WSAGetLastError();
	}

	ResetEvent(linksync[1]);
	ResetEvent(linksync[0]);

	if((lanlink.linkthread=CreateThread(NULL, 0, LanLinkThread, NULL, 0, &ehh))==NULL){
		return -1;
	}
	
	return 0;
}

DWORD WINAPI LanLinkThread(void *number){
	SOCKET mysocket = lanlink.serversocket;
	int numbytes = 0, howmuch, whatwait;
	char *inbuffer, outbuffer[32], *oldbuffer;

	inbuffer = (char*)malloc(32);

	oldbuffer = inbuffer;
	
	while(numbytes<4){
		numbytes += recv(mysocket, inbuffer+numbytes, 4, 0);
	}

	numbytes = 0;
	
	MessageBox(NULL, "Connected.", "Link", MB_OK);
	
	linkid = inbuffer[0];

	linkmem->numgbas = lanlink.numgbas + 1;
	
	howmuch = (lanlink.numgbas)<<2;
	if(linkid) howmuch += 4;

	if(linkid == lanlink.numgbas)
		whatwait = 0;
	else
		whatwait = linkid + 1;

	linkmem->numtransfers = 0;

	if(!linkid) goto firstloop;
	
	while(true){
		
		numbytes = 0;

		while(numbytes<howmuch)
			numbytes += recv(mysocket, inbuffer+numbytes, 32, 0);
		
		while(numbytes){
			switch((inbuffer[0])&0x80){
			case 0:
				switch(inbuffer[0]){
				case 'M':
					linkmem->linkdata[((inbuffer[1])&0xc0)>>6] = *((u16*)inbuffer+1);
					linkmem->linkcmd[0] = ntohs(*(u16*)inbuffer);
					if(linkid==2)
						linkmem->linkflags |= LINK_ID1FINISH;
					if(linkid==3){
						linkmem->linkflags |= LINK_ID1FINISH;
						linkmem->linkflags |= LINK_ID2FINISH;
					}
					break;
				case 'S':
					MessageBox(NULL, "Remote player disconnected.", "Link", MB_OK);
					linkmem->numgbas = 1;
					CloseHandle(lanlink.linkthread);
					lanlink.linkthread = NULL;
					ExitThread(0);
					break;
				default:
					MessageBox(NULL, "Unknown transfer mode", "Error!", MB_OK);
					break;
				}
				break;
			case 0x80:
				inbuffer[0] &= 0x7f;
				linkmem->lastlinktime = ntohl(*(int*)inbuffer);
				break;
			}
			numbytes -= 4;
			inbuffer += 4;
		}
		
		inbuffer = oldbuffer;

		if(linkid&&(++linkmem->numtransfers)==0){
			linkmem->numtransfers = 2;
		}
		
		SetEvent(linksync[linkid]);
firstloop:
		WaitForSingleObject(linksync[whatwait], INFINITE);
		ResetEvent(linksync[whatwait]);
		
		outbuffer[0] = (linkmem->linkcmd[0])>>8;
		outbuffer[1] = tspeed | (linkid << 6);
		*((u16*)outbuffer+1) = linkmem->linkdata[linkid];

		if(linkid){
			send(mysocket, outbuffer, 4, 0);
		}
		else {
			*((int*)outbuffer+1) = htonl(linkmem->lastlinktime);
			outbuffer[4] |= 0x80;
			send(mysocket, outbuffer, 8, 0);
		}
		
	}
	return 0;
}
