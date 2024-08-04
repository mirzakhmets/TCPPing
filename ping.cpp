/****************************************************/
/* 03/03 (c) Alumni, CPing class demo				*/
/* TCP Ping echo on user-defined protocols			*/
/****************************************************/
#include <windows.h>

#include <stdio.h>

#include "CPing.h"

#include <stdlib.h>

int PrUsage(char * sPath) {
	printf("Ping\n");
	printf("Usage: %s <host> <port> <packets> [-options]\n", sPath);
	printf("Options:\n");
	printf("\t-cv\tConnective ping (default)\n");
	printf("\t-ct\tContinuous ping\n");
	printf("\t-n\tSpecify NOOP command\n");
	printf("\t-crlf\tAppend CRLF to NOOP\n");
	printf("\t-t<t>\tSpecify timeout\n\n");
	printf("Example: %s localhost 110 10 -ct -t100", sPath);
	return (0);
}
int main(int argc, char ** argv) {
	char sBuffer[256];
	int iRecvd, iSent, iTotalTimeRecvd, iTotalTimeSent;
	int iStats[8];
	if(argc < 4) {
		PrUsage(argv[0]);
		return (1);
	}
	CPing * cping = new CPing();
	int port = atoi(argv[2]), res;
	int cvping = 1, packets = atoi(argv[3]);
	for(int i = 4; i < argc; i++) {
		if(!strncmp(argv[i], "-ct", 3)) cvping = 0;
		if(!strncmp(argv[i], "-n", 2)) {
			printf("Enter valid NOOP command: ");
			scanf("%s", sBuffer);
			strcpy(cping->szNoop, sBuffer);
		}
		if(!strncmp(argv[i], "-crlf", 5)) strcat(cping->szNoop, "\r\n");
		if(!strncmp(argv[i], "-t", 2)) {
			for(int j = 2; j < strlen(argv[i]); j++) sBuffer[j - 2] = argv[i][j];
			sBuffer[strlen(argv[i]) - 2] = 0;
			cping->iTimeOut = atoi(sBuffer);
		}
	}
	if(cvping) res = cping-> PingConnective(argv[1], port, packets);
	else res = cping->PingContinuous(argv[1], port, packets);
	if(res) {
		memset(iStats, 0, sizeof(int) * 8);
		iRecvd = iSent = iTotalTimeSent = iTotalTimeRecvd = 0;
		iStats[0] = iStats[2] = iStats[4] = iStats[6] = 32768;
		if(!cping->iTotalRes) {
			printf("No requests transmitted");
			return (0);
		}
		if(cvping) printf("Connective ping\n");
		else printf("Continuous ping\n");
		printf("\nTotal requests: %d\nPort: %d\n\n", cping->iTotalRes, port);
		for(int i = 0; i < cping->iTotalRes; i++) {
			iStats[0] = ((cping->Res[i].iTimeRecv < iStats[0]) ? cping->Res[i].iTimeRecv : iStats[0]);
			iStats[1] = ((cping->Res[i].iTimeRecv > iStats[1]) ? cping->Res[i].iTimeRecv : iStats[1]);
			iStats[2] = ((cping->Res[i].iTimeSend < iStats[2]) ? cping->Res[i].iTimeSend : iStats[2]);
			iStats[3] = ((cping->Res[i].iTimeSend > iStats[3]) ? cping->Res[i].iTimeSend : iStats[3]);
			iStats[4] = ((cping->Res[i].iTotalRecvd < iStats[4]) ? cping->Res[i].iTotalRecvd : iStats[4]);
			iStats[5] = ((cping->Res[i].iTotalRecvd > iStats[5]) ? cping->Res[i].iTotalRecvd : iStats[5]);
			iStats[6] = ((cping->Res[i].iTotalSent < iStats[6]) ? cping->Res[i].iTotalSent : iStats[6]);
			iStats[7] = ((cping->Res[i].iTotalSent > iStats[7]) ? cping->Res[i].iTotalSent : iStats[7]);
			iSent += cping->Res[i].iTotalSent;
			iRecvd += cping->Res[i].iTotalRecvd;
			iTotalTimeSent += cping->Res[i].iTimeSend;
			iTotalTimeRecvd += cping->Res[i].iTimeRecv;
			printf("C:%u\tS:%u\n", cping->Res[i].iTimeSend, cping->Res[i].iTimeRecv);
		}
		printf("\nTotal time recvd: %5d\tMin: %5d\tMax: %5d\tAvg: %5d\n", iTotalTimeRecvd, iStats[0], iStats[1], iTotalTimeRecvd / cping->iTotalRes);
		printf("Total time sent: %5d\tMin: %5d\tMax: %5d\tAvg: %5d\n", iTotalTimeSent, iStats[2], iStats[3], iTotalTimeSent / cping->iTotalRes);
		printf("Total recvd (bytes): %5d\tMin: %5d\tMax: %5d\tAvg: %5d\n", iRecvd, iStats[4], iStats[5], iRecvd / cping->iTotalRes);
		printf("Total sent (bytes): %5d\tMin: %5d\tMax: %5d\tAvg: %5d\n", iSent, iStats[6], iStats[7], iSent / cping->iTotalRes);
	} else printf("Error occured\n");
	return (0);
}