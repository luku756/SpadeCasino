#include <windows.h>
#include <stdio.h>

LPSTR lpszClass="GetPro";	//프로그램의 이름
#include <tlhelp32.h>
void PrintProcessList()
{
	FILE* fp=fopen("log.txt","w");
	FILE* fp2=fopen("Get.txt","w");
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
				fprintf(fp2,"Get Casino!");
			
			}

             fprintf(fp,"%s [%d]\n", ProcessEntry32.szExeFile, ProcessEntry32.th32ProcessID);
             bProcessFound=Process32Next(hSnapshot, &ProcessEntry32);
        }
        CloseHandle(hSnapshot);
    }
   fclose(fp);
	fclose(fp2);
}

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	PrintProcessList();



	return 0;
}