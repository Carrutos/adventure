//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include "cmath"

// секция данных игры  
typedef struct {
    float x, y, width, height, rad, dx, dy, speed;
    HBITMAP hBitmap;//хэндл к спрайту шарика 
} sprite;

sprite racket;//ракетка игрока
sprite enemy;//ракетка противника
sprite ball;//шарик
sprite hero;

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
    ball.hBitmap = (HBITMAP)LoadImageA(NULL, "dig10k_penguin.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    racket.hBitmap = (HBITMAP)LoadImageA(NULL, "racket.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    enemy.hBitmap = (HBITMAP)LoadImageA(NULL, "Mandrill.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hero.hBitmap = (HBITMAP)LoadImageA(NULL, "ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBack = (HBITMAP)LoadImageA(NULL, "back.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //------------------------------------------------------

    racket.width = 300;
    racket.height = 300;
    racket.speed = 30;//скорость перемещения ракетки
    racket.x = window.width / 2.;//ракетка посередине окна
    racket.y = window.height - racket.height;//чуть выше низа экрана - на высоту ракетки

    enemy.x = window.width / 2;//х координату оппонета ставим в ту же точку что и игрока
    enemy.y = 0;
    enemy.height = 300;
    enemy.width = 300;


    ball.dy = (rand() % 65 + 35) / 100.;//формируем вектор полета шарика
    ball.dx = -(1 - ball.dy);//формируем вектор полета шарика
    ball.speed = 11;
    ball.rad = 70;
    ball.x = racket.x;//x координата шарика - на середине ракетки
    ball.y = racket.y - ball.rad;//шарик лежит сверху ракетки
    hero.x = window.width / 4;
    hero.y = window.height / 3 + 20;
    hero.height = 150;
    hero.width = 150;
    hero.speed = 10;
    hero.rad = 75;

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
    ShowBitmap(window.context, racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);// ракетка игрока

    

    ShowBitmap(window.context, enemy.x, enemy.y, enemy.width, enemy.height, enemy.hBitmap);//ракетка оппонента
    ShowBitmap(window.context, ball.x - ball.rad, ball.y - ball.rad, 2 * ball.rad, 2 * ball.rad, ball.hBitmap, true);// шарик
    ShowBitmap(window.context, hero.x, hero.y, 2 * hero.rad, 2 * hero.rad, hero.hBitmap);
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

int coll(float ax, float awidth, float ay, float aheight, float bx, float bwidth, float by, float bheight) {

    if (ax + awidth > bx) {
        if (ay + aheight > by && ay < by + bheight && ax < bx) {
            ax = bx - awidth;
            
        }
    }
    if (ay + aheight > by) {
        if (ax + awidth > bx && ax < bx + bwidth && ay < by) {
            ay = by - aheight;
           
        }
    }
    if (ay < by + bheight) {
        if (ax + awidth > bx && ax < bx + bwidth && ay + aheight > by + bheight) {
            ay = by + bheight;
           
        }
    }
    if (ax < bx + bwidth) {
        if (ay + aheight > by && ay < by + bheight && ax + awidth > bx + bwidth) {
            ax = bx + bwidth;
            
        }
    }
    return ax, ay;
}

void moveHero() {
    

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
        if (hero.x + hero.width == enemy.x && hero.y < enemy.y + enemy.height && hero.y + hero.height > enemy.y && hero.x < enemy.x) {
            
        }
        else {
            hero.x += hero.speed;
        }
    }
    else if (GetAsyncKeyState('A')) {
        if (hero.x == enemy.x + enemy.width && hero.y < enemy.y + enemy.height && hero.y + hero.height > enemy.y && hero.x + hero.width > enemy.x + enemy.width) {

        }
        else {
            hero.x -= hero.speed;
        }
    }
    else if (GetAsyncKeyState('W') ) {
        if (hero.y == enemy.y + enemy.height && hero.x < enemy.x + enemy.width && hero.x + hero.width > enemy.x && hero.y + hero.height > enemy.y + enemy.height) {

        }
        else {
            hero.y -= hero.speed;
        }
    }
    else if (GetAsyncKeyState('S')) {
        if (hero.y + hero.height == enemy.y && hero.x < enemy.x + enemy.width && hero.x + hero.width > enemy.x && hero.y < enemy.y) {

        }
        else {
            hero.y += hero.speed;
        }
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

    //hero.x, hero.y = coll(hero.x, hero.width, hero.y, hero.height, enemy.x, enemy.width, enemy.y, enemy.height);
    
}

int APIENTRY wWinMain( HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR  lpCmdLine, int  nCmdShow)
{
    
    InitWindow();//здесь инициализируем все что нужно для рисования в окне
    InitGame();//здесь инициализируем переменные игры

    ShowCursor(NULL);
    
    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        moveHero();
        ShowRacketAndBall();//рисуем фон, ракетку и шарик
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
    }

}
