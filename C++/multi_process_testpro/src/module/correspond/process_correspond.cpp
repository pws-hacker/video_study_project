#include "process_correspond.h"
#include <Windows.h>
#include <iostream>

using namespace std;

// �����ļ�ӳ�� ��ָ��ͬһ�������ڴ� �ڲ�ͬ������ӳ�䵽��ͬ�������ڴ��С�
//     ÿһ�����̶�д�Լ��������ڴ�͵�ͬ�ڶ�д�����ڴ档
//     ���ڲ�ͬ���̵Ĳ�ͬ�����ڴ�ӳ�����ͬһƬ�����ڴ棬���Դﵽ���̼�ͬ����Ŀ��



void CProcessCorrespond::execFileMapping()
{
	HANDLE hMapFile;
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,  // Ȩ�ޣ�����
		FALSE,          // ���̳о��
		TEXT("fileMap")        // ����
	);
	if (hMapFile == NULL)
	{
		hMapFile = CreateFileMapping(
										NULL,  // �����ļ����
										NULL,  // Ĭ��ӳ���ļ��İ�ȫ����
										PAGE_READWRITE, // �ɶ���д
										0,	   // ��λ�ļ���С
										1280,   // ��λ�ļ���С
										TEXT("fileMap") // �����ڴ�����
									);
	}

	if (!hMapFile) return;
	// buf ����ӳ��������ڴ�
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
