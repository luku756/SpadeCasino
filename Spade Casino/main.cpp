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
LPSTR lpszClass="Spade Casino";	//���α׷��� �̸�

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	,LPSTR lpszCmdParam,int nCmdShow)
{
	FILE* fp=fopen("Updater.exe","rb");
	if(fp!=NULL){//������Ʈ ��Ȳ
		fclose(fp);
		DeleteFile("Updater.exe");
		return 0;
	}

	Opener=false;

	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;	//�� ��
	WndClass.cbWndExtra=0;	//�� ��
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);	//������ ��� ����
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);	//���콺 Ŀ����� ����
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);	//���α׷��� ������ ����
	WndClass.hInstance=hInstance;	//�� ���α׷��� �ν��Ͻ�(���ڷ� ����)
	WndClass.lpfnWndProc=(WNDPROC)WndProc;	//�� ���α׷��� ����� �޽���ó���Լ� ����
	WndClass.lpszClassName=lpszClass;	//���α׷��� �̸�
	WndClass.lpszMenuName=NULL;	//����� �޴��� ����
	WndClass.style=CS_HREDRAW | CS_VREDRAW;	//�� �������� ��Ÿ���� ����

	RegisterClass(&WndClass);	//������Ŭ���� ���

	WndClass.lpszClassName="Select Game";	//���α׷��� �̸�
	WndClass.lpfnWndProc=(WNDPROC)SelectProc;	//�� ���α׷��� ����� �޽���ó���Լ� ����
	RegisterClass(&WndClass);	//������Ŭ���� ���

	WndClass.lpszClassName="Seven Poker";	//���α׷��� �̸�
	WndClass.lpfnWndProc=(WNDPROC)SevenPokerProc;	//�� ���α׷��� ����� �޽���ó���Լ� ����
	RegisterClass(&WndClass);	//������Ŭ���� ���
	
	WndClass.lpszClassName="SevenPokerBetter";	//���α׷��� �̸�
	WndClass.lpfnWndProc=(WNDPROC)SevenPokerBetting;	//�� ���α׷��� ����� �޽���ó���Լ� ����
	RegisterClass(&WndClass);	//������Ŭ���� ���
	
	
	WndClass.lpszClassName="Holdem";	//���α׷��� �̸�
	WndClass.lpfnWndProc=(WNDPROC)HoldemProc;	//�� ���α׷��� ����� �޽���ó���Լ� ����
	RegisterClass(&WndClass);	//������Ŭ���� ���
	
	WndClass.lpszClassName="HoldemBetter";	//���α׷��� �̸�
	WndClass.lpfnWndProc=(WNDPROC)HoldemBetting;	//�� ���α׷��� ����� �޽���ó���Լ� ����
	RegisterClass(&WndClass);	//������Ŭ���� ���
	
	WndClass.lpszClassName="Slot Machine";	//���α׷��� �̸�
	WndClass.lpfnWndProc=(WNDPROC)SlotProc;	//�� ���α׷��� ����� �޽���ó���Լ� ����
	RegisterClass(&WndClass);	//������Ŭ���� ���

	WndClass.lpszClassName="Black Jack";	//���α׷��� �̸�
	WndClass.lpfnWndProc=(WNDPROC)BlackJackProc;	//�� ���α׷��� ����� �޽���ó���Լ� ����
	RegisterClass(&WndClass);

	WndClass.cbWndExtra=4;
	WndClass.lpszClassName="BlackJackDoing";	//���α׷��� �̸�
	WndClass.lpfnWndProc=(WNDPROC)BlackJackDoing;	//�� ���α׷��� ����� �޽���ó���Լ� ����
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,	
		CW_USEDEFAULT,CW_USEDEFAULT,550,400,
		NULL,(HMENU)NULL,hInstance,NULL);	//���� ������ ���� �Լ�
	hMain=hWnd;
	ShowWindow(hWnd,nCmdShow);	//�����츦 ȭ�鿡 ����ϴ� �Լ�

	//�޽��� ����
	while(GetMessage(&Message,0,0,0)) {	//�޽��� ť���� �޽����� �����´�. WM_QUIT �϶� false ����
		TranslateMessage(&Message);	//�޽����� �����Ѵ�
		DispatchMessage(&Message);	//WndProc ���� �޽����� ����
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
		CreateWindow("button","���� ����",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,	100,140,100,25,hWnd,(HMENU)ID_BT_NEWGAME,g_hInst,NULL);
		CreateWindow("button","�̾� �ϱ�",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,	260,140,100,25,hWnd,(HMENU)ID_BT_LOADGAME,g_hInst,NULL);

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

			ShowWindow(hWnd,SW_SHOW);	//�����츦 ȭ�鿡 ����ϴ� �Լ�
			//���̺굥���� �ε�

			break;
		case ID_BT_NEWGAME:
			if (DialogBox(g_hInst,MAKEINTRESOURCE(IDD_MAKENAME),	hWnd,MakeNameDlgProc)==1) {
				
				//���� ���� ��ȯ
				Opener=true;
				memcpy(Players,PlayerDefault,sizeof(PLAYER)*10);
				Max_Money=100000;
				Max_Score=0;

				ShowWindow(hWnd,SW_HIDE);
				hWnd=CreateWindow("Select Game","Spade Casino", WS_OVERLAPPEDWINDOW ,100,100,800,600,NULL,(HMENU)NULL,g_hInst,NULL);

				ShowWindow(hWnd,SW_SHOW);	//�����츦 ȭ�鿡 ����ϴ� �Լ�
			}
			break;
		}			

		return 0;
	case WM_DESTROY:
		FreeCardGraphic();
		if(Opener)//�����Ͱ� �־�߸� �����ϱ�
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
		CreateWindow("button","������Ŀ",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,100,140,100,25,hWnd,(HMENU)ID_BT_SEVEN,g_hInst,NULL);
		CreateWindow("button","�ػ罺 Ȧ��",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,260,140,100,25,hWnd,(HMENU)ID_BT_HOLDEM,g_hInst,NULL);
		CreateWindow("button","����",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,420,140,100,25,hWnd,(HMENU)ID_BT_BLACKJAC,g_hInst,NULL);
		CreateWindow("button","���Ըӽ�",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,580,140,100,25,hWnd,(HMENU)ID_BT_SLOT,g_hInst,NULL);
		CreateWindow("button","��� ����",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,300,200,100,25,hWnd,(HMENU)ID_BT_REPROT,g_hInst,NULL);
		CreateWindow("button","���ư���",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,300,500,100,25,hWnd,(HMENU)ID_BT_BACK,g_hInst,NULL);

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
			if(cnt < 4){//�÷��̾� �� ����
				MessageBox(hWnd,"���� �÷��̾ 4�� �̸��Դϴ�. �ٸ� ������ �̿����ּ���.","�ο��� ����",MB_OK);
				return 0;
			}
		
			ShowWindow(hWnd,SW_HIDE);
			hWnd=CreateWindow("Seven Poker","Spade Casino", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,1000,800,NULL,(HMENU)NULL,g_hInst,NULL);

			ShowWindow(hWnd,SW_SHOW);	//�����츦 ȭ�鿡 ����ϴ� �Լ�
			break;
		case ID_BT_HOLDEM:
			if (DialogBox(g_hInst,MAKEINTRESOURCE(IDD_SelHoldemPlayer),	hWnd,SelPlayerDlgProc)==1) {

				ShowWindow(hWnd,SW_HIDE);
				hWnd=CreateWindow("Holdem","Spade Casino", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,1000,800,NULL,(HMENU)NULL,g_hInst,NULL);

				ShowWindow(hWnd,SW_SHOW);	//�����츦 ȭ�鿡 ����ϴ� �Լ�
			}
			break;
		case ID_BT_BLACKJAC:
			if (DialogBox(g_hInst,MAKEINTRESOURCE(IDD_SelBlackJackPlayer),	hWnd,SelBlackPlayerDlgProc)==1) {
				ShowWindow(hWnd,SW_HIDE);
				hWnd=CreateWindow("Black Jack","Spade Casino", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,1100,800,NULL,(HMENU)NULL,g_hInst,NULL);

				ShowWindow(hWnd,SW_SHOW);	//�����츦 ȭ�鿡 ����ϴ� �Լ�
			}
			break;
		case ID_BT_SLOT:
			ShowWindow(hWnd,SW_HIDE);
			hWnd=CreateWindow("Slot Machine","Spade Casino", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,CW_USEDEFAULT,CW_USEDEFAULT,1000,800,NULL,(HMENU)NULL,g_hInst,NULL);

			ShowWindow(hWnd,SW_SHOW);	//�����츦 ȭ�鿡 ����ϴ� �Լ�
			break;
		
		case ID_BT_REPROT:
			sprintf(text,"%s\n�� �ں� : %d\n\n�ְ� �ҵ� : %d\n�ְ� �ں� : %d\n�Ļ��Ų �÷��̾� ��: %d",Players[0].Name,Players[0].Money,Max_Score,Max_Money,Down);
			MessageBox(hWnd,text,"���",NULL);
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
