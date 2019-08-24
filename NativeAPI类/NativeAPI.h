#ifndef _NT_HDRS_
#define _NT_HDRS_

#include <Windows.h>
#include <tchar.h>
#include <tlhelp32.h>

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define MessageBoxTimeoutP HMODULE hUser32 = LoadLibrary(TEXT("user32.dll"));
#define MessageBoxTimeoutC FreeLibrary(hUser32);

typedef /*__success(return >= 0)*/ LONG NTSTATUS;

typedef NTSTATUS *PNTSTATUS;

typedef struct _LSA_UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

typedef enum _HARDERROR_RESPONSE_OPTION
{
	OptionAbortRetryIgnore,
	OptionOk,
	OptionOkCancel,
	OptionRetryCancel,
	OptionYesNo,
	OptionYesNoCancel,
	OptionShutdownSystem
} HARDERROR_RESPONSE_OPTION, *PHARDERROR_RESPONSE_OPTION;

typedef enum _HARDERROR_RESPONSE 
{
	ResponseReturnToCaller,
	ResponseNotHandled,
	ResponseAbort,
	ResponseCancel,
	ResponseIgnore,
	ResponseNo,
	ResponseOk,
	ResponseRetry,
	ResponseYes
} HARDERROR_RESPONSE, *PHARDERROR_RESPONSE;

typedef struct _CLIENT_ID 
{ 
	HANDLE UniqueProcess; 
	HANDLE UniqueThread; 
}CLIENT_ID, *PCLIENT_ID; 

typedef struct _OBJECT_ATTRIBUTES 
{ 
	ULONG Length; 
	HANDLE RootDirectory; 
	PVOID ObjectName; 
	ULONG Attributes; 
	PVOID SecurityDescriptor; 
	PVOID SecurityQualityOfService; 
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef enum _SHUTDOWN_ACTION
{
    ShutdownNoReboot,
    ShutdownReboot,
    ShutdownPowerOff
} SHUTDOWN_ACTION;

typedef UINT(CALLBACK* NTRAISEHARDERROR)(NTSTATUS, ULONG, PUNICODE_STRING, PVOID, HARDERROR_RESPONSE_OPTION, PHARDERROR_RESPONSE);

typedef UINT(CALLBACK* RTLADJUSTPRIVILEGE)(ULONG, BOOL, BOOL, PINT);

typedef int(__stdcall *MSGBOXAAPI)(IN HWND hWnd, IN LPCSTR lpText, IN LPCSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);

typedef int(__stdcall *MSGBOXWAPI)(IN HWND hWnd, IN LPCWSTR lpText, IN LPCWSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);

typedef NTSTATUS(__cdecl *fnRtlSetProcessIsCritical)(IN  BOOLEAN  NewValue, OUT PBOOLEAN OldValue OPTIONAL, IN  BOOLEAN  CheckFlag);

typedef VOID(WINAPI* type_RtlSetProcessIsCritical)(BOOLEAN, PBOOLEAN, BOOLEAN);

typedef VOID(WINAPI* type_RtlSetThreadIsCritical)(BOOLEAN, PBOOLEAN, BOOLEAN);

int MessageBoxTimeoutA(IN HWND hWnd, IN LPCSTR lpText, IN LPCSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);

int MessageBoxTimeoutW(IN HWND hWnd, IN LPCWSTR lpText, IN LPCWSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);

typedef DWORD (__stdcall *ZWOPENPROCESS)( OUT PHANDLE ProcessHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN PCLIENT_ID PCLIENT_ID OPTIONAL ); 

typedef DWORD (__stdcall *ZWTERMINATEPROCESS)( HANDLE hProcess, UINT uExitCode); 

typedef DWORD (WINAPI* lpNtShutdownSystem)(SHUTDOWN_ACTION Action);

#ifdef UNICODE
    #define MessageBoxTimeout MessageBoxTimeoutW
#else
    #define MessageBoxTimeout MessageBoxTimeoutA
#endif 

#define MB_TIMEDOUT 32000

class NativeAPI
{
	private:
	static bool EnableDebugPrivilege();
	static BOOL SetPrivilege(LPCSTR lpPrivilegeName, BOOL fEnable);
	static DWORD GetPidByName(char *szName);
	public:
	static int MakeBlueScreen(DWORD BSODCode);		//0xC000021A
	static int SetSystemProcess();
	static int CancleSystemProcess();
	static int MessageBoxTimeoutS(LPCTSTR lpText,LPCTSTR lpCaption,UINT uType,DWORD dwMilliseconds);
	static BOOL KillProcess(char* Name);
	static BOOL KillProcess(DWORD dwProcessId);
	static DWORD NtShutdown1();
	static DWORD NtReboot();
	static DWORD NtShutdown2();
};

#endif