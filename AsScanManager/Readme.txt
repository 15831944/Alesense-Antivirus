===============��������====================
PVOID CreateScanTask(ScanTaskType Type,wchar_t* Path,PF_SCAN_VIRUS_CALLBACK VirusCallBack,PF_SCAN_FINISH_CALLBACK FinishCallBack,PF_SCAN_ERROR_CALLBACK ErrorCallBack,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
Type:ɨ����������(ScanTaskType ö�ټ����ĸ�¼)
Path:ɨ���·��
VirusCallBack :ɨ�赽����ʱ�ص�
FinishCallBack:ɨ�����ʱ�Ļص�
ErrorCallBack :��������ʱ�Ļص�
�����ص����ͼ�����
DWORD32 CallBackPtr32,DWORD32 CallBackValue 2���Զ��崫��Ĳ���
����ֵ:����������ľ��
-----------------------------------------------------
void InitializeScanManager();
��ʼ��ɨ������� (һ��ʼ�������һ��)
-----------------------------------------------------
void TerminateScanManager();
����������������ͷ��ڴ� (������)
-----------------------------------------------------
int GetScanTaskScanedCount(PVOID TaskHandle);
��ȡɨ���������ɨ���ļ�����
TaskHandle:����ľ��
����ֵ:��ɨ���ļ�����
-----------------------------------------------------
int GetScanTaskVirusCount(PVOID TaskHandle);
��ȡɨ���������ɨ���ļ����ֲ�������
TaskHandle:����ľ��
����ֵ:��ɨ���ļ����ֲ�������
-----------------------------------------------------
void SetScanTaskScanStatusToNormal(PVOID TaskHandle);
����ɨ�������״̬������
TaskHandle:����ľ��
-----------------------------------------------------
void SetScanTaskScanStatusToStopped(PVOID TaskHandle);
����ɨ�������״̬��ֹͣ
TaskHandle:����ľ��
-----------------------------------------------------
void SetScanTaskScanStatusToSuspended(PVOID TaskHandle);
����ɨ�������״̬����ͣ
TaskHandle:����ľ��
-----------------------------------------------------
bool IsScanTaskScanStatusStopped(PVOID TaskHandle);
���ɨ�������״̬�Ƿ���ֹͣ
TaskHandle:����ľ��
����ֵ:�����Ƿ�ֹͣ
-----------------------------------------------------
bool IsScanTaskScanStatusSuspended(PVOID TaskHandle);
���ɨ�������״̬�Ƿ�����ͣ
TaskHandle:����ľ��
����ֵ:�����Ƿ���ͣ
-----------------------------------------------------
bool IsScanTaskScanStatusNormal(PVOID TaskHandle);
���ɨ�������״̬�Ƿ�������
TaskHandle:����ľ��
����ֵ:�����Ƿ�����
===============================================================================
---------------------�ص�����--------------------------------
typedef void(*PF_SCAN_VIRUS_CALLBACK)(PVOID TaskHandle,wchar_t* FileName,wchar_t* VirusName,VirusType VType,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
TaskHandle:����ľ��
FileName:�����ļ���
VirusName:��������
VType:��������(VirusType ö�ټ����ĸ�¼)
DWORD32 CallBackPtr32,DWORD32 CallBackValue 2���Զ��崫��Ĳ���
-----------------------------------------------------
typedef void(*PF_SCAN_ERROR_CALLBACK)(PVOID TaskHandle,int ErrorCode);
TaskHandle:����ľ��
ErrorCode:������
-----------------------------------------------------
typedef void(*PF_SCAN_FINISH_CALLBACK)(PVOID TaskHandle ,ScanTaskExitCode ExitCode);
TaskHandle:����ľ��
ExitCode:�˳���(ScanTaskExitCode ö�ټ����ĸ�¼)
/*=================��¼ ö������========================*/
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
enum ScanTaskExitCode
{
	Automatic,							//�����˳�
	Manual								//�ֶ��˳�
};