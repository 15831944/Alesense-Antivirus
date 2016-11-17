#include <Windows.h>
#include "..\Libs\AsPipeLib.h"

#define SendToCoreProtect(Data) AsSendToNamedPipe(L"AsCoreProtect",(PVOID)Data,sizeof(DRIVERCALLBACKMSG))
#define MAX_CONTENT 512
enum DriverMsgType
{
	DvMsg_File,				//�ļ�&����
	DvMsg_LoadDriver,		//����
	DvMsg_CreateAutoRun,	//ע���-������
	DvMsg_CreateService,	//ע���-������
	DvMsg_ExeIFEO,   //ע���-ӳ��ٳ�
 DvMsg_Shutdown   //�ػ�			//ע���-ӳ��ٳ�
};
enum MsgResult
{
	Res_Forbid,
	Res_Pass
};
typedef struct
{
	int DrvPtr;
	int Result;
	int SelfPtr;
	int MessageType;
	int Sign;
	int Value;
	int SubValue;
	int NeedScan;
	wchar_t Content[MAX_CONTENT];
	wchar_t SubContent[MAX_CONTENT];
	wchar_t TransferPath[MAX_CONTENT];
}DRIVEREVENTMSG,*PDRIVEREVENTMSG;
typedef struct
{
	int DrvPtr;
	int Result;
}DRIVERCALLBACKMSG,*PDRIVERCALLBACKMSG;
void OnProtectMessage(PVOID Message);
void CreateProtectInterface();
void ProtectEventFilter();
void HandlePopWindow(PDRIVEREVENTMSG Msg, BOOL Result);