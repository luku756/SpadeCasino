#pragma once
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "resource.h"
#include "Player.h"

HBITMAP PokerCards[4][13];
HBITMAP CardBack;

void GetCardGraphic(){
	/*
	미리 카드들의 그래픽값을 가져온다.
	클로버-하트-다이아-스페이드 순.

	*/
	int CardImg;
	for(int i=0;i<4;i++){
		switch(i){	//카드가 어떤 것인지 고른다
				case 3: CardImg=IDB_SPADE1; break;
				case 2: CardImg=IDB_DIAMOND1; break;
				case 1: CardImg=IDB_HART1;break;
				case 0: CardImg=IDB_CLOVER1; break;
				}
		for(int j=0;j<13;j++){
			PokerCards[i][j]=LoadBitmap(g_hInst, MAKEINTRESOURCE(CardImg+j));
		}
	}
	CardBack=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BACK));
}

void FreeCardGraphic(){
	for(int i=0;i<4;i++){
		for(int j=0;j<13;j++){
			DeleteObject(PokerCards[i][j]);
		}
	}
	DeleteObject(CardBack);
}

BOOL RotateSizingImage(HDC hdc, HBITMAP hBmp,double dblAngle,int ixRotateAxis, int iyRotateAxis,int ixDisplay, int iyDisplay,double dblSizeRatio=1,HBITMAP hMaskBmp=NULL, int ixMask=0, int iyMask=0){
 //회전축이 바로 그릴 좌표, 즉 ixdisplay와 iydisplay가 된다. 10,10,50,50 으로 할 경우 그림의 10,10 이 화면상의 50,50 에 위치.
	
	int i;
 BITMAP bm;
 GetObject(hBmp, sizeof(BITMAP), &bm);
 POINT apt[3]={0};
 double dblWidth=(double)bm.bmWidth*dblSizeRatio;
 double dblHeight=(double)bm.bmHeight*dblSizeRatio;
 double ixRotate=(int)((double)ixRotateAxis*dblSizeRatio); // 크기가 변하는 것 고려
 double iyRotate=(int)((double)iyRotateAxis*dblSizeRatio);
 const double pi=3.14159265358979323846;


 double dblRadian, dblx, dbly, dblxDest, dblyDest, cosVal, sinVal;
 dblRadian=dblAngle*pi/180.0f;
 cosVal=cos(dblRadian), sinVal=sin(dblRadian);

 // 1. 회전축을 기준으로 상대좌표를 구하고 
 // 2. 회전후 위치좌표(상대좌표)를 얻은 후
 // 3. 얻은 값을 원래의 좌표에 적용.
 for(i=0 ; i<3 ; i++){
  if(i==0){dblx=-ixRotate, dbly=-iyRotate;}    // left up  꼭지점 부분
  else if(i==1){dblx=dblWidth-ixRotate, dbly=-iyRotate;}  // right up 꼭지점 부분
  else if(i==2){dblx=-ixRotate, dbly=dblHeight-iyRotate;} // left low 꼭지점 부분
  dblxDest=dblx*cosVal-dbly*sinVal;
  dblyDest=dblx*sinVal+dbly*cosVal;
  dblxDest+=ixRotate, dblyDest+=iyRotate;
  apt[i].x=ixDisplay-(long)ixRotate+(long)dblxDest;
  apt[i].y=iyDisplay-(long)iyRotate+(long)dblyDest;
 }

 HDC hMemdc;
 HBITMAP hOldBmp;
 hMemdc=CreateCompatibleDC(hdc);
 hOldBmp=(HBITMAP)SelectObject(hMemdc, hBmp);
 BOOL iRes=PlgBlt(hdc, apt, hMemdc, 0, 0, bm.bmWidth, bm.bmHeight, hMaskBmp, ixMask, iyMask);
 SelectObject(hMemdc,hOldBmp);
 DeleteDC(hMemdc);
 return iRes;
}

void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit) {   
	HDC MemDC;    
	HBITMAP OldBitmap;    
	int bx,by;    
	BITMAP bit;    
	MemDC=CreateCompatibleDC(hdc);   
	OldBitmap=(HBITMAP)SelectObject(MemDC, hBit);  
	GetObject(hBit,sizeof(BITMAP),&bit);   
	bx=bit.bmWidth;     by=bit.bmHeight;   
	BitBlt(hdc,x,y,bx,by,MemDC,0,0,SRCCOPY);    
	SelectObject(MemDC,OldBitmap);   
	DeleteDC(MemDC); 
}


void DrawSmallBitmap(HDC hdc,int x,int y,float rate ,HBITMAP hBit) {   
	HDC MemDC;    
	HBITMAP OldBitmap;    
	int bx,by;    
	BITMAP bit;    
	MemDC=CreateCompatibleDC(hdc);   
	OldBitmap=(HBITMAP)SelectObject(MemDC, hBit);  
	GetObject(hBit,sizeof(BITMAP),&bit);   
	bx=bit.bmWidth;     by=bit.bmHeight;   
//	BitBlt(hdc,x,y,bx,by,MemDC,0,0,SRCCOPY); 
	StretchBlt(hdc,x,y,bx*rate,by*rate,MemDC,0,0,bx,by,SRCCOPY);


	SelectObject(MemDC,OldBitmap);   
	DeleteDC(MemDC); 
}

