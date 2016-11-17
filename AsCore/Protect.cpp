#include "Protect.h"
#include "Control.h"
CProtect::CProtect()
{
	PipeHandle = AsCreateNamedPipe(L"AsCoreProtect",UICallBack);
}
CProtect::~CProtect(void)
{

}
bool CProtect::StartProtect()
{
	DWORD Length;
	DWORD32 Pid = GetCurrentProcessId();
	FilterDriverHandle = CreateFile(FILTER_DEVICE_NAME,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(FilterDriverHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(0,L"������������ʧ�� ������Ŀ¼�µ�InstallDriver.exe����װ��������",L"",MB_ICONERROR);
		//ExitProcess(0);
		return FALSE;
	}
	DeviceIoControl(FilterDriverHandle,CTL_STARTPROTECTION,0,0,NULL,0,&Length,0);
	DeviceIoControl(FilterDriverHandle,CTL_INSERTPROTECTPID,&Pid,sizeof(DWORD32),NULL,0,&Length,0);
	EventThreadSign = TRUE;
	BeginThread(EventFilter,this);
	return true;
}
bool CProtect::PauseProtect()
{
	DWORD Length;
	DWORD32 Pid = GetCurrentProcessId();
	DeviceIoControl(FilterDriverHandle,CTL_REMOVEPROTECTPID,&Pid,sizeof(DWORD32),NULL,0,&Length,0);
	DeviceIoControl(FilterDriverHandle,CTL_STOPPROTECTION,0,0,NULL,0,&Length,0);
	CloseHandle(FilterDriverHandle);
	return true;
}
void CProtect::EventFilter(CProtect* This)
{
	PDRIVEREVENTMSG Msg = {0};
	DWORD Length = 0;
	BOOL Result;
	while(TRUE)
	{
		Msg = (PDRIVEREVENTMSG)malloc(sizeof(DRIVEREVENTMSG));
Wait:
		Result = DeviceIoControl(FilterDriverHandle, CTL_WAITFOREVENT, 0, 0, Msg, sizeof(DRIVEREVENTMSG) - MAX_CONTENT*sizeof(WCHAR), &Length, 0);
		if(Result)
		{
		Msg->SelfPtr = (int)Msg;
		GetPathByPID((HANDLE)Msg->SubValue, Msg->TransferPath);
		if (Msg->MessageType != DvMsg_Shutdown)
			IdentifyMod->GetRealPath(Msg->Content, Msg->Content);
		if(Msg->NeedScan)
		{
			Msg->NeedScan = 0;
			;//�˴�ɨ��
			IdentifyMod->CallBackIdentify(Msg->Content, ScanCallBack, (int)Msg, (int)This);
		}
		else
		{
			//�˴�UI
			SendToUIProtect(Msg,sizeof(DRIVEREVENTMSG));
		}
		}
		else
		{
			Sleep(10);
			goto Wait;
		}
	}
}

void ScanCallBack(LPWSTR FilePath,PVIRUS_DESCRIPTION VD,VirusType Danger,DWORD32 SelfPtr,DWORD32 Reserved2)
 {
	 CProtect::PDRIVEREVENTMSG Msg = (CProtect::PDRIVEREVENTMSG)SelfPtr;
	 CProtect* This = (CProtect*)Reserved2;
	 DWORD Length;
	
	 if(Danger)
	 {
		 Msg->Sign = 2;
		 Msg->NeedScan = 1; //�������Ǹ�vir
		 memcpy(Msg->SubContent,VD,sizeof(VIRUS_DESCRIPTION));
		 Msg->Result = CProtect::Res_Forbid;
		 DeviceIoControl(FilterDriverHandle,CTL_EVENTCALLBACK,Msg,sizeof(CProtect::DRIVERCALLBACKMSG),0,0,&Length,0);
		 SendToUIProtect(Msg,MAX_CONTENT*6+8*4);
		 free(Msg);
	 }
	 else
	 {
		  if(Msg->MessageType == CProtect::DvMsg_File)
		  {
			 Msg->Sign = 1;
			 SendToUIProtect(Msg, MAX_CONTENT * 4 + 8 * 4);
			 Msg->Result = CProtect::Res_Pass;
			 DeviceIoControl(FilterDriverHandle,CTL_EVENTCALLBACK,Msg,sizeof(CProtect::DRIVERCALLBACKMSG),0,0,&Length,0);
			 free(Msg);
			 
		  }
		  else
		  {
			  Msg->Sign = 2;//��Ҫ��ʾҲ��Ҫ����
			  SendToUIProtect(Msg, (MAX_CONTENT * 6) + (8 * 4));
		  }
		  //UI
	 }
 }
void UICallBack(PVOID Message)
{
	CProtect::PDRIVERCALLBACKMSG CallBack;
	DWORD Length;
	CallBack = (CProtect::PDRIVERCALLBACKMSG)Message;
	DeviceIoControl(FilterDriverHandle,CTL_EVENTCALLBACK,CallBack,sizeof(CProtect::DRIVERCALLBACKMSG),NULL,0,&Length,0);
}
