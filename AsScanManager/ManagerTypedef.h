#include <Windows.h>
#pragma once
/*========================================*/
#define StartThread(Routine,Argument) CreateThread(0,0,Routine,Argument,0,0)
#define SCAN_POOL_MAX 20
#define ENGINES_NUM 2
/*==================ö������================*/
enum ScanTaskType
{
	FullScan,							 //ȫ��ɨ��
	QuickScan,							 //����ɨ��
	SelectScan,							 //ָ��λ��ɨ��
	QuickIndentify						 //���ټ���
};
enum VirusType
{
	Safe,								 //��ȫ
	Virus,								 //�����ļ�
	Suspicious,							 //�����ļ�
	UnknownFile							 //δ֪�ļ�
};
enum ScanTaskStatus
{
	Normal,								 //������
	Suspended,							 //��ͣ��
	Stoped,								 //ֹͣ��
	Error = 0x004						 //�����
};
enum ScanTaskExitCode
{
	Automatic,							//�Զ��ر�
	Manual								//�ֶ��ر�
};
enum ScanEngine
{
	HeurDet,							//����ʽ
	FileSDVer						    //����ǩ��
};
/*==================�ص�====================*/
typedef void(*PF_SCAN_VIRUS_CALLBACK)(PVOID TaskHandle,wchar_t* FileName,wchar_t* VirusName,VirusType VType,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
typedef void(*PF_SCAN_ERROR_CALLBACK)(PVOID TaskHandle,int ErrorCode);
typedef void(*PF_SCAN_FINISH_CALLBACK)(PVOID TaskHandle ,ScanTaskExitCode ExitCode);
typedef bool(*SCANENGINE_NORMALCALL)(wchar_t* FileName);
typedef void(*SCANENGINE_INITCALL)();
/*==========================================*/
typedef	struct _SCAN_STATUS_
{
  ScanTaskStatus ScanStatus;		     //ɨ��״̬(in & out)
  int ScanedCount;						 //��ɨ����ļ���
  int VirusCount;						 //������
  int ErrorCode;						 //������
  unsigned int TimeUsed;				 //��ʹ�õ�ʱ��
}SCAN_STATUS,*PSCAN_STATUS;
//========================================//
typedef struct _ENGINE_INFORMATION_
{
	HMODULE hDll;
	SCANENGINE_NORMALCALL FunctionCall;
	SCANENGINE_INITCALL Init;
	SCANENGINE_INITCALL Term;
}ENGINE_INFORMATION,*PENGINE_INFORMATION;
/*========================================*/
typedef	struct _SCANTASK_LINKEDLIST_
{
  int ID;
  ScanTaskType Type;                     //ɨ������
  SCAN_STATUS  Status;					 //ɨ��״̬
  HANDLE ThreadHandle;					 //�߳̾��
  void *NextTask;						 //����ָ��
}SCANTASK_LINKEDLIST,*PSCANTASK_LINKEDLIST;
/*========================================*/
typedef	struct _VIRUS_DESCRIPTION_       //��������
{					 
  wchar_t VirusName[32];				 //��������
  DWORD32 SuspiciousValue;				 //���ɶ�
}VIRUS_DESCRIPTION,*PVIRUS_DESCRIPTION;

/*==========================================*/
typedef	struct _SCAN_INFORMATION_		 //ɨ��ʱ���ݵ���Ϣ
{
  wchar_t Path[MAX_PATH];				 //Ҫɨ���·��
  PSCANTASK_LINKEDLIST TaskHandle;						 //ɨ�������Handle
  DWORD32 CallBackPtr32;				 //�ص��õ���Ϣ
  DWORD32 CallBackValue;				 //�ص��õ���Ϣ
  PF_SCAN_VIRUS_CALLBACK  VirusCallBack;
  PF_SCAN_FINISH_CALLBACK FinishCallBack;
  PF_SCAN_ERROR_CALLBACK ErrorCallBack;
}SCAN_INFORMATION,*PSCAN_INFORMATION;
typedef	struct _SCAN_QUEUE_INFORMATION_		 //������Ϣ
{
  SCAN_INFORMATION Info;
  PVOID Next;
}SCAN_QUEUE_INFORMATION,*PSCAN_QUEUE_INFORMATION;
/*========================================*/
