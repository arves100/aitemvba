#define WIN32_LEAN_AND_MEAN

#include<windows.h>
#include<winsock.h>
#include<stdio.h>

#define BACKLOG 4

SOCKET soocket[3];
HANDLE threaad;
HANDLE eveent;
int errror = false;
bool terminate = false;

DWORD WINAPI ServerThread1(LPVOID);
DWORD WINAPI ServerThread2(LPVOID);
DWORD WINAPI ServerThread3(LPVOID);

LPTHREAD_START_ROUTINE ServerThread[3] = {ServerThread1, ServerThread2, ServerThread3};

int main(void){
	WSADATA wsadata;
	int i, players;
	SOCKADDR_IN serverinfo;
	LPHOSTENT kjh;
	IN_ADDR nji;
	char nameppp[100];

	printf("Select number of players (2-4): ");
	do {
		players = getchar() - '2';
	} while(players<0||players>2);
	
	if(WSAStartup(MAKEWORD(1, 1), &wsadata)!=0){
		printf("Winsock couldn't be initialized.\n");
		return 1;
	}
	
	for(i=0;i<3;i++){
		if((soocket[i]=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==INVALID_SOCKET){
			printf("Couldn't create socket (Error %d).\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
	}

	serverinfo.sin_family = AF_INET;
	serverinfo.sin_addr.S_un.S_addr = INADDR_ANY;

	for(i=0;i<3;i++){
		serverinfo.sin_port = htons(6478+i);
		if(bind(soocket[i], (SOCKADDR*)&serverinfo, sizeof(SOCKADDR_IN))==SOCKET_ERROR){
			for(i=0;i<3;i++)
				closesocket(soocket[i]);
			printf("Error during bind(). Error code: %d\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
	}

	for(i=0;i<3;i++){
		if(listen(soocket[i], BACKLOG)==SOCKET_ERROR){
			printf("Error during listen(). Error code: %d\n", WSAGetLastError());
			for(i=0;i<3;i++)
				closesocket(soocket[i]);
			WSACleanup();
			return 1;
		}
	}

	gethostname(nameppp, 100);
	kjh = gethostbyname(nameppp);
	memcpy(&nji, kjh->h_addr_list[0], sizeof(IN_ADDR));
	printf("Server address: %s\n", inet_ntoa(nji));

	if((threaad=CreateThread(NULL, 0, ServerThread[players], &i, 0, 0))==NULL){
		printf("Couldn't create server thread.\n");
			for(i=0;i<3;i++)
				closesocket(soocket[i]);
			WSACleanup();
			return 1;
	}
	
	if((eveent=CreateEvent(NULL, true, false, NULL))==NULL){
		printf("Couldn't create event.\n");
		for(i=0;i<3;i++)
				closesocket(soocket[i]);
		WSACleanup();
		return 1;
	}

	printf("VBALink server running...\n");
	
	WaitForSingleObject(eveent, INFINITE);
	if(errror){
		printf("Errrrrrrrrrrroooooooooooor!!!!!!!!!!!!!");
	}
	printf("Press enter.\n");
	getchar();
	
	for(i=0;i<3;i++){
		closesocket(soocket[i]);
	}
	terminate = true;
	CloseHandle(threaad);
	WSACleanup();
	return 0;
}

DWORD WINAPI ServerThread1(LPVOID number){
	SOCKET clientsocket[2];
	char buffer[32];
	int numbytes = 0;
	SOCKADDR_IN clientinfo;
	int sizee = sizeof(SOCKADDR_IN);
	if((clientsocket[0]=accept(soocket[0], (LPSOCKADDR)&clientinfo, &sizee))==INVALID_SOCKET){
		errror =true;
		ExitThread(0);
	}

	printf("Player #1 connected from %s\n", inet_ntoa(clientinfo.sin_addr));

	if((clientsocket[1]=accept(soocket[0], (LPSOCKADDR)&clientinfo, &sizee))==INVALID_SOCKET){
		errror =true;
		ExitThread(0);
	}

	printf("Player #2 connected from %s\n", inet_ntoa(clientinfo.sin_addr));

	buffer[0] = 0;
	send(clientsocket[0], buffer, 4, 0);
	buffer[0] = 1;
	send(clientsocket[1], buffer, 4, 0);
	printf("OK.\n");
	while(true){
		if(terminate){
			ExitThread(0);
		}
		numbytes = 0;
		while(numbytes<8){
			numbytes += recv(clientsocket[0], buffer+numbytes, 8, 0);
		}
		//putchar('k');
		if(buffer[0]=='S'||buffer[4]=='S'){
			send(clientsocket[1], buffer, 8, 0);
			closesocket(clientsocket[0]);
			closesocket(clientsocket[1]);
			SetEvent(eveent);
			ExitThread(0);
		}
		send(clientsocket[1], buffer, 8, 0);
		numbytes = 0;
		while(numbytes<4){
			numbytes += recv(clientsocket[1], buffer+numbytes, 4, 0);
		}
		if(buffer[0]=='S'){
			send(clientsocket[0], buffer, 8, 0);
			closesocket(clientsocket[0]);
			closesocket(clientsocket[1]);
			SetEvent(eveent);
			ExitThread(0);
		}
		send(clientsocket[0], buffer, 4, 0);
	}
	return 0;
}

DWORD WINAPI ServerThread2(LPVOID number){
	SOCKET clientsocket[3];
	char buffer[32];
	DWORD data[3] = {0, 0xffff434d, 0xffff834d};
	int numbytes = 0, linktime;
	SOCKADDR_IN clientinfo;
	int sizee = sizeof(SOCKADDR_IN);
	if((clientsocket[0]=accept(soocket[1], (LPSOCKADDR)&clientinfo, &sizee))==INVALID_SOCKET){
		errror =true;
		ExitThread(0);
	}

	printf("Player #1 connected from %s\n", inet_ntoa(clientinfo.sin_addr));

	if((clientsocket[1]=accept(soocket[1], (LPSOCKADDR)&clientinfo, &sizee))==INVALID_SOCKET){
		errror =true;
		ExitThread(0);
	}

	printf("Player #2 connected from %s\n", inet_ntoa(clientinfo.sin_addr));

	if((clientsocket[2]=accept(soocket[1], (LPSOCKADDR)&clientinfo, &sizee))==INVALID_SOCKET){
		errror =true;
		ExitThread(0);
	}

	printf("Player #3 connected from %s\n", inet_ntoa(clientinfo.sin_addr));

	buffer[0] = 0;
	send(clientsocket[0], buffer, 4, 0);
	buffer[0] = 1;
	send(clientsocket[1], buffer, 4, 0);
	buffer[0] = 2;
	send(clientsocket[2], buffer, 4, 0);

	printf("OK.\n");

	while(true){
		if(terminate){
			ExitThread(0);
		}
		numbytes = 0;
		while(numbytes<8){
			numbytes += recv(clientsocket[0], buffer+numbytes, 8, 0);
		}
		if(buffer[0]=='S'){
			send(clientsocket[1], buffer, 12, 0);
			send(clientsocket[2], buffer, 12, 0);
			closesocket(clientsocket[0]);
			closesocket(clientsocket[1]);
			closesocket(clientsocket[2]);
			SetEvent(eveent);
			ExitThread(0);
		}

		data[0] = *(DWORD*)buffer;
		linktime = *((int*)buffer+1);
		*((DWORD*)buffer+2) = data[2];
		send(clientsocket[1], buffer, 12, 0);
		
		*((DWORD*)buffer+2) = data[1];
		send(clientsocket[2], buffer, 12, 0);

		numbytes = 0;
		while(numbytes<4){
			numbytes += recv(clientsocket[1], buffer+numbytes, 4, 0);
		}
		if(buffer[0]=='S'){
			send(clientsocket[0], buffer, 8, 0);
			send(clientsocket[2], buffer, 12, 0);
			closesocket(clientsocket[0]);
			closesocket(clientsocket[1]);
			closesocket(clientsocket[2]);
			SetEvent(eveent);
			ExitThread(0);
		}

		data[1] = *(DWORD*)buffer;
		
		numbytes = 4;
		while(numbytes<8){
			numbytes += recv(clientsocket[2], buffer+numbytes, 4, 0);
		}
		if(buffer[0]=='S'){
			send(clientsocket[0], buffer, 8, 0);
			send(clientsocket[1], buffer, 12, 0);
			closesocket(clientsocket[0]);
			closesocket(clientsocket[1]);
			closesocket(clientsocket[2]);
			SetEvent(eveent);
			ExitThread(0);
		}
		data[2] = *((DWORD*)buffer+1);
		send(clientsocket[0], buffer, 8, 0);

	}
	return 0;
}

DWORD WINAPI ServerThread3(LPVOID number){
SOCKET clientsocket[4];
	char buffer[32];
	DWORD data[4] = {0, 0xffff434d, 0xffff834d, 0xffffc34d};
	int numbytes = 0, linktime;
	SOCKADDR_IN clientinfo;
	int sizee = sizeof(SOCKADDR_IN);
	if((clientsocket[0]=accept(soocket[2], (LPSOCKADDR)&clientinfo, &sizee))==INVALID_SOCKET){
		errror =true;
		ExitThread(0);
	}

	printf("Player #1 connected from %s\n", inet_ntoa(clientinfo.sin_addr));

	if((clientsocket[1]=accept(soocket[2], (LPSOCKADDR)&clientinfo, &sizee))==INVALID_SOCKET){
		errror =true;
		ExitThread(0);
	}

	printf("Player #2 connected from %s\n", inet_ntoa(clientinfo.sin_addr));

	if((clientsocket[2]=accept(soocket[2], (LPSOCKADDR)&clientinfo, &sizee))==INVALID_SOCKET){
		errror =true;
		ExitThread(0);
	}

	printf("Player #3 connected from %s\n", inet_ntoa(clientinfo.sin_addr));

	if((clientsocket[3]=accept(soocket[2], (LPSOCKADDR)&clientinfo, &sizee))==INVALID_SOCKET){
		errror =true;
		ExitThread(0);
	}

	printf("Player #4 connected from %s\n", inet_ntoa(clientinfo.sin_addr));

	buffer[0] = 0;
	send(clientsocket[0], buffer, 4, 0);
	buffer[0] = 1;
	send(clientsocket[1], buffer, 4, 0);
	buffer[0] = 2;
	send(clientsocket[2], buffer, 4, 0);
	buffer[0] = 3;
	send(clientsocket[3], buffer, 4, 0);

	printf("OK.\n");

	while(true){
		if(terminate){
			ExitThread(0);
		}
		numbytes = 0;
		while(numbytes<8){
			numbytes += recv(clientsocket[0], buffer+numbytes, 8, 0);
		}
		if(buffer[0]=='S'){
			send(clientsocket[1], buffer, 16, 0);
			send(clientsocket[2], buffer, 16, 0);
			send(clientsocket[3], buffer, 16, 0);
			closesocket(clientsocket[0]);
			closesocket(clientsocket[1]);
			closesocket(clientsocket[2]);
			closesocket(clientsocket[3]);
			SetEvent(eveent);
			ExitThread(0);
		}

		data[0] = *(DWORD*)buffer;
		linktime = *((int*)buffer+1);
		*((DWORD*)buffer+2) = data[2];
		*((DWORD*)buffer+3) = data[3];
		send(clientsocket[1], buffer, 16, 0);
		
		*((DWORD*)buffer+2) = data[1];
		send(clientsocket[2], buffer, 16, 0);

		*((DWORD*)buffer+2) = data[2];
		*((DWORD*)buffer+3) = data[1];
		send(clientsocket[3], buffer, 16, 0);

		numbytes = 0;
		while(numbytes<4){
			numbytes += recv(clientsocket[1], buffer+numbytes, 4, 0);
		}
		if(buffer[0]=='S'){
			send(clientsocket[0], buffer, 12, 0);
			send(clientsocket[2], buffer, 16, 0);
			send(clientsocket[3], buffer, 16, 0);
			closesocket(clientsocket[0]);
			closesocket(clientsocket[1]);
			closesocket(clientsocket[2]);
			closesocket(clientsocket[3]);
			SetEvent(eveent);
			ExitThread(0);
		}

		data[1] = *(DWORD*)buffer;
		
		numbytes = 4;
		while(numbytes<8){
			numbytes += recv(clientsocket[2], buffer+numbytes, 4, 0);
		}
		if(buffer[0]=='S'){
			send(clientsocket[0], buffer, 12, 0);
			send(clientsocket[1], buffer, 16, 0);
			send(clientsocket[3], buffer, 16, 0);
			closesocket(clientsocket[0]);
			closesocket(clientsocket[1]);
			closesocket(clientsocket[2]);
			closesocket(clientsocket[3]);
			SetEvent(eveent);
			ExitThread(0);
		}
		
		data[2] = *((DWORD*)buffer+1);

		numbytes = 8;
		while(numbytes<12){
			numbytes += recv(clientsocket[3], buffer+numbytes, 4, 0);
		}
		if(buffer[0]=='S'){
			send(clientsocket[0], buffer, 12, 0);
			send(clientsocket[1], buffer, 16, 0);
			send(clientsocket[2], buffer, 16, 0);
			closesocket(clientsocket[0]);
			closesocket(clientsocket[1]);
			closesocket(clientsocket[2]);
			closesocket(clientsocket[3]);
			SetEvent(eveent);
			ExitThread(0);
		}

		data[3] = *((DWORD*)buffer+2);
		send(clientsocket[0], buffer, 12, 0);

	}
	return 0;
}

