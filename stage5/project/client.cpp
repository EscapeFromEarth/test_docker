#include "iostream"
#include "cstdarg"
#include "fstream"
#include "unistd.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "cstring"

void Log(const char *format, ...) { // 这样就能格式化输出日志同时刷新缓冲区了。。
	va_list args;
	va_start(args, format);
	int iSize = vsnprintf(nullptr, 0, format, args) + 1;
	std::string sBuffer(iSize, '\0');
	va_start(args, format);
	vsnprintf(&sBuffer[0], iSize, format, args);
	va_end(args);
	std::cout << sBuffer << std::endl;
}

// 众所周知，只要是给个机会让我写点小东西，我就会给他写到天花乱坠，“偏离主题”。。
class Config {
public:
	static Config* GetSingleTon() {
		static Config oConfig;
		return &oConfig;
	}
	std::string m_sIp;
	int m_iPort;
private:
	std::string Trim(std::string sStr) {
		auto iFirst = sStr.find_first_not_of(' ');
		if (iFirst == std::string::npos) {
			return "";
		}
		auto iLast = sStr.find_last_not_of(' ');
		return sStr.substr(iFirst, iLast - iFirst + 1);
	}
	Config() : m_sIp(""), m_iPort(0) {
		std::ifstream oFile("./client.conf");
		if (!oFile.is_open()) {
			Log("ERR: open config file error.");
			return;
		}
		std::string sOneLine;
		while(std::getline(oFile, sOneLine)) {
			if (!sOneLine.empty() && sOneLine.back() == '\r') {
				sOneLine.pop_back(); // **C语言
			}
			Log("MSG: get one line {%s}", sOneLine.c_str());
			auto iEqualPos = sOneLine.find('=');
			if (iEqualPos == std::string::npos) {
				Log("WARN: there is one line without \'=\'.");
				continue;
			}
			std::string sKey = Trim(std::move(sOneLine.substr(0, iEqualPos))),
						sValue = Trim(std::move(sOneLine.substr(iEqualPos + 1,
								 sOneLine.length() - iEqualPos - 1)));
			if (sKey.empty() || sValue.empty()) {
				Log("ERR: one of key or value is empty.");
				continue;
			}
			Log("MSG: get one conf line succ. key %s, value %s", sKey.c_str(), sValue.c_str());
			if (sKey == "server_ip") {
				m_sIp = sValue;
			} else if (sKey == "server_port") {
				m_iPort = std::stoi(sValue);
			} else {
				Log("ERR: one conf line does not match any member. "
					   "key %s, value %s", sKey.c_str(), sValue.c_str());
			}
		}
		Log("MSG: read conf end. m_sIp %s, m_iPort %d", m_sIp.c_str(), m_iPort);
	}
	~Config() = default;
	Config(const Config&) = delete;
	Config &operator=(const Config&) = delete;
};

int main() {
	// std::cout << "TEST: one cout log" << std::endl; // 这个会输出
	// printf("TEST: one printf log\n"); // 这种不会。。
	int iSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (iSocketFd < 0) {
		Log("ERR: create socket error.");
		return 2;
	}
	Log("MSG: create socket succ. socket fd %d", iSocketFd);
	const std::string sIp = Config::GetSingleTon()->m_sIp;
	const int iPort = Config::GetSingleTon()->m_iPort;
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