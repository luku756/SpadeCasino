#include <windows.h>
#include <stdio.h>

LPSTR lpszClass="Updater";	//프로그램의 이름

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	FILE* fp=fopen("Spade Casino.exe","ab");
	fseek(fp,0,2);
	FILE* fp2=fopen("Save.sav","rb");

	typedef struct Player{

		char Name[100];
		int Money;

	}PLAYER;

	PLAYER Players[10];
	char name[22];
	int Max_Money;
	int Max_Score;
	int Down;
	int Mode;

	fread(name,sizeof(char),21,fp2);
	for(int i=0;i<10;i++)	//세이브 데이터 겟
		fread(&Players[i].Money,sizeof(int),1,fp2);
	fread(&Max_Money,sizeof(int),1,fp2);
	fread(&Max_Score,sizeof(int),1,fp2);
	fread(&Down,sizeof(int),1,fp2);
	fread(&Mode,sizeof(int),1,fp2);

	
	fwrite(name,sizeof(char),21,fp);
	for(int i=0;i<10;i++)
		fwrite(&Players[i].Money,sizeof(int),1,fp);
	fwrite(&Max_Money,sizeof(int),1,fp);
	fwrite(&Max_Score,sizeof(int),1,fp);
	fwrite(&Down,sizeof(int),1,fp);
	fwrite(&Mode,sizeof(int),1,fp);

	fclose(fp);
	fclose(fp2);

	return 0;
}