#include <Windows.h>
#include "ScanManager.h"
/*========================================*/
extern INT32 IdentifyTaskCount;
extern SCAN_INFORMATION IdentifyTemp; //���ټ���Temp
extern PSCAN_QUEUE_INFORMATION IdentifyQueue; //���ټ�������
extern PSCAN_QUEUE_INFORMATION IdentifyQueueTop; //���ټ������еĶ���
/*========================================*/
extern "C" _declspec (dllexport) VirusType IdentifyFile(LPWSTR FileName,PVIRUS_DESCRIPTION VirusDescription,PSCAN_INFORMATION Info);
DWORD WINAPI ScanThreadPool(PVOID arg);
