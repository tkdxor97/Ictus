
// ProjectView.cpp: CProjectView 클래스의 구현
//

#include "stdafx.h"
#pragma comment(lib, "winmm")
#include <mmsystem.h>
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Project.h"
#endif

#include "ProjectDoc.h"
#include "ProjectView.h"
#include "ShopDialog.h"
#include "locale.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProjectView

IMPLEMENT_DYNCREATE(CProjectView, CFormView)

BEGIN_MESSAGE_MAP(CProjectView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_COMMAND(ID_SHOP, &CProjectView::OnShop)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_NEW_GAME, &CProjectView::OnBnClickedNewGame)
	ON_BN_CLICKED(IDC_LOAD_GAME, &CProjectView::OnBnClickedLoadGame)
	ON_BN_CLICKED(IDC_END, &CProjectView::OnBnClickedEnd)
	ON_BN_CLICKED(IDC_EASY, &CProjectView::OnBnClickedEasy)
	ON_BN_CLICKED(IDC_NORMAL, &CProjectView::OnBnClickedNormal)
	ON_BN_CLICKED(IDC_HARD, &CProjectView::OnBnClickedHard)
	ON_BN_CLICKED(IDC_RESTART, &CProjectView::OnBnClickedRestart)
	ON_BN_CLICKED(IDC_END2, &CProjectView::OnBnClickedEnd2)
	ON_WM_CTLCOLOR()
	ON_UPDATE_COMMAND_UI(ID_SHOP, &CProjectView::OnUpdateShop)
	ON_BN_CLICKED(IDC_CONTINUE, &CProjectView::OnBnClickedContinue)
//	ON_WM_CLOSE()
ON_COMMAND(ID_INFO, &CProjectView::OnInfo)
END_MESSAGE_MAP()

// CProjectView 생성/소멸

CProjectView::CProjectView()
	: CFormView(IDD_PROJECT_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	coin = 60;
	xpos = 960;
	ypos = 500;
	KEY_UP = KEY_DOWN = KEY_RIGHT = KEY_LEFT = FALSE;
	Start = FALSE, Alive = TRUE;
	fish = (LPCTSTR)IDB_FISH_1;
	fish2 = (LPCTSTR)IDB_FISH1_REVERSE;
	f1 = TRUE, f2 = FALSE, f3 = FALSE, f4 = FALSE, f5 = FALSE, f6 = FALSE, f7 = FALSE, f8 = FALSE;
	srand(time(NULL));
	setlocale(LC_ALL, "korean");
	wIndex = 0;
	current_word = _T("");
	current_word_length = 0;
	mirror = FALSE;
	SH.Create();
	coin_num.Format(_T("%d"), coin_inmap);
	onetime = FALSE;
	font.CreatePointFont(200, _T("210 동화책 L"));
	christmas = FALSE;
	shopopen = TRUE;
	whichlevel = 1;
}

CProjectView::~CProjectView()
{
}

void CProjectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_GAME, Button_New_Game);
	DDX_Control(pDX, IDC_LOAD_GAME, Button_Load_Game);
	DDX_Control(pDX, IDC_END, Button_End);
	DDX_Control(pDX, IDC_EASY, Button_Easy);
	DDX_Control(pDX, IDC_NORMAL, Button_Normal);
	DDX_Control(pDX, IDC_HARD, Button_Hard);
	DDX_Control(pDX, IDC_RESTART, Button_Restart);
	DDX_Control(pDX, IDC_END2, Button_End2);
	DDX_Control(pDX, IDC_CONTINUE, Button_nextstage);
}

BOOL CProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CProjectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	GetDlgItem(IDC_WORD)->ShowWindow(FALSE);
	GetDlgItem(IDC_MEAN)->ShowWindow(FALSE);
	GetDlgItem(IDC_CURRENTWORD)->ShowWindow(FALSE);
	GetDlgItem(IDC_COIN)->ShowWindow(FALSE);
	
	main.LoadBitmap(IDB_MAIN);

	brush.CreatePatternBrush(&main);

	Button_New_Game.LoadBitmaps(IDB_BUTTON_NEW, IDB_BUTTON_NEWCL, IDB_BUTTON_NEW, IDB_BUTTON_NEW);
	Button_Load_Game.LoadBitmaps(IDB_BUTTON_CON, IDB_BUTTON_CONCL, IDB_BUTTON_CON, IDB_BUTTON_CON);
	Button_End.LoadBitmaps(IDB_BUTTON_END, IDB_BUTTON_ENDCL, IDB_BUTTON_END, IDB_BUTTON_END);
	Button_Easy.LoadBitmaps(IDB_BUTTON_EASY, IDB_BUTTON_EASYCL, IDB_BUTTON_EASY, IDB_BUTTON_EASY);
	Button_Normal.LoadBitmaps(IDB_BUTTON_NORMAL, IDB_BUTTON_NORMALCL, IDB_BUTTON_NORMAL, IDB_BUTTON_NORMAL);
	Button_Hard.LoadBitmaps(IDB_BUTTON_HARD, IDB_BUTTON_HARDCL, IDB_BUTTON_HARD, IDB_BUTTON_HARD);
	Button_Restart.LoadBitmaps(IDB_BUTTON_RESET, IDB_BUTTON_RESETCL, IDB_BUTTON_RESET, IDB_BUTTON_RESET);
	Button_End2.LoadBitmaps(IDB_BUTTON_END, IDB_BUTTON_ENDCL, IDB_BUTTON_END, IDB_BUTTON_END);
	Button_nextstage.LoadBitmaps(IDB_BUTTON_NEXT, IDB_BUTTON_NEXTCL, IDB_BUTTON_NEXT, IDB_BUTTON_NEXT);

	PlaySound(MAKEINTRESOURCE(IDR_WAVE1), AfxGetInstanceHandle(), SND_LOOP | SND_RESOURCE | SND_ASYNC | SND_NOSTOP);



}


// CProjectView 인쇄

BOOL CProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CProjectView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CProjectView 진단

#ifdef _DEBUG
void CProjectView::AssertValid() const
{
	CFormView::AssertValid();
}

void CProjectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CProjectDoc* CProjectView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProjectDoc)));
	return (CProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CProjectView 메시지 처리기


void CProjectView::OnDraw(CDC* pDC)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDC MemDC_Fish;
	CDC MemDC_Bubble;
	CDC MemDC_Coin;
	CDC MemDC_SH;
	CDC MemDC_Oct;
	CDC MemDC_Shark;
	CDC MemDC_Prh;

	BITMAP bmpinfo;
	CBitmap bmp;

	CBitmap bmp_coin;
	MemDC_Fish.CreateCompatibleDC(pDC);
	MemDC_Bubble.CreateCompatibleDC(pDC);
	MemDC_Coin.CreateCompatibleDC(pDC);
	MemDC_SH.CreateCompatibleDC(pDC);
	MemDC_Oct.CreateCompatibleDC(pDC);
	MemDC_Shark.CreateCompatibleDC(pDC);
	MemDC_Prh.CreateCompatibleDC(pDC);

	bmp.LoadBitmap(fish);
	bmp.GetBitmap(&bmpinfo);

	bubble_bmpinfo.bmHeight = 30;
	bubble_bmpinfo.bmWidth = 30;

	CRect rt;
	this->GetWindowRect(&rt);
	this->ScreenToClient(&rt);
	pDC->FillRect(&rt, &brush);
	//bmp.CreateCompatibleBitmap(pDC, rt.Width(), rt.Height());


	if (Start == TRUE && Alive == TRUE)
	{
		shopopen = FALSE;
		if (christmas) {
			PlaySound(MAKEINTRESOURCE(IDR_WAVE5), AfxGetInstanceHandle(), SND_LOOP | SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
		}
		else {
			PlaySound(MAKEINTRESOURCE(IDR_WAVE3), AfxGetInstanceHandle(), SND_LOOP | SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
		}
		GetDlgItem(IDC_WORD)->ShowWindow(TRUE);
		GetDlgItem(IDC_MEAN)->ShowWindow(TRUE);
		GetDlgItem(IDC_CURRENTWORD)->ShowWindow(TRUE);
		GetDlgItem(IDC_COIN)->ShowWindow(TRUE);

		if (GetKeyState(VK_LEFT) < 0)		//키보드로 방향 조절, 움직임
			KEY_LEFT = TRUE;
		else
			KEY_LEFT = FALSE;
		if (GetKeyState(VK_RIGHT) < 0)
			KEY_RIGHT = TRUE;
		else
			KEY_RIGHT = FALSE;
		if (GetKeyState(VK_UP) < 0)
			KEY_UP = TRUE;
		else
			KEY_UP = FALSE;
		if (GetKeyState(VK_DOWN) < 0)
			KEY_DOWN = TRUE;
		else
			KEY_DOWN = FALSE;


		if (KEY_LEFT && xpos >0) {
			movex = -5;
			mirror = TRUE;
			InvalidateRect(CRect(xpos - 10, ypos - 10, xpos + bmpinfo.bmWidth + 30, ypos + bmpinfo.bmHeight + 30), false);

		}
		else if (KEY_RIGHT && xpos < 1830) {
			movex = 5;
			mirror = FALSE;
			InvalidateRect(CRect(xpos - 10, ypos - 10, xpos + bmpinfo.bmWidth + 30, ypos + bmpinfo.bmHeight + 30), false);

		}
		else {
			movex = 0;
		}
		if (KEY_UP && ypos > 0) {
			movey = -5;
			InvalidateRect(CRect(xpos - 10, ypos - 10, xpos + bmpinfo.bmWidth + 30, ypos + bmpinfo.bmHeight + 30), false);

		}
		else if (KEY_DOWN && ypos < 920) {
			movey = 5;
			InvalidateRect(CRect(xpos - 10, ypos - 10, xpos + bmpinfo.bmWidth + 30, ypos + bmpinfo.bmHeight + 30), false);
		}
		else {
			movey = 0;
		}

		for (int i = 0; i < bubble_num; ++i)
		{
			if (bubble[i].Alive == TRUE) {
				MemDC_Bubble.SelectObject(&bubble[i].bmp);
				bubble[i].bound();
				bubble[i].xpos += bubble[i].movex;
				bubble[i].ypos += bubble[i].movey;

				pDC->TransparentBlt(bubble[i].xpos, bubble[i].ypos, bubble_bmpinfo.bmWidth + 10, bubble_bmpinfo.bmHeight + 10, &MemDC_Bubble, 0, 0, bubble_bmpinfo.bmWidth, bubble_bmpinfo.bmHeight, RGB(255, 0, 0));
				InvalidateRect(CRect(bubble[i].xpos - 10, bubble[i].ypos - 10, bubble[i].xpos + 60, bubble[i].ypos + 60), false);
				Bubble_Rect.GetAt(i).SetRect(bubble[i].xpos + 6, bubble[i].ypos + 6, bubble[i].xpos + 34, bubble[i].ypos + 34);

				if (IntersectRect(&rcTemp, &MyFishRect, &Bubble_Rect.GetAt(i))) // Crash Point
				{
					current_word += bubble[i].alphabet;
					bubble[i].Alive = FALSE;
					++current_word_length;
					SetDlgItemText(IDC_CURRENTWORD, current_word);
				}
			}

		}
		xpos += movex;
		ypos += movey;
		//FishRect
		FishPos.x = xpos + bmpinfo.bmWidth / 2;
		FishPos.y = ypos + bmpinfo.bmHeight / 2;
		MyFishRect.SetRect(FishPos.x - 32, FishPos.y - 17, FishPos.x + 32, FishPos.y + 17);
		if (onetime == FALSE) {
			bmp_coin.LoadBitmap(IDB_COIN);
			MemDC_Fish.SelectObject(&bmp_coin);
			pDC->TransparentBlt(1670, 15, 30, 30, &MemDC_Fish, 0, 0, 30, 30, RGB(255, 0, 0));
		}
		//	pDC->Rectangle(MyFishRect);


		if (mirror) {	//이동방향이 반대일때, 물고기 뒤집음
			bmp.DeleteObject();

			bmp.LoadBitmap(fish2);
			bmp.GetBitmap(&bmpinfo);
			MemDC_Fish.SelectObject(&bmp);
			pDC->TransparentBlt(xpos, ypos, bmpinfo.bmWidth, bmpinfo.bmHeight, &MemDC_Fish, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(255, 0, 0));
			bmp.DeleteObject();
		}
		else {
			MemDC_Fish.SelectObject(&bmp);
			pDC->TransparentBlt(xpos, ypos, bmpinfo.bmWidth, bmpinfo.bmHeight, &MemDC_Fish, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(255, 0, 0));
		}

		if (current_word_length >= word_length)	//단어 길이만큼 버블을 먹었을 때
		{
			if (!word.Compare(current_word))	//성공
			{
				if (Easy) {
					coin += 10;
					coin_num.Format(_T("%d"), coin);
					SetDlgItemText(IDC_COIN, coin_num);
				}
				else if (Normal) {
					coin += 20;
					coin_num.Format(_T("%d"), coin);
					SetDlgItemText(IDC_COIN, coin_num);
				}
				else if (Hard) {
					coin += 30;
					coin_num.Format(_T("%d"), coin);
					SetDlgItemText(IDC_COIN, coin_num);
				}
				Clear();
			}
			else	//실패
			{
				Die();

			}
		}

		if (rand() % 1000 == 1 && coin_inmap < 10)	//코인 출현
		{
			Coin[coin_inmap].Create();
			Coin[coin_inmap].bmp.LoadBitmap(IDB_COIN);
			coin_inmap++;
		}
		for (int i = 0; i < coin_inmap; ++i)	//코인 움직임, 출력
		{
			if (Coin[i].Alive == TRUE) {

				Coin[i].rect.SetRect(Coin[i].xpos + 5, Coin[i].ypos + 5, Coin[i].xpos + 25, Coin[i].ypos + 25);
				Coin[i].ypos += 2;
				MemDC_Coin.SelectObject(&Coin[i].bmp);
				pDC->TransparentBlt(Coin[i].xpos, Coin[i].ypos, 30, 30, &MemDC_Coin, 0, 0, 30, 30, RGB(255, 0, 0));

				if (IntersectRect(&rcTemp, &MyFishRect, &Coin[i].rect)) // Crash Point
				{
					coin += 5;
					Coin[i].Alive = FALSE;
					coin_num.Format(_T("%d"), coin);
					SetDlgItemText(IDC_COIN, coin_num);

				}
			}
		}
		SH.n++;
		if (SH.n == 100)
		{
			SH.n = 0;
			SH.movey *= -1;
		}
		SH.xpos += SH.movex;
		SH.ypos += SH.movey;
		if (SH.xpos > 2000 || SH.xpos < -100)
			SH.Create();
		MemDC_SH.SelectObject(&SH.bmp);
		pDC->TransparentBlt(SH.xpos, SH.ypos, 50, 50, &MemDC_SH, 0, 0, 50, 50, RGB(255, 0, 0));
		if (SH.LR == 0)
			SH.rect.SetRect(SH.xpos + 5, SH.ypos, SH.xpos + 30, SH.ypos + 50);
		else
			SH.rect.SetRect(SH.xpos + 20, SH.ypos, SH.xpos + 45, SH.ypos + 50);
		if (IntersectRect(&rcTemp, &MyFishRect, &SH.rect)) // Crash Point
		{
			Die();
		}

		if (rand() % 200 == 1 && oct.Alive == FALSE)
		{
			oct.Create();
			oct.Alive = TRUE;
		}
		if (oct.n <= 5)
		{
			++oct.n;
		}
		else if (oct.n <= 20)
		{
			oct.move_x = 2;
			++oct.n;
		}
		else if (oct.n <= 100)
		{
			oct.move_x = 1;
			++oct.n;
		}
		else
		{
			oct.move_x = 3;
			oct.n = 0;
		}

		if (oct.LR == 0) {
			oct.rect.SetRect(oct.xpos + 3, oct.ypos + 3, oct.xpos + 65, oct.ypos +56);
			oct.xpos -= oct.move_x;
			MemDC_Oct.SelectObject(&oct.bmp);
			pDC->TransparentBlt(oct.xpos, oct.ypos, 73 + 20, 50 + 20, &MemDC_Oct, 0, 0, 73, 50, RGB(255, 0, 0));
			if (oct.xpos < -100)
			{
				oct.Alive = FALSE;
			}
		}
		else {
			oct.rect.SetRect(oct.xpos + 27, oct.ypos + 3, oct.xpos + 90, oct.ypos + 65);
			oct.xpos += oct.move_x;
			MemDC_Oct.SelectObject(&oct.bmp);
			pDC->TransparentBlt(oct.xpos, oct.ypos, 73 + 20, 50 + 20, &MemDC_Oct, 0, 0, 73, 50, RGB(255, 0, 0));
			if (oct.xpos > 2000)
			{
				oct.Alive = FALSE;
			}
		}
		if (IntersectRect(&rcTemp, &MyFishRect, &oct.rect)) // Crash Point
		{
			Die();
		}

		if (Normal || Hard)
		{
			if (shark.Alive == FALSE && rand()%1000 == 1)
				shark.Create();
			else if (shark.Alive == TRUE)
			{
				shark.xpos += shark.movex;
				MemDC_Shark.SelectObject(&shark.bmp);
				pDC->TransparentBlt(shark.xpos, shark.ypos, 200, 122, &MemDC_Shark, 0, 0, 82, 50, RGB(255, 0, 0));
				shark.rect.SetRect(shark.xpos+20, shark.ypos+10, shark.xpos + 180, shark.ypos + 110);
				if (shark.xpos > 2000 || shark.xpos < -200)
					shark.Alive = FALSE;
				if (IntersectRect(&rcTemp, &MyFishRect, &shark.rect)) // Crash Point
					Die();
			}
		}
		//piranha
		if (Hard)
		{
			prh.n++;
			if (prh.n == 100)
			{
				prh.n = 0;
				PrhTransition();
			}
			prh.xpos += prh.move_x;
			prh.ypos += prh.move_y;
			MemDC_Prh.SelectObject(&prh.bmp);
			pDC->TransparentBlt(prh.xpos, prh.ypos, 120, 90, &MemDC_Prh, 0, 0, 200, 150, RGB(230, 0, 18));
			if (prh.move_x < 0)		//오른쪽
				prh.rect.SetRect(prh.xpos+8, prh.ypos+8, prh.xpos + 112, prh.ypos + 82);
			else					//왼쪽
				prh.rect.SetRect(prh.xpos+8, prh.ypos+8, prh.xpos + 112, prh.ypos + 82);
			if (IntersectRect(&rcTemp, &MyFishRect, &prh.rect)) // Crash Point
			{
				Die();
			}
		
		}
	}
}


void CProjectView::OnShop()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	m_pDlg = new CShopDialog();
	m_pDlg->m_pView = this;
	m_pDlg->coinnum = coin;
	m_pDlg->Create(IDD_SHOP);
	m_pDlg->ShowWindow(SW_SHOW);
	m_pDlg->fish = fish;

	m_pDlg->f1 = f1;
	m_pDlg->f2 = f2;
	m_pDlg->f3 = f3;
	m_pDlg->f4 = f4;
	m_pDlg->f5 = f5;
	m_pDlg->f6 = f6;;
	m_pDlg->f7 = f7;
	m_pDlg->f8 = f8;
	m_pDlg->Invalidate();

}


BOOL CProjectView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//return TRUE;
	return CFormView::OnEraseBkgnd(pDC);
}


void CProjectView::OnBnClickedNewGame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	sndPlaySound(MAKEINTRESOURCE(IDR_WAVE2), SND_ASYNC | SND_RESOURCE);

	Button_New_Game.ShowWindow(0);
	Button_Load_Game.ShowWindow(0);
	Button_End.ShowWindow(0);
	
	Button_New_Game.EnableWindow(0);
	Button_Load_Game.EnableWindow(0);
	Button_End.EnableWindow(0);

	Button_Easy.ShowWindow(1);
	Button_Normal.ShowWindow(1);
	Button_Hard.ShowWindow(1);

}


void CProjectView::OnBnClickedLoadGame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	sndPlaySound(MAKEINTRESOURCE(IDR_WAVE2), SND_ASYNC | SND_RESOURCE);
	CFile file;
	CFileException e;
	if (!file.Open(_T("SaveFile.dat"), CFile::modeRead, &e))
	{
		e.ReportError();
		return;
	}
	CArchive ar(&file, CArchive::load);
	Serialize(ar);

	Button_New_Game.ShowWindow(0);
	Button_Load_Game.ShowWindow(0);
	Button_End.ShowWindow(0);
	
	Button_New_Game.EnableWindow(0);
	Button_Load_Game.EnableWindow(0);
	Button_End.EnableWindow(0);
	
	Button_Easy.ShowWindow(1);
	Button_Normal.ShowWindow(1);
	Button_Hard.ShowWindow(1);
}


void CProjectView::OnBnClickedEnd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	sndPlaySound(MAKEINTRESOURCE(IDR_WAVE2), SND_ASYNC | SND_RESOURCE);
	exit(1);
}


void CProjectView::OnBnClickedEasy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	sndPlaySound(MAKEINTRESOURCE(IDR_WAVE2), SND_ASYNC | SND_RESOURCE);
	current_word = _T("");
	current_word_length = 0;
	SetDlgItemText(IDC_CURRENTWORD, current_word);
	whichlevel = 1;
	//Read File "EasyList.txt" 
	Easy = TRUE;
	Normal = FALSE;
	Hard = FALSE;

	xpos = 900;
	ypos = 500;
	CStdioFile tip_file;

	if (tip_file.Open(_T("EasyList.txt"), CFile::modeRead | CFile::typeText)) {
		CString str;
		CString word, mean;
		while (tip_file.ReadString(str)) {

			AfxExtractSubString(word, str, 0, ' ');
			AfxExtractSubString(mean, str, 1, ' ');
			Word.Add(word);
			Mean.Add(mean);
		}
		tip_file.Close();
	}
	//end
	wIndex = rand() % (Word.GetCount() - 1);
	main.DeleteObject();
	brush.DeleteObject();
	main.LoadBitmap(IDB_BACKGROUND);
	brush.CreatePatternBrush(&main);
	Invalidate();

	Button_Easy.ShowWindow(0);
	Button_Normal.ShowWindow(0);
	Button_Hard.ShowWindow(0);

	Button_Easy.EnableWindow(0);
	Button_Normal.EnableWindow(0);
	Button_Hard.EnableWindow(0);

	Start = TRUE;
	bubble_num = 15;

	word = Word.GetAt(wIndex);
	word_length = word.GetLength();
	for (int i = 0; i < word_length; ++i)
	{
		TCHAR t = word.GetAt(i);
		if (!(t >= 'A' && t <= 'Z'))
		{
			t += ('A' - 'a');
		}
		bubble[i].bmp.DeleteObject();
		bubble[i].Create(t - 'A');
		Bubble_Rect.Add(CRect(bubble[i].xpos + 5, bubble[i].ypos + 5, bubble[i].xpos + 25, bubble[i].ypos + 25));
	}
	for (int i = word_length; i < bubble_num; ++i)
	{
		int t = rand() % 26;
		bubble[i].bmp.DeleteObject();
		bubble[i].Create(t);
		Bubble_Rect.Add(CRect(bubble[i].xpos + 5, bubble[i].ypos + 5, bubble[i].xpos + 25, bubble[i].ypos + 25));
	}

	SetDlgItemText(IDC_WORD, Word.GetAt(wIndex));
	SetDlgItemText(IDC_MEAN, Mean.GetAt(wIndex));
	coin_num.Format(_T("%d"), coin);
	SetDlgItemText(IDC_COIN, coin_num);

}


void CProjectView::OnBnClickedNormal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	sndPlaySound(MAKEINTRESOURCE(IDR_WAVE2), SND_ASYNC | SND_RESOURCE);
	current_word = _T("");
	current_word_length = 0;
	SetDlgItemText(IDC_CURRENTWORD, current_word);
	whichlevel = 2;
	main.DeleteObject();
	brush.DeleteObject();
	//Read File "NormalList.txt" 
	Easy = FALSE;
	Normal = TRUE;
	Hard = FALSE;

	xpos = 900;
	ypos = 500;

	CStdioFile tip_file;
	if (tip_file.Open(_T("NormalList.txt"), CFile::modeRead | CFile::typeText)) {
		CString str;
		CString word, mean;
		while (tip_file.ReadString(str)) {

			AfxExtractSubString(word, str, 0, ' ');
			AfxExtractSubString(mean, str, 1, ' ');
			Word.Add(word);
			Mean.Add(mean);
		}
		tip_file.Close();
	}
	//end
	wIndex = rand() % (Word.GetCount() - 1);
	main.LoadBitmap(IDB_BACKGROUND);
	brush.CreatePatternBrush(&main);
	Invalidate();
	Button_Easy.ShowWindow(0);
	Button_Normal.ShowWindow(0);
	Button_Hard.ShowWindow(0);

	Button_Easy.EnableWindow(0);
	Button_Normal.EnableWindow(0);
	Button_Hard.EnableWindow(0);

	Start = TRUE;
	bubble_num = 20;

	word = Word.GetAt(wIndex);
	word_length = word.GetLength();
	for (int i = 0; i < word_length; ++i)
	{
		TCHAR t = word.GetAt(i);
		if (!(t >= 'A' && t <= 'Z'))
		{
			t += ('A' - 'a');
		}
		bubble[i].bmp.DeleteObject();
		bubble[i].Create(t - 'A');
		Bubble_Rect.Add(CRect(bubble[i].xpos + 5, bubble[i].ypos + 5, bubble[i].xpos + 25, bubble[i].ypos + 25));
	}
	for (int i = word_length; i < bubble_num; ++i)
	{
		int t = rand() % 26;
		bubble[i].bmp.DeleteObject();
		bubble[i].Create(t);
		Bubble_Rect.Add(CRect(bubble[i].xpos + 5, bubble[i].ypos + 5, bubble[i].xpos + 25, bubble[i].ypos + 25));
	}

	SetDlgItemText(IDC_WORD, Word.GetAt(wIndex));
	SetDlgItemText(IDC_MEAN, Mean.GetAt(wIndex));
	coin_num.Format(_T("%d"), coin);
	SetDlgItemText(IDC_COIN, coin_num);
}


void CProjectView::OnBnClickedHard()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	sndPlaySound(MAKEINTRESOURCE(IDR_WAVE2), SND_ASYNC | SND_RESOURCE);
	current_word = _T("");
	current_word_length = 0;
	SetDlgItemText(IDC_CURRENTWORD, current_word);
	whichlevel = 3;
	main.DeleteObject();
	brush.DeleteObject();

	//	Read File "HardList.txt"
	Easy = FALSE;
	Normal = FALSE;
	Hard = TRUE;

	xpos = 900;
	ypos = 500;

	CStdioFile tip_file;
	if (tip_file.Open(_T("HardList.txt"), CFile::modeRead | CFile::typeText)) {
		CString str;
		CString word, mean;
		while (tip_file.ReadString(str)) {

			AfxExtractSubString(word, str, 0, ' ');
			AfxExtractSubString(mean, str, 1, ' ');
			Word.Add(word);
			Mean.Add(mean);
		}
		tip_file.Close();
	}
	//end
	wIndex = rand() % (Word.GetCount() - 1);
	main.LoadBitmap(IDB_BACKGROUND);
	brush.CreatePatternBrush(&main);
	Invalidate();
	Button_Easy.ShowWindow(0);
	Button_Normal.ShowWindow(0);
	Button_Hard.ShowWindow(0);

	Button_Easy.EnableWindow(0);
	Button_Normal.EnableWindow(0);
	Button_Hard.EnableWindow(0);

	Start = TRUE;
	bubble_num = 20;

	word = Word.GetAt(wIndex);
	word_length = word.GetLength();
	for (int i = 0; i < word_length; ++i)
	{
		TCHAR t = word.GetAt(i);
		if (!(t >= 'A' && t <= 'Z'))
		{
			t += ('A' - 'a');
		}
		bubble[i].bmp.DeleteObject();
		bubble[i].Create(t - 'A');
		Bubble_Rect.Add(CRect(bubble[i].xpos + 5, bubble[i].ypos + 5, bubble[i].xpos + 25, bubble[i].ypos + 25));
	}
	for (int i = word_length; i < bubble_num; ++i)
	{
		int t = rand() % 26;
		bubble[i].bmp.DeleteObject();
		bubble[i].Create(t);
		Bubble_Rect.Add(CRect(bubble[i].xpos + 5, bubble[i].ypos + 5, bubble[i].xpos + 25, bubble[i].ypos + 25));
	}

	prh.Create();
	PrhTransition();

	SetDlgItemText(IDC_WORD, Word.GetAt(wIndex));
	SetDlgItemText(IDC_MEAN, Mean.GetAt(wIndex));
	coin_num.Format(_T("%d"), coin);
	SetDlgItemText(IDC_COIN, coin_num);

}


void CProjectView::OnBnClickedRestart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	sndPlaySound(MAKEINTRESOURCE(IDR_WAVE2), SND_ASYNC | SND_RESOURCE);
	xpos = -100;
	ypos = -100;
	MyFishRect.SetRectEmpty();
	Alive = TRUE;
	Button_Easy.ShowWindow(1);
	Button_Normal.ShowWindow(1);
	Button_Hard.ShowWindow(1);
	
	Button_Easy.EnableWindow(1);
	Button_Normal.EnableWindow(1);
	Button_Hard.EnableWindow(1);

	Button_Restart.ShowWindow(0);
	Button_End2.ShowWindow(0);

	Button_Restart.EnableWindow(0);
	Button_End2.EnableWindow(0);

	Word.RemoveAll();
	Mean.RemoveAll();
	Bubble_Rect.RemoveAll();
	main.DeleteObject();
	brush.DeleteObject();
	main.LoadBitmap(IDB_BACKGROUND);
	brush.CreatePatternBrush(&main);
	Invalidate();
	current_word = _T("");
	current_word_length = 0;

	SetDlgItemText(IDC_CURRENTWORD, current_word);
}


void CProjectView::OnBnClickedEnd2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	sndPlaySound(MAKEINTRESOURCE(IDR_WAVE2), SND_ASYNC | SND_RESOURCE);
	CFile file;
	CFileException e;
	if (!file.Open(_T("SaveFile.dat"), CFile::modeWrite | CFile::modeCreate, &e))
	{
		e.ReportError();
		return;
	}
	CArchive ar(&file, CArchive::store);
	Serialize(ar);
	GetParent()->PostMessage(WM_CLOSE,NULL,NULL);
}


HBRUSH CProjectView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	UINT nID = pWnd->GetDlgCtrlID();
	pDC->SelectObject(&font);
	switch (nID)
	{
	case IDC_WORD:
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
		break;
	case IDC_MEAN:
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
		break;
	case IDC_CURRENTWORD:
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
		break;
	case IDC_COIN:
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CProjectView::Restart()
{
	main.DeleteObject();
	brush.DeleteObject();
	xpos = 900;
	ypos = 500;
	current_word = _T("");
	current_word_length = 0;
	word = _T("");
	Bubble_Rect.RemoveAll();
	wIndex = rand() % (Word.GetCount() - 1);

	word = Word.GetAt(wIndex);
	word_length = word.GetLength();
	onetime = FALSE;
	SetDlgItemText(IDC_WORD, Word.GetAt(wIndex));
	SetDlgItemText(IDC_MEAN, Mean.GetAt(wIndex));
	SetDlgItemText(IDC_CURRENTWORD, current_word);
	for (int i = 0; i < coin_inmap; ++i)
	{
		Coin[i].bmp.DeleteObject();
	}
	coin_inmap = 0;
	for (int i = 0; i < word_length; ++i)
	{
		TCHAR t = word.GetAt(i);
		if (!(t >= 'A' && t <= 'Z'))
		{
			t += ('A' - 'a');
		}
		bubble[i].bmp.DeleteObject();
		bubble[i].Create(t - 'A');
		Bubble_Rect.Add(CRect(bubble[i].xpos + 5, bubble[i].ypos + 5, bubble[i].xpos + 25, bubble[i].ypos + 25));
	}
	for (int i = word_length; i < bubble_num; ++i)
	{
		int t = rand() % 26;
		bubble[i].bmp.DeleteObject();
		bubble[i].Create(t);
		Bubble_Rect.Add(CRect(bubble[i].xpos + 5, bubble[i].ypos + 5, bubble[i].xpos + 25, bubble[i].ypos + 25));
	}
	SH.bmp.DeleteObject();
	SH.Create();
	oct.bmp.DeleteObject();
	oct.Create();
	prh.bmp.DeleteObject();
	PrhTransition();
	prh.Create();
	shark.Alive = FALSE;
	shark.bmp.DeleteObject();
}

void CProjectView::Die()
{
	Alive = FALSE;
	Start = FALSE;
	xpos = 960;
	ypos = 500;
	GetDlgItem(IDC_WORD)->ShowWindow(FALSE);
	GetDlgItem(IDC_MEAN)->ShowWindow(FALSE);
	GetDlgItem(IDC_CURRENTWORD)->ShowWindow(FALSE);
	GetDlgItem(IDC_COIN)->ShowWindow(FALSE);
	main.DeleteObject();
	brush.DeleteObject();
	main.LoadBitmap(IDB_GAMEOVER);
	brush.CreatePatternBrush(&main);
	SH.bmp.DeleteObject();
	SH.Create();
	oct.bmp.DeleteObject();
	oct.Create();
	prh.bmp.DeleteObject();
	PrhTransition();
	prh.Create();
	shopopen = TRUE;
	shark.Create();
	shark.rect.SetRectEmpty();
	shark.Alive = FALSE;
	Button_Restart.ShowWindow(1);
	Button_End2.ShowWindow(1);
	Button_Restart.EnableWindow(1);
	Button_End2.EnableWindow(1);
	sndPlaySound(MAKEINTRESOURCE(IDR_WAVE4), SND_LOOP | SND_RESOURCE | SND_ASYNC);
}


void CProjectView::PrhTransition()
{
	prh.bmp.DeleteObject();
	if ((xpos - prh.xpos) < 0 && (ypos - prh.ypos) < 0) {
		prh.move_x = -1 * (rand() % 3 + 1);
		prh.move_y = -1 * (rand() % 3 + 1);
		prh.bmp.LoadBitmap(IDB_ENEMY_PIRANHA);
	}
	else if ((xpos - prh.xpos) > 0 && (ypos - prh.ypos) < 0) {
		prh.move_x = (rand() % 3 + 1);
		prh.move_y = -1 * (rand() % 3 + 1);
		prh.bmp.LoadBitmap(IDB_ENEMY_PIRANHA_REVERSE);
	}
	else if ((xpos - prh.xpos) > 0 && (ypos - prh.ypos) > 0) {
		prh.move_x = (rand() % 3 + 1);
		prh.move_y = (rand() % 3 + 1);
		prh.bmp.LoadBitmap(IDB_ENEMY_PIRANHA_REVERSE);
	}
	else {
		prh.move_x = -1 * (rand() % 3 + 1);
		prh.move_y = (rand() % 3 + 1);
		prh.bmp.LoadBitmap(IDB_ENEMY_PIRANHA);
	}
}


/*void CProjectView::Wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dwMillisecond)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}*/


void CProjectView::OnUpdateShop(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(shopopen);
}


void CProjectView::Clear()
{
	CDC *pDC = GetDC();
	CDC CLEAR;
	CBitmap bmp_clear;
	CLEAR.CreateCompatibleDC(pDC);
	Alive = FALSE;
	Start = FALSE;
	xpos = 960;
	ypos = 500;
	GetDlgItem(IDC_WORD)->ShowWindow(FALSE);
	GetDlgItem(IDC_MEAN)->ShowWindow(FALSE);
	GetDlgItem(IDC_CURRENTWORD)->ShowWindow(FALSE);
	GetDlgItem(IDC_COIN)->ShowWindow(FALSE);
	Button_nextstage.ShowWindow(1);

	onetime = FALSE;
	main.DeleteObject();
	brush.DeleteObject();
	main.LoadBitmap(IDB_GAME_CLEAR);
	brush.CreatePatternBrush(&main);

	SH.bmp.DeleteObject();
	SH.Create();
	oct.bmp.DeleteObject();
	oct.Create();
	prh.bmp.DeleteObject();
	PrhTransition();
	prh.Create();

	shark.Alive = FALSE;
	shopopen = TRUE;
	shark.bmp.DeleteObject();

}


void CProjectView::OnBnClickedContinue()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	sndPlaySound(MAKEINTRESOURCE(IDR_WAVE2), SND_ASYNC | SND_RESOURCE);
	main.DeleteObject();
	brush.DeleteObject();
	Alive = TRUE;
	Button_nextstage.ShowWindow(0);
	Word.RemoveAll();
	Mean.RemoveAll();
	Bubble_Rect.RemoveAll();
	main.DeleteObject();
	brush.DeleteObject();
	main.LoadBitmap(IDB_BACKGROUND);
	brush.CreatePatternBrush(&main);
	Invalidate();
	current_word = _T("");
	current_word_length = 0;
	SetDlgItemText(IDC_CURRENTWORD, current_word);
	if (whichlevel == 1) {
		OnBnClickedEasy();
	}
	else if (whichlevel == 2) {
		OnBnClickedNormal();
	}
	else if (whichlevel == 3) {
		OnBnClickedHard();
	}
}


void CProjectView::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CFile file;
	CFileException e;
	if (!file.Open(_T("SaveFile.dat"), CFile::modeWrite | CFile::modeCreate, &e))
	{
		e.ReportError();
		return;
	}
	CArchive ar(&file, CArchive::store);
	Serialize(ar);
	CFormView::PostNcDestroy();
}


void CProjectView::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		
		ar << coin << f1 << f2 << f3 << f4 << f5 << f6 << f7 << f8 ;

	}
	else
	{	// loading code
		ar >> coin >> f1 >> f2 >> f3 >> f4 >> f5 >> f6 >> f7 >> f8 ;
	}
}


void CProjectView::OnInfo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString str;
	str.Format(_T("물고기는 키보드 방향키를 이용해 움직입니다!\n\t(←, ↑, →, ↓)\n창 상단에 뜬 영어단어 알파벳 순서대로 화면에 있는 버블을 먹어주세요!\n(주의! : 당신을 노리는 적을 조심하세요!)"));
	MessageBox(str,_T("Ictus Infomation"),IDOK);
}
