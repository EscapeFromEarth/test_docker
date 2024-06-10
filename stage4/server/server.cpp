#include "iostream"
#include "unistd.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "cstring"
#include "string"
#include "cstdarg"

void Log(const char *format, ...) {
	va_list args;
	va_start(args, format);
	int iSize = vsnprintf(nullptr, 0, format, args) + 1;
	std::string sBuffer(iSize, '\0');
	va_start(args, format);
	vsnprintf(&sBuffer[0], iSize, format, args);
	va_end(args);
	std::cout << sBuffer << std::endl;
}

int main() {
	int iSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (iSocketFd < 0) {
		Log("ERR: create socket error.");
		return 1;
	}
	Log("MSG: create socket succ. socket fd %d", iSocketFd);
	const std::string sIp = "0.0.0.0";
	const int iPort = 13352;
	struct sockaddr_in oSockaddr;
	memset(&oSockaddr, 0, sizeof(oSockaddr));
	oSockaddr.sin_family = AF_INET;
	oSockaddr.sin_addr.s_addr = inet_addr(sIp.c_str());
	oSockaddr.sin_port = htons(iPort);
	int iRet = bind(iSocketFd, (sockaddr*)&oSockaddr, sizeof(oSockaddr));
	if (iRet < 0) {
		Log("ERR: socket bind error. ret %d", iRet);
		return 2;
	}
	Log("MSG: socket bind succ. ip=%s, port=%d", sIp.c_str(), iPort);
	iRet = listen(iSocketFd, 1024);
	if (iRet < 0) {
		Log("ERR: socket listen error.");
		return 3;
	}
	Log("MSG: socket bind succ. linstening ...");
	while (true) {
		int iConnectFd = accept(iSocketFd, nullptr, nullptr);
		if (iConnectFd < 0) {
			Log("ERR: socket accept error.");
			continue;
		}
		Log("MSG: socket accept succ. connect fd %d", iConnectFd);
		const int iBufferSize = 1 << 10;
		char sBuffer[iBufferSize];
		int iLen = recv(iConnectFd, sBuffer, iBufferSize - 1, 0);
		if (iLen >= 0 && iLen < iBufferSize) {
			sBuffer[iLen] = '\0';
			Log("MSG: recv client msg succ. connect fd %d, msg len %d,"
				   " msg %s", iConnectFd, iLen, sBuffer);
		} else {
			Log("ERR: recv client msg error. connect fd %d", iConnectFd);
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
			Log("ERR: send msg error. connect fd %d", iConnectFd);
			continue;
		}
		Log("MSG: send msg succ. connect fd %d, msg len %d, msg %s",
			   iConnectFd, iLen, sBuffer);
	}
	Log("ERR: server interrupt.");
	close(iSocketFd);
	return 7;
}