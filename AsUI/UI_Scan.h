#include <Windows.h>
#include <queue>
using namespace std;
#include "..\Libs\AsPipeLib.h"

#define SendToCoreScan(Data,Size) AsSendToNamedPipe(L"AsCoreScan",Data,Size);
typedef struct
{
	int Type;
	WCHAR URL[MAX_PATH];
}SCAN_UIMESSAGE,*PSCAN_UIMESSAGE;
enum TraversalType
{
	File_Quick,
	File_Custom,
	File_Full
};
enum UIScanMessageType
{
	FileScan_Stop = 0xf,
	FileScan_Suspend,
	FileScan_Resume
};
enum ScanInformationCommand
{
	Finished,ManualStop,Error
};
typedef	struct
{
	unsigned int FileTotal;
	unsigned int FindTotal;
	unsigned int TimeTotal;
	unsigned int Progress;
	WCHAR CurrentURL[MAX_PATH];
	bool IsCommand;
	unsigned int Command; //0 == Sign = // 1 == command
}SCAN_STATUSINFORMATION,*PSCAN_STATUSINFORMATION;
typedef	struct _VIRUS_DESCRIPTION_       //��������
{
	wchar_t VirusName[32];				 //��������
	wchar_t EngineName[16];
	DWORD32 SuspiciousValue;				 //���ɶ�
	DWORD32 HowToDeal;					 //������
}VIRUS_DESCRIPTION, *PVIRUS_DESCRIPTION;
typedef	struct
{
	unsigned int Sign;		//��������SCAN_INFORMATION ��Ϊ-1
	unsigned int Reserved;	//Reserved
	unsigned int Reserved2;
	WCHAR URL[MAX_PATH];	//����·��
	VIRUS_DESCRIPTION VD;
}SCAN_VIRINFORMATION, *PSCAN_VIRINFORMATION;
void FileScanCallBack(PVOID Data);
void CreateScanInterface();
void CreateForFileScan(UINT Type,LPWSTR URL);
void SendScanCommand(UINT Id);
bool SelectScanDir(wchar_t* Dir);
void HandleScanningResult();