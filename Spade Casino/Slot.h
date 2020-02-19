#pragma once

#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include "Poker.h"
#include "resource.h"


#define ID_BT_START 10000
#define ID_BT_STOP1 10001
#define ID_BT_STOP2 10002
#define ID_BT_STOP3 10003
#define ID_BT_BET1 10004
#define ID_BT_BET2 10005
#define ID_BT_BET3 10006
#define ID_BT_BET4 10007
#define ID_BT_BET5 10008
#define ID_BT_BET6 10009
#define ID_BT_BET7 10010
#define ID_BT_BET8 10011
#define ID_BT_CLEAR 10012
#define ID_BT_LVUP 10013
#define ID_BT_LVDOWN 10014
#define ID_BT_BACK 10015

#define WM_CALSCORE 100001
int betmoney[8];	// 0 = ���� 1�� 1 = ���� 2�� 3 = ���� 3�� 4 = ���� 1�� 5 = ���� 2��  6 = ���� 3�� 7 = ������ �밢�� 8 = ����� �밢��
int Board[3][3];
int BetedMoney;	//���� ���� �ѷ�
HWND hSlot;
bool selectcheck[3];
HBITMAP hBit;
int betlevel;

void OnTimer(){
	RECT crt;
	HDC hdc,hMemDC;
	HBITMAP OldBit;
	int i;
	int pe[28]={101,105,107,110,111,112,113, 201,205,207,210,211,212,213, 301,305,307,310,311,312,313, 401,405,407,410,411,412,413};
	int xs[8]={150,150,150,250,350,450,150,550};
	int ys[8]={280,380,480,180,180,180,180,180};
	int CardImg;
	char text[300];

	GetClientRect(hSlot,&crt);	//slot�������� ũ�⿡ �°� �簢�� ����
	hdc=GetDC(hSlot);	//slot �������� dc�� �����´�

	if(hBit==NULL)		//������ ũ�⿡ �°� ��Ʈ�� ����
		hBit=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	hMemDC=CreateCompatibleDC(hdc);	//���� dc�� ���Ѵ�
	OldBit=(HBITMAP)SelectObject(hMemDC,hBit);

	FillRect(hMemDC,&crt,GetSysColorBrush(COLOR_WINDOW));	//hMemDC�� ���� �������� �Ѵ�

	sprintf(text,"%d",betlevel);
	TextOut(hMemDC,100,130,text,strlen(text));
	sprintf(text,"�� �� : %d",Players[0].Money-BetedMoney);
	TextOut(hMemDC,100,50,text,strlen(text));
	sprintf(text,"�ɸ� �� : %d",BetedMoney);
	TextOut(hMemDC,300,50,text,strlen(text));
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			Rectangle(hMemDC,225+j*100,212+i*100,325+j*100,312+i*100);
			DrawBitmap(hMemDC,240+j*100,215+i*100,PokerCards[(Board[i][j]/100)-1][(Board[i][j]%100)-1]);
		}
	}
	for(int i=0;i<8;i++){
		sprintf(text,"%d",betmoney[i]);
		TextOut(hMemDC,xs[i],ys[i],text,strlen(text));
	}

	TextOut(hMemDC,800,100,"����ǥ",strlen("����ǥ"));
	TextOut(hMemDC,800,130,"7  :  150��",strlen("7  :  150��"));
	TextOut(hMemDC,800,160,"A  :  80��",strlen("A  :  80��"));
	TextOut(hMemDC,800,190,"K  :  60��",strlen("K  :  60��"));
	TextOut(hMemDC,800,220,"Q  :  40��",strlen("Q  :  40��"));
	TextOut(hMemDC,800,250,"J  :  20��",strlen("J  :  20��"));
	TextOut(hMemDC,800,280,"10  :  4��",strlen("10  :  4��"));
	TextOut(hMemDC,800,310,"5   :  1��",strlen("5   :  1��"));
	TextOut(hMemDC,800,340,"��  :  4��",strlen("��  :  4��"));
	TextOut(hMemDC,800,370,"��  :  3��",strlen("��  :  3��"));
	TextOut(hMemDC,800,400,"��  :  2��",strlen("��  :  2��"));
	TextOut(hMemDC,800,430,"��  :  1��",strlen("��  :  1��"));
	TextOut(hMemDC,750,460,"*����� ���ڴ� �ߺ�����*",strlen("*����� ���ڴ� �ߺ�����*"));

	SelectObject(hMemDC,OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hSlot,hdc);
	InvalidateRect(hSlot,NULL,false);

}

void CalResult();

LRESULT CALLBACK SlotProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc,MemDC;
	PAINTSTRUCT ps;
	HBITMAP MyBitmap, OldBitmap;
	int xs[8]={150,150,150,250,350,450,150,550};
	int ys[8]={280,380,480,180,180,180,180,180};
	int pe[28]={101,105,107,110,111,112,113, 201,205,207,210,211,212,213, 301,305,307,310,311,312,313, 401,405,407,410,411,412,413};
	static HWND start;
	static HWND stop[3];
	static HWND bet[8];
	static HWND clear;
	static HWND level,level2;
	static HWND back;
	static bool selectcheck[3];
	static HANDLE BetTimer;
	char text[200];
	int CardImg;
	int cnt;
	RECT map={225,212,525,512};

	switch(iMessage) {
	case WM_CREATE:
		hSlot=hWnd;
		start=CreateWindow("button","����!",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,700,350,50,25,hWnd,(HMENU)ID_BT_START,g_hInst,NULL);
		stop[0]=CreateWindow("button","����1",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,250,600,50,25,hWnd,(HMENU)ID_BT_STOP1,g_hInst,NULL);
		stop[1]=CreateWindow("button","����2",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,350,600,50,25,hWnd,(HMENU)ID_BT_STOP2,g_hInst,NULL);
		stop[2]=CreateWindow("button","����3",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,450,600,50,25,hWnd,(HMENU)ID_BT_STOP3,g_hInst,NULL);
		bet[0]=CreateWindow("button","����1",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,150,250,50,25,hWnd,(HMENU)ID_BT_BET1,g_hInst,NULL);
		bet[1]=CreateWindow("button","����2",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,150,350,50,25,hWnd,(HMENU)ID_BT_BET2,g_hInst,NULL);
		bet[2]=CreateWindow("button","����3",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,150,450,50,25,hWnd,(HMENU)ID_BT_BET3,g_hInst,NULL);
		bet[3]=CreateWindow("button","����4",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,250,150,50,25,hWnd,(HMENU)ID_BT_BET4,g_hInst,NULL);
		bet[4]=CreateWindow("button","����5",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,350,150,50,25,hWnd,(HMENU)ID_BT_BET5,g_hInst,NULL);
		bet[5]=CreateWindow("button","����6",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,450,150,50,25,hWnd,(HMENU)ID_BT_BET6,g_hInst,NULL);
		bet[6]=CreateWindow("button","����7",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,150,150,50,25,hWnd,(HMENU)ID_BT_BET7,g_hInst,NULL);
		bet[7]=CreateWindow("button","����8",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,550,150,50,25,hWnd,(HMENU)ID_BT_BET8,g_hInst,NULL);
		clear=CreateWindow("button","���� ���",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,650,650,100,25,hWnd,(HMENU)ID_BT_CLEAR,g_hInst,NULL);
		level=CreateWindow("button","���� ���� ��",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,100,100,120,25,hWnd,(HMENU)ID_BT_LVUP,g_hInst,NULL);
		level2=CreateWindow("button","���� ���� �ٿ�",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,230,100,120,25,hWnd,(HMENU)ID_BT_LVDOWN,g_hInst,NULL);
		back=CreateWindow("button","���ư���",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,600,100,100,25,hWnd,(HMENU)ID_BT_BACK,g_hInst,NULL);
		betlevel=10;
		srand(time(NULL));
		Board[0][0]=207; Board[0][1]=207; Board[0][2]=207;
		Board[1][0]=407; Board[1][1]=407; Board[1][2]=407;
		Board[2][0]=307; Board[2][1]=307; Board[2][2]=307;
		OnTimer();

		for(int i=0;i<3;i++)
			EnableWindow(stop[i],false);
		SendMessage(hWnd,WM_COMMAND,ID_BT_CLEAR,0);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		DrawBitmap(hdc,0,0,hBit);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_BT_STOP1:
			selectcheck[0]=true;
			if(selectcheck[0] && selectcheck[1] && selectcheck[2]){
				SendMessage(hWnd,WM_CALSCORE,0,0);
			}
			EnableWindow(stop[0],false);
			break;
		case ID_BT_STOP2:
			selectcheck[1]=true;
			if(selectcheck[0] && selectcheck[1] && selectcheck[2]){
				SendMessage(hWnd,WM_CALSCORE,0,0);
			}
			EnableWindow(stop[1],false);
			break;
		case ID_BT_STOP3:
			selectcheck[2]=true;
			if(selectcheck[0] && selectcheck[1] && selectcheck[2]){
				SendMessage(hWnd,WM_CALSCORE,0,0);
			}
			EnableWindow(stop[2],false);
			break;

		case ID_BT_BET1:
			if(Players[0].Money < BetedMoney+betlevel){
				MessageBox(hWnd, "�������� �ʰ��ؼ� ������ �� �����ϴ�","���� ���ڶ��ϴ�!",MB_OK);
				return 0;
			}
			betmoney[0]+=betlevel;				
			BetedMoney+=betlevel;
			break;
		case ID_BT_BET2:
			if(Players[0].Money < BetedMoney+betlevel){
				MessageBox(hWnd, "�������� �ʰ��ؼ� ������ �� �����ϴ�","���� ���ڶ��ϴ�!",MB_OK);
				return 0;
			}
			betmoney[1]+=betlevel;			
			BetedMoney+=betlevel;
			break;
		case ID_BT_BET3:
			if(Players[0].Money < BetedMoney+betlevel){
				MessageBox(hWnd, "�������� �ʰ��ؼ� ������ �� �����ϴ�","���� ���ڶ��ϴ�!",MB_OK);
				return 0;
			}
			betmoney[2]+=betlevel;			
			BetedMoney+=betlevel;
			break;
		case ID_BT_BET4:
			if(Players[0].Money < BetedMoney+betlevel){
				MessageBox(hWnd, "�������� �ʰ��ؼ� ������ �� �����ϴ�","���� ���ڶ��ϴ�!",MB_OK);
				return 0;
			}
			betmoney[3]+=betlevel;			
			BetedMoney+=betlevel;
			break;
		case ID_BT_BET5:
			if(Players[0].Money < BetedMoney+betlevel){
				MessageBox(hWnd, "�������� �ʰ��ؼ� ������ �� �����ϴ�","���� ���ڶ��ϴ�!",MB_OK);
				return 0;
			}
			betmoney[4]+=betlevel;			
			BetedMoney+=betlevel;
			break;
		case ID_BT_BET6:
			if(Players[0].Money < BetedMoney+betlevel){
				MessageBox(hWnd, "�������� �ʰ��ؼ� ������ �� �����ϴ�","���� ���ڶ��ϴ�!",MB_OK);
				return 0;
			}
			betmoney[5]+=betlevel;			
			BetedMoney+=betlevel;
			break;
		case ID_BT_BET7:
			if(Players[0].Money < BetedMoney+betlevel){
				MessageBox(hWnd, "�������� �ʰ��ؼ� ������ �� �����ϴ�","���� ���ڶ��ϴ�!",MB_OK);
				return 0;
			}
			betmoney[6]+=betlevel;			
			BetedMoney+=betlevel;
			break;
		case ID_BT_BET8:
			if(Players[0].Money < BetedMoney+betlevel){
				MessageBox(hWnd, "�������� �ʰ��ؼ� ������ �� �����ϴ�","���� ���ڶ��ϴ�!",MB_OK);
				return 0;
			}
			betmoney[7]+=betlevel;			
			BetedMoney+=betlevel;
			break;
		case ID_BT_CLEAR://���þ� ���� �ʱ�ȭ
			//Players[0].Money+=BetedMoney;
			for(int i=0;i<8;i++)					
				betmoney[i]=0;

			for(int i=0;i<3;i++)
				selectcheck[i]=false;
			betlevel=10;
			BetedMoney=0;
			InvalidateRect(hWnd,NULL,true);

			break;
		case ID_BT_LVUP://���ô��� ����
			if(betlevel*10 <= 1000000000 && betlevel*10 <= Players[0].Money)
				betlevel*=10;			
			else
				MessageBox(hWnd, "������ �̻����� ���þ��� �ø� �� �����ϴ�!","�̰� �ƴ���!",MB_OK);			
			break;
		case ID_BT_LVDOWN:
			if(betlevel > 10)
				betlevel/=10;
			else
				MessageBox(hWnd, "10 �̸����� ���þ��� ���� �� �����ϴ�!","�̰� �ƴ���!",MB_OK);
			break;
		case ID_BT_START://���� ����
			cnt=0;
			for(int i=0;i<8;i++)
				if(betmoney[i]>0)
					cnt++;
			if(cnt==0){
				MessageBox(hWnd, "�� ���� �̻� ���� �żž� �մϴ�!","�̰� �ƴ���!",MB_OK);
				return 0;
			}
			BetTimer=(HANDLE)SetTimer(hWnd,1,25,NULL);
			for(int i=0;i<3;i++)
				EnableWindow(stop[i],true);
			for(int i=0;i<8;i++)
				EnableWindow(bet[i],false);
			EnableWindow(clear,false);
			EnableWindow(level,false);
			EnableWindow(level2,false);
			EnableWindow(start,false);
			EnableWindow(back,false);
			break;
		case ID_BT_BACK:	//�޴��� ���ư���
			ShowWindow(hSel,SW_SHOW);
			DestroyWindow(hWnd);
			break;
		}			
		OnTimer();
		InvalidateRect(hWnd,NULL,false);

		return 0;
	case WM_TIMER:	//ī�� �ٲٱ�. ���� ���۸��� �ʿ��ϴ�!

		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				if(!selectcheck[j]){
					Board[i][j]=pe[rand()%28];
				}
				else{
					Board[i][j]=Board[i][j];
				}
			}
		}
		OnTimer();
		return 0;
	case WM_CALSCORE:	//���� ���. �󸶳� �����°�?
		KillTimer(hWnd,1);
		CalResult();
		for(int i=0;i<8;i++)
			EnableWindow(bet[i],true);
		EnableWindow(clear,true);
		EnableWindow(level,true);
		EnableWindow(level2,true);
		EnableWindow(start,true);
		EnableWindow(back,true);
		for(int i=0;i<8;i++)					
			betmoney[i]=0;

		for(int i=0;i<3;i++)
			selectcheck[i]=false;

		if(Players[0].Money <= 0){
			if(MessageBox(hWnd, "���� ��� �����̽��ϴ�. �ٽ� ������ �����Ͻðڽ��ϱ�?","�й�..",MB_YESNO)==IDYES){
				DestroyWindow(hWnd);	//������ �ı�
				DestroyWindow(hSel);	//���õ� �ı�
				SendMessage(hMain,WM_COMMAND,101,0);

			}
			else{
				SendMessage(hWnd,WM_CLOSE,0,0);
			}
		}

		return 0;
	case WM_CLOSE:
		DestroyWindow(hMain);
		break;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

void CalResult(){
	int Check[8][3]={	//������
		{Board[0][0],Board[0][1],Board[0][2]},{Board[1][0],Board[1][1],Board[1][2]},{Board[2][0],Board[2][1],Board[2][2]},
		{Board[0][0],Board[1][0],Board[2][0]},{Board[0][1],Board[1][1],Board[2][1]},{Board[0][2],Board[1][2],Board[2][2]},
		{Board[0][0],Board[1][1],Board[2][2]},{Board[0][2],Board[1][1],Board[2][0]}
	};
	int Money=0;
	char text[500]={0,},out[200]={0,};
	bool get;
	int Score=0;/*
	FILE* fp=fopen("log-slot.txt","a");
	fprintf(fp,"���� ��: %d\n",Players[0].Money);
	fprintf(fp,"���� ��: %d\n",BetedMoney);*/


	for(int i=0;i<8;i++){
		Money=betmoney[i];
		if( betmoney[i]!=0){ //�� ���� �ɸ� ���� �ִٸ�
			out[0]=NULL;
			get=false;
			if(Check[i][0]/100 == Check[i][1]/100 && Check[i][1]/100 == Check[i][2]/100 ){	//������ ��ġ
				get=true;
				switch(Check[i][0]/100){
				case 4: Money*=5; sprintf(out,"�����̵� "); break;
				case 3: Money*=4; sprintf(out,"���̾� "); break;
				case 2: Money*=3; sprintf(out,"��Ʈ "); break;
				case 1: Money*=2; sprintf(out,"Ŭ�ι� "); break;
				}
			}
			if(Check[i][0]%100 == Check[i][1]%100 && Check[i][1]%100 == Check[i][2]%100 ){	//���ڰ� ��ġ
				get=true;
				switch(Check[i][0]%100){
				case 1: Money*=80; sprintf(&out[strlen(out)],"1 ");  break;
				case 7: Money*=150; sprintf(&out[strlen(out)],"7!!! ");  break;
				case 10: Money*=4; sprintf(&out[strlen(out)],"10 ");  break;
				case 11: Money*=20; sprintf(&out[strlen(out)],"J ");  break;
				case 12: Money*=40; sprintf(&out[strlen(out)],"Q ");  break;
				case 13: Money*=60; sprintf(&out[strlen(out)],"K ");  break;
				default : Money*=2; sprintf(&out[strlen(out)],"%d ",Check[i][0]%100);  break;
				}
			}

			if(get){//ȹ���ߴٸ�
				sprintf(&text[strlen(text)],"%d��° �� - ",i+1);
				sprintf(&out[strlen(out)],"��÷! %d ȹ��!!\n",Money);
				strcat(text,out);
				Score+=Money;
			}
		}

	}
	if(strlen(text)==0){
		MessageBox(hSlot, "��� ��....�Ф�","���",MB_OK);
	}
	else
		MessageBox(hSlot, text,"���",MB_OK);

	if(Score > Max_Score)
		Max_Score=Score;

	Players[0].Money+=Score;
	if(Players[0].Money >= Max_Money)
		Max_Money=Players[0].Money;

	Players[0].Money-=BetedMoney;
	BetedMoney=0;
/*
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			fprintf(fp,"%d ",Board[i][j]);
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"%s\n",text);
	fprintf(fp,"���� �� : %d\n",Players[0].Money);

	fclose(fp);*/
}

/*
���Ըӽ� ǥ

7 = 150��

A = 80��
ŷ = 60��
�� = 40��
�� = 20��
10 = 4��

�����̵� = 5��
���̾� = 4��
��Ʈ = 3��
Ŭ�ι� = 2��


�װ��� ����� 1,5,7,10,j,q,k ����

*/