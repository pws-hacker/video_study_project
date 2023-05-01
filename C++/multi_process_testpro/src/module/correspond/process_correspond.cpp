#include "process_correspond.h"
#include <Windows.h>
#include <iostream>

using namespace std;

// 共享文件映射 是指将同一个物理内存 在不同进程中映射到不同的虚拟内存中。
//     每一个进程读写自己的虚拟内存就等同于读写物理内存。
//     由于不同进程的不同虚拟内存映射的是同一片物理内存，所以达到进程间同步的目的



void CProcessCorrespond::execFileMapping()
{
	HANDLE hMapFile;
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,  // 权限：所有
		FALSE,          // 不继承句柄
		TEXT("fileMap")        // 名字
	);
	if (hMapFile == NULL)
	{
		hMapFile = CreateFileMapping(
										NULL,  // 物理文件句柄
										NULL,  // 默认映射文件的安全级别
										PAGE_READWRITE, // 可读可写
										0,	   // 高位文件大小
										1280,   // 低位文件大小
										TEXT("fileMap") // 共享内存名称
									);
	}

	if (!hMapFile) return;
	// buf 就是映射的虚拟内存
	char* buffer = (char*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 1280);
	// read 
	if (buffer)
	{
		cout << "read suc ,data === " << buffer << endl;
	}

	while (true) {};

	UnmapViewOfFile(buffer);
	CloseHandle(hMapFile);
}
