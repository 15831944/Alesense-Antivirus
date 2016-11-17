#include <Windows.h>

#pragma once
/*========================================*/
#define StartThread(Routine,Argument) CreateThread(0,0,Routine,Argument,0,0)
#define SCAN_POOL_MAX 20
#define ENGINES_NUM 3
/*==================ö������================*/
enum VirusType
{
	Safe,								 //��ȫ
	Virus,								 //�����ļ�
	Suspicious,							 //�����ļ�
	UnknownFile							 //δ֪�ļ�
};
enum ScanEngine
{
	JavHeur,							//��ʱ����
	SharpArrow,							//SharpArrow����ʽ
	FileVerify						    //����ǩ����֤
};
enum Dealingway
{
	Deal_Upload,		//�ϱ�
	Deal_Delete,		//ɾ��
	Deal_Fix,			//�޸�
	Deal_RegDelete,		//ɾ��[ע���]
	Deal_RegFix			//�޸�[ע���]
};
//=========================================//
typedef	struct _VIRUS_DESCRIPTION_       //��������
{
	wchar_t VirusName[32];				 //��������
	wchar_t EngineName[16];
	DWORD32 SuspiciousValue;				 //���ɶ�
	DWORD32 HowToDeal;					 //������
}VIRUS_DESCRIPTION, *PVIRUS_DESCRIPTION;

/*==========================================*/
/*==================�ص�====================*/
typedef void(*PF_SCAN_CALLBACK)(LPWSTR FileName,PVIRUS_DESCRIPTION VD,VirusType VType,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
typedef bool(*SCANENGINE_NORMALCALL)(LPVOID MainArg,PVIRUS_DESCRIPTION VD);
typedef bool(*SCANENGINE_NOINFORCALL)(LPWSTR FileName);
typedef void(*SCANENGINE_INITCALL)();
//========================================//
typedef struct _ENGINE_INFORMATION_
{
	HMODULE hDll;
	SCANENGINE_NORMALCALL FunctionCall;
	SCANENGINE_NOINFORCALL FunctionCall2;
	SCANENGINE_INITCALL Init;
	SCANENGINE_INITCALL Term;
}ENGINE_INFORMATION,*PENGINE_INFORMATION;
/*========================================*/


typedef	struct _SCAN_INFORMATION_		 //ɨ��ʱ���ݵ���Ϣ
{
  wchar_t Path[MAX_PATH];				 //Ҫɨ���·��
  DWORD32 CallBackPtr32;				 //�ص��õ���Ϣ
  DWORD32 CallBackValue;				 //�ص��õ���Ϣ
  PF_SCAN_CALLBACK  ScanCallBack;
}SCAN_INFORMATION,*PSCAN_INFORMATION;
typedef	struct _SCAN_QUEUE_INFORMATION_		 //������Ϣ
{
  SCAN_INFORMATION Info;
  PVOID Next;
}SCAN_QUEUE_INFORMATION,*PSCAN_QUEUE_INFORMATION;
/*========================================*/
void LoadDllForEngine(const LPWSTR DllPath,const char* FunctionName,PENGINE_INFORMATION EInfo);
void ClearScanTask();
/*==================Export======================*/
//extern "C" _declspec (dllexport) 
extern "C" _declspec (dllexport) PVOID CreateFileIdentify(LPWSTR Path,PF_SCAN_CALLBACK ScanCallBack,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
extern "C" _declspec (dllexport) void InitializeScanManager();
extern "C" _declspec (dllexport) void TerminateScanManager();
/*==================Export======================*/
extern ENGINE_INFORMATION Engines[ENGINES_NUM];
extern HANDLE ThreadPool_Scan[SCAN_POOL_MAX];
extern HANDLE ThreadPool_Scan_MainEvent;
extern HANDLE ThreadPool_Scan_ThreadEvent;
extern HANDLE ThreadPool_Scan_QueueEvent;