#pragma once

#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "Poker.h"
#include "resource.h"


/*
������ ����

�÷��̾ �������� �� �ӵ� ����.
���� �÷��̾� ���� 4 �̸��϶� ���� �����ϰ� �ϱ�.

������ ���� �̻��ϴ�. �պ���.
������ ���������� ������ �ƹ��� ������ ���� �ϴ°� ���ڴ�.

Ŭ�������� �ִϸ��̼� ����

*/

#define WM_Bet_Call 100001
#define WM_Bet_Half 100002
#define WM_Bet_Quarter 100003
#define WM_Bet_Dadang 100004
#define WM_Bet_Check 100005
#define WM_Bet_Bing 100006
#define WM_Bet_Die 100007
#define WM_NEWCARD 100008
#define WM_NEXTTURN 100009
#define WM_REGAME 100010

bool first;
int selectopen; //������ ī�� ����. 0=ī��й�, 1=ī�弱��, 2=ī�弱�� �Ϸ�

void DrawSevenPoker();
int TalkWinner(int P1, int P2, int P3, int P4,int Users[], HWND hWnd);

LRESULT CALLBACK SevenPokerBetting(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);

int BingMoney;
int Canix,Caniy,Canip,Canit,Canir;
bool finani[4];
char Bet[4][200];	//�ش� �÷��̾ �� �����ߴ��� ������ֱ� ���� ���ڿ�. 
int GameTurn;	//�ѹ� ����� ������ ���δ�. 5��° = ī�� 4, 10��° = ī�� 5, 15��° = ī�� 6, 20��° = ī�� 7, 25��° = ���� ��
bool Animation;	//�ִϸ��̼� ������ �ƴ��� Ȯ���Ѵ�.

class SEVENPOKER{

public:
	int Users[4];
	int Call_Money;
	int Game_Money;
	int Cards[4][7];
	int CardTable[52];
	int CardNum;
	int Turn;
	int Dealer;
	int score[4],view[4];
	bool Die[4];

	void SelectUser(){	//�÷��̾� ����

		srand(time(NULL));
		Users[0]=0;
		Users[1]=0;
		Users[2]=0;
		Users[3]=0;

		while(Players[Users[1]].Money >= 0 && (Users[1]==Users[0]))
			Users[1]=rand()%9+1;
		while(Players[Users[2]].Money >= 0 && (Users[2]==Users[0] || Users[2]==Users[1] ))
			Users[2]=rand()%9+1;
		while(Players[Users[3]].Money >= 0 && (Users[3]==Users[0] || Users[3]==Users[1] || Users[3]==Users[2] ))
			Users[3]=rand()%9+1;
	}

	void ChangeUser(int LastPlayer){
		if(Mode==0){//���Ѹ�尡 �ƴҶ�
			int cnt=0;

			for(int i=1;i<10;i++){
				if(Players[i].Money > 0)
					cnt++;
			}
			if(cnt < 3){//���� �÷��̾ 3���� �ȵɶ�
				Users[LastPlayer]=-1;
				return;
			}
		}
		else{//���Ѹ����, ���� ����
			Players[Users[LastPlayer]].Money=10000000;
		}
		bool check=true;
		while(check){
			Users[LastPlayer]=rand()%9+1;

			check=false;
			for(int i=0;i<4;i++){				
				if(i!=LastPlayer){					
					if(Users[i]==Users[LastPlayer]){
						check=true;
						break;
					}
				}
			}

			if(Players[Users[LastPlayer]].Money <= 0){
				check=true;
			}
		}
	}

	void MakeCardBox(){	//������ ������ ī�带 �״´�.
		bool CardView[52];
		memset(CardView,0,sizeof(bool)*52);
		//	for(int i=0;i<52;i++)
		//		CardView[i]=0;
		srand(time(NULL));

		for(int i=0;i<52;i++){
			do{
				CardTable[i]=rand()%52+1;
			}while(CardView[CardTable[i]-1]!=false);
			CardView[CardTable[i]-1]=true;
		}

		for(int i=0;i<52;i++)
			CardTable[i]=100*(((CardTable[i]-1)/13)+1)+((CardTable[i]-1)%13+1);

	}

	void GiveCard(int turn, int Starter){	//Starter���� ���徿 ī�带 �����ش�.
		for(int i=Starter,j=0;j<4;j++,i++,i%=4){
			if(!Die[i])	//���� �ʾҾ�� ī�带 �ش�
				Cards[i][turn]=CardTable[++CardNum];
		}
	}

	void GameStart(){//���� ����, �ʱ�ȭ
		MakeCardBox();
		for(int i=0;i<7;i++){
			Cards[0][i]=0;
			Cards[1][i]=0;
			Cards[2][i]=0;
			Cards[3][i]=0;
		}
		CardNum=0;
		Die[0]=false;
		Die[1]=false;
		Die[2]=false;
		Die[3]=false;/*
					 GiveCard(0,Dealer);
					 GiveCard(1,Dealer);
					 GiveCard(2,Dealer);*/
		selectopen=false;
		Turn=0;
		Game_Money=BingMoney*4;
		Call_Money=BingMoney;

		Players[Users[0]].Money-=BingMoney;
		Players[Users[1]].Money-=BingMoney;
		Players[Users[2]].Money-=BingMoney;
		Players[Users[3]].Money-=BingMoney;
	}

	int CalSevenPoker(HWND hWnd){
		int P[4]={0,};
		int Score[21]={0,};

		for(int i=0;i<4;i++){	//�� �÷��̾��� ���� �е��� ����� ���� ����Ͽ� ���� ���� �и� ã�´�
			P[i]=CalScore(Turn,Cards[i]);

			if(Die[i]){	//���� �� �÷��̾ ���̶��?
				P[i]=-1;
			}
		}

		if(Turn==7)
			return TalkWinner(P[0],P[1],P[2],P[3],Users,hWnd);
	}

	void PlayNPC(HWND hWnd,int PlayerNum){
		int num=AI(PlayerNum)+WM_Bet_Call;
		SendMessage(hWnd,num,0,0);		
		DrawSevenPoker();
	}

	void Recommend(){
		for(int i=0;i<4;i++){
			score[i]=CalRecommend(Turn,Cards[i]);
			view[i]=CalRecommend(Turn-2,&Cards[i][2]);
		}

	}

	int AI(int PlayerNum){	//�е��� ���� ������ ����
		enum {a=0,b,c,d,e,f,g,h,i};
		int bet;
		switch(score[PlayerNum]/10000){
		case 15://��Ƽ��
		case 14://��Ƽ��
			bet=a;
			break;
		case 13://��ī��
			bet=a;
			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 > score[PlayerNum]/10000){//������ ����
						bet=i;
						break;
					}
					if(view[i]/10000 == 8)//��Ƽ��-4
						bet=b;
				}
			}
			break;
		case 12://Ǯ�Ͽ콺
			bet=a;
			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//��ī��
						bet=i;
						break;
					}
					else if(view[i]/10000 == 8)//��Ƽ��-4
						bet=max(c,bet);
					else if((view[i]/10000 == 9 || view[i]/10000 == 7) && (view[i]%10000)/100 > (score[PlayerNum]%10000)/100) //���ڰ� ���� Ʈ����,�����
						bet=max(b,bet);
				}
			}
			break;
		case 11://�÷���
			bet=a;			
			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//��ī��
						bet=i;
						break;
					}
					else if(view[i]/10000 == 8)//��Ƽ��-4
						bet=max(c,bet);
					else if(view[i]/10000 == 9 || view[i]/10000 == 7) //���ڰ� ���� Ʈ����,�����
						bet=max(b,bet);
				}
			}
			break;
		case 10://��Ʈ����Ʈ
			bet=b;

			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//��ī��
						bet=i;
						break;
					}
					else if(view[i]/10000 == 9 || view[i]/10000 == 8 || view[i]/10000 == 7 || view[i]/10000 == 6 || (view[i]/10000 == 5 && (view[i]%10000)/100 > (score[PlayerNum]%10000)/100)) //Ʈ����, ��Ƽ��-4,�÷���-4, ���� ��Ʈ����Ʈ-4,�����
						bet=max(g,bet);
					else if(view[i]/10000 == 4 || view[i]/10000 == 3 || view[i]/10000 == 2)//�����, ��Ʈ����Ʈ-3, �÷���-3
						bet=max(d,bet);
				}
			}
			break;
		case 9://Ʈ����

			switch(Turn){
			case 7: bet=b;
				for(int i=0;i<4;i++){
					if(i!=PlayerNum){
						if(view[i]/10000 > score[PlayerNum]/10000){//���� ũ��
							bet=i;
							break;
						}
						else if(view[i]/10000 == 9 || view[i]/10000 == 8 || view[i]/10000 == 7 || view[i]/10000 == 6 || view[i]/10000 == 5 ) //Ʈ����, ��Ƽ��-4,�÷���-4, ���� ��Ʈ����Ʈ-4,�����
							bet=max(c,bet);
					}
				}
				break;
			case 6: bet=b;
				for(int i=0;i<4;i++){
					if(i!=PlayerNum){
						if(view[i]/10000 > score[PlayerNum]/10000){//���� ũ��
							bet=h;
							break;
						}
					}
				}
				break;
			default: bet=a;
				for(int i=0;i<4;i++){
					if(i!=PlayerNum){
						if(view[i]/10000 > score[PlayerNum]/10000){//���� ũ��
							bet=h;
							break;
						}
					}
				}
				break;
			}
			break;
		case 8: //��Ƽ��-4
			switch(Turn){
			case 7: bet=i; break;
			case 6: bet=a; 				
				for(int i=0;i<4;i++){
					if(i!=PlayerNum){
						if(view[i]/10000 == 13){//��ī��
							bet=e;
							break;
						}
					}
				}				
				break;
			default: bet=a;	break;
			}
			break;
		case 7://�����
			switch(Turn){
			case 7: bet=a; 

				for(int i=0;i<4;i++){
					if(i!=PlayerNum){
						if(view[i]/10000 == 13){//��ī��
							bet=i;
							break;
						}
						else if(view[i]/10000 == 9 || view[i]/10000 == 8 || view[i]/10000 == 7 || view[i]/10000 == 6 || view[i]/10000 == 5) //Ʈ����, ��Ƽ��-4,�÷���-4, ��Ʈ����Ʈ-4,�����
							bet=max(g,bet);
						else if(view[i]/10000 == 4 || view[i]/10000 == 3 || view[i]/10000 == 2)//�����, ��Ʈ����Ʈ-3, �÷���-3
							bet=max(d,bet);
					}
				}
				break;
			case 6: bet=b; break;
			default : bet=a; break;
			}
			break;
		case 6://�÷���-4
			if(Turn==7){
				bet=i;
				break;
			}
			bet=a;
			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//��ī��
						bet=i;
						break;
					}
				}
			}
			break;
		case 5://��Ʈ����Ʈ-4

			if(Turn==7){
				bet=i;
				break;
			}
			bet=a;
			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//��ī��
						bet=i;
						break;
					}
				}
			}
			break;
		case 4://�����
			bet=e;
			if(Turn==7 && ((score[PlayerNum]%10000)/100 == 14 || (score[PlayerNum]%10000)/100 == 11 || (score[PlayerNum]%10000)/100 == 12 || (score[PlayerNum]%10000)/100 == 13 ))//���� �����
				bet=d;
			if(Turn <= 6)
				bet=d;

			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//��ī��
						bet=i;
						break;
					}
				}
			}
			break;
		case 3://�÷���-3
			if(Turn>=6)
				bet=i;
			else
				bet=e;

			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//��ī��
						bet=i;
						break;
					}
				}
			}
			break;
		case 2://��Ʈ����Ʈ-3

			if(Turn>=6)
				bet=i;
			else if(Turn==5)
				bet=f;
			else
				bet=e;

			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//��ī��
						bet=i;
						break;
					}
				}
			}
			break;
		case 1:
			switch(Turn){
			case 7: bet=i; break;
			case 6: bet=h; break;
			default: bet=g; break;
			}
			break;
		}

		int bettyp_se[9][6]={
			{2,1,3},
			{4,1,2,3,0},
			{3,2,3,0},
			{2,2,0},
			{4,2,0,4,5},
			{5,2,0,4,5,6},
			{3,0,4,5},
			{4,0,4,5,6},
			{1,6}
		};


		int betting;
		bool check=true;
		do{
			betting=bettyp_se[bet][rand()%bettyp_se[bet][0]+1];
			check=false;
			if(PlayerNum != Dealer){//���� ������ �ƴ϶��
				if(betting == 4 || betting == 5)
					check=true;
			}
		}while(check);

		return betting;

	}
};


HBITMAP hBitSeven;

SEVENPOKER Game;	
int xs_s[4]={260,100,700,800};
int xp_s[4]={64,0,-64,0};
int ys_s[4]={600,150,60,550};
int yp_s[4]={0,64,0,-64};


void DrawSevenPoker(){

	RECT crt;
	HDC hdc,hMemDC;
	HBITMAP OldBit,CardImg;
	char text[200];
	int xt[4]={260,100,700,670};
	int yt[4]={600,150,150,550};

	GetClientRect(nGame,&crt);	//slot�������� ũ�⿡ �°� �簢�� ����
	hdc=GetDC(nGame);	//slot �������� dc�� �����´�

	if(hBitSeven==NULL)		//������ ũ�⿡ �°� ��Ʈ�� ����
		hBitSeven=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	hMemDC=CreateCompatibleDC(hdc);	//���� dc�� ���Ѵ�
	OldBit=(HBITMAP)SelectObject(hMemDC,hBitSeven);

	FillRect(hMemDC,&crt,GetSysColorBrush(COLOR_WINDOW));	//hMemDC�� ���� �������� �Ѵ�

	for(int j=0;j<4;j++){
		if(Game.Die[j]){//�ش� �÷��̾ �������� ��
			for(int i=0;Game.Cards[j][i]!=0 && i < Game.Turn ;i++){
				//�޸� ī�常 �����ֵ��� �Ѵ�.
				RotateSizingImage(hMemDC,CardBack,90*j,10,10,30+i*xp_s[j]+xs_s[j],30+i*yp_s[j]+ys_s[j],1,0,0,0);
			}
		}
		else{ //������� ��
			for(int i=0;i<Game.Turn;i++){
				if(i==Game.Turn-1 && !finani[j]){//�ִϸŴϼ� �ȉ�ٸ�
					break;
				}

				if(j!=0 && (i==0 || i==1 || i==6 || (selectopen!=2 && i==2)) && GameTurn!=25 )	//npc�� 1,2,7��° ī��� �������� �ʴ´�
					CardImg=CardBack;
				else
					CardImg=PokerCards[(Game.Cards[j][i]/100)-1][(Game.Cards[j][i]%100)-1];

				if(GameTurn==25)
					RotateSizingImage(hMemDC,CardImg,90*j,10,10,30+i*xp_s[j]+xs_s[j],30+i*yp_s[j]+ys_s[j],1,0,0,0);
				else
					RotateSizingImage(hMemDC,CardImg,90*j,10,10,30+i*xp_s[j]+xs_s[j],30+i*yp_s[j]+ys_s[j],1,0,0,0);
				
			/*	sprintf(text,"%d",Game.Cards[j][i]);
				TextOut(hMemDC,30+i*xp_s[j]+xs_s[j],30+i*yp_s[j]+ys_s[j],text,strlen(text));*/

			}
		}
		if(j==Game.Dealer){
			TextOut(hMemDC,xt[j],yt[j]-50,"����",4);
		}
		sprintf(text,"%s",Players[Game.Users[j]].Name);
		TextOut(hMemDC,xt[j],yt[j]-30,text,strlen(text));

		sprintf(text,"�� : %d",Players[Game.Users[j]].Money);
		TextOut(hMemDC,xt[j],yt[j]-10,text,strlen(text));
		TextOut(hMemDC,xt[j]+100,yt[j]-10,Bet[j],strlen(Bet[j]));

	}

	sprintf(text,"�ǵ� : %d",Game.Game_Money);

	TextOut(hMemDC,300,300,text,strlen(text));
	sprintf(text,"�� �ݾ� : %d",Game.Call_Money);
	TextOut(hMemDC,300,330,text,strlen(text));

	for(int i=0;i<(52-Game.CardNum)/4;i++){

		DrawBitmap(hMemDC,430-3*i,350-3*i,CardBack);
	}		
	if(Canip!=-1){
		CardImg=PokerCards[(Game.Cards[Canip][Game.Turn-1]/100)-1][(Game.Cards[Canip][Game.Turn-1]%100)-1];
		if((Game.Turn == 1 || Game.Turn == 2 || Game.Turn == 3 || Game.Turn==7) && Canip!=0)
			CardImg=CardBack;
		RotateSizingImage(hMemDC,CardImg,Canir,10,10,Canix,Caniy,1,0,0,0);
	}

	if(selectopen==1 && !Animation){
		TextOut(hMemDC,30+xp_s[0]+xs_s[0],yp_s[0]+ys_s[0],"������ ī�带 �����ϼ���",24);
	}

	SelectObject(hMemDC,OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(nGame,hdc);
	InvalidateRect(nGame,NULL,false);

}

LRESULT CALLBACK SevenPokerProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	char text[200];
	static bool bet;	//bet = �÷��̾� ������, ���� �����찡 ������ ��. ���ð��� �����ϱ� ������ ���� ������ ���� �ʴ´�.
	static bool NewCard;	//NewCard = ���� ���� ���� ���� ��, �� ī�带 �����ش�.
	static int PlayerNum;	//���� ���� �÷��̾��� ��ȣ.
	static HANDLE BetTimer;
	static bool GameFin;	//������ �������� ���� false, �������� true
	int cnt,DieWin,x,y,tmp;

	switch(iMessage) {
	case WM_CREATE:

		for(int i=100;;i*=10){
			if(i*10000 >= Players[0].Money){
				BingMoney=i;
				break;
			}
		}
		Game.SelectUser();
		Game.Dealer=0;
		PlayerNum=3;
		Game.GameStart();
		bet=true;
		nGame=hWnd;
		Bet[0][0]=0;
		Bet[1][0]=0;
		Bet[2][0]=0;
		Bet[3][0]=0;
		NewCard=false;
		GameFin=false;
		GameTurn=0;
		Canix=-100;
		Caniy=-100;
		Canip=-1;
		Animation=false;
		BetTimer=(HANDLE)SetTimer(hWnd,1,500,NULL);

		DrawSevenPoker();

		return 0;
	case WM_REGAME:

		for(int i=100;;i*=10){
			if(i*10000 >= Players[0].Money){
				BingMoney=i;
				break;
			}
		}
		PlayerNum=Game.Dealer-1;
		Game.GameStart();
		bet=true;
		Bet[0][0]=0;
		Bet[1][0]=0;
		Bet[2][0]=0;
		Bet[3][0]=0;
		NewCard=false;
		GameFin=false;
		GameTurn=0;
		Canip=-1;		
		Animation=false;

		BetTimer=(HANDLE)SetTimer(hWnd,1,500,NULL);

		DrawSevenPoker();

		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		DrawBitmap(hdc,0,0,hBitSeven);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		}		
		return 0;
	case WM_LBUTTONDOWN:
		if(selectopen==1){//ī�� ����

			x=LOWORD(lParam);
			y=HIWORD(lParam);
			if(20+xs_s[0] <= x && x <= 86+xs_s[0] && 20+ys_s[0] <= y && y <=  113+ys_s[0]){//ù��° ��
				tmp=Game.Cards[0][0];
				Game.Cards[0][0]=Game.Cards[0][2];
				Game.Cards[0][2]=tmp;
				SetTimer(hWnd,1,1000,NULL);
				selectopen=2;
			}
			else if(20+xp_s[0]+xs_s[0] <= x && x <= 86+xp_s[0]+xs_s[0] && 20+yp_s[0]+ys_s[0] <= y && y <= 113+yp_s[0]+ys_s[0]){
				tmp=Game.Cards[0][1];
				Game.Cards[0][1]=Game.Cards[0][2];
				Game.Cards[0][2]=tmp;
				SetTimer(hWnd,1,1000,NULL);
				selectopen=2;
			}
			else if(20+2*xp_s[0]+xs_s[0] <= x && x <= 86+2*xp_s[0]+xs_s[0] && 20+2*yp_s[0]+ys_s[0] <= y && y <= 113+2*yp_s[0]+ys_s[0]){

				SetTimer(hWnd,1,1000,NULL);
				selectopen=2;
			}

		}

		return 0;
	case WM_NEXTTURN:
		if(Game.Turn < 3){
			Canip=Game.Dealer-1;
			//Canip=3;
			Canit=-1;
			Animation=true;
			finani[0]=false;
			finani[1]=false;
			finani[2]=false;
			finani[3]=false;
			SetTimer(hWnd,2,25,NULL);
			Game.GiveCard(Game.Turn++,0);	//ī�� �й�
			if(Game.Turn == 3){

				Game.Recommend();
				KillTimer(hWnd,1);
				selectopen=1;
			}
			return 0;
		}

		GameTurn++;	//�� �� ����

		//���̰� ���̸� �ڵ����� �����̰� �¸�
		cnt=0;
		for(int i=0;i<4;i++){
			if(Game.Die[i])
				cnt++;
			else
				DieWin=i;
		}
		if(cnt==3){	//���̰� ��
			KillTimer(hWnd,1);	//������ �������Ƿ� Ÿ�̸Ӵ� �����Ѵ�.
			sprintf(text,"����: %s!",Players[Game.Users[DieWin]].Name);
			MessageBox(hWnd, text,"���",NULL);
			Players[Game.Users[DieWin]].Money+=Game.Game_Money;	//���ڿ��� ���� �߰��Ѵ�
			Game.Game_Money=0;	//���õǾ��� ���� �ʱ�ȭ
			GameFin=true;	//���� ����
			if(Players[0].Money <= 0){
				if(MessageBox(hWnd, "���� ��� �����̽��ϴ�. �ٽ� ������ �����Ͻðڽ��ϱ�?","�й�..",MB_YESNO)==IDYES){
					DestroyWindow(hWnd);	//������Ŀ�� �ı�
					DestroyWindow(hSel);	//���õ� �ı�
					SendMessage(hMain,WM_COMMAND,101,0);
				}
				else{
					SendMessage(hWnd,WM_CLOSE,0,0);
				}
			}

			for(int i=1;i<4;i++){
				if(Players[Game.Users[i]].Money <= 0){
					sprintf(text,"%s �й�!!",Players[Game.Users[i]].Name);
					Game.ChangeUser(i);
					Down++;
					if(Game.Users[i] == -1){//����Ұ�
						MessageBox(hWnd,"���� �÷��̾ 4�� �̸��Դϴ�. �ٸ� ������ �̿����ּ���.","�¸�!",MB_OK);

						ShowWindow(hSel,SW_SHOW);
						DestroyWindow(hWnd);	//������Ŀ�� �ı�
					}
					else{
						sprintf(&text[strlen(text)],"\n%s �� �����մϴ�!",Players[Game.Users[i]].Name);
						MessageBox(hWnd, text,"���",MB_OK);
					}
				}
			}

			if(MessageBox(hWnd, "�ѹ� �� �Ͻðڽ��ϱ�?","������",MB_YESNO)==IDYES){	//�ѹ��� ���� ����� �Ѵٸ� �ٽ� �غ��Ѵ�.
				Game.Dealer=DieWin;
				SendMessage(hWnd,WM_REGAME,0,0);
			}
			else{	//�ƴ϶�� ���� ����ȭ������ ���ư���.
				ShowWindow(hSel,SW_SHOW);
				DestroyWindow(hWnd);	//������Ŀ�� �ı�
			}
			return 0;
		}

		if(GameTurn==25){	//���� ��.

			DrawSevenPoker();
			KillTimer(hWnd,1);	//������ �������Ƿ� Ÿ�̸Ӵ� �����Ѵ�.
			int winer=Game.CalSevenPoker(hWnd);	//�е��� ����Ͽ� ���ڸ� ������.

			if(winer==0){	//���ڰ� 0��, �� �÷��̾��� ��� ����� �����Ѵ�.
				Max_Score=max(Max_Score,Game.Game_Money);
				Max_Money=max(Max_Money,Players[0].Money+Game.Game_Money);
			}

			Players[Game.Users[winer]].Money+=Game.Game_Money;	//���ڿ��� ���� �߰��Ѵ�
			Game.Game_Money=0;	//���õǾ��� ���� �ʱ�ȭ
			GameFin=true;	//���� ����

			if(Players[0].Money <= 0){
				if(MessageBox(hWnd, "���� ��� �����̽��ϴ�. �ٽ� ������ �����Ͻðڽ��ϱ�?","�й�..",MB_YESNO)==IDYES){
					DestroyWindow(hWnd);	//������Ŀ�� �ı�
					DestroyWindow(hSel);	//���õ� �ı�
					SendMessage(hMain,WM_COMMAND,101,0);

				}
				else{
					SendMessage(hWnd,WM_CLOSE,0,0);
				}
			}
			for(int i=1;i<4;i++){
				if(Players[Game.Users[i]].Money <= 0){
					sprintf(text,"%s �й�!!",Players[Game.Users[i]].Name);
					Game.ChangeUser(i);
					Down++;
					if(Game.Users[i] == -1){//�Ϲݸ�� ����
						MessageBox(hWnd,"�� �÷��̾���� �Ļ���׽��ϴ�! ���Ѹ��� �����մϴ�!","�¸�!",MB_OK);
						InitUnlimited();
						Game.SelectUser();
					}
					else{
						sprintf(&text[strlen(text)],"\n%s �� �����մϴ�!",Players[Game.Users[i]].Name);
						MessageBox(hWnd, text,"���",MB_OK);
					}
				}
			}

			if(MessageBox(hWnd, "�ѹ� �� �Ͻðڽ��ϱ�?","������",MB_YESNO)==IDYES){	//�ѹ��� ���� ����� �Ѵٸ� �ٽ� �غ��Ѵ�.
				Game.Dealer=winer;
				SendMessage(hWnd,WM_REGAME,0,0);
			}
			else{	//�ƴ϶�� ���� ����ȭ������ ���ư���.
				ShowWindow(hSel,SW_SHOW);
				DestroyWindow(hWnd);	//������Ŀ�� �ı�
			}
			return 0;
		}
		else if(GameTurn > 0 && GameTurn%5==0){	// 5���� ���, ī�带 �ִ� ��.
			Canip=Game.Dealer-1;
			//Canip=3;
			Canit=-1;
			Animation=true;
			finani[0]=false;
			finani[1]=false;
			finani[2]=false;
			finani[3]=false;
			SetTimer(hWnd,2,25,NULL);
			Game.GiveCard(Game.Turn++,0);	//ī�� �й�
			for(int i=0;i<4;i++){
				if(!Game.Die[i] && Players[Game.Users[i]].Money > 0){	//�� �÷��̾ ���� �ʾҰ�, ���ε� �ƴ϶��
					Bet[i][0]=0;	//���ñ�� �ʱ�ȭ
				}
			}
			Game.Recommend();
			DrawSevenPoker();

		}
		else{	//ī������� ���� �ƴ�, �÷��̾���� ���� ��
			PlayerNum++;	//���� �÷��̾�� �Ѿ��
			PlayerNum%=4;
			if(!Game.Die[PlayerNum] && Players[Game.Users[PlayerNum]].Money > 0){	//���� �÷��̾ �������� �ʰ�, ���ε� �ƴ� ��쿡�� ����
				if(PlayerNum==0){	//������ ��, ���� ���ϵ带 �����Ѵ�

					if(Game.Dealer == 0)
						first=true;
					else
						first=false;
					bet=false;	//������ �ڴ�� ������� �ʵ��� �Ѵ�.
					CreateWindow("SevenPokerBetter","����",WS_CHILD | WS_VISIBLE ,500,500,150,90,hWnd,(HMENU)NULL,g_hInst,NULL);	//���� ���ϵ� ����

					DrawSevenPoker();
				}
				else{	//npc���� ��
					Game.PlayNPC(hWnd,PlayerNum);	//�ڵ����� ����
				}
			}
			else
				SendMessage(hWnd,WM_NEXTTURN,0,0);
		}
		return 0;	

	case WM_TIMER:
		switch(wParam){
		case 1:	//���� ���� Ÿ�̸�
			if(GameFin)	//���� ������ �����ٸ�
				return 0;	//���� �ʴ´�

			if(!bet)	//���� ���� ������ ���� ������̶��
				return 0;	//���� �ʴ´�

			if(Animation)//�ִϸ��̼� ���̶�� ���� �ʴ´�
				return 0;

			if(selectopen == 1)//ī�� ������ �ȵǾ��ٸ�
				return 0;

			SendMessage(hWnd,WM_NEXTTURN,0,0);
			break;
		case 2://ī�� �ִϸ��̼� Ÿ�̸�
			x=430 - 3*((52-Game.CardNum)/4);
			y=350 - 3*((52-Game.CardNum)/4);
			Canir=(Canip*90)*Canit/10;
			Canit++;
			if(Canit==11){
				Canit=0;
				finani[Canip]=true;				
			}
			if(Canit==0){//ù �����̶��
				while(1){
					Canip++;
					Canip%=4;
					if(finani[Canip]){//�ѹ��� ���Ҵٸ�
						Canip=-1;
						Animation=false;
						KillTimer(hWnd,2);
						break;
					}
					if(Game.Die[Canip]){//������ �÷��̾���
						finani[Canip]=true;
					}
					else
						break;
				}
			}

			Canix=abs(x-(x - ( 30+(Game.Turn-1)*xp_s[Canip]+xs_s[Canip]) ) * (Canit) / 10 );
			Caniy=abs(y-(y - ( 30+(Game.Turn-1)*yp_s[Canip]+ys_s[Canip]) ) * (Canit) / 10 );
			DrawSevenPoker();

			break;
		}
		return 0;

	case WM_Bet_Call :	
		bet=true;
		Game.Game_Money+=Game.Call_Money;
		Players[Game.Users[PlayerNum]].Money-=Game.Call_Money;
		strcpy(Bet[PlayerNum],"��");
		if(Players[Game.Users[PlayerNum]].Money <= 0){//����
			Players[Game.Users[PlayerNum]].Money=0;
			strcpy(Bet[PlayerNum],"All - In!");
		}

		DrawSevenPoker();
		break;	
	case WM_Bet_Half :
		bet=true;
		Game.Call_Money=(Game.Game_Money/2);
		Game.Game_Money+=(Game.Game_Money/2);
		Players[Game.Users[PlayerNum]].Money-=(Game.Game_Money/2);
		strcpy(Bet[PlayerNum],"����");
		if(Players[Game.Users[PlayerNum]].Money <= 0){//����
			Players[Game.Users[PlayerNum]].Money=0;
			strcpy(Bet[PlayerNum],"All - In!");
		}

		DrawSevenPoker();
		break;	
	case WM_Bet_Quarter :
		bet=true;
		Game.Call_Money=(Game.Game_Money/4);
		Game.Game_Money+=(Game.Game_Money/4);
		Players[Game.Users[PlayerNum]].Money-=(Game.Game_Money/4);
		strcpy(Bet[PlayerNum],"����");
		if(Players[Game.Users[PlayerNum]].Money <= 0){//����
			Players[Game.Users[PlayerNum]].Money=0;
			strcpy(Bet[PlayerNum],"All - In!");
		}		
		DrawSevenPoker();
		break;	
	case WM_Bet_Dadang :
		bet=true;
		Game.Call_Money=(Game.Call_Money*2);
		Game.Game_Money+=(Game.Call_Money*2);
		Players[Game.Users[PlayerNum]].Money-=(Game.Call_Money*2);
		strcpy(Bet[PlayerNum],"����");
		if(Players[Game.Users[PlayerNum]].Money <= 0){//����
			Players[Game.Users[PlayerNum]].Money=0;
			strcpy(Bet[PlayerNum],"All - In!");
		}		
		DrawSevenPoker();
		break;	
	case WM_Bet_Check :	
		bet=true;
		strcpy(Bet[PlayerNum],"üũ");
		if(Players[Game.Users[PlayerNum]].Money <= 0){//����
			Players[Game.Users[PlayerNum]].Money=0;
			strcpy(Bet[PlayerNum],"All - In!");
		}		
		DrawSevenPoker();
		break;	
	case WM_Bet_Bing :	
		bet=true;
		Game.Game_Money+=BingMoney;
		Players[Game.Users[PlayerNum]].Money-=BingMoney;
		strcpy(Bet[PlayerNum],"��");
		if(Players[Game.Users[PlayerNum]].Money <= 0){//����
			Players[Game.Users[PlayerNum]].Money=0;
			strcpy(Bet[PlayerNum],"All - In!");
		}		
		DrawSevenPoker();
		break;	
	case WM_Bet_Die :		
		bet=true;
		Game.Die[PlayerNum]=true;
		strcpy(Bet[PlayerNum],"Die");		
		DrawSevenPoker();
		break;

	case WM_CLOSE:
		DestroyWindow(hMain);
		break;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}


int TalkWinner(int P1, int P2, int P3, int P4,int Users[], HWND hWnd){
	char text[4][200],out[300];
	int P[4]={P1,P2,P3,P4};
	for(int i=0;i<4;i++){

		switch(P[i]/10000){
		case 12: strcpy(text[i],"�ξ� ��Ʈ����Ʈ �÷���");	break;
		case 11: strcpy(text[i],"��Ʈ����Ʈ �÷���");	break;
		case 10: strcpy(text[i],"��ī��");	break;
		case 9: strcpy(text[i],"Ǯ�Ͽ콺");	break;
		case 8:	strcpy(text[i],"�÷���");	break;
		case 7: strcpy(text[i],"����ƾ");	break;
		case 6: strcpy(text[i],"�� ��Ʈ����Ʈ");	break;
		case 5: strcpy(text[i],"��Ʈ����Ʈ");	break;
		case 4: strcpy(text[i],"Ʈ����");	break;
		case 3: strcpy(text[i],"�����");	break;
		case 2: strcpy(text[i],"�����");	break;
		case 1: strcpy(text[i],"����� ");	break;
		}


		switch((P[i]%10000)/100){			
		case 14: sprintf(out,"A ");	break;
		case 13: sprintf(out,"K ");	break;
		case 12: sprintf(out,"Q ");	break;
		case 11: sprintf(out,"J ");	break;
		default : sprintf(out,"%d ", (P[i]%10000)/100);	break;
		}

		if((P[i]/10000)==1){
			strcat(text[i],out);
			strcat(text[i],"ž");
		}

		if((2<=(P[i]/10000) && (P[i]/10000)<=4) || (P[i]/10000)==10 ||  (P[i]/10000)==9){
			strcat(out,text[i]);
			strcpy(text[i],out);
		}

		if((P[i]/10000)==12 || (P[i]/10000)==11 || (P[i]/10000)==8){
			strcpy(out,text[i]);
			switch(P[i]%100){
			case 4: strcpy(text[i],"�����̵� "); break;
			case 3: strcpy(text[i],"���̾� "); break;
			case 2: strcpy(text[i],"��Ʈ "); break;
			case 1: strcpy(text[i],"Ŭ�ι� "); break;
			}
			strcat(text[i],out);
		}

		if(P[i]==-1){//������ �÷��̾�
			strcpy(text[i],"����");
		}
	}
	sprintf(out,"%s : ",Players[Users[0]].Name);
	strcat(out,text[0]);
	sprintf(&out[strlen(out)],"\n%s: ",Players[Users[1]].Name);
	strcat(out,text[1]);
	sprintf(&out[strlen(out)],"\n%s: ",Players[Users[2]].Name);
	strcat(out,text[2]);
	sprintf(&out[strlen(out)],"\n%s: ",Players[Users[3]].Name);
	strcat(out,text[3]);

	int max=0,maxi=-1;
	for(int i=0;i<4;i++){
		if(P[i] > max){
			max=P[i];
			maxi=i;
		}

		else if(P[i]/10000 == 8 && max/10000 == 8){//�Ѵ� �÷���
			if(P[i]%10 > max%10){//���� �켱
				max=P[i];
				maxi=i;
			}
		}
	}
	sprintf(text[0],"\n���� : %s",Players[Users[maxi]].Name);



	strcat(out,text[0]);
	MessageBox(hWnd,out,"���",NULL);

	return maxi;
}


LRESULT CALLBACK SevenPokerBetting(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc,MemDC;
	PAINTSTRUCT ps;
	HBITMAP MyBitmap, OldBitmap;
	char text[200];
	int x,y;

	switch(iMessage) {
	case WM_CREATE:
		//MessageBox(hWnd,"������ �ð�!","���",NULL);

		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);

		MemDC=CreateCompatibleDC(hdc);


		MyBitmap=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Bet_Call));
		OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);
		StretchBlt(hdc,0,0,50,30,MemDC,0,0,50,30,SRCCOPY);

		MyBitmap=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Bet_Half));
		OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);
		StretchBlt(hdc,50,0,50,30,MemDC,0,0,50,30,SRCCOPY);

		MyBitmap=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Bet_Quarter));
		OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);
		StretchBlt(hdc,100,0,50,30,MemDC,0,0,50,30,SRCCOPY);

		MyBitmap=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Bet_Dadang));
		OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);
		StretchBlt(hdc,0,30,50,30,MemDC,0,0,50,30,SRCCOPY);

		if(first){
			MyBitmap=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Bet_Bing));
			OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);
			StretchBlt(hdc,50,30,50,30,MemDC,0,0,50,30,SRCCOPY);

			MyBitmap=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Bet_Check));
			OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);
			StretchBlt(hdc,100,30,50,30,MemDC,0,0,50,30,SRCCOPY);
		}
		else{
			Rectangle(hdc,50,30,100,60);
			Rectangle(hdc,100,30,150,60);
		}
		MyBitmap=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Bet_Die));
		OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);
		StretchBlt(hdc,0,60,150,30,MemDC,0,0,150,30,SRCCOPY);

		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {

			//case ID_BT_SEVEN:
			//break;

		}			

		return 0;
	case WM_LBUTTONDOWN:

		x=LOWORD(lParam);
		y=HIWORD(lParam);
		if(0<=x && x<=50 && 0<=y && y<=30){	//call
			SendMessage(nGame,WM_Bet_Call,0,0);
			DestroyWindow(hWnd);
		}
		else if(50<=x && x<=100 && 0<=y && y<=30){	//Half
			SendMessage(nGame,WM_Bet_Half,0,0);
			DestroyWindow(hWnd);
		}
		else if(100<=x && x<=150 && 0<=y && y<=30){	//Quarter
			SendMessage(nGame,WM_Bet_Quarter,0,0);
			DestroyWindow(hWnd);
		}
		else if(0<=x && x<=50 && 30<=y && y<=60){	//Dadang
			SendMessage(nGame,WM_Bet_Dadang,0,0);
			DestroyWindow(hWnd);
		}
		else if(50<=x && x<=100 && 30<=y && y<=60 && first){	//Check
			SendMessage(nGame,WM_Bet_Check,0,0);
			DestroyWindow(hWnd);
		}
		else if(100<=x && x<=150 && 30<=y && y<=60 && first){	//Bing
			SendMessage(nGame,WM_Bet_Bing,0,0);
			DestroyWindow(hWnd);
		}
		else if(0<=x && x<=150 && 60<=y && y<=90){	//Die
			SendMessage(nGame,WM_Bet_Die,0,0);
			DestroyWindow(hWnd);
		}


		return 0;	

	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}