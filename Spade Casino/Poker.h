#pragma once;
#include <Windows.h>
#include <stdio.h>
#include "resource.h"
#include "Player.h"
#include "Draw.h"



int CalScore(int Turn, int getcards[]){

	int score,max_score=0;
	int Cards[7],cnt,mark,mcnt;
	int stpcnt,stptmp[7],base,stpchk[7];
	int plushcnt,plushtmp[7];
	int stcnt,sttmp[7],stchk[7];
	int paircnt[7],pairtmp[7][7],pairsize,pairview;

	//�� ����ŭ �ش�
	for(int i=0;i<Turn;i++)
		Cards[i]=getcards[i];

	for(int i=0, temp;i<Turn;i++){		//�� ������ ����
		for(int j=i;j<Turn;j++){
			if(Cards[i]%100>Cards[j]%100){
				temp=Cards[i];
				Cards[i]=Cards[j];
				Cards[j]=temp;
			}
		}
	}

	//��Ƽ�� üũ
	for(int i=0;i<Turn;i++){
		stpcnt=0;
		stpchk[0]=0;
		stpchk[1]=0;
		stpchk[2]=0;
		stpchk[3]=0;
		stpchk[4]=0;
		mark=Cards[i]/100;
		base=Cards[i]%100;
		for(int j=i;j<Turn;j++){
			if(Cards[j]/100 == mark && Cards[j]%100 == 1 && stpchk[0]==0){//i��°�� j��° ������ ��ġ, ���ڰ� A
				stptmp[stpcnt++]=Cards[j];
				stpchk[0]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 10 && stpchk[1]==0){//i��°�� j��° ������ ��ġ, ���ڰ� 10
				stptmp[stpcnt++]=Cards[j];
				stpchk[1]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 11 && stpchk[2]==0){//i��°�� j��° ������ ��ġ, ���ڰ� J
				stptmp[stpcnt++]=Cards[j];
				stpchk[2]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 12 && stpchk[3]==0){//i��°�� j��° ������ ��ġ, ���ڰ� Q
				stptmp[stpcnt++]=Cards[j];
				stpchk[3]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 13 && stpchk[4]==0){//i��°�� j��° ������ ��ġ, ���ڰ� K
				stptmp[stpcnt++]=Cards[j];
				stpchk[4]=1;
			}
		}
		if(stpcnt > 4){	//5�� �̻�, �� �ξ� ��Ʈ����Ʈ �÷���

			score = 10000*12 + 100*(stptmp[stpcnt-1]%100);
			if(stptmp[0]%100==1){	//���̽� ž
				score = 10000*12 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 12 && mark > max_score%100)	//�÷����� ���ں��� ���� �켱
				max_score=score;
			else
				max_score=max(score,max_score);
		}
	}

	//��Ʈ����Ʈ �÷���
	for(int i=0;i<Turn;i++){
		stptmp[0]=Cards[i];
		stpcnt=1;
		stpchk[0]=0;
		stpchk[1]=0;
		stpchk[2]=0;
		stpchk[3]=0;
		stpchk[4]=0;
		mark=Cards[i]/100;
		base=Cards[i]%100;
		for(int j=i+1;j<Turn;j++){
			if(Cards[j]/100 == mark && Cards[j]%100 == base+1 && stpchk[0]==0){//i��°�� j��° ������ ��ġ, ���ڰ� +1
				stptmp[stpcnt++]=Cards[j];
				stpchk[0]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+2 && stpchk[1]==0){//i��°�� j��° ������ ��ġ, ���ڰ� +2
				stptmp[stpcnt++]=Cards[j];
				stpchk[1]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+3 && stpchk[2]==0){//i��°�� j��° ������ ��ġ, ���ڰ� +3
				stptmp[stpcnt++]=Cards[j];
				stpchk[2]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+4 && stpchk[3]==0){//i��°�� j��° ������ ��ġ, ���ڰ� +4
				stptmp[stpcnt++]=Cards[j];
				stpchk[3]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+5 && stpchk[4]==0){//i��°�� j��° ������ ��ġ, ���ڰ� +5
				stptmp[stpcnt++]=Cards[j];
				stpchk[4]=1;
			}
		}
		if(stpcnt > 4){	//5�� �̻�, �� �ξ� ��Ʈ����Ʈ �÷���

			score = 10000*11 + 100*(stptmp[stpcnt-1]%100);
			if(stptmp[0]%100==1){	//���̽� ž
				score = 10000*11 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 11 && mark > max_score%100)	//�÷����� ���ں��� ���� �켱
				max_score=score;
			else
				max_score=max(score,max_score);
		}
	}

	//�÷���
	for(int i=0;i<Turn;i++){
		plushtmp[0]=Cards[i];
		plushcnt=1;
		mark=Cards[i]/100;
		for(int j=i+1;j<Turn;j++){
			if(Cards[j]/100 == mark){//i��°�� j��° ������ ��ġ
				plushtmp[plushcnt++]=Cards[j];
			}
		}
		if(plushcnt > 4){	//5�� �̻�, �� �÷���

			score = 10000*8 + 100*(plushtmp[plushcnt-1]%100);
			if(plushtmp[0]%100==1){	//���̽� ž
				score = 10000*8 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 8 && mark > max_score%100)	//�÷����� ���ں��� ���� �켱
				max_score=score;
			else
				max_score=max(score,max_score);
		}
	}

	//��Ʈ����Ʈ
	for(int i=0;i<Turn;i++){
		sttmp[0]=Cards[i];
		stcnt=1;
		stchk[0]=0;
		stchk[1]=0;
		stchk[2]=0;
		stchk[3]=0;
		stchk[4]=0;
		base=Cards[i]%100;
		for(int j=i+1;j<Turn;j++){
			if(Cards[j]%100 == base+1 && stchk[0]==0){//i��°�� j��° ���ڰ� +1
				sttmp[stcnt++]=Cards[j];
				stchk[0]=1;
			}
			if(Cards[j]%100 == base+2 && stchk[1]==0){//i��°�� j��° ���ڰ� +2
				sttmp[stcnt++]=Cards[j];
				stchk[1]=1;
			}
			if(Cards[j]%100 == base+3 && stchk[2]==0){//i��°�� j��° ���ڰ� +3
				sttmp[stcnt++]=Cards[j];
				stchk[2]=1;
			}
			if(Cards[j]%100 == base+4 && stchk[3]==0){//i��°�� j��° ���ڰ� +4
				sttmp[stcnt++]=Cards[j];
				stchk[3]=1;
			}
		}
		if(stcnt > 4){	//5�� �̻�, �� ��Ʈ����Ʈ

			score = 10000*5 + 100*(sttmp[stcnt-1]%100) + sttmp[stcnt-1]/100;
			if(sttmp[0]%100==1){	//���̽� ž, �齺Ʈ����Ʈ
				score = 10000*6 + 100*14 + sttmp[0]/100;
			}
			max_score=max(score,max_score);
		}
	}

	//����ƾ
	for(int i=0;i<Turn;i++){
		stcnt=0;
		stchk[0]=0;
		stchk[1]=0;
		stchk[2]=0;
		stchk[3]=0;
		stchk[4]=0;
		for(int j=i;j<Turn;j++){
			if(Cards[j]%100 == 1 && stchk[0]==0){//i��°�� j��° ���ڰ� A
				sttmp[stcnt++]=Cards[j];
				stchk[0]=1;
			}
			if(Cards[j]%100 == 10 && stchk[1]==0){//i��°�� j��° ���ڰ� 10
				sttmp[stcnt++]=Cards[j];
				stchk[1]=1;
			}
			if(Cards[j]%100 == 11 && stchk[2]==0){//i��°�� j��° ���ڰ� 11
				sttmp[stcnt++]=Cards[j];
				stchk[2]=1;
			}
			if(Cards[j]%100 == 12 && stchk[3]==0){//i��°�� j��° ���ڰ� 12
				sttmp[stcnt++]=Cards[j];
				stchk[3]=1;
			}
			if(Cards[j]%100 == 13 && stchk[4]==0){//i��°�� j��°  ���ڰ� 13
				sttmp[stcnt++]=Cards[j];
				stchk[4]=1;
			}
		}
		if(stcnt > 4){	//5�� �̻�, �� ����ƾ

			score = 10000*7 + 100*14 + sttmp[stcnt-1]/100;
			max_score=max(score,max_score);
		}
	}

	//��� �ø���
	pairsize=0;
	paircnt[0]=0;
	paircnt[1]=0;
	paircnt[2]=0;
	paircnt[3]=0;
	paircnt[4]=0;
	paircnt[5]=0;
	paircnt[6]=0;
	for(int i=0;i<Turn;i++){
		for(pairview=0;pairview<pairsize;pairview++){
			if(pairtmp[pairview][0]%100 == Cards[i]%100){
				pairtmp[pairview][paircnt[pairview]++]=Cards[i];
				pairview=-1;
				break;
			}
		}
		if(pairview != -1){
			pairtmp[pairview][paircnt[pairview]++]=Cards[i];
			pairsize++;
		}
	}
	for(int i=0;i<Turn;i++){
		if(paircnt[i] >= 4){	//��ī��			
			score = 10000*10 + 100*(pairtmp[i][3]%100) + 4;
			if(pairtmp[i][0]%100 == 1)
				score = 10000*10 + 100*14 + pairtmp[i][0]/100;
			max_score=max(score,max_score);
		}
		else if(paircnt[i] == 3){	//Ʈ����		
			score = 10000*4 + 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, max( pairtmp[i][1]/100, pairtmp[i][2]/100));
			if(pairtmp[i][0]%100 == 1)
				score = 10000*4 + 100*14 + pairtmp[i][0]/100;
			max_score=max(score,max_score);

			for(int j=0;j<Turn;j++){
				if(paircnt[j] == 2){//Ǯ�Ͽ콺
					score = 10000*9 + 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, max( pairtmp[i][1]/100, pairtmp[i][2]/100));
					if(pairtmp[i][0]%100 == 1)
						score = 10000*9 + 100*14 + pairtmp[i][0]/100;
					max_score=max(score,max_score);
				}
			}
		}
		else if(paircnt[i] == 2){	//�����		
			score = 10000*2 + 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, pairtmp[i][1]/100);
			if(pairtmp[i][0]%100 == 1)
				score = 10000*2 + 100*14 + pairtmp[i][0]/100;

			max_score=max(score,max_score);

			for(int j=0;j<Turn;j++){
				if(i != j && paircnt[j] == 2){//�����
					score = 10000*3;
					if(pairtmp[i][0]%100 > pairtmp[j][0]%100)
						score += 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, pairtmp[i][1]/100);
					else
						score += 100*(pairtmp[j][0]%100) + max(pairtmp[j][0]/100, pairtmp[j][1]/100);
					if(pairtmp[i][0]%100 == 1 || pairtmp[j][0]%100 == 1)
						score = 10000*3 + 100*14 + max(pairtmp[i][0]/100, pairtmp[i][1]/100);


					max_score=max(score,max_score);
				}
			}
		}
	}

	if(max_score == 0){	//�ƹ����͵� �ش� ����, �����
		max_score=10000 + 100*(Cards[Turn-1]%100) + Cards[Turn-1]/100;
		if(Cards[0]%100 == 1)//���̽�
			max_score=10000 + 100*14 + Cards[0]/100;
	}

	return max_score;
	/*
	
��Ƽ��			12	0	
��Ƽ��			11	0	
��ī��			10	0	
Ǯ�Ͽ콺		9	0	
�÷���			8	0	
����ƾ			7	0	
�齺Ʈ����Ʈ	6	0	
��Ʈ����Ʈ		5	0	
Ʈ����			4	0	
�����			3	0	
�����			2	0	
�����			1	0
	*/

}

int CalRecommend(int Turn, int getcards[]){

	int score,max_score=0;
	int Cards[7],cnt,mark,mcnt;
	int stpcnt,stptmp[7],base,stpchk[7];
	int plushcnt,plushtmp[7];
	int stcnt,sttmp[7],stchk[7];
	int paircnt[7],pairtmp[7][7],pairsize,pairview;

	//�� ����ŭ �ش�
	for(int i=0;i<Turn;i++)
		Cards[i]=getcards[i];

	for(int i=0, temp;i<Turn;i++){		//�� ������ ����
		for(int j=i;j<Turn;j++){
			if(Cards[i]%100>Cards[j]%100){
				temp=Cards[i];
				Cards[i]=Cards[j];
				Cards[j]=temp;
			}
		}
	}

	//��Ƽ�� üũ
	for(int i=0;i<Turn;i++){
		stpcnt=0;
		stpchk[0]=0;
		stpchk[1]=0;
		stpchk[2]=0;
		stpchk[3]=0;
		stpchk[4]=0;
		mark=Cards[i]/100;
		base=Cards[i]%100;
		for(int j=i;j<Turn;j++){
			if(Cards[j]/100 == mark && Cards[j]%100 == 1 && stpchk[0]==0){//i��°�� j��° ������ ��ġ, ���ڰ� A
				stptmp[stpcnt++]=Cards[j];
				stpchk[0]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 10 && stpchk[1]==0){//i��°�� j��° ������ ��ġ, ���ڰ� 10
				stptmp[stpcnt++]=Cards[j];
				stpchk[1]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 11 && stpchk[2]==0){//i��°�� j��° ������ ��ġ, ���ڰ� J
				stptmp[stpcnt++]=Cards[j];
				stpchk[2]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 12 && stpchk[3]==0){//i��°�� j��° ������ ��ġ, ���ڰ� Q
				stptmp[stpcnt++]=Cards[j];
				stpchk[3]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 13 && stpchk[4]==0){//i��°�� j��° ������ ��ġ, ���ڰ� K
				stptmp[stpcnt++]=Cards[j];
				stpchk[4]=1;
			}
		}
		if(stpcnt > 4){	//5�� �̻�, �� �ξ� ��Ʈ����Ʈ �÷���

			score = 10000*15 + 100*(stptmp[stpcnt-1]%100);
			if(stptmp[0]%100==1){	//���̽� ž
				score = 10000*15 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 15 && mark > max_score%100)	//�÷����� ���ں��� ���� �켱
				max_score=score;
			else
				max_score=max(score,max_score);
		}
		else if(stpcnt == 4){	//��Ƽ��-4
			score = 10000*8 + 100*(stptmp[3]%100);
			if(stptmp[0]%100==1){	//���̽� ž
				score = 10000*8 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 8 && mark > max_score%100)	//�÷����� ���ں��� ���� �켱
				max_score=score;
			else
				max_score=max(score,max_score);

		}
	}

	//��Ʈ����Ʈ �÷���
	for(int i=0;i<Turn;i++){
		stptmp[0]=Cards[i];
		stpcnt=1;
		stpchk[0]=0;
		stpchk[1]=0;
		stpchk[2]=0;
		stpchk[3]=0;
		stpchk[4]=0;
		mark=Cards[i]/100;
		base=Cards[i]%100;
		for(int j=i+1;j<Turn;j++){
			if(Cards[j]/100 == mark && Cards[j]%100 == base+1 && stpchk[0]==0){//i��°�� j��° ������ ��ġ, ���ڰ� +1
				stptmp[stpcnt++]=Cards[j];
				stpchk[0]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+2 && stpchk[1]==0){//i��°�� j��° ������ ��ġ, ���ڰ� +2
				stptmp[stpcnt++]=Cards[j];
				stpchk[1]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+3 && stpchk[2]==0){//i��°�� j��° ������ ��ġ, ���ڰ� +3
				stptmp[stpcnt++]=Cards[j];
				stpchk[2]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+4 && stpchk[3]==0){//i��°�� j��° ������ ��ġ, ���ڰ� +4
				stptmp[stpcnt++]=Cards[j];
				stpchk[3]=1;
			}
		}
		if(stpcnt > 4){	//5�� �̻�, �� ��Ʈ����Ʈ �÷���

			score = 10000*14 + 100*(stptmp[stpcnt-1]%100);
			if(stptmp[0]%100==1){	//���̽� ž
				score = 10000*14 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 14 && mark > max_score%100)	//�÷����� ���ں��� ���� �켱
				max_score=score;
			else
				max_score=max(score,max_score);
		}
		else if(stpcnt == 4){	//��Ƽ��-4
			score = 10000*8 + 100*(stptmp[3]%100);
			if(stptmp[0]%100==1){	//���̽� ž
				score = 10000*8 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 8 && mark > max_score%100)	//�÷����� ���ں��� ���� �켱
				max_score=score;
			else
				max_score=max(score,max_score);

		}
	}

	//�÷���
	for(int i=0;i<Turn;i++){
		plushtmp[0]=Cards[i];
		plushcnt=1;
		mark=Cards[i]/100;
		for(int j=i+1;j<Turn;j++){
			if(Cards[j]/100 == mark){//i��°�� j��° ������ ��ġ
				plushtmp[plushcnt++]=Cards[j];
			}
		}
		if(plushcnt > 4){	//5�� �̻�, �� �÷���

			score = 10000*11 + 100*(plushtmp[plushcnt-1]%100);
			if(plushtmp[0]%100==1){	//���̽� ž
				score = 10000*11 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 11 && mark > max_score%100)	//�÷����� ���ں��� ���� �켱
				max_score=score;
			else
				max_score=max(score,max_score);
		}
		else if(plushcnt == 4){	//�÷���-4
			score = 10000*6 + 100*(plushtmp[3]%100);
			if(plushtmp[0]%100==1){	//���̽� ž
				score = 10000*6 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 6 && mark > max_score%100)	//�÷����� ���ں��� ���� �켱
				max_score=score;
			else
				max_score=max(score,max_score);

		}
		else if(plushcnt == 3){	//�÷���-3
			score = 10000*3 + 100*(plushtmp[2]%100);
			if(plushtmp[0]%100==1){	//���̽� ž
				score = 10000*3 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 3 && mark > max_score%100)	//�÷����� ���ں��� ���� �켱
				max_score=score;
			else
				max_score=max(score,max_score);
		}
	}

	//��Ʈ����Ʈ
	for(int i=0;i<Turn;i++){
		sttmp[0]=Cards[i];
		stcnt=1;
		stchk[0]=0;
		stchk[1]=0;
		stchk[2]=0;
		stchk[3]=0;
		base=Cards[i]%100;
		for(int j=i+1;j<Turn;j++){
			if(Cards[j]%100 == base+1 && stchk[0]==0){//i��°�� j��° ���ڰ� +1
				sttmp[stcnt++]=Cards[j];
				stchk[0]=1;
			}
			if(Cards[j]%100 == base+2 && stchk[1]==0){//i��°�� j��° ���ڰ� +2
				sttmp[stcnt++]=Cards[j];
				stchk[1]=1;
			}
			if(Cards[j]%100 == base+3 && stchk[2]==0){//i��°�� j��° ���ڰ� +3
				sttmp[stcnt++]=Cards[j];
				stchk[2]=1;
			}
			if(Cards[j]%100 == base+4 && stchk[3]==0){//i��°�� j��° ���ڰ� +4
				sttmp[stcnt++]=Cards[j];
				stchk[3]=1;
			}
		}
		if(stcnt >= 5){	//5�� �̻�, �� ��Ʈ����Ʈ

			score = 10000*10 + 100*(sttmp[stcnt-1]%100) + sttmp[stcnt-1]/100;
			if(sttmp[0]%100==1){	//���̽� ž
				score = 10000*10 + 100*14 + sttmp[0]/100;
			}
			max_score=max(score,max_score);
		}
		else if(stcnt == 4){	//��Ʈ����Ʈ-4
			score = 10000*5 + 100*(sttmp[3]%100) + sttmp[3]/100;
			if(sttmp[0]%100==1){	//���̽� ž
				score = 10000*5 + 100*14 + sttmp[0]/100;
			}
			max_score=max(score,max_score);

		}
		else if(stcnt == 3){	//��Ʈ����Ʈ-3	
			score = 10000*2 + 100*(sttmp[2]%100) + sttmp[2]/100;
			if(sttmp[0]%100==1){	//���̽� ž
				score = 10000*2 + 100*14 + sttmp[0]/100;
			}
			max_score=max(score,max_score);
		}
	}


		//����ƾ
	for(int i=0;i<Turn;i++){
		stcnt=0;
		stchk[0]=0;
		stchk[1]=0;
		stchk[2]=0;
		stchk[3]=0;
		stchk[4]=0;
		base=Cards[i]%100;
		for(int j=i;j<Turn;j++){
			if(Cards[j]%100 == 1 && stchk[0]==0){//i��°�� j��° ���ڰ� +1
				sttmp[stcnt++]=Cards[j];
				stchk[0]=1;
			}
			if(Cards[j]%100 == 10 && stchk[1]==0){//i��°�� j��° ���ڰ� +10
				sttmp[stcnt++]=Cards[j];
				stchk[1]=1;
			}
			if(Cards[j]%100 == 11 && stchk[2]==0){//i��°�� j��° ���ڰ� +11
				sttmp[stcnt++]=Cards[j];
				stchk[2]=1;
			}
			if(Cards[j]%100 == 12 && stchk[3]==0){//i��°�� j��° ���ڰ� +12
				sttmp[stcnt++]=Cards[j];
				stchk[3]=1;
			}
			if(Cards[j]%100 == 13 && stchk[4]==0){//i��°�� j��°  ���ڰ� +13
				sttmp[stcnt++]=Cards[j];
				stchk[4]=1;
			}
		}
		if(stcnt > 4){	//5�� �̻�, �� ����ƾ
			score = 10000*10 + 100*14 + sttmp[stcnt-1]/100;
			max_score=max(score,max_score);
		}
		else if(stcnt == 4){	//��Ʈ����Ʈ-4
			score = 10000*5 + 100*(sttmp[3]%100) + sttmp[3]/100;
			if(sttmp[0]%100==1){	//���̽� ž
				score = 10000*5 + 100*14 + sttmp[0]/100;
			}
			max_score=max(score,max_score);

		}
		else if(stcnt == 3){	//��Ʈ����Ʈ-3	
			score = 10000*2 + 100*(sttmp[2]%100) + sttmp[2]/100;
			if(sttmp[0]%100==1){	//���̽� ž
				score = 10000*2 + 100*14 + sttmp[0]/100;
			}
			max_score=max(score,max_score);
		}
	}


	//��� �ø���
	pairsize=0;
	paircnt[0]=0;
	paircnt[1]=0;
	paircnt[2]=0;
	paircnt[3]=0;
	paircnt[4]=0;
	paircnt[5]=0;
	paircnt[6]=0;
	for(int i=0;i<Turn;i++){
		for(pairview=0;pairview<pairsize;pairview++){
			if(pairtmp[pairview][0]%100 == Cards[i]%100){
				pairtmp[pairview][paircnt[pairview]++]=Cards[i];
				pairview=-1;
				break;
			}
		}
		if(pairview != -1){
			pairtmp[pairview][paircnt[pairview]++]=Cards[i];
			pairsize++;
		}
	}
	for(int i=0;i<Turn;i++){
		if(paircnt[i] >= 4){	//��ī��			
			score = 10000*13 + 100*(pairtmp[i][3]%100) + 4;
			if(pairtmp[i][0]%100 == 1)
				score = 10000*13 + 100*14 + pairtmp[i][0]/100;
			max_score=max(score,max_score);
		}
		else if(paircnt[i] == 3){	//Ʈ����		
			score = 10000*9 + 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, max( pairtmp[i][1]/100, pairtmp[i][2]/100));
			if(pairtmp[i][0]%100 == 1)
				score = 10000*9 + 100*14 + pairtmp[i][0]/100;
			max_score=max(score,max_score);

			for(int j=0;j<Turn;j++){
				if(paircnt[j] == 2){//Ǯ�Ͽ콺
					score = 10000*12 + 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, max( pairtmp[i][1]/100, pairtmp[i][2]/100));
					if(pairtmp[i][0]%100 == 1)
						score = 10000*12 + 100*14 + pairtmp[i][0]/100;
					max_score=max(score,max_score);
				}
			}
		}
		else if(paircnt[i] == 2){	//�����		
			score = 10000*4 + 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, pairtmp[i][1]/100);
			if(pairtmp[i][0]%100 == 1)
				score = 10000*4 + 100*14 + pairtmp[i][0]/100;

			max_score=max(score,max_score);

			for(int j=0;j<Turn;j++){
				if(i != j && paircnt[j] == 2){//�����
					score = 10000*7;
					if(pairtmp[i][0]%100 > pairtmp[j][0]%100)
						score += 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, pairtmp[i][1]/100);
					else
						score += 100*(pairtmp[j][0]%100) + max(pairtmp[j][0]/100, pairtmp[j][1]/100);
					if(pairtmp[i][0]%100 == 1 || pairtmp[j][0]%100 == 1)
						score = 10000*7 + 100*14 + max(pairtmp[i][0]/100, pairtmp[i][1]/100);


					max_score=max(score,max_score);
				}
			}
		}
	}

	if(max_score == 0){	//�ƹ����͵� �ش� ����, �����
		max_score=10000 + 100*(Cards[Turn-1]%100) + Cards[Turn-1]/100;
	}

	return max_score;



	/*

	��Ŀ ����-����

	������ȣ(10000) ž�й�ȣ(100) �����ȣ 

	��Ƽ��			15	0
	��Ƽ��			14	0
	��ī��			13	0
	Ǯ�Ͽ콺		12	0
	�÷���			11	0
	��Ʈ����Ʈ		10	0
	Ʈ����			9	0
	��Ƽ��-4			8	0
	�����			7	0
	�÷���-4			6	0
	��Ʈ����Ʈ-4		5	0
	�����			4	0
	�÷���-3			3	0
	��Ʈ����Ʈ-3		2	0
	�����			1	0

	*/
}

/*
���� ���

�� : �ջ���� ������ �ݾװ� ���� �ݾ��� �����Ѵ�
 
���� : �ջ���� ������ �ݾ��� �ι踦 �����Ѵ�
 
���� : ���� ���õ� �ǵ��� ������ �����Ѵ�
 
���� : ���� ���õ� �ǵ��� 1/4�� ������
 
üũ : �������� �ʰ� ����������� ���ñ����� �ѱ�� (���� ����)
 
�� : �⺻�ǵ��� �����Ѵ� (���� ����)
 
���� : �ش� ���� �����Ѵ� 


*/
/*

��Ŀ ����

			������ȣ(10000) ž�й�ȣ(100) �����ȣ 

��Ƽ��			12	0	
��Ƽ��			11	0	
��ī��			10	0	
Ǯ�Ͽ콺		9	0	
�÷���			8	0	
����ƾ			7	0	
�齺Ʈ����Ʈ	6	0	
��Ʈ����Ʈ		5	0	
Ʈ����			4	0	
�����			3	0	
�����			2	0	
�����			1	0
*/