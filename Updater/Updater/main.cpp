#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

LPSTR lpszClass="Updater";	//프로그램의 이름

bool ViewProcessList()
{
   HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if(hSnapshot)
   {
        PROCESSENTRY32 ProcessEntry32;
        BOOL bProcessFound;
        ProcessEntry32.dwSize=sizeof(PROCESSENTRY32);
        bProcessFound=Process32First(hSnapshot, &ProcessEntry32);
        while(bProcessFound)
        {
			if(strcmp(ProcessEntry32.szExeFile,"Spade Casino.exe")==0){
				return 0;
			
			}
             bProcessFound=Process32Next(hSnapshot, &ProcessEntry32);
        }
        CloseHandle(hSnapshot);
    }

   return 1;
}

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	FILE* fp;
	int k=0;
	//FILE* fps=fopen("log_prolist.txt","a");
	while(!ViewProcessList()){
	//	fprintf(fps,"%d\n",k++);
	}
	//fclose(fps);
	fp=fopen("Spade Casino.exe","ab");
	if(fp==NULL){
		fp=fopen("Spade Casino_Missing.txt","w");
		fprintf(fp,"%s",lpszCmdParam);
		fclose(fp);
		return -1;
	}

	fseek(fp,-sizeof(int)*14-sizeof(char)*21,2);
	FILE* fp2=fopen("Save.sav","rb");

	if(fp2==NULL){
		fp2=fopen("Save_Missing.txt","w");
		fclose(fp2);
		return -1;
	}

	typedef struct Player{
		char Name[100];
		int Money;

	}PLAYER;

	char name[22];
	PLAYER Players[10];
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

	DeleteFile("Save.sav");
	
	
	STARTUPINFO si={sizeof(STARTUPINFO),};
	PROCESS_INFORMATION pi;
	CreateProcess(NULL,"Spade Casino.exe",NULL,false,0,0,NULL,NULL,&si,&pi);
	//WinExec("Spade Casino.exe",SW_SHOW);

	return 0;
}