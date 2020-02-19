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

	//패 수만큼 준다
	for(int i=0;i<Turn;i++)
		Cards[i]=getcards[i];

	for(int i=0, temp;i<Turn;i++){		//패 순서로 정렬
		for(int j=i;j<Turn;j++){
			if(Cards[i]%100>Cards[j]%100){
				temp=Cards[i];
				Cards[i]=Cards[j];
				Cards[j]=temp;
			}
		}
	}

	//로티플 체크
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
			if(Cards[j]/100 == mark && Cards[j]%100 == 1 && stpchk[0]==0){//i번째와 j번째 문양이 일치, 숫자가 A
				stptmp[stpcnt++]=Cards[j];
				stpchk[0]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 10 && stpchk[1]==0){//i번째와 j번째 문양이 일치, 숫자가 10
				stptmp[stpcnt++]=Cards[j];
				stpchk[1]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 11 && stpchk[2]==0){//i번째와 j번째 문양이 일치, 숫자가 J
				stptmp[stpcnt++]=Cards[j];
				stpchk[2]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 12 && stpchk[3]==0){//i번째와 j번째 문양이 일치, 숫자가 Q
				stptmp[stpcnt++]=Cards[j];
				stpchk[3]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 13 && stpchk[4]==0){//i번째와 j번째 문양이 일치, 숫자가 K
				stptmp[stpcnt++]=Cards[j];
				stpchk[4]=1;
			}
		}
		if(stpcnt > 4){	//5장 이상, 즉 로얄 스트레이트 플러쉬

			score = 10000*12 + 100*(stptmp[stpcnt-1]%100);
			if(stptmp[0]%100==1){	//에이스 탑
				score = 10000*12 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 12 && mark > max_score%100)	//플러쉬는 숫자보다 문양 우선
				max_score=score;
			else
				max_score=max(score,max_score);
		}
	}

	//스트레이트 플러쉬
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
			if(Cards[j]/100 == mark && Cards[j]%100 == base+1 && stpchk[0]==0){//i번째와 j번째 문양이 일치, 숫자가 +1
				stptmp[stpcnt++]=Cards[j];
				stpchk[0]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+2 && stpchk[1]==0){//i번째와 j번째 문양이 일치, 숫자가 +2
				stptmp[stpcnt++]=Cards[j];
				stpchk[1]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+3 && stpchk[2]==0){//i번째와 j번째 문양이 일치, 숫자가 +3
				stptmp[stpcnt++]=Cards[j];
				stpchk[2]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+4 && stpchk[3]==0){//i번째와 j번째 문양이 일치, 숫자가 +4
				stptmp[stpcnt++]=Cards[j];
				stpchk[3]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+5 && stpchk[4]==0){//i번째와 j번째 문양이 일치, 숫자가 +5
				stptmp[stpcnt++]=Cards[j];
				stpchk[4]=1;
			}
		}
		if(stpcnt > 4){	//5장 이상, 즉 로얄 스트레이트 플러쉬

			score = 10000*11 + 100*(stptmp[stpcnt-1]%100);
			if(stptmp[0]%100==1){	//에이스 탑
				score = 10000*11 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 11 && mark > max_score%100)	//플러쉬는 숫자보다 문양 우선
				max_score=score;
			else
				max_score=max(score,max_score);
		}
	}

	//플러쉬
	for(int i=0;i<Turn;i++){
		plushtmp[0]=Cards[i];
		plushcnt=1;
		mark=Cards[i]/100;
		for(int j=i+1;j<Turn;j++){
			if(Cards[j]/100 == mark){//i번째와 j번째 문양이 일치
				plushtmp[plushcnt++]=Cards[j];
			}
		}
		if(plushcnt > 4){	//5장 이상, 즉 플러쉬

			score = 10000*8 + 100*(plushtmp[plushcnt-1]%100);
			if(plushtmp[0]%100==1){	//에이스 탑
				score = 10000*8 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 8 && mark > max_score%100)	//플러쉬는 숫자보다 문양 우선
				max_score=score;
			else
				max_score=max(score,max_score);
		}
	}

	//스트레이트
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
			if(Cards[j]%100 == base+1 && stchk[0]==0){//i번째와 j번째 숫자가 +1
				sttmp[stcnt++]=Cards[j];
				stchk[0]=1;
			}
			if(Cards[j]%100 == base+2 && stchk[1]==0){//i번째와 j번째 숫자가 +2
				sttmp[stcnt++]=Cards[j];
				stchk[1]=1;
			}
			if(Cards[j]%100 == base+3 && stchk[2]==0){//i번째와 j번째 숫자가 +3
				sttmp[stcnt++]=Cards[j];
				stchk[2]=1;
			}
			if(Cards[j]%100 == base+4 && stchk[3]==0){//i번째와 j번째 숫자가 +4
				sttmp[stcnt++]=Cards[j];
				stchk[3]=1;
			}
		}
		if(stcnt > 4){	//5장 이상, 즉 스트레이트

			score = 10000*5 + 100*(sttmp[stcnt-1]%100) + sttmp[stcnt-1]/100;
			if(sttmp[0]%100==1){	//에이스 탑, 백스트레이트
				score = 10000*6 + 100*14 + sttmp[0]/100;
			}
			max_score=max(score,max_score);
		}
	}

	//마운틴
	for(int i=0;i<Turn;i++){
		stcnt=0;
		stchk[0]=0;
		stchk[1]=0;
		stchk[2]=0;
		stchk[3]=0;
		stchk[4]=0;
		for(int j=i;j<Turn;j++){
			if(Cards[j]%100 == 1 && stchk[0]==0){//i번째와 j번째 숫자가 A
				sttmp[stcnt++]=Cards[j];
				stchk[0]=1;
			}
			if(Cards[j]%100 == 10 && stchk[1]==0){//i번째와 j번째 숫자가 10
				sttmp[stcnt++]=Cards[j];
				stchk[1]=1;
			}
			if(Cards[j]%100 == 11 && stchk[2]==0){//i번째와 j번째 숫자가 11
				sttmp[stcnt++]=Cards[j];
				stchk[2]=1;
			}
			if(Cards[j]%100 == 12 && stchk[3]==0){//i번째와 j번째 숫자가 12
				sttmp[stcnt++]=Cards[j];
				stchk[3]=1;
			}
			if(Cards[j]%100 == 13 && stchk[4]==0){//i번째와 j번째  숫자가 13
				sttmp[stcnt++]=Cards[j];
				stchk[4]=1;
			}
		}
		if(stcnt > 4){	//5장 이상, 즉 마운틴

			score = 10000*7 + 100*14 + sttmp[stcnt-1]/100;
			max_score=max(score,max_score);
		}
	}

	//페어 시리즈
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
		if(paircnt[i] >= 4){	//포카드			
			score = 10000*10 + 100*(pairtmp[i][3]%100) + 4;
			if(pairtmp[i][0]%100 == 1)
				score = 10000*10 + 100*14 + pairtmp[i][0]/100;
			max_score=max(score,max_score);
		}
		else if(paircnt[i] == 3){	//트리플		
			score = 10000*4 + 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, max( pairtmp[i][1]/100, pairtmp[i][2]/100));
			if(pairtmp[i][0]%100 == 1)
				score = 10000*4 + 100*14 + pairtmp[i][0]/100;
			max_score=max(score,max_score);

			for(int j=0;j<Turn;j++){
				if(paircnt[j] == 2){//풀하우스
					score = 10000*9 + 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, max( pairtmp[i][1]/100, pairtmp[i][2]/100));
					if(pairtmp[i][0]%100 == 1)
						score = 10000*9 + 100*14 + pairtmp[i][0]/100;
					max_score=max(score,max_score);
				}
			}
		}
		else if(paircnt[i] == 2){	//원페어		
			score = 10000*2 + 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, pairtmp[i][1]/100);
			if(pairtmp[i][0]%100 == 1)
				score = 10000*2 + 100*14 + pairtmp[i][0]/100;

			max_score=max(score,max_score);

			for(int j=0;j<Turn;j++){
				if(i != j && paircnt[j] == 2){//투페어
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

	if(max_score == 0){	//아무런것도 해당 없음, 노페어
		max_score=10000 + 100*(Cards[Turn-1]%100) + Cards[Turn-1]/100;
		if(Cards[0]%100 == 1)//에이스
			max_score=10000 + 100*14 + Cards[0]/100;
	}

	return max_score;
	/*
	
로티플			12	0	
스티플			11	0	
포카드			10	0	
풀하우스		9	0	
플러시			8	0	
마운틴			7	0	
백스트레이트	6	0	
스트레이트		5	0	
트리플			4	0	
투페어			3	0	
원페어			2	0	
노페어			1	0
	*/

}

int CalRecommend(int Turn, int getcards[]){

	int score,max_score=0;
	int Cards[7],cnt,mark,mcnt;
	int stpcnt,stptmp[7],base,stpchk[7];
	int plushcnt,plushtmp[7];
	int stcnt,sttmp[7],stchk[7];
	int paircnt[7],pairtmp[7][7],pairsize,pairview;

	//패 수만큼 준다
	for(int i=0;i<Turn;i++)
		Cards[i]=getcards[i];

	for(int i=0, temp;i<Turn;i++){		//패 순서로 정렬
		for(int j=i;j<Turn;j++){
			if(Cards[i]%100>Cards[j]%100){
				temp=Cards[i];
				Cards[i]=Cards[j];
				Cards[j]=temp;
			}
		}
	}

	//로티플 체크
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
			if(Cards[j]/100 == mark && Cards[j]%100 == 1 && stpchk[0]==0){//i번째와 j번째 문양이 일치, 숫자가 A
				stptmp[stpcnt++]=Cards[j];
				stpchk[0]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 10 && stpchk[1]==0){//i번째와 j번째 문양이 일치, 숫자가 10
				stptmp[stpcnt++]=Cards[j];
				stpchk[1]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 11 && stpchk[2]==0){//i번째와 j번째 문양이 일치, 숫자가 J
				stptmp[stpcnt++]=Cards[j];
				stpchk[2]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 12 && stpchk[3]==0){//i번째와 j번째 문양이 일치, 숫자가 Q
				stptmp[stpcnt++]=Cards[j];
				stpchk[3]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == 13 && stpchk[4]==0){//i번째와 j번째 문양이 일치, 숫자가 K
				stptmp[stpcnt++]=Cards[j];
				stpchk[4]=1;
			}
		}
		if(stpcnt > 4){	//5장 이상, 즉 로얄 스트레이트 플러쉬

			score = 10000*15 + 100*(stptmp[stpcnt-1]%100);
			if(stptmp[0]%100==1){	//에이스 탑
				score = 10000*15 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 15 && mark > max_score%100)	//플러쉬는 숫자보다 문양 우선
				max_score=score;
			else
				max_score=max(score,max_score);
		}
		else if(stpcnt == 4){	//로티플-4
			score = 10000*8 + 100*(stptmp[3]%100);
			if(stptmp[0]%100==1){	//에이스 탑
				score = 10000*8 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 8 && mark > max_score%100)	//플러쉬는 숫자보다 문양 우선
				max_score=score;
			else
				max_score=max(score,max_score);

		}
	}

	//스트레이트 플러쉬
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
			if(Cards[j]/100 == mark && Cards[j]%100 == base+1 && stpchk[0]==0){//i번째와 j번째 문양이 일치, 숫자가 +1
				stptmp[stpcnt++]=Cards[j];
				stpchk[0]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+2 && stpchk[1]==0){//i번째와 j번째 문양이 일치, 숫자가 +2
				stptmp[stpcnt++]=Cards[j];
				stpchk[1]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+3 && stpchk[2]==0){//i번째와 j번째 문양이 일치, 숫자가 +3
				stptmp[stpcnt++]=Cards[j];
				stpchk[2]=1;
			}
			if(Cards[j]/100 == mark && Cards[j]%100 == base+4 && stpchk[3]==0){//i번째와 j번째 문양이 일치, 숫자가 +4
				stptmp[stpcnt++]=Cards[j];
				stpchk[3]=1;
			}
		}
		if(stpcnt > 4){	//5장 이상, 즉 스트레이트 플러쉬

			score = 10000*14 + 100*(stptmp[stpcnt-1]%100);
			if(stptmp[0]%100==1){	//에이스 탑
				score = 10000*14 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 14 && mark > max_score%100)	//플러쉬는 숫자보다 문양 우선
				max_score=score;
			else
				max_score=max(score,max_score);
		}
		else if(stpcnt == 4){	//스티플-4
			score = 10000*8 + 100*(stptmp[3]%100);
			if(stptmp[0]%100==1){	//에이스 탑
				score = 10000*8 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 8 && mark > max_score%100)	//플러쉬는 숫자보다 문양 우선
				max_score=score;
			else
				max_score=max(score,max_score);

		}
	}

	//플러쉬
	for(int i=0;i<Turn;i++){
		plushtmp[0]=Cards[i];
		plushcnt=1;
		mark=Cards[i]/100;
		for(int j=i+1;j<Turn;j++){
			if(Cards[j]/100 == mark){//i번째와 j번째 문양이 일치
				plushtmp[plushcnt++]=Cards[j];
			}
		}
		if(plushcnt > 4){	//5장 이상, 즉 플러쉬

			score = 10000*11 + 100*(plushtmp[plushcnt-1]%100);
			if(plushtmp[0]%100==1){	//에이스 탑
				score = 10000*11 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 11 && mark > max_score%100)	//플러쉬는 숫자보다 문양 우선
				max_score=score;
			else
				max_score=max(score,max_score);
		}
		else if(plushcnt == 4){	//플러쉬-4
			score = 10000*6 + 100*(plushtmp[3]%100);
			if(plushtmp[0]%100==1){	//에이스 탑
				score = 10000*6 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 6 && mark > max_score%100)	//플러쉬는 숫자보다 문양 우선
				max_score=score;
			else
				max_score=max(score,max_score);

		}
		else if(plushcnt == 3){	//플러쉬-3
			score = 10000*3 + 100*(plushtmp[2]%100);
			if(plushtmp[0]%100==1){	//에이스 탑
				score = 10000*3 + 100*14;
			}
			score+=mark;
			if(max_score/10000 == 3 && mark > max_score%100)	//플러쉬는 숫자보다 문양 우선
				max_score=score;
			else
				max_score=max(score,max_score);
		}
	}

	//스트레이트
	for(int i=0;i<Turn;i++){
		sttmp[0]=Cards[i];
		stcnt=1;
		stchk[0]=0;
		stchk[1]=0;
		stchk[2]=0;
		stchk[3]=0;
		base=Cards[i]%100;
		for(int j=i+1;j<Turn;j++){
			if(Cards[j]%100 == base+1 && stchk[0]==0){//i번째와 j번째 숫자가 +1
				sttmp[stcnt++]=Cards[j];
				stchk[0]=1;
			}
			if(Cards[j]%100 == base+2 && stchk[1]==0){//i번째와 j번째 숫자가 +2
				sttmp[stcnt++]=Cards[j];
				stchk[1]=1;
			}
			if(Cards[j]%100 == base+3 && stchk[2]==0){//i번째와 j번째 숫자가 +3
				sttmp[stcnt++]=Cards[j];
				stchk[2]=1;
			}
			if(Cards[j]%100 == base+4 && stchk[3]==0){//i번째와 j번째 숫자가 +4
				sttmp[stcnt++]=Cards[j];
				stchk[3]=1;
			}
		}
		if(stcnt >= 5){	//5장 이상, 즉 스트레이트

			score = 10000*10 + 100*(sttmp[stcnt-1]%100) + sttmp[stcnt-1]/100;
			if(sttmp[0]%100==1){	//에이스 탑
				score = 10000*10 + 100*14 + sttmp[0]/100;
			}
			max_score=max(score,max_score);
		}
		else if(stcnt == 4){	//스트레이트-4
			score = 10000*5 + 100*(sttmp[3]%100) + sttmp[3]/100;
			if(sttmp[0]%100==1){	//에이스 탑
				score = 10000*5 + 100*14 + sttmp[0]/100;
			}
			max_score=max(score,max_score);

		}
		else if(stcnt == 3){	//스트레이트-3	
			score = 10000*2 + 100*(sttmp[2]%100) + sttmp[2]/100;
			if(sttmp[0]%100==1){	//에이스 탑
				score = 10000*2 + 100*14 + sttmp[0]/100;
			}
			max_score=max(score,max_score);
		}
	}


		//마운틴
	for(int i=0;i<Turn;i++){
		stcnt=0;
		stchk[0]=0;
		stchk[1]=0;
		stchk[2]=0;
		stchk[3]=0;
		stchk[4]=0;
		base=Cards[i]%100;
		for(int j=i;j<Turn;j++){
			if(Cards[j]%100 == 1 && stchk[0]==0){//i번째와 j번째 숫자가 +1
				sttmp[stcnt++]=Cards[j];
				stchk[0]=1;
			}
			if(Cards[j]%100 == 10 && stchk[1]==0){//i번째와 j번째 숫자가 +10
				sttmp[stcnt++]=Cards[j];
				stchk[1]=1;
			}
			if(Cards[j]%100 == 11 && stchk[2]==0){//i번째와 j번째 숫자가 +11
				sttmp[stcnt++]=Cards[j];
				stchk[2]=1;
			}
			if(Cards[j]%100 == 12 && stchk[3]==0){//i번째와 j번째 숫자가 +12
				sttmp[stcnt++]=Cards[j];
				stchk[3]=1;
			}
			if(Cards[j]%100 == 13 && stchk[4]==0){//i번째와 j번째  숫자가 +13
				sttmp[stcnt++]=Cards[j];
				stchk[4]=1;
			}
		}
		if(stcnt > 4){	//5장 이상, 즉 마운틴
			score = 10000*10 + 100*14 + sttmp[stcnt-1]/100;
			max_score=max(score,max_score);
		}
		else if(stcnt == 4){	//스트레이트-4
			score = 10000*5 + 100*(sttmp[3]%100) + sttmp[3]/100;
			if(sttmp[0]%100==1){	//에이스 탑
				score = 10000*5 + 100*14 + sttmp[0]/100;
			}
			max_score=max(score,max_score);

		}
		else if(stcnt == 3){	//스트레이트-3	
			score = 10000*2 + 100*(sttmp[2]%100) + sttmp[2]/100;
			if(sttmp[0]%100==1){	//에이스 탑
				score = 10000*2 + 100*14 + sttmp[0]/100;
			}
			max_score=max(score,max_score);
		}
	}


	//페어 시리즈
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
		if(paircnt[i] >= 4){	//포카드			
			score = 10000*13 + 100*(pairtmp[i][3]%100) + 4;
			if(pairtmp[i][0]%100 == 1)
				score = 10000*13 + 100*14 + pairtmp[i][0]/100;
			max_score=max(score,max_score);
		}
		else if(paircnt[i] == 3){	//트리플		
			score = 10000*9 + 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, max( pairtmp[i][1]/100, pairtmp[i][2]/100));
			if(pairtmp[i][0]%100 == 1)
				score = 10000*9 + 100*14 + pairtmp[i][0]/100;
			max_score=max(score,max_score);

			for(int j=0;j<Turn;j++){
				if(paircnt[j] == 2){//풀하우스
					score = 10000*12 + 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, max( pairtmp[i][1]/100, pairtmp[i][2]/100));
					if(pairtmp[i][0]%100 == 1)
						score = 10000*12 + 100*14 + pairtmp[i][0]/100;
					max_score=max(score,max_score);
				}
			}
		}
		else if(paircnt[i] == 2){	//원페어		
			score = 10000*4 + 100*(pairtmp[i][0]%100) + max(pairtmp[i][0]/100, pairtmp[i][1]/100);
			if(pairtmp[i][0]%100 == 1)
				score = 10000*4 + 100*14 + pairtmp[i][0]/100;

			max_score=max(score,max_score);

			for(int j=0;j<Turn;j++){
				if(i != j && paircnt[j] == 2){//투페어
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

	if(max_score == 0){	//아무런것도 해당 없음, 노페어
		max_score=10000 + 100*(Cards[Turn-1]%100) + Cards[Turn-1]/100;
	}

	return max_score;



	/*

	포커 족보-예측

	족보번호(10000) 탑패번호(100) 문양번호 

	로티플			15	0
	스티플			14	0
	포카드			13	0
	풀하우스		12	0
	플러시			11	0
	스트레이트		10	0
	트리플			9	0
	스티플-4			8	0
	투페어			7	0
	플러시-4			6	0
	스트레이트-4		5	0
	원페어			4	0
	플러시-3			3	0
	스트레이트-3		2	0
	노페어			1	0

	*/
}

/*
배팅 방법

콜 : 앞사람이 베팅한 금액과 같은 금액을 베팅한다
 
따당 : 앞사람이 베팅한 금액의 두배를 베팅한다
 
하프 : 현재 배팅된 판돈의 절반을 배팅한다
 
쿼터 : 현재 배팅된 판돈의 1/4을 배팅한
 
체크 : 베팅하지 않고 다음사람에게 배팅권한을 넘긴다 (선만 가능)
 
삥 : 기본판돈을 베팅한다 (선만 가능)
 
다이 : 해당 판을 포기한다 


*/
/*

포커 족보

			족보번호(10000) 탑패번호(100) 문양번호 

로티플			12	0	
스티플			11	0	
포카드			10	0	
풀하우스		9	0	
플러시			8	0	
마운틴			7	0	
백스트레이트	6	0	
스트레이트		5	0	
트리플			4	0	
투페어			3	0	
원페어			2	0	
노페어			1	0
*/