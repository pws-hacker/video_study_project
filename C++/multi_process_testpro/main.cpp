#include "correspond/process_correspond.h"
#include <stdlib.h>
#include "../network_programming/src/module/socket/socket_init.hpp"


int main()
{
	//CProcessCorrespond::execFileMapping();



	CSocketInit socketInit;
	SOCKET hClient = socket(AF_INET, SOCK_STREAM, 0);
	if (hClient == INVALID_SOCKET)
	{
		cout << "¼àÌýÌ×½Ó×Ö´´½¨Ê§°Ü!" << endl;
		return -1;
	}
	int port = 12381;


	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("192.168.1.100");
	addr.sin_port = htons(port);


	int ret = connect(hClient, (sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		cout << "Á¬½Ó·þÎñÆ÷Ê§°Ü!" << endl;
		return -1;
	}


	//int ret = bind(hListenSocket, (const sockaddr*)&addr, sizeof(addr));
	//if (ret == SOCKET_ERROR)
	//{
	//	cout << "°ó¶¨Ì×½Ó×ÖÊ§°Ü!" << endl;
	//	return -1;
	//}
	getchar();

	system("pause");
	return 0;
}