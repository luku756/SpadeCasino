#pragma once
#include <Windows.h>
#include <stdio.h>
#include "resource.h"
/*
�ֻ��� ���. �ڷᱸ���� ������������ ����Ѵ�.
���̺����ϵ� ���⼭
*/
HWND hMain,nGame,hSel;
HINSTANCE g_hInst;
bool Opener;

typedef struct Player{

	char Name[30];
	int Money;

}PLAYER;

PLAYER Players[10];
PLAYER PlayerDefault[10]={
	{"����",100000},
	{"�Ѽ���",200000},
	{"�ּ���",200000},
	{"������",200000},
	{"������",300000},
	{"����",500000},
	{"�ֿ´���",1000000},
	{"������",1000000},
	{"������",2000000},
	{"������",5000000},
};/*
PLAYER PlayerDefault[10]={
	{"����",1000000},
	{"�Ѽ���",2000},
	{"�ּ���",2000},
	{"������",2000},
	{"������",3000},
	{"����",5000},
	{"�ֿ´���",1000},
	{"������",1000},
	{"������",2000},
	{"������",5000},
};*/

PLAYER Unlimited[9]={
	{"Mr. Las Vegas",10000000},
	{"Miss. Hong Kong",10000000},
	{"��������",10000000},
	{"Ÿ¥",10000000},
	{"�븧��",10000000},
	{"All-in",10000000},
	{"Jack Pot",10000000},
	{"�ѹ�",10000000},
	{"����",10000000}
};

int Max_Money=100000;
int Max_Score=0;
int Down;
int Mode=0; //0-�Ϲݸ�� 1-���Ѹ��

void InitUnlimited(){
	Mode=1;
	memcpy(&Players[1],Unlimited,sizeof(PLAYER)*9);

}
//
//void LoadData(){
//
//	FILE* fp=fopen("save.sav","rb");
//	if(fp==NULL){
//		MessageBox(hMain, "���̺갡 �����ϴ�","�ε�",MB_OK);
//		return;
//	}
//	fread(Players[0].Name,sizeof(char),21,fp);
//	for(int i=0;i<10;i++)
//		fread(&Players[i].Money,sizeof(int),1,fp);
//	fread(&Max_Money,sizeof(int),1,fp);
//	fread(&Max_Score,sizeof(int),1,fp);
//	fread(&Down,sizeof(int),1,fp);
//	fread(&Mode,sizeof(int),1,fp);
//	
//	fclose(fp);
//}
//
//void SaveData(){
//
//	FILE* fp=fopen("save.sav","wb");
//	fwrite(Players[0].Name,sizeof(char),21,fp);
//	for(int i=0;i<10;i++)
//		fwrite(&Players[i].Money,sizeof(int),1,fp);
//	fwrite(&Max_Money,sizeof(int),1,fp);
//	fwrite(&Max_Score,sizeof(int),1,fp);
//	fwrite(&Down,sizeof(int),1,fp);
//	fwrite(&Mode,sizeof(int),1,fp);
//	
//	fclose(fp);
//}


//�ùٸ� ���̺� ���. ���� ���Ǹ� ���� ��� ����
//

void LoadData(){

	FILE* fp=fopen("Spade Casino.exe","rb");	
	fseek(fp,-sizeof(int)*14-sizeof(char)*21,2);

	if(Mode==0)
		memcpy(Players,PlayerDefault,sizeof(PLAYER)*10);
	else
		memcpy(&Players[1],Unlimited,sizeof(PLAYER)*9);

	fread(Players[0].Name,sizeof(char),21,fp);
	for(int i=0;i<10;i++)	//���̺� ������ ��
		fread(&Players[i].Money,sizeof(int),1,fp);
	fread(&Max_Money,sizeof(int),1,fp);
	fread(&Max_Score,sizeof(int),1,fp);
	fread(&Down,sizeof(int),1,fp);
	fread(&Mode,sizeof(int),1,fp);

	fclose(fp);
}

void SaveData(){

	//���� ���̺� ������ �����ϰ�
	FILE* fp=fopen("Save.sav","wb");
	fwrite(Players[0].Name,sizeof(char),21,fp);
	for(int i=0;i<10;i++)
		fwrite(&Players[i].Money,sizeof(int),1,fp);
	fwrite(&Max_Money,sizeof(int),1,fp);
	fwrite(&Max_Score,sizeof(int),1,fp);
	fwrite(&Down,sizeof(int),1,fp);
	fwrite(&Mode,sizeof(int),1,fp);

	fclose(fp);

	//���ҽ��� �ִ� �������͸� ���
	fp=fopen("Updater.exe","wb");

	HRSRC hRSrc;
	DWORD Size;
	HGLOBAL hMem;
	PVOID ptr;

	hRSrc=FindResource(g_hInst,MAKEINTRESOURCE(IDR_UPDATER),"EXE");
	Size=SizeofResource(g_hInst,hRSrc);
	hMem=LoadResource(g_hInst,hRSrc);
	ptr=LockResource(hMem);

	fwrite(ptr,1,Size,fp);
	fclose(fp);

	STARTUPINFO si={sizeof(STARTUPINFO),};
	PROCESS_INFORMATION pi;


	//�������͸� �����Ų��.
//	char text[200]="Updater.exe";
	//sprintf(&text[strlen(text)],);

	CreateProcess(NULL,"Updater.exe",NULL,false,0,0,NULL,NULL,&si,&pi);
	//WinExec("Updater.exe",SW_SHOW);

	/*
	�������ʹ� save.sav�� Spade Casino.exe�� �޺κп� ����� save.sav ����, �ٽ� Spade Casino.exe�� ������� �������͸� �����.
	�� ù ����ÿ��� �޺κп� ���̺갡 �پ����� �����Ƿ� Adder�� ������Ѽ� ���ٿ���� �Ѵ�.
	*/


}
