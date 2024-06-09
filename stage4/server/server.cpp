#include "iostream"
#include "unistd.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "cstring"

int main() {
	int iSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (iSocketFd < 0) {
		printf("ERR: create socket error.\n");
		return 1;
	}
	printf("MSG: create socket succ. socket fd %d\n", iSocketFd);
	const std::string sIp = "0.0.0.0";
	const int iPort = 13352;
	struct sockaddr_in oSockaddr;
	memset(&oSockaddr, 0, sizeof(oSockaddr));
	oSockaddr.sin_family = AF_INET;
	oSockaddr.sin_addr.s_addr = inet_addr(sIp.c_str());
	oSockaddr.sin_port = htons(iPort);
	int iRet = bind(iSocketFd, (sockaddr*)&oSockaddr, sizeof(oSockaddr));
	if (iRet < 0) {
		printf("ERR: socket bind error. ret %d\n", iRet);
		return 2;
	}
	printf("MSG: socket bind succ. ip=%s, port=%d\n", sIp.c_str(), iPort);
	iRet = listen(iSocketFd, 1024);
	if (iRet < 0) {
		printf("ERR: socket listen error.");
		return 3;
	}
	printf("MSG: socket bind succ. linstening ...\n");
	while (true) {
		int iConnectFd = accept(iSocketFd, nullptr, nullptr);
		if (iConnectFd < 0) {
			printf("ERR: socket accept error.\n");
			continue;
		}
		printf("MSG: socket accept succ. connect fd %d\n", iConnectFd);
		const int iBufferSize = 1 << 10;
		char sBuffer[iBufferSize];
		int iLen = recv(iConnectFd, sBuffer, iBufferSize - 1, 0);
		if (iLen >= 0 && iLen < iBufferSize) {
			sBuffer[iLen] = '\0';
			printf("MSG: recv client msg succ. connect fd %d, msg len %d,"
				   " msg %s\n", iConnectFd, iLen, sBuffer);
		} else {
			printf("ERR: recv client msg error. connect fd %d\n", iConnectFd);
			continue;
		}
		for (int i = 0; i < iLen; i++) {
			char &cBit = sBuffer[i];
			if (islower(cBit)) {
				cBit = toupper(cBit);
			}
		}
		int iRet = send(iConnectFd, sBuffer, iLen, 0);
		if (iRet < 0) {
			printf("ERR: send msg error. connect fd %d\n", iConnectFd);
			continue;
		}
		printf("MSG: send msg succ. connect fd %d, msg len %d, msg %s\n",
			   iConnectFd, iLen, sBuffer);
	}
	printf("ERR: server interrupt.\n");
	close(iSocketFd);
	return 7;
}