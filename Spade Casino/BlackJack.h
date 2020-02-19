#pragma once

#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "Poker.h"
#include "resource.h"

/*
ī�尡 10���̸� �ƽ��з� ����� ���� ģ��(1.5�� ����)
�ִϸ��̼��߿� ������ ��ŵ

*/

typedef struct _Cards{	//�÷��̾���� ī�带 �����ϴ� ����ü.
	int Cards[3][10];//ī����� ��ȣ. �ִ� 2������ ���ø������ϴ�.
	int Score[3];	//ī�幭������ ������
	int CardSize[3];	//�й������� ī���
	int PairSize;	//��� �й����� �����°�?
}JACKCARD;

void DrawBlackJack();
LRESULT CALLBACK BlackJackDoing(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);

//�� �÷��̾��� x��ǥ
int xs_b[4]={400,400,50,800};
//�� �������� y��ǥ
int ys_b[3]={650,450,250};
int yp_b[4]={-560,0,0,0};

int PlayerSize;
HBITMAP hBitBLACK;
int GameMode; /*0-ī�� ��� 1-��� ��(������ ����) 2-���� ī�� ���� ���� 3-���
			  0-ī�� ���
			  11-�÷��̾� 1 �ൿ ()

			  */
bool ani;//�ִϸ��̼� ����
int anix,aniy,anip[8],anis,anic,anit,anipair[8];//�ִϸ��̼� x,y, �� ������, size,ī����, Ƚ��,��� ��ȣ

char Doing[4][3][300];
bool Next;

bool BetTime[3];//������ �������� �˷��ش�.
bool CanHit[3];//hit �Ҽ� �ִ��� �˷��ش�.
bool BlindDealer;
int NowPlayer;

#define WM_GAMETURN 100001
#define WM_HIT 100002
#define WM_CHECKDOING 100003
#define WM_CALACCOUNT 100004
#define ID_BETEDIT 101
#define ID_BETBUTTON 102


class BLACKJACK{
public:
	int Users[4];//Users[0]-����, 1- ����
	JACKCARD Cards[4];
	int CardTable[208];	//4���� ī�� ���
	int CardNum;
	int Betting[3];//���õ� ����.
	int Insuarance[3];	//�����

	void SelectUsers(){
		bool check;
		for(int i=1;i<PlayerSize;i++){
			check=true;

			while(check){
				Users[i]=rand()%9+1;
				if(Players[Users[i]].Money <= 0)
					continue;
				check=false;
				for(int j=0;j<i;j++){
					if(Users[i]==Users[j]){
						check=true;
						break;
					}
				}
			}

		}
		Users[0]=Users[1];//������ 0��°
		Users[1]=0;
	}

	void ChangeUser(int PlayerNum){
		char text[200];

		sprintf(text,"%s �� �Ļ��߽��ϴ�.\n",Players[Users[PlayerNum]].Name);
		bool check=true;
		while(check){
			check=false;
			Users[PlayerNum]=rand()%9+1;
			if(Players[Users[PlayerNum]].Money <= 0){
				check=true;
				continue;
			}

			for(int i=1;i<Player_Size;i++){
				if(Users[i] == Users[PlayerNum] && i!=PlayerNum){
					check=true;
					break;
				}
			}


		}

		sprintf(&text[strlen(text)],"%s�� �����մϴ�.",Players[Users[PlayerNum]].Name);
		MessageBox(nGame, text,"�Ļ�!",MB_OK);
	}

	void OutCheck(){
		//�ƿ��� �÷��̾�, �Ļ�� �÷��̾ �ִ��� Ȯ���Ѵ�.
		int cnt=0;
		for(int i=1;i<Player_Size;i++){
			if(Players[Users[i]].Money <= 0){
				Down++;
				if(Mode==1){//���Ѹ��� ����
					Players[Users[i]].Money=10000000;
				}
			}
		}

		if(Mode ==0){//�Ϲݸ��� ��ü Ȥ�� ������ �ٿ�
			for(int i=0;i<10;i++){
				if(Players[i].Money > 0) //������ ��
					cnt++;
			}

			if(cnt==1){///���� ��, ���Ѹ�� ����
				MessageBox(nGame,"�� �÷��̾���� ��� �Ļ���׽��ϴ�! ���Ѹ��� �����մϴ�!","�¸�!",MB_OK);
				InitUnlimited();
				SelectUsers();
				return ;
			}

			if(cnt < PlayerSize){//�� ���� ������� ���� ����
				PlayerSize=cnt;
				MessageBox(nGame, "�÷��̾� ���� �����մϴ�. �ο� ���� �����մϴ�.","��� ����",MB_OK);
				SelectUsers();
			}
			else{//���� ����
				for(int i=0;i<PlayerSize;i++){
					if(i==1)
						continue;
					if(Players[Users[i]].Money <= 0){
						ChangeUser(i);
					}
				}

			}

		}


	}

	void MakeCardBox(){	//������ ������ ī�带 �״´�.
		int CardView[52];
		memset(CardView,0,sizeof(int)*52);
		//	for(int i=0;i<52;i++)
		//		CardView[i]=0;
		srand(time(NULL));

		for(int i=0;i<208;i++){
			do{
				CardTable[i]=rand()%52+1;
			}while(CardView[CardTable[i]-1]>=4);
			CardView[CardTable[i]-1]++;
		}

		for(int i=0;i<208;i++)
			CardTable[i]=100*(((CardTable[i]-1)/13)+1)+((CardTable[i]-1)%13+1);

	}

	void GameStart(){
		MakeCardBox();
/*
		CardTable[0]=101;
		CardTable[4]=212;
		CardTable[1]=101;
		CardTable[2]=101;
		CardTable[5]=201;
		CardTable[6]=201;
		CardTable[8]=301;
		CardTable[9]=401;
		CardTable[10]=401;*/

		/*
		CardTable[0]=101;
		CardTable[4]=201;
		CardTable[8]=301;
		CardTable[9]=401;
		CardTable[10]=402;
		CardTable[11]=302;
		CardTable[12]=303;
		CardTable[13]=303;
		CardTable[14]=302;
		CardTable[15]=202;
		CardTable[16]=213;
		CardTable[17]=212;
		CardTable[18]=202;
		CardTable[19]=102;
		CardTable[20]=201;
		CardTable[21]=302;
		CardTable[22]=301;
		CardTable[23]=102;*/



		SelectUsers();
		CardNum=0;
		for(int i=1;i<PlayerSize;i++){
			Betting[i-1]=0;
			Insuarance[i-1]=0;
			Cards[i].PairSize=0;
			Cards[i].CardSize[0]=0;
		}
		Cards[0].PairSize=0;
		Cards[0].CardSize[0]=0;
		DealCard();
	}

	void MakePair(int PlayerNum){
		//�ش� �÷��̾�� �� �й����� �ش�
		//�й����� ���� �ø��� �þ �й����� ����� 0���� �ʱ�ȭ.
		Cards[PlayerNum].CardSize[Cards[PlayerNum].PairSize++]=0;
	}

	void GiveCard(int PlayerNum, int PairNum){
		//�ش� �÷��̾��� �й����� ī�带 �ش�.
		Cards[PlayerNum].Cards[PairNum][Cards[PlayerNum].CardSize[PairNum]++]=CardTable[CardNum++];
	}

	void DealCard(){//ó���� �и� �����ִ� ��.
		int t;
		for(int i=0;i<PlayerSize;i++)
			MakePair(i);
		ani=true;
		anix=-100;
		aniy=-100;
		anit=0;
		anic=0;
		anis=PlayerSize*2;
		switch(PlayerSize){
		case 2:
			anip[0]=1; anip[1]=0;anip[2]=1;anip[3]=0;
			anipair[0]=0; anipair[1]=0;anipair[2]=0;anipair[3]=0;
			break;
		case 3:
			anip[0]=2; anip[1]=1; anip[2]=0; anip[3]=2; anip[4]=1; anip[5]=0; 
			anipair[0]=0; anipair[1]=0;anipair[2]=0;anipair[3]=0;anipair[4]=0;anipair[5]=0;
			break;
		case 4:
			anip[0]=2; anip[1]=1; anip[2]=3; anip[3]=0; anip[4]=2; anip[5]=1; anip[6]=3; anip[7]=0; 
			anipair[0]=0; anipair[1]=0;anipair[2]=0;anipair[3]=0;anipair[4]=0;anipair[5]=0;anipair[6]=0;anipair[7]=0;
			break;
		}

		SetTimer(nGame,2,50,0);
	}

	void CalScore(){
		int Ace=0,score;
		for(int i=0;i<PlayerSize;i++){
			for(int j=0;j<Cards[i].PairSize;j++){
				score=0;
				Ace=0;
				for(int k=0;k<Cards[i].CardSize[j];k++){
					if(Cards[i].Cards[j][k]%100 < 10)	//1~9�� �׸�ŭ�� ����
						score += Cards[i].Cards[j][k]%100;
					else//10�� ���ڴ� 10�� ����
						score += 10;

					if(Cards[i].Cards[j][k]%100 == 1)//���̽� ���� ���
						Ace++;
				}
				while(score <= 11 && Ace > 0){
					score += 10;
					Ace--;
				}
				Cards[i].Score[j]=score;
			}

		}

	}

	void PlayNPC(int PlayerNum){//NPC�� �ڵ����� �����Ѵ�.
		for(int i=0;i<Cards[PlayerNum].PairSize;i++){
			if(Cards[PlayerNum].PairSize < 3 && Cards[PlayerNum].Cards[i][0] % 100 == Cards[PlayerNum].Cards[i][1] % 100){
				Split(PlayerNum,i);
				return ;
			}
		}

		int score;
		int ace;
		for(int i=0;i<Cards[PlayerNum].PairSize;i++){
			score=0;
			ace=0;
			for(int j=0;j<Cards[PlayerNum].CardSize[i];j++){
				if(Cards[PlayerNum].Cards[i][j] % 100 < 10)
					score += Cards[PlayerNum].Cards[i][j] % 100;
				else
					score += 10;
				if(Cards[PlayerNum].Cards[i][j] % 100 == 1)
					ace++;
			}
			if(ace>0 && score <= 11)
				score+=10;
			if(score < 15){

				ani=true;
				anix=-100;
				aniy=-100;
				anit=0;
				anic=0;
				anis=1;
				anip[0]=PlayerNum;
				anipair[0]=i;
				strcpy(Doing[PlayerNum][i],"hit");
				SetTimer(nGame,2,50,0);
				return ;
			}
			else if(score > 21){

				strcpy(Doing[PlayerNum][i],"Bust");
			}
			else if(score == 21 &&  Cards[PlayerNum].PairSize == 1 && Cards[PlayerNum].CardSize[0]==2){
				strcpy(Doing[PlayerNum][i],"BlackJack");
			}
			else
				strcpy(Doing[PlayerNum][i],"Stay");
			DrawBlackJack();
		}

		Next=true;
	}

	void PlayDealer(){
		CalScore();
		if(Cards[0].Score[0] < 17){

			ani=true;
			anix=-100;
			aniy=-100;
			anit=0;
			anic=0;
			anis=1;
			anip[0]=0;
			anipair[0]=0;
			strcpy(Doing[0][0],"hit");
			SetTimer(nGame,2,50,0);
			return ;

		}
		else if(Cards[0].Score[0] == 21 && Cards[0].CardSize[0] == 2){	
			strcpy(Doing[0][0],"BlackJack");
			SendMessage(nGame,WM_CALACCOUNT,0,0);
			return ;
		}
		else if(Cards[0].Score[0] <= 21){
			strcpy(Doing[0][0],"Stay");
			SendMessage(nGame,WM_CALACCOUNT,0,0);

		}
		else{
			strcpy(Doing[0][0],"Bust");
			SendMessage(nGame,WM_CALACCOUNT,0,0);
			return ;
		}

	}

	void Split(int PlayerNum, int PairNum){//ī�带 ������.
		if(Cards[PlayerNum].PairSize==1){
			MakePair(PlayerNum);
			Cards[PlayerNum].CardSize[0]--;
			Cards[PlayerNum].CardSize[Cards[PlayerNum].PairSize-1]=1;
			Cards[PlayerNum].Cards[1][0]=Cards[PlayerNum].Cards[0][1];

			ani=true;
			anix=-100;
			aniy=-100;
			anit=0;
			anic=0;
			anis=2;
			anip[0]=PlayerNum;
			anip[1]=PlayerNum;
			anipair[0]=0;
			anipair[1]=Cards[PlayerNum].PairSize-1;
			Betting[PlayerNum-1]*=2;
		}
		else{

			MakePair(PlayerNum);

			Cards[PlayerNum].CardSize[PairNum]--;
			Cards[PlayerNum].CardSize[2]=1;
			Cards[PlayerNum].Cards[2][0]=Cards[PlayerNum].Cards[PairNum][1];
			anipair[0]=PairNum;

			ani=true;
			anix=-100;
			aniy=-100;
			anit=0;
			anic=0;
			anis=2;
			anip[0]=PlayerNum;
			anip[1]=PlayerNum;
			anipair[1]=2;
			Betting[PlayerNum-1]*=1.5;
		}
		SetTimer(nGame,2,50,0);

	}


};
BLACKJACK GameB;
HWND hBlackJackDoing;
int MakedDoer;

void DrawBlackJack(){

	RECT crt;
	HDC hdc,hMemDC;
	HBITMAP OldBit;
	int CardImg,x,y;
	char text[200];

	GetClientRect(nGame,&crt);	//slot�������� ũ�⿡ �°� �簢�� ����
	hdc=GetDC(nGame);	//slot �������� dc�� �����´�

	if(hBitBLACK==NULL)		//������ ũ�⿡ �°� ��Ʈ�� ����
		hBitBLACK=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	hMemDC=CreateCompatibleDC(hdc);	//���� dc�� ���Ѵ�
	OldBit=(HBITMAP)SelectObject(hMemDC,hBitBLACK);

	FillRect(hMemDC,&crt,GetSysColorBrush(COLOR_WINDOW));	//hMemDC�� ���� �������� �Ѵ�

	for(int i=0;i<PlayerSize;i++){

		for(int j=0;j<GameB.Cards[i].PairSize;j++){

			for(int k=0;k<GameB.Cards[i].CardSize[j];k++){

				if(k<5){
					x=xs_b[i]+k*60;
					y=ys_b[j]+yp_b[i];
				}
				else{

					x=xs_b[i]+(k-5)*60;
					y=ys_b[j]+yp_b[i]-85;
				}

				if(BlindDealer && i==0 && k ==1)
					DrawSmallBitmap(hMemDC,x,y,0.9,CardBack);
				else
					DrawSmallBitmap(hMemDC,x,y,0.9,PokerCards[(GameB.Cards[i].Cards[j][k]/100)-1][(GameB.Cards[i].Cards[j][k]%100)-1]);
	/*			sprintf(text,"%d",GameB.Cards[i].Cards[j][k]);
				TextOut(hMemDC,x,y,text,strlen(text));*/

			}
/*
			sprintf(text,"%d",GameB.Cards[i].Score[j]);
			TextOut(hMemDC,xs_b[i]-40,ys_b[j]+yp_b[i]-40,text,strlen(text));*/
			TextOut(hMemDC,xs_b[i]-40,ys_b[j]+yp_b[i]-70,Doing[i][j],strlen(Doing[i][j]));
		}
		if(i>0){
			sprintf(text,"Bet : %d",GameB.Betting[i-1]);
			TextOut(hMemDC,xs_b[i]+90,535+yp_b[i],text,strlen(text));

		}
		if(NowPlayer == 1 && GameB.Betting[0]==0){
			sprintf(text,"�������ּ���. %d ~ %d",Players[0].Money/1000,Players[0].Money/10);
			TextOut(hMemDC,550,540,text,strlen(text));
		}
		if(i == 0)
			sprintf(text,"���� %s",Players[GameB.Users[i]].Name);

		else
			sprintf(text,"%s",Players[GameB.Users[i]].Name);
		TextOut(hMemDC,xs_b[i]-(!i*130),535+yp_b[i]+(!i*100),text,strlen(text));

		sprintf(text,"money : %d",Players[GameB.Users[i]].Money);
		TextOut(hMemDC,xs_b[i]-(!i*130),550+yp_b[i]+(!i*100),text,strlen(text));
	}

	DrawSmallBitmap(hMemDC,140,50,0.9,CardBack);

	if(ani){//�ִϸ��̼� ���¶��
		DrawSmallBitmap(hMemDC,anix,aniy,0.9,CardBack);
	}/*
	sprintf(text,"%d",NowPlayer);
	TextOut(hMemDC,10,10,text,strlen(text));*/

	SelectObject(hMemDC,OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(nGame,hdc);
	InvalidateRect(nGame,NULL,false);

}

HWND Doer[3];
LRESULT CALLBACK BlackJackProc(HWND hWnd,UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HWND hEdit,hBt;

	char text[200];
	static HANDLE BetTimer;
	static HWND betwnd;
	static bool PlayerTurn;
	static bool GameFin;
	int call,x,y;
	int win,money,sc;
	bool check;

	switch(iMessage) {
	case WM_CREATE:
		hEdit=CreateWindow("edit",NULL,WS_CHILD| ES_NUMBER | WS_BORDER | ES_AUTOHSCROLL,550,600,100,25,hWnd,(HMENU)ID_BETEDIT,g_hInst,NULL);
		hBt=CreateWindow("button","����",WS_CHILD | BS_PUSHBUTTON,650,600,50,25,hWnd,(HMENU)ID_BETBUTTON,g_hInst,NULL);

		GameFin=false;
		BlindDealer=true;
		Next=false;
		nGame=hWnd;
		GameMode=0;

		if(PlayerSize == 2)
			NowPlayer=1;
		else
			NowPlayer=2;

		Doing[0][0][0]=0;
		Doing[1][0][0]=0;
		Doing[1][1][0]=0;
		Doing[1][2][0]=0;
		Doing[2][0][0]=0;
		Doing[2][1][0]=0;
		Doing[2][2][0]=0;
		Doing[3][0][0]=0;
		Doing[3][1][0]=0;
		Doing[3][2][0]=0;


		GameB.GameStart();
		DrawBlackJack();
		PlayerTurn=false;
		SetTimer(hWnd,1,100,0);
		return 0;
	case WM_REGAME:
		
		Doing[0][0][0]=0;
		Doing[1][0][0]=0;
		Doing[1][1][0]=0;
		Doing[1][2][0]=0;
		Doing[2][0][0]=0;
		Doing[2][1][0]=0;
		Doing[2][2][0]=0;
		Doing[3][0][0]=0;
		Doing[3][1][0]=0;
		Doing[3][2][0]=0;

		GameFin=false;
		BlindDealer=true;
		Next=false;
		nGame=hWnd;
		GameMode=0;

		if(PlayerSize == 2)
			NowPlayer=1;
		else
			NowPlayer=2;

		GameB.GameStart();
		DrawBlackJack();
		PlayerTurn=false;
		SetTimer(hWnd,1,100,0);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		DrawBitmap(hdc,0,0,hBitBLACK);
		EndPaint(hWnd, &ps);
		return 0;
		break;
	case WM_GAMETURN:
		if(Next){
			if(PlayerSize == 2){
				if(NowPlayer == 1)
					NowPlayer = 0;
			}
			else if(PlayerSize == 3){
				if(NowPlayer == 2)
					NowPlayer = 1;
				else if(NowPlayer == 1)
					NowPlayer = 0;
			}
			else{				
				if(NowPlayer == 2)
					NowPlayer = 1;
				else if(NowPlayer == 1)
					NowPlayer = 3;
				else if(NowPlayer == 3)
					NowPlayer = 0;
			}
			Next=false;
		}


		switch(NowPlayer){//��� �÷��̾��ΰ�
		case 1://�÷��̾�
			ShowWindow(hEdit,SW_SHOW);
			ShowWindow(hBt,SW_SHOW);
			CanHit[0]=true;
			PlayerTurn=true;
			break;

		case 2:
			if(GameB.Betting[1] == 0){
				GameB.Betting[1] = Players[GameB.Users[2]].Money / (rand()%990 + 10);
			}
			GameB.PlayNPC(2);
			break;

		case 3:
			if(GameB.Betting[2] == 0){
				GameB.Betting[2] = Players[GameB.Users[3]].Money / (rand()%990 + 10);
			}
			GameB.PlayNPC(3);
			break;
		case 0:
			BlindDealer=false;
			GameB.PlayDealer();
			break;
		}
		DrawBlackJack();

		break;
	case WM_CALACCOUNT://����� ����Ѵ�.
		if(GameFin)
			return 0;
		GameFin=true;
		KillTimer(hWnd,1);

		sc=Players[0].Money;
		for(int i=1;i<PlayerSize;i++){
			money=GameB.Betting[i-1]/GameB.Cards[i].PairSize;//�й����� ���õ� ��
			for(int j=0;j<GameB.Cards[i].PairSize;j++){

				if(strcmp(Doing[0][0],"BlackJack")==0){//������ ����
					if(strcmp(Doing[i][j],"BlackJack")!=0){//���� ���� �ƴ�
						Players[GameB.Users[i]].Money -= money;
						Players[GameB.Users[0]].Money += money;
						strcpy(Doing[i][j],"Lose");
					}
					else
						strcpy(Doing[i][j],"Draw");
				}
				else{//���� �ƴ�
					if(strcmp(Doing[i][j],"BlackJack")==0){//���� ����
						Players[GameB.Users[i]].Money += money * 1.5;
						Players[GameB.Users[0]].Money -= money * 1.5;
						strcpy(Doing[i][j],"Win");
					}
				}

				if(strcmp(Doing[0][0],"Bust")==0){//������ ����Ʈ
					if(strcmp(Doing[i][j],"Bust")!=0){//���� �� ����Ʈ
						Players[GameB.Users[i]].Money += money;
						Players[GameB.Users[0]].Money -= money;
						strcpy(Doing[i][j],"Win");
					}
					else
						strcpy(Doing[i][j],"Draw");

				}

				if(GameB.Cards[0].Score[0] <= 21){
					if(GameB.Cards[i].Score[j] > GameB.Cards[0].Score[0] && strcmp(Doing[i][j],"Bust")!=0){//����Ʈ �ƴϰ� �� ����
						Players[GameB.Users[i]].Money += money;
						Players[GameB.Users[0]].Money -= money;
						strcpy(Doing[i][j],"Win");
					}
					else if(GameB.Cards[i].Score[j] == GameB.Cards[0].Score[0]){
						
						strcpy(Doing[i][j],"Draw");
					}
					else{
						Players[GameB.Users[i]].Money -= money;
						Players[GameB.Users[0]].Money += money;
						strcpy(Doing[i][j],"Lose");
					}
				}
			}
		}
		
		DrawBlackJack();

		if(Players[GameB.Users[1]].Money <= 0){//�Ļ�
			if(MessageBox(hWnd, "���� ��� �����̽��ϴ�. �ٽ� ������ �����Ͻðڽ��ϱ�?","�й�..",MB_YESNO)==IDYES){
					DestroyWindow(hWnd);	//������Ŀ�� �ı�
					DestroyWindow(hSel);	//���õ� �ı�
					SendMessage(hMain,WM_COMMAND,101,0);

				}
				else{
					SendMessage(hWnd,WM_CLOSE,0,0);
				}	
		}

		sc = Players[0].Money - sc;
		if(sc > 0){//���� �þ�����, ���� ����
			sprintf(text,"%d �� ȹ��!",sc);
			MessageBox(hWnd, text,"�¸�!",MB_OK);
			Max_Score=max(Max_Score,sc);
			Max_Money=max(Max_Money,Players[0].Money);
		}
		else{
			sprintf(text,"%d ���� �Ҿ����ϴ�!",-sc);
			MessageBox(hWnd, text,"�й�...",MB_OK);

		}
		GameB.OutCheck();

		if(MessageBox(hWnd, "�ѹ� �� �Ͻðڽ��ϱ�?","������",MB_YESNO)==IDYES){	//�ѹ��� ���� ����� �Ѵٸ� �ٽ� �غ��Ѵ�.
				SendMessage(hWnd,WM_REGAME,0,0);
			}
			else{	//�ƴ϶�� ���� ����ȭ������ ���ư���.
				ShowWindow(hSel,SW_SHOW);
				DestroyWindow(hWnd);	//������Ŀ�� �ı�
			}

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_BETBUTTON:
			GameB.Betting[0]=GetDlgItemInt(hWnd,ID_BETEDIT,NULL,false);

			if(GameB.Betting[0] < Players[0].Money/10 && GameB.Betting[0] > Players[0].Money/1000){

				ShowWindow(hEdit,SW_HIDE);
				ShowWindow(hBt,SW_HIDE);
				MakedDoer=0;
				Doer[0]=CreateWindow("BlackJackDoing","����",WS_CHILD | WS_VISIBLE ,600,520,160,120,hWnd,(HMENU)NULL,g_hInst,NULL);
				DrawBlackJack();
			}
			else{
				GameB.Betting[0]=0;
				MessageBox(hWnd,"�ѵ� ���� �ݾ��� ������ �ּ���.","���� ������!",MB_OK);
			}
			break;

		}
		return 0;
	case WM_CHECKDOING:
		switch(wParam){
		case 1://hit

			ani=true;
			anix=-100;
			aniy=-100;
			anit=0;
			anic=0;
			anis=1;
			anip[0]=1;
			anipair[0]=lParam;
			strcpy(Doing[1][lParam],"hit");
			SetTimer(nGame,2,50,0);

			break;
		case 2://stay
			CanHit[lParam]=false;
			if(GameB.Cards[lParam].PairSize == 1 && GameB.Cards[lParam].CardSize[0]==2 && GameB.Cards[lParam].Score[0] == 21){
				strcpy(Doing[1][lParam],"BlackJack");
			}
			else
				strcpy(Doing[1][lParam],"stay");
			DestroyWindow(Doer[lParam]);
			break;
		case 3://split
			GameB.Split(1,lParam);
			if(GameB.Cards[1].PairSize == 2){
				MakedDoer=1;
				CanHit[1]=true;
				Doer[1]=CreateWindow("BlackJackDoing","����",WS_CHILD | WS_VISIBLE ,600,320,160,90,hWnd,(HMENU)NULL,g_hInst,NULL);
			}
			if(GameB.Cards[1].PairSize == 3){
				MakedDoer=2;
				CanHit[2]=true;
				Doer[2]=CreateWindow("BlackJackDoing","����",WS_CHILD | WS_VISIBLE ,600,120,160,90,hWnd,(HMENU)NULL,g_hInst,NULL);
			}

			break;
		case 4://double down

			ani=true;
			anix=-100;
			aniy=-100;
			anit=0;
			anic=0;
			anis=1;
			anip[0]=1;
			anipair[0]=0;
			strcpy(Doing[1][0],"Double-Down");
			GameB.Betting[0]*=2;
			SetTimer(nGame,2,50,0);
			Next=true;
			PlayerTurn=false;
			DestroyWindow(Doer[0]);

			break;

		case 5://bust

			CanHit[lParam]=false;
			strcpy(Doing[1][lParam],"Bust");
			DestroyWindow(Doer[lParam]);

			break;
		}

		DrawBlackJack();

		break;
	case WM_TIMER:
		switch(wParam){
		case 1:
			if(ani)//�ִϸ��̼� ���̸�
				return 0;

			if(GameFin)
				return 0;

			if(PlayerTurn){//����� �Է� ����
				check=true;
				for(int i=0;i<GameB.Cards[1].PairSize;i++){
					if(CanHit[i]){
						check=false;
					}
				}
				if(check){
					Next=true;
					PlayerTurn=false;
				}
				else
					return 0;
			}

			SendMessage(hWnd,WM_GAMETURN,NowPlayer,0);
			break;
		case 2://�ִϸ��̼�
			anit++;

			if(anit==11){//�ϳ��� ī�� �ִϸ��̼��� ������ ���� ī���
				GameB.GiveCard(anip[anic],anipair[anic]);//ī�带 �ش�
				anit=0;		
				anic++;
				if(anic == anis){//�ִϸ��̼� ����	
					ani=false;	
					KillTimer(hWnd,2);
					GameB.CalScore();
					DrawBlackJack();
					break;
				}
			}

			if(GameB.Cards[anip[anic]].CardSize[anipair[anic]] < 5){
				x=GameB.Cards[anip[anic]].CardSize[anipair[anic]]*60;
				y=yp_b[anip[anic]];
			}
			else{
				x=(GameB.Cards[anip[anic]].CardSize[anipair[anic]]-5)*60;
				y=yp_b[anip[anic]]-85;
			}


			anix=abs(140 - ( 140 - xs_b[anip[anic]] - x ) * anit / 10 );
			aniy=abs(50 -  ( 50  - ys_b[anipair[anic]] - y) * anit / 10 );
			DrawBlackJack();

			break;
		}
		return 0;

	case WM_CLOSE:
		DestroyWindow(hMain);
		break;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}



BOOL CALLBACK SelBlackPlayerDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	static int PlayerCnt;
	switch(iMessage) {
	case WM_INITDIALOG:
		PlayerCnt=0;
		for(int i=0;i<10;i++)
			if(Players[i].Money > 0)
				PlayerCnt++;
		for(int i=4; i>=PlayerCnt; i--){
			EnableWindow(GetDlgItem( hDlg , IDC_BUTTON1-1+i),false);
		}
		return TRUE;
	case WM_COMMAND:
		switch (wParam) {
		case IDC_BUTTON1:
			PlayerSize=2;
			EndDialog(hDlg,1);
			return TRUE;
		case IDC_BUTTON2:
			PlayerSize=3;
			EndDialog(hDlg,1);
			return TRUE;
		case IDC_BUTTON3:
			PlayerSize=4;
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


LRESULT CALLBACK BlackJackDoing(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam){


	HDC hdc,MemDC;
	PAINTSTRUCT ps;
	HBITMAP MyBitmap, OldBitmap;
	char text[200];
	int x,y;
	static bool set;
	static bool First;
	int PairNum;
	bool check;

	switch(iMessage) {
	case WM_CREATE:
		SetWindowLong(hWnd,0,MakedDoer);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);

		MemDC=CreateCompatibleDC(hdc);
		PairNum=GetWindowLongA(hWnd,0);

		if(GameB.Cards[1].Score[PairNum] > 21){
			SendMessage(nGame,WM_CHECKDOING,5,PairNum);
			return 0;
		}
		else if(GameB.Cards[1].Score[PairNum] == 21){ //�ֻ���, �ٷ� ������
			SendMessage(nGame,WM_CHECKDOING,2,PairNum);
			return 0;
		}


		DrawBitmap(hdc,0,60,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BlackJack_Hit)));
		DrawBitmap(hdc,0,90,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BlackJack_Stay)));

		if(!First){
			DrawBitmap(hdc,0,0,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BlackJack_DoubleDown)));
		}
		if(GameB.Cards[1].PairSize < 3 && PairNum < 2 && GameB.Cards[1].CardSize[PairNum]==2 && GameB.Cards[1].Cards[PairNum][0] % 100 == GameB.Cards[1].Cards[PairNum][1] % 100){
			DrawBitmap(hdc,0,30,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BlackJack_Split)));
		}
		/*
		sprintf(text,"%d",PairNum);
		TextOut(hdc,0,0,text,strlen(text));*/


		EndPaint(hWnd, &ps);
		return 0;

	case WM_LBUTTONDOWN:
		if(ani)
			return 0;

		x=LOWORD(lParam);
		y=HIWORD(lParam);

		PairNum=GetWindowLong(hWnd,0);

		if( 0 <= x && x <= 160 && 60 <= y && y <= 90){//hit
			First=true;
			SendMessage(nGame,WM_CHECKDOING,1,PairNum);
		}
		else if( 0 <= x && x <= 160 && 90 <= y && y <= 120){//stay
			SendMessage(nGame,WM_CHECKDOING,2,PairNum);
		}
		else if( 0 <= x && x <= 160 && 30 <= y && y <= 60 ){//split
			if(GameB.Cards[1].PairSize < 3 && PairNum < 2 && GameB.Cards[1].CardSize[PairNum]==2 && GameB.Cards[1].Cards[PairNum][0] % 100 == GameB.Cards[1].Cards[PairNum][1] % 100){
				First=true;
				SendMessage(nGame,WM_CHECKDOING,3,PairNum);
			}
		}
		else if(!First && 0 <= x && x <= 160 && 0 <= y && y <= 30 && PairNum == 0){//double down
			SendMessage(nGame,WM_CHECKDOING,4,PairNum);

		}


		return 0;	

	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));

}