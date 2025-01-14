
// ProjectView.h: CProjectView 클래스의 인터페이스
//

#pragma once
#include "afxwin.h"
#include "afxtempl.h"

class CProjectDoc;
class CShopDialog;
class Bubble {
public:
	Bubble(){}

	int xpos, ypos, movex, movey;
	TCHAR alphabet;
	BOOL Alive;
	CBitmap bmp;
	~Bubble(){}

	void Create(int t) {
		while (1) {
			xpos = rand() % 1870;
			ypos = rand() % 930;
			if (!(xpos >= 300 && xpos <= 1400 && ypos >= 200 && ypos <= 700))
				break;
		}
		while (1) {
			movex = rand() % 7 - 3;
			movey = rand() % 7 - 3;
			if (movex != 0 && movey != 0)
				break;
		}
		alphabet = 'A' + t;
		Alive = TRUE;
		bmp.LoadBitmap(324+t);
	}
	void bound() {
		if (xpos > 1870) {
			if (movex > 0)
				movex *= -1;
		}
		else if (xpos < 0) {
			if (movex < 0)
				movex *= -1;
		}
		if (ypos > 930) {
			if (movey > 0)
				movey *= -1;
		}
		else if (ypos < 0) {
			if (movey < 0)
				movey *= -1;
		}

	}
};
class class_coin {
public:
	int xpos, ypos;
	CBitmap bmp;
	BOOL Alive;
	CRect rect;
	class_coin() {
	}
	void Create()
	{
		xpos = rand() % 1800;
		ypos = 0;
		Alive = TRUE;
	}
};
class Octopus {
public :
	int xpos, ypos;
	int move_x;
	CBitmap bmp;
	BOOL Alive;
	CRect rect;
	int LR;
	int n;
	Octopus() {
		Alive = FALSE;
	};
	void Create() {
		LR = rand() % 2;
		n = 0;
		move_x = 3;
		rect.SetRectEmpty();
		bmp.DeleteObject();
		if (LR == 0) // 오른쪽에서 출현
		{
			xpos = 2000;
			ypos = rand() % 900;
			bmp.LoadBitmap(IDB_ENEMY_OCTOPUS);
		}
		else         // 왼쪽에서 출현
		{
			xpos = -100;
			ypos = rand() % 900;
			bmp.LoadBitmap(IDB_ENEMY_OCTOPUS_REVERSE);
		}
	}
};
class SeaHorse{
public:
	int xpos, ypos;
	int movex, movey;
	CBitmap bmp;
	CRect rect;
	int LR;
	int n;
	SeaHorse(){};
	void Create() {
		n = 0;
		LR = rand() % 2;
		ypos = rand() % 600 + 150;
		movey = 3;
		rect.SetRectEmpty();
		bmp.DeleteObject();
		if (LR == 0) {
			xpos = -100;
			bmp.LoadBitmap(IDB_ENEMY_SEAHORSE_REVERSE);
			movex = 2;
		}
		else {
			xpos = 2000;
			bmp.LoadBitmap(IDB_ENEMY_SEAHORSE);
			movex = -2;
		}
	}
};
class Piranha {
public:
	int xpos, ypos;
	int move_x, move_y;
	CBitmap bmp;
	CRect rect;
	int LR;
	int n;
	Piranha() {}
	void Create() {
		n = 0;
		LR = rand() % 4; // 방향설정
		rect.SetRectEmpty();
		if (LR == 0) { // 위에서
			xpos = rand() % 1800;
			ypos = -150;
		}
		else if (LR == 1) { // 오른쪽에서
			xpos = 1800;
			ypos = rand() % 900;
		}
		else if (LR == 2) { // 아래에서
			xpos = rand() % 1800;
			ypos = 1000;
		}
		else if (LR == 3) { // 왼쪽에서
			xpos = -200;
			ypos = rand() % 900;
		}

	}
	
	~Piranha() {}
};
class Shark {
public:
	int xpos, ypos;
	int movex;
	CBitmap bmp;
	CRect rect;
	int LR;
	BOOL Alive;

	Shark() { Alive = FALSE; };
	void Create() {
		bmp.DeleteObject();
		Alive = TRUE;
		LR = rand() % 2;
		rect.SetRectEmpty();
		if (LR == 0) // 오른쪽에서 출현
		{
			movex = -5;
			xpos = 2000;
			ypos = rand() % 800;
			bmp.LoadBitmap(IDB_ENEMY_SHARK);
		}
		else         // 왼쪽에서 출현
		{
			movex = 5;
			xpos = -200;
			ypos = rand() % 800;
			bmp.LoadBitmap(IDB_ENEMY_SHARK_REVERSE);
		}
	}
};

class CProjectView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CProjectView();
	DECLARE_DYNCREATE(CProjectView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_PROJECT_FORM };
#endif

// 특성입니다.
public:
	BOOL f1, f2, f3, f4, f5, f6, f7, f8;
	CProjectDoc* GetDocument() const;
	CArray <CString, CString> Word;
	CArray <CString, CString> Mean;
	CString word;
	int word_length;
	int wIndex;
	CRect MyFishRect;
	CPoint FishPos;
	CArray <CRect, CRect> Bubble_Rect;
	CRect rcTemp;
	CString current_word;
	int current_word_length;
	class_coin Coin[10];
	int coin_inmap = 0;
	SeaHorse SH;
	Octopus oct;
	Shark shark;
	Piranha prh;
// 작업입니다.
public:
	int xpos, ypos, movex, movey;
	int coin;
	BOOL KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT;
	BOOL Start, Alive;
	CBrush brush;
	LPCTSTR fish;
	LPCTSTR fish2;
	CString Lskin, Rskin;
	CShopDialog* m_pDlg;
	int bubble_num;
	Bubble bubble[30];
	BITMAP bubble_bmpinfo;
	CBitmap main;
	BOOL mirror;
	BOOL Easy, Normal, Hard;
	CString coin_num;
	BOOL onetime;
	CFont font;
	BOOL christmas;
	BOOL shopopen;
	int whichlevel;
// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* /*pDC*/);
public:
	afx_msg void OnShop();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBitmapButton Button_New_Game;
	CBitmapButton Button_Load_Game;
	CBitmapButton Button_End;
	CBitmapButton Button_Easy;
	CBitmapButton Button_Normal;
	CBitmapButton Button_Hard;
	CBitmapButton Button_Restart;
	CBitmapButton Button_End2;
	CBitmapButton Button_nextstage;
	afx_msg void OnBnClickedNewGame();
	afx_msg void OnBnClickedLoadGame();
	afx_msg void OnBnClickedEnd();
	afx_msg void OnBnClickedEasy();
	afx_msg void OnBnClickedNormal();
	afx_msg void OnBnClickedHard();
	afx_msg void OnBnClickedRestart();
	afx_msg void OnBnClickedEnd2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void Restart();
	void Die();
	void PrhTransition();
	//void Wait(DWORD dwMillisecond);
	afx_msg void OnUpdateShop(CCmdUI *pCmdUI);
	void Clear();

	afx_msg void OnBnClickedContinue();
	virtual void PostNcDestroy();
	virtual void Serialize(CArchive& ar);
//	afx_msg void OnClose();
	afx_msg void OnInfo();
};

#ifndef _DEBUG  // ProjectView.cpp의 디버그 버전
inline CProjectDoc* CProjectView::GetDocument() const
   { return reinterpret_cast<CProjectDoc*>(m_pDocument); }
#endif