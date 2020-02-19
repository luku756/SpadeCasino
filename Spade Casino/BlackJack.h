#pragma once

#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "Poker.h"
#include "resource.h"

/*
카드가 10장이면 맥스패로 블랙잭과 같이 친다(1.5배 보상)
애니매이션중에 누르면 스킵

*/

typedef struct _Cards{	//플레이어들의 카드를 보관하는 구조체.
	int Cards[3][10];//카드들의 번호. 최대 2번까지 스플릿가능하다.
	int Score[3];	//카드묶음들의 점수들
	int CardSize[3];	//패묶음당의 카드수
	int PairSize;	//몇개의 패묶음을 가졌는가?
}JACKCARD;

void DrawBlackJack();
LRESULT CALLBACK BlackJackDoing(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);

//세 플레이어의 x좌표
int xs_b[4]={400,400,50,800};
//세 묶음당의 y좌표
int ys_b[3]={650,450,250};
int yp_b[4]={-560,0,0,0};

int PlayerSize;
HBITMAP hBitBLACK;
int GameMode; /*0-카드 배분 1-배분 후(뭐할지 묻기) 2-딜러 카드 한장 오픈 3-결산
			  0-카드 배분
			  11-플레이어 1 행동 ()

			  */
bool ani;//애니매이션 여부
int anix,aniy,anip[8],anis,anic,anit,anipair[8];//애니매이션 x,y, 할 유저들, size,카운터, 횟수,페어 번호

char Doing[4][3][300];
bool Next;

bool BetTime[3];//배팅할 차례인지 알려준다.
bool CanHit[3];//hit 할수 있는지 알려준다.
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
	int Users[4];//Users[0]-딜러, 1- 유저
	JACKCARD Cards[4];
	int CardTable[208];	//4벌의 카드 사용
	int CardNum;
	int Betting[3];//배팅된 돈들.
	int Insuarance[3];	//보험금

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
		Users[0]=Users[1];//딜러는 0번째
		Users[1]=0;
	}

	void ChangeUser(int PlayerNum){
		char text[200];

		sprintf(text,"%s 가 파산했습니다.\n",Players[Users[PlayerNum]].Name);
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

		sprintf(&text[strlen(text)],"%s로 변경합니다.",Players[Users[PlayerNum]].Name);
		MessageBox(nGame, text,"파산!",MB_OK);
	}

	void OutCheck(){
		//아웃된 플레이어, 파산된 플레이어가 있는지 확인한다.
		int cnt=0;
		for(int i=1;i<Player_Size;i++){
			if(Players[Users[i]].Money <= 0){
				Down++;
				if(Mode==1){//무한모드면 복구
					Players[Users[i]].Money=10000000;
				}
			}
		}

		if(Mode ==0){//일반모드면 교체 혹은 사이즈 다운
			for(int i=0;i<10;i++){
				if(Players[i].Money > 0) //생존자 수
					cnt++;
			}

			if(cnt==1){///게임 셋, 무한모드 진입
				MessageBox(nGame,"적 플레이어들을 모두 파산시켰습니다! 무한모드로 진입합니다!","승리!",MB_OK);
				InitUnlimited();
				SelectUsers();
				return ;
			}

			if(cnt < PlayerSize){//이 판을 계속하지 못할 정도
				PlayerSize=cnt;
				MessageBox(nGame, "플레이어 수가 부족합니다. 인원 수를 변경합니다.","모드 변경",MB_OK);
				SelectUsers();
			}
			else{//유지 가능
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

	void MakeCardBox(){	//랜덤한 순서로 카드를 쌓는다.
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
		//해당 플레이어에게 새 패묶음을 준다
		//패묶음의 수를 늘리고 늘어난 패묶음의 장수를 0으로 초기화.
		Cards[PlayerNum].CardSize[Cards[PlayerNum].PairSize++]=0;
	}

	void GiveCard(int PlayerNum, int PairNum){
		//해당 플레이어의 패묶음에 카드를 준다.
		Cards[PlayerNum].Cards[PairNum][Cards[PlayerNum].CardSize[PairNum]++]=CardTable[CardNum++];
	}

	void DealCard(){//처음에 패를 나눠주는 것.
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
					if(Cards[i].Cards[j][k]%100 < 10)	//1~9는 그만큼의 점수
						score += Cards[i].Cards[j][k]%100;
					else//10과 문자는 10의 점수
						score += 10;

					if(Cards[i].Cards[j][k]%100 == 1)//에이스 숫자 기록
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

	void PlayNPC(int PlayerNum){//NPC가 자동으로 배팅한다.
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

	void Split(int PlayerNum, int PairNum){//카드를 나눈다.
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

	GetClientRect(nGame,&crt);	//slot윈도우의 크기에 맞게 사각형 조절
	hdc=GetDC(nGame);	//slot 윈도우의 dc를 가져온다

	if(hBitBLACK==NULL)		//윈도우 크기에 맞게 비트맵 생성
		hBitBLACK=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	hMemDC=CreateCompatibleDC(hdc);	//가상 dc를 정한다
	OldBit=(HBITMAP)SelectObject(hMemDC,hBitBLACK);

	FillRect(hMemDC,&crt,GetSysColorBrush(COLOR_WINDOW));	//hMemDC를 전부 공백으로 한다

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
			sprintf(text,"배팅해주세요. %d ~ %d",Players[0].Money/1000,Players[0].Money/10);
			TextOut(hMemDC,550,540,text,strlen(text));
		}
		if(i == 0)
			sprintf(text,"딜러 %s",Players[GameB.Users[i]].Name);

		else
			sprintf(text,"%s",Players[GameB.Users[i]].Name);
		TextOut(hMemDC,xs_b[i]-(!i*130),535+yp_b[i]+(!i*100),text,strlen(text));

		sprintf(text,"money : %d",Players[GameB.Users[i]].Money);
		TextOut(hMemDC,xs_b[i]-(!i*130),550+yp_b[i]+(!i*100),text,strlen(text));
	}

	DrawSmallBitmap(hMemDC,140,50,0.9,CardBack);

	if(ani){//애니매이션 상태라면
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
		hBt=CreateWindow("button","배팅",WS_CHILD | BS_PUSHBUTTON,650,600,50,25,hWnd,(HMENU)ID_BETBUTTON,g_hInst,NULL);

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


		switch(NowPlayer){//몇번 플레이어인가
		case 1://플레이어
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
	case WM_CALACCOUNT://결과를 계산한다.
		if(GameFin)
			return 0;
		GameFin=true;
		KillTimer(hWnd,1);

		sc=Players[0].Money;
		for(int i=1;i<PlayerSize;i++){
			money=GameB.Betting[i-1]/GameB.Cards[i].PairSize;//패묶음당 배팅된 돈
			for(int j=0;j<GameB.Cards[i].PairSize;j++){

				if(strcmp(Doing[0][0],"BlackJack")==0){//딜러가 블랙잭
					if(strcmp(Doing[i][j],"BlackJack")!=0){//나는 블랙잭 아님
						Players[GameB.Users[i]].Money -= money;
						Players[GameB.Users[0]].Money += money;
						strcpy(Doing[i][j],"Lose");
					}
					else
						strcpy(Doing[i][j],"Draw");
				}
				else{//블랙잭 아님
					if(strcmp(Doing[i][j],"BlackJack")==0){//나는 블랙잭
						Players[GameB.Users[i]].Money += money * 1.5;
						Players[GameB.Users[0]].Money -= money * 1.5;
						strcpy(Doing[i][j],"Win");
					}
				}

				if(strcmp(Doing[0][0],"Bust")==0){//딜러의 버스트
					if(strcmp(Doing[i][j],"Bust")!=0){//나는 안 버스트
						Players[GameB.Users[i]].Money += money;
						Players[GameB.Users[0]].Money -= money;
						strcpy(Doing[i][j],"Win");
					}
					else
						strcpy(Doing[i][j],"Draw");

				}

				if(GameB.Cards[0].Score[0] <= 21){
					if(GameB.Cards[i].Score[j] > GameB.Cards[0].Score[0] && strcmp(Doing[i][j],"Bust")!=0){//버스트 아니고 더 높다
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

		if(Players[GameB.Users[1]].Money <= 0){//파산
			if(MessageBox(hWnd, "돈을 모두 잃으셨습니다. 다시 게임을 시작하시겠습니까?","패배..",MB_YESNO)==IDYES){
					DestroyWindow(hWnd);	//세븐포커는 파괴
					DestroyWindow(hSel);	//선택도 파괴
					SendMessage(hMain,WM_COMMAND,101,0);

				}
				else{
					SendMessage(hWnd,WM_CLOSE,0,0);
				}	
		}

		sc = Players[0].Money - sc;
		if(sc > 0){//돈이 늘었을때, 전적 갱신
			sprintf(text,"%d 원 획득!",sc);
			MessageBox(hWnd, text,"승리!",MB_OK);
			Max_Score=max(Max_Score,sc);
			Max_Money=max(Max_Money,Players[0].Money);
		}
		else{
			sprintf(text,"%d 원을 잃었습니다!",-sc);
			MessageBox(hWnd, text,"패배...",MB_OK);

		}
		GameB.OutCheck();

		if(MessageBox(hWnd, "한번 더 하시겠습니까?","리게임",MB_YESNO)==IDYES){	//한번더 할지 물어보고 한다면 다시 준비한다.
				SendMessage(hWnd,WM_REGAME,0,0);
			}
			else{	//아니라면 게임 선택화면으로 돌아간다.
				ShowWindow(hSel,SW_SHOW);
				DestroyWindow(hWnd);	//세븐포커는 파괴
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
				Doer[0]=CreateWindow("BlackJackDoing","베팅",WS_CHILD | WS_VISIBLE ,600,520,160,120,hWnd,(HMENU)NULL,g_hInst,NULL);
				DrawBlackJack();
			}
			else{
				GameB.Betting[0]=0;
				MessageBox(hWnd,"한도 내의 금액을 배팅해 주세요.","잘좀 쓰세요!",MB_OK);
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
				Doer[1]=CreateWindow("BlackJackDoing","베팅",WS_CHILD | WS_VISIBLE ,600,320,160,90,hWnd,(HMENU)NULL,g_hInst,NULL);
			}
			if(GameB.Cards[1].PairSize == 3){
				MakedDoer=2;
				CanHit[2]=true;
				Doer[2]=CreateWindow("BlackJackDoing","베팅",WS_CHILD | WS_VISIBLE ,600,120,160,90,hWnd,(HMENU)NULL,g_hInst,NULL);
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
			if(ani)//애니매이션 중이면
				return 0;

			if(GameFin)
				return 0;

			if(PlayerTurn){//사용자 입력 대기면
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
		case 2://애니매이션
			anit++;

			if(anit==11){//하나의 카드 애니매이션이 끝나면 다음 카드로
				GameB.GiveCard(anip[anic],anipair[anic]);//카드를 준다
				anit=0;		
				anic++;
				if(anic == anis){//애니매이션 종료	
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
		else if(GameB.Cards[1].Score[PairNum] == 21){ //최상패, 바로 스테이
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