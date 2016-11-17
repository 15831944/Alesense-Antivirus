#include "UI_Protect.h"
#include "UI_Scan.h"
#include "LuaOperation.h"
static int ScanedCounts = 0;
static int FoundCounts = 0;
queue<PDRIVEREVENTMSG> PopWindowQueue;
HANDLE PopWindowEvent;
void OnProtectMessage(PVOID Message)
{
	PDRIVEREVENTMSG Event = (PDRIVEREVENTMSG)Message;
	PVIRUS_DESCRIPTION VD = (PVIRUS_DESCRIPTION)Event->SubContent;
	LPWSTR FilePath = Event->Content;
	if (Event->Sign != NULL)
	{
		if (Event->NeedScan) //����Ҫ�ص�
			FoundCounts++;
		InformUI_Int(TypeOfInformUI::FoundCount, FoundCounts);
		ScanedCounts++;
		InformUI_Int(TypeOfInformUI::CreatedCount, ScanedCounts);
		InformUI_String(TypeOfInformUI::CreatePath, Event->Content);
		if (Event->Sign == 1)
			return;
	}
	PDRIVEREVENTMSG QueueMsg = (PDRIVEREVENTMSG)malloc(sizeof(DRIVEREVENTMSG));
	RtlCopyMemory(QueueMsg, Message, sizeof(DRIVEREVENTMSG));
	PopWindowQueue.push(QueueMsg);
}
void CreateProtectInterface()
{
	AsCreateNamedPipe(L"AsUIProtect", OnProtectMessage);
	PopWindowEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ProtectEventFilter, NULL, NULL, NULL);
}
void GetFileName(LPCWSTR Path, LPWSTR Name)
{
	WCHAR Buffer[MAX_PATH] = { 0 };
	_wsplitpath(Path, NULL, NULL, Name, Buffer);
	lstrcat(Name, Buffer);
	if (lstrlen(Name) >= 32)
	{
		Name[28] = '.';
		Name[29] = '.';
		Name[30] = '.';
		Name[31] = '\0';
	}
}
void GetShortPath(LPCWSTR Path, LPWSTR ShortPath)
{
	lstrcpy(ShortPath, Path);
	if (lstrlen(ShortPath) >= 44)
	{
		ShortPath[40] = '.';
		ShortPath[41] = '.';
		ShortPath[42] = '.';
		ShortPath[43] = '\0';
	}
}
void HandlePopWindow(PDRIVEREVENTMSG Msg, BOOL Result)
{

	SetEvent(PopWindowEvent);
	if (Msg == NULL)
		return;
	Msg->Result = Result;
	SendToCoreProtect(Msg);
	free(Msg);
}
void ProtectEventFilter()
{

	WCHAR ShortPath[2][32];
	WCHAR FText[4][64];
	PDRIVEREVENTMSG Event;
	while (1)
	{
		while (PopWindowQueue.empty())
			Sleep(50);
		Event = PopWindowQueue.front();
		PopWindowQueue.pop();
		if (Event->NeedScan)
		{
			InformUI_PopWindow(PopWarningWindow, 0, L"ȷ��", NULL, TRUE, TRUE, L"��⵽��в����", Event->Content, NULL, L"��⵽��в�����Ѿ���ֹ������");
		}
		else
			switch (Event->MessageType)
		{
			case DvMsg_CreateAutoRun:
				GetFileName(Event->TransferPath, ShortPath[0]);
				wsprintf(FText[1], L"���ڽ���:%s", ShortPath[0]);
				wsprintf(FText[2], L"������:%s", Event->Content);
				InformUI_PopWindow(PopWarningWindow, (DWORD)Event, L"��ֹ", L"����", TRUE, TRUE, L"��⵽���̴�����������", FText[1], FText[2], L"�޸���������ľ�����ֶ�,�����û��ָ���˴β���,�����ֹ.");
				break;
			case DvMsg_CreateService:
				GetFileName(Event->TransferPath, ShortPath[0]);
				wsprintf(FText[1], L"���ڽ���:%s", ShortPath[0]);
				wsprintf(FText[2], L"����:%s", Event->Content);
				InformUI_PopWindow(PopWarningWindow, (DWORD)Event, L"��ֹ", L"����", TRUE, TRUE, L"��⵽���̴���ϵͳ����", FText[1], FText[2], L"����ϵͳ������ľ�����ֶ�,�����û��ָ���˴β���,�����ֹ.");
				break;
			case DvMsg_ExeIFEO:   //ע���-ӳ��ٳ�
				GetFileName(Event->TransferPath, ShortPath[0]);
				wsprintf(FText[1], L"���ɽ���:%s", ShortPath[0]);
				wsprintf(FText[2], L"�ٳ�Ŀ��:%s", Event->Content);

				InformUI_PopWindow(PopWarningWindow, (DWORD)Event, L"��ֹ", NULL, TRUE, TRUE, L"���ص�ӳ��ٳ���Ϊ", FText[1], FText[2], L"ӳ��ٳ�Ϊľ������ֶ�,�����������ʹ��,����ֹ�˲���");
				break;
			case DvMsg_LoadDriver:
				GetFileName(Event->TransferPath, ShortPath[0]);
				wsprintf(FText[1], L"���ڽ���:%s", ShortPath[0]);
				wsprintf(FText[2], L"��������:%s", Event->Content);
				InformUI_PopWindow(PopWarningWindow, (DWORD)Event, L"��ֹ", L"����", TRUE, TRUE, L"��⵽���̼�����������", FText[1], FText[2], L"����������ʹľ���ȡ���Ȩ��,�����δ��ʹ�õײ����,�����ֹ");
				break;
			default:

				break;
		}
		WaitForSingleObject(PopWindowEvent, INFINITE);
		ResetEvent(PopWindowEvent);
	}
}
