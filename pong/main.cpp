// linker::system::subsystem - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include "cmath"
#include <string>

// секция данных игры  
typedef struct {
    float x, y, width, height, rad, dx, dy, speed;
    HBITMAP hBitmap;//хэндл к спрайту шарика 
} sprite;

sprite oven;//ракетка игрока
sprite stove;//ракетка противника
sprite fridge;//шарик
sprite hero;
sprite statue1;
sprite statue2;
sprite statue3;
sprite table;
sprite enemy;

struct {
    int score, balls;//количество набранных очков и оставшихся "жизней"
    bool action = false;//состояние - ожидание (игрок должен нажать пробел) или игра
} game;

struct {
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width, height;//сюда сохраним размеры окна которое создаст программа
} window;

HBITMAP hBack;// хэндл для фонового изображения

//cекция кода

void InitGame()
{
    //в этой секции загружаем спрайты с помощью функций gdi
    //пути относительные - файлы должны лежать рядом с .exe 
    //результат работы LoadImageA сохраняет в хэндлах битмапов, рисование спрайтов будет произовдиться с помощью этих хэндлов
    oven.hBitmap = (HBITMAP)LoadImageA(NULL, "oven.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    stove.hBitmap = (HBITMAP)LoadImageA(NULL, "stove.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    fridge.hBitmap = (HBITMAP)LoadImageA(NULL, "fridge.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hero.hBitmap = (HBITMAP)LoadImageA(NULL, "ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    table.hBitmap = (HBITMAP)LoadImageA(NULL, "table.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    statue1.hBitmap = (HBITMAP)LoadImageA(NULL, "statue1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    statue2.hBitmap = (HBITMAP)LoadImageA(NULL, "statue2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    statue3.hBitmap = (HBITMAP)LoadImageA(NULL, "statue3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    enemy.hBitmap = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBack = (HBITMAP)LoadImageA(NULL, "marblefloor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //------------------------------------------------------

    fridge.width = 200;
    fridge.height = 200;
    fridge.x = 0;
    fridge.y = 0;

    stove.width = 200;
    stove.height = 200;
    stove.x = 200;
    stove.y = 0;

    oven.width = 200;
    oven.height = 200;
    oven.x = 400;
    oven.y = 0;

    table.width = 600;
    table.height = 300;
    table.x = window.width / 2 - 300;
    table.y = window.height - 300;

    statue1.width = 150;
    statue1.height = 150;
    statue1.x = window.width - 200;
    statue1.y = 0;

    statue3.width = 200;
    statue3.height = 400;
    statue3.x = 0;
    statue3.y = window.height - 400;

    statue2.width = 100;
    statue2.height = 300;
    statue2.x = window.width - 100;
    statue2.y = window.height - 400;

    hero.x = window.width / 4;
    hero.y = window.height / 3 + 20;
    hero.height = 150;
    hero.width = 150;
    hero.speed = 10;
    hero.rad = 75;

    enemy.x = window.width / 4 * 3 - 30;
    enemy.y = window.height - 200;
    enemy.speed = 5;
    enemy.height = 150;
    enemy.width = 150;

    game.score = 0;
    game.balls = 9;


}

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false)
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(hDC); // Создаем контекст памяти, совместимый с контекстом отображения
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);// Выбираем изображение bitmap в контекст памяти

    if (hOldbm) // Если не было ошибок, продолжаем работу
    {
        GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm); // Определяем размеры изображения

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(0, 0, 0));//все пиксели черного цвета будут интепретированы как прозрачные
        }
        else
        {
            StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // Рисуем изображение bitmap
        }

        SelectObject(hMemDC, hOldbm);// Восстанавливаем контекст памяти
    }

    DeleteDC(hMemDC); // Удаляем контекст памяти
}

void ShowRacketAndBall()
{
    ShowBitmap(window.context, 0, 0, window.width, window.height, hBack);//задний фон
    ShowBitmap(window.context, hero.x, hero.y, 2 * hero.rad, 2 * hero.rad, hero.hBitmap);
    ShowBitmap(window.context, oven.x, oven.y, oven.width, oven.height, oven.hBitmap);
    ShowBitmap(window.context, stove.x, stove.y, stove.width, stove.height, stove.hBitmap);
    ShowBitmap(window.context, fridge.x, fridge.y, fridge.width, fridge.height, fridge.hBitmap);
    ShowBitmap(window.context, table.x, table.y, table.width, table.height, table.hBitmap);
    ShowBitmap(window.context, statue1.x, statue1.y, statue1.width, statue1.height, statue1.hBitmap);
    ShowBitmap(window.context, statue2.x, statue2.y, statue2.width, statue2.height, statue2.hBitmap);
    ShowBitmap(window.context, statue3.x, statue3.y, statue3.width, statue3.height, statue3.hBitmap);
    ShowBitmap(window.context, enemy.x, enemy.y, enemy.width, enemy.height, enemy.hBitmap);
}

void InitWindow()
{
    SetProcessDPIAware();
    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);//из хэндла окна достаем хэндл контекста устройства для рисования
    window.width = r.right - r.left;//определяем размеры и сохраняем
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);//второй буфер
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));//привязываем окно к контексту
    GetClientRect(window.hWnd, &r);

}

int coll(sprite b) {
    int score = 0;
    if (hero.x + hero.width >= b.x && hero.y < b.y + b.height && hero.y + hero.height > b.y && hero.x + hero.width <= b.x + hero.speed) {
        score = 1;
    }
    else if (hero.x <= b.x + b.width && hero.y < b.y + b.height && hero.y + hero.height > b.y && hero.x >= b.x + b.width - hero.speed) {
        score = 2;
    }
    else if (hero.y <= b.y + b.height && hero.x < b.x + b.width && hero.x + hero.width > b.x && hero.y >= b.y + b.height - hero.speed) {
        score = 3;
    }
    else if (hero.y + hero.height >= b.y && hero.x < b.x + b.width && hero.x + hero.width > b.x && hero.y + hero.height <= b.y + hero.speed) {
        score = 4;
    }
    if (score == 1) {
        hero.x = b.x - 1 - hero.width;
    }
    else if (score == 2) {
        hero.x = b.x + b.width + 1;
    }
    else if (score == 3) {
        hero.y = b.y + b.height + 1;
    }
    else if (score == 4) {
        hero.y = b.y - 1 - hero.height;
    }
    return score;
}

int collCase() {
    if (coll(table) != 0 || coll(statue1) != 0 || coll(statue2) != 0 || coll(statue3) != 0 || coll(enemy) != 0) {
        return 1;
    }
    else {
        return 0;
    }
    /*if (hero.x + hero.width >= oven.x && hero.y < oven.y + oven.height && hero.y + hero.height > oven.y && hero.x + hero.width <= oven.x + hero.speed) {
        return 1;
    }
    if (hero.x <= oven.x + oven.width && hero.y < oven.y + oven.height && hero.y + hero.height > oven.y && hero.x >= oven.x + oven.width - hero.speed) {
        return 2;
    }
    if (hero.y <= oven.y + oven.height && hero.x < oven.x + oven.width && hero.x + hero.width > oven.x && hero.y >= oven.y + oven.height - hero.speed) {
        return 3;
    }
    if (hero.y + hero.height >= oven.y && hero.x < oven.x + oven.width && hero.x + hero.width > oven.x && hero.y + hero.height <= oven.y + hero.speed) {
        return 4;
    }*/
    /*if (hero.x + hero.width >= oven.x && hero.y < oven.y + oven.height && hero.y + hero.height > oven.y && hero.x + hero.width <= oven.x + hero.speed ||
    hero.x <= oven.x + oven.width && hero.y < oven.y + oven.height && hero.y + hero.height > oven.y && hero.x >= oven.x + oven.width - hero.speed ||
    hero.y <= oven.y + oven.height && hero.x < oven.x + oven.width && hero.x + hero.width > oven.x && hero.y >= oven.y + oven.height - hero.speed ||
    hero.y + hero.height >= oven.y && hero.x < oven.x + oven.width && hero.x + hero.width > oven.x && hero.y + hero.height <= oven.y + hero.speed) {
        return 1;
    }
    newXY = min(hero.x + hero.width - oven.x, oven.x + oven.width - hero.x, hero.y + hero.height - oven.y, oven.y + oven.height - hero.y);
    if (newXY == hero.x + hero.width - oven.x) {
    hero.x = hero.x + hero.width - oven.x;
    }
    else if (newXY == oven.x + oven.width - hero.x) {
    hero.x = oven.x + oven.width - hero.x
    }
    etc*/

}

void moveHero() {
    collCase();
    if (GetAsyncKeyState('D') && GetAsyncKeyState('W')) {
        hero.x += hero.speed / sqrt(2);
        hero.y -= hero.speed / sqrt(2);
    }
    else if (GetAsyncKeyState('W') && GetAsyncKeyState('A')) {
        hero.x -= hero.speed / sqrt(2);
        hero.y -= hero.speed / sqrt(2);
    }
    else if (GetAsyncKeyState('S') && GetAsyncKeyState('A')) {
        hero.x -= hero.speed / sqrt(2);
        hero.y += hero.speed / sqrt(2);
    }
    else if (GetAsyncKeyState('S') && GetAsyncKeyState('D')) {
        hero.x += hero.speed / sqrt(2);
        hero.y += hero.speed / sqrt(2);
    }
    else if (GetAsyncKeyState('D')) {
        hero.x += hero.speed;
    }
    else if (GetAsyncKeyState('A')) {
        hero.x -= hero.speed;
    }
    else if (GetAsyncKeyState('W')) {
        hero.y -= hero.speed;
    }
    else if (GetAsyncKeyState('S')) {
        hero.y += hero.speed;
    }

    if (hero.x < 0) {
        hero.x = 0;
    }
    if (hero.x + hero.width > window.width) {
        hero.x = window.width - hero.width;
    }
    if (hero.y < 0) {
        hero.y = 0;
    }
    if (hero.y + hero.height > window.height) {
        hero.y = window.height - hero.height;
    }

}

void moveEnemy() {
    if (abs(enemy.x - hero.x) > 350 && abs(enemy.y - hero.y) > 350) {
        if (enemy.x <= window.width / 4) {
            enemy.y -= enemy.speed;
        }
        if (enemy.y <= 200) {
            enemy.x += enemy.speed;
        }
        if (enemy.x >= window.width / 4 * 3) {
            enemy.y += enemy.speed;
        }
        if (enemy.y >= window.height - 450) {
            enemy.x -= enemy.speed;
        }
    }
    else {
        if (enemy.x - hero.x > 180) {
            enemy.x -= enemy.speed;
        }
        else if (hero.x - enemy.x > 180) {
            enemy.x += enemy.speed;
        }
        else if (enemy.y - hero.y > 180) {
            enemy.y -= enemy.speed;
        }
        else if (hero.y - enemy.y > 180) {
            enemy.y += enemy.speed;
        }
    }
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR  lpCmdLine, int  nCmdShow)
{

    InitWindow();//здесь инициализируем все что нужно для рисования в окне
    InitGame();//здесь инициализируем переменные игры

    ShowCursor(NULL);

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        moveHero();
        moveEnemy();
        ShowRacketAndBall();//рисуем фон, ракетку и шарик
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
    }

}
