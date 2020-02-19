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
	�̸� ī����� �׷��Ȱ��� �����´�.
	Ŭ�ι�-��Ʈ-���̾�-�����̵� ��.

	*/
	int CardImg;
	for(int i=0;i<4;i++){
		switch(i){	//ī�尡 � ������ ����
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
 //ȸ������ �ٷ� �׸� ��ǥ, �� ixdisplay�� iydisplay�� �ȴ�. 10,10,50,50 ���� �� ��� �׸��� 10,10 �� ȭ����� 50,50 �� ��ġ.
	
	int i;
 BITMAP bm;
 GetObject(hBmp, sizeof(BITMAP), &bm);
 POINT apt[3]={0};
 double dblWidth=(double)bm.bmWidth*dblSizeRatio;
 double dblHeight=(double)bm.bmHeight*dblSizeRatio;
 double ixRotate=(int)((double)ixRotateAxis*dblSizeRatio); // ũ�Ⱑ ���ϴ� �� ���
 double iyRotate=(int)((double)iyRotateAxis*dblSizeRatio);
 const double pi=3.14159265358979323846;


 double dblRadian, dblx, dbly, dblxDest, dblyDest, cosVal, sinVal;
 dblRadian=dblAngle*pi/180.0f;
 cosVal=cos(dblRadian), sinVal=sin(dblRadian);

 // 1. ȸ������ �������� �����ǥ�� ���ϰ� 
 // 2. ȸ���� ��ġ��ǥ(�����ǥ)�� ���� ��
 // 3. ���� ���� ������ ��ǥ�� ����.
 for(i=0 ; i<3 ; i++){
  if(i==0){dblx=-ixRotate, dbly=-iyRotate;}    // left up  ������ �κ�
  else if(i==1){dblx=dblWidth-ixRotate, dbly=-iyRotate;}  // right up ������ �κ�
  else if(i==2){dblx=-ixRotate, dbly=dblHeight-iyRotate;} // left low ������ �κ�
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

