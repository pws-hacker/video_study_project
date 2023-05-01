
#include "multi_threading/thread_base.h"
#include <stdlib.h>
#include "socket/socket_init.hpp"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

int main()
{
	//CThreadBaseKnowledge::execThreadSynchronous();
	//CThreadBaseKnowledge::execProcessSynchronous();


	CSocketInit socketInit;
	SOCKET hListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (hListenSocket == INVALID_SOCKET)
	{
		cout << "�����׽��ִ���ʧ��!" << endl;
		return -1;
	}
	int port = 12381;


	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("192.168.1.100");
	addr.sin_port = htons(port);


	int ret = bind(hListenSocket, (const sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		cout << "���׽���ʧ��!" << endl;
		return -1;
	}

	if (listen(hListenSocket, 10) == SOCKET_ERROR)
	{
		cout << "����ʧ��!" << endl;
		closesocket(hListenSocket);
		return -1;
	}

	sockaddr_in clientAddr;
	int iLen = sizeof(sockaddr_in);
	SOCKET hClient = accept(hListenSocket,(sockaddr*)&clientAddr, &iLen);

	if (hClient == INVALID_SOCKET)
	{
		cout << "���տͻ���ʧ��!" << endl;
		closesocket(hListenSocket);
		return -1;
	}

	cout << "��ͻ��˽�������..." << endl;




	getchar();
	system("pause");
	return 0;
}