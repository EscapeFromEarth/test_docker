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

// 因为不清楚容器 ip 长什么样，所以还是运行时指定
int main(int argc, char *argv[]) { // argv[0] 是可执行程序名称
	if (argc < 3) {
		Log("ERR: Instruction format error. example: ./clent server_ip server_prot");
		return 1;
	}
	int iSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (iSocketFd < 0) {
		Log("ERR: create socket error.");
		return 2;
	}
	Log("MSG: create socket succ. socket fd %d", iSocketFd);
	const std::string sIp = argv[1];
	const int iPort = atoi(argv[2]);
	struct sockaddr_in oSockaddr;
	memset(&oSockaddr, 0, sizeof(oSockaddr));
	oSockaddr.sin_family = AF_INET;
	oSockaddr.sin_addr.s_addr = inet_addr(sIp.c_str());
	oSockaddr.sin_port = htons(iPort);
	int iRet = connect(iSocketFd, (sockaddr*)&oSockaddr, sizeof(oSockaddr));
	if (iRet) {
		Log("ERR: socket connect error. ip %s, port %d", sIp.c_str(), iPort);
		return 3;
	}
	Log("MSG: socket connect succ. ip %s, port %d", sIp.c_str(), iPort);
	std::string sData = "hello";
	iRet = send(iSocketFd, sData.c_str(), sData.size(), 0);
	if (iRet < 0) {
		Log("ERR: send msg error. msg %s", sData.c_str());
		return 4;
	}
	Log("MSG: send msg succ. msg %s", sData.c_str());
	const int iBufferSize = 1 << 10;
	char sBuffer[iBufferSize];
	int iLen = recv(iSocketFd, sBuffer, iBufferSize - 1, 0);
	if (iLen >= 0 && iLen < iBufferSize) {
		sBuffer[iLen] = '\0';
		Log("MSG: recv server msg succ. msg len %d, msg %s", iLen, sBuffer);
	} else {
		Log("ERR: recv client msg error. socket fd %d", iSocketFd);
		return 5;
	}
	close(iSocketFd);
	
	return 0;
}