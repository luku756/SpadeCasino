#pragma once

#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "Poker.h"
#include "resource.h"


/*
수정할 사항

플레이어가 다이했을 때 속도 가속.
남은 플레이어 수가 4 미만일때 게임 가능하게 하기.

올인이 조금 이상하다. 손보자.
누군가 올인했을때 앞으로 아무도 배팅이 없게 하는게 좋겠다.

클릭했을때 애니매이션 생략

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
int selectopen; //보여줄 카드 고르기. 0=카드분배, 1=카드선택, 2=카드선택 완료

void DrawSevenPoker();
int TalkWinner(int P1, int P2, int P3, int P4,int Users[], HWND hWnd);

LRESULT CALLBACK SevenPokerBetting(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);

int BingMoney;
int Canix,Caniy,Canip,Canit,Canir;
bool finani[4];
char Bet[4][200];	//해당 플레이어가 뭘 배팅했는지 출력해주기 위한 문자열. 
int GameTurn;	//한번 진행될 때마다 쌓인다. 5번째 = 카드 4, 10번째 = 카드 5, 15번째 = 카드 6, 20번째 = 카드 7, 25번째 = 게임 셋
bool Animation;	//애니매이션 중인지 아닌지 확인한다.

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

	void SelectUser(){	//플레이어 선택

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
		if(Mode==0){//무한모드가 아닐때
			int cnt=0;

			for(int i=1;i<10;i++){
				if(Players[i].Money > 0)
					cnt++;
			}
			if(cnt < 3){//남은 플레이어가 3명이 안될때
				Users[LastPlayer]=-1;
				return;
			}
		}
		else{//무한모드라면, 올인 복구
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

	void MakeCardBox(){	//랜덤한 순서로 카드를 쌓는다.
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

	void GiveCard(int turn, int Starter){	//Starter부터 한장씩 카드를 나눠준다.
		for(int i=Starter,j=0;j<4;j++,i++,i%=4){
			if(!Die[i])	//죽지 않았어야 카드를 준다
				Cards[i][turn]=CardTable[++CardNum];
		}
	}

	void GameStart(){//게임 시작, 초기화
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

		for(int i=0;i<4;i++){	//네 플레이어의 각각 패들의 경우의 수를 계산하여 가장 높은 패를 찾는다
			P[i]=CalScore(Turn,Cards[i]);

			if(Die[i]){	//만약 이 플레이어가 다이라면?
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

	int AI(int PlayerNum){	//패들을 보고 배팅을 결정
		enum {a=0,b,c,d,e,f,g,h,i};
		int bet;
		switch(score[PlayerNum]/10000){
		case 15://로티플
		case 14://스티플
			bet=a;
			break;
		case 13://포카드
			bet=a;
			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 > score[PlayerNum]/10000){//나보다 높다
						bet=i;
						break;
					}
					if(view[i]/10000 == 8)//스티플-4
						bet=b;
				}
			}
			break;
		case 12://풀하우스
			bet=a;
			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//포카드
						bet=i;
						break;
					}
					else if(view[i]/10000 == 8)//스티플-4
						bet=max(c,bet);
					else if((view[i]/10000 == 9 || view[i]/10000 == 7) && (view[i]%10000)/100 > (score[PlayerNum]%10000)/100) //숫자가 높은 트리플,투페어
						bet=max(b,bet);
				}
			}
			break;
		case 11://플러시
			bet=a;			
			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//포카드
						bet=i;
						break;
					}
					else if(view[i]/10000 == 8)//스티플-4
						bet=max(c,bet);
					else if(view[i]/10000 == 9 || view[i]/10000 == 7) //숫자가 높은 트리플,투페어
						bet=max(b,bet);
				}
			}
			break;
		case 10://스트레이트
			bet=b;

			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//포카드
						bet=i;
						break;
					}
					else if(view[i]/10000 == 9 || view[i]/10000 == 8 || view[i]/10000 == 7 || view[i]/10000 == 6 || (view[i]/10000 == 5 && (view[i]%10000)/100 > (score[PlayerNum]%10000)/100)) //트리플, 스티플-4,플러시-4, 높은 스트레이트-4,투페어
						bet=max(g,bet);
					else if(view[i]/10000 == 4 || view[i]/10000 == 3 || view[i]/10000 == 2)//원페어, 스트레이트-3, 플러시-3
						bet=max(d,bet);
				}
			}
			break;
		case 9://트리플

			switch(Turn){
			case 7: bet=b;
				for(int i=0;i<4;i++){
					if(i!=PlayerNum){
						if(view[i]/10000 > score[PlayerNum]/10000){//보다 크면
							bet=i;
							break;
						}
						else if(view[i]/10000 == 9 || view[i]/10000 == 8 || view[i]/10000 == 7 || view[i]/10000 == 6 || view[i]/10000 == 5 ) //트리플, 스티플-4,플러시-4, 높은 스트레이트-4,투페어
							bet=max(c,bet);
					}
				}
				break;
			case 6: bet=b;
				for(int i=0;i<4;i++){
					if(i!=PlayerNum){
						if(view[i]/10000 > score[PlayerNum]/10000){//보다 크면
							bet=h;
							break;
						}
					}
				}
				break;
			default: bet=a;
				for(int i=0;i<4;i++){
					if(i!=PlayerNum){
						if(view[i]/10000 > score[PlayerNum]/10000){//보다 크면
							bet=h;
							break;
						}
					}
				}
				break;
			}
			break;
		case 8: //스티플-4
			switch(Turn){
			case 7: bet=i; break;
			case 6: bet=a; 				
				for(int i=0;i<4;i++){
					if(i!=PlayerNum){
						if(view[i]/10000 == 13){//포카드
							bet=e;
							break;
						}
					}
				}				
				break;
			default: bet=a;	break;
			}
			break;
		case 7://투페어
			switch(Turn){
			case 7: bet=a; 

				for(int i=0;i<4;i++){
					if(i!=PlayerNum){
						if(view[i]/10000 == 13){//포카드
							bet=i;
							break;
						}
						else if(view[i]/10000 == 9 || view[i]/10000 == 8 || view[i]/10000 == 7 || view[i]/10000 == 6 || view[i]/10000 == 5) //트리플, 스티플-4,플러시-4, 스트레이트-4,투페어
							bet=max(g,bet);
						else if(view[i]/10000 == 4 || view[i]/10000 == 3 || view[i]/10000 == 2)//원페어, 스트레이트-3, 플러시-3
							bet=max(d,bet);
					}
				}
				break;
			case 6: bet=b; break;
			default : bet=a; break;
			}
			break;
		case 6://플러시-4
			if(Turn==7){
				bet=i;
				break;
			}
			bet=a;
			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//포카드
						bet=i;
						break;
					}
				}
			}
			break;
		case 5://스트레이트-4

			if(Turn==7){
				bet=i;
				break;
			}
			bet=a;
			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//포카드
						bet=i;
						break;
					}
				}
			}
			break;
		case 4://원페어
			bet=e;
			if(Turn==7 && ((score[PlayerNum]%10000)/100 == 14 || (score[PlayerNum]%10000)/100 == 11 || (score[PlayerNum]%10000)/100 == 12 || (score[PlayerNum]%10000)/100 == 13 ))//상위 원페어
				bet=d;
			if(Turn <= 6)
				bet=d;

			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//포카드
						bet=i;
						break;
					}
				}
			}
			break;
		case 3://플러시-3
			if(Turn>=6)
				bet=i;
			else
				bet=e;

			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//포카드
						bet=i;
						break;
					}
				}
			}
			break;
		case 2://스트레이트-3

			if(Turn>=6)
				bet=i;
			else if(Turn==5)
				bet=f;
			else
				bet=e;

			for(int i=0;i<4;i++){
				if(i!=PlayerNum){
					if(view[i]/10000 == 13){//포카드
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
			if(PlayerNum != Dealer){//만일 딜러가 아니라면
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

	GetClientRect(nGame,&crt);	//slot윈도우의 크기에 맞게 사각형 조절
	hdc=GetDC(nGame);	//slot 윈도우의 dc를 가져온다

	if(hBitSeven==NULL)		//윈도우 크기에 맞게 비트맵 생성
		hBitSeven=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	hMemDC=CreateCompatibleDC(hdc);	//가상 dc를 정한다
	OldBit=(HBITMAP)SelectObject(hMemDC,hBitSeven);

	FillRect(hMemDC,&crt,GetSysColorBrush(COLOR_WINDOW));	//hMemDC를 전부 공백으로 한다

	for(int j=0;j<4;j++){
		if(Game.Die[j]){//해당 플레이어가 다이했을 때
			for(int i=0;Game.Cards[j][i]!=0 && i < Game.Turn ;i++){
				//뒷면 카드만 보여주도록 한다.
				RotateSizingImage(hMemDC,CardBack,90*j,10,10,30+i*xp_s[j]+xs_s[j],30+i*yp_s[j]+ys_s[j],1,0,0,0);
			}
		}
		else{ //살아있을 때
			for(int i=0;i<Game.Turn;i++){
				if(i==Game.Turn-1 && !finani[j]){//애니매니션 안됬다면
					break;
				}

				if(j!=0 && (i==0 || i==1 || i==6 || (selectopen!=2 && i==2)) && GameTurn!=25 )	//npc의 1,2,7번째 카드는 보여주지 않는다
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
			TextOut(hMemDC,xt[j],yt[j]-50,"딜러",4);
		}
		sprintf(text,"%s",Players[Game.Users[j]].Name);
		TextOut(hMemDC,xt[j],yt[j]-30,text,strlen(text));

		sprintf(text,"돈 : %d",Players[Game.Users[j]].Money);
		TextOut(hMemDC,xt[j],yt[j]-10,text,strlen(text));
		TextOut(hMemDC,xt[j]+100,yt[j]-10,Bet[j],strlen(Bet[j]));

	}

	sprintf(text,"판돈 : %d",Game.Game_Money);

	TextOut(hMemDC,300,300,text,strlen(text));
	sprintf(text,"콜 금액 : %d",Game.Call_Money);
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
		TextOut(hMemDC,30+xp_s[0]+xs_s[0],yp_s[0]+ys_s[0],"보여줄 카드를 선택하세요",24);
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
	static bool bet;	//bet = 플레이어 턴으로, 배팅 윈도우가 켜졌을 때. 배팅값을 리턴하기 전까진 다음 턴으로 가지 않는다.
	static bool NewCard;	//NewCard = 다음 턴이 딜러 턴일 때, 새 카드를 나눠준다.
	static int PlayerNum;	//현재 턴인 플레이어의 번호.
	static HANDLE BetTimer;
	static bool GameFin;	//게임이 진행중일 때는 false, 끝났으면 true
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
		if(selectopen==1){//카드 선택

			x=LOWORD(lParam);
			y=HIWORD(lParam);
			if(20+xs_s[0] <= x && x <= 86+xs_s[0] && 20+ys_s[0] <= y && y <=  113+ys_s[0]){//첫번째 패
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
			Game.GiveCard(Game.Turn++,0);	//카드 분배
			if(Game.Turn == 3){

				Game.Recommend();
				KillTimer(hWnd,1);
				selectopen=1;
			}
			return 0;
		}

		GameTurn++;	//한 턴 진행

		//다이가 셋이면 자동으로 남은이가 승리
		cnt=0;
		for(int i=0;i<4;i++){
			if(Game.Die[i])
				cnt++;
			else
				DieWin=i;
		}
		if(cnt==3){	//다이가 셋
			KillTimer(hWnd,1);	//게임이 끝났으므로 타이머는 제거한다.
			sprintf(text,"승자: %s!",Players[Game.Users[DieWin]].Name);
			MessageBox(hWnd, text,"결과",NULL);
			Players[Game.Users[DieWin]].Money+=Game.Game_Money;	//승자에게 돈을 추가한다
			Game.Game_Money=0;	//배팅되었던 돈을 초기화
			GameFin=true;	//게임 종료
			if(Players[0].Money <= 0){
				if(MessageBox(hWnd, "돈을 모두 잃으셨습니다. 다시 게임을 시작하시겠습니까?","패배..",MB_YESNO)==IDYES){
					DestroyWindow(hWnd);	//세븐포커는 파괴
					DestroyWindow(hSel);	//선택도 파괴
					SendMessage(hMain,WM_COMMAND,101,0);
				}
				else{
					SendMessage(hWnd,WM_CLOSE,0,0);
				}
			}

			for(int i=1;i<4;i++){
				if(Players[Game.Users[i]].Money <= 0){
					sprintf(text,"%s 패배!!",Players[Game.Users[i]].Name);
					Game.ChangeUser(i);
					Down++;
					if(Game.Users[i] == -1){//진행불가
						MessageBox(hWnd,"남은 플레이어가 4명 미만입니다. 다른 게임을 이용해주세요.","승리!",MB_OK);

						ShowWindow(hSel,SW_SHOW);
						DestroyWindow(hWnd);	//세븐포커는 파괴
					}
					else{
						sprintf(&text[strlen(text)],"\n%s 로 변경합니다!",Players[Game.Users[i]].Name);
						MessageBox(hWnd, text,"결과",MB_OK);
					}
				}
			}

			if(MessageBox(hWnd, "한번 더 하시겠습니까?","리게임",MB_YESNO)==IDYES){	//한번더 할지 물어보고 한다면 다시 준비한다.
				Game.Dealer=DieWin;
				SendMessage(hWnd,WM_REGAME,0,0);
			}
			else{	//아니라면 게임 선택화면으로 돌아간다.
				ShowWindow(hSel,SW_SHOW);
				DestroyWindow(hWnd);	//세븐포커는 파괴
			}
			return 0;
		}

		if(GameTurn==25){	//게임 셋.

			DrawSevenPoker();
			KillTimer(hWnd,1);	//게임이 끝났으므로 타이머는 제거한다.
			int winer=Game.CalSevenPoker(hWnd);	//패들을 계산하여 승자를 가린다.

			if(winer==0){	//승자가 0번, 즉 플레이어일 경우 기록을 갱신한다.
				Max_Score=max(Max_Score,Game.Game_Money);
				Max_Money=max(Max_Money,Players[0].Money+Game.Game_Money);
			}

			Players[Game.Users[winer]].Money+=Game.Game_Money;	//승자에게 돈을 추가한다
			Game.Game_Money=0;	//배팅되었던 돈을 초기화
			GameFin=true;	//게임 종료

			if(Players[0].Money <= 0){
				if(MessageBox(hWnd, "돈을 모두 잃으셨습니다. 다시 게임을 시작하시겠습니까?","패배..",MB_YESNO)==IDYES){
					DestroyWindow(hWnd);	//세븐포커는 파괴
					DestroyWindow(hSel);	//선택도 파괴
					SendMessage(hMain,WM_COMMAND,101,0);

				}
				else{
					SendMessage(hWnd,WM_CLOSE,0,0);
				}
			}
			for(int i=1;i<4;i++){
				if(Players[Game.Users[i]].Money <= 0){
					sprintf(text,"%s 패배!!",Players[Game.Users[i]].Name);
					Game.ChangeUser(i);
					Down++;
					if(Game.Users[i] == -1){//일반모드 엔딩
						MessageBox(hWnd,"적 플레이어들을 파산시켰습니다! 무한모드로 진입합니다!","승리!",MB_OK);
						InitUnlimited();
						Game.SelectUser();
					}
					else{
						sprintf(&text[strlen(text)],"\n%s 로 변경합니다!",Players[Game.Users[i]].Name);
						MessageBox(hWnd, text,"결과",MB_OK);
					}
				}
			}

			if(MessageBox(hWnd, "한번 더 하시겠습니까?","리게임",MB_YESNO)==IDYES){	//한번더 할지 물어보고 한다면 다시 준비한다.
				Game.Dealer=winer;
				SendMessage(hWnd,WM_REGAME,0,0);
			}
			else{	//아니라면 게임 선택화면으로 돌아간다.
				ShowWindow(hSel,SW_SHOW);
				DestroyWindow(hWnd);	//세븐포커는 파괴
			}
			return 0;
		}
		else if(GameTurn > 0 && GameTurn%5==0){	// 5단위 배수, 카드를 주는 턴.
			Canip=Game.Dealer-1;
			//Canip=3;
			Canit=-1;
			Animation=true;
			finani[0]=false;
			finani[1]=false;
			finani[2]=false;
			finani[3]=false;
			SetTimer(hWnd,2,25,NULL);
			Game.GiveCard(Game.Turn++,0);	//카드 분배
			for(int i=0;i<4;i++){
				if(!Game.Die[i] && Players[Game.Users[i]].Money > 0){	//이 플레이어가 죽지 않았고, 올인도 아니라면
					Bet[i][0]=0;	//배팅기록 초기화
				}
			}
			Game.Recommend();
			DrawSevenPoker();

		}
		else{	//카드배정도 끝도 아닌, 플레이어들의 배팅 턴
			PlayerNum++;	//다음 플레이어로 넘어간다
			PlayerNum%=4;
			if(!Game.Die[PlayerNum] && Players[Game.Users[PlayerNum]].Money > 0){	//현재 플레이어가 다이하지 않고, 올인도 아닐 경우에만 진행
				if(PlayerNum==0){	//유저의 턴, 배팅 차일드를 생성한다

					if(Game.Dealer == 0)
						first=true;
					else
						first=false;
					bet=false;	//게임이 멋대로 진행되지 않도록 한다.
					CreateWindow("SevenPokerBetter","베팅",WS_CHILD | WS_VISIBLE ,500,500,150,90,hWnd,(HMENU)NULL,g_hInst,NULL);	//배팅 차일드 생성

					DrawSevenPoker();
				}
				else{	//npc들의 턴
					Game.PlayNPC(hWnd,PlayerNum);	//자동으로 진행
				}
			}
			else
				SendMessage(hWnd,WM_NEXTTURN,0,0);
		}
		return 0;	

	case WM_TIMER:
		switch(wParam){
		case 1:	//게임 진행 타이머
			if(GameFin)	//만약 게임이 끝났다면
				return 0;	//들어가지 않는다

			if(!bet)	//만약 현재 유저의 배팅 대기중이라면
				return 0;	//들어가지 않는다

			if(Animation)//애니매이션 중이라면 들어가지 않는다
				return 0;

			if(selectopen == 1)//카드 오픈이 안되었다면
				return 0;

			SendMessage(hWnd,WM_NEXTTURN,0,0);
			break;
		case 2://카드 애니매이션 타이머
			x=430 - 3*((52-Game.CardNum)/4);
			y=350 - 3*((52-Game.CardNum)/4);
			Canir=(Canip*90)*Canit/10;
			Canit++;
			if(Canit==11){
				Canit=0;
				finani[Canip]=true;				
			}
			if(Canit==0){//첫 시작이라면
				while(1){
					Canip++;
					Canip%=4;
					if(finani[Canip]){//한바퀴 돌았다면
						Canip=-1;
						Animation=false;
						KillTimer(hWnd,2);
						break;
					}
					if(Game.Die[Canip]){//다이한 플레이어라면
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
		strcpy(Bet[PlayerNum],"콜");
		if(Players[Game.Users[PlayerNum]].Money <= 0){//올인
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
		strcpy(Bet[PlayerNum],"하프");
		if(Players[Game.Users[PlayerNum]].Money <= 0){//올인
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
		strcpy(Bet[PlayerNum],"쿼터");
		if(Players[Game.Users[PlayerNum]].Money <= 0){//올인
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
		strcpy(Bet[PlayerNum],"따당");
		if(Players[Game.Users[PlayerNum]].Money <= 0){//올인
			Players[Game.Users[PlayerNum]].Money=0;
			strcpy(Bet[PlayerNum],"All - In!");
		}		
		DrawSevenPoker();
		break;	
	case WM_Bet_Check :	
		bet=true;
		strcpy(Bet[PlayerNum],"체크");
		if(Players[Game.Users[PlayerNum]].Money <= 0){//올인
			Players[Game.Users[PlayerNum]].Money=0;
			strcpy(Bet[PlayerNum],"All - In!");
		}		
		DrawSevenPoker();
		break;	
	case WM_Bet_Bing :	
		bet=true;
		Game.Game_Money+=BingMoney;
		Players[Game.Users[PlayerNum]].Money-=BingMoney;
		strcpy(Bet[PlayerNum],"삥");
		if(Players[Game.Users[PlayerNum]].Money <= 0){//올인
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
		case 12: strcpy(text[i],"로얄 스트레이트 플러쉬");	break;
		case 11: strcpy(text[i],"스트레이트 플러쉬");	break;
		case 10: strcpy(text[i],"포카드");	break;
		case 9: strcpy(text[i],"풀하우스");	break;
		case 8:	strcpy(text[i],"플러쉬");	break;
		case 7: strcpy(text[i],"마운틴");	break;
		case 6: strcpy(text[i],"백 스트레이트");	break;
		case 5: strcpy(text[i],"스트레이트");	break;
		case 4: strcpy(text[i],"트리플");	break;
		case 3: strcpy(text[i],"투페어");	break;
		case 2: strcpy(text[i],"원페어");	break;
		case 1: strcpy(text[i],"노페어 ");	break;
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
			strcat(text[i],"탑");
		}

		if((2<=(P[i]/10000) && (P[i]/10000)<=4) || (P[i]/10000)==10 ||  (P[i]/10000)==9){
			strcat(out,text[i]);
			strcpy(text[i],out);
		}

		if((P[i]/10000)==12 || (P[i]/10000)==11 || (P[i]/10000)==8){
			strcpy(out,text[i]);
			switch(P[i]%100){
			case 4: strcpy(text[i],"스페이드 "); break;
			case 3: strcpy(text[i],"다이아 "); break;
			case 2: strcpy(text[i],"하트 "); break;
			case 1: strcpy(text[i],"클로버 "); break;
			}
			strcat(text[i],out);
		}

		if(P[i]==-1){//다이한 플레이어
			strcpy(text[i],"다이");
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

		else if(P[i]/10000 == 8 && max/10000 == 8){//둘다 플러시
			if(P[i]%10 > max%10){//문양 우선
				max=P[i];
				maxi=i;
			}
		}
	}
	sprintf(text[0],"\n승자 : %s",Players[Users[maxi]].Name);



	strcat(out,text[0]);
	MessageBox(hWnd,out,"결과",NULL);

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
		//MessageBox(hWnd,"배팅의 시간!","기록",NULL);

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