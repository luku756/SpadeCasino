#include <windows.h>
#include "Player.h"
#include <stdio.h>
#include "SevenPoker.h"
#include "Holdem.h"
#include "Slot.h"
#include "BlackJack.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK SelectProc(HWND,UINT,WPARAM,LPARAM);

BOOL CALLBACK MakeNameDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
LPSTR lpszClass="Spade Casino";	//프로그램의 이름

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	,LPSTR lpszCmdParam,int nCmdShow)
{
	FILE* fp=fopen("Updater.exe","rb");
	if(fp!=NULL){//업데이트 상황
		fclose(fp);
		DeleteFile("Updater.exe");
		return 0;
	}

	Opener=false;

	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;	//빈 값
	WndClass.cbWndExtra=0;	//빈 값
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);	//윈도우 배경 색상
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);	//마우스 커서모양 지정
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);	//프로그램의 아이콘 지정
	WndClass.hInstance=hInstance;	//이 프로그램의 인스턴스(인자로 들어옴)
	WndClass.lpfnWndProc=(WNDPROC)WndProc;	//이 프로그램이 사용할 메시지처리함수 지정
	WndClass.lpszClassName=lpszClass;	//프로그램의 이름
	WndClass.lpszMenuName=NULL;	//사용할 메뉴를 지정
	WndClass.style=CS_HREDRAW | CS_VREDRAW;	//이 윈도우의 스타일을 정의

	RegisterClass(&WndClass);	//윈도우클래스 등록

	WndClass.lpszClassName="Select Game";	//프로그램의 이름
	WndClass.lpfnWndProc=(WNDPROC)SelectProc;	//이 프로그램이 사용할 메시지처리함수 지정
	RegisterClass(&WndClass);	//윈도우클래스 등록

	WndClass.lpszClassName="Seven Poker";	//프로그램의 이름
	WndClass.lpfnWndProc=(WNDPROC)SevenPokerProc;	//이 프로그램이 사용할 메시지처리함수 지정
	RegisterClass(&WndClass);	//윈도우클래스 등록
	
	WndClass.lpszClassName="SevenPokerBetter";	//프로그램의 이름
	WndClass.lpfnWndProc=(WNDPROC)SevenPokerBetting;	//이 프로그램이 사용할 메시지처리함수 지정
	RegisterClass(&WndClass);	//윈도우클래스 등록
	
	
	WndClass.lpszClassName="Holdem";	//프로그램의 이름
	WndClass.lpfnWndProc=(WNDPROC)HoldemProc;	//이 프로그램이 사용할 메시지처리함수 지정
	RegisterClass(&WndClass);	//윈도우클래스 등록
	
	WndClass.lpszClassName="HoldemBetter";	//프로그램의 이름
	WndClass.lpfnWndProc=(WNDPROC)HoldemBetting;	//이 프로그램이 사용할 메시지처리함수 지정
	RegisterClass(&WndClass);	//윈도우클래스 등록
	
	WndClass.lpszClassName="Slot Machine";	//프로그램의 이름
	WndClass.lpfnWndProc=(WNDPROC)SlotProc;	//이 프로그램이 사용할 메시지처리함수 지정
	RegisterClass(&WndClass);	//윈도우클래스 등록

	WndClass.lpszClassName="Black Jack";	//프로그램의 이름
	WndClass.lpfnWndProc=(WNDPROC)BlackJackProc;	//이 프로그램이 사용할 메시지처리함수 지정
	RegisterClass(&WndClass);

	WndClass.cbWndExtra=4;
	WndClass.lpszClassName="BlackJackDoing";	//프로그램의 이름
	WndClass.lpfnWndProc=(WNDPROC)BlackJackDoing;	//이 프로그램이 사용할 메시지처리함수 지정
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,	
		CW_USEDEFAULT,CW_USEDEFAULT,550,400,
		NULL,(HMENU)NULL,hInstance,NULL);	//실제 윈도우 생성 함수
	hMain=hWnd;
	ShowWindow(hWnd,nCmdShow);	//윈도우를 화면에 출력하는 함수

	//메시지 루프
	while(GetMessage(&Message,0,0,0)) {	//메시지 큐에서 메시지를 가져온다. WM_QUIT 일때 false 리턴
		TranslateMessage(&Message);	//메시지를 가공한다
		DispatchMessage(&Message);	//WndProc 으로 메시지를 전달
	}
	return Message.wParam;
}

#define ID_BT_NEWGAME 101
#define ID_BT_LOADGAME 102


LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch(iMessage) {
	case WM_CREATE:
		GetCardGraphic();
		CreateWindow("button","새로 시작",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,	100,140,100,25,hWnd,(HMENU)ID_BT_NEWGAME,g_hInst,NULL);
		CreateWindow("button","이어 하기",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,	260,140,100,25,hWnd,(HMENU)ID_BT_LOADGAME,g_hInst,NULL);

		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case ID_BT_LOADGAME:
			LoadData();
			Opener=true;
			ShowWindow(hWnd,SW_HIDE);
			hWnd=CreateWindow("Select Game","Spade Casino", WS_OVERLAPPEDWINDOW ,100,100,800,600,NULL,(HMENU)NULL,g_hInst,NULL);

			ShowWindow(hWnd,SW_SHOW);	//윈도우를 화면에 출력하는 함수
			//세이브데이터 로드

			break;
		case ID_BT_NEWGAME:
			if (DialogBox(g_hInst,MAKEINTRESOURCE(IDD_MAKENAME),	hWnd,MakeNameDlgProc)==1) {
				
				//게임 모드로 전환
				Opener=true;
				memcpy(Players,PlayerDefault,sizeof(PLAYER)*10);
				Max_Money=100000;
				Max_Score=0;

				ShowWindow(hWnd,SW_HIDE);
				hWnd=CreateWindow("Select Game","Spade Casino", WS_OVERLAPPEDWINDOW ,100,100,800,600,NULL,(HMENU)NULL,g_hInst,NULL);

				ShowWindow(hWnd,SW_SHOW);	//윈도우를 화면에 출력하는 함수
			}
			break;
		}			

		return 0;
	case WM_DESTROY:
		FreeCardGraphic();
		if(Opener)//데이터가 있어야만 저장하기
			SaveData();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

#define ID_BT_SEVEN 201
#define ID_BT_HOLDEM 202
#define ID_BT_BLACKJAC 203
#define ID_BT_SLOT 204

#define ID_BT_REPROT 205
#define ID_BT_BACK 206
#define ID_BT_HELP 20

LRESULT CALLBACK SelectProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	char text[200];
	int cnt;
	switch(iMessage) {
	case WM_CREATE:
		CreateWindow("button","세븐포커",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,100,140,100,25,hWnd,(HMENU)ID_BT_SEVEN,g_hInst,NULL);
		CreateWindow("button","텍사스 홀덤",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,260,140,100,25,hWnd,(HMENU)ID_BT_HOLDEM,g_hInst,NULL);
		CreateWindow("button","블랙잭",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,420,140,100,25,hWnd,(HMENU)ID_BT_BLACKJAC,g_hInst,NULL);
		CreateWindow("button","슬롯머신",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,580,140,100,25,hWnd,(HMENU)ID_BT_SLOT,g_hInst,NULL);
		CreateWindow("button","기록 보기",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,300,200,100,25,hWnd,(HMENU)ID_BT_REPROT,g_hInst,NULL);
		CreateWindow("button","돌아가기",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,300,500,100,25,hWnd,(HMENU)ID_BT_BACK,g_hInst,NULL);

		hSel=hWnd;

		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		if(Mode==1)
			TextOut(hdc,20,20,"Unlimeted",9);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case ID_BT_SEVEN:
			cnt=0;
			for(int i=0;i<10;i++){
				if(Players[i].Money > 0)
					cnt++;
			}
			if(cnt < 4){//플레이어 수 부족
				MessageBox(hWnd,"남은 플레이어가 4명 미만입니다. 다른 게임을 이용해주세요.","인원수 부족",MB_OK);
				return 0;
			}
		
			ShowWindow(hWnd,SW_HIDE);
			hWnd=CreateWindow("Seven Poker","Spade Casino", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,1000,800,NULL,(HMENU)NULL,g_hInst,NULL);

			ShowWindow(hWnd,SW_SHOW);	//윈도우를 화면에 출력하는 함수
			break;
		case ID_BT_HOLDEM:
			if (DialogBox(g_hInst,MAKEINTRESOURCE(IDD_SelHoldemPlayer),	hWnd,SelPlayerDlgProc)==1) {

				ShowWindow(hWnd,SW_HIDE);
				hWnd=CreateWindow("Holdem","Spade Casino", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,1000,800,NULL,(HMENU)NULL,g_hInst,NULL);

				ShowWindow(hWnd,SW_SHOW);	//윈도우를 화면에 출력하는 함수
			}
			break;
		case ID_BT_BLACKJAC:
			if (DialogBox(g_hInst,MAKEINTRESOURCE(IDD_SelBlackJackPlayer),	hWnd,SelBlackPlayerDlgProc)==1) {
				ShowWindow(hWnd,SW_HIDE);
				hWnd=CreateWindow("Black Jack","Spade Casino", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,1100,800,NULL,(HMENU)NULL,g_hInst,NULL);

				ShowWindow(hWnd,SW_SHOW);	//윈도우를 화면에 출력하는 함수
			}
			break;
		case ID_BT_SLOT:
			ShowWindow(hWnd,SW_HIDE);
			hWnd=CreateWindow("Slot Machine","Spade Casino", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,CW_USEDEFAULT,CW_USEDEFAULT,1000,800,NULL,(HMENU)NULL,g_hInst,NULL);

			ShowWindow(hWnd,SW_SHOW);	//윈도우를 화면에 출력하는 함수
			break;
		
		case ID_BT_REPROT:
			sprintf(text,"%s\n내 자본 : %d\n\n최고 소득 : %d\n최고 자본 : %d\n파산시킨 플레이어 수: %d",Players[0].Name,Players[0].Money,Max_Score,Max_Money,Down);
			MessageBox(hWnd,text,"기록",NULL);
			break;
		case ID_BT_BACK:
			ShowWindow(hMain,SW_SHOW);
			DestroyWindow(hWnd);
			break;
					
		}
		return 0;
	case WM_DESTROY:
		return 0;
	case WM_CLOSE:
			DestroyWindow(hMain);
		break;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}



BOOL CALLBACK MakeNameDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	static int PlayerCnt;
	char text[100];
	switch(iMessage) {
	case WM_INITDIALOG:
		
			EnableWindow(GetDlgItem( hDlg , IDOK),false);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))  {
		case IDC_GETNAME:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:

				GetDlgItemText(hDlg,IDC_GETNAME,text, 100 );
				if(strlen(text)>0){

					EnableWindow(GetDlgItem( hDlg , IDOK),true);
				}
				if(strlen(text)==0 || strlen(text) > 20){

					EnableWindow(GetDlgItem( hDlg , IDOK),false);
				}
				break;
			}
			return true;
		case IDOK:
			GetDlgItemText(hDlg,IDC_GETNAME,PlayerDefault[0].Name, 22 );
			PlayerDefault[0].Name[20]=0;
			EndDialog(hDlg,1);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg,0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}
