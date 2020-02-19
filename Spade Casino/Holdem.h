#pragma once

#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "Poker.h"
#include "resource.h"
/*
�߰�����

Ŭ�������� �ִϸ��̼� ����
*/
typedef struct _PLACE{
	int xs[3];
	int ys[3];
	int rs;
}PLACE;

PLACE Place[8]={//�е��� ��ġ�� ȸ�� ������ �����Ѵ�.
	{{400,470,430},{680,680,570},0},
	{{140,205,230},{580,650,560},45},
	{{100,100,160},{320,400,360},90},
	{{180,120,180},{100,170,200},135},
	{{490,400,430},{70,70,150},180},
	{{750,810,690},{100,170,200},225},
	{{830,830,690},{400,320,360},270},
	{{830,770,690},{590,650,560},315},
};

int ps[7][8]={
	{0,4},
	{0,3,5},
	{0,2,4,6},
	{0,2,3,5,6},
	{0,1,3,4,5,7},
	{0,1,2,3,5,6,7},
	{0,1,2,3,4,5,6,7}
};

int Player_Size;	//�÷��̾��� ��.
int Round;	//���� � ��Ȳ����. 
/*
00 - ����ī�� ���,
11 - 1���� ����, 12-2���� ����, 13-3���� ����, 14-4���� ����, 
21 - �ø��ö� ����, 22 - 4��° ����, 23-5��° ����
31 - ���, 32- ��� fold

*/
bool hani;//hero �� (������) �� �����ִ� �ִϸ��̼�.
int hanix,haniy,hanit,hanip,hanir,hanic;
bool pani;
int panix,paniy,panit,panic,panimax;

HBITMAP hBitHoldem,hBitHoldem_Bet;

int Raiser; //�������� ������ �����?
int BetterNum;//���� ������ �����?
char Bets[8][200];
int RaiseCnt;	//�̹� ���忡 ����� ����̳� �ߴ°�? 3���� �ƽ�
bool Open;	//ī�带 �����ϴ����� ����
void DrawHoldem();
void DrawHoldem_Bet();

LRESULT CALLBACK HoldemBetting(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);

#define WM_HOLDEM_BLIND 100000
#define WM_HOLDEM_CHECK 100001
#define WM_HOLDEM_CALL 100002
#define WM_HOLDEM_RAISE 100003
#define WM_HOLDEM_Allin 100004
#define WM_HOLDEM_FOLD 100005
#define WM_HOLDEM_BET 100006
#define WM_HOLDEM_NEXTROUND 100007

//char viewreco[9][200];

bool CanCheck,AllinCheck;
int AllinMoney[8];

class HOLDEM{

public:

	int Users[8];
	int CardTable[52];
	int CardCnt;
	int Cards[8][2];	//�÷��̾��� ��
	int Flops[5];	//���� ��	
	int FlopSize;
	int Score[8];	//���� ������
	int Blind;//����ε� �ݾ�
	int Dealer;//���� ����
	int Game_Money;//�ǵ�
	int Call_Money;//���Ҷ� �����ϴ� ��
	int AllIn_Money;//���ν�, �ɷ����� �ݾ�
	bool Fold[8];//�׾����� ����
	bool Do[8];

	void SelectUser(){
		bool check;
		Users[0]=0;

		for(int i=1;i<Player_Size;i++){
			check=true;

			while(check){
				Users[i]=rand()%9+1;
				if(Players[Users[i]].Money <= 0)
					continue;
				check=false;
				for(int j=1;j<i;j++){
					if(Users[i]==Users[j]){
						check=true;
						break;
					}
				}
			}

		}
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

	void MakeCardBox(){	//������ ������ ī�带 �״´�.
		bool CardView[52];
		memset(CardView,0,sizeof(bool)*52);

		for(int i=0;i<52;i++){
			do{
				CardTable[i]=rand()%52+1;
			}while(CardView[CardTable[i]-1]!=false);
			CardView[CardTable[i]-1]=true;
		}

		for(int i=0;i<52;i++)
			CardTable[i]=100*(((CardTable[i]-1)/13)+1)+((CardTable[i]-1)%13+1);

		CardCnt=0;
	}

	void StartGame(){
		SelectUser();
		MakeCardBox();
		FlopSize=0;
		for(int i=0;i<Player_Size;i++){
			for(int j=0;j<2;j++){
				Cards[i][j]=0;
			}
		//	viewreco[i][0]=0;
		}
		for(int i=0;i<Player_Size;i++){
			Fold[i]=false;
			Bets[i][0]=NULL;
		}
		AllIn_Money=0;
		hani=true;
		hanix=300;
		haniy=200;
		hanit=0;
		hanip=Dealer;
		hanic=0;
		SetTimer(nGame,2,50,0);
	}

	void ReverseFlop(int num){
		Flops[num]=CardTable[CardCnt++];
		FlopSize=num+1;
	}

	void PlayNPC(){
		int num;
		int bet,k;

		bet=AI();

		switch(bet){
		case 0://üũ
			SendMessage(nGame,WM_HOLDEM_CHECK,0,0);
			break;
		case 1://��
			SendMessage(nGame,WM_HOLDEM_CALL,0,0);
			break;
		case 6://����
			SendMessage(nGame,WM_HOLDEM_Allin,0,0);
			break;
		case 7: //����
			SendMessage(nGame,WM_HOLDEM_FOLD,0,0);
			break;
		default://������
			SendMessage(nGame,WM_HOLDEM_RAISE,0,bet-1);
			break;
		}

		//while(1){
		//	num=rand()%100;
		//	if(num<40){
		//		bet=WM_HOLDEM_RAISE;
		//	}
		//	else if(num < 70){
		//		bet=WM_HOLDEM_CALL;
		//	}
		//	else if(num < 85){
		//		bet=WM_HOLDEM_CHECK;
		//	}
		//	else if(num < 99){
		//		bet=WM_HOLDEM_FOLD;
		//	}
		//	else{
		//		bet=WM_HOLDEM_Allin;
		//	}

		//	if(AllinCheck){//���θ���
		//		if(bet!=WM_HOLDEM_Allin && bet!=WM_HOLDEM_FOLD)//�����̳� ���� �ƴϸ� �ٽ�
		//			continue;
		//	}
		//	if(bet==WM_HOLDEM_CHECK && !CanCheck)//check �ȵǴµ� �������
		//		continue;

		//	if(RaiseCnt >= 3 && bet==WM_HOLDEM_RAISE)//3���̻� ����� �õ����� ���
		//		continue;

		//	break;
		//}
		//if(bet==WM_HOLDEM_RAISE)
		//	SendMessage(nGame,bet,0,num/10+1);
		//else
		//	SendMessage(nGame,bet,0,0);
	}

	void Allin(int PlayerNum){
		if(AllIn_Money == 0){//ù ����
			AllIn_Money=Players[Users[PlayerNum]].Money;
			AllinMoney[PlayerNum]=Players[Users[PlayerNum]].Money;
			Players[Users[PlayerNum]].Money=0;
			Game_Money+=AllIn_Money;
			return ;
		}

		if(Players[Users[PlayerNum]].Money <= AllIn_Money){//���αݾ׺��� ���� ���ٸ�
			AllinMoney[PlayerNum]=Players[Users[PlayerNum]].Money;
			AllIn_Money=Players[Users[PlayerNum]].Money;
			Players[Users[PlayerNum]].Money=0;
			Game_Money+=AllIn_Money;

			for(int i=0;i<Player_Size;i++){// �� �����ֱ�
				if(!Fold[i] && AllinMoney[i] >= AllIn_Money){// �� �׾��� ������ �����ߴ�
					Players[Users[i]].Money += (AllinMoney[i] - AllIn_Money);	//������ �����ش�
					Game_Money -= (AllinMoney[i] - AllIn_Money);
					AllinMoney[i] = AllIn_Money;
				}
			}
		}
		else{//ũ�ٸ�
			AllinMoney[PlayerNum] = AllIn_Money;
			Players[Users[PlayerNum]].Money -= AllIn_Money;
			Game_Money += AllIn_Money;
		}

	}

	bool RoundFinCheck(){
		//���尡 �������� Ȯ��. �������� true, �ƴϸ� false ����
		int cnt=0;

		for(int i=0;i<Player_Size;i++){
			if(Do[i])
				cnt++;
		}
		if(cnt==Player_Size)
			return true;

		else
			return false;

	}

	void ResetDo(int PlayerNum){
		//�������� ���������� �� �������Ѵ�.
		if(PlayerNum == -1 && AllinCheck){// ���λ���
			for(int i=0;i<Player_Size;i++){
				Do[i]=true;
			}
			return ;
		}

		for(int i=0;i<Player_Size;i++){
			Do[i]=false;
			if(Fold[i])
				Do[i]=true;
		}
		if(PlayerNum != -1){
			Raiser=PlayerNum;
			Do[PlayerNum]=true;
		}
	}

	int CalHoldem(){
		int tmp[7];
		char out[200];
		for(int i=0;i<Player_Size;i++){
			if(Fold[i]){
				Score[i]=0;
				continue;
			}

			memcpy(tmp,Flops,sizeof(int)*5);
			memcpy(&tmp[5],Cards[i],sizeof(int)*2);
			Score[i]=CalScore(7,tmp);

			switch(Score[i]/10000){
			case 12: strcpy(Bets[i],"�ξ� ��Ʈ����Ʈ �÷���");	break;
			case 11: strcpy(Bets[i],"��Ʈ����Ʈ �÷���");	break;
			case 10: strcpy(Bets[i],"��ī��");	break;
			case 9: strcpy(Bets[i],"Ǯ�Ͽ콺");	break;
			case 8:	strcpy(Bets[i],"�÷���");	break;
			case 7: strcpy(Bets[i],"����ƾ");	break;
			case 6: strcpy(Bets[i],"�� ��Ʈ����Ʈ");	break;
			case 5: strcpy(Bets[i],"��Ʈ����Ʈ");	break;
			case 4: strcpy(Bets[i],"Ʈ����");	break;
			case 3: strcpy(Bets[i],"�����");	break;
			case 2: strcpy(Bets[i],"�����");	break;
			case 1: strcpy(Bets[i],"����� ");	break;
			}


			switch((Score[i]%10000)/100){			
			case 14: sprintf(out,"A ");	break;
			case 13: sprintf(out,"K ");	break;
			case 12: sprintf(out,"Q ");	break;
			case 11: sprintf(out,"J ");	break;
			default : sprintf(out,"%d ", (Score[i]%10000)/100);	break;
			}

			if((Score[i]/10000)==1){
				strcat(Bets[i],out);
				strcat(Bets[i],"ž");
			}

			if((2<=(Score[i]/10000) && (Score[i]/10000)<=4) || (Score[i]/10000)==10 ||  (Score[i]/10000)==9){
				strcat(out,Bets[i]);
				strcpy(Bets[i],out);
			}

			if((Score[i]/10000)==12 || (Score[i]/10000)==11 || (Score[i]/10000)==8){
				strcpy(out,Bets[i]);
				switch(Score[i]%100){
				case 4: strcpy(Bets[i],"�����̵� "); break;
				case 3: strcpy(Bets[i],"���̾� "); break;
				case 2: strcpy(Bets[i],"��Ʈ "); break;
				case 1: strcpy(Bets[i],"Ŭ�ι� "); break;
				}
				strcat(Bets[i],out);
			}

		}

		int max=0,maxi=-1;

		for(int i=0;i<Player_Size;i++){
			if(Score[i] > max){
				max=Score[i];
				maxi=i;
			}
			else if(Score[i]/10000 == 8 && max/10000 == 8){//�Ѵ� �÷���
				if(Score[i]%10 > max%10){//���� �켱
					max=Score[i];
					maxi=i;
				}
			}
		}
		if(max == CalScore(5,Flops)){//�ְ������� �ٷ� ������
			maxi = 0;
			for(int i=0;i<Player_Size;i++){
				if(!Fold[i] && Score[i] == max){
					maxi--;
				}
			}
		}


		return maxi;

	}

	void FoldCheck(){
		int cnt=0;
		for(int i=0;i<Player_Size;i++){
			if(Fold[i])
				cnt++;
		}
		if(cnt==Player_Size-1){
			Round=32;
			SendMessage(nGame,WM_HOLDEM_NEXTROUND,0,0);
		}

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
				SelectUser();
				return ;
			}

			if(cnt < Player_Size){//�� ���� ������� ���� ����
				Player_Size=cnt;
				MessageBox(nGame, "�÷��̾� ���� �����մϴ�. �ο� ���� �����մϴ�.","��� ����",MB_OK);
				SelectUser();
			}
			else{//���� ����
				for(int i=1;i<Player_Size;i++){
					if(Players[Users[i]].Money <= 0){
						ChangeUser(i);
					}
				}

			}

		}


	}

	int AI(){
		//BetterNum �� �и� ���� �����Ͽ� �����Ѵ�.
		enum {a=0,b,c,d,e,f,g,h};
		enum {NoPair=1, Straight_3, Plush_3, OnePair,Straight_4,Plush_4,Twopair,StraightPlush_4,Triple,Straight,Plush,FullHouse,FoueCard,StraightPlush,LoyalStraightPlush};
		int bet, score, flopscore,floplevel;
		int card[7];
		memcpy(card,Cards[BetterNum],sizeof(int)*2);
		memcpy(&card[2],Flops,sizeof(int)*FlopSize);
		score=CalRecommend(FlopSize+2,card);
		flopscore=CalRecommend(FlopSize,Flops);
		floplevel=flopscore/10000;
/*
		switch((score%10000)/100){			
		case 14: sprintf(viewreco[BetterNum],"A ");	break;
		case 13: sprintf(viewreco[BetterNum],"K ");	break;
		case 12: sprintf(viewreco[BetterNum],"Q ");	break;
		case 11: sprintf(viewreco[BetterNum],"J ");	break;
		default : sprintf(viewreco[BetterNum],"%d ", (score%10000)/100);	break;
		}


		if((score/10000)==15 || (score/10000)==14 || (score/10000)==11 || (score/10000)==8 || (score/10000)==6 || (score/10000)==3){
			switch(score%100){
			case 4: strcat(viewreco[BetterNum],"�����̵� "); break;
			case 3: strcat(viewreco[BetterNum],"���̾� "); break;
			case 2: strcat(viewreco[BetterNum],"��Ʈ "); break;
			case 1: strcat(viewreco[BetterNum],"Ŭ�ι� "); break;
			}
		}

		switch(score/10000){
		case 15: strcat(viewreco[BetterNum],"�ξ� ��Ʈ����Ʈ �÷���"); 	break;
		case 14: strcat(viewreco[BetterNum],"��Ʈ����Ʈ �÷���"); 	break;
		case 13: strcat(viewreco[BetterNum],"��ī��"); 	break;
		case 12: strcat(viewreco[BetterNum],"Ǯ�Ͽ콺"); 	break;
		case 11: strcat(viewreco[BetterNum],"�÷���");	break;
		case 10: strcat(viewreco[BetterNum],"��Ʈ����Ʈ");	break;
		case 9: strcat(viewreco[BetterNum],"Ʈ����");	break;
		case 8: strcat(viewreco[BetterNum],"��Ƽ��-4");	break;
		case 7: strcat(viewreco[BetterNum],"�����");	break;
		case 6: strcat(viewreco[BetterNum],"�÷���-4");	break;
		case 5: strcat(viewreco[BetterNum],"��Ʈ����Ʈ-4");	break;
		case 4: strcat(viewreco[BetterNum],"�����");	break;
		case 3: strcat(viewreco[BetterNum],"�÷���-3");	break;
		case 2: strcat(viewreco[BetterNum],"��Ʈ����Ʈ-3");	break;
		case 1: strcat(viewreco[BetterNum],"����� ");	break;
		}

		
		switch((flopscore%10000)/100){			
		case 14: sprintf(viewreco[8],"A ");	break;
		case 13: sprintf(viewreco[8],"K ");	break;
		case 12: sprintf(viewreco[8],"Q ");	break;
		case 11: sprintf(viewreco[8],"J ");	break;
		default : sprintf(viewreco[8],"%d ", (flopscore%10000)/100);	break;
		}


		if((flopscore/10000)==15 || (flopscore/10000)==14 || (flopscore/10000)==11 || (flopscore/10000)==8 || (flopscore/10000)==6 || (flopscore/10000)==3){
			switch(flopscore%100){
			case 4: strcat(viewreco[8],"�����̵� "); break;
			case 3: strcat(viewreco[8],"���̾� "); break;
			case 2: strcat(viewreco[8],"��Ʈ "); break;
			case 1: strcat(viewreco[8],"Ŭ�ι� "); break;
			}
		}

		switch(flopscore/10000){
		case 15: strcat(viewreco[8],"�ξ� ��Ʈ����Ʈ �÷���"); 	break;
		case 14: strcat(viewreco[8],"��Ʈ����Ʈ �÷���"); 	break;
		case 13: strcat(viewreco[8],"��ī��"); 	break;
		case 12: strcat(viewreco[8],"Ǯ�Ͽ콺"); 	break;
		case 11: strcat(viewreco[8],"�÷���");	break;
		case 10: strcat(viewreco[8],"��Ʈ����Ʈ");	break;
		case 9: strcat(viewreco[8],"Ʈ����");	break;
		case 8: strcat(viewreco[8],"��Ƽ��-4");	break;
		case 7: strcat(viewreco[8],"�����");	break;
		case 6: strcat(viewreco[8],"�÷���-4");	break;
		case 5: strcat(viewreco[8],"��Ʈ����Ʈ-4");	break;
		case 4: strcat(viewreco[8],"�����");	break;
		case 3: strcat(viewreco[8],"�÷���-3");	break;
		case 2: strcat(viewreco[8],"��Ʈ����Ʈ-3");	break;
		case 1: strcat(viewreco[8],"����� ");	break;
		}*/

		if(score == flopscore && FlopSize < 5)//�ֻ��п� �����а� ��ġ�ϸ� �縮��
			bet=e;
		else if(score == flopscore && flopscore == 5)	//�����п� �� �ֻ��а� ��ġ�ϸ� ����
			bet=f;
		else{//���� ���� ��
			switch(score/10000){
			case LoyalStraightPlush://��Ƽ��
			case StraightPlush://��Ƽ��
				if(floplevel == StraightPlush || floplevel == StraightPlush_4)//�� �ֻ��пʹ� �ٸ� ��Ƽ��, ��Ƽ��-4
					bet=e;
				else if(floplevel == Straight_4 || floplevel == Plush_4)//�����а� ��Ʈ����Ʈ-4, �÷���-4
					bet=c;
				else if(floplevel == Straight_3 || floplevel == Plush_3)//�����а� ��Ʈ����Ʈ-3, �÷���-3
					bet=b;
				else
					bet=a;

				break;

			case FoueCard://��ī��
				if(floplevel == Triple)
					bet=d;
				else if(floplevel == OnePair)
					bet=a;
				else if(floplevel == Twopair)
					bet=b;
				else if(floplevel == FullHouse)
					bet=e;
				else
					bet=a;
				break;

			case Plush:
				if(floplevel == Plush || floplevel == Plush_4)
					bet=e;
				else if(floplevel == Plush_3)
					bet=b;
				else if(floplevel == Triple)
					bet=c;
				else if(floplevel == Twopair)
					bet=b;
				else
					bet=a;

				break;

			case Triple:
				if(floplevel == OnePair && FlopSize < 4)
					bet=c;
				else if(floplevel == OnePair)
					bet=b;
				else if(floplevel == Straight_4 || floplevel == Plush_4 || StraightPlush_4)
					bet=e;
				else if(floplevel == Straight_3 || floplevel == Plush_3)
					bet=d;
				else
					bet=a;
				break;

			case StraightPlush_4:
				if(floplevel == StraightPlush_4 && FlopSize == 4)
					bet=d;
				else if(floplevel == StraightPlush_4 && FlopSize == 5)
					bet=f;
				else if((floplevel == Straight_3 || floplevel == Plush_3) && FlopSize < 5)
					bet=b;
				else if(floplevel == NoPair && FlopSize < 5)
					bet=b;
				else if(FlopSize = 5)
					bet=f;
				else
					bet=c;

				break;

			case Twopair :
				if(floplevel == Twopair && FlopSize < 4)
					bet=d;
				else if(floplevel == Twopair)
					bet=e;
				else if(floplevel == OnePair && FlopSize < 4)
					bet=c;
				else if(floplevel == OnePair)
					bet=b;
				else if(floplevel == StraightPlush_4 || floplevel == Straight_4 || floplevel == Plush_4)
					bet=e;
				else
					bet=d;

				break;

			case Plush_4:
				if(floplevel == Plush_3 && FlopSize < 5)
					bet=e;
				else if(floplevel == Plush_3)
					bet=f;
				else if(floplevel == Plush_4 && FlopSize < 5)
					bet=e;
				else if(floplevel == NoPair && FlopSize < 5)
					bet=c;
				else if(floplevel == NoPair)
					bet=f;
				else
					bet=d;

				break;

			case Straight_4:
				if(floplevel == Straight_3 && FlopSize < 5)
					bet=e;
				else if(floplevel == Straight_3)
					bet=f;
				else if(floplevel == Straight_4 && FlopSize < 5)
					bet=e;
				else if(floplevel == NoPair && FlopSize < 5)
					bet=c;
				else if(floplevel == NoPair)
					bet=f;
				else
					bet=d;

				break;

			case OnePair:
				if(floplevel == Straight_3 || floplevel == Plush_3)
					bet=c;
				else if(FlopSize < 5)
					bet=h;
				else
					bet=g;
				break;

			case Plush_3:
				if(floplevel == Plush_3 && FlopSize < 4)
					bet=e;
				else if(floplevel == Plush_3)
					bet=f;
				else if(floplevel == Straight_3)
					bet=h;
				else if(FlopSize < 4)
					bet=e;
				else
					bet=g;
				break;

			case Straight_3:
				if(floplevel == Straight_3 && FlopSize < 4)
					bet=e;
				else if(floplevel == Straight_3)
					bet=f;
				else if(floplevel == Plush_3)
					bet=h;
				else if(FlopSize < 4)
					bet=e;
				else
					bet=g;
				break;
			case NoPair:
				if((flopscore % 10000)/100 > 10 && FlopSize < 4)
					bet=b;
				else if((flopscore % 10000)/100 > 10)
					bet=g;
				else if(FlopSize < 4)
					bet=c;
				else
					bet=f;

				break;
			}
		}

		int betting;
		/*
		0-üũ 1-�� 2-������1 3-������2 4-������3 5-������4 6-���� 7-����
		*/
		int bettyp_se[10][7]={
			{6,1,2,3,4,5,6},//a
			{5,1,2,3,4,5},//b
			{3,1,2,3},//c
			{4,0,1,2,3},//d
			{2,0,1},//e
			{1,1},//d
			{3,0,1,7},//e
			{1,7},//f
			{2,1,7},//g
			{4,1,2,3,7}//h
		};
		bool check=true;
		do{
			betting=bettyp_se[bet][rand()%bettyp_se[bet][0]+1];
			check=false;
			if( 2 <= betting && betting <= 5){//�������϶�
				if(RaiseCnt >= 3){//������ƽ� �����ߴٸ�
					check=true;
				}
			}

			if(betting == 0 && !CanCheck ){//üũ�϶� üũ�� �Ұ����ϴٸ�
				check=true;
			}

			if(betting != 6 && AllinCheck){//���θ�絥 ������ �ƴ϶��
				if(betting == 5)//������-4�� �������� �����Ѵ�
					betting = 6;
				else//�� �ܴ� ����
					betting = 7;
			}

		}while(check);

		
		return betting;

	}
};


HOLDEM GameH;	
void DrawHoldem(){

	RECT crt;
	HDC hdc,hMemDC;
	HBITMAP OldBit;
	int CardImg;
	char text[200];

	GetClientRect(nGame,&crt);	//slot�������� ũ�⿡ �°� �簢�� ����
	hdc=GetDC(nGame);	//slot �������� dc�� �����´�

	if(hBitHoldem==NULL)		//������ ũ�⿡ �°� ��Ʈ�� ����
		hBitHoldem=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	hMemDC=CreateCompatibleDC(hdc);	//���� dc�� ���Ѵ�
	OldBit=(HBITMAP)SelectObject(hMemDC,hBitHoldem);

	FillRect(hMemDC,&crt,GetSysColorBrush(COLOR_WINDOW));	//hMemDC�� ���� �������� �Ѵ�

	for(int i=0;i<Player_Size;i++){
		for(int j=0;j<2;j++){

			if(GameH.Cards[i][j] == 0)
				break;

			if(i!=0 && (!Open || GameH.Fold[i]))
				RotateSizingImage(hMemDC,CardBack,Place[ps[Player_Size-2][i]].rs,33,46,Place[ps[Player_Size-2][i]].xs[j],Place[ps[Player_Size-2][i]].ys[j],1,0,0,0);

			else
				RotateSizingImage(hMemDC,PokerCards[(GameH.Cards[i][j]/100)-1][(GameH.Cards[i][j]%100)-1],Place[ps[Player_Size-2][i]].rs,33,46,Place[ps[Player_Size-2][i]].xs[j],Place[ps[Player_Size-2][i]].ys[j],1,0,0,0);

		}

		if(i==GameH.Dealer){
			TextOut(hMemDC,Place[ps[Player_Size-2][i]].xs[2],Place[ps[Player_Size-2][i]].ys[2]-25,"����",4);
		}
		if(i==BetterNum){
			TextOut(hMemDC,Place[ps[Player_Size-2][i]].xs[2],Place[ps[Player_Size-2][i]].ys[2]-50,"*",1);
		}

		if(i==Raiser){
			TextOut(hMemDC,Place[ps[Player_Size-2][i]].xs[2]-20,Place[ps[Player_Size-2][i]].ys[2],"Ra",2);
		}

		sprintf(text,"%d P : %s",i+1,Players[GameH.Users[i]].Name);
		TextOut(hMemDC,Place[ps[Player_Size-2][i]].xs[2],Place[ps[Player_Size-2][i]].ys[2],text,strlen(text));
		sprintf(text,"�� : %d",Players[GameH.Users[i]].Money);
		TextOut(hMemDC,Place[ps[Player_Size-2][i]].xs[2],Place[ps[Player_Size-2][i]].ys[2]+25,text,strlen(text));
		sprintf(text,"%s",Bets[i]);
		TextOut(hMemDC,Place[ps[Player_Size-2][i]].xs[2],Place[ps[Player_Size-2][i]].ys[2]+50,text,strlen(text));
	}

	for(int i=0;i<GameH.FlopSize;i++){
		DrawBitmap(hMemDC,300+i*70,300,PokerCards[(GameH.Flops[i]/100)-1][(GameH.Flops[i]%100)-1]);
	}
	DrawBitmap(hMemDC,300,200,CardBack);

	if(hani){//�ִϸ��̼� ���̶��
		RotateSizingImage(hMemDC,CardBack,hanir,33,46,hanix,haniy,1,0,0,0);
	}

	if(pani){
		DrawBitmap(hMemDC,panix,paniy,CardBack);
	}

	sprintf(text,"Round : %d",Round);
	TextOut(hMemDC,10,10,text,strlen(text));
	sprintf(text,"Blind : %4d /%4d",GameH.Blind/2,GameH.Blind);
	TextOut(hMemDC,10,30,text,strlen(text));
	sprintf(text,"Dealer : %s",Players[GameH.Users[GameH.Dealer]].Name);
	TextOut(hMemDC,10,50,text,strlen(text));

	sprintf(text,"�ǵ� : %d",GameH.Game_Money);
	TextOut(hMemDC,300,400,text,strlen(text));

	if(AllinCheck){//���� ��Ȳ�̶��

		sprintf(text,"Cost : %d",GameH.AllIn_Money);
		TextOut(hMemDC,300,430,text,strlen(text));
	}
	else{

		sprintf(text,"Call : %d",GameH.Call_Money);
		TextOut(hMemDC,300,430,text,strlen(text));
	}
	//TextOut(hMemDC,300,460,viewreco[8],strlen(viewreco[8]));


	SelectObject(hMemDC,OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(nGame,hdc);
	InvalidateRect(nGame,NULL,false);

}

LRESULT CALLBACK HoldemProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT pss;

	char text[200];
	static HANDLE BetTimer;
	static bool bet;//����� �Է� ������ΰ�?
	static bool blind;	//����ε� �´°�?
	static bool first;
	static HWND betwnd;
	int call;
	int win;

	switch(iMessage) {
	case WM_CREATE:
		nGame=hWnd;
		srand(time(NULL));
		GameH.StartGame();
		GameH.Dealer=0;
		GameH.Blind=100;

		for(int i=100;;i*=10){
			if(i*10000 >= Players[0].Money){
				GameH.Blind=i;
				break;
			}
		}

		Round=11;
		CanCheck=true;
		AllinCheck=false;
		BetterNum=GameH.Dealer;//���� ���� ������� ����
		blind=false;
		Open=false;
		SendMessage(hWnd,WM_HOLDEM_BET,0,0);
		return 0;
	case WM_REGAME:

		GameH.StartGame();
		GameH.Dealer++;
		GameH.Dealer%=Player_Size;
		for(int i=100;;i*=10){
			if(i*10000 >= Players[0].Money){
				GameH.Blind=i;
				break;
			}
		}
		Round=11;
		CanCheck=true;
		AllinCheck=false;
		BetterNum=GameH.Dealer;//���� ���� ������� ����
		blind=false;
		Open=false;
		SendMessage(hWnd,WM_HOLDEM_BET,0,0);
		SetTimer(hWnd,1,1000,NULL);
		//DrawHoldem();

		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &pss);
		DrawBitmap(hdc,0,0,hBitHoldem);
		EndPaint(hWnd, &pss);
		return 0;
	case WM_HOLDEM_BET:
		//���� ����

		BetterNum++;
		BetterNum%=Player_Size;

		if(Round == 11 && BetterNum==(GameH.Dealer+1)%Player_Size && !blind){//��������ε�
			SendMessage(hWnd,WM_HOLDEM_BLIND,0,1);
			DrawHoldem();
			return 0;
		}
		else if(Round == 11 && BetterNum==(GameH.Dealer+2)%Player_Size && !blind){//�����ε�
			SendMessage(hWnd,WM_HOLDEM_BLIND,0,2);
			DrawHoldem();
			return 0;
		}

		if(GameH.RoundFinCheck()){//�̹����� ��������
			if(Round %10 < 4){//������ ��������
				Round+=10;
				if(AllinCheck){//���θ�忡�� �� ����-��� ����
					BetterNum=-10;
					Open=true;
				}
			}
			else{//����
				Round=31;				
				DrawHoldem();
			}
			KillTimer(hWnd,1);
			SendMessage(hWnd,WM_HOLDEM_NEXTROUND,0,0);

			DrawHoldem();
			return 0;
		}

		if(GameH.Do[BetterNum]){//�̹� ������ �÷��̾�� (��� ����)
			SendMessage(hWnd,WM_HOLDEM_BET,0,0);
			return 0;
		}


		if(BetterNum==0){//�÷��̾�
			bet=true;
			DestroyWindow(betwnd);
			betwnd=CreateWindow("HoldemBetter","����",WS_CHILD | WS_VISIBLE ,500,400,160,150,hWnd,(HMENU)NULL,g_hInst,NULL);
		}
		else{
			bet=true;
			GameH.PlayNPC();
		}

		DrawHoldem();

		break;
	case WM_HOLDEM_NEXTROUND:
		//���� �Ѿ��. ī�带 �����ְ� �ʱ�ȭ��Ų��.
		if(Round==21){
			panic=0;
			panimax=2;
			Round=12;
		}
		else if(Round==22){
			panic=3;
			panimax=3;
			Round=13;
		}
		else if(Round==23){
			panic=4;
			panimax=4;
			Round=14;
		}
		else if(Round==31){//���� ��
			Open=true;
			win=GameH.CalHoldem();

			if(win >= 0)//���ڰ� �ִٸ�
				sprintf(text,"����: P%d %s",win+1,Players[GameH.Users[win]].Name);
		}
		else if(Round==32){//���� ��, ��� fold

			for(int i=0;i<Player_Size;i++)
				if(!GameH.Fold[i]){
					win=i;
					break;
				}
				sprintf(text,"�� �÷��̾ ���� ������ ����Ǿ����ϴ�.\n����: P%d %s",win+1,Players[GameH.Users[win]].Name);

		}

		if(Round/10 == 1){//���� ����
			panix=300;
			paniy=200;
			pani=true;
			panit=0;
			SetTimer(hWnd,3,50,0);
			SetTimer(hWnd,1,1000,NULL);
			RaiseCnt=0;
			GameH.ResetDo(-1);
			CanCheck=true;
			first=false;
			Raiser=-1;
			BetterNum=GameH.Dealer;
			DrawHoldem();

		}
		else if(Round / 10 ==3){//���� ��
			if(win < 0){
				for(int i=0;i<Player_Size;i++){
					if(!GameH.Fold[i]){
						Players[GameH.Users[i]].Money += GameH.Game_Money/(-win);
					}
				}
			}
			else
				Players[GameH.Users[win]].Money += GameH.Game_Money;

			if(win==0){//���ڰ� �������
				Max_Score=max(Max_Score,GameH.Game_Money);
			}
			Max_Money=max(Max_Money,Players[GameH.Users[0]].Money);
			GameH.Game_Money=0;

			if(Players[0].Money <= 0){//�Ļ�
				if(MessageBox(hWnd, "���� ��� �����̽��ϴ�. �ٽ� ������ �����Ͻðڽ��ϱ�?","�й�..",MB_YESNO)==IDYES){
					DestroyWindow(hWnd);	//Ȧ���� �ı�
					DestroyWindow(hSel);	//���õ� �ı�
					SendMessage(hMain,WM_COMMAND,101,0);
				}
				else{
					SendMessage(hWnd,WM_CLOSE,0,0);
				}
			}

			DrawHoldem();
			if(win==0)
				MessageBox(hWnd, text,"�¸�",MB_OK);
			else if(win < 0)
				MessageBox(hWnd, "���º�! �ǵ��� ���� �����ϴ�.","���º�",MB_OK);
			else
				MessageBox(hWnd, text,"�й�",MB_OK);

			GameH.OutCheck();

			if(MessageBox(hWnd, "�ѹ� �� �Ͻðڽ��ϱ�?","������",MB_YESNO)==IDYES){	//�ѹ��� ���� ����� �Ѵٸ� �ٽ� �غ��Ѵ�.
				SendMessage(hWnd,WM_REGAME,0,0);
			}
			else{	//�ƴ϶�� ���� ����ȭ������ ���ư���.
				ShowWindow(hSel,SW_SHOW);
				DestroyWindow(hWnd);	//������Ŀ�� �ı�
			}

		}
		break;
	case WM_TIMER:
		switch(wParam){
		case 1:
			if(bet)
				return 0;
			if(pani)
				return 0;

			SendMessage(hWnd,WM_HOLDEM_BET,0,0);

			break;
		case 2://�ִϸ��̼�
			hanit++;
			hanir=(Place[ps[Player_Size-2][hanip]].rs)*hanit/10;
			if(hanit == 11){
				GameH.Cards[hanip][hanic / Player_Size]=GameH.CardTable[GameH.CardCnt++];
				hanit=0;
				hanip++;
				hanip%=Player_Size;
				hanic++;
				if(hanic==Player_Size*2){
					KillTimer(hWnd,2);

					SetTimer(hWnd,1,1000,NULL);
					hani=false;
					DrawHoldem();
					break;
				}
			}
			hanix = abs(300 - (300 - Place[ps[Player_Size-2][hanip]].xs[hanic / Player_Size]) * hanit / 10);
			haniy = abs(200 - (200 - Place[ps[Player_Size-2][hanip]].ys[hanic / Player_Size]) * hanit / 10);
			DrawHoldem();

			break;
		case 3: //�ִϸ��̼�
			panit++;
			if(panit == 11){
				GameH.ReverseFlop(panic);
				if(panic==panimax){
					KillTimer(hWnd,3);
					pani=false;
					DrawHoldem();
					break;
				}
				panit=0;
				panic++;
			}
			//DrawBitmap(hMemDC,300+i*70,300,PokerCards[(GameH.Flops[i]/100)-1][(GameH.Flops[i]%100)-1]);

			panix = abs(300 + ( panic * 70 ) * panit / 10);
			paniy = abs(200 + ( 100 * panit / 10 ) );
			DrawHoldem();
			break;
		}
		return 0;

	case WM_HOLDEM_CHECK:
		bet=false;
		first=true;
		GameH.Do[BetterNum]=true;
		strcpy(Bets[BetterNum],"Check");
		break;
	case WM_HOLDEM_BLIND:
		bet=false;
		CanCheck=false;
		switch(lParam){//���ΰ� �����ΰ�
		case 1://����
			GameH.ResetDo(BetterNum);
			if(Players[GameH.Users[BetterNum]].Money <= GameH.Blind/2){//���� ���ڶ�� �������� �ع�����
				SendMessage(hWnd,WM_HOLDEM_Allin,0,0);
				blind=true;
				break;
			}
			Players[GameH.Users[BetterNum]].Money -= GameH.Blind/2;
			GameH.Game_Money += GameH.Blind/2;
			GameH.Call_Money = GameH.Blind/2;
			break;
		case 2://��
			blind=true;
			if(Players[GameH.Users[BetterNum]].Money <= GameH.Blind){//���� ���ڶ�� �������� �ع�����
				SendMessage(hWnd,WM_HOLDEM_Allin,0,0);
				break;
			}
			GameH.ResetDo(BetterNum-1);
			GameH.Do[BetterNum]=true;
			Players[GameH.Users[BetterNum]].Money -= GameH.Blind;
			GameH.Game_Money += GameH.Blind;
			GameH.Call_Money = GameH.Blind;
			break;
		}
		strcpy(Bets[BetterNum],"Blind");
		break;
	case WM_HOLDEM_CALL:

		bet=false;
		GameH.Do[BetterNum]=true;
		if(CanCheck){//���� ������ ������
			GameH.ResetDo(BetterNum);
			GameH.Call_Money=GameH.Blind;
			if(Round == 11 || Round == 12){//�ʹ��̸� �ݰ� �ݶ�
				GameH.Call_Money/=2;
			}
		}
		if(Players[GameH.Users[BetterNum]].Money <= GameH.Call_Money){//���� ���ڶ�� �������� �ع�����
			SendMessage(hWnd,WM_HOLDEM_Allin,0,0);
			break;
		}

		CanCheck=false;

		Players[GameH.Users[BetterNum]].Money -= GameH.Call_Money;
		GameH.Game_Money += GameH.Call_Money;
		strcpy(Bets[BetterNum],"Call");
		break;
	case WM_HOLDEM_RAISE:
		RaiseCnt++;
		GameH.ResetDo(BetterNum);
		bet=false;
		CanCheck=false;

		if(Round == 11 || Round == 12)
			call = GameH.Blind/2;
		else
			call = GameH.Blind;
		call*=lParam;	//������ ������ŭ �����ش�

		GameH.Call_Money += call;


		if(Players[GameH.Users[BetterNum]].Money <= GameH.Call_Money){//���� ���ڶ�� �������� �ع�����
			SendMessage(hWnd,WM_HOLDEM_Allin,0,0);
			break;
		}

		Players[GameH.Users[BetterNum]].Money -= GameH.Call_Money;
		GameH.Game_Money += GameH.Call_Money;
		strcpy(Bets[BetterNum],"Raise");
		break;
	case WM_HOLDEM_Allin:
		if(!AllinCheck)//ù��° ����			
			GameH.ResetDo(BetterNum);

		GameH.Do[BetterNum]=true;
		bet=false;
		AllinCheck=true;
		CanCheck=false;
		strcpy(Bets[BetterNum],"All In");
		GameH.Allin(BetterNum);
		break;
	case WM_HOLDEM_FOLD:
		bet=false;		
		GameH.Do[BetterNum]=true;
		GameH.Fold[BetterNum]=true;
		strcpy(Bets[BetterNum],"Fold");
		GameH.FoldCheck();
		break;
	case WM_CLOSE:
		DestroyWindow(hMain);
		break;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

BOOL CALLBACK SelPlayerDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	static int PlayerCnt;
	switch(iMessage) {
	case WM_INITDIALOG:
		PlayerCnt=0;
		for(int i=0;i<10;i++)
			if(Players[i].Money > 0)
				PlayerCnt++;
		for(int i=8; i>=PlayerCnt; i--){
			EnableWindow(GetDlgItem( hDlg , IDC_BUTTON1-1+i),false);
		}
		return TRUE;
	case WM_COMMAND:
		switch (wParam) {
		case IDC_BUTTON1:
			Player_Size=2;
			EndDialog(hDlg,1);
			return TRUE;
		case IDC_BUTTON2:
			Player_Size=3;
			EndDialog(hDlg,1);
			return TRUE;
		case IDC_BUTTON3:
			Player_Size=4;
			EndDialog(hDlg,1);
			return TRUE;
		case IDC_BUTTON4:
			Player_Size=5;
			EndDialog(hDlg,1);
			return TRUE;
		case IDC_BUTTON5:
			Player_Size=6;
			EndDialog(hDlg,1);
			return TRUE;
		case IDC_BUTTON6:
			Player_Size=7;
			EndDialog(hDlg,1);
			return TRUE;
		case IDC_BUTTON7:
			Player_Size=8;
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

HWND hHolBet;

int Rota;
bool raise,nowani;

void DrawHoldem_Bet(){

	RECT crt;
	HDC hdc,hMemDC,MemDC;
	HBITMAP OldBit;	
	HBITMAP MyBitmap, OldBitmap;
	int k;

	GetClientRect(hHolBet,&crt);	//slot�������� ũ�⿡ �°� �簢�� ����
	hdc=GetDC(hHolBet);	//slot �������� dc�� �����´�

	if(hBitHoldem_Bet==NULL)		//������ ũ�⿡ �°� ��Ʈ�� ����
		hBitHoldem_Bet=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	hMemDC=CreateCompatibleDC(hdc);	//���� dc�� ���Ѵ�
	OldBit=(HBITMAP)SelectObject(hMemDC,hBitHoldem_Bet);
	MemDC=CreateCompatibleDC(hMemDC);

	FillRect(hMemDC,&crt,GetSysColorBrush(COLOR_WINDOW));	//hMemDC�� ���� �������� �Ѵ�

	if(!AllinCheck){//���λ��¸� ���ΰ� ���常 �۵�
		if(CanCheck){
			DrawBitmap(hMemDC,0,0,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Holdem_Check)));
		}
		if(RaiseCnt < 3){
			if(!raise){
				DrawBitmap(hMemDC,0,30,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Holdem_Raise)));
			}
			else{

				DrawBitmap(hMemDC,0,30,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Holdem_Raise_1)));
				DrawBitmap(hMemDC,40,30,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Holdem_Raise_2)));
				DrawBitmap(hMemDC,80,30,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Holdem_Raise_3)));
				DrawBitmap(hMemDC,120,30,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Holdem_Raise_4)));
				if(nowani){//�ִϸ��̼� �߿��� ���׸��� ����Ѵ�
					SelectObject(MemDC, LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Holdem_Raise)));
					BitBlt(hMemDC, 0,30,160-Rota,30,MemDC,0,0,SRCCOPY);
				}
			}
		}

		DrawBitmap(hMemDC,0,60,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Holdem_Call)));
	}
	DrawBitmap(hMemDC,0,90,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Holdem_Allin)));
	DrawBitmap(hMemDC,0,120,LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Holdem_Fold)));


	SelectObject(hMemDC,OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hHolBet,hdc);
	InvalidateRect(hHolBet,NULL,false);
}

LRESULT CALLBACK HoldemBetting(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc,MemDC;
	PAINTSTRUCT ps;
	HBITMAP MyBitmap, OldBitmap;
	char text[200];
	int x,y;

	switch(iMessage) {
	case WM_CREATE:

		hHolBet=hWnd;
		raise=false;
		DrawHoldem_Bet();
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		DrawBitmap(hdc,0,0,hBitHoldem_Bet);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_TIMER:
		Rota++;
		if(Rota >=160){
			nowani=false;
			KillTimer(hWnd,1);
		}
		DrawHoldem_Bet();
		return 0;
	case WM_LBUTTONDOWN:

		x=LOWORD(lParam);
		y=HIWORD(lParam);
		if(0<=x && x<=160 && 0<=y && y<=30 && CanCheck && !AllinCheck){	//Check
			SendMessage(nGame,WM_HOLDEM_CHECK,0,0);
			DestroyWindow(hWnd);
		}
		else if(0<=x && x<=160 && 30<=y && y<=60 && !raise && !AllinCheck && RaiseCnt < 3){	//raise
			raise=true;
			nowani=true;
			Rota=0;
			DrawHoldem_Bet();
			SetTimer(hWnd,1,10,NULL);
		}
		else if(0<=x && x<=40 && 30<=y && y<=60 && raise){	//raise_1
			SendMessage(nGame,WM_HOLDEM_RAISE,0,1);
			DestroyWindow(hWnd);
		}
		else if(40<=x && x<=80 && 30<=y && y<=60 && raise){	//raise_2
			SendMessage(nGame,WM_HOLDEM_RAISE,0,2);
			DestroyWindow(hWnd);
		}
		else if(80<=x && x<=120 && 30<=y && y<=60 && raise){	//raise_3
			SendMessage(nGame,WM_HOLDEM_RAISE,0,3);
			DestroyWindow(hWnd);
		}
		else if(120<=x && x<=160 && 30<=y && y<=60 && raise){	//raise_4
			SendMessage(nGame,WM_HOLDEM_RAISE,0,4);
			DestroyWindow(hWnd);
		}
		else if(0<=x && x<=160 && 60<=y && y<=90 && !AllinCheck){	//call
			SendMessage(nGame,WM_HOLDEM_CALL,0,0);
			DestroyWindow(hWnd);
		}
		else if(0<=x && x<=160 && 90<=y && y<=120){	//all-in
			SendMessage(nGame,WM_HOLDEM_Allin,0,0);
			DestroyWindow(hWnd);
		}

		else if(0<=x && x<=160 && 120<=y && y<=150 ){	//fold
			SendMessage(nGame,WM_HOLDEM_FOLD,0,0);
			DestroyWindow(hWnd);
		}


		return 0;	

	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}