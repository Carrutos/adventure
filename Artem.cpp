#include <windows.h>
#include <string>

struct {
	HWND hWnd;
	HDC device_context, context;
	int width, height;
} window;

struct obj {
	float x, y;
	int width, height, speed, dirx, diry;
	HBITMAP hBitmap;
};

struct boxl1 {
	float x, y;
	int width = window.width / 8;
	int height = window.height / 8;
	HBITMAP hBitmap = (HBITMAP)LoadImageA(NULL, "mandrill.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
};

struct boxl2 {
	float x, y;
	int width = window.width / 7;
	int height = window.height / 8;
	HBITMAP hBitmap = (HBITMAP)LoadImageA(NULL, "tiger.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
};

obj racket, ball;
boxl1 box11, box12, box13, box14, box15, box16, box17, box18, box21, box22, box23, box24, box25, box26, box27, box31, box32, box33, box34,
box35, box36;
boxl2 box41, box42, box43, box44, box45, box46, box47, box51, box52, box53, box54, box55, box56, box61, box62, box63, box64, box65;
boxl1 boxes1[]{ box11, box12, box13, box14, box15, box16, box17, box18, box21, box22, box23, box24, box25, box26, box27, box31, box32, box33, box34,
box35, box36 };
boxl2 boxes2[]{ box41, box42, box43, box44, box45, box46, box47, box51, box52, box53, box54, box55, box56, box61, box62, box63, box64, box65 };
int lives = 3;
int score = 0;
int level = 1;
HBITMAP hBack;

void InitGame() {
	racket.x = window.height / 2 - 75;
	racket.y = window.height - 30;
	racket.width = 150;
	racket.height = 30;
	racket.speed = 10;
	ball.x = window.width / 2 - 50;
	ball.y = window.height - 130;
	ball.width = ball.height = 100;
	ball.speed = 8;
	racket.hBitmap = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	ball.hBitmap = (HBITMAP)LoadImageA(NULL, "ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	box11.y = box12.y = box13.y = box14.y = box15.y = box16.y = box17.y = box18.y = 50;
	box21.y = box22.y = box23.y = box24.y = box25.y = box26.y = box27.y = 50 + window.height / 8;
	box31.y = box32.y = box33.y = box34.y = box35.y = box36.y = 50 + window.height / 4;
	box41.y = box42.y = box43.y = box44.y = box45.y = box46.y = box47.y = -200 - window.height / 4;
	box51.y = box52.y = box53.y = box54.y = box55.y = box56.y = -200 - window.height / 8;
	box61.y = box62.y = box63.y = box64.y = box65.y = -200;
	box11.x = box41.x = 0;
	box12.x = box31.x = window.width / 8;
	box13.x = box32.x = window.width / 4;
	box14.x = box33.x = window.width / 8 * 3;
	box15.x = box34.x = window.width / 2;
	box16.x = box35.x = window.width / 8 * 5;
	box17.x = box36.x = window.width / 8 * 6;
	box18.x = window.width / 8 * 7;
	box21.x = window.width / 16;
	box22.x = window.width / 16 * 3;
	box23.x = window.width / 16 * 5;
	box24.x = window.width / 16 * 7;
	box25.x = window.width / 16 * 9;
	box26.x = window.width / 16 * 11;
	box27.x = window.width / 16 * 13;
	box42.x = box61.x = window.width / 7;
	box43.x = box62.x = window.width / 7 * 2;
	box44.x = box63.x = window.width / 7 * 3;
	box45.x = box64.x = window.width / 7 * 4;
	box46.x = box65.x = window.width / 7 * 5;
	box47.x = window.width / 7 * 6;
	box51.x = window.width / 14;
	box52.x = window.width / 14 * 3;
	box53.x = window.width / 14 * 5;
	box54.x = window.width / 14 * 7;
	box55.x = window.width / 14 * 9;
	box56.x = window.width / 14 * 11;
	hBack = (HBITMAP)LoadImageA(NULL, "jungle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void setText() {
	SetTextColor(window.context, RGB(250, 250, 250));
	SetBkMode(window.context, TRANSPARENT);
	auto hFont = CreateFont(50, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 10, 0, L"CALIBRI");
	auto hTmp = (HFONT)SelectObject(window.context, hFont);
}

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false)
{
	HBITMAP hbm, hOldbm;
	HDC hMemDC;
	BITMAP bm;

	hMemDC = CreateCompatibleDC(hDC);
	hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);

	if (hOldbm) {
		GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm);
		StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		SelectObject(hMemDC, hOldbm);
	}
	DeleteDC(hMemDC);
}

void InitWindow() {
	SetProcessDPIAware();
	window.hWnd = CreateWindow(L"edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

	RECT r;
	GetClientRect(window.hWnd, &r);
	window.device_context = GetDC(window.hWnd);
	window.width = r.right - r.left;
	window.height = r.bottom - r.top;
	window.context = CreateCompatibleDC(window.device_context);
	SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));
	GetClientRect(window.hWnd, &r);
}

void ballMove() {
	if (ball.dirx > 0) {
		ball.x += ball.speed;
	}
	else {
		ball.x -= ball.speed;
	}
	if (ball.diry > 0) {
		ball.y += ball.speed;
	}
	else {
		ball.y -= ball.speed;
	}
}

void coll() {
	if (ball.x <= 0) {
		ball.x == 0;
		ball.dirx = 1;
	}
	if (ball.x + ball.width >= window.width) {
		ball.x = window.width;
		ball.dirx = -1;
	}
	if (ball.y <= 0) {
		ball.y == 0;
		ball.diry = 1;
	}
	if (level == 1) {
		for (int i = 0; i < 21; i++) {
			if (ball.x + ball.width >= boxes1[i].x && ball.y < boxes1[i].y + boxes1[i].height && ball.y + ball.height > boxes1[i].y && ball.x + ball.width <= boxes1[i].x + ball.speed) {
				ball.x = boxes1[i].x - 1 - ball.width;
				ball.dirx = -1;
				boxes1[i].x = boxes1[i].y = -200;
			}
			if (ball.x <= boxes1[i].x + boxes1[i].width && ball.y < boxes1[i].y + boxes1[i].height && ball.y + ball.height > boxes1[i].y && ball.x >= boxes1[i].x + boxes1[i].width - ball.speed) {
				ball.x = boxes1[i].x + boxes1[i].width + 1;
				ball.dirx = 1;
				boxes1[i].x = boxes1[i].y = -200;
			}
			if (ball.y <= boxes1[i].y + boxes1[i].height && ball.x < boxes1[i].x + boxes1[i].width && ball.x + ball.width > boxes1[i].x && ball.y >= boxes1[i].y + boxes1[i].height - ball.speed) {
				ball.y = boxes1[i].y + boxes1[i].height + 1;
				ball.diry = 1;
				boxes1[i].x = boxes1[i].y = -200;
			}
			if (ball.y + ball.height >= boxes1[i].y && ball.x < boxes1[i].x + boxes1[i].width && ball.x + ball.width > boxes1[i].x && ball.y + ball.height <= boxes1[i].y + ball.speed) {
				ball.y = boxes1[i].y - 1 - ball.height;
				ball.diry = -1;
				boxes1[i].x = boxes1[i].y = -200;
			}
		}
	}
	else if (level == 2) {
		for (int i = 0; i < 18; i++) {
			if (ball.x + ball.width >= boxes2[i].x && ball.y < boxes2[i].y + boxes2[i].height && ball.y + ball.height > boxes2[i].y && ball.x + ball.width <= boxes2[i].x + ball.speed) {
				ball.x = boxes2[i].x - 1 - ball.width;
				ball.dirx = -1;
				boxes2[i].x = boxes2[i].y = -200;
			}
			if (ball.x <= boxes2[i].x + boxes2[i].width && ball.y < boxes2[i].y + boxes2[i].height && ball.y + ball.height > boxes2[i].y && ball.x >= boxes2[i].x + boxes2[i].width - ball.speed) {
				ball.x = boxes2[i].x + boxes2[i].width + 1;
				ball.dirx = 1;
				boxes2[i].x = boxes2[i].y = -200;
			}
			if (ball.y <= boxes2[i].y + boxes2[i].height && ball.x < boxes2[i].x + boxes2[i].width && ball.x + ball.width > boxes2[i].x && ball.y >= boxes2[i].y + boxes2[i].height - ball.speed) {
				ball.y = boxes2[i].y + boxes2[i].height + 1;
				ball.diry = 1;
				boxes2[i].x = boxes2[i].y = -200;
			}
			if (ball.y + ball.height >= boxes2[i].y && ball.x < boxes2[i].x + boxes2[i].width && ball.x + ball.width > boxes2[i].x && ball.y + ball.height <= boxes2[i].y + ball.speed) {
				ball.y = boxes2[i].y - 1 - ball.height;
				ball.diry = -1;
				boxes2[i].x = boxes2[i].y = -200;
			}
		}
	}
	if (ball.x + ball.width >= racket.x && ball.y < racket.y + racket.height && ball.y + ball.height > racket.y && ball.x + ball.width <= racket.x + ball.speed) {
		ball.x = racket.x - 1 - ball.width;
		ball.dirx = -1;
	}
	if (ball.x <= racket.x + racket.width && ball.y < racket.y + racket.height && ball.y + ball.height > racket.y && ball.x >= racket.x + racket.width - ball.speed) {
		ball.x = racket.x + racket.width + 1;
		ball.dirx = 1;
	}
	if (ball.y + ball.height >= racket.y && ball.x < racket.x + racket.width && ball.x + ball.width > racket.x && ball.y + ball.height <= racket.y + ball.speed) {
		ball.y = racket.y - 1 - ball.height;
		ball.diry = -1;
	}
}

void newLevel() {
	int hit = 0;
	for (int i = 0; i < 21; i++) {
		if (boxes1[i].x == -200) {
			hit++;
		}
	}
	if (hit == 21) {
		ball.x = window.width / 2 - 50;
		ball.y = window.height - 130;
		while (box41.y != 50) {
			for (int i = 0; i < 18; i++) {
				boxes2[i].y += 5;
			}
		}
	}
}

std::string A = "score: " + std::to_string((int)score);
std::string B = "lives: " + std::to_string((int)lives);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {

	//UNREFERENCED_PARAMETER(hPrevInstance);
	//UNREFERENCED_PARAMETER(lpCmdLine);
	InitGame();
	InitWindow();
	setText();
	ShowCursor(NULL);
	while (not(GetAsyncKeyState('A')) && not(GetAsyncKeyState('D'))) {
		TextOutA(window.context, window.width / 2 - 100, window.height / 2, "Rumble in the Jungle", 20);
		TextOutA(window.context, window.width / 2 - 105, window.height / 2 + 100, "Press A or D to begin", 21);
		if (GetAsyncKeyState('A')) {
			ball.dirx = -1;
		}
		else if (GetAsyncKeyState('D')) {
			ball.dirx = 1;
		}
	}
	while (not(GetAsyncKeyState(VK_ESCAPE))) {
		TextOutA(window.context, 10, window.height - 100, A.c_str(), 9);
		TextOutA(window.context, 10, window.height - 50, B.c_str(), 9);
		ShowBitmap(window.context, 0, 0, window.width, window.height, hBack);
		ShowBitmap(window.context, racket.x, racket.y, racket.width, racket.height, racket.hBitmap);
		ShowBitmap(window.context, ball.x, ball.y, ball.width, ball.height, ball.hBitmap);
		for (int i = 0; i < 21; i++) {
			ShowBitmap(window.context, boxes1[i].x, boxes1[i].y, boxes1[i].width, boxes1[i].height, boxes1[i].hBitmap);
		}
		for (int i = 0; i < 18; i++) {
			ShowBitmap(window.context, boxes2[i].x, boxes2[i].y, boxes2[i].width, boxes2[i].height, boxes2[i].hBitmap);
		}
		coll();
		ballMove();
		newLevel();
		if (GetAsyncKeyState('A')) {
			racket.x -= racket.speed;
		}
		else if (GetAsyncKeyState('D')) {
			racket.x += racket.speed;
		}
		if (ball.x == window.height - ball.height) {
			lives -= 1;
			ball.x = window.width / 2 - 50;
			ball.y = window.height - 130;
		}
		if (lives == 0) {
			break;
		}
		BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
		Sleep(16);
	}
}
