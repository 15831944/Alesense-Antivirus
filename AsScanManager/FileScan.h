#include <Windows.h>
#include "Manager.h"
/*========================================*/
extern INT32 IndentifyTaskCount;
extern SCAN_INFORMATION IndentifyTemp; //���ټ���Temp
extern PSCAN_QUEUE_INFORMATION IndentifyQueue; //���ټ�������
extern PSCAN_QUEUE_INFORMATION IndentifyQueueTop; //���ټ������еĶ���
/*========================================*/
void FileTraversal(PSCAN_INFORMATION Info);
DWORD WINAPI ScanThreadPool(PVOID arg);
VirusType IndentifyFile(wchar_t* FileName,PVIRUS_DESCRIPTION VirusDescription,PSCAN_INFORMATION Info);
