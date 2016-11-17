#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Psapi.lib")
#include "Core.h"
#pragma comment (lib,"..\\Libs\\AsScanManager.Lib")
#include "..\Libs\AsPipeLib.h"

enum VirusType
{
	Safe,								 //��ȫ
	Virus,								 //�����ļ�
	Suspicious,							 //�����ļ�
	UnknownFile							 //δ֪�ļ�
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

typedef void(*PF_SCAN_CALLBACK)(LPWSTR FileName,PVIRUS_DESCRIPTION VD,VirusType VType,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
extern "C" _declspec (dllimport) void InitializeScanManager();
extern "C" _declspec (dllimport) void TerminateScanManager();
extern "C" _declspec (dllimport) PVOID CreateFileIdentify(LPWSTR Path,PF_SCAN_CALLBACK ScanCallBack,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
extern "C" _declspec (dllexport) VirusType IdentifyFile(LPWSTR FileName,PVIRUS_DESCRIPTION VirusDescription,PVOID Info);
class CIdentify
{
private:
	BOOL GetImageRealPath(wchar_t* Dest, wchar_t* OrigPath);
public:
	CIdentify(void);
	~CIdentify(void);
	void GetRealPath(wchar_t* Dest, wchar_t* OrigReg);
	VirusType DirectlyIdentify(LPWSTR FilePath,PVIRUS_DESCRIPTION VD);
	void CallBackIdentify(LPWSTR FilePath, PF_SCAN_CALLBACK CallBack,int CallBackPtr32,int CallBackValue);
	friend void DirScanCallBack(LPWSTR FilePath,LPWSTR DangerName,VirusType Danger,DWORD32 SelfPtr,DWORD32 Reserved2);
};

