#include "NativeAPI.h"
//#include "NativeAPI.cpp" 
#include <iostream>

using namespace std;

int main(void)
{
	char in;
	int pid;
	BOOL ret;
	int re;
	char name[50],empty[50];
	cout << "欢迎使用NativeAPI类3.0版本演示程序，请使用管理员权限运行以使用全部功能。\n以下为操作菜单:" << endl;
	cout << "[操作 代码] ---       操作" << endl;
	cout << "[    1    ] --- 触发系统蓝屏(蓝屏代码:0xC000021A)" << endl;
	cout << "[    2    ] --- 设置为系统关键进程" << endl;
	cout << "[    3    ] --- 取消设置为系统关键进程" << endl;
	cout << "[    4    ] --- 弹出10秒后自动关闭的弹窗" << endl;
	cout << "[    5    ] --- 以内核权限杀死指定名称的进程" << endl;
	cout << "[    6    ] --- 以内核权限杀死指定PID的进程" << endl;
	cout << "[    7    ] --- 以内核权限关闭计算机[方式1]" << endl;
	cout << "[    8    ] --- 以内核权限关闭计算机[方式2]" << endl;
	cout << "[    9    ] --- 以内核权限重新启动计算机" << endl;
	cout << "[    A    ] --- 安全退出本程序" << endl;
	cout << "请注意：当程序变为红色背景时直接关闭本程序将会导致系统蓝屏！" << endl;
	while (true)
	{
		cout << "请输入操作代码：";
		cin.clear();
		in = getchar();
		switch (in)
		{
		case '1':
		{
			NativeAPI::MakeBlueScreen(0xC000031A);
			break;
		}
		case '2':
		{
			re=NativeAPI::SetSystemProcess();
			if(re==0)
			{
				system("color 47");
				cout << "设置为系统进程成功!" << endl;
			}
			else if(re==-1)
			{
				cout << "设置为系统进程失败：无法获得DEBUG权限" << endl;
			}
			else if(re==-2)
			{
				cout << "设置为系统进程失败：无法加载ntdll.dll" << endl;
			}
			break;
		}
		case '3':
		{
			re=NativeAPI::CancleSystemProcess();
			if(re==0)
			{
				system("color 07");
				cout << "已取消设置为系统关键进程!" << endl;
			}
			else if(re==-1)
			{
				cout << "取消设置为系统进程失败：无法获得DEBUG权限" << endl;
			}
			else if(re==-2)
			{
				cout << "取消设置为系统进程失败：无法加载ntdll.dll" << endl;
			}
			break;
		}
		case '4':
		{
			NativeAPI::MessageBoxTimeoutS(TEXT("此对话框会在10秒后自动关闭"), TEXT("演示对话框"), MB_OK | MB_ICONINFORMATION, 10000);
			cout << "对话框已弹出！" << endl;
			break;
		}
		case '5':
		{
			do
			{
				cout << "请输入要结束的进程名:";
				cin.getline(name,50);
				cin.getline(name,50);
				cin.clear();
			}
			while(strcmp(name,empty)==0);
			ret=NativeAPI::KillProcess(name);
			if(!ret)
			{
				cout << "进程 "<<name<<" 已被结束"<<endl;
			}
			else
			{
				cout << "进程 "<<name<<" 无法被结束"<<endl;
			}
			break;
		}
		case '6':
		{
			do
			{
				cout << "请输入要结束的进程PID:";
				cin.getline(name,50);
				cin.getline(name,50);
				cin.clear();
				pid=atoi(name);
			}
			while(pid==0);
			ret=NativeAPI::KillProcess(pid);
			if(!ret)
			{
				cout << "进程 "<<pid<<" 已被结束"<<endl;
			}
			else
			{
				cout << "进程 "<<pid<<" 无法被结束"<<endl;
			}
			break;
		}
		case '7':
		{
		    cout<<"正在尝试以内核权限关闭计算机[方式1]......"<<endl;
		    NativeAPI::NtShutdown1();
		}
		case '8':
		{
		    cout<<"正在尝试以内核权限关闭计算机[方式2]......"<<endl;
		    NativeAPI::NtShutdown2();
		}
		case '9':
		{
		    cout<<"正在尝试以内核权限重新启动......"<<endl;
		    NativeAPI::NtReboot();
		}
		case 'A':
		{
			NativeAPI::CancleSystemProcess();
			system("color 07");
			exit(0);
			break;
		}
		}
	}
	return 0;
}
