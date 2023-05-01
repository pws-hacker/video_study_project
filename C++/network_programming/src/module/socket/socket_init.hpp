#ifndef _SOCKET_INIT_H_
#define _SOCKET_INIT_H_


#include <iostream>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")

using namespace std;

class CSocketInit
{
public:

	CSocketInit()
	{
		WSADATA wsaData;
		int iResult = 0;
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR)
		{
			cout << "¶¯Ì¬Á´½Ó¿â¼ÓÔØÊ§°Ü!" << endl;
		}
	};

	~CSocketInit()
	{
		WSACleanup();
	}




};


#endif // !_SOCKET_INIT_H_
