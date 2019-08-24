#include "NativeAPI.h"

int MessageBoxTimeoutA(IN HWND hWnd, IN LPCSTR lpText, IN LPCSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds)
{
    static MSGBOXAAPI MsgBoxTOA = NULL;
    if (!MsgBoxTOA)
    {
        HMODULE hUser32 = GetModuleHandle(_T("user32.dll"));
        if (hUser32)
        {
            MsgBoxTOA = (MSGBOXAAPI)GetProcAddress(hUser32, "MessageBoxTimeoutA");
            //fall through to 'if (MsgBoxTOA)...'
        }
        else
        {
            //stuff happened, add code to handle it here 
            //(possibly just call MessageBox())
            return 0;
        }
    }
    if (MsgBoxTOA)
    {
        return MsgBoxTOA(hWnd, lpText, lpCaption, uType, wLanguageId, dwMilliseconds);
    }
    return 0;
}

int MessageBoxTimeoutW(IN HWND hWnd, IN LPCWSTR lpText, IN LPCWSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds)
{
    static MSGBOXWAPI MsgBoxTOW = NULL;
    if (!MsgBoxTOW)
    {
        HMODULE hUser32 = GetModuleHandle(_T("user32.dll"));
        if (hUser32)
        {
            MsgBoxTOW = (MSGBOXWAPI)GetProcAddress(hUser32, "MessageBoxTimeoutW");
            //fall through to 'if (MsgBoxTOW)...'
        }
        else
        {
            //stuff happened, add code to handle it here 
            //(possibly just call MessageBox())
            return 0;
        }
    }
    if (MsgBoxTOW)
    {
        return MsgBoxTOW(hWnd, lpText, lpCaption, uType, wLanguageId, dwMilliseconds);
    }
    return 0;
}

bool NativeAPI::EnableDebugPrivilege()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return false;
	}
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		CloseHandle(hToken);
		return false;
	}
	return true;
}

BOOL NativeAPI::SetPrivilege(LPCSTR lpPrivilegeName, BOOL fEnable)
{

	HANDLE hToken;
	TOKEN_PRIVILEGES NewState;
	LUID luidPrivilegeLUID;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		/*If opening token failed...*/
		return FALSE;
	}
	if (fEnable == FALSE) /*We disable all the privileges... */
	{
		if (!AdjustTokenPrivileges(hToken, TRUE, NULL, NULL, NULL, NULL))
		{
			return FALSE;
		}
		else return TRUE;
	}
	/*Look up the LUID value of the privilege... */
	LookupPrivilegeValue(NULL, lpPrivilegeName, &luidPrivilegeLUID);
	NewState.PrivilegeCount = 1;
	NewState.Privileges[0].Luid = luidPrivilegeLUID;
	NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	/*Improve this process's privilege, so we can shut down the system next. */
	if (!AdjustTokenPrivileges(hToken, FALSE, &NewState, NULL, NULL, NULL))
	{
		return FALSE;
	}
	/*We should not only check if the improving was successed... */
	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		return FALSE;
	}
	return TRUE;
}

DWORD NativeAPI::GetPidByName(char *szName) 
{ 
    HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
    PROCESSENTRY32 pe32={0};
    DWORD dwRet=0;
    hProcessSnap =CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE) 
        return 0;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if(Process32First(hProcessSnap, &pe32)) 
    { 
        do 
        { 
            if(lstrcmpi(szName,pe32.szExeFile)==0) 
            { 
                dwRet=pe32.th32ProcessID;
                break;
            }
        } 
        while (Process32Next(hProcessSnap,&pe32));
    } 
    else return 0;
    if(hProcessSnap !=INVALID_HANDLE_VALUE) CloseHandle(hProcessSnap);
        return dwRet;
} 

int NtGetDebug()
{
    HINSTANCE hDLL = LoadLibrary(TEXT("ntdll.dll"));
	RTLADJUSTPRIVILEGE RtlAdjustPrivilege;
	int nEn = 0;
	HANDLE hToken;
	LUID luidPrivilegeLUID;
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luidPrivilegeLUID);
	if (hDLL != NULL)
	{
		RtlAdjustPrivilege = (RTLADJUSTPRIVILEGE)GetProcAddress(hDLL, "RtlAdjustPrivilege");
		if (!RtlAdjustPrivilege)
		{
			// handle the error
			FreeLibrary(hDLL);
			return -1;
		}
		RtlAdjustPrivilege(luidPrivilegeLUID.LowPart, TRUE, FALSE, &nEn);
		FreeLibrary(hDLL);
	}
	else
	{
	    FreeLibrary(hDLL);
	    return -3;
	}
	FreeLibrary(hDLL);
	return 0;
}

int NativeAPI::MakeBlueScreen(DWORD BSODCode)
{
	HINSTANCE hDLL = LoadLibrary(TEXT("ntdll.dll"));
	NTRAISEHARDERROR NtRaiseHardError;
	RTLADJUSTPRIVILEGE RtlAdjustPrivilege;
	int nEn = 0;
	HARDERROR_RESPONSE reResponse;
	if (hDLL != NULL)
	{
		NtRaiseHardError = (NTRAISEHARDERROR)GetProcAddress(hDLL, "NtRaiseHardError");
		RtlAdjustPrivilege = (RTLADJUSTPRIVILEGE)GetProcAddress(hDLL, "RtlAdjustPrivilege");
		if (!NtRaiseHardError)
		{
			// handle the error
			FreeLibrary(hDLL);
			return 0;
		}
		if (!RtlAdjustPrivilege)
		{
			// handle the error
			FreeLibrary(hDLL);
			return 0;
		}
		RtlAdjustPrivilege(0x13, TRUE, FALSE, &nEn);//0x13 = SeShutdownPrivilege
		NtRaiseHardError(BSODCode,0,0,0,OptionShutdownSystem,&reResponse);
		FreeLibrary(hDLL);
	}
	return 1;
}

int NativeAPI::SetSystemProcess()
{
	//EnableDebugPrivilege();
	//fnRtlSetProcessIsCritical pRtlSetProcessIsCritical;
	HMODULE hDll = LoadLibrary(TEXT("ntdll.dll"));
	if (hDll)
	{
		type_RtlSetProcessIsCritical RtlSetProcessIsCritical = (type_RtlSetThreadIsCritical)GetProcAddress(hDll, "RtlSetProcessIsCritical");
		type_RtlSetProcessIsCritical RtlSetThreadIsCritical = (type_RtlSetThreadIsCritical)GetProcAddress(hDll, "RtlSetThreadIsCritical");
		//bool bSuccess = SetPrivilege(SE_DEBUG_NAME, TRUE);
		int ret=NtGetDebug();
		if (ret!=0)
		{
			FreeLibrary(hDll);
			return -1;
		}
		else
		{
			RtlSetProcessIsCritical(TRUE, NULL, FALSE);
			RtlSetThreadIsCritical(TRUE, NULL, FALSE);
			FreeLibrary(hDll);
			return 0;
		}
	}
	else
	{
		FreeLibrary(hDll);
		return -2;
	} 
}

int NativeAPI::CancleSystemProcess()
{
	//EnableDebugPrivilege();
	//fnRtlSetProcessIsCritical pRtlSetProcessIsCritical;
	HMODULE hDll = LoadLibrary(TEXT("ntdll.dll"));
	if (hDll)
	{
		type_RtlSetProcessIsCritical RtlSetProcessIsCritical = (type_RtlSetThreadIsCritical)GetProcAddress(hDll, "RtlSetProcessIsCritical");
		type_RtlSetProcessIsCritical RtlSetThreadIsCritical = (type_RtlSetThreadIsCritical)GetProcAddress(hDll, "RtlSetThreadIsCritical");
		int ret=NtGetDebug();
		if (ret!=0)
		{
			return -1;
		}
		else
		{
			RtlSetProcessIsCritical(FALSE, NULL, FALSE);
			RtlSetThreadIsCritical(FALSE, NULL, FALSE);
			SetPrivilege(NULL, FALSE);
			return 0;
		}
	}
	else return -2;
}

int NativeAPI::MessageBoxTimeoutS(LPCTSTR lpText,LPCTSTR lpCaption,UINT uType,DWORD dwMilliseconds)
{
	MessageBoxTimeoutP;
	int ret = MessageBoxTimeout(NULL, lpText, lpCaption, uType, 0, dwMilliseconds);
	MessageBoxTimeoutC;
	return ret;
}

BOOL NativeAPI::KillProcess(char* Name)
{
	DWORD dwProcessId=GetPidByName(Name);
	HMODULE hModule = GetModuleHandle("ntdll.dll");  
	if(hModule == NULL) 
		return FALSE; 
	ZWOPENPROCESS ZwOpenProcess = (ZWOPENPROCESS)GetProcAddress(hModule,"ZwOpenProcess"); 
	if(ZwOpenProcess == NULL) 
		return FALSE; 
	ZWTERMINATEPROCESS ZwTerminateProcess = (ZWTERMINATEPROCESS)GetProcAddress(hModule,"ZwTerminateProcess"); 
	if(ZwTerminateProcess == NULL) 
		return FALSE; 
	HANDLE hProcess = NULL; 
	CLIENT_ID ClientId; 
	ClientId.UniqueProcess = (HANDLE)dwProcessId; 
	ClientId.UniqueThread = 0; 
	OBJECT_ATTRIBUTES oa; 
	oa.Length = sizeof(oa); 
	oa.RootDirectory = 0; 
	oa.ObjectName = 0; 
	oa.Attributes = 0; 
	oa.SecurityDescriptor = 0; 
	oa.SecurityQualityOfService = 0; 
	ZwOpenProcess(&hProcess,PROCESS_TERMINATE,&oa,&ClientId); 
	if(hProcess == NULL) 
		return FALSE; 
	return ZwTerminateProcess(hProcess,0);
}

BOOL NativeAPI::KillProcess(DWORD dwProcessId)
{
	HMODULE hModule = GetModuleHandle("ntdll.dll");  
	if(hModule == NULL) 
		return FALSE; 
	ZWOPENPROCESS ZwOpenProcess = (ZWOPENPROCESS)GetProcAddress(hModule,"ZwOpenProcess"); 
	if(ZwOpenProcess == NULL) 
		return FALSE; 
	ZWTERMINATEPROCESS ZwTerminateProcess = (ZWTERMINATEPROCESS)GetProcAddress(hModule,"ZwTerminateProcess"); 
	if(ZwTerminateProcess == NULL) 
		return FALSE; 
	HANDLE hProcess = NULL; 
	CLIENT_ID ClientId; 
	ClientId.UniqueProcess = (HANDLE)dwProcessId; 
	ClientId.UniqueThread = 0; 
	OBJECT_ATTRIBUTES oa; 
	oa.Length = sizeof(oa); 
	oa.RootDirectory = 0; 
	oa.ObjectName = 0; 
	oa.Attributes = 0; 
	oa.SecurityDescriptor = 0; 
	oa.SecurityQualityOfService = 0; 
	ZwOpenProcess(&hProcess,PROCESS_TERMINATE,&oa,&ClientId); 
	if(hProcess == NULL) 
		return FALSE; 
	return ZwTerminateProcess(hProcess,0);
}

DWORD NativeAPI::NtShutdown1()
{
    HMODULE hNtdll;
    TOKEN_PRIVILEGES tkp;
    HANDLE hToken;
    lpNtShutdownSystem NtShutdownSystem;
    OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY|TOKEN_ADJUST_PRIVILEGES,&hToken);
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
    CloseHandle(hToken);
    hNtdll = GetModuleHandle("ntdll.dll");
    if (hNtdll)
    {
        NtShutdownSystem = (lpNtShutdownSystem)GetProcAddress(hNtdll,"NtShutdownSystem");
        if (NtShutdownSystem != NULL)
        {
            return NtShutdownSystem(ShutdownNoReboot);
        }
    }
    else
    {
        tkp.Privileges[0].Attributes = 0; 
        ;AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
        return -1;
    }
}

DWORD NativeAPI::NtReboot()
{
    HMODULE hNtdll;
    TOKEN_PRIVILEGES tkp;
    HANDLE hToken;
    lpNtShutdownSystem NtShutdownSystem;
    OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY|TOKEN_ADJUST_PRIVILEGES,&hToken);
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
    CloseHandle(hToken);
    hNtdll = GetModuleHandle("ntdll.dll");
    if (hNtdll)
    {
        NtShutdownSystem = (lpNtShutdownSystem)GetProcAddress(hNtdll,"NtShutdownSystem");
        if (NtShutdownSystem != NULL)
        {
            return NtShutdownSystem(ShutdownReboot);
        }
    }
    else
    {
        tkp.Privileges[0].Attributes = 0; 
        ;AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
        return -1;
    }
}

DWORD NativeAPI::NtShutdown2() //may be
{
    HMODULE hNtdll;
    TOKEN_PRIVILEGES tkp;
    HANDLE hToken;
    lpNtShutdownSystem NtShutdownSystem;
    OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY|TOKEN_ADJUST_PRIVILEGES,&hToken);
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
    CloseHandle(hToken);
    hNtdll = GetModuleHandle("ntdll.dll");
    if (hNtdll)
    {
        NtShutdownSystem = (lpNtShutdownSystem)GetProcAddress(hNtdll,"NtShutdownSystem");
        if (NtShutdownSystem != NULL)
        {
            return NtShutdownSystem(ShutdownPowerOff);
        }
    }
    else
    {
        tkp.Privileges[0].Attributes = 0; 
        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
        return -1;
    }
}