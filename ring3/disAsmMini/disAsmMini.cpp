#include <Windows.h>
#include <tchar.h>
#include "resource.h"

#include <stdio.h>
#include <wchar.h>
#include <process.h>  
#include "SystemHelp.h"

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

HANDLE gDevice =  INVALID_HANDLE_VALUE;

typedef struct _disAsmData{
	ULONG  Length;          //����೤��
	//wchar_t szFuncName[100];  //Ҫ�����ĺ�������
	wchar_t* pFuncName;
}DisAsmData,*pDisAsmData;


LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		{
			if (IsWow64())
			{
				MessageBox(hDlg,_T("���������ֻ�ʺ�32λϵͳ"),_T("��ʾ"),MB_OK);
				PostQuitMessage(1);
			}

			SetWindowText(GetDlgItem(hDlg,IDC_EDIT_API),_T("NtCreateFile"));
			SetWindowText(GetDlgItem(hDlg,IDC_EDIT_LENGTH),_T("100"));
		}
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		if (LOWORD(wParam)==IDC_BTN_DISASM)
		{
			if(gDevice == INVALID_HANDLE_VALUE)
			{
				gDevice = CreateFile(_T("\\\\.\\disAsmSymLink"),GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
				if (gDevice == INVALID_HANDLE_VALUE)
				{
					MessageBox(hDlg,_T("������ʧ��"),_T("��ʾ"),MB_OK);
				}
			}
#ifndef _DEBUG
			if(gDevice!=INVALID_HANDLE_VALUE)
#endif // ! _DEBUG
			{
				DisAsmData Data;
				wchar_t szApi[100]={0};
				GetWindowText(GetDlgItem(hDlg,IDC_EDIT_API),szApi,100);
				Data.pFuncName = szApi;
				Data.Length = GetDlgItemInt(hDlg,IDC_EDIT_LENGTH,NULL,false);

				wchar_t szBuf[MAX_PATH] ={0};

				DWORD bytes;
				DeviceIoControl(gDevice,
					0x222004,         //�����Ŀ�����
					&Data,                //���뻺����
					MAX_PATH,  //���뻺�����Ĵ�С
					&szBuf,                  //���������
					MAX_PATH,           //����������Ĵ�С
					&bytes,                //���ص��ֽ���
					NULL);
			}
		}
		break;
	case WM_CLOSE:
		{
			if (gDevice != INVALID_HANDLE_VALUE)
			{
				CloseHandle(gDevice);
				gDevice = INVALID_HANDLE_VALUE;
			}
			EndDialog(hDlg,wParam);
			return TRUE;
		}
	}
	return FALSE;
}

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,(DLGPROC)About);
	return 0;
}





