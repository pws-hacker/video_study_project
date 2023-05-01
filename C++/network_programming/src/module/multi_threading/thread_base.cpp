#include "thread_base.h"

#include <iostream>
#include <Windows.h>
using namespace std;
unsigned int g_data = 0;

// 线程间同步
//		当多个线程无限制地在同一段时间内访问同一资源时,会发生错误
// 同步方法
//    1. 原子访问，互锁函数  (局限性：单个变量)
//	  2. 临界区是一段连续的代码区域				 (只能用于同步单个进程的线程)
//			进入临界区，临界区对象的引用计数加1，同一个线程可以多次调用
//			进入 n 次，必须 离开 n 次后，其他线程才可以 进入
//    3. 内核对象
//			临界区非常适合于在同一个进程内部以序列化的方式访问共享的数据
//			当一个线程与其他线程执行的某些操作取得同步时，需要使用内核对象来同步线程
//			常用内核对象：互斥变量，信号量，和事件，还包括文件，控制台输入，文件变化通知，可等待的计时器
//
//			每一个内核对象在任何时候都处于两种状态之一，信号态和无信号态
//
//			线程在等待其中的一个或多个内核对象时，如果等待的内核对象处于无信号状态，线程自身将被
//				系统挂起，直到等待的内核对象变为有信号状态时，线程才恢复运行
//			线程等待函数  WaitForSingleObject WaitForMultipleObjects

//			互斥变量-类似于临界区，可以同步多个进程间的数据访问
//				CreateMutex 
//			事件 



// 创建临界区对象
CRITICAL_SECTION g_section;
// 创建互斥锁内核对象
HANDLE g_hMutex;
// 创建事件内核对象
HANDLE g_hEvent;

// 使用原子互锁
void execAtomLock()
{
	InterlockedIncrement(&g_data); // 以原子方式互锁 自动+1
}

// 使用临界区
void execCriticalSection()
{
	// 进入临界区
	EnterCriticalSection(&g_section);
	g_data++; // 临界区
	// 离开临界区
	LeaveCriticalSection(&g_section);
}

// 使用互斥锁内核对象
void execCoreObject()
{
	WaitForSingleObject(g_hMutex, INFINITE);
	g_data++;
	ReleaseMutex(g_hMutex);
}

// 使用事件内核对象
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
		//g_data++; // 临界区
		//execAtomLock();
		//execCriticalSection();
		//execCoreObject();
		execEventObject();
	};
	return 0;

}

void CThreadBaseKnowledge::execThreadSynchronous()
{
	// 初始化临界区对象
	InitializeCriticalSection(&g_section);

	// 创建互斥锁内核对象
	g_hMutex = CreateMutex(NULL, FALSE, NULL);
	// 创建事件内核对象
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
	//  INFINITE 一直等待  这个是等待 thread_obj 这个线程对象走完。 而上面的是解决竞争问题。
	WaitForSingleObject(thread_obj, INFINITE);//  这行代码如果不注释的话，打印结果不一定是 希望值
	cout << "最后数据: g_data = " << g_data << endl;
	CloseHandle(thread_obj);

	// 释放临界区对象
	DeleteCriticalSection(&g_section);

	CloseHandle(g_hMutex);
	CloseHandle(g_hEvent);
}


void execCreateProcess()
{
	TCHAR commandLine[] = TEXT("C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe www.baidu.com");
	//STARTUPINFOW startInfo = { sizeof(STARTUPINFOW) };
	STARTUPINFO startInfo;  // 不初始化崩溃
	//ZeroMemory(&startInfo, sizeof(STARTUPINFOW));  // windows api init
	memset(&startInfo, 0, sizeof(STARTUPINFOW));	 // c init

	PROCESS_INFORMATION processInfo;

	//memset(&startInfo, 0, sizeof(LPSTARTUPINFOW));
	BOOL ret = CreateProcess(
		NULL,  // 不指定可执行文件的文件名
		commandLine, // 命令行参数
		NULL,  // 默认进程安全性
		NULL,  // 默认线程安全性
		FALSE, // 指定当前进程的句柄是否被子进程继承
		0,	   // 指定附加的用来控制优先类和进程的创建的标志
		NULL,  // 使用当前进程的环境变量
		NULL,  // 使用当前进程的驱动和目录
		&startInfo, // 指向用于决定新进程的主窗体如何显示的结构体
		&processInfo // 进程信息结构体
	);
	if (!ret)
	{
		cout << "进程创建失败!" << endl;
		return;
	}
	else
	{
		WaitForSingleObject(processInfo.hProcess, INFINITE);
		cout << "新创建 进程 ID = " << processInfo.dwProcessId << ",线程 ID = " << processInfo.dwThreadId << endl;
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
}


// 进程间通信
//		1. 管道
//		2. 消息队列
//		3. 共享内存
//		4. 共享文件映射



void CThreadBaseKnowledge::execProcessSynchronous()
{
	//execCreateProcess();

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
	cout << "buffer size = " << sizeof(buffer) << endl;
	// scanf_s 空格被截断了
	//scanf_s("%s", buffer, 128);
	//cin >> buffer;  // cin 空格也被截断了
	cin.get(buffer,128);
	while (true) {};

	UnmapViewOfFile(buffer);
	CloseHandle(hMapFile);
}
