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
	cout << "��ӭʹ��NativeAPI��3.0�汾��ʾ������ʹ�ù���ԱȨ��������ʹ��ȫ�����ܡ�\n����Ϊ�����˵�:" << endl;
	cout << "[���� ����] ---       ����" << endl;
	cout << "[    1    ] --- ����ϵͳ����(��������:0xC000021A)" << endl;
	cout << "[    2    ] --- ����Ϊϵͳ�ؼ�����" << endl;
	cout << "[    3    ] --- ȡ������Ϊϵͳ�ؼ�����" << endl;
	cout << "[    4    ] --- ����10����Զ��رյĵ���" << endl;
	cout << "[    5    ] --- ���ں�Ȩ��ɱ��ָ�����ƵĽ���" << endl;
	cout << "[    6    ] --- ���ں�Ȩ��ɱ��ָ��PID�Ľ���" << endl;
	cout << "[    7    ] --- ���ں�Ȩ�޹رռ����[��ʽ1]" << endl;
	cout << "[    8    ] --- ���ں�Ȩ�޹رռ����[��ʽ2]" << endl;
	cout << "[    9    ] --- ���ں�Ȩ���������������" << endl;
	cout << "[    A    ] --- ��ȫ�˳�������" << endl;
	cout << "��ע�⣺�������Ϊ��ɫ����ʱֱ�ӹرձ����򽫻ᵼ��ϵͳ������" << endl;
	while (true)
	{
		cout << "������������룺";
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
				cout << "����Ϊϵͳ���̳ɹ�!" << endl;
			}
			else if(re==-1)
			{
				cout << "����Ϊϵͳ����ʧ�ܣ��޷����DEBUGȨ��" << endl;
			}
			else if(re==-2)
			{
				cout << "����Ϊϵͳ����ʧ�ܣ��޷�����ntdll.dll" << endl;
			}
			break;
		}
		case '3':
		{
			re=NativeAPI::CancleSystemProcess();
			if(re==0)
			{
				system("color 07");
				cout << "��ȡ������Ϊϵͳ�ؼ�����!" << endl;
			}
			else if(re==-1)
			{
				cout << "ȡ������Ϊϵͳ����ʧ�ܣ��޷����DEBUGȨ��" << endl;
			}
			else if(re==-2)
			{
				cout << "ȡ������Ϊϵͳ����ʧ�ܣ��޷�����ntdll.dll" << endl;
			}
			break;
		}
		case '4':
		{
			NativeAPI::MessageBoxTimeoutS(TEXT("�˶Ի������10����Զ��ر�"), TEXT("��ʾ�Ի���"), MB_OK | MB_ICONINFORMATION, 10000);
			cout << "�Ի����ѵ�����" << endl;
			break;
		}
		case '5':
		{
			do
			{
				cout << "������Ҫ�����Ľ�����:";
				cin.getline(name,50);
				cin.getline(name,50);
				cin.clear();
			}
			while(strcmp(name,empty)==0);
			ret=NativeAPI::KillProcess(name);
			if(!ret)
			{
				cout << "���� "<<name<<" �ѱ�����"<<endl;
			}
			else
			{
				cout << "���� "<<name<<" �޷�������"<<endl;
			}
			break;
		}
		case '6':
		{
			do
			{
				cout << "������Ҫ�����Ľ���PID:";
				cin.getline(name,50);
				cin.getline(name,50);
				cin.clear();
				pid=atoi(name);
			}
			while(pid==0);
			ret=NativeAPI::KillProcess(pid);
			if(!ret)
			{
				cout << "���� "<<pid<<" �ѱ�����"<<endl;
			}
			else
			{
				cout << "���� "<<pid<<" �޷�������"<<endl;
			}
			break;
		}
		case '7':
		{
		    cout<<"���ڳ������ں�Ȩ�޹رռ����[��ʽ1]......"<<endl;
		    NativeAPI::NtShutdown1();
		}
		case '8':
		{
		    cout<<"���ڳ������ں�Ȩ�޹رռ����[��ʽ2]......"<<endl;
		    NativeAPI::NtShutdown2();
		}
		case '9':
		{
		    cout<<"���ڳ������ں�Ȩ����������......"<<endl;
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
