#include "iostream"
#include "unistd.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "cstring"

// 因为不清楚容器 ip 长什么样，所以还是运行时指定
int main(int argc, char *argv[]) { // argv[0] 是可执行程序名称
	if (argc < 3) {
		printf("ERR: Instruction format error. example: ./clent server_ip server_prot\n");
		return 1;
	}
	int iSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (iSocketFd < 0) {
		printf("ERR: create socket error.\n");
		return 2;
	}
	printf("MSG: create socket succ. socket fd %d\n", iSocketFd);
	const std::string sIp = argv[1];
	const int iPort = atoi(argv[2]);
	struct sockaddr_in oSockaddr;
	memset(&oSockaddr, 0, sizeof(oSockaddr));
	oSockaddr.sin_family = AF_INET;
	oSockaddr.sin_addr.s_addr = inet_addr(sIp.c_str());
	oSockaddr.sin_port = htons(iPort);
	int iRet = connect(iSocketFd, (sockaddr*)&oSockaddr, sizeof(oSockaddr));
	if (iRet) {
		printf("ERR: socket connect error. ip %s, port %d\n", sIp.c_str(), iPort);
		return 3;
	}
	printf("MSG: socket connect succ. ip %s, port %d\n", sIp.c_str(), iPort);
	std::string sData = "hello";
	iRet = send(iSocketFd, sData.c_str(), sData.size(), 0);
	if (iRet < 0) {
		printf("ERR: send msg error. msg %s\n", sData.c_str());
		return 4;
	}
	printf("MSG: send msg succ. msg %s\n", sData.c_str());
	const int iBufferSize = 1 << 10;
	char sBuffer[iBufferSize];
	int iLen = recv(iSocketFd, sBuffer, iBufferSize - 1, 0);
	if (iLen >= 0 && iLen < iBufferSize) {
		sBuffer[iLen] = '\0';
		printf("MSG: recv server msg succ. msg len %d, msg %s\n", iLen, sBuffer);
	} else {
		printf("ERR: recv client msg error. socket fd %d\n", iSocketFd);
		return 5;
	}
	close(iSocketFd);
	
	return 0;
}