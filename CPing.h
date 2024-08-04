#ifndef __CPING_CLASS
#define __CPING_CLASS
#define MAX_SENDS 1111111
#define NOOP "NOOP"
typedef struct {
	unsigned int iTimeSend;
	unsigned int iTimeRecv;
	unsigned int iTotalSent, iTotalRecvd;
	unsigned int iPort;
}
pingstore;
class CPing {
	public: CPing();
	~CPing();
	char szNoop[256];
	int iLastError,
	iTotalRes,
	iBytesToRecv;
	unsigned int iTimeOut,
	iTotalSent,
	iTotalRecvd;
	unsigned int PingContinuous(char * szHost, unsigned int iPort, unsigned int iPackets);
	unsigned int PingConnective(char * szHost, unsigned int iPort, unsigned int iPackets);
	pingstore * Res;
	private: WSADATA wsadata;
};
#endif