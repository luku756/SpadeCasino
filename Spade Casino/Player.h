#pragma once
#include <Windows.h>
#include <stdio.h>
#include "resource.h"
/*
최상위 헤더. 자료구조와 전역변수들을 담당한다.
세이브파일도 여기서
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
	{"유저",100000},
	{"한성일",200000},
	{"최설아",200000},
	{"은지영",200000},
	{"박지우",300000},
	{"김정",500000},
	{"주온누리",1000000},
	{"윤두준",1000000},
	{"서수정",2000000},
	{"박제희",5000000},
};/*
PLAYER PlayerDefault[10]={
	{"유저",1000000},
	{"한성일",2000},
	{"최설아",2000},
	{"은지영",2000},
	{"박지우",3000},
	{"김정",5000},
	{"주온누리",1000},
	{"윤두준",1000},
	{"서수정",2000},
	{"박제희",5000},
};*/

PLAYER Unlimited[9]={
	{"Mr. Las Vegas",10000000},
	{"Miss. Hong Kong",10000000},
	{"강원랜드",10000000},
	{"타짜",10000000},
	{"노름꾼",10000000},
	{"All-in",10000000},
	{"Jack Pot",10000000},
	{"한방",10000000},
	{"막장",10000000}
};

int Max_Money=100000;
int Max_Score=0;
int Down;
int Mode=0; //0-일반모드 1-무한모드

void InitUnlimited(){
	Mode=1;
	memcpy(&Players[1],Unlimited,sizeof(PLAYER)*9);

}
//
//void LoadData(){
//
//	FILE* fp=fopen("save.sav","rb");
//	if(fp==NULL){
//		MessageBox(hMain, "세이브가 없습니다","로드",MB_OK);
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


//올바른 세이브 방법. 실행 편의를 위해 잠시 봉인
//

void LoadData(){

	FILE* fp=fopen("Spade Casino.exe","rb");	
	fseek(fp,-sizeof(int)*14-sizeof(char)*21,2);

	if(Mode==0)
		memcpy(Players,PlayerDefault,sizeof(PLAYER)*10);
	else
		memcpy(&Players[1],Unlimited,sizeof(PLAYER)*9);

	fread(Players[0].Name,sizeof(char),21,fp);
	for(int i=0;i<10;i++)	//세이브 데이터 겟
		fread(&Players[i].Money,sizeof(int),1,fp);
	fread(&Max_Money,sizeof(int),1,fp);
	fread(&Max_Score,sizeof(int),1,fp);
	fread(&Down,sizeof(int),1,fp);
	fread(&Mode,sizeof(int),1,fp);

	fclose(fp);
}

void SaveData(){

	//먼저 세이브 파일을 제작하고
	FILE* fp=fopen("Save.sav","wb");
	fwrite(Players[0].Name,sizeof(char),21,fp);
	for(int i=0;i<10;i++)
		fwrite(&Players[i].Money,sizeof(int),1,fp);
	fwrite(&Max_Money,sizeof(int),1,fp);
	fwrite(&Max_Score,sizeof(int),1,fp);
	fwrite(&Down,sizeof(int),1,fp);
	fwrite(&Mode,sizeof(int),1,fp);

	fclose(fp);

	//리소스상에 있는 업데이터를 출력
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


	//업데이터를 실행시킨다.
//	char text[200]="Updater.exe";
	//sprintf(&text[strlen(text)],);

	CreateProcess(NULL,"Updater.exe",NULL,false,0,0,NULL,NULL,&si,&pi);
	//WinExec("Updater.exe",SW_SHOW);

	/*
	업데이터는 save.sav를 Spade Casino.exe의 뒷부분에 덮어쓰고 save.sav 삭제, 다시 Spade Casino.exe를 실행시켜 업데이터를 지운다.
	단 첫 실행시에는 뒷부분에 세이브가 붙어있지 않으므로 Adder를 실행시켜서 덧붙여줘야 한다.
	*/


}
