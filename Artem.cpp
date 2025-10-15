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

struct box {
	float x, y;
	int width;
	int height;
};

obj racket, ball;
box boxes[39]{};
int lives = 3;
int score = 0;
int level = 1;
int ballSafe = -200;
int past = 0;
int rayx, rayy, hitx, hity, hitd;
bool diry;
HBITMAP hBack, mandrill, tiger;

void InitGame() {
	racket.x = window.width / 2 - 75;
	racket.y = window.height - 30;
	racket.width = 150;
	racket.height = 30;
	racket.speed = 10;
	ball.x = window.width / 2 - 50;
	ball.y = window.height - 130;
	ball.width = ball.height = 100;
	ball.speed = 8;
	ball.dirx = ball.diry = 0;
	diry = false;
	racket.hBitmap = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	ball.hBitmap = (HBITMAP)LoadImageA(NULL, "ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	mandrill = (HBITMAP)LoadImageA(NULL, "mandrill.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	tiger = (HBITMAP)LoadImageA(NULL, "tiger.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	for (int i = 0; i < 39; i++) {
		//position
		if (i < 8) {
			boxes[i].x = window.width / 8 * i;
			boxes[i].y = 50;
		}
		else if (i < 15) {
			boxes[i].x = window.width / 8 * (i - 8) + window.width / 16;
			boxes[i].y = 50 + window.height / 8;
		}
		else if (i < 21) {
			boxes[i].x = window.width / 8 * (i - 14);
			boxes[i].y = 50 + window.height / 4;
		}
		else if (i < 28) {
			boxes[i].x = window.width / 7 * (i - 21);
			boxes[i].y = ballSafe - window.height / 4;
		}
		else if (i < 34) {
			boxes[i].x = window.width / 7 * (i - 28) + window.width / 14;
			boxes[i].y = ballSafe - window.height / 8;
		}
		else {
			boxes[i].x = window.width / 7 * (i - 33);
			boxes[i].y = ballSafe;
		}
		//size
		if (i < 21) {
			boxes[i].width = window.width / 8;
			boxes[i].height = window.height / 8;
		}
		else {
			boxes[i].width = window.width / 7;
			boxes[i].height = window.height / 8;
		}
	}
	hBack = (HBITMAP)LoadImageA(NULL, "jungle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void setText() {
	SetTextColor(window.context, RGB(250, 250, 250));
	SetBkMode(window.context, TRANSPARENT);
	auto hFont = CreateFont(50, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 10, 0, L"CALIBRI");
	auto hTmp = (HFONT)SelectObject(window.context, hFont);
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

void ballMove() {
	int move = 0;
	if (past == ball.speed * 2) {
		move = ball.speed;
	}
	else if (past > ball.speed) {
		move = ball.speed - (past - ball.speed);
	}
	if (ball.dirx == 1) {
		ball.x += move;
	}
	else if (ball.dirx == -1) {
		ball.x -= move;
	}
	if (diry == true) {
		ball.y += move;
	}
	else if (diry == false) {
		ball.y -= move;
	}
}

void collWall() {
	if (ball.x - ball.speed <= 0) {
		ball.x = 1;
		ball.dirx = 1;
	}
	if (ball.x + ball.width + ball.speed >= window.width) {
		ball.x = window.width - ball.width - 1;
		ball.dirx = -1;
	}
	if (ball.y - ball.speed <= 0) {
		ball.y = 1;
		diry = true;
	}
}

void collBox() {
	bool hita = false;
	for (int i = 0; i < 21; i++) {
		if (level == 2) {
			i += 21;
		}
		if (ball.x + ball.width + ball.speed >= boxes[i].x && ball.y < boxes[i].y + boxes[i].height && ball.y + ball.height > boxes[i].y && ball.x + ball.width <= boxes[i].x + ball.speed * 2) {
			past += ball.x + ball.width - boxes[i].x - 1;
			ball.x = boxes[i].x - ball.width - 1;
			ball.dirx = -1;
			score++;
			boxes[i].x = boxes[i].y = ballSafe;
			hita = true;
			break;
		}
		if (ball.x - ball.speed <= boxes[i].x + boxes[i].width && ball.y < boxes[i].y + boxes[i].height && ball.y + ball.height > boxes[i].y && ball.x >= boxes[i].x + boxes[i].width - ball.speed * 2) {
			past += ball.x - boxes[i].x - boxes[i].width + 1;
			ball.x = boxes[i].x + boxes[i].width + 1;
			ball.dirx = 1;
			score++;
			boxes[i].x = boxes[i].y = ballSafe;
			hita = true;
			break;
		}
		if (ball.y - ball.speed <= boxes[i].y + boxes[i].height && ball.x < boxes[i].x + boxes[i].width && ball.x + ball.width > boxes[i].x && ball.y >= boxes[i].y + boxes[i].height - ball.speed * 2) {
			past += ball.y - boxes[i].y - boxes[i].height + 1;
			ball.y = boxes[i].y + boxes[i].height + 1;
			ball.diry = 1;
			score++;
			boxes[i].x = boxes[i].y = ballSafe;
			hita = true;
			break;
		}
		if (ball.y + ball.height + ball.speed >= boxes[i].y && ball.x < boxes[i].x + boxes[i].width && ball.x + ball.width > boxes[i].x && ball.y + ball.height <= boxes[i].y + ball.speed * 2) {
			past += ball.y + ball.height - boxes[i].y - 1;
			ball.y = boxes[i].y - ball.height - 1;
			ball.diry = -1;
			score++;
			boxes[i].x = boxes[i].y = ballSafe;
			hita = true;
			break;
		}
		if (level == 2) {
			i -= 21;
		}
	}
	if (hita == false) {
		past = ball.speed * 2;
	}
}

void collRacket() {
	if (ball.x + ball.width + ball.speed >= racket.x && ball.y < racket.y + racket.height && ball.y + ball.height > racket.y && ball.x + ball.width <= racket.x + ball.speed * 2) {
		ball.x = racket.x - ball.width - 1;
		ball.dirx = -1;
	}
	if (ball.x - ball.speed <= racket.x + racket.width && ball.y < racket.y + racket.height && ball.y + ball.height > racket.y && ball.x >= racket.x + racket.width - ball.speed * 2) {
		ball.x = racket.x + racket.width + 1;
		ball.dirx = 1;
	}
	if (ball.y + ball.height + ball.speed >= racket.y && ball.x < racket.x + racket.width && ball.x + ball.width > racket.x && ball.y + ball.height <= racket.y + ball.speed * 2) {
		ball.y = racket.y - ball.height - 1;
		diry = false;
	}
}

void tutorial() {
	while (true) {
		ball.x = racket.x + 25;
		ball.y = window.height - 130;
		ShowBitmap(window.context, 0, 0, window.width, window.height, hBack);
		ShowBitmap(window.context, racket.x, racket.y, racket.width, racket.height, racket.hBitmap);
		ShowBitmap(window.context, ball.x, ball.y, ball.width, ball.height, ball.hBitmap);
		TextOutA(window.context, window.width / 2 - 100, window.height / 2, "Rumble in the Jungle", 20);
		TextOutA(window.context, window.width / 2 - 105, window.height / 2 + 100, "Press A or D to begin", 21);
		if (GetAsyncKeyState('A')) {
			ball.dirx = -1;
			break;
		}
		else if (GetAsyncKeyState('D')) {
			ball.dirx = 1;
			break;
		}
		BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
		Sleep(16);
	}
}

void newLevel() {
	int hit = 0;
	for (int i = 0; i < 21; i++) {
		if (boxes[i].x == ballSafe) {
			hit++;
		}
	}
	if (hit == 5) {
		level = 2;
		ball.x = window.width / 2 - 50;
		ball.y = window.height - 130;
		for (int i = 21; i < 39; i++) {
			boxes[i].y += 250 + window.height / 4;
		}
		tutorial();
	}
}

void die() {
	if (ball.y + 8 >= window.height - ball.height) {
		lives -= 1;
		diry = false;
		tutorial();
	}
}

void trace() {
	bool hita = false;
	if (ball.y - ball.speed <= hity && diry == false || ball.y + ball.height + ball.speed >= hity && diry == true) {
		hita = true;
		if (diry == false) {
			past += ball.y - hity;
			ball.x += (ball.y - hity) * ball.dirx;
			ball.y = hity;
		}
		else {
			past += hity - ball.y - ball.height;
			ball.x += (hity - ball.y - ball.height) * ball.dirx;
			ball.y = hity - ball.height;
		}
		if (hitd != -1) {
			boxes[hitd].x = boxes[hitd].y = ballSafe;
			score++;
		}
		if (hitx <= 4 || hitx >= window.width - 4 || hitd != -1 && boxes[hitd].y + ball.speed < hity && boxes[hitd].y + boxes[hitd].height - ball.speed > hity) {
			ball.dirx = -ball.dirx;
		}
		if (hitd != -1 && (boxes[hitd].y + ball.speed >= hity || boxes[hitd].y + boxes[hitd].height - ball.speed <= hity) || hity <= 4) {
			diry = not(diry);
		}
		hitd = -1;
	}
	if (hita == false && past == 0) {
		past = ball.speed * 2;
	}
	else if (hita == false) {
		past += ball.speed;
	}
}

void racketMove() {
	if (GetAsyncKeyState('A') && racket.x > 0) {
		racket.x -= racket.speed;
	}
	else if (GetAsyncKeyState('D') && racket.x + racket.width < window.width) {
		racket.x += racket.speed;
	}
}
void ShowImage() {

	ShowBitmap(window.context, 0, 0, window.width, window.height, hBack);
	ShowBitmap(window.context, racket.x, racket.y, racket.width, racket.height, racket.hBitmap);
	ShowBitmap(window.context, ball.x, ball.y, ball.width, ball.height, ball.hBitmap);
	for (int i = 0; i < 21; i++) {
		ShowBitmap(window.context, boxes[i].x, boxes[i].y, boxes[i].width, boxes[i].height, mandrill);
	}
	if (level == 2) {
		for (int i = 21; i < 39; i++) {
			ShowBitmap(window.context, boxes[i].x, boxes[i].y, boxes[i].width, boxes[i].height, tiger);
		}
	}
}

void rayConstr() {
	if (ball.dirx == 1) {
		rayx = ball.x + ball.width;
	}
	else if (ball.dirx == -1) {
		rayx = ball.x;
	}
	if (diry == true) {
		rayy = ball.y + ball.height;
	}
	else if (diry == false) {
		rayy = ball.y;
	}
	while (rayx < window.width && rayx > 0 && rayy < window.height && rayy > 0) {
		ShowBitmap(window.context, rayx, rayy, 6, 6, ball.hBitmap);
		bool hita = false;
		for (int i = 0; i < 21; i++) {
			if (level == 2) {
				i += 21;
			}
			if (rayy >= boxes[i].y && rayy <= boxes[i].y + boxes[i].height && rayx >= boxes[i].x && rayx <= boxes[i].x + boxes[i].width) {
				hitd = i;
				hity = rayy;
				hitx = rayx + ball.width / 2 * ball.dirx;
				hita = true;
				if (diry == true) {
					break;
				}
			}
			if (level == 2) {
				i -= 21;
			}
			if (level == 2 && i == 17) {
				break;
			}
		}
		if (hita == true) {
			break;
		}
		else {
			hity = rayy;
			hitx = rayx;
		}
		rayx = rayx + 4 * ball.dirx;
		if (diry == true) {
			rayy += 4;
		}
		else {
			rayy -= 4;
		}
	}
}

std::string A;
std::string B;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {
	InitWindow();
	InitGame();
	setText();
	ShowCursor(NULL);
	tutorial();
	while (not(GetAsyncKeyState(VK_ESCAPE))) {
		ShowImage();
		A = "score: " + std::to_string((int)score);
		B = "lives: " + std::to_string((int)lives);
		TextOutA(window.context, 10, window.height - 100, A.c_str(), 9);
		TextOutA(window.context, 10, window.height - 50, B.c_str(), 8);
		while (past < ball.speed) {
			//collWall();
			//collBox();
			trace();
			collRacket();
			rayConstr();
		}
		ballMove();
		past = 0;
		if (level == 1) {
			newLevel();
		}
		racketMove();
		die();
		if (lives == 0) {
			//break;
		}
		BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
		Sleep(16);
	}
}
