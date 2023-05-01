#include "thread_base.h"

#include <iostream>
#include <Windows.h>
using namespace std;
unsigned int g_data = 0;

// �̼߳�ͬ��
//		������߳������Ƶ���ͬһ��ʱ���ڷ���ͬһ��Դʱ,�ᷢ������
// ͬ������
//    1. ԭ�ӷ��ʣ���������  (�����ԣ���������)
//	  2. �ٽ�����һ�������Ĵ�������				 (ֻ������ͬ���������̵��߳�)
//			�����ٽ������ٽ�����������ü�����1��ͬһ���߳̿��Զ�ε���
//			���� n �Σ����� �뿪 n �κ������̲߳ſ��� ����
//    3. �ں˶���
//			�ٽ����ǳ��ʺ�����ͬһ�������ڲ������л��ķ�ʽ���ʹ��������
//			��һ���߳��������߳�ִ�е�ĳЩ����ȡ��ͬ��ʱ����Ҫʹ���ں˶�����ͬ���߳�
//			�����ں˶��󣺻���������ź��������¼����������ļ�������̨���룬�ļ��仯֪ͨ���ɵȴ��ļ�ʱ��
//
//			ÿһ���ں˶������κ�ʱ�򶼴�������״̬֮һ���ź�̬�����ź�̬
//
//			�߳��ڵȴ����е�һ�������ں˶���ʱ������ȴ����ں˶��������ź�״̬���߳�������
//				ϵͳ����ֱ���ȴ����ں˶����Ϊ���ź�״̬ʱ���̲߳Żָ�����
//			�̵߳ȴ�����  WaitForSingleObject WaitForMultipleObjects

//			�������-�������ٽ���������ͬ��������̼�����ݷ���
//				CreateMutex 
//			�¼� 



// �����ٽ�������
CRITICAL_SECTION g_section;
// �����������ں˶���
HANDLE g_hMutex;
// �����¼��ں˶���
HANDLE g_hEvent;

// ʹ��ԭ�ӻ���
void execAtomLock()
{
	InterlockedIncrement(&g_data); // ��ԭ�ӷ�ʽ���� �Զ�+1
}

// ʹ���ٽ���
void execCriticalSection()
{
	// �����ٽ���
	EnterCriticalSection(&g_section);
	g_data++; // �ٽ���
	// �뿪�ٽ���
	LeaveCriticalSection(&g_section);
}

// ʹ�û������ں˶���
void execCoreObject()
{
	WaitForSingleObject(g_hMutex, INFINITE);
	g_data++;
	ReleaseMutex(g_hMutex);
}

// ʹ���¼��ں˶���
void execEventObject()
{
	WaitForSingleObject(g_hEvent, INFINITE);
	g_data++;
	SetEvent(g_hEvent);
}

DWORD WINAPI ThreadProc(LPVOID lp)
{
	for (int i = 0; i < 100000; i++)
	{
		//g_data++; // �ٽ���
		//execAtomLock();
		//execCriticalSection();
		//execCoreObject();
		execEventObject();
	};
	return 0;

}

void CThreadBaseKnowledge::execThreadSynchronous()
{
	// ��ʼ���ٽ�������
	InitializeCriticalSection(&g_section);

	// �����������ں˶���
	g_hMutex = CreateMutex(NULL, FALSE, NULL);
	// �����¼��ں˶���
	g_hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	HANDLE thread_obj = CreateThread(NULL, 0, ThreadProc, 0, 0, 0);
	if (!thread_obj) return;
	for (int i = 0; i < 100000; i++)
	{
		//g_data++;
		//execAtomLock();
		//execCriticalSection();
		//execCoreObject();
		execEventObject();

	};
	//  INFINITE һֱ�ȴ�  ����ǵȴ� thread_obj ����̶߳������ꡣ ��������ǽ���������⡣
	WaitForSingleObject(thread_obj, INFINITE);//  ���д��������ע�͵Ļ�����ӡ�����һ���� ϣ��ֵ
	cout << "�������: g_data = " << g_data << endl;
	CloseHandle(thread_obj);

	// �ͷ��ٽ�������
	DeleteCriticalSection(&g_section);

	CloseHandle(g_hMutex);
	CloseHandle(g_hEvent);
}


void execCreateProcess()
{
	TCHAR commandLine[] = TEXT("C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe www.baidu.com");
	//STARTUPINFOW startInfo = { sizeof(STARTUPINFOW) };
	STARTUPINFO startInfo;  // ����ʼ������
	//ZeroMemory(&startInfo, sizeof(STARTUPINFOW));  // windows api init
	memset(&startInfo, 0, sizeof(STARTUPINFOW));	 // c init

	PROCESS_INFORMATION processInfo;

	//memset(&startInfo, 0, sizeof(LPSTARTUPINFOW));
	BOOL ret = CreateProcess(
		NULL,  // ��ָ����ִ���ļ����ļ���
		commandLine, // �����в���
		NULL,  // Ĭ�Ͻ��̰�ȫ��
		NULL,  // Ĭ���̰߳�ȫ��
		FALSE, // ָ����ǰ���̵ľ���Ƿ��ӽ��̼̳�
		0,	   // ָ�����ӵ���������������ͽ��̵Ĵ����ı�־
		NULL,  // ʹ�õ�ǰ���̵Ļ�������
		NULL,  // ʹ�õ�ǰ���̵�������Ŀ¼
		&startInfo, // ָ�����ھ����½��̵������������ʾ�Ľṹ��
		&processInfo // ������Ϣ�ṹ��
	);
	if (!ret)
	{
		cout << "���̴���ʧ��!" << endl;
		return;
	}
	else
	{
		WaitForSingleObject(processInfo.hProcess, INFINITE);
		cout << "�´��� ���� ID = " << processInfo.dwProcessId << ",�߳� ID = " << processInfo.dwThreadId << endl;
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
}


// ���̼�ͨ��
//		1. �ܵ�
//		2. ��Ϣ����
//		3. �����ڴ�
//		4. �����ļ�ӳ��



void CThreadBaseKnowledge::execProcessSynchronous()
{
	//execCreateProcess();

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
	cout << "buffer size = " << sizeof(buffer) << endl;
	// scanf_s �ո񱻽ض���
	//scanf_s("%s", buffer, 128);
	//cin >> buffer;  // cin �ո�Ҳ���ض���
	cin.get(buffer,128);
	while (true) {};

	UnmapViewOfFile(buffer);
	CloseHandle(hMapFile);
}
