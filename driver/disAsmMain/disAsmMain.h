#ifndef DISASMMAIN_H
#define DISASMMAIN_H
#endif
 
#include <ntddk.h>
#include "disasm.h"
 
//��RING3��ͨѶ�ṹ
typedef struct _disAsmData{
    ULONG  Length;          //����೤��
	wchar_t*  szFuncName; //Ҫ�����ĺ�������
}DisAsmData,*pDisAsmData;
 
DRIVER_INITIALIZE DriverEntry;
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath);
 
__drv_dispatchType(IRP_MJ_DEVICE_CONTROL)
DRIVER_DISPATCH DispatchDeviceIoControl;
NTSTATUS DispatchDeviceIoControl(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
 
__drv_dispatchType_other
DRIVER_DISPATCH DispatchCompletion;
NTSTATUS DispatchCompletion(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
 
DRIVER_UNLOAD Unload;
VOID Unload(PDRIVER_OBJECT pDriverObject);