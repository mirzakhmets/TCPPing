/****************************************************/
/* 03/03 (c) Alumni, CPing class					*/
/* TCP Ping echo on user-defined protocols			*/
/****************************************************/
/* TCP-U-Protocols: smtp/25, pop3/110, ftp/21		*/
/* and others which have response to NOOP command	*/
/****************************************************/
#define _WINSOCKAPI_

#include <winsock2.h>

#include <windows.h>

#include "CPing.h"

#include <string.h>

#include <stdio.h>

#include <stdlib.h>

CPing::CPing() {
	iTimeOut = 1000;
	iBytesToRecv = 0;
	iLastError = WSAStartup(MAKEWORD(2, 0), & wsadata);
	strcpy(szNoop, NOOP);
}
CPing::~CPing() {
	WSACleanup();
}
unsigned int CPing::PingContinuous(char * szHost, unsigned int iPort, unsigned int iPackets) {
	struct hostent * host = NULL;
	struct sockaddr_in saddr;
	unsigned int s = 0;
	unsigned int dw1, dw2, dw3;
	char szBuffer[256];
	if(!iBytesToRecv) iBytesToRecv = strlen(szNoop);
	if(iPackets > MAX_SENDS) return (0);
	free(Res);
	Res = (pingstore * ) malloc(sizeof(pingstore) * iPackets);
	memset(Res, 0, sizeof(pingstore) * iPackets);
	s = socket(AF_INET, SOCK_STREAM, 0);
	if(!s) return (0);
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char * ) & iTimeOut, sizeof(iTimeOut));
	setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char * ) & iTimeOut, sizeof(iTimeOut));
	host = gethostbyname(szHost);
	if(host == NULL) return (0);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(iPort);
	saddr.sin_addr = * ((struct in_addr * ) host->h_addr);
	if(connect(s, (struct sockaddr * ) & saddr, sizeof(saddr)) == -1) return (0);
	for(int i = 0; i < iPackets; i++) {
		iTotalRes++;
		sprintf(szBuffer, "%s\r\n", szNoop);
		dw1 = GetTickCount();
		int iSent = send(s, szBuffer, strlen(szBuffer), 0);
		dw2 = GetTickCount();
		int iRecv = recv(s, szBuffer, iBytesToRecv, 0);
		dw3 = GetTickCount();
		Res[i].iPort = iPort;
		Res[i].iTimeSend = dw2 - dw1;
		Res[i].iTimeRecv = dw3 - dw2;
		Res[i].iTotalSent = ((iSent == SOCKET_ERROR) ? 0 : iSent);
		Res[i].iTotalRecvd = ((iRecv == SOCKET_ERROR) ? 0 : iRecv);
		if(iRecv <= 0) {
			closesocket(s);
			return ((iTotalRes) ? 1 : 0);
		}
	}
	closesocket(s);
	return (1);
}
unsigned int CPing::PingConnective(char * szHost, unsigned int iPort, unsigned int iPackets) {
	struct hostent * host = NULL;
	struct sockaddr_in saddr;
	unsigned int s = 0;
	unsigned int dw1, dw2, dw3;
	char szBuffer[256];
	if(iPackets > MAX_SENDS) return (0);
	free(Res);
	Res = (pingstore * ) malloc(sizeof(pingstore) * iPackets);
	memset(Res, 0, sizeof(pingstore) * iPackets);
	if(!iBytesToRecv) iBytesToRecv = strlen(szNoop);
	host = gethostbyname(szHost);
	if(host == NULL) return (0);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(iPort);
	saddr.sin_addr = * ((struct in_addr * ) host->h_addr);
	for(int i = 0; i < iPackets; i++) {
		s = socket(AF_INET, SOCK_STREAM, 0);
		if(!s) return ((iTotalRes) ? 1 : 0);
		setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char * ) & iTimeOut, sizeof(iTimeOut));
		setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char * ) & iTimeOut, sizeof(iTimeOut));
		if(connect(s, (struct sockaddr * ) & saddr, sizeof(saddr)) == -1) return ((iTotalRes) ? 1 : 0);
		iTotalRes++;
		sprintf(szBuffer, "%s\r\n", szNoop);
		dw1 = GetTickCount();
		int iSent = send(s, szBuffer, strlen(szBuffer), 0);
		dw2 = GetTickCount();
		int iRecv = recv(s, szBuffer, iBytesToRecv, 0);
		dw3 = GetTickCount();
		Res[i].iPort = iPort;
		Res[i].iTimeSend = dw2 - dw1;
		Res[i].iTimeRecv = dw3 - dw2;
		Res[i].iTotalSent = ((iSent == SOCKET_ERROR) ? 0 : iSent);
		Res[i].iTotalRecvd = ((iRecv == SOCKET_ERROR) ? 0 : iRecv);
		closesocket(s);
	}
	return (1);
}